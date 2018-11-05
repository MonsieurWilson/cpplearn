
#include <sys/time.h>
#include <cerrno>
#include <cstdint>
#include <iostream>

#include "qfs_parquet_writer.h"
#include "log.h"
#include "configs.h"
#include "misc.h"


QfsParquetWriter::QfsParquetWriter()
    : m_qfs_client(nullptr),
      m_total_written(0)
{
    m_qfs_client = KFS::Connect(GetGlobalVars()->m_dfs_host.c_str(), GetGlobalVars()->m_dfs_port);
}

QfsParquetWriter::~QfsParquetWriter()
{
}

void QfsParquetWriter::generate_filename(time_t file_start_time, time_t file_end_time)
{
    struct tm *tm1 = localtime(&file_start_time);
    if (GetGlobalVars()->m_enable_second_level_dir) {
        snprintf(m_qfs_path, 64, "/%s/%d%02d%02d", 
                 GetGlobalVars()->m_dfs_root.c_str(),
                 tm1->tm_year + 1900, tm1->tm_mon + 1, tm1->tm_mday);
    } else {
        snprintf(m_qfs_path, 64, "/%s", GetGlobalVars()->m_dfs_root.c_str());
    }
    if (!m_qfs_client->Exists(m_qfs_path)) {
        m_qfs_client->Mkdirs(m_qfs_path);
    }
    snprintf(m_qfs_name, 64, "%s/%lu-%lu-%lu.parquet", m_qfs_path, file_start_time, file_end_time, id);
}

void QfsParquetWriter::rename_file(time_t file_start_time, time_t file_end_time) 
{
    char new_name[64];
    snprintf(new_name, 64, "%s/%lu-%lu-%lu.parquet", m_qfs_path, file_start_time, file_end_time, id);
    m_qfs_client->Rename(m_qfs_name, new_name);
}

void QfsParquetWriter::write_to_qfs(void)
{
    int64_t bytes_written;
    struct timeval tv, last_tv;
    double timeval, bps;
    ParquetMemChunk* parquet_chunk;
    while ((parquet_chunk = GetConfigs().read_parquet_mem_work(id))) {
        if (0 == m_qfs_fd) {
            generate_filename(parquet_chunk->start_time, parquet_chunk->end_time);
            if (0 > (m_qfs_fd = m_qfs_client->Create(m_qfs_name, 1))) {
                LOG_ERROR("[qfs-{}] Open {} failed: {}", id, m_qfs_name, KFS::ErrorCodeToStr(m_qfs_fd));
                m_qfs_fd = 0;
                /* We simply skip this parquet_chunk */
                goto clean;
            }
        }
        gettimeofday(&last_tv, 0);
        bytes_written = m_qfs_client->Write(m_qfs_fd, (const char *)parquet_chunk->data, parquet_chunk->length);
        if (bytes_written != parquet_chunk->length) {
            LOG_ERROR("[qfs-{}] qfsWrite failed: {}", id, KFS::ErrorCodeToStr(bytes_written));
        } else {
            m_total_written += bytes_written;
        }
        gettimeofday(&tv, 0);
        timeval = (tv.tv_sec*1000000 + tv.tv_usec - last_tv.tv_sec*1000000 - last_tv.tv_usec) / 1000000.0;
        bps = bytes_written/(timeval * 1000000);
        LOG_INFO("[qfs-{}] received_size:{}, chunk_size:{}, time:{}, bps:{}MB/s", id,
                 m_total_written, parquet_chunk->length, timeval, bps);
clean:
        if (0 != parquet_chunk->flag) {
            m_qfs_client->Close(m_qfs_fd);
            m_qfs_fd = 0;
            rename_file(parquet_chunk->start_time, parquet_chunk->end_time);
        }
        delete parquet_chunk;
    }
    LOG_DEBUG("[qfs-{}] wait read_parquet_mem_work", id);
    GetConfigs().show_all_ring_stat();
    xdf::rw_ring_wait_event(GetConfigs().parquet_mem_work_ring[id], 3);
}
