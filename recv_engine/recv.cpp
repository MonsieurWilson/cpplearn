
#include <sched.h>
#include <sys/time.h>
#include <cstdint>
#include <getopt.h>
#include <thread>

#include "packet.h"
#include "misc.h"
#include "log.h"
#include "packet_receiver.h"
#include "mem_parquet_writer.h"
#include "hdfs_parquet_writer.h"
#include "qfs_parquet_writer.h"

#define MAX_FILENAME_LEN 64


static const char *program;
static char conf_file[MAX_FILENAME_LEN];
static char log_file [MAX_FILENAME_LEN];
static char pid_file [MAX_FILENAME_LEN];

static void Usage()
{
    fprintf(stdout, "USAGE\n"
                    "       %s --url|-u <zmq_url>[,<zmq_url>...] [OPTIONS]\n\n", program);
    fprintf(stdout, "DESCRIPTION\n"
                    "       This program is a daemon process, which will connect particular ports\n"
                    "       specified by command line parameters, and convert received packets into\n"
                    "       Apache Parquet file format. Then, put the parquet file into given HDFS.\n\n");
    fprintf(stdout, "OPTIONS\n"
                    "       --url, -u\n"
                    "             url address to connect (format: tcp://127.0.0.1:4500)\n"
                    "             NOTE: Mutiple url address are seperated using comma ',' without any spaces.\n"
                    "                   For instance, tcp://127.0.0.1:4500,tcp://127.0.0.1:4501\n"
                    "       --mode, -m\n"
                    "             ZMQ mode. Illegal values are \"PULL\" and \"SUB\", other modes will be silently\n"
                    "             ignored. Default to be \"PULL\".\n");
}

static void signal_handler(int sig_num) 
{
    switch (sig_num) {
        case SIGTERM:
            unlink(pid_file);
            exit(0);
        case SIGHUP:
            GetConfigs().reload(conf_file);
            break;
        case SIGINT:
        case SIGQUIT:
        case SIGILL:
        case SIGABRT:
        case SIGFPE:
        case SIGCHLD:
            break;
        case SIGSEGV:
            LOG_CRITICAL("process ({}) received SIGSEGV signal!", 
                         getpid());
            exit(-1);
        default:
            break;
    }
}

static bool parse_args(int argc, char **argv)
{
    if (argc < 2) {
        Usage();
        return false;
    }

    const char *short_opts = "m:u:h";
    const struct option long_opts[] = {
        {"help",  0, NULL, 'h'},
        {"mode",  1, NULL, 'm'},
        {"url",   1, NULL, 'u'},
        {NULL,    0, NULL,  0 }
    };

    int option;
    opterr = 0;
    while (-1 != (option = getopt_long(argc, argv, short_opts, long_opts, NULL))) {
        switch (option) {
            case 'h':
                Usage();
                return false;
            case 'm':
                if (0 == strcmp(optarg, "PULL") ||
                    0 == strcmp(optarg, "SUB")) {
                    GetGlobalVars()->m_zmq_mode = optarg;
                }
                break;
            case 'u':
                {
                    char *url;
                    int idx = 0;
                    char *val = optarg;
                    for (;; ++idx, val = nullptr) {
                        url = strtok(val, ",");
                        if (url == nullptr) {
                            break;
                        }
                        GetGlobalVars()->m_zmq_url.push_back(url);
                    }
                    GetGlobalVars()->m_zmq_port_num = idx;
                }
                break;
            default:
                fprintf(stdout, "Invalid parameters, using --help, -h to see more details.\n");
                return false;
        }
    }

    if (0 == GetGlobalVars()->m_zmq_port_num) {
        fprintf(stdout, "ERROR: parameter url must be specified!\n\n");
        Usage();
        return false;
    }

    return true;
}

static void recv_packet(void)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(15, &mask);
    if( sched_setaffinity(0, sizeof(mask), &mask) == -1 ) {
        LOG_WARN("set recv_packet affinity failed..");  
    }
    while( true ) {
        PacketReceiver::instance().recv_packet();
    }
}

static void write_to_mem(size_t id)
{
    // cpu_set_t mask;
    // CPU_ZERO(&mask);
    // CPU_SET(10+id, &mask);
    // if( sched_setaffinity(0, sizeof(mask), &mask) == -1 ) {
    //     LOG_WARN("set write_to_mem affinity failed..");  
    // }
    while( true ) {
        MemParquetWriter::instance(id).write_to_mem();
    }
}

static void write_to_hdfs(size_t id)
{
    while( true ) {
        HdfsParquetWriter::instance(id).write_to_hdfs();
    }
}

static void write_to_qfs(size_t id)
{
    while( true ) {
        QfsParquetWriter::instance(id).write_to_qfs();
    }
}

static void run_thread(void)
{
    std::vector<int> thread_ids;
    for (size_t idx = 0; idx < PACKET_GROUP_WORK_RING_COUNT; ++idx) {
        thread_ids.push_back(idx);
        std::thread (write_to_mem, thread_ids[idx]).detach();
    }
    thread_ids.clear();
    for (size_t idx = 0; idx < PARQUET_MEM_WORK_RING_COUNT; ++idx) {
        thread_ids.push_back(idx);
        if (0 == GetGlobalVars()->m_storage_backend.compare("hdfs")) {
            std::thread (write_to_hdfs, thread_ids[idx]).detach();
        } else {
            std::thread (write_to_qfs, thread_ids[idx]).detach();
        }
    }
    std::thread (recv_packet).detach();
    sleep(1);
    GetConfigs().io.run();
}

int main(int argc, char* argv[])
{
    program = basename(argv[0]);
    snprintf(pid_file,  MAX_FILENAME_LEN, "/var/run/%s.pid", program);
    snprintf(conf_file, MAX_FILENAME_LEN, "/etc/%s.conf", program);
    snprintf(log_file,  MAX_FILENAME_LEN, "/var/log/%s", program);

    if (!GetLog().init("recv_logger", log_file)) {
        exit(-1);
    }

    if (!GetConfigs().init(conf_file)) {
        LOG_CRITICAL("configuration initialization failed");
        exit(-1);
    }

    if (!parse_args(argc, argv)) {
        exit(-1);
    }

    if (0 != daemon(0, 0)) {
        fprintf(stdout, "daemon operation failed (errno %d): %s",
                        errno, strerror(errno));
        exit(-1);
    }

    if (!xdf::singleton(pid_file, signal_handler)) {
        exit(-1);
    }

    run_thread();

    return 0;
}

