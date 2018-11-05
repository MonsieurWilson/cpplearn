
#include <cstdint>
#include <sys/time.h>
#include <cassert>
#include <zmq.h>

#include "packet_receiver.h"
#include "configs.h"
#include "log.h"
#include "misc.h"

static size_t recv_total_count = 0;
static size_t recv_total_size = 0;
static struct timeval tv2, last_tv;
static const size_t PACKET_HEADER_LEN = sizeof(PacketHead);


PacketReceiver::PacketReceiver() 
    : packet_group(nullptr), timer_(GetConfigs().io, std::chrono::seconds(GetGlobalVars()->m_timer_span))
{
    int timeout = 1000;
    int queue_length = 5000;
    int ret;
    context = zmq_ctx_new();

    if( GetGlobalVars()->m_zmq_mode == "PULL" ) {
        receiver = zmq_socket(context, ZMQ_PULL);
    } else {
        receiver = zmq_socket(context, ZMQ_SUB);
        zmq_setsockopt(receiver, ZMQ_SUBSCRIBE, 0, 0);
    }
    zmq_setsockopt(receiver, ZMQ_RCVHWM, &queue_length, sizeof(int));
    zmq_setsockopt(receiver, ZMQ_RCVTIMEO, &timeout, sizeof(int));
    for (int i = 0; i < GetGlobalVars()->m_zmq_port_num; ++i) {
        ret = zmq_connect(receiver, GetGlobalVars()->m_zmq_url[i].c_str());
        if( -1 == ret ) {
            LOG_CRITICAL("zmq connect {} error, {}", GetGlobalVars()->m_zmq_url[i].c_str(), zmq_strerror(errno));
            exit(-1);
        }
    }

    timer_.expires_from_now(std::chrono::seconds(GetGlobalVars()->m_timer_span));
    timer_.async_wait(std::bind(&PacketReceiver::on_timeout, this));
    last_flush_time = std::chrono::steady_clock::now();
    m_do_packet_group_flush = false;
    gettimeofday(&last_tv, 0);
}

PacketReceiver::~PacketReceiver()
{
}
    
void PacketReceiver::on_timeout(void)
{
    timer_.expires_from_now(std::chrono::seconds(GetGlobalVars()->m_timer_span));
    timer_.async_wait(std::bind(&PacketReceiver::on_timeout, this));

    std::chrono::seconds elapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - last_flush_time);
    if( elapsed.count() >= GetGlobalVars()->m_flush_packet_group_interval ) {
        m_do_packet_group_flush = true;
    }
}

void PacketReceiver::packet_group_flush_to_mem(void)
{
    last_flush_time = std::chrono::steady_clock::now();
    if( packet_group->add_rows != 0 ) {
        while (!GetConfigs().write_packet_group_work(packet_group));
        packet_group = nullptr;
        m_do_packet_group_flush = false;
    }
}

void PacketReceiver::add_to_packet_group(const PacketHead& ph, uint32_t packet_len)
{
    int64_t add_rows = packet_group->add_rows;
    packet_group->time_values[add_rows]  = ph.time;
    packet_group->sip_values[add_rows]   = ph.sip;
    packet_group->dip_values[add_rows]   = ph.dip;
    packet_group->sport_values[add_rows] = ph.sp;
    packet_group->dport_values[add_rows] = ph.dp;
    packet_group->dsize_values[add_rows] = ph.dsize;
    packet_group->proto_values[add_rows] = ph.ip_proto;
    packet_group->payload_values[add_rows].len = packet_len;
    packet_group->data_size += PACKET_HEADER_LEN + packet_len;
    packet_group->add_rows = ++add_rows;
    /* discard usec field */
    int64_t ts = ph.time >> 32;
    packet_group->start_time = std::min(packet_group->start_time, ts);
    packet_group->end_time = std::max(packet_group->end_time, ts);
}

void PacketReceiver::recv_packet(void)
{
    int ret, rcvmore;
    size_t sz;

    ret = zmq_recv(receiver, &packet_header, PACKET_HEADER_LEN, 0);

    if (GetGlobalVars()->m_enable_second_level_dir && packet_group &&
        xdf::is_different_day(packet_group->start_time, packet_header.time)) {
        packet_group_flush_to_mem();
    }

    if( nullptr == packet_group ) {
        while( nullptr == (packet_group = GetConfigs().get_packet_group_resource()) ) {
            LOG_DEBUG("[receiver] wait get_packet_group_resource");
            GetConfigs().show_all_ring_stat();
            xdf::mrmw_ring_wait_event(GetConfigs().packet_group_resource_ring, 3);
        }
        packet_group->add_rows   = 0;
        packet_group->data_size  = 0;
        packet_group->start_time = LLONG_MAX;
        packet_group->end_time   = LLONG_MIN;
    }

    if( ret == (int)PACKET_HEADER_LEN ) {
        sz = sizeof(rcvmore);
        ret = zmq_getsockopt(receiver, ZMQ_RCVMORE, &rcvmore, &sz);
        ret = zmq_recv(receiver, const_cast<uint8_t*>(packet_group->payload_values[packet_group->add_rows].ptr),
                       MAX_PACKET_LENGTH, 0);
        if( ret != -1 ) {
            add_to_packet_group(packet_header, (uint32_t)ret);
            recv_total_size += ret + PACKET_HEADER_LEN;
            if( (++recv_total_count % 100000) == 0 ) {
                gettimeofday(&tv2, 0);
                double tm = (tv2.tv_sec*1000000 + tv2.tv_usec - last_tv.tv_sec*1000000 - last_tv.tv_usec) / 1000000.0;
                double bps = 8 * recv_total_size / (tm * 1000000);
                double pps = 100000 / tm;
                LOG_INFO("[zmq] throughput: {}Mbps, {}msg/s", bps, pps);
                last_tv = tv2;
                recv_total_size = 0;
            }
            if( packet_group->add_rows == MAX_PACKET_COUNT ) {
                m_do_packet_group_flush = true;
            }
        }
    }
    if( m_do_packet_group_flush ) {
        packet_group_flush_to_mem();
    }
}
