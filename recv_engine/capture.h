//capture.h

#ifndef _XDF_CAPTURE_H_
#define _XDF_CAPTURE_H_

#include <string>
#include "packet.h"


class PacketCapture
{
private:	
	pcap_t*      m_pd;            // the packet descriptor
	int          m_promisc_flag;
	int          m_snaplen;
	std::string  m_pcap_cmd;
	std::string  m_device;

public:
	~PacketCapture();

	static PacketCapture& instance(int snaplen=0)
	{
		static PacketCapture g_Capture(snaplen);
		return g_Capture;
	}

	void InitDevice(const std::string& device, const std::string& capfile);
	int	 RunCapture(void (PacketProcess)(u_char *, struct pcap_pkthdr *, u_char *), void* user);

private:
	PacketCapture();
	PacketCapture(int snaplen = 0);
};


extern PacketCapture& GetPacketCapture();

//////////////////////////////////////////////////////////////////////////////////

#endif  //_XDF_CAPTURE_H_
