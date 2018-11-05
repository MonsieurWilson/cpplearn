#ifndef _QFS_PARQUET_READER_H
#define _QFS_PARQUET_READER_H

#include <kfs/KfsClient.h>
#include <kfs/KfsAttr.h>

#include "dfs_parquet_rearder.h"


class QfsParquetReader : public DfsParquetReader
{
    public:
        QfsParquetReader(TaskInfo &v);

        ~QfsParquetReader();

    private:
        void scan_file(const char *root) override;

        void scan_file_impl(const char *root);

        bool read_file_from_dfs(const char *file_name) override;

        KFS::KfsClient *           qfs_client;
};

#endif
