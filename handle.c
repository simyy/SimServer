#include <stdio.h>
#include <error.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "Util.h"
#include "handle.h"
#include "RespModule.h"
#include "ReqModule.h"

void handleRequest(int fd)
{
	char buffer[200];
	int  ret;

	struct ReqInfo* reqInfo;
	reqInfo = (struct ReqInfo*)malloc(sizeof(struct ReqInfo));
	InitReqInfo(reqInfo);

	GetReqContent(fd, reqInfo);
	printf("status: %d\n", reqInfo->status);
	printf("recv buffer: %s\n", reqInfo->resource);
	ReturnResponse(fd, reqInfo);

	close(fd);
}
