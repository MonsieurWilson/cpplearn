#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <unordered_map>


/* program name */
static const char *program = "search";

/* constants for length of fix sized ring */
static const unsigned int PB_RING_SIZE = 4096;
static const unsigned int FL_RING_SIZE = 20;

/* constants for common buffer size */
static const unsigned int BUF_SIZE = 128;

/* constants for task status */
enum Status {
    INVALID = -1,
    PENDING     ,
    RUNNING     ,
    FINISHED    ,
    CANCELED    ,
    FAILED      ,
    /* TODO
     * task uuid persistence, eg SQLite, MySQL etc.
     */
    DELETED     ,
};

/* constants for error code */
static const unsigned int MAINERR    = 10;
static const unsigned int CLIENTERR  = 20;
static const unsigned int DAEMONERR  = 30;
static const unsigned int SIGTERMERR = 40;
static const unsigned int SIGSEGVERR = 50;

/* constants map for task status <enum-string> mapping */
static std::unordered_map<std::string, Status> status_map(
        {
            {"INVALID",  INVALID},
            {"PENDING",  PENDING},
            {"RUNNING",  RUNNING},
            {"FINISHED", FINISHED},
            {"CANCELED", CANCELED},
            {"FAILED",   FAILED},
            {"DELETED",  DELETED}
        });

/* constants for configuration files */
static const char *g_pid_file  = "/var/run/search.pid";
static const char *g_unix_sock = "/var/run/search.sock";
static const char *g_conf_file = "/etc/search.conf";
static const char *g_log_file  = "/var/log/search";
static const char *g_pcap_dir  = "/tmp/pcap";

#endif
