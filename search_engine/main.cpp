#include <arpa/inet.h>
#include <getopt.h>
#include <cstring>

#include "search_daemon.h"
#include "search_client.h"
#include "task_info.h"
#include "utils.h"
#include "constants.h"


static TaskInfo request;

static void Usage() {
    fprintf(stdout, "USAGE\n"
                    "       %s --mode|-m <mode> [OPTIONS]\n\n", program);
    fprintf(stdout, "DESCRIPTION\n"
                    "       This program is a toolkit to scan parquet files stored in HDFS according to given\n"
                    "       time range and five tuples, and generate a pcap file only containing related payloads.\n\n");
    fprintf(stdout, "OPTIONS\n");
    fprintf(stdout, "       --mode, -m\n"
                    "               search: invoke a search request to the server\n"
                    "               status: query the status of searching request by ID\n"
                    "               cancel: cancel the searching job by ID\n"
                    "               daemon: start the daemon\n");
    fprintf(stdout, "       --uuid, -u\n"
                    "               (must be specified in \"status\" and \"cancel\" mode)\n"
                    "               request id\n");
    fprintf(stdout, "       --stime, -b\n"
                    "               (must be specified in \"search\" mode)\n"
                    "               start time (format: \"YYYY-mm-dd HH:MM:SS\")\n");
    fprintf(stdout, "       --etime, -e\n"
                    "               (must be specified in \"search\" mode)\n"
                    "               end time   (format: \"YYYY-mm-dd HH:MM:SS\")\n");
    fprintf(stdout, "       --sip, -s\n"
                    "               source IP address (format: 192.168.0.1 [dot-decimal notation])\n");
    fprintf(stdout, "       --dip, -d\n"
                    "               destination IP address\n");
    fprintf(stdout, "       --sport, -S\n"
                    "               source port number\n");
    fprintf(stdout, "       --dport, -D\n"
                    "               destination port number\n");
    fprintf(stdout, "       --proto, -p\n"
                    "               IP protocal number\n");
    fprintf(stdout, "       --limit, -l\n"
                    "               the maximum size of downloaded pcap file (MB [default value is 1GB])\n");
    fprintf(stdout, "       --file, -f\n"
                    "               the name of downloaded pcap file\n\n");
    fprintf(stdout, "NOTES\n");
    fprintf(stdout, "       All the OPTIONS will be ignored in \"daemon\" mode\n");
}

static bool parse_args(int argc, char **argv) {
    if (argc < 2) {
        Usage();
        return false;
    }

    const char *short_opts = "m:u:b:e:s:d:l:S:D:p:f:h";
    const struct option long_opts[] = {
        {"help",  0, NULL, 'h'},
        {"mode",  1, NULL, 'm'},
        {"uuid",  1, NULL, 'u'},
        {"stime", 1, NULL, 'b'},
        {"etime", 1, NULL, 'e'},
        {"sip",   1, NULL, 's'},
        {"dip",   1, NULL, 'd'},
        {"sport", 1, NULL, 'S'},
        {"dport", 1, NULL, 'D'},
        {"proto", 1, NULL, 'p'},
        {"limit", 1, NULL, 'l'},
        {"file",  1, NULL, 'f'},
        {NULL,    0, NULL,  0 }
    };

    int option;
    opterr = 0;
    struct in_addr st_addr;
    while (-1 != (option = getopt_long(argc, argv, short_opts, long_opts, NULL))) {
        switch (option) {
            case 'h':
                Usage();
                return false;
            case 'm':
                request.set_str_val_by_key("mode", optarg);
                break;
            case 'u':
                request.set_str_val_by_key("uuid", optarg);
                break;
            case 'b':
                request.set_str_val_by_key("stime", optarg);
                break;
            case 'e':
                request.set_str_val_by_key("etime", optarg);
                break;
            case 's':
                if (0 != inet_aton(optarg, &st_addr)) {
                    request.set_int_val_by_key("sip", ntohl(st_addr.s_addr));
                    break;
                } else {
                    fprintf(stderr, "ERROR: Invalid source IP address format!\n");
                    return false;
                }
            case 'd':
                if (0 != inet_aton(optarg, &st_addr)) {
                    request.set_int_val_by_key("dip", ntohl(st_addr.s_addr));
                    break;
                } else {
                    fprintf(stderr, "ERROR: Invalid destination IP address format!\n");
                    return false;
                }
            case 'S':
                request.set_int_val_by_key("sport", atoi(optarg));
                break;
            case 'D':
                request.set_int_val_by_key("dport", atoi(optarg));
                break;
            case 'p':
                request.set_int_val_by_key("proto", atoi(optarg));
                break;
            case 'l':
                /* Bug #136246 fix: add symbol L to avoid integer overflow */
                request.set_int_val_by_key("limit", atoi(optarg) * 1024L * 1024L);
                break;
            case 'f':
                request.set_str_val_by_key("file_name", optarg);
                break;
            default:
                fprintf(stderr, "Invalid parameters, using --help, -h to see more details.\n");
                return false;
        }
    }

    /* parameter validation */
    const char *uuid  = request.get_str_val_by_key("uuid");
    const char *stime = request.get_str_val_by_key("stime");
    const char *etime = request.get_str_val_by_key("etime");
    const char *mode  = request.get_str_val_by_key("mode");
    uint64_t    limit = request.get_int_val_by_key("limit");
    if (0 != strcmp(mode, "status") && 0 != strcmp(mode, "search") &&
        0 != strcmp(mode, "cancel") && 0 != strcmp(mode, "daemon")) {
        fprintf(stderr, "ERROR: unrecognized given mode!\n\n");
        Usage();
        return false;
    }

    if (0 == strcmp(mode, "status") && 0 == strlen(uuid)) {
        fprintf(stderr, "ERROR: uuid must be specified in status mode!\n\n");
        Usage();
        return false;
    }

    if (0 == strcmp(mode, "cancel") && 0 == strlen(uuid)) {
        fprintf(stderr, "ERROR: uuid must be specified in cancel mode!\n\n");
        Usage();
        return false;
    }

    if (0 == strcmp(mode, "search") &&
        (0 == strlen(stime) || 0 == strlen(etime))) {
        fprintf(stderr, 
                "ERROR: stime and etime must be specified in search mode!\n\n");
        Usage();
        return false;
    }

    /* assign default values */
    if (0 == limit) {
        request.set_int_val_by_key("limit", 1024 * 1024 * 1024); // 1G
    }

    return true;
}

int main(int argc, char **argv) {
    /* parse command line arguments */
    if (!parse_args(argc, argv)) {
        exit(MAINERR);
    }

    /* start search engine daemon */
    if (0 == strcmp(request.get_str_val_by_key("mode"), "daemon")) {
        exit(searchd());
    } 

    /* start search engine client */
    exit(searchc(request));
}

