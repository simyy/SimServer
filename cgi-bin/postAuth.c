#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1000

int main(void)
{
	char *buf, *p;
	int  length;
	char content[MAXLINE], data[MAXLINE];

	if((buf = getenv("CONTENT-LENGTH")) != NULL){
		length = atol(buf);
	}

	p = fgets(data, length+1, stdin);
	if(NULL == p){
		sprintf(content, "Something is wrong\r\n");
	}
	else{
		sprintf(content, "Info: %s\r\n", data);
	}

	printf("Content-length: %d\r\n", strlen(content));
	printf("Content-type: text/html\r\n\r\n");
	printf("%s", content);
	fflush(stdout);
	return 0;
}
