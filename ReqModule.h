/*
 * ReqModule.h
 * ===============
 * (c) Copyright XianDa Yu 2013/11/2
 * Interface to handle request
 */

#ifndef _REQUEST_H
#define _REQUEST_H

#include "Util.h"
#include "pool.h"

#define MAX_REQ_LINE 1024


/* The functions for request handle */
int     ParseHttpHeader(char *buffer, struct ReqInfo* reqInfo, struct pool* m_pool);
int     GetReqContent(int fd, struct ReqInfo* reqInfo, struct pool* m_pool);
ssize_t ReadLine(int fd, char* buffer, size_t len);

#endif
