.PHONY : all
all : epoll_module.o select_module.o poll_module.o

epoll_module.o : epoll_module.c event.h ../ReqModule.h ../RespModule.h ../log.h ../Util.h
	gcc -g -c epoll_module.c

select_module.o : select_module.c event.h ../ReqModule.h ../RespModule.h ../log.h ../Util.h
	gcc -g -c select_module.c

poll_module.o : poll_module.c event.h ../ReqModule.h ../RespModule.h ../log.h ../Util.h
	gcc -g -c poll_module.c

clean:
	rm *.o
