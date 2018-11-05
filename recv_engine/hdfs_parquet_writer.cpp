
#include <cstdint>
#include <sys/time.h>
#include <iostream>

#include "hdfs_parquet_writer.h"
#include "log.h"
#include "configs.h"
#include "misc.h"


HdfsParquetWriter::HdfsParquetWriter()
    : file_(nullptr),
      total_written(0)
{
    hdfs_connect();
}

HdfsParquetWriter::~HdfsParquetWriter()
{
    hdfsDisconnect(hdfs_);
}

void HdfsParquetWriter::generate_filename(time_t file_start_time, time_t file_end_time)
{
    struct tm *tm1 = localtime(&file_start_time);
    if (GetGlobalVars()->m_enable_second_level_dir) {
        snprintf(hdfs_path, 64, "/%s/%d%02d%02d", 
                 GetGlobalVars()->m_dfs_root.c_str(),
                 tm1->tm_year + 1900, tm1->tm_mon + 1, tm1->tm_mday);
    } else {
        snprintf(hdfs_path, 64, "/%s", GetGlobalVars()->m_dfs_root.c_str());
    }
    if (0 != hdfsExists(hdfs_, hdfs_path)) {
        hdfsCreateDirectory(hdfs_, hdfs_path);
    }
    snprintf(hdfs_name, 64, "%s/%lu-%lu-%lu.parquet", hdfs_path, file_start_time, file_end_time, id);
}

void HdfsParquetWriter::rename_file(time_t file_start_time, time_t file_end_time) 
{
    char new_name[64];
    snprintf(new_name, 64, "%s/%lu-%lu-%lu.parquet", hdfs_path, file_start_time, file_end_time, id);
    hdfsRename(hdfs_, hdfs_name, new_name);
}

void HdfsParquetWriter::write_to_hdfs(void)
{
    int64_t bytes_written;
    struct timeval tv, last_tv;
    double timeval, bps;
    ParquetMemChunk* parquet_chunk;
    while( (parquet_chunk = GetConfigs().read_parquet_mem_work(id)) ) {
        /* reconnect HDFS */
        if( hdfs_ == nullptr ) {
            LOG_WARN("[hdfs-{}] connect HDFS failed, retry", id);
            hdfs_connect();
        }

        if( file_ == nullptr ) {
            generate_filename(parquet_chunk->start_time, parquet_chunk->end_time);
            file_ = hdfsOpenFile(hdfs_, hdfs_name, O_WRONLY, 0, 1, PARQUET_FILE_CHUNK_SIZE);
            if( file_ == nullptr ) {
                LOG_ERROR("[hdfs-{}] hdfsOpenFile {} failed: {}", id, hdfs_name, hdfsGetLastError());
                /* We simply skip this parquet_chunk */
                goto clean;
            }
        }
        gettimeofday(&last_tv, 0);
        bytes_written = hdfsWrite(hdfs_, file_, parquet_chunk->data, parquet_chunk->length);
        total_written += bytes_written;
        if( bytes_written != parquet_chunk->length ) {
            LOG_ERROR("[hdfs-{}] hdfsWrite failed: {}", id, hdfsGetLastError());
        }
        gettimeofday(&tv, 0);
        timeval = (tv.tv_sec*1000000 + tv.tv_usec - last_tv.tv_sec*1000000 - last_tv.tv_usec) / 1000000.0;
        bps = bytes_written/(timeval * 1000000);
        LOG_INFO("[hdfs-{}] received_size:{}, chunk_size:{}, time:{}, bps:{}MB/s", id,
                 total_written, parquet_chunk->length, timeval, bps);
clean:
        if( 0 != parquet_chunk->flag ) {
            hdfsCloseFile(hdfs_, file_);
            file_ = nullptr;
            rename_file(parquet_chunk->start_time, parquet_chunk->end_time);
        }
        delete parquet_chunk;
    }
    LOG_DEBUG("[hdfs-{}] wait read_parquet_mem_work", id);
    GetConfigs().show_all_ring_stat();
    xdf::rw_ring_wait_event(GetConfigs().parquet_mem_work_ring[id], 3);
}

void HdfsParquetWriter::hdfs_connect(void)
{
    struct hdfsBuilder *builder = hdfsNewBuilder();
    hdfsBuilderSetNameNode(builder, GetGlobalVars()->m_dfs_host.c_str());
    hdfsBuilderSetNameNodePort(builder, GetGlobalVars()->m_dfs_port);
    hdfsBuilderConfSetStr(builder, "dfs.client.log.severity", GetGlobalVars()->m_dfs_loglevel.c_str());
    hdfsBuilderConfSetStr(builder, "dfs.domain.socket.path", "/var/lib/hadoop-hdfs/dn_socket");
    hdfsBuilderConfSetStr(builder, "output.default.chunksize", "8192");
    hdfs_ = hdfsBuilderConnect(builder);
    hdfsFreeBuilder(builder);
}
