#ifndef _DISK_PCAP_WRITER_H
#define _DISK_PCAP_WRITER_H

#include <pcap/pcap.h>

#include "task_info.h"


class DiskPcapWriter 
{
    public:
        /*
         * Name:
         *  DiskPcapWriter::instance
         *
         * Description:
         *  This function is called when caller wanna a DiskPcapWriter
         *  instance.
         *
         * Parameters:
         *  v - [IN] a `TaskInfo` structure by reference.
         *
         * Returns:
         *  DiskPcapWriter instance.
         *
         * NOTE:
         *  None.
         */
        static DiskPcapWriter &instance(TaskInfo &v) {
            static DiskPcapWriter s_disk_pcap_writer(v);
            return s_disk_pcap_writer;
        }

        /*
         * Name:
         *  DiskPcapWriter::write_pcap
         *
         * Description:
         *  This function is used to write payloads stored in memory
         *  into disk pcap file.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  None.
         * 
         * NOTE:
         *  None.
         */
        void write_pcap();

        ~DiskPcapWriter();

    private:

        DiskPcapWriter(TaskInfo &v);

        struct pcap_pkthdr            ph;
        pcap *                        handler;
        pcap_dumper_t *               p_dumper;
        TaskInfo &                    task_info;
};

#endif
