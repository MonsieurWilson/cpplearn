
#ifndef _XDF_HDFS_PARQUET_WRITER_H
#define _XDF_HDFS_PARQUET_WRITER_H

#include <hdfs/hdfs.h>

#include "packet_define.h"

class HdfsParquetWriter
{
private:
    hdfsFS                     hdfs_;
    hdfsFile                   file_;
    char                       hdfs_path[64];
    char                       hdfs_name[64];
    int64_t                    total_written;
    size_t                     id;

private:
    HdfsParquetWriter();

    void generate_filename(time_t, time_t);
    void rename_file(time_t, time_t);
    void hdfs_connect(void);

public:
    ~HdfsParquetWriter();

    static HdfsParquetWriter& instance(size_t id)
    {
        static HdfsParquetWriter *s_hdfs_parquet_writer = 
                    new HdfsParquetWriter[PARQUET_MEM_WORK_RING_COUNT];
        s_hdfs_parquet_writer[id].id = id;
        return s_hdfs_parquet_writer[id];
    }

    void write_to_hdfs(void);

};

#endif //_XDF_HDFS_PARQUET_WRITER_H
