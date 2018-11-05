
#ifndef _NSF_PACKET_H_
#define _NSF_PACKET_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>
#include <stdint.h>
#ifdef __SSSE3__
#include <tmmintrin.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


typedef struct pcap_pkthdr pcaphdr;

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

/////////////////////////////////////////////////////////////////////////////////
/* ether */

#define ETHERNET_HEADER_LEN           14
#define ETHERNET_MAX_LEN_ENCAP          1518    /* 802.3 (+LLC) or ether II ? */ 
#define ETHERNET_TYPE_IP              0x0800

typedef struct EthernetHdr_ {
    uint8_t eth_dst[6];
    uint8_t eth_src[6];
    uint16_t eth_type;
} EthernetHdr;

/////////////////////////////////////////////////////////////////////////////////
/* ipv4 */

#define IPV4_HEADER_LEN           20    /**< Header length */
#define    IPV4_MAXPACKET_LEN        65535 /**< Maximum packet size */

typedef struct IPV4Hdr_
{
    uint8_t ip_verhl;     /**< version & header length */
    uint8_t ip_tos;       /**< type of service */
    uint16_t ip_len;      /**< length */
    uint16_t ip_id;       /**< id */
    uint16_t ip_off;      /**< frag offset */
    uint8_t ip_ttl;       /**< time to live */
    uint8_t ip_proto;     /**< protocol (tcp, udp, etc) */
    uint16_t ip_csum;     /**< checksum */
    union {
        struct {
            struct in_addr ip_src;/**< source address */
            struct in_addr ip_dst;/**< destination address */
        } ip4_un1;
        uint16_t ip_addrs[4];
    } ip4_hdrun1;
} IPV4Hdr;


#define s_ip_src                          ip4_hdrun1.ip4_un1.ip_src
#define s_ip_dst                          ip4_hdrun1.ip4_un1.ip_dst
#define s_ip_addrs                        ip4_hdrun1.ip_addrs


/////////////////////////////////////////////////////////////////////////////////
/* tcp */

#define TCP_HEADER_LEN                       20
#define TCP_OPTLENMAX                        40

typedef struct TCPHdr_
{
    uint16_t th_sport;  /**< source port */
    uint16_t th_dport;  /**< destination port */
    uint32_t th_seq;    /**< sequence number */
    uint32_t th_ack;    /**< acknowledgement number */
    uint8_t th_offx2;   /**< offset and reserved */
    uint8_t th_flags;   /**< pkt flags */
    uint16_t th_win;    /**< pkt window */
    uint16_t th_sum;    /**< checksum */
    uint16_t th_urp;    /**< urgent pointer */
} TCPHdr; 

/////////////////////////////////////////////////////////////////////////////////
/* udp */

#define UDP_HEADER_LEN         8

typedef struct UDPHdr_
{
    uint16_t uh_sport;  /* source port */
    uint16_t uh_dport;  /* destination port */
    uint16_t uh_len;    /* length */
    uint16_t uh_sum;    /* checksum */
} UDPHdr;

/////////////////////////////////////////////////////////////////////////////////
/* packet */

typedef struct _Packet {
    uint32_t            src;
    uint32_t            dst;

    uint16_t            sp;
    uint16_t            dp;

    uint16_t            ip_proto;
    uint16_t            dsize;
    uint8_t             *data;

    struct pcap_pkthdr  *pkth;           // BPF data 
    uint8_t             *pkt;            // base pointer to the raw packet data

    EthernetHdr         *ethh;
    IPV4Hdr             *ip4h;
    TCPHdr              *tcph;
    UDPHdr              *udph;

} Packet;

#ifdef __cplusplus
}
#endif

#endif //_NSF_PACKET_H_

