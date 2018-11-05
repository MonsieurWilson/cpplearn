#include <fcntl.h>
#include <atomic>
#include <thread>
#include <chrono>

#include "utils.h"
#include "log.h"
#include "payload.h"
#include "config.h"
#include "qfs_parquet_reader.h"


QfsParquetReader::QfsParquetReader(TaskInfo &v) : DfsParquetReader(v) {
    qfs_client = KFS::Connect(GetGlobalVars()->dfs_host, GetGlobalVars()->dfs_port);
}

QfsParquetReader::~QfsParquetReader() {
}

bool QfsParquetReader::read_file_from_dfs(const char *file_name) {
    KFS::KfsFileAttr file_attr;
    int qfs_fd = 0;

    qfs_fd = qfs_client->Open(file_name, O_RDONLY, 1); 
    if (0 != qfs_client->Stat(qfs_fd, file_attr)) {
        LOG_ERROR("- [QfsParquetReader::read_file_from_qfs] [{}] QfsStat error: {}.",
                  task_info.get_str_val_by_key("uuid"),
                  KFS::ErrorCodeToStr(qfs_fd));
        return true;
    }
    int64_t file_len = file_attr.fileSize;

    std::shared_ptr<Buffer> file_contents = 
        std::make_shared<Buffer>((uint8_t *)malloc(sizeof(uint8_t) * file_len), file_len);

    if (file_len != qfs_client->Read(qfs_fd, (char *)file_contents->get_ptr(), file_len)) {
        LOG_ERROR("- [QfsParquetReader::read_file_from_qfs] [{}] QfsRead error: {}.",
                  task_info.get_str_val_by_key("uuid"),
                  KFS::ErrorCodeToStr(qfs_fd));
        return true;
    }
    qfs_client->Close(qfs_fd);

    return decode_parquet(file_contents);
}

void QfsParquetReader::scan_file(const char *root) {
    if (nullptr == qfs_client) {
        LOG_ERROR("- [QfsParquetReader::scan_file] [{}] Failed to connect QFS.",
                  task_info.get_str_val_by_key("uuid"));
        goto out;
    }

    if (!qfs_client->Exists(root)) {
        LOG_ERROR("- [QfsParquetReader::scan_file] [{}] QFS root {} doesn't exist.",
                  task_info.get_str_val_by_key("uuid"),
                  root);
        goto out;
    }

    scan_file_impl(root);

out:
    file_list_ring_push(new DfsFileName("", 1));
}

void QfsParquetReader::scan_file_impl(const char *root) {
    std::vector<std::string> dir_contents;
    KFS::KfsFileAttr file_attr;
    time_t ts, start_ts, end_ts;

    qfs_client->Readdir(root, dir_contents);
    for (auto &val : dir_contents) {
        if (0 == val.compare(".") || 0 == val.compare("..")) {
            continue;
        }
        std::string abs_path(root);
        abs_path += "/" + val;
        qfs_client->Stat(abs_path.c_str(), file_attr);
        if (!continue_searching) {
            break;
        } else if (file_attr.isDirectory) {
            ts = parquet_file_dir_to_timestamp(const_cast<char *>(abs_path.c_str()));
            if (stime <= ts && ts <= etime) {
                scan_file_impl(val.c_str()) ;
            }
        } else {
            parquet_file_path_to_timestamp(const_cast<char *>(abs_path.c_str()), start_ts, end_ts);
            if (stime <= end_ts && etime >= start_ts) {
                file_list_ring_push(new DfsFileName(abs_path));
                ++file_list_ring_size;
            }
        }
    }
}

