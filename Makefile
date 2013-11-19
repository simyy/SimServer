webServ : webServ.o handle.o ReqModule.o RespModule.o Resource.o Util.o pool.o
	gcc -g -o webServ webServ.o handle.o ReqModule.o RespModule.o Resource.o  Util.o pool.o

webServ.o : webServ.c handle.h
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

clean :
	rm *.o webServ
