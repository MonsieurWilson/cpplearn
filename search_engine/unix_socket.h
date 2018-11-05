#ifndef _UNIX_SOCKET_H
#define _UNIX_SOCKET_H

#include "config.h"


/*
 * Name:
 *  unix_serv_listen
 *
 * Description:
 *  This function will initialize a unix socket and return a 
 *  binded socket descriptor. (Server endpoint)
 *
 * Parameters:
 *  sock_name - [IN] name of unix socket.
 *
 * Returns:
 *  postive integer,  a listening socket descriptor.
 *  negative integer,  errno number.
 *
 * NOTE:
 *  The returned socket descriptor is non-blocked.
 */
int unix_serv_listen(const char *sock_name);

/*
 * Name:
 *  unix_serv_accept
 *
 * Description:
 *  This function will block until the client connect the
 *  listening socket, then return the socket descriptor
 *  connected and user id of caller. (Server endpoint)
 *
 * Parameters:
 *  listen_fd - [IN] a listening socket descriptor.
 *
 * Returns:
 *  postive integer, a connected client socket descriptor.
 *  negative integer,  errno number.
 *
 * NOTE:
 *  The returned socket descriptor is non-blocked.
 */
int unix_serv_accept(int listen_fd);

/* 
 * Name:
 *  unix_cli_connect
 *
 * Description:
 *  This function is used to connect to a unix socket server.
 *  (Client endpoint)
 *
 * Parameters:
 *  sock_name - [IN] name of unix socket.
 *
 * Returns:
 *  postive integer,  a connected server socket descriptor.
 *  negative integer,  errno number.
 *
 * NOTE:
 *  None.
 */
int unix_cli_connect(const char *sock_name);

#endif
