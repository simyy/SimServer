/*
 * Resource.c
 * ==========
 * (c) Copyright Xianda Yu 2013/11/4
 * 
 */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "Resource.h"

/*   */
int CheckResource(struct ReqInfo* reqInfo)
{
	if(reqInfo->resource == NULL)
		return  -1;
	char path[1000];
	bzero(path, 1000);
	memcpy(path, ROOT_DIRECTORY, sizeof(ROOT_DIRECTORY));
		
	strcat(path, reqInfo->resource);
	return	open(path, O_RDONLY);	
}

int ReturnResource(int client_fd, int res_fd, struct ReqInfo* reqInfo)
{
	char c;
	int  n;	

	while((n = read(res_fd, &c, 1))){
		if(n < 0){
			perror("Read file error !\n");
			return -1;
		}
		if(write(client_fd, &c, 1) <= 0){
			perror("Send file error !\n");
			return -1;
		}
	}
	return 0;
}

/* Returns an error message  */
int ReturnErrorMsg(int conn, struct ReqInfo * reqinfo)
{
	char buffer[100];
	sprintf(buffer, "<HTML>\n<HEAD>\n<TITLE>Server Error %d</TITLE>\n</HEAD>\n\n", reqinfo->status);
	WriteLine(conn, buffer, strlen(buffer));
	sprintf(buffer, "<BODY>\n<H1>Server Error %d</H1>\n", reqinfo->status);
	WriteLine(conn, buffer, strlen(buffer));
   	sprintf(buffer, "<P>The request could not be completed.</P>\n</BODY>\n</HTML>\n");
  	WriteLine(conn, buffer, strlen(buffer));
	return 0;
}

/* Write a Line to a socket */
ssize_t WriteLine(int fd, char* buffer, size_t n)
{
	size_t  start = 0;
	ssize_t m;

    while(start < n){
		m = write(fd, buffer, n - start);
		if(m <= 0){
			if(errno == EINTR)
				m = 0;
			else{
				perror("Write error !\n");
				return -1;
			}	
		}

		start  += m;
		buffer += m;
	}	

	return n;
}

