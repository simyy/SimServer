/*
 * Resource.h
 * ==========
 * (c) Copyright Xianda Yu 2013/11/4
 * Check and get the resource which is requested
 */

#ifndef _RESOURCE_H
#define _RESOURCE_H

#include "Util.h"

int CheckResource(struct ReqInfo* reqInfo);
int ReturnResource(int client_fd, int res_fd, struct ReqInfo* reqInfo);
int ReturnErrorMsg(int conn, struct ReqInfo* reqinfo);
ssize_t WriteLine(int fd, char* buffer, size_t n);

#endif
