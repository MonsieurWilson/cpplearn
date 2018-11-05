#ifndef _DFS_PARQUET_READER_H
#define _DFS_PARQUET_READER_H

#include <atomic>
#include <memory>

#include "payload.h"
#include "task_info.h"


struct DfsFileName
{
    std::string file_name;
    int         flag;

    DfsFileName(const std::string &s, int f=0)
        : file_name(s), flag(f) {}
};

class DfsParquetReader
{
    public:
        explicit DfsParquetReader(TaskInfo &v);

        virtual ~DfsParquetReader() noexcept = 0;

        /*
         * Name:
         *  DfsParquetReader::do_search
         *
         * Description:
         *  This function is used to do search task.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  None.
         *
         * NOTE:
         *  None.
         */
        void do_search();

    protected:
        /*
         * Name:
         *  DfsParquetReader::decode_parquet
         *
         * Description:
         *  This function is used to decode a parquet stream.
         *
         * Parameters:
         *  contents - [IN] a buffer that contains parquet stream.
         *
         * Returns:
         *  true, should coutinue searching.
         *  false, should stop searching.
         *
         * NOTE:
         *  This function should be called in read_file_from_dfs function
         *  which is implemented by derived classes.
         */
        bool decode_parquet(std::shared_ptr<Buffer> &contents);

        std::atomic<unsigned long> match_packet_size;
        std::atomic<bool>          continue_searching;
        unsigned long              file_list_ring_size;
        time_t                     stime;
        time_t                     etime;
        TaskInfo &                 task_info;

    private:
        /*
         * Name:
         *  DfsParquetReader::scan_file
         *
         * Parameters:
         *  root - [IN] the root directory where parquet files locate in.
         *
         * Returns:
         *  None.
         *
         * NOTE:
         *  All derived classes should implement this function.
         */
        virtual void scan_file(const char *root) = 0;

        /*
         * Name:
         *  DfsParquetReader::read_file_from_dfs
         *
         * Description:
         *  This function is used to read particular file in HDFS
         *  and match five tuples according to user inputs.
         *
         * Parameters:
         *  file_name - the absolute path of file stored in DFS.
         *
         * Returns:
         *  true, reach the limit size.
         *  false, doesn't reach the limit size.
         *
         * NOTE:
         *  All derived classes should implement this function.
         */
        virtual bool read_file_from_dfs(const char *file_name) = 0;

        /*
         * Name:
         *  DfsParquetReader::is_match_session
         * 
         * Description:
         *  this function is used to check whether given five tuples
         *  matches searching arguments.
         *
         * Parameters:
         *  sip   - [IN] source address
         *  dip   - [IN] destination address
         *  sport - [IN] source port number
         *  dport - [IN] destination port number
         *  proto - [IN] ip protocol
         * 
         * Return:
         *  true, match.
         *  false, doesn't match.
         *
         * NOTE:
         *  None.
         */
        bool is_match_session(uint32_t sip,   uint32_t dip,
                              uint16_t sport, uint16_t dport,
                              uint16_t proto);

};

#endif
