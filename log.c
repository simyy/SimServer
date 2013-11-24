/*
 * log.c
 * =======
 * (c) Copyright Xianda Yu 2013/11/23
 *
 */

#include <stdio.h>
#include <time.h>

#define LOGNAME "access.log"

void getTime(char* tm)
{
	time_t timeval;
	struct tm* timeInfo;

	(void)time(&timeval);
	timeInfo = localtime(&timeval);
	strftime(tm, 80 , "%Y-%m-%d %I:%M:%S ", timeInfo );
}

void writeLog(char* buf)
{
	char content[100];
	FILE* log_fp = NULL;

	log_fp = fopen(LOGNAME, "a+");	
	getTime(content);
	sprintf(content, "%s%s\n", content, buf);
	fwrite(content, strlen(content), 1, log_fp);
	fclose(log_fp);
}

