#include <atomic>
#include <thread>
#include <chrono>

#include "utils.h"
#include "log.h"
#include "payload.h"
#include "config.h"
#include "hdfs_parquet_reader.h"


HdfsParquetReader::HdfsParquetReader(TaskInfo &v) : DfsParquetReader(v) {
    struct hdfsBuilder *builder = hdfsNewBuilder();
    hdfsBuilderSetNameNode(builder, GetGlobalVars()->dfs_host);
    hdfsBuilderSetNameNodePort(builder, GetGlobalVars()->dfs_port);
    hdfsBuilderConfSetStr(builder, "dfs.client.log.severity", "ERROR");
    hdfsBuilderConfSetStr(builder, "dfs.domain.socket.path", "/var/lib/hadoop-hdfs/dn_socket");
    hdfs_fs = hdfsBuilderConnect(builder);
    hdfsFreeBuilder(builder);
}

HdfsParquetReader::~HdfsParquetReader() {
    hdfsDisconnect(hdfs_fs);
}

bool HdfsParquetReader::read_file_from_dfs(const char *file_name) {
    int num_entries;

    hdfsFile hdfs_file = hdfsOpenFile(hdfs_fs, file_name, O_RDONLY, 0, 1, 0x20000000);
    hdfsFileInfo *file_info = hdfsListDirectory(hdfs_fs, file_name, &num_entries);
    tOffset file_len = file_info->mSize; 
    hdfsFreeFileInfo(file_info, num_entries);

    std::shared_ptr<Buffer> file_contents = 
        std::make_shared<Buffer>((uint8_t *)malloc(sizeof(uint8_t) * file_len), file_len);

    int32_t read_len;
    tOffset already_read_len = 0;
    while (0 != (read_len = hdfsRead(hdfs_fs, 
                                     hdfs_file, 
                                     (void *)(file_contents->get_ptr()+already_read_len), 
                                     file_len))) {
        already_read_len += read_len;
    }   
    
    if (already_read_len != file_len) {
        LOG_ERROR("- [HdfsParquetReader::read_file_from_qfs] [{}] HdfsRead error: {}.",
                  task_info.get_str_val_by_key("uuid"),
                  hdfsGetLastError());
        return true;
    }

    if (-1 == hdfsCloseFile(hdfs_fs, hdfs_file)) {
        LOG_ERROR("- [HdfsParquetReader::read_file_from_qfs] [{}] HdfsCloseFile error: {}.",
                  task_info.get_str_val_by_key("uuid"),
                  hdfsGetLastError());
        return true;
    }

    return decode_parquet(file_contents);
}

void HdfsParquetReader::scan_file(const char *root) {
    if (0 != hdfsExists(hdfs_fs, root)) {
        LOG_ERROR("- [HdfsParquetReader::scan_file] [{}] HDFS root {} doesn't exist.",
                  task_info.get_str_val_by_key("uuid"),
                  root);
        goto out;
    }

    scan_file_impl(root);

out:
    /* Add tailing entry to file list ring. */
    file_list_ring_push(new DfsFileName("", 1));
}

void HdfsParquetReader::scan_file_impl(const char *root) {
    int num_entries = 0;
    hdfsFileInfo *dir_info =
            hdfsListDirectory(hdfs_fs, root, &num_entries);

    time_t ts, start_ts, end_ts;
    hdfsFileInfo *d_info = dir_info;
    for (int idx = 0; idx < num_entries; ++idx, ++d_info) {
        if (!continue_searching) {
            break;
        } else if (kObjectKindDirectory == d_info->mKind) {
            ts = parquet_file_dir_to_timestamp(d_info->mName);
            if (stime <= ts && ts <= etime) {
                scan_file_impl(d_info->mName) ;
            }
        } else {
            parquet_file_path_to_timestamp(d_info->mName, start_ts, end_ts);
            if (stime <= end_ts && etime >= start_ts) {
                file_list_ring_push(new DfsFileName(d_info->mName));
                ++file_list_ring_size;
            }
        }
    }
}

