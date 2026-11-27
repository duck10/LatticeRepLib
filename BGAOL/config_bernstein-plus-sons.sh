#!/bin/sh
HTTPDSERVER=www.bernstein-plus-sons.com;export HTTPDSERVER
HTDOCS=${HOME}/public_html/software/bgaol;export HTDOCS
SEARCHURL=http://www.bernstein-plus-sons.com/software/bgaol;export SEARCHURL
CGIPATH=http://www.bernstein-plus-sons.com/cgi-bin;export CGIPATH
BINDEST=/usr/home/yaya/public_html/cgi-bin; export BINDEST
CGIBIN=$BINDEST; export CGIBIN
FC=/usr/local/bin/gfortran; export FC
FFLAGS="-O3 -static"; export FFLAGS
CGIBINEXT=/cgi-bin; export CGIBINEXT
CGIMETHOD=POST; export CGIMETHOD
HTDOCSEXT=/software/bgaol; export HTDOCSEXT

