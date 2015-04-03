/*
 * select_module.c
 * ===============
 * (c) Copyright Xianda Yu 2013/12/16
 * the select module
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#include "event.h"
#include "../ReqModule.h"
#include "../RespModule.h"
#include "../log.h"
#include "../Util.h"

int select_process(int fd)
{
	int i;
	int len;
	int flag;
	int conn;
	int maxi;
	int maxfd;
	int sockfd;
	int nready;
    pid_t pid;
	 
	fd_set rset, allset;
	int client[FD_SETSIZE];
	struct sockaddr_in client_addr;

	//struct pool *m_pool;
	
	//m_pool = createPool(2048);

	len = sizeof(struct sockaddr);

	maxfd = fd;
	for(i = 0; i < FD_SETSIZE; ++i){
		client[i] = -1;
	}

	FD_ZERO(&allset);
	FD_SET(fd, &allset);

	for(;;){
		rset = allset;
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);
		
		//server sock_fd, then receive
		if(FD_ISSET(fd, &rset)){
			conn = accept(fd, (struct sockaddr*)&client_addr, (unsigned int*)&len);
			
			for(i = 0; i < FD_SETSIZE; ++i){
				if(client[i] < 0)
					client[i] = conn;
					break;
			}			

			if(i == FD_SETSIZE){
				perror("too many clients!\n");
				break;
			}

			FD_SET(conn, &allset);
			if(conn > maxfd)
				maxfd = conn;

			if(i > maxi)
				maxi = i;

			if(--nready <= 0)
				continue;
		}

		for(i = 0; i <= maxi; ++i){
			if((sockfd = client[i]) < 0)
				continue;

			if(FD_ISSET(sockfd, &rset)){
                pid = fork();
                if (pid == 0) {
                    close(fd);
                    printf("process %d forked\n", getpid());
	    		    handleRequest(sockfd);
	    		    exit(0);
                }
                close(sockfd);
	    	    waitpid(-1, NULL, WNOHANG);

				FD_CLR(sockfd, &allset);	

				if(--nready <= 0)
					break;
			}
		}
	}

	return 0;
}	
