USER=yaya
HOST=blondie.arcib.org:8083

all: lrl_web.cgi lrl_web.cpp lrl_web.html

lrl_web.cgi: lrl_web.o do_posix_spawn.o
	g++ -g -o lrl_web.cgi lrl_web.o do_posix_spawn.o -l cgicc

lrl_web.o: lrl_web.cpp
	g++ -g -c lrl_web.cpp

do_posix_spawn.o: do_posix_spawn.c
	gcc -g -c do_posix_spawn.c

lrl_web.cpp: lrl_web.m4
	m4 --define cgicpp=1 --define lrlwebuser=$(USER) --define lrlwebhost=$(HOST) \
	   --define lrlwebcgi=lrl_web.cgi --define lrlwebtmp=tmp \
          lrl_web.m4 > lrl_web.cpp 

lrl_web.html: lrl_web.m4
	m4 --define lrlwebuser=$(USER) --define lrlwebhost=$(HOST) \
	  --define lrlwebcgi=lrl_web.cgi --define lrlwebtmp=tmp \
	  lrl_web.m4 > lrl_web.html 

clean:
	rm -rf  lrl_web.cgi lrl_web.cpp lrl_web.html

