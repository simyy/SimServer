/*
 * A Simple Web Server
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <error.h>

#define PORT 9000

#define EOL "\r\n"
#define EOL_SIZE 2

int recv_new(int fd, char *buffer); 
void send_new(int fd, char *msg);

int main(int argc, char* argv[])
{
	int serv_fd;
	int client_fd;

	int ret;

	pid_t pid;
	struct sockaddr_in serv_addr;
	struct sockaddr_in client_addr;
	int    len = sizeof(struct sockaddr_in);

	serv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(serv_fd < 0){
		perror("create socket fail !\n");
		exit(1);
	}
	
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);

	int on = 1;
	ret = setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
	if(ret < 0){
		perror("setsockopt fail !\n");
		exit(1);
	}
	
	ret = bind(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(ret < 0){
		perror("bind fail !\n");
		exit(1);
	}

	ret = listen(serv_fd, 5);
	if(ret < 0){
		perror("listen fail !\n");
		exit(1);
	}

	while(1){
		client_fd = accept(serv_fd, (struct sockaddr*)&client_addr, &len);
		if(client_fd < 0){
			perror("accept fail !\n");
			continue;
		}
		char buffer[200];
		recv_new(client_fd, buffer);
		printf("recv buffer: %s\n", buffer);
		char content[] = "<head><head><title>index.html</title></head><body>hello world!</body>";
		char response[512];
		sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n%s", strlen(content), content);
		send(client_fd, response, sizeof(response), 0);
		close(client_fd);
	}

	return 0;
}

int recv_new(int fd, char *buffer) {
	char *p = buffer; // Use of a pointer to the buffer rather than dealing with the buffer directly
	int eol_matched = 0; // Use to check whether the recieved byte is matched with the buffer byte or not
	while (recv(fd, p, 1, 0) != 0) // Start receiving 1 byte at a time
	{
		if (*p == EOL[eol_matched]) // if the byte matches with the first eol byte that is '\r'
		{
			++eol_matched;
			if (eol_matched == EOL_SIZE) // if both the bytes matches with the EOL
			{
				*(p + 1 - EOL_SIZE) = '\0'; // End the string
				return (strlen(buffer)); // Return the bytes recieved
			}
		} else {
			eol_matched = 0;
		}
		p++; // Increment the pointer to receive next byte
	}
	return (0);
}
