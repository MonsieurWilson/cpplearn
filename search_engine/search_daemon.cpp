#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <uuid/uuid.h>
#include <cstddef>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <thread>

#include "search_daemon.h"
#include "search_core.h"
#include "config.h"
#include "log.h"
#include "utils.h"
#include "unix_socket.h"
#include "monitor.h"
#include "constants.h"


static void subprocess_cleaness() {
    pid_t pid;
    int   stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        auto  bptr = GetGlobalVars()->running_pool.begin(),
              eptr = GetGlobalVars()->running_pool.end();
        const char *uuid = nullptr;

        /* lookup in the running_pool according to pid */
        for (; bptr != eptr; ++bptr) {
            if (pid == bptr->second) {
                uuid = bptr->first.c_str();
                break;
            }
        }
        if (nullptr == uuid) {
            continue;
        }

        switch (WEXITSTATUS(stat)) {
            case 0:
                /* child process exits successfully */
                break;
            case SIGTERMERR:
                /* child process receives SIGTERM signal
                 * 1. remove left pcap file
                 * 2. mark task status as `CANCELED` */
                LOG_WARN("- [daemon] Task {} is going to be canceled.", uuid);
                unlink(GetGlobalVars()->task_pool[uuid]
                        .get_str_val_by_key("file_name"));
                GetGlobalVars()->task_pool[uuid]
                        .set_str_val_by_key("status", "CANCELED");
                break;
            case SIGSEGVERR:
                /* child process encounters fatal error
                 * 1. remove left pcap file
                 * 2. mark task status as `FAILED` */
                LOG_WARN("- [daemon] Task {} has been failed.", uuid);
                unlink(GetGlobalVars()->task_pool[uuid]
                        .get_str_val_by_key("file_name"));
                GetGlobalVars()->task_pool[uuid]
                        .set_str_val_by_key("status", "FAILED");
                break;
            default:
                break;
        }

        GetGlobalVars()->running_pool.erase(uuid);
    }
}

static void signal_handler(int sig_num) {
    switch (sig_num) {
        case SIGTERM:
            unlink(g_pid_file);
            unlink(g_unix_sock);
            exit(SIGTERMERR);
        case SIGHUP:
            LOG_INFO("- [daemon] Reload from configuration file.");

            GetConfig().reload();
            break;
        case SIGCHLD: 
            subprocess_cleaness();
            break;
        case SIGINT:
        case SIGQUIT:
        case SIGILL:
        case SIGABRT:
        case SIGFPE:
        case SIGSEGV:
            if (syscall(SYS_gettid) == GetGlobalVars()->monitor_tid) {
                LOG_WARN("- [monitor] Monitor encountered an error and was forced to exit.");
                pthread_exit(NULL);
            } else {
                LOG_CRITICAL("- [daemon] Search daemon received signal {}!", 
                             sig_num);
                exit(SIGSEGVERR);
            }
        default:
            break;
    }
}

static void daemon_loop() {
    int listen_fd, cli_fd, max_fd;
    int pipe_fd[2] = {-1, -1};
    pid_t pid;
    struct timeval timeout;
    fd_set read_set, all_set;

    if (-1 == access(g_pcap_dir, F_OK) && -1 == mkdir(g_pcap_dir, 0755)) {
        LOG_CRITICAL("- [daemon] Initialize pcap directory failed: {}",
                     strerror(errno));
        return;
    }

    if ((listen_fd = unix_serv_listen(g_unix_sock)) < 0) {
        LOG_CRITICAL("- [daemon] Initialize unix socket failed.");
        return;
    }

    FD_ZERO(&all_set);
    FD_SET(listen_fd, &all_set);
    max_fd = listen_fd;

    /* 
     * spawn a thread to do cleanning in case `task_pool` grows too large. 
     */
    auto monitor_th = []() { 
        GetGlobalVars()->monitor_tid = syscall(SYS_gettid);
        Monitor m;
        m.monitor_loop();
    };
    std::thread(monitor_th).detach();

    for (; ;) {
        if (-1 == kill(GetGlobalVars()->monitor_tid, 0)) {
            std::thread(monitor_th).detach();
        }

        read_set = all_set;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        /* 
         * two kinds of file descriptors:
         * 1. received requests from search client
         * 2. received responses from child process
         */
        if (select(max_fd + 1, &read_set, NULL, NULL, &timeout) < 0) {
            if (EINTR == errno) {
                /* ignore EINTR signal */
                continue;
            } else {
                LOG_CRITICAL("- [daemon] Select encounters an error: {}.", strerror(errno));
                return;
            }
        }

        /* 1. request from search client */
        if (FD_ISSET(listen_fd, &read_set)) {
            TaskInfo request;
            const char *uuid;

            cli_fd = unix_serv_accept(listen_fd);

            if (cli_fd < 0) {
                LOG_ERROR("- [daemon] Unable to accept client's connection: {}.", strerror(errno));
                continue;
            }

            if (!read_sock(cli_fd, request)) {
                LOG_ERROR("- [daemon] Failed to receive client's request.");
                continue;
            }
            LOG_DEBUG("- [daemon] clients's request: {}.", request.json_string());

            uuid = request.get_str_val_by_key("uuid");
            if (0 == strcmp(request.get_str_val_by_key("mode"), "status")) {
                if (GetGlobalVars()->task_pool.count(uuid) > 0) {
                    request.set_str_val_by_key("status",
                        GetGlobalVars()->task_pool[uuid].get_str_val_by_key("status"));
                    request.set_int_val_by_key("file_size",
                        GetGlobalVars()->task_pool[uuid].get_int_val_by_key("file_size"));
                } else {
                    request.set_str_val_by_key("status", "INVALID");
                }
                write_sock(cli_fd, request);
            } else if (0 == strcmp(request.get_str_val_by_key("mode"), "cancel")) {
                if (GetGlobalVars()->running_pool.count(uuid) > 0) {
                    kill(GetGlobalVars()->running_pool[uuid], SIGTERM);
                    request.set_str_val_by_key("status", "CANCELED");
                } else {
                    request.set_str_val_by_key("status", "INVALID");
                }
                LOG_DEBUG("- [daemon] daemon's reply: {}.", request.json_string());
                write_sock(cli_fd, request);
            } else if (0 == strcmp(request.get_str_val_by_key("mode"), "search")) {
                char new_uuid[BUF_SIZE];
                uuid = new_uuid;

                generate_uuid(new_uuid);
                request.set_str_val_by_key("status", "PENDING");
                request.set_str_val_by_key("uuid", new_uuid);
                if (0 == strlen(request.get_str_val_by_key("file_name"))) {
                    char file_name[BUF_SIZE];
                    snprintf(file_name, BUF_SIZE, "%s/%s.pcap",
                             g_pcap_dir, new_uuid);
                    request.set_str_val_by_key("file_name", file_name);
                }

                char ctime[BUF_SIZE];
                request.set_str_val_by_key("ctime", 
                    timestamp_to_strftime(time(NULL), ctime));
                LOG_DEBUG("- [daemon] daemon's reply: {}.", request.json_string());
                write_sock(cli_fd, request);

                GetGlobalVars()->task_pool[new_uuid] = std::move(request);
                GetGlobalVars()->pending_queue.push(new_uuid);
            } else {
                LOG_WARN("- [daemon] Unrecognized mode, but code shouldn't reach here.");
                LOG_WARN("- [daemon] TaskInfo: {}.", request.json_string());
                request.set_str_val_by_key("status", "INVALID");
                LOG_DEBUG("- [daemon] daemon's reply: {}.", request.json_string());
                write_sock(cli_fd, request);
            }
            close(cli_fd);
        }

        /* 2. read response from child process*/
        if (-1 != pipe_fd[0] && FD_ISSET(pipe_fd[0], &read_set)) {
            TaskInfo reply;
            if (!read_sock(pipe_fd[0], reply)) {
                /* child processes has been canceled */
            } else {
                const char *uuid = reply.get_str_val_by_key("uuid");
                GetGlobalVars()->task_pool[uuid]
                    .set_int_val_by_key("file_size",
                                        reply.get_int_val_by_key("file_size"));
                GetGlobalVars()->task_pool[uuid]
                    .set_str_val_by_key("status", "FINISHED");
                GetGlobalVars()->running_pool.erase(uuid);
            }
            close(pipe_fd[0]);
            FD_CLR(pipe_fd[0], &all_set);
            pipe_fd[0] = -1;
        }

        /* spawn a child process to do task */
        if (!GetGlobalVars()->pending_queue.empty() && 
            0 == GetGlobalVars()->running_pool.size()) {
            const char *uuid = GetGlobalVars()->pending_queue.front().c_str();
            TaskInfo &request = GetGlobalVars()->task_pool[uuid];
            GetGlobalVars()->pending_queue.pop();

            if (0 == strcmp("CANCELED", request.get_str_val_by_key("status"))) {
                /* task has been canceled */
                continue;
            } else {
                request.set_str_val_by_key("status", "RUNNING");
            }

            if (pipe(pipe_fd) < 0) {
                LOG_ERROR("- [daemon] Failed to build pipe, can't spawn job {}.", 
                          uuid);
                request.set_str_val_by_key("status", "FAILED");
                continue;
            }

            if ((pid = fork()) < 0) {
                LOG_ERROR("- [daemon] Failed to fork, can't spawn job {}.", uuid);
                request.set_str_val_by_key("status", "FAILED");
                continue;
            } else if (pid > 0) {
                GetGlobalVars()->running_pool[uuid] = pid;

                /* close write endpoint */
                close(pipe_fd[1]);
                /* add read endpoint to read set */
                FD_SET(pipe_fd[0], &all_set);
                max_fd = std::max(max_fd, pipe_fd[0]);
            } else {
                /* close read endpoint */
                close(pipe_fd[0]);
                close(listen_fd);
                
                /* search core */
                search_core(request);

                /* task has been finished, write back results */
                write_sock(pipe_fd[1], request);

                /* search process exit */
                close(pipe_fd[1]);
                _exit(0);
            }
        }
    }
}

int searchd() {
    /* run as daemon */
    if (0 != daemon(0, 0)) {
        fprintf(stderr, "ERROR: daemon operation failed (errno %d): %s",
                        errno, strerror(errno));
        goto err;
    } 

    /* make sure there is only one daemon running in background */
    if (!singleton(g_pid_file, signal_handler)) {
        goto err;
    }

    /* initialize logger */
    if (!GetLog().init("search_logger", g_log_file)) {
        goto err;
    }

    /* initialize configuration */
    if (!GetConfig().init(g_conf_file)) {
        LOG_CRITICAL("- [daemon] Configuration initialization failed");
        goto err;
    }

    /* start daemon loop */
    daemon_loop();

err:
    return DAEMONERR;
}

