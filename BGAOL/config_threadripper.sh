#!/bin/sh
host_name=flops.arcib.org:8084
HTTPDSERVER=${host_name};export HTTPDSERVER
SEARCHURL=http://${host_name}/bgaol;export SEARCHURL
CGIPATH=http://${host_name}/cgi-bin/;export CGIPATH
BINDEST=/var/www/cgi-bin; export BINDEST
CGIBIN=$BINDEST; export CGIBIN
FC="gfortran --static"; export FC
FFLAGS="-O3"; export FFLAGS
CGIBINEXT=/cgi-bin; export CGIBINEXT
CGIMETHOD=POST; export CGIMETHOD
HTDOCS=/var/www/html/bgaol; export HTDOCS
HTDOCSEXT=/bgaol; export HTDOCSEXT

