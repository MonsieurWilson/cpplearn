//capture.cpp

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#ifndef WIN32
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <sys/mman.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
#endif

#include "capture.h"


PacketCapture & GetPacketCapture()
{
    static PacketCapture& g_Capture = PacketCapture::instance();
    return g_Capture;
}
// IRIX 6.2 hack!
#ifndef IRIX
static const u_int16_t SNAPLEN = 1514;
#else
static const u_int16_t SNAPLEN = 1500;
#endif

static const int PROMISC = 1;
static const int READ_TIMEOUT = 500;   //∫¡√Î

//////////////////////////////////////////////////////////////////////////////////

PacketCapture::PacketCapture(int snaplen)
    : m_pd(0), m_promisc_flag(PROMISC), m_snaplen(snaplen? snaplen:SNAPLEN), m_pcap_cmd("")
{
}

PacketCapture::~PacketCapture()
{
    if( m_pd != 0 ) {
        pcap_close(m_pd);
        m_pd = 0;
    }
}

int PacketCapture::RunCapture(void (PacketProcess)(u_char *user, struct pcap_pkthdr *pkthdr, u_char *pkt), void* user)
{
    int ret =  pcap_dispatch(m_pd, 10000, (pcap_handler)PacketProcess, (u_char*)user);
    if( 0 == ret && "" == m_device ) {
        return -1;
    }
    return ret;
}

void PacketCapture::InitDevice(const std::string& device, const std::string& capfile)
{
    m_device = device;
    bpf_u_int32 localnet, netmask;    // net addr holders
    struct bpf_program fcode;         // Finite state machine holder
    char errorbuf[PCAP_ERRBUF_SIZE];  // buffer to put error strings in
    bpf_u_int32 defaultnet = 0xFFFFFF00;
    printf("Initializing Network Interface...\n");

    char* intf = const_cast<char *>(device.c_str());
    if( "" != device ) {
        m_pd = pcap_open_live(intf, m_snaplen, m_promisc_flag, READ_TIMEOUT, errorbuf);
    } else {
        m_pd = pcap_open_offline(const_cast<char *>(capfile.c_str()), errorbuf);
    }

    if( m_pd == NULL ) {
        std::ostringstream errStream;
        errStream << errorbuf << ": " << intf;
        throw std::runtime_error(errStream.str());
    }

    if( pcap_lookupnet(intf, &localnet, &netmask, errorbuf) < 0 ) {
        netmask = htonl(defaultnet);
    }
 
    netmask = 0;
    if( pcap_compile(m_pd, &fcode, NULL, 1, netmask) < 0 ) {
        throw std::runtime_error("ERROR: packet capture FSM compilation failed!\n");
    }

    if( pcap_setfilter(m_pd, &fcode) < 0 ) {
        throw std::runtime_error("ERROR: packet capture setfilter error!\n");
    }

    if( pcap_datalink(m_pd) < 0 ) {
        throw std::runtime_error("ERROR: packet capture datalink grab error!\n");
    }
}

