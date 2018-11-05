/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <uuid/uuid.h>

#include <cerrno>
#include <cstring>
#include <iostream>


static const size_t BUF_SIZE = 512;
static const char *PROGRAM_NAME = "leave_namespace";

static void Usage() {
    char tmpl[] = "Usage: %s [OPTIONS]\n\n"
                  "OPTIONS\n"
                  "    -i   unshare IPC namespace\n"
                  "    -m   unshare mount namespace\n"
                  "    -n   unshare network namespace\n"
                  "    -p   unshare PID namespace\n"
                  "    -u   unshare UTS namespace\n"
                  "    -U   unshare user namespace";
    char usage[BUF_SIZE];
    snprintf(usage, BUF_SIZE, tmpl, PROGRAM_NAME);
    std::cout << usage << std::endl;
    exit(1);
}

static void generate_uuid(char *out) {
    uuid_t uuid;
    uuid_generate(uuid);
    uuid_unparse(uuid, out);
}

int main(int argc, char **argv) {
    int flag = 0, opt, ret;

    while ((opt = getopt(argc, argv, "imnpuUh")) != -1) {
        switch (opt) {
            case 'i': flag |= CLONE_NEWIPC;   break;
            case 'm': flag |= CLONE_NEWNS;    break;
            case 'n': flag |= CLONE_NEWNET;   break;
            case 'p': flag |= CLONE_NEWPID;   break;
            case 'u': flag |= CLONE_NEWUTS;   break;
            case 'U': flag |= CLONE_NEWUSER;  break;
            case 'h': Usage();                break;
            default:  Usage();                break;
        }
    }

    if (flag == 0) {
        Usage();
    }

    if (unshare(flag) == -1) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return 1;
    }

    /* generate a random uuid and set it as the new hostname */
    char uuid[BUF_SIZE];
    generate_uuid(uuid);
    sethostname(uuid, strlen(uuid));

    /* execute a new bash to substitute current process */
    execlp("bash", "bash", (char *)NULL);

    return 0;
}
