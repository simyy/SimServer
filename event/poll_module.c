/*
 * poll_module.c
 * ================
 * (c) Copyright XianDa Yu 2013/12/21
 * the poll module
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>

#include "event.h"
#include "../ReqModule.h"
#include "../RespModule.h"
#include "../log.h"
#include "../Util.h"

#define POLL_SIZE 1024

int poll_process(int fd)
{
	int i;
	int max;
	int len;
	int conn;
	int flag;
	int sockfd;
	int nready;

	struct pollfd      client[POLL_SIZE];
	struct sockaddr_in client_addr;
	ssize_t n;

	struct pool *m_pool;

	//m_pool = createPool(2048);

	client[0].fd = fd;
	client[0].events = POLLIN;

	for(i = 0; i < POLL_SIZE; ++i){
		client[i].fd = -1;
	}

	max = fd + 1;
	len = sizeof(struct sockaddr);
	while(1){
		nready = poll(client, max+1, 0);
		if(client[0].revents & POLLIN){
			conn = accept(fd, (struct sockaddr*)&client_addr, (unsigned int*)&len);
			
			for(i = 1; i < POLL_SIZE; ++i){
				if(client[i].fd < 0){
					client[i].fd = conn;
					break;
				}
			}

			if(i == POLL_SIZE){
				perror("too many clients!\n");
				break;
			}

			client[i].events = POLLIN;
			if(i > max)
				max = i;
			if(--nready <= 0)
				continue;
		}

		for(i = 1; i < max; ++i){
			if((sockfd = client[i].fd) < 0)
				continue;

			if(client[i].revents & POLLIN){
				struct ReqInfo* reqInfo;
				//reqInfo = (struct ReqInfo*)palloc(m_pool, sizeof(struct ReqInfo));
				reqInfo = (struct ReqInfo*)malloc(sizeof(struct ReqInfo));

				InitReqInfo(reqInfo);
				//flag = GetReqContent(sockfd, reqInfo, m_pool);
				flag = GetReqContent(sockfd, reqInfo);
				if(flag != 0){
					if(flag == -1)
						printf("select timeout\n");
					if(flag == 1)
						printf("select fail\n");
				}

				if(reqInfo->resource != NULL){
					printf("status: %d\n", reqInfo->status);
					writeLog(reqInfo->resource);
					printf("recv buffer: %s\n", reqInfo->resource);
					
					if(reqInfo->pageType == STATIC)
						printf("static page...\n");
					else
						printf("dynamic page...\n");
					ReturnResponse(sockfd, reqInfo);
				}
				close(sockfd);
				client[i].fd = -1;

				if(--nready <= 0)
					break;
			}
		}
	}
	
	//destroyPool(m_pool);	
	return 0;
}
