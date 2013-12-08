webServ : webServ.o handle.o ReqModule.o RespModule.o Resource.o Util.o pool.o daemon.o cgi log.o epoll_module.o 
	gcc -g -o webServ webServ.o handle.o ReqModule.o RespModule.o Resource.o  Util.o pool.o daemon.o log.o epoll_module.o

webServ.o : webServ.c handle.h event.h
	gcc -g -c  webServ.c 

handle.o : handle.c handle.h Util.h RespModule.h ReqModule.h
	gcc -g -c  handle.c

ReqModule.o : ReqModule.c ReqModule.h Util.h
	gcc -g -c ReqModule.c

RespModule.o : RespModule.c RespModule.h Util.h Resource.h
	gcc -g -c RespModule.c

Resource.o : Resource.c Resource.h Util.h
	gcc -g -c Resource.c

pool.o : pool.c pool.h
	gcc -g -c pool.c

Util.o : Util.c Util.h
	gcc -g -c Util.c

daemon.o : daemon.c
	gcc -g -c daemon.c

log.o : log.c 
	gcc -g -c log.c

cgi:
	(cd cgi-bin; make)

epoll_module.o : epoll_module.c event.h 
	gcc -g -c epoll_module.c

clean :
	rm *.o webServ 
	(cd cgi-bin; make clean)
