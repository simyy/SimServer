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
#define LISTEN_NUM 1024

#define USEEPOLL 0 
#define USESELECT 1
#define USEDAEMON 0

int main(int argc, char* argv[])
{
	int i;
	int serv_fd;
	int client_fd;
	int ret;

	if(USEDAEMON)
		init_daemon();

	pid_t pid;
	struct sockaddr_in serv_addr;
	struct sockaddr_in client_addr;
	int    len = sizeof(struct sockaddr_in);
	/* Create Socket */
	serv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(serv_fd < 0){
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
	if(ret < 0){
		perror("setsockopt Reuse fail !\n");
		exit(1);
	}

	/* Disable the Nagle(TCP No Delay) algorithm */
	/*
	ret = setsockopt(serv_fd, IPPROTO_TCP, TCP_NODELAY, (char*)&on, sizeof(int));
	if(ret < 0){
		perror("setsockopt Nodelay fail !\n");
		exit(1);
	}
	*/
	/* Bind address to Socket */
	ret = bind(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(ret < 0){
		perror("bind fail !\n");
		exit(1);
	}
	
	/* Start to listen socket */
	ret = listen(serv_fd, LISTEN_NUM);
	if(ret < 0){
		perror("listen fail !\n");
		exit(1);
	}

	if(USESELECT){
		select_process(serv_fd);
		return 0;
	}

	if(USEEPOLL){
/*
		for(i = 0; i < 2; ++i){
			pid = fork();

			switch(pid){
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
*/		
		setnonblocking(serv_fd);
		epoll_process(serv_fd);
		return 0;
	}
	
	/* Loop to accept and handle connections */
	while(1){
		client_fd = accept(serv_fd, (struct sockaddr*)&client_addr, &len);
		if(client_fd < 0){
			perror("accept fail !\n");
			continue;
		}
	/*	
		pid = fork();
		if(pid == 0){
			close(serv_fd);
			handleRequest(client_fd);
			exit(0);
		}
			
		close(client_fd);
		waitpid(-1, NULL, WNOHANG);
*/
		handleRequest(client_fd);	
	}

	close(serv_fd);
	return 0;
}
