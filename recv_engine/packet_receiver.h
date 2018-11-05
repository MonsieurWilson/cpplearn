
#ifndef _XDF_PACKET_RECEIVER_H
#define _XDF_PACKET_RECEIVER_H

#include <chrono>
#include <boost/asio/steady_timer.hpp>

#include "packet_define.h"

class PacketReceiver
{
private:
    void*                                 context;
    void*                                 receiver;

    PacketGroup*                          packet_group;
    PacketHead                            packet_header;

    boost::asio::steady_timer             timer_;
    std::chrono::steady_clock::time_point last_flush_time;
    bool                                  m_do_packet_group_flush;

private:
    PacketReceiver();

public:
    ~PacketReceiver();

    static PacketReceiver& instance(void)
    {
        static PacketReceiver s_packet_receiver;
        return s_packet_receiver;
    }
    
    void on_timeout(void);
    void packet_group_flush_to_mem(void);
    void add_to_packet_group(const PacketHead& ph, uint32_t packet_len);
    void recv_packet(void);

};

#endif //_XDF_PACKET_RECEIVER_H
