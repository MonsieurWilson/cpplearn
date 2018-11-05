#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "config.h"
#include "utils.h"
#include "search_client.h"
#include "unix_socket.h"
#include "constants.h"


int searchc(TaskInfo &request) {
    TaskInfo reply;
    char buffer[BUF_SIZE];
    int ret, fd;

    if (!process_exist(g_pid_file)) {
        fprintf(stderr, "ERROR: daemon should be running first!\n");
        return CLIENTERR;
    }

    /* establish a connection to daemon */
    if ((fd = unix_cli_connect(g_unix_sock)) < 0) {
        fprintf(stderr, "ERROR: failed to connect unix socket!\n");
        return CLIENTERR;
    }

    /* submit a request to daemon */
    if (!write_sock(fd, request)) {
        fprintf(stderr, "ERROR: failed to send request to daemon!\n");
        return CLIENTERR;
    }

    /* read response from daemon */
    if (!read_sock(fd, reply)) {
        fprintf(stderr, "ERROR: failed to receive reply from daemon!\n");
        return CLIENTERR;
    }

    if (0 == strcmp(reply.get_str_val_by_key("mode"), "search")) {
        const char *uuid = reply.get_str_val_by_key("uuid");
        if (nullptr == uuid) {
            snprintf(buffer, BUF_SIZE, 
                     "ERROR: encountered an error submitting task to daemon!\n");
            ret = CLIENTERR;
        } else {
            snprintf(buffer, BUF_SIZE, "The UUID of task: %s\n", uuid);
            ret = 0;
        }
    } else {
        const char *status = reply.get_str_val_by_key("status");
        switch (status_map[status]) {
            case FINISHED:
                snprintf(buffer, BUF_SIZE, "Task has been finished.\n"
                         "Total matched size (including 24 Byte pcap file header): %lu.\n",
                         reply.get_int_val_by_key("file_size"));
                break;
            case RUNNING:
                snprintf(buffer, BUF_SIZE, "Task is still running.\n");
                break;
            case PENDING:
                snprintf(buffer, BUF_SIZE, "Task is pending.\n");
                break;
            case CANCELED:
                snprintf(buffer, BUF_SIZE, "Task has been canceled.\n");
                break;
            case FAILED:
                snprintf(buffer, BUF_SIZE, "Task has been failed.\n");
                break;
            default:
                snprintf(buffer, BUF_SIZE, "No such task, please examine given uuid.\n");
                break;
        }
        ret = status_map[status];
    }
    fprintf(stdout, "%s", buffer);

    close(fd);
    return ret;
}

