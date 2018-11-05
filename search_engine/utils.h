#ifndef _UTILS_H
#define _UTILS_H

#include <sys/types.h>

#include "constants.h"
#include "task_info.h"


struct SockHeader 
{
    char     msg_type[BUF_SIZE];
    uint64_t msg_length;
};

/*
 * Name:
 *  strftime_to_timestamp
 *
 * Parameters:
 *  str_fmt_time - [IN] time value in format given by the second parameter.
 *  fmt          - [IN] time format. (The default format is "yyyy-mm-dd HH:MM:SS")
 * 
 * Returns:
 *  time stamp.
 *
 * NOTE:
 *  None.
 */
time_t strftime_to_timestamp(const char *str_fmt_time, 
                             const char *fmt="%Y-%m-%d %H:%M:%S");

/*
 * Name:
 *  timestamp_to_strftime
 *
 * Parameters:
 *  ts           - [IN] time stamp.
 *  str_fmt_time - [OUT] time value in format given by the second parameter.
 *  fmt          - [IN] time format. (The default format is "yyyy-mm-dd HH:MM:SS")
 * 
 * Returns:
 *  A pointer which points at the time value stored in `str_fmt_time`.
 *
 * NOTE:
 *  The function caller should allocate memory for parameter `str_fmt_time` in advance.
 */
char *timestamp_to_strftime(const time_t ts, char *str_fmt_time, 
                            const char *fmt="%Y-%m-%d %H:%M:%S");

/*
 * Name:
 *  parquet_file_dir_to_timestamp
 * 
 * Parameters:
 *  file_dir - [IN] absolute path of the parquet file.
 *
 * Returns:
 *  time stamp.
 *
 * NOTE:
 *  None.
 */
time_t parquet_file_dir_to_timestamp(char *file_dir);

/*
 * Name:
 *  parquet_file_path_to_timestamp
 * 
 * Parameters:
 *  file_path - [IN] directory of the parquet file.
 *  start_ts  - [OUT] the first timestamp of the parquet file.
 *  end_ts    - [OUT] the second timestamp of the parquet file.
 *
 * Returns:
 *  time stamp.
 *
 * NOTE:
 *  None.
 */
void parquet_file_path_to_timestamp(char *file_path, time_t &start_ts, time_t &end_ts);

/* 
 * Name:
 *  my_signal
 *
 * Description:
 *  This function is a wrapper function of system call `sigaction`.
 *
 * Parameters:
 *  signo - [IN] signal number.
 *  func  - [IN] signal handler function.
 *
 * Returns:
 *  None.
 *
 * NOTE:
 *  None.
 */
void (*my_signal(int signo, void (*func)(int)))(int);

/* 
 * Name:
 *  singleton
 *
 * Description:
 *  This function is used to implement singleton pattern for
 *  daemon process.
 *
 * Parameters:
 *  pid_file    - [IN] pid file of daemon process.
 *  sig_handler - [IN] signal handler function.
 *
 * Returns:
 *  true, success.
 *  false, failure.
 *
 * NOTE:
 *  None.
 */
bool singleton(const char *pid_file, void (*sig_handler)(int));

/*
 * Name:
 *  process_exist
 *
 * Description:
 *  This function is used to check whether process given by 
 *  pid file exists.
 *
 * Parameters:
 *  pid_file - [IN] pid file of daemon process.
 *
 * Returns:
 *  true, exists.
 *  false, not exist.
 *
 * NOTE:
 *  None.
 */
bool process_exist(const char *pid_file);

/* 
 * Name:
 *  generate_uuid
 *
 * Description:
 *  This function is used to generate a random UUID for 
 *  each income task.
 *
 * Parameters:
 *  out - [OUT] uuid.
 *
 * Returns:
 *  None.
 *
 * NOTE:
 *  UUID was transformed into an char array of 36 bytes. The
 *  caller should ensure that the length of `out` array is 
 *  at least 37 bytes.
 */
void generate_uuid(char *out);

/* 
 * Name:
 *  readn
 *
 * Description:
 *  Read n bytes from a descriptor.
 * 
 * Parameters:
 *  fd   - [IN] socket fd.
 *  vptr - [IN] buffer for reading.
 *  n    - [IN] expected length of reading.
 *
 * Returns:
 *  >0, actual length of reading.
 *  =0, EOF.
 *  <0, error.
 *
 * NOTE:
 *  This function is a wrapper function of system call 
 *  `read`, which is carefully designed for network IO.
 */
ssize_t readn(int fd, void *vptr, size_t n);

/* 
 * Name:
 *  writen
 *
 * Description:
 *  Write n bytes to a descriptor.
 * 
 * Parameters:
 *  fd   - [IN] socket fd.
 *  vptr - [IN] buffer for writing.
 *  n    - [IN] expected length of writing.
 *
 * Returns:
 *  >0, actual length of writing.
 *  =0, EOF.
 *  <0, error.
 *
 * NOTE:
 *  This function is a wrapper function of system call 
 *  `write`, which is carefully designed for network IO.
 */
ssize_t writen(int fd, const void *vptr, size_t n);

/* 
 * Name:
 *  read_unix_sock
 *
 * Description:
 *  This function is used to read `TaskInfo` from a socket.
 *
 * Parameters:
 *  fd        - [IN]  an opening unix socket descriptor.
 *  task_info - [OUT] the task information.
 *
 * Returns:
 *  true, success.
 *  false, failure.
 *
 * NOTE:
 *  None.
 */
bool read_sock(int fd, TaskInfo &task_info);

/* 
 * Name:
 *  write_unix_sock
 *
 * Description:
 *  This function is used to write `TaskInfo` to a socket.
 *
 * Parameters:
 *  fd        - [IN] an opening unix socket descriptor.
 *  task_info - [IN] the task information.
 *
 * Returns:
 *  true, success.
 *  false, failure.
 *
 * NOTE:
 *  None.
 */
bool write_sock(int fd, TaskInfo &task_info);

#endif
