/*
 * RespModule.c
 * ===============
 * (c) Copyright Xianda Yu 2013/11/3
 */

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "RespModule.h"
#include "Util.h"
#include "Resource.h"

int ReturnResponse(int fd, struct ReqInfo* reqInfo)
{
	char c;
	int  res_fd;
	int flag;
	
	if (reqInfo->pageType == DYNAMIC) {
		servDynamic(fd, reqInfo);
		return 0;
	}

	/*  */
	res_fd = CheckResource(reqInfo); 
	if (reqInfo->status == 200 && res_fd < 0) {
		if(errno == EACCES)
			/* Pemission denied */
			reqInfo->status = 401;
		else
			/* Not Found */
			reqInfo->status = 404;
	}
	
	if (reqInfo->type == FULL)
		OutputHttpHeaders(fd, reqInfo);
	
	if (reqInfo->status == 200) {
		flag = ReturnResource(fd, res_fd, reqInfo);
		if(flag == -1){
			perror("Return Resouce fail !\n");
			return -1;
		}
	}
	else
		ReturnErrorMsg(fd, reqInfo);

	if (res_fd > 0)
		close(res_fd);

    if (reqInfo != NULL) {
        free(reqInfo);
        reqInfo = NULL;
    }

//	FreeReqInfo(reqInfo);
	
	/*
	char content[] = "<head><head><title>index.html</title></head><body>hello world!</body>";
	char response[512];
	sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n%s", strlen(content), content);
	flag = write(fd, response, sizeof(response));
	if(flag < 1){
		perror("write fail !\n");
	}
	*/
	return 0;
}

/* should use write once */
int OutputHttpHeaders(int conn, struct ReqInfo* reqinfo)
{
	char buffer[100];
	sprintf(buffer, "HTTP/1.0 %d OK\r\nServer: WebServ v0.1\r\nContent-Type: text/html\r\n\r\n", reqinfo->status);
//	sprintf(buffer, "HTTP/1.0 %d OK\r\n", reqinfo->status);
	WriteLine(conn, buffer, strlen(buffer));
	//WriteLine(conn, "Server: WebServ v0.1\r\n", 24);
	//WriteLine(conn, "Content-Type: text/html\r\n", 25);
	//WriteLine(conn, "\r\n", 2);
	return 0;
}

int servDynamic(int fd, struct ReqInfo* reqInfo)
{
	char buffer[200];
	sprintf(buffer, "HTTP/1.0 %d OK\r\n", reqInfo->status);
	WriteLine(fd, buffer, strlen(buffer));
	WriteLine(fd, "Server: WebServ v0.1\r\n", 24);

	if(reqInfo->method == GET)
		getDynamic(fd, reqInfo);
	else
		postDynamic(fd, reqInfo);
	return 0;
}

int getDynamic(int fd, struct ReqInfo* reqInfo)
{
	char* emptylist[] = { NULL };
	char* p = strstr(reqInfo->resource, "getAuth?");
	p += 8;	
	printf("---execv getAuth----\n");
	if(fork() == 0){
		setenv("QUERY_STRING", p, 1);
		dup2(fd, STDOUT_FILENO);
		execve("cgi-bin/getAuth", emptylist, environ);
	}
	return 0;
}

int postDynamic(int fd, struct ReqInfo* reqInfo)
{
	char* emptylist[] = { NULL };
	char* p = strstr(reqInfo->resource, "postAuth?");
	p += 9;	
	printf("---execv postAuth----\n");
	if(fork() == 0){
		setenv("QUERY_STRING", p, 1);
		dup2(fd, STDOUT_FILENO);
		execve("cgi-bin/postAuth", emptylist, environ);
	}

	return 0;
}
