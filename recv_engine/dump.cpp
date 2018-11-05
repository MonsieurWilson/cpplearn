#include <unistd.h>
#include <zmq.h>
#include <vector>
#include <string.h>
#include <string>
#include <iostream>
#include <getopt.h>

#include "packet.h"
#include "capture.h"
#include "misc.h"

static std::string g_CardName("");
static std::string g_PcapFile("");
static std::string g_mode("");
static std::string g_url("");

Packet g_Packet;
struct timeval  tv1, tv2, last_tv;
time_t g_cur_time = 0;
bool   g_Stop = false;
int    debug_print = 0;
int    print_packet = 0;
void*  context = NULL;
void*  sender = NULL;

static size_t total_count = 0;
static size_t total_bytes = 0;
static size_t last_total_bytes = 0;

static bool decode_tcp(Packet& p)
{
    if( (p.dsize < TCP_HEADER_LEN) ) {
        p.tcph = NULL;
        return false;
    }
    TCPHdr* tcph = (TCPHdr *)(p.data);   // lay TCP on top of the data
    p.tcph = tcph;
    uint16_t hlen = (((tcph)->th_offx2 & 0xf0) >> 2);   // multiply the payload offset value by 4
    if( (hlen < TCP_HEADER_LEN || hlen > p.dsize || hlen > TCP_HEADER_LEN + TCP_OPTLENMAX) ) {
        p.tcph = NULL;
        return false;
    }
            
    p.sp = ntohs(tcph->th_sport);
    p.dp = ntohs(tcph->th_dport);
    p.data += hlen;  // set the data pointer and size
    p.dsize -= hlen;

    return true;
}


static bool decode_udp(Packet& p)
{
    if( (p.dsize < UDP_HEADER_LEN) ) {
        p.udph = NULL;
        return false;
    }
    UDPHdr* udph = (UDPHdr *)(p.data);
    p.udph = udph;
    uint16_t udplen = ntohs(udph->uh_len); 
    if( ((udplen < UDP_HEADER_LEN) || (udplen > p.dsize)) ) {
        p.udph = NULL;
        return false;
    }

    p.sp = ntohs(udph->uh_sport);
    p.dp = ntohs(udph->uh_dport);
    p.data += UDP_HEADER_LEN;
    p.dsize = (uint16_t)(udplen - UDP_HEADER_LEN);    // length was validated up above

    return true;
}

static bool decode_ipv4(Packet& p)
{
    if( (p.dsize < IPV4_HEADER_LEN || ((p.data[0] & 0xf0) >> 4) != 4) ) {
        p.ip4h = NULL;
        return false;
    }

    p.ip4h = (IPV4Hdr *)(p.data);

    uint16_t hlen = ((p.ip4h->ip_verhl & 0x0f) << 2);
    uint16_t ip_len = ntohs(p.ip4h->ip_len);
    if( (hlen < IPV4_HEADER_LEN || hlen > ip_len || p.dsize < ip_len) ) {
        p.ip4h = NULL;
        return false;
    }

    p.data += hlen;
    p.dsize = ip_len - hlen;

    p.src = (uint32_t)p.ip4h->s_ip_src.s_addr;
    p.dst = (uint32_t)p.ip4h->s_ip_dst.s_addr;
    p.ip_proto = p.ip4h->ip_proto;
    switch( p.ip4h->ip_proto ) {
    case IPPROTO_TCP:
        decode_tcp(p);
        break;
    case IPPROTO_UDP:
        decode_udp(p);
        break;
    default:
        break;
    }

    return true;
}

static bool decode_ether(Packet& p)
{
    if( (p.pkth == NULL || p.pkt == NULL) ) {
        return false;
    }
    if( (p.pkth->caplen < ETHERNET_HEADER_LEN) ) {
        p.ethh = NULL;
        return false;
    }
    p.ethh = (EthernetHdr *)(p.pkt);

    p.data = p.pkt + ETHERNET_HEADER_LEN;    //pktÎªuint8_tÀàÐÍ
    p.dsize = (u_int16_t)(p.pkth->caplen - ETHERNET_HEADER_LEN);

    switch( ntohs(p.ethh->eth_type) ) {
    case ETHERNET_TYPE_IP:
        decode_ipv4(p);
        break;
    default:
        break;
    }

    return true;
}

typedef struct _PacketHead
{
    uint64_t time;
    uint32_t sip;
    uint32_t dip;
    uint16_t sp;
    uint16_t dp;
    uint16_t dsize;
    uint16_t ip_proto;
} PacketHead;

static void decode(Packet& p)
{
    //xdf::hex_print((const char*)p.pkt, p.pkth->caplen, std::cout);
    decode_ether(p);
    if( debug_print ) {
        printf("time:%lu, ip_proto:%u, %x:%u -> %x:%u, len:%u\n",
        g_cur_time, p.ip_proto, p.src, p.sp, p.dst, p.dp, p.pkth->caplen);
    }
    ++total_count;
    total_bytes += p.pkth->caplen + sizeof(PacketHead);
    if( (total_count % 100000) == 0 ) {
        gettimeofday(&tv2, 0);
        double tm = (tv2.tv_sec*1000000 + tv2.tv_usec - last_tv.tv_sec*1000000 - last_tv.tv_usec) / 1000000.0;
        double bps = (total_bytes - last_total_bytes)/(tm * 1000000);
        double total_tm = (tv2.tv_sec*1000000 + tv2.tv_usec - tv1.tv_sec*1000000 - tv1.tv_usec) / 1000000.0;
        double total_bps = total_bytes/(total_tm * 1000000);
        last_total_bytes = total_bytes;
        last_tv = tv2;
        printf("total_count:%lu, total bps:%fMB/s, time:%f, bps:%fMB/s\n",total_count,total_bps,tm,bps);
    }
    PacketHead ph = { (uint64_t)g_cur_time, p.src, p.dst, p.sp, p.dp, (uint16_t)p.pkth->caplen, p.ip_proto};
    int ret = zmq_send(sender, &ph, sizeof(ph), ZMQ_SNDMORE);
    if( debug_print) 
        printf("send head len: %d\n", ret);
    ret = zmq_send(sender, p.pkt, p.pkth->caplen, 0);
    if( debug_print) 
        printf("send packet len: %d\n", ret);
}

static void PacketProcess(u_char *user, struct pcap_pkthdr *pkthdr, u_char *pkt)
{
    memset(&g_Packet, 0, sizeof(Packet));
    g_Packet.pkth = pkthdr;
    g_Packet.pkt = pkt;
    g_Packet.ip4h = 0;
    g_Packet.udph = 0;
    g_Packet.sp = 0;
    g_Packet.dp = 0;
    g_cur_time = g_Packet.pkth->ts.tv_sec;
    if( (print_packet) ) {
        printf("get a pkt caplen:%u\n", g_Packet.pkth->caplen);
    }
    if( g_Packet.pkth->caplen > 1514 )
        g_Packet.pkth->caplen = 1514;
    decode(g_Packet);
}

static int RunMemSend()
{
    ++total_count;
    g_cur_time = time(NULL);
    PacketHead ph = { (uint64_t)g_cur_time, 0, 0, 0, 0, 1514, 1};
    int ret = zmq_send(sender, &ph, sizeof(ph), ZMQ_SNDMORE);
    uint8_t *payload = (uint8_t *) malloc(sizeof(uint8_t) * 1514);
    ret = zmq_send(sender, payload, 1514, 0);
    free(payload);
    total_bytes += 1514 + sizeof(PacketHead);
    if( (total_count % 1000000) == 0 ) {
        gettimeofday(&tv2, 0);
        double tm = (tv2.tv_sec*1000000 + tv2.tv_usec - last_tv.tv_sec*1000000 - last_tv.tv_usec) / 1000000.0;
        double bps = (total_bytes - last_total_bytes)/(tm * 1000000);
        double total_tm = (tv2.tv_sec*1000000 + tv2.tv_usec - tv1.tv_sec*1000000 - tv1.tv_usec) / 1000000.0;
        double total_bps = total_bytes/(total_tm * 1000000);
        last_total_bytes = total_bytes;
        last_tv = tv2;
        printf("total_count:%lu, total bps:%fMB/s, time:%f, bps:%fMB/s\n",total_count,total_bps,tm,bps);
    }
    return ret;
}

static void usage(void)
{
    printf("program [-i <dev> -r <pcap file> -p -d]\n"
        " -i <dev>: the device name to monitor\n"
        " -r <pcap file>: the pcap file to read\n"
        " -m mode: PUB or PUSH\n"
        " -u: zeromq url[ipc:///tmp/zmq_ipc_num or tcp://127.0.0.1:4500]\n"
        " -p: print packet info\n"
        " -d: print debug message\n");
}

static int parse_args(int argc, char *argv[])
{
    int option_index, opt;
    char **argvopt = argv;
    static struct option lgopts[] = { /* no long options */
        {NULL, 0, 0, 0 }
    };

    if( argc < 2 )
        return -1;
    while( (opt = getopt_long(argc, argvopt, "i:r:m:u:pdl", lgopts, &option_index)) != EOF ) {
        switch( opt ) {
        case 'i':
            if( optarg == NULL || *optarg == '\0' ) {
                printf("ERROR: Unknown option '%c'\n", opt);
                usage();
                return -1;
            } else {
                g_CardName = optarg;
            }
            break;
        case 'r':
            if( optarg == NULL || *optarg == '\0' ) {
                printf("ERROR: Unknown option '%c'\n", opt);
                usage();
                return -1;
            } else {
                g_PcapFile = optarg;
            }
            break;
        case 'm':
            if( optarg == NULL || *optarg == '\0' ) {
                printf("ERROR: Unknown option '%c'\n", opt);
                usage();
                return -1;
            } else {
                g_mode = optarg;
            }
            break;
        case 'u':
            if( optarg == NULL || *optarg == '\0' ) {
                printf("ERROR: Unknown option '%c'\n", opt);
                usage();
                return -1;
            } else {
                g_url = optarg;
            }
            break;
        case 'p':
            print_packet = 1;
            debug_print = 1;
            break;
        case 'd':
            debug_print = 1;
            break;
        default:
            printf("ERROR: Unknown option '%c'\n", opt);
            return -1;
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    if( parse_args(argc, argv) != 0 ) {
        usage();
        return -1;
    }
    context = zmq_ctx_new();

    int ret;
    int queue_length = 5000;
    if( g_mode == "PUB" ) {
        sender = zmq_socket(context, ZMQ_PUB);
    } else {
        sender = zmq_socket(context, ZMQ_PUSH);
    }
    zmq_setsockopt(sender, ZMQ_SNDHWM, &queue_length,sizeof(queue_length));
    ret = zmq_bind(sender, g_url.c_str());

    if( g_CardName != "" || g_PcapFile != "" ) {
        GetPacketCapture().InitDevice(g_CardName, g_PcapFile);
    }
    gettimeofday(&tv1, 0);
    last_tv = tv1;
    while( !g_Stop ) {
        if( g_CardName != "" || g_PcapFile != "" ) {
            ret = GetPacketCapture().RunCapture(PacketProcess, NULL);
        } else {
            ret = RunMemSend();
        }
        if( ret < 0 ) {
            gettimeofday(&tv2, 0);
            double tm = (tv2.tv_sec*1000000 + tv2.tv_usec - last_tv.tv_sec*1000000 - last_tv.tv_usec) / 1000000.0;
            double bps = (total_bytes - last_total_bytes)/(tm * 1000000);
            double total_tm = (tv2.tv_sec*1000000 + tv2.tv_usec - tv1.tv_sec*1000000 - tv1.tv_usec) / 1000000.0;
            double total_bps = total_bytes/(total_tm * 1000000);
            last_total_bytes = total_bytes;
            last_tv = tv2;
            printf("total_count:%lu, total_bytes:%lu, total time:%f, total bps:%fMB/s, time:%f, bps:%fMB/s\n",total_count,total_bytes,total_tm,total_bps,tm,bps);
            g_Stop = true;
            return 0;
        }
    }

    return 0;
}
