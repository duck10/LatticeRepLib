#!/bin/bash
export QUERY_STRING="df=lrl_web.dat"
export USER="yaya"
lrl_web_dir_num=`/home/${USER}/public_html/cgi-bin/Count.cgi -bump|/usr/bin/cut -d ' ' -f 2`
lrl_web_dir="tmp/${USER}_lrl_web_${lrl_web_dir_num}"
mkdir -p ${lrl_web_dir}
touch ${lrl_web_dir}/operation_01
touch ${lrl_web_dir}/lrl_web_data_01
touch ${lrl_web_dir}/lrl_web_output_01
touch ${lrl_web_dir}/operation_02
touch ${lrl_web_dir}/lrl_web_data_02
touch ${lrl_web_dir}/lrl_web_output_02
touch ${lrl_web_dir}/operation_03
touch ${lrl_web_dir}/lrl_web_data_03
touch ${lrl_web_dir}/lrl_web_output_03
touch ${lrl_web_dir}/operation_04
touch ${lrl_web_dir}/lrl_web_data_04
touch ${lrl_web_dir}/lrl_web_output_04
touch ${lrl_web_dir}/operation_05
touch ${lrl_web_dir}/lrl_web_data_05
touch ${lrl_web_dir}/lrl_web_output_05
touch ${lrl_web_dir}/operation_06
touch ${lrl_web_dir}/lrl_web_data_06
touch ${lrl_web_dir}/lrl_web_output_06
touch ${lrl_web_dir}/operation_07
touch ${lrl_web_dir}/lrl_web_data_07
touch ${lrl_web_dir}/lrl_web_output_07
touch ${lrl_web_dir}/operation_08
touch ${lrl_web_dir}/lrl_web_data_08
touch ${lrl_web_dir}/lrl_web_output_08
touch ${lrl_web_dir}/operation_09
touch ${lrl_web_dir}/lrl_web_data_09
touch ${lrl_web_dir}/lrl_web_output_09
touch ${lrl_web_dir}/operation_10
touch ${lrl_web_dir}/lrl_web_data_10
touch ${lrl_web_dir}/lrl_web_output_10
echo -n $lrl_web_dir

