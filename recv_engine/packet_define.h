//packet_define.h

#ifndef _XDF_PACKET_DEFINE_H
#define _XDF_PACKET_DEFINE_H

#include <stdint.h>
#include <parquet/types.h>
#include <parquet/util/memory.h>

using namespace parquet;

/* ----------- Important constants values ----------- */
// the maximum number of packets that a PacketGroup can cantain
const static size_t MAX_PACKET_COUNT = 100000;
// the maximum length of received packets
const static size_t MAX_PACKET_LENGTH = 1536;
// the maximum length of one PacketGroup: 2400020 + 100000*1536 = 79200016 = 148M
const static size_t MAX_PACKET_GROUP_SIZE = 0x0A000000; //160M
// the maximum length of a parquet file
const static size_t MAX_PARQUET_FILE_SIZE =   0x40000000; //1G
// the maximum length of a parquet chunk
const static size_t PARQUET_FILE_CHUNK_SIZE = 0x20000000; //512M
// the threshold length to invoke the memory parquet file flushing into disk
const static size_t PARQUET_FILE_FLUSH_SIZE = MAX_PARQUET_FILE_SIZE - MAX_PACKET_GROUP_SIZE; // 864M

// the number of threads of mem_parquet_writer
const static size_t PACKET_GROUP_WORK_RING_COUNT = 4;
// the number of threads of disk_parquet_writer
const static size_t PARQUET_MEM_WORK_RING_COUNT = PACKET_GROUP_WORK_RING_COUNT;
// the size of packet_group_work_ring
const static size_t WORK_RING_CACHE_COUNT = 2;
// the size of parquet_mem_work_ring
const static size_t PARQUET_FILE_CHUNK_COUNT = WORK_RING_CACHE_COUNT * (MAX_PARQUET_FILE_SIZE / PARQUET_FILE_CHUNK_SIZE);
// the size of packet_group_resource_ring
const static size_t PACKET_GROUP_COUNT = WORK_RING_CACHE_COUNT * PACKET_GROUP_WORK_RING_COUNT;
// the size of parquet_mem_resource_ring
const static size_t PARQUET_MEM_COUNT = WORK_RING_CACHE_COUNT * PARQUET_MEM_WORK_RING_COUNT;

typedef struct _PacketGroup
{
    int64_t    add_rows;
    int64_t    start_time;
    int64_t    end_time;
    size_t     data_size;
    int64_t    time_values[MAX_PACKET_COUNT];
    int32_t    sip_values[MAX_PACKET_COUNT];
    int32_t    dip_values[MAX_PACKET_COUNT];
    int32_t    sport_values[MAX_PACKET_COUNT];
    int32_t    dport_values[MAX_PACKET_COUNT];
    int32_t    dsize_values[MAX_PACKET_COUNT];
    int32_t    proto_values[MAX_PACKET_COUNT];
    ByteArray  payload_values[MAX_PACKET_COUNT];
} PacketGroup;

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

typedef struct _ParquetMemChunk
{
    const uint8_t*           data;
    int64_t                  length;
    int64_t                  flag;
    time_t                   start_time;
    time_t                   end_time;
    std::shared_ptr<Buffer>  buffer;
} ParquetMemChunk;

#endif //_XDF_PACKET_DEFINE_H
