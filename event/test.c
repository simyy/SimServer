#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "event.h"

void sendBuffer(int conn)
{
	char buffer[100];

	strcpy(buffer, "hello");

	send(conn, buffer, strlen(buffer), 0);
}

int main()
{
	int fd;
	int on;
	int rs;
	int len;
	int conn;
	char buffer[100];
	int flag1, flag2;
	struct sockaddr_in serv_addr, clt_addr;
	struct timeval timeout;

	int epfd;
	struct epoll_event ev;
	struct epoll_event events[1024];
	void (*doSomething)(int);

	fd = socket(AF_INET, SOCK_STREAM, 0);

	on = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	//timeout.tv_sec  = 5;
	//timeout.tv_usec = 0;
	//setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

	bzero(&serv_addr, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port   = htons(9090);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	rs = bind(fd, (struct sockaddr*)(&serv_addr), sizeof(struct sockaddr));
	if(rs < 0){
		perror("");
		close(fd);
		return -1;
	}

	rs = listen(fd, 5);
	if(rs < 0){
		perror("");
		close(fd);
		return -1;
	}
	
	len = sizeof(clt_addr);

	epoll_init(fd, epfd, &ev, events);

	doSomething = sendBuffer; 
	while(1){
		epoll_process(fd, epfd, &ev, events, doSomething);	
	}


	close(fd);

	return 0;
}

