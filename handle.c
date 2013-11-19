#include <stdio.h>
#include <error.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "Util.h"
#include "handle.h"
#include "RespModule.h"
#include "ReqModule.h"
#include "pool.h"

void handleRequest(int fd)
{
	struct pool* m_pool;
	int  ret;

	m_pool = createPool(500);

	struct ReqInfo* reqInfo;
	//reqInfo = (struct ReqInfo*)malloc(sizeof(struct ReqInfo));
	reqInfo = (struct ReqInfo*)palloc(m_pool, sizeof(struct ReqInfo));
	InitReqInfo(reqInfo);

	GetReqContent(fd, reqInfo);
	printf("status: %d\n", reqInfo->status);
	printf("recv buffer: %s\n", reqInfo->resource);
	ReturnResponse(fd, reqInfo);

	destroyPool(m_pool);
	close(fd);
}
