#!/bin/sh
HTTPDSERVER=`hostname -f`/~${USER};export HTTPDSERVER
SEARCHURL=http://`hostname -f`/~${USER}/bgaol;export SEARCHURL
CGIPATH=http://`hostname -f`/cgi-bin/cgiwrap/${USER};export CGIPATH
FC=g95; export FC
FFLAGS="-O3"; export FFLAGS

