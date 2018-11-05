#ifndef _SEARCH_CORE_H
#define _SEARCH_CORE_H

#include "task_info.h"
#include "config.h"


/*
 * Name: 
 *  search_core
 *
 * Description:
 *  This function is the entry of core search process.
 *
 * Parameters:
 *  request - [IN]  the task information by reference. 
 *
 * Returns:
 *  None.
 *
 * NOTE:
 *  None.
 */
void search_core(TaskInfo &request);

#endif
