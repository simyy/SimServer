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
    pid_t pid;

	struct pollfd      client[POLL_SIZE];
	struct sockaddr_in client_addr;
	ssize_t n;

	struct pool *m_pool;

	//m_pool = createPool(2048);

	client[0].fd = fd;
	client[0].events = POLLIN;

	for(i = 1; i < POLL_SIZE; ++i){ //don't rewrite on client[1]
		client[i].fd = -1;
	}

	max = 0;
	len = sizeof(struct sockaddr);
	while(1){
		nready = poll(client, max+1, 1000);
        if (nready < 0) {
            perror("poll error!\n");
            break;
        }
        else if (nready == 0) {
            perror("time out\n");
            continue;
        }
		if(client[0].revents & POLLIN){
			conn = accept(fd, (struct sockaddr*)&client_addr, (unsigned int*)&len);
            if (conn == -1) {
                perror("accept error");
            }
			
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

	    	    pid = fork();
	    	    if(pid == 0){
	    	    	close(fd);
                    printf("process %d forked\n", getpid());
	    	    	handleRequest(sockfd);
	    	    	exit(0);
	    	    }
	    	    close(sockfd);
	    	    waitpid(-1, NULL, WNOHANG);

			    if(--nready <= 0)
				    continue;
			}
		}
	}
	
	//destroyPool(m_pool);	
	return 0;
}
