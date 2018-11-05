#ifndef _SEARCH_CLIENT_H
#define _SEARCH_CLIENT_H

#include "task_info.h"


/*
 * Name: 
 *  searchc
 *
 * Description:
 *  This function is the entry of search engine client.
 *
 * Parameters:
 *  request - [IN] the request task information.
 *
 * Returns:
 *  None.
 *
 * NOTE:
 *  None.
 */
int searchc(TaskInfo &request);

#endif
