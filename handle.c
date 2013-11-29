#include <stdio.h>
#include <error.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "handle.h"
#include "RespModule.h"
#include "ReqModule.h"
#include "pool.h"
#include "Util.h"

void handleRequest(int fd)
{
	printf("\n\n===========================\n");
	struct pool* m_pool;
	int  ret;

	m_pool = createPool(500);

	struct ReqInfo* reqInfo;
	reqInfo = (struct ReqInfo*)palloc(m_pool, sizeof(struct ReqInfo));
	InitReqInfo(reqInfo);

	GetReqContent(fd, reqInfo, m_pool);
	if(reqInfo->resource != NULL){
		printf("status: %d\n", reqInfo->status);
		writeLog(reqInfo->resource);
		printf("recv buffer: %s\n", reqInfo->resource);

		//if(reqInfo->pageType == STATIC)
		if(reqInfo->pageType == 0)
			printf("static page..\n");
		else
			printf("dynamic page..\n");
		ReturnResponse(fd, reqInfo);
	}

	destroyPool(m_pool);
	close(fd);

	printf("===========================\n\n");
}
