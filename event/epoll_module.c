#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <errno.h>

#include "event.h"

int epoll_init(int fd, int epfd, struct epoll_event ev, struct epoll_event events[1024])
{
	int flag;

	epfd = epoll_create(1024);
	
	ev.data.fd = fd;
	ev.events = EPOLLIN|EPOLLET;
	flag = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
	if(flag < 0){
		perror("epoll_ctl error !\n");
		return -1;
	}

	return 0;
}

int epoll_process(int fd, int epfd, struct epoll_event ev,struct epoll_event events[1024])
{
	int i;
	int nfds;
	int conn;
	int len;
	int newfd;
	int n;

	char buffer[1024];

	struct sockaddr_in serv_addr;
	struct sockaddr_in client_addr;

	len = sizeof(struct sockaddr);

	nfds = epoll_wait(epfd, events, 1000, 500);
	
	for(i = 0; i < nfds; ++i){
		if(events[i].data.fd == fd){
			conn = accept(fd, (struct sockaddr*)&client_addr, (unsigned int*)&len);
			if(conn < 0){
				perror("accept error!\n");
				continue;
			}

			ev.data.fd = conn;
			ev.events  = EPOLLOUT|EPOLLET;			
			epoll_ctl(epfd, EPOLL_CTL_ADD, conn, &ev);
		}
		else if(events[i].events&EPOLLOUT){
			/* send respost */
			if((newfd = events[i].data.fd) < 0)
				continue;

			bzero(buffer, sizeof(buffer));
			/* do somethind with responst */

			flag = send(newfd, buffer, strlen(buffer), 0);
			if(flag < 0){
				perror("send error!\n");
				continue;
			}
		}
		else if(events[i].events&EPOLLIN){
			/* receive request */			
			if((newfd = events[i].data.fd) < 0)
				continue; 

			bzero(buffer, sizeof(buffer));
			if((n = read(newfd, buffer, 1024)) < 0){
				if(errno == ECONNRESET){
					close(newfd);
					events[i].data.fd = -1;
					printf("errno ECONRESET!\n");
				}
				else
					perror("readbuffer error!\n");
			}
			else if(n == 0){
				close(newfd);
				events[i].data.fd = -1;
				printf("client close!\n");
			}
			else{
				/* do something with buffer */
			}	
		}
	}	

	return 0;
}

