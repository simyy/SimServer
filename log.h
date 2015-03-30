/*
 * log.h
 * ======
 * (c) Copyright Xianda Yu 2013/11/23
 * the log of web server
 * 
 */

#ifndef _LOG_H 
#define _LOG_H

/* get time */
void getTime(char* time);

/* write log to access.log */
void writeLog(char* buf);

#endif 
