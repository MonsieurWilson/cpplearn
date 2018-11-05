#ifndef _HDFS_PARQUET_READER_H
#define _HDFS_PARQUET_READER_H

#include <hdfs/hdfs.h>

#include "dfs_parquet_rearder.h"


class HdfsParquetReader : public DfsParquetReader
{
    public:
        HdfsParquetReader(TaskInfo &v);

        ~HdfsParquetReader();

    private:
        void scan_file(const char *root) override;

        void scan_file_impl(const char *root);

        bool read_file_from_dfs(const char *file_name) override;

        hdfsFS                     hdfs_fs;
};

#endif
