#include <thread>
#include <cstring>
#include <csignal>

#include "search_core.h"
#include "config.h"
#include "parquet_reader_factory.h"
#include "disk_pcap_writer.h"
#include "utils.h"


static const char * uuid;

static void signal_handler(int sig_num) {
    switch (sig_num) {
        case SIGTERM:
            _exit(SIGTERMERR);
        case SIGINT:
        case SIGQUIT:
        case SIGILL:
        case SIGABRT:
        case SIGFPE:
        case SIGSEGV:
            LOG_CRITICAL("- [{}] Process ({}) received signal {}!", 
                         uuid, getpid(), sig_num);
            _exit(SIGSEGVERR);
        default:
            break;
    }
}

void search_core(TaskInfo &request) {
    /* TODO avoid reload configuration file */
    GetConfig().reload();

    /* overwrite father process's signal handler */
    my_signal(SIGTERM, signal_handler);
    my_signal(SIGINT,  signal_handler);
    my_signal(SIGQUIT, signal_handler);
    my_signal(SIGILL,  signal_handler);
    my_signal(SIGABRT, signal_handler);
    my_signal(SIGFPE,  signal_handler);
    my_signal(SIGSEGV, signal_handler);

    auto dfs_parquet_reader = 
        [&request]() { ParquetReaderFactory::parquet_reader(request)->do_search(); };
    auto disk_pcap_writer   =
        [&request]() { DiskPcapWriter::instance(request).write_pcap(); };

    std::thread dfs_parquet_reader_th(dfs_parquet_reader);
    std::thread disk_pcap_writer_th(disk_pcap_writer);

    dfs_parquet_reader_th.join();
    disk_pcap_writer_th.join();
}

