/*
 *
 *
 *
 *
 */

#ifndef  _EVENT_H
#define _EVENT_H

/*  */
int epoll_init(int fd, int epfd, struct epoll_event ev, struct epoll_event events[1024]);

/*  */
int epoll_process(int fd, int epfd, struct epoll_event ev,struct epoll_event events[1024]);


#endif
