/*
 * Util.c
 * ======
 * (c) Copyright Xianda Yu 2013/11/3
 *
 */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "Util.h"

/* Initialize request information */
void InitReqInfo(struct ReqInfo* reqInfo)
{
	reqInfo->method   = UNSUPPORTED;
	reqInfo->type     = SIMPLE;
	reqInfo->resource = NULL;
	reqInfo->status   = 200;

	reqInfo->pageType = STATIC;
}

void FreeReqInfo(struct ReqInfo* reqInfo)
{
	if(reqInfo->resource != NULL){
		free(reqInfo->resource);
	}
}	

/*  */
void Trim(char* buffer)
{
	int n = 0;

	n = strlen(buffer) - 1;
	/* isalnum can judge  char and num */
	while(!isalnum(buffer[n]) && n >= 0){
		buffer[n--] == '\0';
	}
}

int Pipe(int pipefd[2])
{
	if(pipe(pipefd) == -1)
		return 0;
	return 1;
}
