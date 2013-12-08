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

/* set blocking  */
void setnonblocking(int sock);

 /* epoll process  */
int epoll_process(int fd);
 
 #endif
