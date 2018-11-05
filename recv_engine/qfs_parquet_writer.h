
#ifndef _XDF_QFS_PARQUET_WRITER_H
#define _XDF_QFS_PARQUET_WRITER_H

#include <kfs/KfsClient.h>
#include <kfs/KfsAttr.h>

#include "packet_define.h"

class QfsParquetWriter
{
private:
    int                        m_qfs_fd;
    KFS::KfsClient *           m_qfs_client;
    char                       m_qfs_path[64];
    char                       m_qfs_name[64];
    int64_t                    m_total_written;
    size_t                     id;

private:
    QfsParquetWriter();

    void generate_filename(time_t, time_t);
    void rename_file(time_t, time_t);

public:
    ~QfsParquetWriter();

    static QfsParquetWriter& instance(size_t id)
    {
        static QfsParquetWriter *s_qfs_parquet_writer = 
                    new QfsParquetWriter[PARQUET_MEM_WORK_RING_COUNT];
        s_qfs_parquet_writer[id].id = id;
        return s_qfs_parquet_writer[id];
    }

    void write_to_qfs(void);

};

#endif //_XDF_QFS_PARQUET_WRITER_H
