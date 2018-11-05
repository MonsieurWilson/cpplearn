#include <atomic>
#include <thread>
#include <chrono>

#include <parquet/util/memory.h>
#include <parquet/api/reader.h>

#include "dfs_parquet_rearder.h"
#include "utils.h"
#include "log.h"
#include "config.h"

using namespace std::chrono;
using parquet::ParquetFileReader;
using parquet::FileMetaData;
using parquet::RowGroupReader;
using parquet::ByteArray;
using parquet::ColumnReader;
using parquet::Int64Reader;
using parquet::Int32Reader;
using parquet::ByteArrayReader;


DfsParquetReader::DfsParquetReader(TaskInfo &v) : task_info(v) {
    stime = strftime_to_timestamp(task_info.get_str_val_by_key("stime"));
    etime = strftime_to_timestamp(task_info.get_str_val_by_key("etime"));

    /* the header length of pcap file */
    match_packet_size = 24;

    continue_searching = true;
    file_list_ring_size = 0;
}

DfsParquetReader::~DfsParquetReader() {
}

void DfsParquetReader::do_search() {
    auto dfs_file_scanner =
        [this] () {
            steady_clock::time_point begin = steady_clock::now();
            scan_file(GetGlobalVars()->dfs_root);
            duration<double> elapsed = duration_cast<duration<double>>(steady_clock::now() - begin);
            LOG_INFO("- [DfsParquetReader::scan_file] [{}] Time elapsed: {}s.",
                     task_info.get_str_val_by_key("uuid"), elapsed.count());
            LOG_DEBUG("- [DfsParquetReader::scan_file] [{}] Matched file number: {}.", 
                      task_info.get_str_val_by_key("uuid"), file_list_ring_size);
        };

    auto dfs_file_reader = 
        [this] () { 
            bool finished = false;
            while (!finished) {
                DfsFileName *fn_ptr = static_cast<DfsFileName *>(file_list_ring_pop());
                if (1 == fn_ptr->flag) {
                    finished = true;
                }
                if (!finished && continue_searching) {
                    steady_clock::time_point begin = steady_clock::now();
                    continue_searching = read_file_from_dfs(fn_ptr->file_name.c_str());
                    duration<double> elapsed = duration_cast<duration<double>>(steady_clock::now() - begin);
                    LOG_DEBUG("- [DfsParquetReader::read_file_from_dfs] [{}] Time elapsed: {}s.",
                              task_info.get_str_val_by_key("uuid"),
                              elapsed.count());
                }
                delete fn_ptr;
            }
        };

    steady_clock::time_point begin = steady_clock::now();
    std::vector<std::thread> threads;
    threads.push_back(std::thread(dfs_file_scanner));
    for (uint16_t th_num = 0; 
         th_num != GetGlobalVars()->dfs_file_reader_thread_num;
         ++th_num) {
        threads.push_back(std::thread(dfs_file_reader));
    }
    for (auto &th : threads) {
        th.join();
    }
    duration<double> elapsed = duration_cast<duration<double>>(steady_clock::now() - begin);
    LOG_INFO("- [DfsParquetReader::read_files_from_dfs] [{}] Time elapsed: {}s.",
             task_info.get_str_val_by_key("uuid"), elapsed.count());

    payload_block_ring_push(new PayloadBlock(nullptr, 1));
    task_info.set_int_val_by_key("file_size", match_packet_size);
    LOG_INFO("- [DfsParquetReader::do_search] [{}] Matched packet size: {}Byte.",
             task_info.get_str_val_by_key("uuid"),
             task_info.get_int_val_by_key("file_size"));
}

bool DfsParquetReader::decode_parquet(std::shared_ptr<Buffer> &contents) {
    std::shared_ptr<arrow::Buffer> buffer = 
                     std::make_shared<arrow::Buffer>(contents->get_ptr(), contents->get_len());
    auto source = std::make_shared<arrow::io::BufferReader>(buffer);
    std::unique_ptr<ParquetFileReader> parquet_reader =
                             ParquetFileReader::Open(std::move(source));
    std::shared_ptr<FileMetaData> file_metadata = parquet_reader->metadata();
    int num_row_groups = file_metadata->num_row_groups();
    int num_columns = file_metadata->num_columns();
    if (8 != num_columns) {
        LOG_WARN("- [DfsParquetReader::decode_parquet] [{}] "
                 "Wrong column number, parquet file will be skipped.",
                 task_info.get_str_val_by_key("uuid"));
        parquet_reader->Close();
        return true;
    }

    /* The maximum value of batch size of Parquet CPP is 1024. */
    int batch_size = std::min(1024, static_cast<int>(GetGlobalVars()->dfs_file_reader_batch_size));
    int64_t values_read = 0;
    std::unique_ptr<int64_t[]>   time_val(new int64_t[batch_size]);
    std::unique_ptr<int32_t[]>   sip_val(new int32_t[batch_size]),
                                 dip_val(new int32_t[batch_size]),
                                 sport_val(new int32_t[batch_size]),
                                 dport_val(new int32_t[batch_size]),
                                 proto_val(new int32_t[batch_size]);
    std::unique_ptr<ByteArray[]> payload_val(new ByteArray[batch_size]);
    uint64_t limit  = task_info.get_int_val_by_key("limit");
    for (int group_num = 0; group_num < num_row_groups; ++group_num) {
        std::shared_ptr<RowGroupReader> row_group_reader    = parquet_reader->RowGroup(group_num);
        std::shared_ptr<ColumnReader> column_time_reader    = row_group_reader->Column(0);
        std::shared_ptr<ColumnReader> column_sip_reader     = row_group_reader->Column(1);
        std::shared_ptr<ColumnReader> column_dip_reader     = row_group_reader->Column(2);
        std::shared_ptr<ColumnReader> column_sport_reader   = row_group_reader->Column(3);
        std::shared_ptr<ColumnReader> column_dport_reader   = row_group_reader->Column(4);
        std::shared_ptr<ColumnReader> column_dsize_reader   = row_group_reader->Column(5);
        std::shared_ptr<ColumnReader> column_proto_reader   = row_group_reader->Column(6);
        std::shared_ptr<ColumnReader> column_payload_reader = row_group_reader->Column(7);

        Int64Reader* time_reader        = static_cast<Int64Reader*>(column_time_reader.get());
        Int32Reader* sip_reader         = static_cast<Int32Reader*>(column_sip_reader.get());
        Int32Reader* dip_reader         = static_cast<Int32Reader*>(column_dip_reader.get());
        Int32Reader* sport_reader       = static_cast<Int32Reader*>(column_sport_reader.get());
        Int32Reader* dport_reader       = static_cast<Int32Reader*>(column_dport_reader.get());
        Int32Reader* dsize_reader       = static_cast<Int32Reader*>(column_dsize_reader.get());
        Int32Reader* proto_reader       = static_cast<Int32Reader*>(column_proto_reader.get());
        ByteArrayReader* payload_reader = static_cast<ByteArrayReader*>(column_payload_reader.get());

        while (time_reader->HasNext()  && sip_reader->HasNext()   &&
               sport_reader->HasNext() && dip_reader->HasNext()   &&
               dport_reader->HasNext() && dsize_reader->HasNext() &&
               proto_reader->HasNext() && payload_reader->HasNext()) {
            time_reader   ->ReadBatch(batch_size, nullptr, nullptr, time_val.get(),    &values_read);
            sip_reader    ->ReadBatch(batch_size, nullptr, nullptr, sip_val.get(),     &values_read);
            dip_reader    ->ReadBatch(batch_size, nullptr, nullptr, dip_val.get(),     &values_read);
            sport_reader  ->ReadBatch(batch_size, nullptr, nullptr, sport_val.get(),   &values_read);
            dport_reader  ->ReadBatch(batch_size, nullptr, nullptr, dport_val.get(),   &values_read);
            proto_reader  ->ReadBatch(batch_size, nullptr, nullptr, proto_val.get(),   &values_read);
            payload_reader->ReadBatch(batch_size, nullptr, nullptr, payload_val.get(), &values_read);

            for (int64_t idx = 0; idx < values_read; ++idx) {
                /* discard usec field */
                time_t tv_sec = (uint64_t)time_val[idx] >> 32;
                if (0 != stime && 0 != etime && (tv_sec > etime || tv_sec < stime)) {
                    continue;
                }
                if (!is_match_session((uint32_t)sip_val[idx],   (uint32_t)dip_val[idx],
                                      (uint16_t)sport_val[idx], (uint16_t)dport_val[idx],
                                      (uint16_t)proto_val[idx])) {
                    continue;
                }
                match_packet_size += payload_val[idx].len + 16;
                if (0 != limit && match_packet_size > limit &&
                    0 == GetGlobalVars()->enable_total_matches) {
                    LOG_DEBUG("- [DfsParquetReader::decode_parquet] [{}] "
                              "Hit the limit, please set enable_total_matches "
                              "for further matches", task_info.get_str_val_by_key("uuid"));
                    return false;
                } else {
                    uint8_t *buf = (uint8_t *)malloc(payload_val[idx].len);
                    memcpy(buf, payload_val[idx].ptr, payload_val[idx].len);
                    PayloadBlock *pb = 
                        new PayloadBlock(new Payload(time_val[idx], payload_val[idx].len, buf));
                    payload_block_ring_push(pb);
                }
            }
        }
    }
    parquet_reader->Close();
    return true;

}

bool DfsParquetReader::is_match_session(uint32_t sip,   uint32_t dip,
                                        uint16_t sport, uint16_t dport,
                                        uint16_t proto) {
    uint32_t _sip    = task_info.get_int_val_by_key("sip");
    uint32_t _dip    = task_info.get_int_val_by_key("dip");
    uint16_t _sport  = task_info.get_int_val_by_key("sport");
    uint16_t _dport  = task_info.get_int_val_by_key("dport");
    uint16_t _proto  = task_info.get_int_val_by_key("proto");

    // protocol check
    if (0 != _proto && _proto != proto) {
        return false;
    }

    // bidirectional check
    if (0 != _sip && 0 != _dip) {
        return (_sip == sip && _dip == dip) || (_dip == sip && _sip == dip);
    }
    if (0 != _sport && 0 != _dport) {
        return ((_sport == sport && _dport == dport) || 
                (_dport == sport && _sport == dport));
    }

    // single directional check
    if (0 != _sip && _sip != sip) {
        return false;
    }
    if (0 != _dip && _dip != dip) {
        return false;
    }
    if (0 != _sport && _sport != sport) {
        return false;
    }
    if (0 != _dport && _dport != dport) {
        return false;
    }
    return true;
}

