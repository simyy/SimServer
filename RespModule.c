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

	/*  */
	res_fd = CheckResource(reqInfo); 
	if(reqInfo->status == 200 && res_fd < 0){
		if(errno == EACCES)
			/* Pemission denied */
			reqInfo->status = 401;
		else
			/* Not Found */
			reqInfo->status = 404;
	}
	
	if(reqInfo->type == FULL)
		OutputHttpHeaders(fd, reqInfo);
		
	if(reqInfo->status == 200){
		flag = ReturnResource(fd, res_fd, reqInfo);
		if(flag == -1){
			perror("Return Resouce fail !\n");
			return -1;
		}
	}
	else
		ReturnErrorMsg(fd, reqInfo);

	if(res_fd > 0)
		close(res_fd);

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

int OutputHttpHeaders(int conn, struct ReqInfo* reqinfo)
{
	char buffer[100];
	sprintf(buffer, "HTTP/1.0 %d OK\r\n", reqinfo->status);
	WriteLine(conn, buffer, strlen(buffer));
	WriteLine(conn, "Server: WebServ v0.1\r\n", 24);
	WriteLine(conn, "Content-Type: text/html\r\n", 25);
	WriteLine(conn, "\r\n", 2);
	return 0;
}
