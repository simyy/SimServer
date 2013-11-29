#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1000

int main(void)
{
	char *buf, *p;
	char name[MAXLINE], password[MAXLINE], content[MAXLINE];

	/* get arguments*/
	if((buf = getenv("QUERY_STRING")) != NULL){
		p = strchr(buf, '&');
		*p = '\0';
		strcpy(name, buf);
		strcpy(password, ++p);
	}

	sprintf(content, "Welcome ~~~: %s, and your password : %s\r\n", name, password);
	sprintf(content, "%s\r\n", content);
	sprintf(content, "%sThanks for your visting !\r\n", content);

	printf("Content-length: %d\r\n", strlen(content));
	printf("Content-type: text/html\r\n\r\n");
	printf("%s", content);
	
	fflush(stdout);

	return 0;
}
