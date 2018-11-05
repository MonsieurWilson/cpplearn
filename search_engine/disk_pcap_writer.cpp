#include <chrono>
#include <queue>
#include <vector>

#include "config.h"
#include "log.h"
#include "payload.h"
#include "disk_pcap_writer.h"

using namespace std::chrono;


DiskPcapWriter::DiskPcapWriter(TaskInfo &v) : task_info(v) {
    handler = pcap_open_dead(DLT_EN10MB, 0xFFFF);
    LOG_DEBUG("- [DiskPcapWriter::constructor] [{}] Pcap file name = {}.", 
              task_info.get_str_val_by_key("uuid"),
              task_info.get_str_val_by_key("file_name"));
    p_dumper = pcap_dump_open(handler, 
                              task_info.get_str_val_by_key("file_name"));
}

DiskPcapWriter::~DiskPcapWriter() {
}

void DiskPcapWriter::write_pcap() {
    std::priority_queue<PayloadBlock*, std::vector<PayloadBlock*>, compare> payload_blocks;
    PayloadBlock *pb_ptr;

    /* do heap sort according to timestamp of payloads */
    while (true) {
        pb_ptr = (PayloadBlock *) payload_block_ring_pop();
        
        if (pb_ptr->flag) {
            delete pb_ptr;
            break;
        }

        payload_blocks.push(pb_ptr);
    }
 
    steady_clock::time_point begin = steady_clock::now();
    /* write pcap file */
    while (!payload_blocks.empty()) {
        pb_ptr = payload_blocks.top();
        ph.ts.tv_sec  = pb_ptr->payload->get_time_stamp() >> 32;
        ph.ts.tv_usec = pb_ptr->payload->get_time_stamp() & ((1L << 32) - 1);
        ph.len = ph.caplen = pb_ptr->payload->get_payload_len();
        pcap_dump((u_char *)p_dumper, &ph, pb_ptr->payload->get_payload_ptr());
        payload_blocks.pop();
        delete pb_ptr;
    }

    /* 
     * The kernel won't flush the kernel ring buffer to disk
     * util the file descriptor is closed.
     */
    pcap_dump_close(p_dumper);
    duration<double> elapsed = duration_cast<duration<double>>(steady_clock::now() - begin);
    LOG_INFO("- [DiskPcapWriter::write_pcap] [{}] Time elapsed: {}s.", 
             task_info.get_str_val_by_key("uuid"), elapsed.count());
}

