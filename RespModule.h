/*
 * RespModule.h
 * ===============
 * (c) Copyright Xianda Yu 2013/11/3
 * Consturct response functions
 */

#ifndef _RESP_CONSTRUCT_H
#define _RESP_CONSTRUCT_H

#include "Util.h"

int ReturnResponse(int fd, struct ReqInfo* reqInfo);
int OutputHttpHeaders(int conn, struct ReqInfo* reqinfo);

int servDynamic(int fd, struct ReqInfo* reqInfo);
int getDynamic(int fd, struct ReqInfo* reqInfo);
int postDynamic(int fd, struct ReqInfo* reqInfo);

#endif
