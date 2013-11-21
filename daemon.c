/*
 * daemon.c
 * =========
 * (c) Copyright Xianda Yu 2013/11/21
 * linux daemon process
 *
 */
#include "Util.h"

#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

void init_daemon(void)
{
    int pid;
    int i;
    if(pid=fork())
        exit(0);       
    else if(pid< 0)
        exit(1);
    setsid();          
    
    if(pid=fork())
        exit(0);     
    else if(pid< 0)
        exit(1);
    
    for(i=0;i< NOFILE;++i)  
        close(i);

    chdir("./");      
    umask(0);   
    return;
}
