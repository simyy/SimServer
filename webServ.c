/*
 * A Simple Web Server
 * ===================
 * (c) Copyright Xianda Yu 2013/10/31
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <error.h>
#include <netinet/tcp.h>

#include "handle.h"
#include "pool.h"
#include "Util.h"
#include "event/event.h"

#define PORT 9000
#define LISTEN_NUM 2048 

#define USEEPOLL  1 
#define USESELECT 0 
#define USEPOLL   0 

#define WORKERS   2

#define USEDAEMON 0

int main(int argc, char* argv[])
{
	int    i;
	int    ret;
	int    serv_fd;
	int    client_fd;
	pid_t  pid;
	struct sockaddr_in serv_addr;
	struct sockaddr_in client_addr;
	int    len = sizeof(struct sockaddr_in);

    /* as a daemon process*/
	if (USEDAEMON)
		init_daemon();

	/* Create Socket */
	serv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_fd < 0) {
		perror("create socket fail !\n");
		exit(1);
	}
	
	/* Init server address */
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);
	
	/* Set address/port reuse in diffrent Socket */	
	int on = 1;
	ret = setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
	if (ret < 0) {
		perror("setsockopt Reuse fail !\n");
		exit(1);
	}

	/* Disable the Nagle(TCP No Delay) algorithm */
	ret = setsockopt(serv_fd, IPPROTO_TCP, TCP_NODELAY, (char*)&on, sizeof(int));
	if (ret < 0) {
		perror("setsockopt Nodelay fail !\n");
		exit(1);
	}

	/* Bind address to Socket */
	ret = bind(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (ret < 0) {
		perror("bind fail !\n");
		exit(1);
	}
	
	/* Start to listen socket */
	ret = listen(serv_fd, LISTEN_NUM);
	if (ret < 0) {
		perror("listen fail !\n");
		exit(1);
	}

    /* select a event module */
	if (USESELECT) {
	    // use select module
        printf("Event: select\n");
		select_process(serv_fd);
		return 0;
	}
    else if (USEPOLL) {
	    // use poll module 
        printf("Event: poll\n");
		poll_process(serv_fd);
		return 0;
	}
    else if (USEEPOLL) {
	    // use epoll module 
        printf("Event: epoll\n");
		for (i = 0; i < WORKERS-1; ++i) {
			pid = fork();

			switch(pid) {
			case -1:
				printf("fork error!\n");
				return -1;
			case 0:
				epoll_process(serv_fd);
			default:
				printf("process %d forked\n", pid);
				break;

			}
		}
		setnonblocking(serv_fd);
		epoll_process(serv_fd);
		return 0;
	}
    else {
	    // normal module
	    while(1) {
	    	client_fd = accept(serv_fd, (struct sockaddr*)&client_addr, &len);
	    	if (client_fd < 0) {
	    		perror("accept fail !\n");
	    		continue;
	    	}
	    	pid = fork();
	    	if (pid == 0) {
	    		close(serv_fd);
                printf("process %d forked\n", getpid());
	    		handleRequest(client_fd);
	    		exit(0);
	    	}
	    	close(client_fd);
	    	waitpid(-1, NULL, WNOHANG);
	    }
    }
	close(serv_fd);
	return 0;
}
