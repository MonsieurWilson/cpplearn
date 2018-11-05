
#include <stdint.h>
#include <sys/time.h>
#include <parquet/api/schema.h>

#include "log.h"
#include "misc.h"
#include "mem_parquet_writer.h"

MemParquetWriter::MemParquetWriter() 
    : ring_pool(new RingPool),
      sink(nullptr),
      file_writer(nullptr),
      total_received_length(0),
      total_compressed_size(0),
      total_received_size(0),
      already_flushed_size(0),
      file_start_time(0),
      file_end_time(0),
      timer_(GetConfigs().io, std::chrono::seconds(GetGlobalVars()->m_timer_span)),
      last_flush_time(std::chrono::steady_clock::now()),
      m_do_parquet_file_flush(false)
{
    if (GetGlobalVars()->m_enable_spark) {
        auto time_node    = PrimitiveNode::Make("time", Repetition::REQUIRED, Type::INT64, LogicalType::UINT_64);
        auto sip_node     = PrimitiveNode::Make("sip", Repetition::REQUIRED, Type::INT32, LogicalType::UINT_32);
        auto dip_node     = PrimitiveNode::Make("dip", Repetition::REQUIRED, Type::INT32, LogicalType::UINT_32);
        auto sport_node   = PrimitiveNode::Make("sport", Repetition::REQUIRED, Type::INT32, LogicalType::UINT_16);
        auto dport_node   = PrimitiveNode::Make("dport", Repetition::REQUIRED, Type::INT32, LogicalType::UINT_16);
        auto dsize_node   = PrimitiveNode::Make("dsize", Repetition::REQUIRED, Type::INT32, LogicalType::UINT_16);
        auto proto_node   = PrimitiveNode::Make("proto", Repetition::REQUIRED, Type::INT32, LogicalType::UINT_16);
        auto payload_node = PrimitiveNode::Make("payload", Repetition::REQUIRED, Type::BYTE_ARRAY, LogicalType::NONE);
        gnode = std::static_pointer_cast<GroupNode>(GroupNode::Make("schema", Repetition::REQUIRED,
                        std::vector<NodePtr>({time_node, sip_node, dip_node, sport_node, dport_node,
                                              dsize_node, proto_node, payload_node})));
    } else {
        auto time_node    = PrimitiveNode::Make("time", Repetition::REQUIRED, Type::INT64, LogicalType::UINT_64);
        auto sip_node     = PrimitiveNode::Make("sip", Repetition::REQUIRED, Type::INT32, LogicalType::UINT_32);
        auto dip_node     = PrimitiveNode::Make("dip", Repetition::REQUIRED, Type::INT32, LogicalType::UINT_32);
        auto sport_node   = PrimitiveNode::Make("sport", Repetition::REQUIRED, Type::INT32, LogicalType::UINT_16);
        auto dport_node   = PrimitiveNode::Make("dport", Repetition::REQUIRED, Type::INT32, LogicalType::UINT_16);
        auto dsize_node   = PrimitiveNode::Make("dsize", Repetition::REQUIRED, Type::INT32, LogicalType::UINT_16);
        auto proto_node   = PrimitiveNode::Make("proto", Repetition::REQUIRED, Type::INT32, LogicalType::UINT_16);
        auto payload_node = PrimitiveNode::Make("payload", Repetition::REQUIRED, Type::BYTE_ARRAY, LogicalType::NONE);
        gnode = std::static_pointer_cast<GroupNode>(GroupNode::Make("schema", Repetition::REQUIRED,
                        std::vector<NodePtr>({time_node, sip_node, dip_node, sport_node, dport_node,
                                              dsize_node, proto_node, payload_node})));
    }

    if (GetGlobalVars()->m_enable_compression) {
        std::shared_ptr<schema::ColumnPath> compress_column = 
            std::make_shared<schema::ColumnPath>(std::vector<std::string>{"payload"});
        std::shared_ptr<schema::ColumnPath> uncompressed_column = 
            std::make_shared<schema::ColumnPath>(std::vector<std::string>{"time"});
        writer_properties = WriterProperties::Builder().disable_statistics()\
                            ->compression(compress_column, Compression::SNAPPY)\
                            ->compression(uncompressed_column, Compression::UNCOMPRESSED)\
                            ->build();
    } else {
        writer_properties = WriterProperties::Builder().disable_statistics()->build();
    }

    timer_.expires_from_now(std::chrono::seconds(GetGlobalVars()->m_timer_span));
    timer_.async_wait(std::bind(&MemParquetWriter::on_timeout, this));
}

MemParquetWriter::~MemParquetWriter() {
}

void MemParquetWriter::on_timeout(void)
{
    timer_.expires_from_now(std::chrono::seconds(GetGlobalVars()->m_timer_span));
    timer_.async_wait(std::bind(&MemParquetWriter::on_timeout, this));

    std::chrono::seconds elapsed = 
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - last_flush_time);
    if( elapsed.count() >= GetGlobalVars()->m_flush_parquet_file_interval && file_writer != nullptr) {
        m_do_parquet_file_flush = true;
    }
}

void MemParquetWriter::write_to_parquet_mem(PacketGroup* packet_group)
{
    if( packet_group->add_rows == 0 )
        return;
    struct timeval  tv, last_tv;
    gettimeofday(&last_tv, 0);
    int64_t sink_head = sink->Tell();
    int64_t add_rows = packet_group->add_rows;
    auto row_group_writer = file_writer->AppendRowGroup(add_rows);
    auto time_writer    = static_cast<Int64Writer*>(row_group_writer->NextColumn());
    time_writer->WriteBatch(add_rows, nullptr, nullptr, packet_group->time_values);
    auto sip_writer     = static_cast<Int32Writer*>(row_group_writer->NextColumn());
    sip_writer->WriteBatch(add_rows, nullptr, nullptr, packet_group->sip_values);
    auto dip_writer     = static_cast<Int32Writer*>(row_group_writer->NextColumn());
    dip_writer->WriteBatch(add_rows, nullptr, nullptr, packet_group->dip_values);
    auto sport_writer   = static_cast<Int32Writer*>(row_group_writer->NextColumn());
    sport_writer->WriteBatch(add_rows, nullptr, nullptr, packet_group->sport_values);
    auto dport_writer   = static_cast<Int32Writer*>(row_group_writer->NextColumn());
    dport_writer->WriteBatch(add_rows, nullptr, nullptr, packet_group->dport_values);
    auto dsize_writer   = static_cast<Int32Writer*>(row_group_writer->NextColumn());
    dsize_writer->WriteBatch(add_rows, nullptr, nullptr, packet_group->dsize_values);
    auto proto_writer   = static_cast<Int32Writer*>(row_group_writer->NextColumn());
    proto_writer->WriteBatch(add_rows, nullptr, nullptr, packet_group->proto_values);
    auto payload_writer = static_cast<ByteArrayWriter*>(row_group_writer->NextColumn());
    payload_writer->WriteBatch(add_rows, nullptr, nullptr, packet_group->payload_values);
    row_group_writer->Close();
    total_received_length += add_rows;
    total_received_size += packet_group->data_size;
    total_compressed_size += sink->Tell() - sink_head;
    gettimeofday(&tv, 0);
    double tm = (tv.tv_sec*1000000 + tv.tv_usec - last_tv.tv_sec*1000000 - last_tv.tv_usec) / 1000000.0;
    double bps = (packet_group->data_size)/(tm * 1000000);
    LOG_INFO("[parquet-{}] received_packets:{}, received_size:{}, compressed_size:{}, "
             "chunk_size:{}, time:{}, bps:{}MB/s",
             id,
             total_received_length, total_received_size, total_compressed_size, 
             packet_group->data_size, tm, bps);
    while (!GetConfigs().put_packet_group_resource(packet_group)) {
        LOG_DEBUG("[parquet-{}] wait put_packet_group_resource", id);
        GetConfigs().show_all_ring_stat();
        xdf::mrmw_ring_wait_event(GetConfigs().packet_group_resource_ring, 3);
    }
    xdf::mrmw_ring_notify_all(GetConfigs().packet_group_resource_ring);
}

void MemParquetWriter::parquet_mem_flush_to_dfs(int64_t flag)
{
    ParquetMemChunk *parquet_chunk = new ParquetMemChunk;
    parquet_chunk->data = sink->Data() + already_flushed_size;
    parquet_chunk->flag = flag;
    parquet_chunk->start_time = file_start_time;
    parquet_chunk->end_time = file_end_time;
    if( flag ) {
        file_writer->Close();
        file_writer = nullptr;
        last_flush_time = std::chrono::steady_clock::now();
        m_do_parquet_file_flush = false;
        parquet_chunk->length = sink->Tell() - already_flushed_size;
        parquet_chunk->buffer = sink->GetBuffer();
    } else {
        parquet_chunk->length = PARQUET_FILE_CHUNK_SIZE;
    }
    while (!GetConfigs().write_parquet_mem_work(parquet_chunk, id)) {
        LOG_DEBUG("[parquet-{}] wait write_parquet_mem_work", id);
        GetConfigs().show_all_ring_stat();
        xdf::rw_ring_wait_event(GetConfigs().parquet_mem_work_ring[id], 3);
    }
    xdf::rw_ring_notify_one(GetConfigs().parquet_mem_work_ring[id]);
    already_flushed_size += parquet_chunk->length;
}

void MemParquetWriter::write_to_mem(void)
{
    PacketGroup* packet_group;
    int64_t remain_length;
    time_t start_time, end_time;
    while( nullptr == (packet_group = GetConfigs().read_packet_group_work(id)) ) {
        LOG_DEBUG("[parquet-{}] wait read_packet_group_work", id);
        GetConfigs().show_all_ring_stat();
        if (file_writer && m_do_parquet_file_flush) {
            goto flush;
        }
        xdf::rw_ring_wait_event(GetConfigs().packet_group_work_ring[id], 3);
    }

    start_time = packet_group->start_time;
    end_time   = packet_group->end_time;

    if (GetGlobalVars()->m_enable_second_level_dir && file_writer &&
        xdf::is_different_day(file_start_time, start_time)) {
        parquet_mem_flush_to_dfs(1);
    }

    if( nullptr == file_writer ) {
        sink.reset(new RingOutputStream(ring_pool.get(), (int64_t)MAX_PARQUET_FILE_SIZE));
        file_writer = ParquetFileWriter::Open(sink, gnode, writer_properties);
        already_flushed_size = 0;
        file_start_time = start_time;
        file_end_time   = end_time;
    } else {
        file_start_time = std::min(start_time, file_start_time);
        file_end_time   = std::max(end_time, file_end_time);
    }

    write_to_parquet_mem(packet_group);
    remain_length = sink->Tell() - already_flushed_size;
    if( sink->Tell() >= (int64_t)PARQUET_FILE_FLUSH_SIZE) {
        m_do_parquet_file_flush = true;
    }

flush:
    if( m_do_parquet_file_flush ) {
        parquet_mem_flush_to_dfs(1);
    } 
    else if( remain_length >= (int64_t)PARQUET_FILE_CHUNK_SIZE ) {
        parquet_mem_flush_to_dfs(0);
    }
}

