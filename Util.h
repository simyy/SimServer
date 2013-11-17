/*
 * Util.h
 * ======
 * (c) Copyright Xianda Yu 2013/11/3
 * some commen functions for web server
 */

#ifndef _UTIL_H
#define _UTIL_H

#define ROOT_DIRECTORY "/home/yu/Desktop/webServ/www"

/* User difined info */
enum Req_Method {GET, HEAD, UNSUPPORTED};
enum Req_Type   {SIMPLE, FULL};

struct ReqInfo{
	enum Req_Method method;
	enum Req_Type   type;

	char*           resource;
	int             status;
};
/* Init and Free request info */
void InitReqInfo(struct ReqInfo* reqInfo);
void FreeReqInfo(struct ReqInfo* reqInfo);

/* as trim in JAVA */
void Trim(char* buffer);

#endif
