//misc.h

#ifndef _XDF_MISC_H_
#define _XDF_MISC_H_

#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <fcntl.h>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>


namespace xdf
{

    template<class Stream>
    void hex_print(const char * buf, int len, Stream& stream) {
        const char num[] = "0123456789ABCDEF";
        const char * p = buf;
        std::string textstr;
        int  count = 0;

        if( len <= 0 )
            return;
        stream << std::hex;
        for( ; count < len; ++p, ++count ) {
            if( count % 16 == 0 ) {
                stream << "0x" << count << "\t";
                textstr = "";
            }
            stream << num[((*p)&0xf0)>>4] << num[(*p)&0xf] << " ";
            textstr += isprint(static_cast<unsigned char>(*p))? *p : '.';

            if( count % 16 == 15 ) {
                stream << "   " << textstr << std::endl;
            }
        }
        if( count % 16 != 0 ) {
            stream << std::string((17 - count%16) * 3, ' ') << textstr << std::endl;
        }
        stream << std::dec;
    }

    typedef struct _ThreadInfo {    /* Used as argument to thread_start() */
        void *(*func)(void *);
        void *arg;
    } ThreadInfo;

    void *thread_start(void *arg);

    bool thread_create(void* (*func)(void *), void* arg, bool use_dpdk = false);

    time_t strftime_to_timestamp(const char *str_fmt_time, const char *fmt="%Y-%m-%d %H:%M:%S");

    char *timestamp_to_strftime(const time_t ts, char *str_fmt_time, const char *fmt="%Y-%m-%d %H:%M:%S");

    bool singleton(const char *pid_file, void (*sig_handler)(int));

    bool is_different_day(time_t t1, time_t t2);

} //namespace xdf

#endif //_XDF_MISC_H_
