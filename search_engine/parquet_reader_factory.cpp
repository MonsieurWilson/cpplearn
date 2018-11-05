#include "parquet_reader_factory.h"
#include "config.h"
#include "log.h"

std::unique_ptr<DfsParquetReader>
ParquetReaderFactory::parquet_reader(TaskInfo &v) {
    DfsParquetReader *parquet_reader = nullptr;
    const char *backend = GetGlobalVars()->storage_backend;
    if (0 == strcmp(backend, "qfs")) {
        parquet_reader = new QfsParquetReader(v);
    } else if (0 == strcmp(backend, "hdfs")) {
        parquet_reader = new HdfsParquetReader(v);
    } else {
        LOG_WARN("- [ParquetReaderFactory::parquet_reader] "
                 "Unsupported storage backend, HDFS will be used by default.");
        parquet_reader = new HdfsParquetReader(v);
    }

    return std::unique_ptr<DfsParquetReader>(parquet_reader);
}

