/*
 * epoll_module.c
 * ================
 * (c) Copyright XianDa Yu 2013/12/7
 * the epoll module
 *
 */
 
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>


#include "event.h"
#include "../ReqModule.h"
#include "../RespModule.h"
#include "../log.h"
#include "../Util.h"
 
void setnonblocking(int sock)
{
	int opts;
	opts = fcntl(sock, F_GETFL);
	 if(opts < 0){
		perror("fcntl(sock,GETFL)");
		exit(1);
	}
	opts = opts | O_NONBLOCK;
	if(fcntl(sock, F_SETFL, opts)<0){
		perror("fcntl(sock,SETFL,opts)");
		exit(1);
	}
}
 
int epoll_process(int fd)
{
	int i;
	int n;
	int nfds;
	int epfd;
	int len;
	int flag;
	int newfd;
	int conn;
	
	char buffer[1024];
	struct pool *m_pool;
	struct epoll_event ev;
	struct epoll_event events[1024];
	struct sockaddr_in client_addr;

	//m_pool = createPool(2048);
	epfd = epoll_create(1024);
	
	//close(fd);
	ev.data.fd = fd;
	ev.events = EPOLLIN|EPOLLET;
	flag = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    if(flag < 0){
		perror("epoll_ctl error !\n");
		return -1;
	}
	
	len = sizeof(struct sockaddr);
	while(1){
		nfds = epoll_wait(epfd, events, 512, 500);
		for(i = 0; i < nfds; ++i){
            if(events[i].data.fd == fd){
				conn = accept(fd, (struct sockaddr*)&client_addr, (unsigned int*)&len);
                if(conn < 0){
                    perror("accept error!\n");
                        continue;
                }
				setnonblocking(conn);
                ev.data.fd = conn;
                ev.events  = EPOLLIN|EPOLLET;                        
                epoll_ctl(epfd, EPOLL_CTL_ADD, conn, &ev);
            }
			else if((events[i].events & EPOLLERR) ||
					(events[i].events & EPOLLHUP)){
				close(events[i].data.fd);
				epoll_ctl(epfd, EPOLL_CTL_ADD, conn, &ev);
				continue;	
			}
			else if(events[i].events&EPOLLIN){
            	/* receive request */                        
				struct ReqInfo* reqInfo;
                if((newfd = events[i].data.fd) < 0)
                    continue; 
				printf("read fd: %d\n", newfd);
				//reqInfo = (struct ReqInfo*)palloc(m_pool, sizeof(struct ReqInfo));
				reqInfo = (struct ReqInfo*)malloc(sizeof(struct ReqInfo));
               	InitReqInfo(reqInfo); 
				//flag = GetReqContent(newfd, reqInfo, m_pool);
				flag = GetReqContent(newfd, reqInfo);
				if(flag != 0){
					if(flag == -1)
						printf("select timeout\n");
					if(flag == 1)
						printf("select fail\n");

				}

				reqInfo->fd = newfd;
				ev.data.fd = newfd;	
				ev.data.ptr = reqInfo;
				ev.events = EPOLLOUT|EPOLLET;
				
				epoll_ctl(epfd, EPOLL_CTL_MOD, newfd, &ev);        
            }
		   	else if(events[i].events&EPOLLOUT){
             	/* send respost */
            	if((newfd = events[i].data.fd) < 0)
                    continue;
				printf("write fd: %d\n", newfd);
				struct ReqInfo* reqInfo = (struct ReqInfo*)events[i].data.ptr;
				if(reqInfo->resource != NULL){
					printf("status: %d\n", reqInfo->status);
					writeLog(reqInfo->resource);
					printf("recv buffer: %s\n", reqInfo->resource);
					
					if(reqInfo->pageType == STATIC)
						printf("static page...\n");
					else
						printf("dynamic page...\n");
					ReturnResponse(reqInfo->fd, reqInfo);
					close(reqInfo->fd);
				}
				ev.data.fd = -1;
				epoll_ctl(epfd, EPOLL_CTL_DEL, newfd, &ev);
            }

		}
	}

	//destroyPool(m_pool);
	return 0;
}
