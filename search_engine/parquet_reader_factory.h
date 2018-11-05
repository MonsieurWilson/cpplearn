#ifndef _PARQUET_READER_FACTORY_H
#define _PARQUET_READER_FACTORY_H

#include "dfs_parquet_rearder.h"
#include "hdfs_parquet_reader.h"
#include "qfs_parquet_reader.h"


class ParquetReaderFactory
{
    public:
        /*
         * Name:
         *  ParquetReaderFactory::parquet_reader
         *
         * Description:
         *  This function is used to create a parquet reader
         *  according to backend storage configuration.
         *
         * Parameters:
         *  v - [IN] the task information.
         *  
         * Returns:
         *  a smart pointer which points at a DFS parquet reader object.
         *
         * NOTE:
         *  None.
         */
        static std::unique_ptr<DfsParquetReader> parquet_reader(TaskInfo &v);
};

#endif
