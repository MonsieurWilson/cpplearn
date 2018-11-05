
#ifndef _XDF_MEM_PARQUET_WRITER_H
#define _XDF_MEM_PARQUET_WRITER_H

#include <stdint.h>
#include <chrono>
#include <boost/asio/steady_timer.hpp>
#include <parquet/statistics.h>
#include <parquet/api/writer.h>
#include <arrow/status.h>

#include "ring_output_stream.h"

using namespace parquet;
using schema::NodePtr;
using schema::GroupNode;
using schema::PrimitiveNode;


class MemParquetWriter
{
private:
    std::shared_ptr<GroupNode>              gnode;
    std::shared_ptr<WriterProperties>       writer_properties;
    std::shared_ptr<RingPool>               ring_pool;
    std::shared_ptr<RingOutputStream>       sink;
    std::unique_ptr<ParquetFileWriter>      file_writer;
    int64_t                                 total_received_length;
    int64_t                                 total_compressed_size;
    int64_t                                 total_received_size;
    int64_t                                 already_flushed_size;
    time_t                                  file_start_time;
    time_t                                  file_end_time;

    boost::asio::steady_timer               timer_;
    std::chrono::steady_clock::time_point   last_flush_time;
    bool                                    m_do_parquet_file_flush;
    size_t                                  id;

private:
    MemParquetWriter();

    void on_timeout(void);
    void write_to_parquet_mem(PacketGroup* packet_group);
    void parquet_mem_flush_to_dfs(int64_t flag);

public:
    ~MemParquetWriter();

    static MemParquetWriter& instance(size_t id)
    {
        static MemParquetWriter *s_mem_parquet_writer =
                    new MemParquetWriter[PACKET_GROUP_WORK_RING_COUNT];
        s_mem_parquet_writer[id].id = id;
        return s_mem_parquet_writer[id];
    }

    void write_to_mem(void);
};

#endif //_XDF_MEM_PARQUET_WRITER_H
