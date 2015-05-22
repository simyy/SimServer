#simpleServer
>This is a simple web server writing in C on Linux.
>Build in 2013, Copyright Xianda Yu, Based on Paul Griffiths 1999.
>Using socket to build a web server in c, and multiprocess to handle with some requests at the same time.

##Install
istall command:
```
make
```
clean unsource file:
```
make clean
```

##Update log
*  2013/11/19 : 
  *  dd memory pool function.
  *  fix bug in pool.		
*  2013/11/21 :
  *  add a function of linux daemon process.
*  2013/11/24 :
  *  add log function.
  *  add cgi, but has error.
*  2013/11/29 :
  *  fix bug in pool.
  *  fix some waring.	
*  2013/11/30
  *  implement cgi get function
*  2013/12/3
  *  fix bug in cgi
*  2013/12/6
add event
*  2013/12/16
  *  add select module and fix select module
  *  move all event module to event files
*  2013/12/21
  *  add poll module
*  2015/3/1
  * fix bug in pool
  * make all event module running in processes
