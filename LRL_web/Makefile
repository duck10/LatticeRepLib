
USER=yaya
HOST=blondie.arcib.org:8083

all: nlrl_web.cgi lrl_web.cpp nlrl_web.html

nlrl_web.cgi: lrl_web.o do_posix_spawn.o
	g++ -g -o nlrl_web.cgi lrl_web.o do_posix_spawn.o -l cgicc


lrl_web.o: lrl_web.cpp
	g++ -g -c lrl_web.cpp

do_posix_spawn.o: do_posix_spawn.c
	gcc -g -c do_posix_spawn.c

lrl_web.cpp: lrl_web.m4
	m4 --define cgicpp=1 --define lrl_web_user=$(USER) \
	  --define lrl_web_host=$(HOST)  --define lrl_web_cgi=nlrl_web.cgi \
	  --define lrl_web_tmp=tmp --define lrl_web_sdbx=sandbox \
	  lrl_web.m4 > lrl_web.cpp

nlrl_web.html: lrl_web.m4
	m4 --define lrl_web_user=$(USER) --define lrl_web_host=$(HOST) \
	  --define lrl_web_cgi=nlrl_web.cgi --define lrl_web_tmp=tmp \
	  --define lrl_web_sdbx=sandbox \
	  lrl_web.m4 > nlrl_web.html 

clean:
	rm -rf  lrl_web.cgi lrl_web.cpp lrl_web.html

sandbox: nlrl_web.html nlrl_web.cgi
	cp nlrl_web.html ../lrl_web/
	cp nlrl_web.cgi ../cgi-bin/

built:  lrl_web.cpp nlrl_web.html
