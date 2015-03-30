/*
 * Util.h
 * ======
 * (c) Copyright Xianda Yu 2013/11/3
 * some commen functions for web server
 */

#ifndef _UTIL_H
#define _UTIL_H

#define ROOT_DIRECTORY "/home/y/simple-web-server/SimServer/www"
extern char **environ; /* defined by libc */


/* User difined info */
enum Req_Method {GET, HEAD, UNSUPPORTED};
enum Req_Type   {SIMPLE, FULL};
enum Req_PageType {STATIC, DYNAMIC};

struct ReqInfo{
	enum Req_Method method;
	enum Req_Type   type;

//	int pageType;//0-static 1-dynamic
	enum Req_PageType pageType;	

	char*           resource;
	int             status;

	int 			fd;
};

/* Init and Free request info */
void InitReqInfo(struct ReqInfo* reqInfo);
void FreeReqInfo(struct ReqInfo* reqInfo);

/* as trim in JAVA */
void Trim(char* buffer);

/* linux daemon process */
void init_daemon();

/*  */
int Pipe(int pipefd[2]);

#endif
