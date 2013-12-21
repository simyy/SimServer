/*
 * event.h
 * ===============
 * (c) Copyright XianDa Yu 2013/12/7
 * including all the events, such as
 * select/poll/epoll 
 *
 */
 
#ifndef _EVENT_H
#define _EVENT_H

//#define FD_SETSIZE 1024

/* set blocking  */
void setnonblocking(int sock);

/* epoll process  */
int epoll_process(int fd);

/* select process */
int select_process(int fd); 

/* poll process */
int poll_process(int fd);

#endif
