#!/bin/bash
export QUERY_STRING="df=lrl_web.dat"
export USER=$1
export TMPDIR=$2
lrl_web_prefix_num=`/home/${USER}/public_html/cgi-bin/Count.cgi -bump|/usr/bin/cut -d ' ' -f 2`
lrl_web_prefix="/home/${USER}/public_html/${TMPDIR}/lrl_web_${lrl_web_prefix_num}"
mkdir -p ${lrl_web_prefix}
echo -n ${lrl_web_prefix}
