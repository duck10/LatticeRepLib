
// SOURCE OF lrl_web.cpp GENERATED FROM lrl_web.m4
// lrl_web_host: blondie.arcib.org:8083
// lrl_web_user: yaya
// lrl_web_sdbx: lrl_web_sdbx
// lrl_web_tmp: tmp
// LRLWEBSANDBOX: 
// LRLWEBSBPRFX: 
// LRLWEBCGI: lrl_web.cgi
 
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <bits/stdc++.h>

#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

#include "do_posix_spawn.h"
#define LRL_WEB_DEBUG 0
#define NUMOPS_MAX 8
#define LRL_WEB_HOST std::string("blondie.arcib.org:8083")
#define LRL_WEB_USER std::string("yaya")
#define LRL_WEB_CGI std::string("lrl_web.cgi")
#define LRL_WEB_TMP std::string("tmp")
using namespace std;
using namespace cgicc;
void  dumpList(const Cgicc& formData); 
void  process(const Cgicc& formData); 

std::string tmp_lrl_web;  //directory for processing
std::string html_tmp_lrl_web; //html version of tmp_lrl_web
std::string myls;
int main(int argc, 
     char **argv)
{
   char buf[1024];
   std::string xbufstr;
   std::string ybufstr;
   std::string zbufstr;
   xbufstr=std::string("/home/")+LRL_WEB_USER+std::string("/public_html/cgi-bin/make_lrl_web_prefix_2.bash "+LRL_WEB_USER+" "+LRL_WEB_TMP);
   ybufstr=std::string("/home/")+LRL_WEB_USER+std::string("/public_html");
   zbufstr=std::string("~")+LRL_WEB_USER;
   if (do_exec_to_buffer(xbufstr.c_str(),buf,1024)!=0)
       exit(-1);
   tmp_lrl_web=std::string(buf);
   html_tmp_lrl_web=std::string(tmp_lrl_web).replace(0,ybufstr.length(),zbufstr);
   
   try {
      Cgicc cgi;

      // Send HTTP header
      std::cout << HTTPHTMLHeader() << std::endl;

      // Set up the HTML document"
      std::cout << cgicc::HTMLDoctype() << std::endl;
      std::cout << html() << std::endl;
      std::cout << "<HEAD>" << std::endl;
      std::cout << "	<meta charset=\"utf-8\">" << std::endl;
      std::cout << " 	" << std::endl;
      std::cout << " <script  type=\"text/javascript\">" << std::endl;
      std::cout << " const EventListenerMode = {capture: true};" << std::endl;
      std::cout << " " << std::endl;
      std::cout << "function preventGlobalMouseEvents () {" << std::endl;
      std::cout << "  document.body.style['pointer-events'] = 'none';" << std::endl;
      std::cout << "  return true;" << std::endl;
      std::cout << "}" << std::endl;
      std::cout << "" << std::endl;
      std::cout << "function restoreGlobalMouseEvents () {" << std::endl;
      std::cout << "  document.body.style['pointer-events'] = 'auto';" << std::endl;
      std::cout << "  return;" << std::endl;
      std::cout << "}" << std::endl;
      std::cout << "" << std::endl;
      std::cout << "function mousemoveListener (e) {" << std::endl;
      std::cout << "  e.stopPropagation ();" << std::endl;
      std::cout << "  // do whatever is needed while the user is moving the cursor around" << std::endl;
      std::cout << "  return true;" << std::endl;
      std::cout << "}" << std::endl;
      std::cout << "" << std::endl;
      std::cout << "function mouseupListener (e) {" << std::endl;
      std::cout << "  restoreGlobalMouseEvents ();" << std::endl;
      std::cout << "  document.removeEventListener ('mouseup',   mouseupListener,   EventListenerMode);" << std::endl;
      std::cout << "  document.removeEventListener ('mousemove', mousemoveListener, EventListenerMode);" << std::endl;
      std::cout << "  e.stopPropagation ();" << std::endl;
      std::cout << "  return true;" << std::endl;
      std::cout << "}" << std::endl;
      std::cout << " " << std::endl;

      std::cout << "function captureMouseEvents (e) {" << std::endl;
      std::cout << "  preventGlobalMouseEvents ();" << std::endl;
      std::cout << "  document.addEventListener ('mouseup',   mouseupListener,   EventListenerMode);" << std::endl;
      std::cout << "  document.addEventListener ('mousemove', mousemoveListener, EventListenerMode);" << std::endl;
      std::cout << "  e.preventDefault ();" << std::endl;
      std::cout << "  e.stopPropagation ();" << std::endl;
      std::cout << "  return true;" << std::endl;
      std::cout << "}" << std::endl;
      std::cout << "" << std::endl;
      std::cout << "function twodig(myintinp) {" << std::endl;
      std::cout << "  var myint=Number(myintinp);" << std::endl;
      std::cout << "  if (myint<10) {" << std::endl;
      std::cout << "    return \"0\"+myint;" << std::endl;
      std::cout << "  } else {" << std::endl;
      std::cout << "    return \"\"+myint;" << std::endl;
      std::cout << "  }" << std::endl;
      std::cout << "}" << std::endl;
      std::cout << "" << std::endl;
 
      std::cout << "function setchaininput(row){" << std::endl;
      std::cout << "    let rownum=((row)|0);" << std::endl;
      std::cout << "    var tdrownum=twodig(rownum);" << std::endl;
      std::cout << "    if (rownum < 2) {" << std::endl;
      std::cout << "      let firstrow=document.getElementById(\"chain_01\");" << std::endl;
      std::cout << "      if (!(firstrow.value===\"new_input\")){" << std::endl;
      std::cout << "        alert(\"Request for data prior to row 1 ignored\");" << std::endl;
      std::cout << "      }" << std::endl;
      std::cout << "      firstrow.value=\"new_input\";" << std::endl;
      std::cout << "      document.getElementById(\"block_01\").style=\"display:inline\";" << std::endl;
      std::cout << "      document.getElementById(\"block_01a\").style=\"display:inline\";" << std::endl;
      std::cout << "      document.getElementById(\"block_01b\").style=\"display:inline\";" << std::endl;
      std::cout << "      document.getElementById(\"block_01c\").style=\"display:inline\";" << std::endl;
      std::cout << "      document.getElementById(\"block_01d\").style=\"display:inline\";" << std::endl;
      std::cout << "      return true;" << std::endl;
      std::cout << "    } else if (rownum > "<<NUMOPS_MAX<<") {" << std::endl;
      std::cout << "      alert(\"Request for data after row "<<NUMOPS_MAX<<" ignored\");" << std::endl;
      std::cout << "      return false;" << std::endl;
      std::cout << "    }" << std::endl;
      std::cout << "    let priornum=rownum-1;" << std::endl;
      std::cout << "    if (document.getElementById(\"chain_\"+tdrownum).value==\"chain_input\") {" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+twodig(priornum)+\"c\").style=\"display:inline\";" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+twodig(priornum)+\"d\").style=\"display:inline\";" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdrownum+\"b\").style=\"display:none\";" << std::endl;
      std::cout << "    } else {" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdrownum+\"b\").style=\"display:inline\";" << std::endl;
      std::cout << "    }" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+tdrownum).style=\"display:inline\";" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+tdrownum+\"a\").style=\"display:inline\";" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+tdrownum+\"c\").style=\"display:inline\";" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+tdrownum+\"d\").style=\"display:inline\";" << std::endl;
      std::cout << "    return true;" << std::endl;
      std::cout << "}" << std::endl;
      std::cout << "" << std::endl;

      std::cout << "function changenumops2(){" << std::endl;
      std::cout << "  let mynumops=((document.getElementById(\"numops2\").value)|0);" << std::endl;
      std::cout << "  if (mynumops < 1) mynumops=1;" << std::endl;
      std::cout << "  if (mynumops > "<<NUMOPS_MAX<<") mynumops="<<NUMOPS_MAX<<";" << std::endl;
      std::cout << "  document.getElementById(\"numops2\").value=mynumops.toString();" << std::endl;
      std::cout << "  document.getElementById(\"numops\").value=mynumops.toString();" << std::endl;
      std::cout << "  changenumops();" << std::endl; 
      std::cout << "  return true;" << std::endl;
      std::cout << "}" << std::endl;
      std::cout << "" << std::endl;

      
      std::cout << "function changenumops(){" << std::endl;
      std::cout << "  var ii;" << std::endl;
      std::cout << "  var tdii;" << std::endl;
      std::cout << "  let mynumops=((document.getElementById(\"numops\").value)|0);" << std::endl;
      std::cout << "  if (mynumops < 1) mynumops=1;" << std::endl;
      std::cout << "  if (mynumops > "<<NUMOPS_MAX<<") mynumops="<<NUMOPS_MAX<<";" << std::endl;
      std::cout << "  document.getElementById(\"numops\").value=mynumops.toString();" << std::endl;
      std::cout << "  document.getElementById(\"numops2\").value=mynumops.toString();" << std::endl;
      std::cout << "  for (ii=1; ii<mynumops+1;ii++) {" << std::endl;
      std::cout << "    tdii = twodig(ii);" << std::endl;
      std::cout << "    // alert(\"enable block_\"+tdii);" << std::endl;
      std::cout << "    let mychain=document.getElementById(\"chain_\"+tdii).value;" << std::endl;
      std::cout << "    setchaininput(ii);" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+tdii).style=\"display:inline\";" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+tdii+\"a\").style=\"display:inline\";" << std::endl;
      std::cout << "    if (mychain!=\"chain_input\") {" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdii+\"b\").style=\"display:inline\";" << std::endl;
      std::cout << "    } else {" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdii+\"b\").style=\"display:none\";" << std::endl;
      std::cout << "    }" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+tdii+\"c\").style=\"display:inline\";" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+tdii+\"d\").style=\"display:inline\";" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+tdii+\"_running\").style=\"display:none\";" << std::endl;
      std::cout << "    if (ii > 1) {" << std::endl;
      std::cout << "      document.getElementById(\"hrule_\"+tdii).style=\"display:inline\";" << std::endl;
      std::cout << "    }" << std::endl;
      std::cout << "    changeoperation(tdii);" << std::endl;
      std::cout << "  }" << std::endl;
      std::cout << "  document.getElementById(\"ScrollTo\").value=twodig(parseInt((mynumops+1)/2));" << std::endl;
      std::cout << "  if (mynumops < "<<NUMOPS_MAX<<") {" << std::endl;
      std::cout << "    for (ii=mynumops+1; ii<"<<NUMOPS_MAX+1<<";ii++) {" << std::endl;
      std::cout << "      tdii = twodig(ii);" << std::endl;
      std::cout << "      // alert(\"disable block_\"+tdii);" << std::endl;
      std::cout << "      if (ii > 1) {" << std::endl;
      std::cout << "        document.getElementById(\"hrule_\"+tdii).style=\"display:none\";" << std::endl;
      std::cout << "      }" << std::endl;
      std::cout << "      document.getElementById(\"operation_\"+tdii).value=\"NoOp\";" << std::endl;
      std::cout << "      changeoperation(tdii);" << std::endl;
      std::cout << "      document.getElementById(\"chain_\"+tdii).value=\"chain_input\";" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdii).style=\"display:none\";" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdii+\"a\").style=\"display:none\";" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdii+\"b\").style=\"display:none\";" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdii+\"c\").style=\"display:none\";" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdii+\"d\").style=\"display:none\";" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdii+\"_running\").style=\"display:none\";" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdii+\"b_cmdgen\").style=\"display:none\";" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdii+\"b_cmdpath\").style=\"display:none\";" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdii+\"b_cmdperturb\").style=\"display:none\";" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdii+\"b_cmdscale\").style=\"display:none\";" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdii+\"b_cmdtos6l\").style=\"display:none\";" << std::endl;
      std::cout << "      if (ii > 1) {" << std::endl;
      std::cout << "        document.getElementById(\"hrule_\"+tdii).style=\"display:none\";" << std::endl;
      std::cout << "      }" << std::endl;
      std::cout << "    }" << std::endl;
      std::cout << "  }" << std::endl;
      std::cout << "  return true;" << std::endl;
      std::cout << "}" << std::endl;
      std::cout << "" << std::endl;
      std::cout << "function noop(){" << std::endl;
      std::cout << "  return true;" << std::endl;
      std::cout << "}" << std::endl;
      std::cout << "" << std::endl;
      std::cout << "function running(rownum) {" << std::endl;
      std::cout << "  var ii;" << std::endl;
      std::cout << "  var tdii;" << std::endl;
      std::cout << "  let mynumops=((document.getElementById(\"numops\").value)|0);" << std::endl;
      std::cout << "  if (mynumops < 1) mynumops=1;" << std::endl;
      std::cout << "  if (mynumops > 8) mynumops=8;" << std::endl;
      std::cout << "  document.getElementById(\"numops\").value=mynumops.toString();" << std::endl;
      std::cout << "  document.getElementById(\"submit_00\").disabled=true;" << std::endl;
      std::cout << "  document.getElementById(\"submit_000\").disabled=true;" << std::endl;
      std::cout << "  for (ii=1; ii<mynumops+1;ii++) {" << std::endl;
      std::cout << "      tdii = twodig(ii);" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+tdii+\"_running\").style=\"display:inline\";" << std::endl;
      std::cout << "      document.getElementById(\"submit_\"+tdii).disabled=true;" << std::endl;
      std::cout << "  }" << std::endl;
      std::cout << "  document.getElementById(\"ScrollTo\").value=rownum;" << std::endl;
      std::cout << "  let timerId = setTimeout(noop,500);" << std::endl;
      std::cout << "  return true;" << std::endl;
      std::cout << "}" << std::endl;
      std::cout << "" << std::endl;


      std::cout << "function changeoperation(rownum) {" << std::endl;
      std::cout << std::string("   var tdrownum=twodig(rownum);") << std::endl;
      std::cout << std::string("   var outdivid=document.getElementById(\"lrl_web_output_\"+tdrownum);") << std::endl;
      std::cout << std::string("   outdivid.scrollTop = \"0\";") << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).scrollTop=\"0\";")<< std::endl;
      std::cout << " let operation=document.getElementById(\"operation_\"+tdrownum).value;" << std::endl;
      std::cout << " if (operation!=\"CmdGen\") {" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdgen\").style=\"display:none\";" << std::endl;
      std::cout << " }" << std::endl;
      std::cout << " if (operation!=\"CmdPath\") {" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdpath\").style=\"display:none\";" << std::endl;
      std::cout << " }" << std::endl;
      std::cout << " if (operation!=\"CmdPerturb\") {" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdperturb\").style=\"display:none\";" << std::endl;
      std::cout << " }" << std::endl;
      std::cout << " if (operation!=\"CmdScale\") {" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdscale\").style=\"display:none\";" << std::endl;
      std::cout << " }" << std::endl;
      std::cout << " if (operation!=\"CmdToS6L\") {" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdtos6l\").style=\"display:none\";" << std::endl;
      std::cout << " }" << std::endl;
      std::cout << " if (operation==\"Dirichlet\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1><strong>Command: Dirichlet</strong><br />")
      +std::string("<em><u>Purpose:</u></em> draw Dirichlet cells of listed cells<br />")
      +std::string("The Dirichlet cell (also known as Voronoi domain, Voronoi cell, Delaunay domain, or Wigner-Seitz cell) is the 3-D analog of Delaunay triangulation. It is the points nearest to a particular lattice point, usually the origin, rather than other lattice points.<br />")
      +std::string("<em><u>Output type:</u></em> images of Dirichlet cells in multiple orientations <br />")
      +std::string("<em><u>Parameters:</u></em> --blockorigin blkorig (0) and --blocksize blksz(10) <br />")
      +std::string("limit the portion of the listed cells drawn to prevent overload<br />")
      +std::string("<hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdDelone\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1><strong>Command: compute Selling-reduced primitive cells</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Perform Selling/Delone reduction on input cells<br />")
      +std::string("<em><u>Output type:</u></em> S<sup>6</sup> form of the reduced cells of input. The Delone-reduced<br />")
      +std::string("cell has only non-acute interaxial angles.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl;
      std::cout << " } else if (operation==\"CmdDists\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: compute NCDist and CS6Dist distances</strong><br />")
      +std::string("<em><u>Purpose:</u></em> For a group of input cells, distances are reported in 3 different calculations:<br />")
      +std::string("the simple Euclidean distance, the NCDist (G<sup>6</sup>) distance, and the CS6Dist<br />")
      +std::string("(S<sup>6</sup>) distances. Distances are calculated in 2 different ways. The first<br />")
      +std::string("is the rolling calculation of pairs in the list. The second is the distance of each <br />")
      +std::string("cell from the first input cell.<br />")
      +std::string("<em><u>Output type:</u></em> The distances for each input are output on a single line, starting with a <br />")
      +std::string("semicolon so that it will be ignored by succeeding programs. The input cells are echoed<br />")
      +std::string("to the output line as primitive and a, b, c &alpha;, &beta;, &gamma;, so that they <br />")
      +std::string("can be used for further input. Each output cell is followed<br />")
      +std::string("by its ordinal number, and the distance calculations list the ordinals for the<br />")
      +std::string("cells used of each calculation.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdGen\") {" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdgen\").style=\"display:inline\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdpath\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdperturb\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdscale\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdtos6l\").style=\"display:none\";" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: Generate cells of a particular type or types</strong><br />")
      +std::string("<em><u>Purpose:</u></em> To Create valid unit cells of various user-chosen types<br />")
      +std::string("<em><u>Output type:</u></em> Output cells are reported in G<sup>6</sup> form, randomly chosen cell<br />")
      +std::string("parameters for the various selected types. Lattice types are shown following <br />")
      +std::string("the numerical parameters.<br />")
      +std::string("<em><u>Parameters:</u></em> (default values in parentheses)<br />")
      +std::string("--- The number of cells of each type to be generated (1)<br />")
      +std::string("--- The type(s) of selected output requested. Examples are<br />")
      +std::string("--- &quot;all&quot; (default) -- produces examples of each of the 44 Niggli types <br />")
      +std::string("(see the International Tables) and also each of the 24 Delone types (all)<br />")
      +std::string("--- a number between 1 and 44 will generate examples of that Niggli type. For instance,<br />")
      +std::string("24 will create Niggli type 24 (which is hR). {allowed values: 1-44}<br />")
      +std::string("--- &quot;m&quot; generates monoclinic examples {allowed types: c, t, h, o, m, a}<br />")
      +std::string("--- &quot;mC&quot; generates C-centered monoclinic examples {mP, mS, mC, hR, hP, tP, tI, cF, cI, cP, aP}<br />")
      +std::string("--- &quot;A1&quot; generates a general triclinic Delone type with no S<sup>6</sup> zero scalars.<br />")
      +std::string("--- &quot;O4&quot; generates the centered orthorhombic Delone type with signature {00r sst}.<br />")
      +std::string("--- &quot;O&quot; will generate all orthorhombic Delone types {allowed types: C, T, H, O, M, A}<br />")
      +std::string("--- Unrecognized input will generate all types<br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdLM\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: apply Lattice Matching algorithm to listed cells</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Taking the first input cells as the &quot;REFERENCE&quot;, the alternative cells of<br />")
      +std::string("each succeeding cell are examined to find a closest match to the reference lattice. <br />")
      +std::string("<p style=margin-left:40px>Andrews, Lawrence C., Herbert J. Bernstein, and Nicholas K. Sauter. <br />")
      +std::string("&quot;Approximating lattice similarity.&quot; Acta Crystallographica Section A: Foundations and Advances 79.5, 480-484 (2023).</p> <em><u>Output type:</u></em> a, b, c &alpha;, &beta;, &gamma;, with the lattice centering<br />")
      +std::string("chosen to be the same as that of the reference cell.<br />")
      +std::string("<em><u>Example:</u></em><br />")
      +std::string("f 10 10 10 90 90 90<br />")
      +std::string("p 7 7.1 6.9 59 60 61<br />")
      +std::string("end<br />")
      +std::string("; Lattice Matching<br />")
      +std::string("F 10.000 10.000 10.000 90.000 90.000 90.000 ( 0 degrees in S6) REFERENCE<br />")
      +std::string("F 10.050 9.821 9.832 89.143 92.094 88.781 ( 3.07179 degrees in S6)<br />")
      +std::string("end<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdNiggli\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: compute Niggli-reduced primitive cells</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Perform Niggli reduction on input cells. The result has<br />")
      +std::string("the shortest edge lengths of the lattice and is the &quot;most orthogonal&quot; unit cell.<br />")
      +std::string("<em><u>Output type:</u></em> G<sup>6</sup> form of the reduced cells of input<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdPath\") {" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdgen\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdpath\").style=\"display:inline\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdperturb\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdscale\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdtos6l\").style=\"display:none\";" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: compute path between pairs of cells</strong><br />")
      +std::string("<em><u>Purpose:</u></em> A series of points is computed between each pair of input cells. The<br />")
      +std::string("computation is done in S<sup>6</sup>. For n input cells, n-1 paths will be<br />")
      +std::string("generated. The individual points are labeled with their ordinals and each <br />")
      +std::string("segment is labeled (after the 6 parameters).<br />")
      +std::string("<em><u>Output type:</u></em> S<sup>6</sup><br />")
      +std::string("<em><u>Parameters:</u></em> The number of points in -each- path (20)<br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl;
      std::cout << " } else if (operation==\"CmdPerturb\") {" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdgen\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdpath\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdperturb\").style=\"display:inline\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdscale\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdtos6l\").style=\"display:none\";" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: compute perturbed versions of input cells</strong><br />")
      +std::string("<em><u>Purpose:</u></em> For each input cell, compute slight altered versions<br />")
      +std::string("<em><u>Output type:</u></em> unit cell edge lengths and angles<br />")
      +std::string("<em><u>Parameters:</u></em> The number of perturbations of each input cell (20) and the amount of<br />")
      +std::string("perturbation in parts-per-thousand (ppt) (1 ppt). The calculation is done in S<sup>6</sup>,<br />")
      +std::string("and the perturbation is in a direction orthogonal to the S<sup>6</sup> vector..<br />")
      +std::string("<em><u>Example:</u></em> requesting 10 perturbations and 10 ppt (1%) perturbations.<br />")
      +std::string("f 10 10 10 90 90 90<br />")
      +std::string("end<br />")
      +std::string("; Perturb vectors<br />")
      +std::string("<br />")
      +std::string("F 10 10 10 90 90 90 original input cell<br />")
      +std::string("F 9.940 9.916 9.884 89.573 89.357 89.565 perturbed <br />")
      +std::string("F 10.016 10.029 9.960 89.567 90.358 90.109 perturbed <br />")
      +std::string("F 9.956 9.921 9.929 89.938 89.497 89.452 perturbed <br />")
      +std::string("F 9.896 9.992 10.053 90.576 89.709 89.376 perturbed <br />")
      +std::string("F 9.969 9.989 9.984 90.509 89.490 89.668 perturbed <br />")
      +std::string("F 9.963 9.984 9.904 89.655 89.147 90.336 perturbed <br />")
      +std::string("F 10.015 9.928 9.962 89.793 90.276 89.394 perturbed <br />")
      +std::string("F 10.019 9.922 9.884 89.295 89.772 89.926 perturbed <br />")
      +std::string("F 9.914 9.918 9.909 89.327 89.348 89.820 perturbed <br />")
      +std::string("F 10.020 9.978 10.119 90.295 90.410 89.960 perturbed <br />")
      +std::string("<br />")
      +std::string("; CmdPerturb: number of cells to produce = 10 orthogonal perturbation of 10 /1000<br />")
      +std::string("end<br />")
      +std::string("<hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdS6Refl\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: apply S6 reflections to input cells</strong><br />")
      +std::string("<em><u>Purpose:</u></em> The 24 permutations of the S<sup>6</sup> cell vectors. Duplicates are removed.<br />")
      +std::string("<em><u>Output type:</u></em> S<sup>6</sup><br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<em><u>Example:</u></em><br />")
      +std::string("f 10 10 10 90 90 90<br />")
      +std::string("end<br />")
      +std::string("; S6 reflections<br />")
      +std::string("S6 0.00000 0.00000 0.00000 -100.00000 -100.00000 -100.00000 0<br />")
      +std::string("S6 0.00000 -100.00000 -100.00000 -100.00000 0.00000 0.00000 1<br />")
      +std::string("S6 -100.00000 0.00000 -100.00000 0.00000 -100.00000 0.00000 2<br />")
      +std::string("S6 -100.00000 -100.00000 0.00000 0.00000 0.00000 -100.00000 3<br />")
      +std::string("<br />")
      +std::string("or presenting those as unit cells (not output by the command):<br />")
      +std::string("P 10.000 10.000 10.000 90.000 90.000 90.000 <br />")
      +std::string("P 17.321 10.000 10.000 90.000 125.264 125.264 <br />")
      +std::string("P 10.000 17.321 10.000 125.264 90.000 125.264 <br />")
      +std::string("P 10.000 10.000 17.321 125.264 125.264 90.000 <br />")
      +std::string("<br />")
      +std::string("The values of 17.321 are from the Delone &quot;d&quot; vector, d=-(a+b+c)<br />")
      +std::string("<br />")
      +std::string("But Niggli reduction of those 4 cells gives:<br />")
      +std::string("G6 100.000 100.000 100.000 0.000 0.000 0.000 <br />")
      +std::string("G6 100.000 100.000 100.000 0.000 0.000 0.000 <br />")
      +std::string("G6 100.000 100.000 100.000 0.000 0.000 0.000 <br />")
      +std::string("G6 100.000 100.000 100.000 0.000 0.000 0.000 <br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdScale\") {" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdgen\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdpath\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdperturb\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdscale\").style=\"display:inline\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdtos6l\").style=\"display:none\";" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: Scale unit cells data to single size</strong><br />")
      +std::string("<em><u>Purpose:</u></em><br />")
      +std::string("<em><u>Output type:</u></em> Chosen vector type (one of S6, V7, DC7u, RI (root invariant))<br />")
      +std::string(" <em><u>Parameters:</u></em> Vector type to scale and output(S6)<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdSella\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: apply Sella algorithm</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Determine potentially higher symmetry for input cells<br />")
      +std::string("<em><u>Output type:</u></em> agreement factors for each of the Bravais lattice types<br />")
      +std::string("that fill well and reduced cell information for the input cells. Also the name<br />")
      +std::string("of the graphical output file.<br />")
      +std::string("<em><u>Graphical output:</u></em> the figure of the hierarchy of Bravais lattice types <br />")
      +std::string("with numerical agreement determined by Sella.<br />")
      +std::string("<p  style=margin-left:40px> Grimmer, H., &amp; Nespolo, M. (2006). <br />")
      +std::string("Geminography: the crystallography of twins. Zeitschrift<br />")
      +std::string("f&uuml;r Kristallographie-Crystalline Materials, 221(1), 28-50.)</p>  <em><u>Parameters:</u></em> choices if only some outputs are needed (all)<br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdToB4\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: compute Bravais tetrahedron (B4)</strong><br />")
      +std::string("<em><u>Purpose:</u></em> computes the edge vectors of the unit cell and their negative sum. These 4<br />")
      +std::string("vectors also define the Bravais tetrahedron Delone, B. N., Galiulin, R. V., <br />")
      +std::string("&amp; Shtogrin, M. I. (1975). On the Bravais types of <br />")
      +std::string("lattices. Journal of Soviet Mathematics, 4(1), 79-156.<br />")
      +std::string("<em><u>Output type:</u></em> echoes the input cell as a, b, c, &alpha;, &beta;, &gamma;, <br />")
      +std::string("followed by the vector representations of the unit cell edges and their vector sum,<br />")
      +std::string("and then a listing of their lengths.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdToC3\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: compute complex cell presentation (C</strong><strong><sup>3</sup></strong><strong>)</strong><br />")
      +std::string("<em><u>Purpose:</u></em> presents input cells as C<sup>3</sup><br />")
      +std::string("<em><u>Output type:</u></em> the C<sup>3</sup> version of the input (note that lattice centering is not removed)<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<em><u>Example:</u></em><br />")
      +std::string("; To C3<br />")
      +std::string("g 100 110 120 0 1 2<br />")
      +std::string("end<br />")
      +std::string("C3 (0.000,-101.500) (0.500,-111.000) (1.000,-120.500) <br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdToCell\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: compute side-angle cells (a, b, c, &alpha;, &beta;, &gamma;)</strong><br />")
      +std::string("<em><u>Purpose:</u></em> converts input cell representations to cell parameters<br />")
      +std::string("(note that lattice centering is not removed)<br />")
      +std::string("<em><u>Output type:</u></em> a, b, c, &alpha;, &beta;, &gamma;. If the input cell is not in <br />")
      +std::string("cell parameters already, then primitive is assumed.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdToG6\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: compute G</strong><strong><sup>6</sup></strong><strong> version of cells</strong><br />")
      +std::string("<em><u>Purpose:</u></em> convert to G<sup>6</sup> representation (note that lattice centering is not removed)<br />")
      +std::string("<em><u>Output type:</u></em> G<sup>6</sup><br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdToS6\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: compute S<sup>6</sup> version of cells</strong><br />")
      +std::string("<em><u>Purpose:</u></em> convert to S<sup>6</sup> representation (note that lattice centering is not removed)<br />")
      +std::string("<em><u>Output type:</u></em> S<sup>6</sup><br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdToS6L\") {" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdgen\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdpath\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdperturb\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdscale\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdtos6l\").style=\"display:inline\";" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: compute linearized S</strong><strong><sup>6</sup></strong><strong> or Root Invariant version of cells</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Both of the output choices are 6-space vectors of positive numbers.<br />")
      +std::string("The values are derived by first applying Selling/Delone reduction so that all of <br />")
      +std::string("the S<sup>6</sup> values are non-positive. Then the square roots of the negative <br />")
      +std::string("of those values are used to create the result (linearized S<sup>6</sup>). The<br />")
      +std::string("root invariant is generated by applying sorting rules of Bright, Cooper, and Kurlin. <P style=margin-left:40px>Bright, M., Cooper, A. I., &amp; Kurlin, V. (2021). <br />")
      +std::string("Welcome to a continuous world of 3-dimensional lattices. <br />")
      +std::string("arXiv preprint arxiv:2109.11538, 2.</p> <em><u>Output type:</u></em> The type followed by the parameters.<br />")
      +std::string("<em><u>Parameters:</u></em> &quotall&quot or blank for output of both types, &quot;R&quot; for root invariant, and S for linearized S<sup>6</sup> <br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdToU\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: compute unsorted Dirichlet cells (dc7unsrt)</strong><br />")
      +std::string("<em><u>Purpose:</u></em> convert to the 7 parameter form related to the Dirichlet<br />")
      +std::string("(also known as Voronoi or Wigner-Seitz) unit cell. (note that lattice centering is not removed)<br />")
      +std::string("Bernstein and Andrews, 2023.<br />")
      +std::string("<em><u>Output type:</u></em> DC7u<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdToV7\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: compute V<sup>7</sup> version of cells</strong><br />")
      +std::string("<em><u>Purpose:</u></em> convert to the 7 parameter vector of Andrews, Bernstein, Pelletier, 1980.<br />")
      +std::string("The vector is define in terms of the Niggli reduced cell a,b,c and the Niggli reduced<br />")
      +std::string("reciprocal cell 1/a*, 1/b*, 1/c*, and the cube root of the reduced cell volume.<br />")
      +std::string(" (note that lattice centering is not removed). Note that all 7 scalars are in the units of Angstroms.<br />")
      +std::string("<em><u>Output type:</u></em> V<sup>7</sup><br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"CmdVolume\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: compute volumes of listed cells</strong><br />")
      +std::string("<em><u>Purpose:</u></em> compute volumes of listed cells<br />")
      +std::string("<em><u>Output type:</u></em> Lattice centering, then cell parameters (a, b, c, &alpha;, &beta;, &gamma;), <br />")
      +std::string("followed by the volume<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else if (operation==\"PlotC3\") {" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1> <strong>Command: [PlotC3</strong>]<br />")
      +std::string("<em><u>Purpose:</u></em> Graphical output of multiple unit cells in C<sup>3</sup><br />")
      +std::string("<em><u>Output type:</u></em> Name of the file containing the plots<br />")
      +std::string("<em><u>Graphical output:</u></em> 2-D plots of the 3 complex coordinates in C<sup>3</sup><br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("The 3 (2-D) coordinates are plotted in a single row. The all-negative quadrants <br />")
      +std::string("are shown. A consequence is that if the unit cells have not been Selling/Delone<br />")
      +std::string("reduced, then points will plot outside of that quadrant.<br />")
      +std::string(" <hr><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl; 
      std::cout << " } else {" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdgen\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdpath\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdperturb\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdscale\").style=\"display:none\";" << std::endl;
      std::cout << "   document.getElementById(\"block_\"+tdrownum+\"b_cmdtos6l\").style=\"display:none\";" << std::endl;
      std::cout << std::string("   document.getElementById(\"lrl_web_help_\"+tdrownum).innerHTML=")+std::string("\"<font size=-1><strong>Command: Check Input</strong><br />")
      +std::string("<em><u>Purpose:</u></em> Verify input lattice types and parameters<br />")
      +std::string("<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />")
      +std::string("Warnings are output for invalid inputs.<br />")
      +std::string("<em><u>Parameters:</u></em> NA<br />")
      +std::string("<hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br />")
      +std::string(" All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />")
      +std::string("---  RANDOM: Random (valid) unit cell;<br />")
      +std::string("---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />")
      +std::string("---  semicolon: lines beginning with a semicolon are treated as comments<br />")
      +std::string("---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />")
      +std::string("---  END: ends the data input section<br />")
      +std::string("Examples of unit cell inputs<br />")
      +std::string("P 10 20 30 90 111 90<br />")
      +std::string("G 100 400 900 0 -215.02 0<br />")
      +std::string("S6 0 -107.51 0 7.51 -400 -792.49 <br />")
      +std::string("; this is a comment<br />")
      +std::string("end<br /></font>\"")+std::string(";") << std::endl;

      std::cout << " }" << std::endl;
      std::cout << " return true;" << std::endl;
      std::cout << "}" << std::endl;

      std::cout << "" << std::endl;
      std::cout << "function pfloat(pfield){" << std::endl;
      std::cout << "    // validate for non-negative float   " << std::endl;
      std::cout << "    var charsAllowed=\"0123456789.+\";" << std::endl;
      std::cout << "    var allowed;" << std::endl;
      std::cout << "    var plusfound;" << std::endl;
      std::cout << "    var dotfound;" << std::endl;
      std::cout << "    plusfound = 0;" << std::endl;
      std::cout << "    dotfound = 0;" << std::endl;
      std::cout << "    for(var i=0;i<pfield.value.length;i++){       " << std::endl;
      std::cout << "        allowed=false;" << std::endl;
      std::cout << "        for(var j=0;j<charsAllowed.length;j++){" << std::endl;
      std::cout << "            if( pfield.value.charAt(i)==charsAllowed.charAt(j) ){ " << std::endl;
      std::cout << "               allowed=true;" << std::endl;
      std::cout << "               if (j == 11) {" << std::endl;
      std::cout << "                 plusfound++;" << std::endl;
      std::cout << "                 allowed=false;" << std::endl;
      std::cout << "                 plusfound--;" << std::endl;
      std::cout << "                 break;" << std::endl;
      std::cout << "               } else if (j == 10) {" << std::endl;
      std::cout << "                 dotfound++;" << std::endl;
      std::cout << "                 if (dotfound > 1) {" << std::endl;
      std::cout << "                   allowed=false;" << std::endl;
      std::cout << "                   dotfound--;" << std::endl;
      std::cout << "                   break;" << std::endl;
      std::cout << "                 }" << std::endl;
      std::cout << "               } " << std::endl;
      std::cout << "               break;" << std::endl;
      std::cout << "            }" << std::endl;
      std::cout << "        }" << std::endl;
      std::cout << "        if(allowed==false){ pfield.value = pfield.value.replace(pfield.value.charAt(i),\"\"); i--; }" << std::endl;
      std::cout << "    }" << std::endl;
      std::cout << "    return true;" << std::endl;
      std::cout << "}" << std::endl;
      std::cout << "" << std::endl;
      std::cout << "function gfloat(field){" << std::endl;
      std::cout << "    // validate for float   " << std::endl;
      std::cout << "    var charsAllowed=\"0123456789.+-\";" << std::endl;
      std::cout << "    var allowed;" << std::endl;
      std::cout << "    var plusminusfound;" << std::endl;
      std::cout << "    var dotfound;" << std::endl;
      std::cout << "    plusminusfound = 0;" << std::endl;
      std::cout << "    dotfound = 0;" << std::endl;
      std::cout << "    otherfound = 0;" << std::endl;
      std::cout << "    for(var i=0;i<field.value.length;i++){       " << std::endl;
      std::cout << "        allowed=false;" << std::endl;
      std::cout << "        for(var j=0;j<charsAllowed.length;j++){" << std::endl;
      std::cout << "            if( field.value.charAt(i)==charsAllowed.charAt(j) ){ " << std::endl;
      std::cout << "               allowed=true;" << std::endl;
      std::cout << "               if (j == 11 || j == 12) {" << std::endl;
      std::cout << "                 if (otherfound>0 || dotfound>0 || plusminusfound>0) {" << std::endl;
      std::cout << "                   allowed=false;" << std::endl;
      std::cout << "                 } else {" << std::endl;
      std::cout << "                 plusminusfound++;" << std::endl;
      std::cout << "                 }" << std::endl;
      std::cout << "               } else if (j == 10) {" << std::endl;
      std::cout << "                 if (dotfound>0) {" << std::endl;
      std::cout << "                   allowed=false;" << std::endl;
      std::cout << "                 } else {" << std::endl;
      std::cout << "                 dotfound++;" << std::endl;
      std::cout << "                 } " << std::endl;
      std::cout << "               } else {" << std::endl;
      std::cout << "                 otherfound++;" << std::endl;
      std::cout << "               } " << std::endl;
      std::cout << "            }" << std::endl;
      std::cout << "        }" << std::endl;
      std::cout << "        if(allowed==false){ field.value = field.value.replace(field.value.charAt(i),\"\"); i--; }" << std::endl;
      std::cout << "    }" << std::endl;
      std::cout << "    return true;" << std::endl;
      std::cout << "}" << std::endl;
      std::cout << "" << std::endl;
      std::cout << "</script>" << std::endl;
      std::cout << "" << std::endl;
      std::cout << "<TITLE>" << std::endl;
      std::cout << "LRL_WEB Lattice Representation Library Tool Web Page" << std::endl;
      std::cout << "</TITLE> " << std::endl;
      std::cout << "</HEAD> " << std::endl;
      std::cout << "" << std::endl;
      std::cout << "" << std::endl;

      cgicc::const_form_iterator scrollto_iter;
      std::string scrollto;
      scrollto_iter = cgi.getElement("ScrollTo");
      if (scrollto_iter == cgi.getElements().end()) {
        scrollto = std::string("mark_01");
      } else {
        scrollto = std::string("mark_")+scrollto_iter->getValue();
      }

      std::cout << "<BODY onload=\"document.getElementById('"+scrollto+"').scrollIntoView();changenumops();changeoperation('01');changeoperation('02');changeoperation('03');changeoperation('04');changeoperation('05');changeoperation('06');changeoperation('07');changeoperation('08');\">" << std::endl;
      std::cout << "<a name=\"mark_00\" id=\"mark_00\" />" << std::endl;
      std::cout << "<font face=\"Arial,Helvetica,Times\" size=\"3\">" << std::endl;
      std::cout << "<hr />" << std::endl;
      std::cout << "<center>" << std::endl;
      std::cout << "rawprefix: " << "'"+tmp_lrl_web+"/'" << ", htmlprefix: "<< "'"+html_tmp_lrl_web+"/'"<<std::endl;
      std::cout << "</center>" << std::endl;

      // Dump form 
     if (LRL_WEB_DEBUG)  dumpList(cgi);

      // Process form
      process(cgi);

      // Close the HTML document
      std::cout << "</BODY>" << std::endl;
      std::cout << "</HTML>" << std::endl;
   }
   catch(exception& e) {
      // handle any errors - omitted for brevity
   }
}

// Convert line breaks and special characters to HTML 
std::string plaintext2html(std::string & dst, std::string src){
    size_t dstlen, srclen, ii;
    char c;
    srclen=src.length();
    dstlen=dst.length();
    if (dstlen>0){
      dst.clear();
      dstlen=0;
    }
    for(ii=0;ii<srclen;ii++){
      c=src[ii];
      switch (c) {
        case('\n'):
          dst.append("<br />");
          break;
        case('\r'):
          dst.append("<br />");
          if(ii<srclen-1 && src[ii+1]=='\n') ii++;
          break;
        case('&'):
          dst.append("&amp;");
          break;
        case('<'):
          dst.append("&lt;");
          break;
        case('>'):
          dst.append("&gt;");
          break;
        case(0):
          dst.append("&#0;");
          break;
        default:
          dst.push_back(c);
          break;
      }
    }
    return dst;
}

  void  dumpList(const Cgicc& formData) 
   {
     std::string htstr;
     cout << h2("Form Data via vector") << std::endl; 
     cout << cgicc::div().set("align","center") << std::endl;
     cout << "<pre>"<<std::endl<<myls<<std::endl<<"</pre>"<<std::endl; 
     cout << table()<< std::endl;
     cout << tr().set("class","title") << td("Element <br /> Name") 
          << td("Element <br /> Value") << tr() << std::endl;
 
    // Iterate through the vector, and print out each value
     const_form_iterator iter;
     for(iter = formData.getElements().begin(); 
         iter != formData.getElements().end(); 
         ++iter) {
       cout << tr().set("class","data") << td(iter->getName()) 
            << td(plaintext2html(htstr,iter->getValue())) << tr() << std::endl;
     }
     cout << table() << cgicc::div() << std::endl;
  }


// Process the form
 void  process(const Cgicc& formData) 
 {
    cgicc::const_form_iterator numops_iter;
    cgicc::const_form_iterator lrl_web_data_iter;
    cgicc::const_form_iterator lrl_web_output_iter;
    cgicc::const_form_iterator chain_iter;
    cgicc::const_form_iterator operation_iter;
    cgicc::const_form_iterator lrl_web_data_cmdgen_ngen_iter;
    cgicc::const_form_iterator lrl_web_data_cmdgen_ltype_iter;
    cgicc::const_form_iterator lrl_web_data_cmdpath_npath_iter;
    cgicc::const_form_iterator lrl_web_data_cmdperturb_npert_iter;
    cgicc::const_form_iterator lrl_web_data_cmdperturb_ppk_iter;
    cgicc::const_form_iterator lrl_web_data_cmdscale_type_iter;
    cgicc::const_form_iterator lrl_web_data_cmdtos6l_type_iter;
    ofstream lrl_web_data_file;
    std::string lrl_web_data_data;
    std::string chain;
    std::string xactstr;
    size_t numops, numops2;
    size_t numop;
    std::string twodig_array[NUMOPS_MAX+1]={"00","01","02","03","04","05","06","07","08"
#if NUMOPS_MAX > 8
#if NUMOPS_MAX < 10
#define NUMOPS_MAX 10 
#endif
    ,"09","10"
#endif
#if NUMOPS_MAX > 10
#if NUMOPS_MAX < 20
#define NUMOPS_MAX 20
#endif
    ,"11","12","13","14","15","16","17","18","19","20"
#endif
#if NUMOPS_MAX > 20
#if NUMOPS_MAX < 30
#define NUMOPS_MAX 30
#endif
    ,"21","22","23","24","25","26","27","28","29","30"
#endif
#if NUMOPS_MAX > 30
#if NUMOPS_MAX < 40
#define NUMOPS_MAX 40
#endif
    ,"31","32","33","34","35","36","37","38","39","40"
#endif
#if NUMOPS_MAX > 40
#if NUMOPS_MAX < 50
#define NUMOPS_MAX 50
#endif
    ,"41","42","43","44","45","46","47","48","49","50"
#endif
#if NUMOPS_MAX > 50
#if NUMOPS_MAX < 60
#define NUMOPS_MAX 60
#endif
    ,"51","52","53","54","55","56","57","58","59","60"
#endif
#if NUMOPS_MAX > 60
#if NUMOPS_MAX < 70
#define NUMOPS_MAX 70
#endif
    ,"61","62","63","64","65","66","67","68","69","70"
#endif
#if NUMOPS_MAX > 70
#if NUMOPS_MAX < 80
#define NUMOPS_MAX 80
#endif
    ,"71","72","73","74","75","76","77","78","79","80"
#endif
#if NUMOPS_MAX > 80
#if NUMOPS_MAX < 90
#define NUMOPS_MAX 90
#endif
    ,"81","82","83","84","85","86","87","88","89","90"
#endif
#if NUMOPS_MAX > 90
#if NUMOPS_MAX < 99
#define NUMOPS_MAX 99
#endif
    ,"91","92","93","94","95","96","97","98","99"
#endif
    };

    numops_iter = formData.getElement("numops");
    if (numops_iter == formData.getElements().end()) {
      numops = 1;
    } else {
      numops = (size_t)atol(numops_iter->getValue().c_str());
    }

    std::cout << "<hr />" << std::endl;
    std::cout << "<center>" << std::endl;
    std::cout << "<table border=0>" <<std::endl;
    std::cout << "<tr>" <<std::endl;
    std::cout << "<td align=left valign=top><a name=\"celticknots\"><img src=\"http://"+LRL_WEB_HOST+"/~"+LRL_WEB_USER+"/images/leftcorner.gif\" /></a></td>" <<std::endl;
    std::cout << "<td width=150 rowspan=\"2\">&nbsp;</td>" <<std::endl;
    std::cout << "<td align=center rowspan=\"2\"><img src=\"http://"+LRL_WEB_HOST+"/~"+LRL_WEB_USER+"/images/dragon1a.gif\" /></td>" <<std::endl;
    std::cout << "<td width=150 rowspan=\"2\">&nbsp;</td>" <<std::endl;
    std::cout << "<td align=right valign=top><img src=\"http://"+LRL_WEB_HOST+"/~"+LRL_WEB_USER+"/images/rightcorner.gif\" /></td>" <<std::endl;
    std::cout << "</tr>" <<std::endl;
    std::cout << "<tr> <td align=left valign=bottom><img src=\"http://blondie.arcib.org:8083/~yaya/images/leftbottomcorner.gif\" /></td>" <<std::endl;
    std::cout << "<td align=right valign=bottom><img src=\"http://blondie.arcib.org:8083/~yaya/images/rightbottomcorner.gif\" /></td></tr>" <<std::endl;
    std::cout << "</table>" <<std::endl;
    std::cout << h2(" LRL_WEB, Lattice Representation Library Tool Web Page") << std::endl;
    std::cout << "<br /> by" << std::endl;
    std::cout << "<br /> Lawrence C. Andrews, Herbert J. Bernstein, Ronin Institute for Independent Scholarship," << std::endl;
    std::cout << "<br /><A HREF=mailto:lawrence.andrews@ronininstitute.org>lawrence.andrews@ronininstitute.org</A>" << std::endl;
    std::cout << "<a href=mailto:yayahjb@gmail.com>yayahjb@gmail.com</a><br />" << std::endl;
    std::cout << "<P>" << std::endl;
    std::cout << "<tr>" << std::endl;
    std::cout << "<td colspan=3 align=center><font size=\"-1\">" << std::endl;
    std::cout << "<a href=\"#timeline\">Timeline image</a> by Elizabeth Kincaid, Elizabeth Kincaid Watercolors, <br />" << std::endl;
    std::cout << "<a href=\"#celticknot\">Celtic knots</a> by Susan Jones Davis, <br />" << std::endl;
    std::cout << "Sleeping Dragon line art image by Gordon Dylan Johnson, " << std::endl;
    std::cout << "<a href=\"https://openclipart.org/detail/226147/sleeping-dragon-line-art\">https://openclipart.org/detail/226147/sleeping-dragon-line-art</a></font></td>" << std::endl;
    std::cout << "</tr>" << std::endl;
    xactstr=std::string("<FORM method=POST ACTION=\"http://"+LRL_WEB_HOST+"/~");
    xactstr+=LRL_WEB_USER;
    xactstr+=std::string("/cgi-bin/"+LRL_WEB_CGI+"\" onsubmit=\"return running('00')\">");
    std::cout << xactstr  << std::endl;
    std::cout << "<br />" << std::endl;
    std::cout << "<font size=+2><b>Assorted tools to do various calculations for crystallographic lattices.</b></font>"<< std::endl;
    std::cout << "<br />" << std::endl;
    std::cout << "<table border=0>" << std::endl;
    std::cout << "<tr>" << std::endl;
    std::cout << "<td align=center width=300px><a href=\"http://blondie.arcib.org:8083/~yaya/lrl_web/lrl_web_tutorial.html\" target=\"_blank\"><font size=+1><b>Tutorial</b></font></a></td>" << std::endl;
    std::cout << "<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>" << std::endl;
    std::cout << "<td align=center width=300px><a href=#DeloneChars><font size=+1><b>Table of Delone Characters</b></font></a></td>" << std::endl;
    std::cout << "</tr>" << std::endl;
    std::cout << "<tr>" << std::endl;
    std::cout << "<td></td>" << std::endl;
    std::cout << "<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>" << std::endl;
    std::cout << "<td align=center width=300px><a href=#NiggliFams><font size=+1><b>Table of Niggli Lattice Families</b></font></a></td>" << std::endl;
    std::cout << "</tr>" << std::endl;
    std::cout << "<tr>" << std::endl;
    std::cout << "<td align=center width=300px><a href=\"http://iterate.sf.net/bgaol\" target=\"_blank\"><font size=+1><b>G<sup>6</sup> Bravais General Analysis of Lattices (BGAOL)</b></font></a></td>" << std::endl;
    std::cout << "<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>" << std::endl;
    std::cout << "<td align=center width=300px><a href=\"http://blondie.arcib.org:8084/sauc-1.1.1/\" target=\"_blank\"><font size=+1><b>G<sup>6</sup> Search for Alternative Unit Cells (SAUC)</b></font></a></td>" << std::endl;
    std::cout << "</tr>" << std::endl;
    std::cout << "</table>" << std::endl;

    std::cout << "<STRONG>" << std::endl;
    std::cout << "<font size=+1><b>Please read the <a href=\"#notice\">NOTICE</a> below before use of this web page</b></font>" << std::endl;
    std::cout << "</STRONG>" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"search\"></a>" << std::endl;
    std::cout << "<label for=\"numops\">Number of operation windows: </label>" << std::endl;
    std::cout << "<select name=\"numops\" id=\"numops\" onchange=\"changenumops()\" size=\"1\">" << std::endl;
    if (numops==1) {
      std::cout << "  <option selected value=\"1\">1</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"1\">1</option>" << std::endl;
    }
    if (numops==2) {
      std::cout << "  <option selected value=\"2\">2</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"2\">2</option>" << std::endl;
    }
    if (numops==3) {
      std::cout << "  <option selected value=\"3\">3</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"3\">3</option>" << std::endl;
    }
    if (numops==4) {
      std::cout << "  <option selected value=\"4\">4</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"4\">4</option>" << std::endl;
    }
    if (numops==5) {
      std::cout << "  <option selected value=\"5\">5</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"5\">5</option>" << std::endl;
    }
    if (numops==6) {
      std::cout << "  <option selected value=\"6\">6</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"6\">6</option>" << std::endl;
    }
    if (numops==7) {
      std::cout << "  <option selected value=\"7\">7</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"7\">7</option>" << std::endl;
    }
    if (numops==8) {
      std::cout << "  <option selected value=\"8\">8</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"8\">8</option>" << std::endl;
    }
#if NUMOPS_MAX>8
    if (numops==9) {
      std::cout << "  <option selected value=\"9\">9</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"9\">9</option>" << std::endl;
    }
    if (numops==10) {
      std::cout << "  <option selected value=\"10\">10</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"10\">10</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>10
    if (numops==11) {
      std::cout << "  <option selected value=\"11\">11</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"11\">11</option>" << std::endl;
    }
    if (numops==12) {
      std::cout << "  <option selected value=\"12\">12</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"12\">12</option>" << std::endl;
    }
    if (numops==13) {
      std::cout << "  <option selected value=\"13\">13</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"13\">13</option>" << std::endl;
    }
    if (numops==14) {
      std::cout << "  <option selected value=\"14\">14</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"14\">14</option>" << std::endl;
    }
    if (numops==15) {
      std::cout << "  <option selected value=\"15\">15</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"15\">15</option>" << std::endl;
    }
    if (numops==16) {
      std::cout << "  <option selected value=\"16\">16</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"16\">16</option>" << std::endl;
    }
    if (numops==17) {
      std::cout << "  <option selected value=\"17\">17</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"17\">17</option>" << std::endl;
    }
    if (numops==18) {
      std::cout << "  <option selected value=\"18\">18</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"18\">18</option>" << std::endl;
    }
    if (numops==19) {
      std::cout << "  <option selected value=\"19\">19</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"19\">19</option>" << std::endl;
    }
    if (numops==20) {
      std::cout << "  <option selected value=\"20\">10</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"20\">20</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>10
    if (numops==21) {
      std::cout << "  <option selected value=\"21\">21</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"21\">21</option>" << std::endl;
    }
    if (numops==22) {
      std::cout << "  <option selected value=\"22\">22</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"22\">22</option>" << std::endl;
    }
    if (numops==23) {
      std::cout << "  <option selected value=\"23\">23</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"23\">23</option>" << std::endl;
    }
    if (numops==24) {
      std::cout << "  <option selected value=\"24\">24</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"24\">24</option>" << std::endl;
    }
    if (numops==25) {
      std::cout << "  <option selected value=\"25\">25</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"25\">25</option>" << std::endl;
    }
    if (numops==26) {
      std::cout << "  <option selected value=\"26\">26</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"26\">26</option>" << std::endl;
    }
    if (numops==27) {
      std::cout << "  <option selected value=\"27\">27</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"27\">27</option>" << std::endl;
    }
    if (numops==28) {
      std::cout << "  <option selected value=\"28\">28</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"28\">28</option>" << std::endl;
    }
    if (numops==29) {
      std::cout << "  <option selected value=\"29\">29</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"29\">29</option>" << std::endl;
    }
    if (numops==30) {
      std::cout << "  <option selected value=\"30\">30</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"30\">30</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>30
    if (numops==31) {
      std::cout << "  <option selected value=\"31\">31</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"31\">31</option>" << std::endl;
    }
    if (numops==32) {
      std::cout << "  <option selected value=\"32\">32</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"32\">32</option>" << std::endl;
    }
    if (numops==33) {
      std::cout << "  <option selected value=\"33\">33</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"33\">33</option>" << std::endl;
    }
    if (numops==34) {
      std::cout << "  <option selected value=\"34\">34</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"34\">34</option>" << std::endl;
    }
    if (numops==35) {
      std::cout << "  <option selected value=\"35\">35</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"35\">35</option>" << std::endl;
    }
    if (numops==36) {
      std::cout << "  <option selected value=\"36\">36</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"36\">36</option>" << std::endl;
    }
    if (numops==37) {
      std::cout << "  <option selected value=\"37\">37</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"37\">37</option>" << std::endl;
    }
    if (numops==38) {
      std::cout << "  <option selected value=\"38\">38</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"38\">38</option>" << std::endl;
    }
    if (numops==39) {
      std::cout << "  <option selected value=\"39\">39</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"39\">39</option>" << std::endl;
    }
    if (numops==40) {
      std::cout << "  <option selected value=\"40\">40</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"40\">40</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>40
    if (numops==41) {
      std::cout << "  <option selected value=\"41\">41</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"41\">41</option>" << std::endl;
    }
    if (numops==42) {
      std::cout << "  <option selected value=\"42\">42</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"42\">42</option>" << std::endl;
    }
    if (numops==43) {
      std::cout << "  <option selected value=\"43\">43</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"43\">43</option>" << std::endl;
    }
    if (numops==44) {
      std::cout << "  <option selected value=\"44\">44</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"44\">44</option>" << std::endl;
    }
    if (numops==45) {
      std::cout << "  <option selected value=\"45\">45</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"45\">45</option>" << std::endl;
    }
    if (numops==46) {
      std::cout << "  <option selected value=\"46\">46</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"46\">46</option>" << std::endl;
    }
    if (numops==47) {
      std::cout << "  <option selected value=\"47\">47</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"47\">47</option>" << std::endl;
    }
    if (numops==48) {
      std::cout << "  <option selected value=\"48\">48</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"48\">48</option>" << std::endl;
    }
    if (numops==49) {
      std::cout << "  <option selected value=\"49\">49</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"49\">49</option>" << std::endl;
    }
    if (numops==50) {
      std::cout << "  <option selected value=\"50\">50</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"50\">50</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>50
    if (numops==51) {
      std::cout << "  <option selected value=\"51\">51</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"51\">51</option>" << std::endl;
    }
    if (numops==52) {
      std::cout << "  <option selected value=\"52\">52</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"52\">52</option>" << std::endl;
    }
    if (numops==53) {
      std::cout << "  <option selected value=\"53\">53</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"53\">53</option>" << std::endl;
    }
    if (numops==54) {
      std::cout << "  <option selected value=\"54\">54</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"54\">54</option>" << std::endl;
    }
    if (numops==55) {
      std::cout << "  <option selected value=\"55\">55</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"55\">55</option>" << std::endl;
    }
    if (numops==56) {
      std::cout << "  <option selected value=\"56\">56</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"56\">56</option>" << std::endl;
    }
    if (numops==57) {
      std::cout << "  <option selected value=\"57\">57</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"57\">57</option>" << std::endl;
    }
    if (numops==58) {
      std::cout << "  <option selected value=\"58\">58</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"58\">58</option>" << std::endl;
    }
    if (numops==59) {
      std::cout << "  <option selected value=\"59\">59</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"59\">59</option>" << std::endl;
    }
    if (numops==60) {
      std::cout << "  <option selected value=\"60\">60</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"60\">60</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>60
    if (numops==61) {
      std::cout << "  <option selected value=\"61\">61</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"61\">61</option>" << std::endl;
    }
    if (numops==62) {
      std::cout << "  <option selected value=\"62\">62</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"62\">62</option>" << std::endl;
    }
    if (numops==63) {
      std::cout << "  <option selected value=\"63\">63</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"63\">63</option>" << std::endl;
    }
    if (numops==64) {
      std::cout << "  <option selected value=\"64\">64</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"64\">64</option>" << std::endl;
    }
    if (numops==65) {
      std::cout << "  <option selected value=\"65\">65</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"65\">65</option>" << std::endl;
    }
    if (numops==66) {
      std::cout << "  <option selected value=\"66\">66</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"66\">66</option>" << std::endl;
    }
    if (numops==67) {
      std::cout << "  <option selected value=\"67\">67</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"67\">67</option>" << std::endl;
    }
    if (numops==68) {
      std::cout << "  <option selected value=\"68\">68</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"68\">68</option>" << std::endl;
    }
    if (numops==69) {
      std::cout << "  <option selected value=\"69\">69</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"69\">69</option>" << std::endl;
    }
    if (numops==70) {
      std::cout << "  <option selected value=\"70\">70</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"70\">70</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>70
    if (numops==71) {
      std::cout << "  <option selected value=\"71\">71</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"71\">71</option>" << std::endl;
    }
    if (numops==72) {
      std::cout << "  <option selected value=\"72\">72</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"72\">72</option>" << std::endl;
    }
    if (numops==73) {
      std::cout << "  <option selected value=\"73\">73</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"73\">73</option>" << std::endl;
    }
    if (numops==74) {
      std::cout << "  <option selected value=\"74\">74</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"74\">74</option>" << std::endl;
    }
    if (numops==75) {
      std::cout << "  <option selected value=\"75\">75</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"75\">75</option>" << std::endl;
    }
    if (numops==76) {
      std::cout << "  <option selected value=\"76\">76</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"76\">76</option>" << std::endl;
    }
    if (numops==77) {
      std::cout << "  <option selected value=\"77\">77</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"77\">77</option>" << std::endl;
    }
    if (numops==78) {
      std::cout << "  <option selected value=\"78\">78</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"78\">78</option>" << std::endl;
    }
    if (numops==79) {
      std::cout << "  <option selected value=\"79\">79</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"79\">79</option>" << std::endl;
    }
    if (numops==80) {
      std::cout << "  <option selected value=\"80\">80</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"80\">80</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>80
    if (numops==81) {
      std::cout << "  <option selected value=\"81\">81</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"81\">81</option>" << std::endl;
    }
    if (numops==82) {
      std::cout << "  <option selected value=\"82\">82</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"82\">82</option>" << std::endl;
    }
    if (numops==83) {
      std::cout << "  <option selected value=\"83\">83</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"83\">83</option>" << std::endl;
    }
    if (numops==84) {
      std::cout << "  <option selected value=\"84\">84</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"84\">84</option>" << std::endl;
    }
    if (numops==85) {
      std::cout << "  <option selected value=\"85\">85</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"85\">85</option>" << std::endl;
    }
    if (numops==86) {
      std::cout << "  <option selected value=\"86\">86</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"86\">86</option>" << std::endl;
    }
    if (numops==87) {
      std::cout << "  <option selected value=\"87\">87</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"87\">87</option>" << std::endl;
    }
    if (numops==88) {
      std::cout << "  <option selected value=\"88\">88</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"88\">88</option>" << std::endl;
    }
    if (numops==89) {
      std::cout << "  <option selected value=\"89\">89</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"89\">89</option>" << std::endl;
    }
    if (numops==90) {
      std::cout << "  <option selected value=\"90\">90</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"90\">90</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>90
    if (numops==91) {
      std::cout << "  <option selected value=\"91\">91</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"91\">91</option>" << std::endl;
    }
    if (numops==92) {
      std::cout << "  <option selected value=\"92\">92</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"92\">92</option>" << std::endl;
    }
    if (numops==93) {
      std::cout << "  <option selected value=\"93\">93</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"93\">93</option>" << std::endl;
    }
    if (numops==94) {
      std::cout << "  <option selected value=\"94\">94</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"94\"94</option>" << std::endl;
    }
    if (numops==95) {
      std::cout << "  <option selected value=\"95\">95</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"95\">95</option>" << std::endl;
    }
    if (numops==96) {
      std::cout << "  <option selected value=\"96\">96</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"96\">96</option>" << std::endl;
    }
    if (numops==97) {
      std::cout << "  <option selected value=\"97\">97</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"97\">97</option>" << std::endl;
    }
    if (numops==98) {
      std::cout << "  <option selected value=\"98\">98</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"98\">98</option>" << std::endl;
    }
    if (numops==99) {
      std::cout << "  <option selected value=\"99\">99</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"99\">99</option>" << std::endl;
    }
#endif
    std::cout << "</select>&nbsp;&nbsp;" << std::endl;
    std::cout << "<INPUT type=\"submit\" id=\"submit_00\" onsubmit=\"return running('00')\">" << std::endl;
    std::cout << "<INPUT type=\"reset\">" << std::endl;
    std::cout << "<br />" << std::endl;
    std::cout << "<input type=hidden name=\"OutputStyle\" value=\"TEXT\" />" << std::endl;
    std::cout << "<input type=hidden name=\"ScrollTo\" id=\"ScrollTo\" value=\""+twodig_array[numops]+"\" />" << std::endl;
    std::cout << "  <tr>" << std::endl;
    std::cout << "  <td colspan=3 align=\"center\">" << std::endl;
    std::cout << "  </td>" << std::endl;
    std::cout << "  </tr>" << std::endl;
    std::cout << "<table border=2>" << std::endl;
    std::cout << "<tr><td valign=top>" << std::endl;
    std::cout << "  <table>" << std::endl;
    std::string currentoutput=string("");
    char * prevoutbuf = (char *)malloc(1);
    size_t prevoutbuflen = 1;
    prevoutbuf[0] = 0;
    for (numop=1; numop <=NUMOPS_MAX; numop++) {
      std::string chain;
      std::string operation;
      std::string selected;
      std::string lrl_web_data=string(tmp_lrl_web+"/lrl_web_data_"+twodig_array[numop]);
      std::string lrl_web_output=string(tmp_lrl_web+"/lrl_web_output_"+twodig_array[numop]);
      std::string lwd=string(tmp_lrl_web+"lwd_"+twodig_array[numop]);
      std::string prioroutput=currentoutput;
      std::string currentoutput=lrl_web_output;
      std::string lrl_web_data_cmdgen_ngen;
      std::string lrl_web_data_cmdgen_ltype;
      std::string lrl_web_data_cmdpath_npath;
      std::string lrl_web_data_cmdperturb_npert;
      std::string lrl_web_data_cmdperturb_ppk;
      std::string lrl_web_data_cmdscale_type;
      std::string lrl_web_data_cmdsella_rawprefix;
      std::string lrl_web_data_cmdsella_htmlprefix;
      std::string lrl_web_data_cmdtos6l_type;
      std::string lrl_web_data_dirichlet_rawprefix;
      std::string lrl_web_data_dirichlet_htmlprefix;
      std::string lrl_web_data_plotc3_rawprefix;
      std::string lrl_web_data_plotc3_htmlprefix;
      std::string active=std::string("\"display:inline\"");
      if(numop > numops) active=std::string("\"display:none\"");
      chain_iter =  formData.getElement("chain_"+twodig_array[numop]);
      if (chain_iter == formData.getElements().end()) {
        if (numop < 2 ) {
          chain = "new_input";
        } else {
          chain = "chain_input";
        }
      } else {
        chain = chain_iter->getValue();
      }
      if (chain.compare("new_input")==0 || numop < 2) {
        lrl_web_data_iter = formData.getElement("lrl_web_data_"+twodig_array[numop]);
        lrl_web_data_data = lrl_web_data_iter->getValue();
      } else {
        lrl_web_data_iter = formData.getElement("lrl_web_output_"+twodig_array[numop-1]);
        lrl_web_data_data = std::string(prevoutbuf);
      }
      // std::cout << "<tr><td colspan=\"3\">" << "lrl_web_data_data"+twodig_array[numop] << "<pre>"<<(lrl_web_data_data).c_str()<<"</pre>" <<"</td></tr>" << std::endl;
      std::string at=std::string("");
      std::string path=std::string(tmp_lrl_web+"/lrl_web_data_"+twodig_array[numop]);
      std::string opmod=std::string("");
      if(string_to_file(at.c_str(), path.c_str(), lrl_web_data_data.c_str())) exit(-1);
      lrl_web_output_iter = formData.getElement("lrl_web_output"+twodig_array[numop]);
      operation_iter = formData.getElement("operation_"+twodig_array[numop]);
      if (operation_iter == formData.getElements().end()) {
        operation = "NoOp";
      } else {
        operation = operation_iter->getValue();
      }
      lrl_web_data_cmdgen_ngen=std::string("1");
      lrl_web_data_cmdgen_ltype=std::string("all");
      lrl_web_data_cmdperturb_npert=std::string("20");
      lrl_web_data_cmdperturb_ppk=std::string("1");
      lrl_web_data_cmdpath_npath=std::string("20");
      lrl_web_data_cmdscale_type=std::string("S6");
      lrl_web_data_cmdsella_rawprefix=tmp_lrl_web+std::string("/");
      lrl_web_data_cmdsella_htmlprefix=html_tmp_lrl_web+std::string("/");
      lrl_web_data_cmdtos6l_type=std::string("S6L");
      lrl_web_data_dirichlet_rawprefix=tmp_lrl_web+std::string("/");
      lrl_web_data_dirichlet_htmlprefix=html_tmp_lrl_web+std::string("/");
      lrl_web_data_plotc3_rawprefix=tmp_lrl_web+std::string("/");
      lrl_web_data_plotc3_htmlprefix=html_tmp_lrl_web+std::string("/");
      if (operation=="CmdGen") {
        lrl_web_data_cmdgen_ngen_iter=formData.getElement("lrl_web_data_"+twodig_array[numop]+"_cmdgen_ngen");
        lrl_web_data_cmdgen_ltype_iter=formData.getElement("lrl_web_data_"+twodig_array[numop]+"_cmdgen_ltype");
        lrl_web_data_cmdgen_ngen = (lrl_web_data_cmdgen_ngen_iter==formData.getElements().end())?std::string("1"):lrl_web_data_cmdgen_ngen_iter->getValue();
        lrl_web_data_cmdgen_ltype = (lrl_web_data_cmdgen_ltype_iter==formData.getElements().end())?std::string("all"):lrl_web_data_cmdgen_ltype_iter->getValue();
        opmod=(std::string(" ")+lrl_web_data_cmdgen_ngen+std::string(" ")+lrl_web_data_cmdgen_ltype);
        // std::cout << "<tr><td colspan=\"3\">" << "lrl_web_data_"+twodig_array[numop]+"_cmdgen_ngen" << (opmod).c_str() <<"</td></tr>" << std::endl;
      } else if (operation=="CmdPath") {
        lrl_web_data_cmdpath_npath_iter=formData.getElement("lrl_web_data_"+twodig_array[numop]+"_cmdpath_npath");
        lrl_web_data_cmdpath_npath = (lrl_web_data_cmdpath_npath_iter==formData.getElements().end())?std::string("20"):lrl_web_data_cmdpath_npath_iter->getValue();
        opmod=(std::string(" ")+lrl_web_data_cmdpath_npath);
        // std::cout << "<tr><td colspan=\"3\">" << "lrl_web_data_"+twodig_array[numop]+"_cmdpath_npath" << (opmod).c_str() <<"</td></tr>" << std::endl;
      } else if (operation=="CmdPerturb") {
        lrl_web_data_cmdperturb_npert_iter=formData.getElement("lrl_web_data_"+twodig_array[numop]+"_cmdperturb_npert");
        lrl_web_data_cmdperturb_ppk_iter=formData.getElement("lrl_web_data_"+twodig_array[numop]+"_cmdperturb_ppk");
        lrl_web_data_cmdperturb_npert = (lrl_web_data_cmdperturb_npert_iter==formData.getElements().end())?std::string("20"):lrl_web_data_cmdperturb_npert_iter->getValue();
        lrl_web_data_cmdperturb_ppk = (lrl_web_data_cmdperturb_ppk_iter==formData.getElements().end())?std::string("1"):lrl_web_data_cmdperturb_ppk_iter->getValue();
        opmod=(std::string(" ")+lrl_web_data_cmdperturb_npert+std::string(" ")+lrl_web_data_cmdperturb_ppk);
        // std::cout << "<tr><td colspan=\"3\">" << "lrl_web_data_"+twodig_array[numop]+"_cmdperturb_npert" << (opmod).c_str() <<"</td></tr>" << std::endl;
      } else if (operation=="CmdScale") {
        lrl_web_data_cmdscale_type_iter=formData.getElement("lrl_web_data_"+twodig_array[numop]+"_cmdscale_type");
        lrl_web_data_cmdscale_type = (lrl_web_data_cmdscale_type_iter==formData.getElements().end())?std::string("S6"):lrl_web_data_cmdscale_type_iter->getValue();
        opmod=(std::string(" ")+lrl_web_data_cmdscale_type);
        // std::cout << "<tr><td colspan=\"3\">" << "lrl_web_data_"+twodig_array[numop]+"_cmdscale_type" << (opmod).c_str() <<"</td></tr>" << std::endl;
      } else if (operation=="CmdSella") {
        lrl_web_data_cmdsella_rawprefix = tmp_lrl_web+std::string("/");
        lrl_web_data_cmdsella_htmlprefix = html_tmp_lrl_web+std::string("/");
        opmod=(std::string(" --host blondie.arcib.org:8083 ")
          +std::string(" --rawprefix ")+std::string(lrl_web_data_cmdsella_rawprefix)
          +std::string(" --htmlprefix ")+std::string(lrl_web_data_cmdsella_htmlprefix));
        // std::cout << "<tr><td colspan=\"3\">" << "lrl_web_data_"+twodig_array[numop]+"_cmdsella_prefixes" << (opmod).c_str() <<"</td></tr>" << std::endl;
      } else if (operation=="CmdToS6L") {
        lrl_web_data_cmdtos6l_type_iter=formData.getElement("lrl_web_data_"+twodig_array[numop]+"_cmdtos6l_type");
        lrl_web_data_cmdtos6l_type = (lrl_web_data_cmdtos6l_type_iter==formData.getElements().end())?std::string("S6L"):lrl_web_data_cmdtos6l_type_iter->getValue();
        opmod=(std::string(" ")+lrl_web_data_cmdtos6l_type);
        // std::cout << "<tr><td colspan=\"3\">" << "lrl_web_data_"+twodig_array[numop]+"_cmdtos6l_type" << (opmod).c_str() <<"</td></tr>" << std::endl;
      } else if (operation=="PlotC3") {
        lrl_web_data_plotc3_rawprefix = tmp_lrl_web+std::string("/");
        lrl_web_data_plotc3_htmlprefix = html_tmp_lrl_web+std::string("/");
        opmod=(std::string(" --host blondie.arcib.org:8083 ")
          +std::string(" --rawprefix ")+std::string(lrl_web_data_plotc3_rawprefix)
          +std::string(" --htmlprefix ")+std::string(lrl_web_data_plotc3_htmlprefix));
        // std::cout << "<tr><td colspan=\"3\">" << "lrl_web_data_"+twodig_array[numop]+"_plotc3_prefixes" << (opmod).c_str() <<"</td></tr>" << std::endl;
      } else if (operation=="Dirichlet") {
        lrl_web_data_dirichlet_rawprefix = tmp_lrl_web+std::string("/");
        lrl_web_data_dirichlet_htmlprefix = html_tmp_lrl_web+std::string("/");
        opmod=(std::string(" --host blondie.arcib.org:8083 ")
          +std::string(" --rawprefix ")+std::string(lrl_web_data_dirichlet_rawprefix)
          +std::string(" --htmlprefix ")+std::string(lrl_web_data_dirichlet_htmlprefix));
        // std::cout << "<tr><td colspan=\"3\">" << "lrl_web_data_"+twodig_array[numop]+"_dirichlet_prefixes" << (opmod).c_str() <<"</td></tr>" << std::endl;
      }
      std::string oppath=std::string(tmp_lrl_web+"/operation_"+twodig_array[numop]);
      if(string_to_file(at.c_str(), oppath.c_str(), (operation+opmod).c_str())) {
         std::cout << "<tr><td colspan=\"3\">string_to_file of"+tmp_lrl_web+"/operation_"+twodig_array[numop]+" failed</td></tr>"<<std::endl;
      }
      std::string xprocess_next_output=
        std::string("/home/")+LRL_WEB_USER+std::string("/public_html/cgi-bin/")+string("")+string("process_next_output.bash")+
        std::string(" ")+lrl_web_output+std::string(" ")+path+std::string(" ")+oppath;
        // std::cout << "<tr><td colspan=\"3\">" << xprocess_next_output <<"</td></tr>" << std::endl;
      char outlength[128];
      size_t outlen;
      if (do_exec_to_buffer(xprocess_next_output.c_str(),outlength,128)!=0) {
         std::cout << "<tr><td colspan=\"3\">process_next_output of"+tmp_lrl_web+"/lrl_web_output_"+twodig_array[numop]+" failed</td></tr>"<<std::endl;
         std::cout << "<tr><td colspan=\"3\">"<< std::string(outlength) << "</td></tr>"<<std::endl;
      }
      outlen=atol(outlength);
      // std::cout << "<tr><td colspan=\"3\">" << xprocess_next_output << "</td></tr>" << std::endl;
      // std::cout << "<tr><td colspan=\"3\">" << outlen << "</td></tr>" << std::endl;
      selected = "";
      if (numop > 1) {
        std::cout << "    <tr><td colspan=3 align=center><div name=\"hrule_"+twodig_array[numop]+"\" id=\"hrule_"+twodig_array[numop]+"\" style="+active+">" << std::endl;
        std::cout << "    <img src=http://blondie.arcib.org:8083/~yaya/lrl_web/sep5.svg>" << std::endl;
        std::cout << "    </div></td></tr>" << std::endl;
      }
      std::cout << "  <tr>" << std::endl;
      std::cout << "  <td valign=top>" << std::endl;
      std::cout << "  <a name=\"mark_"+twodig_array[numop]+"\" id=\"mark_"+twodig_array[numop]+"\" />" << std::endl;
      if (numop ==NUMOPS_MAX) {
        std::cout << "  <a name=\"mark_000\" id=\"mark_000\" />" << std::endl;
      }
      std::cout << "  <div id=\"block_"+twodig_array[numop]+"\" style="+active+">" << std::endl;
      std::cout << "  <h1 align=left>"<<twodig_array[numop]<<"</h1><br />" << std::endl;  
      std::cout << "  <label for=\"chain_"+twodig_array[numop]+"\">Source of data:</label><br />" << std::endl;
      std::cout << "  <select name=\"chain_"+twodig_array[numop]+"\" id=\"chain_"+twodig_array[numop]+
          "\" size=\"1\" onchange=\"setchaininput('" << numop << "')\">" << std::endl;
      selected=chain.compare("new_input")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"new_input\">use new input</option>" << std::endl;
      selected=chain.compare("chain_input")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"chain_input\">use prior output</option>" << std::endl;
      std::cout << "  </select>" << std::endl;
      std::cout << "  <br />" << std::endl;
      std::cout << "  <br />" << std::endl;
      std::cout << "  <label for=\"submit_"+twodig_array[numop]+"\">Submit all data:</label><br />" << std::endl;
      std::cout << "  <INPUT type=\"submit\" id=\"submit_"+twodig_array[numop]+"\" onsubmit=\"return running('"+twodig_array[numop]+"')\">" << std::endl;
      std::cout << "  <br />" << std::endl;
      std::cout << "  <br />" << std::endl;
      std::cout << "  </div>" << std::endl;
      std::cout << std::endl;

      std::cout << "  <div name=\"block_"+twodig_array[numop]+"_running\" id=\"block_"+twodig_array[numop]+"_running\" style=\"display:none\">" << std::endl;
      std::cout << "  <label for=\"progress_img_"+twodig_array[numop]+"\">Running:</label><br />" << std::endl;
      std::cout << "  <img src=\"http://blondie.arcib.org:8083/~yaya/images/progress_small.gif\" id=\"progress_img_"+twodig_array[numop]+"\" alt=\"running\" />" << std::endl;
      std::cout << "  <br />" << std::endl;
      std::cout << "  </div>" << std::endl;

      std::cout << std::endl;
      std::cout << "  </td>" << std::endl;
      std::cout << "  <td align=left>" << std::endl;
      std::cout << "  <div id=\"block_"+twodig_array[numop]+"a\" style="+active+">" << std::endl; 
      std::cout << "  <label for=\"operation_"+twodig_array[numop]+"\">Select an operation:</label><br />" << std::endl;
      std::cout << "  <select name=\"operation_"+twodig_array[numop]+"\" id=\"operation_"+twodig_array[numop]+"\" size=\"28\" onchange=\"changeoperation(\'"+twodig_array[numop]+"')\">" << std::endl;
      std::cout << "  <optgroup label=\"Information\">" << std::endl;
      selected=operation.compare("NoOp")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"NoOp\"><b>Check Input</b></option>" << std::endl;
      selected=operation.compare("CmdDists")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdDists\"><b>Distances</b>: compute NCDist and CS6Dist distances</option>" << std::endl;
      selected=(operation.compare("CmdVolume")==0)?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdVolume\"><b>Volume</b>:  compute volumes of listed cells</option>" << std::endl;
      std::cout << "  </optgroup>" << std::endl;
      std::cout << "  <optgroup label=\"Graphical Information\">"  << std::endl;
      selected=operation.compare("CmdSella")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdSella\"><b>Sella</b>:  apply Sella algorithm</option>" << std::endl;
      selected=operation.compare("PlotC3")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"PlotC3\"><b>PlotC3</b>:  draw C3 plot of listed cells</option>" << std::endl;
      selected=operation.compare("Dirichlet")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"Dirichlet\"><b>Dirichlet</b>:  draw Dirichlet cells of listed cells</option>" << std::endl;
      std::cout << "  </optgroup>" << std::endl;
      std::cout << "  <optgroup label=\"Output Only\">"  << std::endl;
      selected=operation.compare("CmdGen")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdGen\"><b>Generate cells</b>: Generate cells of a particular type or types</option>" << std::endl;
      std::cout << "  </optgroup>" << std::endl;
      std::cout << "  <optgroup label=\"Type Conversion\">" << std::endl;
      selected=operation.compare("CmdToB4")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToB4\"><b>To B4</b>: compute Bravais tetrahedron (B4)</option>" << std::endl;
      selected=operation.compare("CmdToC3")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToC3\"><b>To C3</b>: compute complex cell presentation (C3)</option>" << std::endl;
      selected=operation.compare("CmdToCell")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToCell\"><b>To Cell</b>: compute side-angle cells (a, b, c, &alpha;, &beta;, &gamma;)</option>" << std::endl;
      selected=operation.compare("CmdToG6")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToG6\"><b>To G6</b>: compute G6 version of cells</option>" << std::endl;
      selected=operation.compare("CmdToS6")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToS6\"><b>To S6</b>: compute S6 version of cells</option>" << std::endl;
      selected=operation.compare("CmdToS6L")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToS6L\"><b>To S6L</b>: compute linearized S6 or Root Invariant version of cells</option>" << std::endl;
      selected=operation.compare("CmdToU")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToU\"><b>To Dirichlet (unsorted)</b>: compute unsorted Dirichlet cells (dc7unsrt)</option>" << std::endl;
      selected=operation.compare("CmdToV7")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToV7\"><b>To V7</b>: compute V7 version of cells</option>" << std::endl;
      std::cout << "  </optgroup>" << std::endl;
      std::cout << "  <optgroup label=\"Reduction\">" << std::endl;
      selected=operation.compare("CmdDelone")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdDelone\"><b>Delone</b>:compute Selling-reduced primitive cells</option>" << std::endl;
      selected=operation.compare("CmdNiggli")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdNiggli\"><b>Niggli</b>:compute Niggli-reduced primitive cells</option>" << std::endl;
      std::cout << "  </optgroup>" << std::endl;
      std::cout << "  <optgroup label=\"Modify Input\">" << std::endl;
      selected=operation.compare("CmdLM")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdLM\"><b>Matching</b>: apply Lattice Matching algorithm to listed cells</option>" << std::endl;
      selected=operation.compare("CmdPath")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdPath\"><b>Path</b>: compute path between pairs of cells</option>" << std::endl;
      selected=operation.compare("CmdPerturb")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdPerturb\"><b>Perturb</b>: compute perturbed versions of input cells</option>" << std::endl;
      selected=operation.compare("CmdS6Refl")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdS6Refl\"><b>S6Reflections</b>: apply S6 reflections to input cells</option>" << std::endl;
      selected=operation.compare("CmdScale")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdScale\"><b>Scale</b>: rescale cells to reference cell</option>" << std::endl;
      std::cout << "  </optgroup>" << std::endl;
      std::cout << "  </select>" << std::endl;
      std::cout << "  </div>" << std::endl;
      std::cout << "  </td>" << std::endl;
      std::cout << "  <td align=left>" << std::endl;

      if (operation.compare("CmdGen")==0) {
        std::cout << "  <div id=\"block_"+twodig_array[numop]+"b_cmdgen\" style=\"display:inline\">"  <<  std::endl;
      } else {
        std::cout << "  <div id=\"block_"+twodig_array[numop]+"b_cmdgen\" style=\"display:none\">"  <<  std::endl;
      }
      std::cout << "  <label for=\"lrl_web_data_"+twodig_array[numop]+"_cmdgen_ngen\">Number of each type:</label>&nbsp;"  <<  std::endl;
      std::cout << "  <input id=\"lrl_web_data_"+twodig_array[numop]+"_cmdgen_ngen\" name=\"lrl_web_data_"+twodig_array[numop]+"_cmdgen_ngen\" type=\"number\" value=\""
        +lrl_web_data_cmdgen_ngen+"\" />&nbsp;&nbsp;"  <<  std::endl;
      std::cout << "  <label for=\"lrl_web_data_"+twodig_array[numop]+"_cmdgen_ltype\">Lattice type:</label>&nbsp;"  <<  std::endl;
      std::cout << "  <input id=\"lrl_web_data_"+twodig_array[numop]+"_cmdgen_ltype\" name=\"lrl_web_data_"+twodig_array[numop]+"_cmdgen_ltype\" type=\"text\" value=\""
        +lrl_web_data_cmdgen_ltype+"\" />"  <<  std::endl;
      std::cout << "  <br />"  <<  std::endl;
      std::cout << "  </div>"  <<  std::endl;

      if (operation.compare("CmdPerturb")==0) {
        std::cout << "  <div id=\"block_"+twodig_array[numop]+"b_cmdperturb\" style=\"display:inline\">"  <<  std::endl;
      } else {
        std::cout << "  <div id=\"block_"+twodig_array[numop]+"b_cmdperturb\" style=\"display:none\">"  <<  std::endl;
      }
      std::cout << "  <label for=\"lrl_web_data_"+twodig_array[numop]+"_cmdperturb_npert\">Number of perturbations:</label>&nbsp;"  <<  std::endl;
      std::cout << "  <input id=\"lrl_web_data_"+twodig_array[numop]+"_cmdperturb_npert\" name=\"lrl_web_data_"+twodig_array[numop]+"_cmdperturb_npert\" type=\"number\" value=\""
        +lrl_web_data_cmdperturb_npert+"\" min=\"1\"/>&nbsp;&nbsp;" <<  std::endl;
      std::cout << "  <label for=\"lrl_web_data_"+twodig_array[numop]+"_cmdperturb_ppk\">Parts per 1000:</label>&nbsp;"  <<  std::endl;
      std::cout << "  <input id=\"lrl_web_data_"+twodig_array[numop]+"_cmdperturb_ppk\" name=\"lrl_web_data_"+twodig_array[numop]+"_cmdperturb_ppk\" type=\"number\" value=\""
        +lrl_web_data_cmdperturb_ppk+"\" min=\"1\" max=\"1000\"/>"  <<  std::endl;
      std::cout << "  <br />"  << std::endl;
      std::cout << "  </div>"  << std::endl;

      if (operation.compare("CmdPath")==0) {
        std::cout << "  <div id=\"block_"+twodig_array[numop]+"b_cmdpath\" style=\"display:inline\">"  <<  std::endl;
      } else {
        std::cout << "  <div id=\"block_"+twodig_array[numop]+"b_cmdpath\" style=\"display:none\">"  <<  std::endl;
      }
      std::cout << "  <label for=\"lrl_web_data_"+twodig_array[numop]+"_cmdpath_npath\">Number of steps in the path:</label>&nbsp;"  <<  std::endl;
      std::cout << "  <input id=\"lrl_web_data_"+twodig_array[numop]+"_cmdpath_npath\" name=\"lrl_web_data_"+twodig_array[numop]+"_cmdpath_npath\" type=\"number\" value=\""
        +lrl_web_data_cmdpath_npath+"\" min=\"1\"/>" <<  std::endl;
      std::cout << "  <br />"  << std::endl;
      std::cout << "  </div>"  << std::endl;

      if (operation.compare("CmdScale")==0) {
        std::cout << "  <div id=\"block_"+twodig_array[numop]+"b_cmdscale\" style=\"display:inline\">"  <<  std::endl;
      } else {
        std::cout << "  <div id=\"block_"+twodig_array[numop]+"b_cmdscale\" style=\"display:none\">"  <<  std::endl;
      }
      std::cout << "  <label for=\"lrl_web_data_"+twodig_array[numop]+"_cmdscale_type\">Type of scaled cells: S6, V7, DC7u, or RI</label>&nbsp;"  <<  std::endl;
      std::cout << "  <select id=\"lrl_web_data_"+twodig_array[numop]+"_cmdscale_type\" name=\"lrl_web_data_"+twodig_array[numop]+"_cmdscale_type\">" <<  std::endl;
      if (lrl_web_data_cmdtos6l_type[0]=='S'||lrl_web_data_cmdtos6l_type[0]=='s') {
        std::cout << "     <option selected value=\"S6\">S6</option>"  <<  std::endl;
      } else {
        std::cout << "     <option value=\"S6\">S6</option>"  <<  std::endl;
      }
      if (lrl_web_data_cmdtos6l_type[0]=='V'||lrl_web_data_cmdtos6l_type[0]=='v') {
        std::cout << "     <option selected value=\"V7\">V7</option>"  <<  std::endl;
      } else {
        std::cout << "     <option value=\"V7\">V7</option>"  <<  std::endl;
      }
      if (lrl_web_data_cmdtos6l_type[0]=='D'||lrl_web_data_cmdtos6l_type[0]=='d') {
        std::cout << "     <option selected value=\"DC7u\">DC7u</option>"  <<  std::endl;
      } else {
        std::cout << "     <option value=\"DC7u\">DC7u</option>"  <<  std::endl;
      }
      if (lrl_web_data_cmdtos6l_type[0]=='R'||lrl_web_data_cmdtos6l_type[0]=='r') {
        std::cout << "     <option selected value=\"RI\">RI</option>"  <<  std::endl;
      } else {
        std::cout << "     <option value=\"RI\">RI</option>"  <<  std::endl;
      }
      std::cout << "  </select>"  << std::endl;
      std::cout << "  <br />"  << std::endl;
      std::cout << "  </div>"  << std::endl;

      if (operation.compare("CmdToS6L")==0) {
        std::cout << "  <div id=\"block_"+twodig_array[numop]+"b_cmdtos6l\" style=\"display:inline\">"  <<  std::endl;
      } else {
        std::cout << "  <div id=\"block_"+twodig_array[numop]+"b_cmdtos6l\" style=\"display:none\">"  <<  std::endl;
      }
      std::cout << "  <label for=\"lrl_web_data_"+twodig_array[numop]+"_cmdtos6l_type\">Type of linearized S6: S6L, RI or blank for both:</label>&nbsp;"  <<  std::endl;
      std::cout << "  <select id=\"lrl_web_data_"+twodig_array[numop]+"_cmdtos6l_type\" name=\"lrl_web_data_"+twodig_array[numop]+"_cmdtos6l_type\">" <<  std::endl;
      if (lrl_web_data_cmdtos6l_type[0]=='S'||lrl_web_data_cmdtos6l_type[0]=='s') {
      std::cout << "     <option selected value=\"S6L\">linearized S6</option>"  <<  std::endl;
      std::cout << "     <option value=\"RI\">root invariant</option>"  <<  std::endl;
      std::cout << "     <option value=\"  \">both S6L and RI</option>"  <<  std::endl;
      } else if (lrl_web_data_cmdtos6l_type[0]=='R'||lrl_web_data_cmdtos6l_type[0]=='r') {
      std::cout << "     <option value=\"S6L\">linearized S6</option>"  <<  std::endl;
      std::cout << "     <option selected value=\"RI\">root invariant</option>"  <<  std::endl;
      std::cout << "     <option value=\"  \">both S6L and RI</option>"  <<  std::endl;
      } else {
      std::cout << "     <option value=\"S6L\">linearized S6</option>"  <<  std::endl;
      std::cout << "     <option value=\"RI\">root invariant</option>"  <<  std::endl;
      std::cout << "     <option selectedvalue=\"  \">both S6L and RI</option>"  <<  std::endl;
      }
      std::cout << "  </select>"  << std::endl;
      std::cout << "  <br />"  << std::endl;
      std::cout << "  </div>"  << std::endl;

      if (chain.compare("new_input")==0 || numop < 2) {
        std::cout << "  <div id=\"block_"+twodig_array[numop]+"b"+"\" style="+active+"> " << std::endl;
      } else {
        std::cout << "  <div id=\"block_"+twodig_array[numop]+"b"+"\" style=\"display:none\" >" <<std::endl;
      }
      std::cout << "  <label for=\"lrl_web_data_"+twodig_array[numop]+"\">Input data:</label><br />" << std::endl;
      std::cout << "  <textarea name=\"lrl_web_data_"+twodig_array[numop]+"\" id=\"lrl_web_data_"
          +twodig_array[numop]+"\" rows=\"9\" cols=\"100\" placeholder=\"command input ... followed by end\">";
      if (lrl_web_data_iter==formData.getElements().end()) {
          lrl_web_data_data=std::string("");
      } else { lrl_web_data_data=lrl_web_data_iter->getValue();
      }
      std::cout << lrl_web_data_data << "</textarea>" << std::endl;
      std::cout << "  </div>" << std::endl;
      std::cout << "  <div id=\"block_"+twodig_array[numop]+"c"+"\" style="+active+">" << std::endl;
      std::cout << "  <br />" << std::endl;
      std::cout << "  <label for=\"lrl_web_output_"+twodig_array[numop]+"\">Tool Output:</label><br />" << std::endl;
      std::cout << "  <div name=\"lrl_web_output_"+twodig_array[numop]+"\" id=\"lrl_web_output_"+twodig_array[numop]+"\"" << std::endl;
      std::cout << "   style=\"overflow-y: auto;text-align: left;height: 108px;width:720px; border-style: solid; border-width: thin; resize: both;\" >" << std::endl;
      string processed_output=string(std::string("/home/")+LRL_WEB_USER+std::string("/public_html/cgi-bin/do_exec_to_buffer.bash")+" "+lrl_web_output);
      if (outlen > 0) {
        size_t ip, ipob, iprev, ipfnb, ipmarkup, istart, iend;
        int ismarkup, islb;
        char outputbuf[outlen+1];
        do_exec_to_buffer(processed_output.c_str(),outputbuf,outlen);
        outputbuf[outlen]=0;
        istart=0;
        std::cout << std::string("<font size=\"-1\">");
        for(iend=0;iend<outlen;iend++){
          if (outputbuf[iend]=='\n'|| outputbuf[iend]=='\r' || outputbuf[iend]==0) {
            outputbuf[iend]=0;
            std::cout << std::string(outputbuf+istart);
            if (iend < outlen) {
              std::cout << std::string("<br />") << std::endl;
            };
            outputbuf[iend]='\n';
            istart=iend+1;
          }
        }
        std::cout << "<br />end</font>" << std::endl;
        if (prevoutbuflen > 0 && prevoutbuf ) {
           free(prevoutbuf);
        }
        prevoutbuf=(char*)malloc(outlen+3);
        ipob=0;
        ismarkup=0;
        iprev=0;
        islb=1;
        for (ip=0; ip<outlen; ip++) {
           if (ismarkup==0 && (outputbuf[ip]=='<' || outputbuf[ip]==';')) {
             ipmarkup = ip+1;
             ismarkup++;
           } else if (ismarkup>0 && (outputbuf[ip]=='>' || outputbuf[ip] == 0 || outputbuf[ip]== '\n' || outputbuf[ip]=='\r')) {
             ismarkup=0;
             if (ip-ipmarkup>2 && outputbuf[ipmarkup]=='b' && outputbuf[ipmarkup+1]=='r') {
               prevoutbuf[ipob++]='\n';
               iprev=ipob;
               islb=0;
             }
           } else if (ismarkup==0) {
             if (outputbuf[ip]!=' '&&islb==1) islb=0; 
             if (islb==0) prevoutbuf[ipob++]=outputbuf[ip];
             if (outputbuf[ip]=='\n' || outputbuf[ip]=='\r' || outputbuf[ip]==0) islb=1;
           }
        }
        prevoutbuf[ipob++]='\n';
        prevoutbuf[ipob++]=0;
        prevoutbuflen = ipob;
      }
      std::cout << "  </div>" << std::endl;
      std::cout << "  </div>" << std::endl;
      std::cout << "  <div id=\"block_"+twodig_array[numop]+"d"+"\" style="+active+">" << std::endl;
      std::cout << "  <br />" << std::endl;
      std::cout << "  <label for=\"lrl_web_help_"+twodig_array[numop]+"\">Tool Help:</label><br />" << std::endl;
      std::cout << "  <div name=\"lrl_web_help_"+twodig_array[numop]+"\" id=\"lrl_web_help_"+twodig_array[numop]+"\"" << std::endl;
      std::cout << "  style=\"overflow-y: auto;text-align: left;height: 108px;width:720px; border-style: solid; border-width: thin; resize: both;\" >" << std::endl;
      std::cout << "<font size=-1><strong>Command: Check Input</strong><br /><em><u>Purpose:</u></em> Verify input lattice types and parameters<br /><em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.<br />Warnings are output for invalid inputs.<br /><em><u>Parameters:</u></em> NA<br /><hr />LRL_Web Data Inputs:  There are 5 types of input lines. Except for 'END', they can be combined in any order.<br /> All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.<br />---  RANDOM: Random (valid) unit cell;<br />---  Crystal lattice input: 'A', 'B', 'C', 'P', 'R', 'F', 'I' followed by three axis lengths and three angles (in degrees);<br />---  semicolon: lines beginning with a semicolon are treated as comments<br />---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.<br />---  END: ends the data input section<br />Examples of unit cell inputs<br />P 10 20 30 90 111 90<br />G 100 400 900 0 -215.02 0<br />S6 0 -107.51 0 7.51 -400 -792.49 <br />; this is a comment<br />end</font>" << std::endl;
      std::cout << "  </div>" << std::endl;
      std::cout << "  </div>" << std::endl;
      std::cout << "" << std::endl;
      std::cout << "  </td>" << std::endl;
      std::cout << "  </tr>" << std::endl;
      //std::cout << "<tr><td colspan=\"3\">"+processed_output+"</td></tr>"<<std::endl;
    }
    if (prevoutbuflen > 0 && prevoutbuf) free(prevoutbuf);
    std::cout << "    </table>" << std::endl;
    std::cout << "</td>" << std::endl;
    std::cout << "<tr>" << std::endl;
    std::cout << "<td>" << std::endl;
    std::cout << "<center>" << std::endl;
    std::cout << "<INPUT type=\"hidden\" NAME=\"Flush\" VALUE=\"DUMMY\">" << std::endl;

    numops_iter = formData.getElement("numops");
    if (numops_iter == formData.getElements().end()) {
      numops2 = 1;
    } else {
      numops2 = (size_t)atol(numops_iter->getValue().c_str());
    }

    std::cout << "<label for=\"numops2\">Number of operation windows: </label>" << std::endl;
    std::cout << "<select name=\"numops2\" id=\"numops2\" onchange=\"changenumops2()\" size=\"1\">" << std::endl;
    if (numops2==1) {
      std::cout << "  <option selected value=\"1\">1</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"1\">1</option>" << std::endl;
    }
    if (numops2==2) {
      std::cout << "  <option selected value=\"2\">2</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"2\">2</option>" << std::endl;
    }
    if (numops2==3) {
      std::cout << "  <option selected value=\"3\">3</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"3\">3</option>" << std::endl;
    }
    if (numops2==4) {
      std::cout << "  <option selected value=\"4\">4</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"4\">4</option>" << std::endl;
    }
    if (numops2==5) {
      std::cout << "  <option selected value=\"5\">5</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"5\">5</option>" << std::endl;
    }
    if (numops2==6) {
      std::cout << "  <option selected value=\"6\">6</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"6\">6</option>" << std::endl;
    }
    if (numops2==7) {
      std::cout << "  <option selected value=\"7\">7</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"7\">7</option>" << std::endl;
    }
    if (numops2==8) {
      std::cout << "  <option selected value=\"8\">8</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"8\">8</option>" << std::endl;
    }
#if NUMOPS_MAX>8
    if (numops2==9) {
      std::cout << "  <option selected value=\"9\">9</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"9\">9</option>" << std::endl;
    }
    if (numops2==10) {
      std::cout << "  <option selected value=\"10\">10</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"10\">10</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>10
    if (numops2==11) {
      std::cout << "  <option selected value=\"11\">11</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"11\">11</option>" << std::endl;
    }
    if (numops2==12) {
      std::cout << "  <option selected value=\"12\">12</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"12\">12</option>" << std::endl;
    }
    if (numops2==13) {
      std::cout << "  <option selected value=\"13\">13</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"13\">13</option>" << std::endl;
    }
    if (numops2==14) {
      std::cout << "  <option selected value=\"14\">14</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"14\">14</option>" << std::endl;
    }
    if (numops2==15) {
      std::cout << "  <option selected value=\"15\">15</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"15\">15</option>" << std::endl;
    }
    if (numops2==16) {
      std::cout << "  <option selected value=\"16\">16</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"16\">16</option>" << std::endl;
    }
    if (numops2==17) {
      std::cout << "  <option selected value=\"17\">17</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"17\">17</option>" << std::endl;
    }
    if (numops2==18) {
      std::cout << "  <option selected value=\"18\">18</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"18\">18</option>" << std::endl;
    }
    if (numops2==19) {
      std::cout << "  <option selected value=\"19\">19</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"19\">19</option>" << std::endl;
    }
    if (numops2==20) {
      std::cout << "  <option selected value=\"20\">10</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"10\">20</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>10
    if (numops2==21) {
      std::cout << "  <option selected value=\"21\">21</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"21\">21</option>" << std::endl;
    }
    if (numops2==22) {
      std::cout << "  <option selected value=\"22\">22</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"22\">22</option>" << std::endl;
    }
    if (numops2==23) {
      std::cout << "  <option selected value=\"23\">23</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"23\">23</option>" << std::endl;
    }
    if (numops2==24) {
      std::cout << "  <option selected value=\"24\">24</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"24\">24</option>" << std::endl;
    }
    if (numops2==25) {
      std::cout << "  <option selected value=\"25\">25</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"25\">25</option>" << std::endl;
    }
    if (numops2==26) {
      std::cout << "  <option selected value=\"26\">26</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"26\">26</option>" << std::endl;
    }
    if (numops2==27) {
      std::cout << "  <option selected value=\"27\">27</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"27\">27</option>" << std::endl;
    }
    if (numops2==28) {
      std::cout << "  <option selected value=\"28\">28</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"28\">28</option>" << std::endl;
    }
    if (numops2==29) {
      std::cout << "  <option selected value=\"29\">29</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"29\">29</option>" << std::endl;
    }
    if (numops2==30) {
      std::cout << "  <option selected value=\"30\">30</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"30\">30</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>30
    if (numops2==31) {
      std::cout << "  <option selected value=\"31\">31</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"31\">31</option>" << std::endl;
    }
    if (numops2==32) {
      std::cout << "  <option selected value=\"32\">32</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"32\">32</option>" << std::endl;
    }
    if (numops2==33) {
      std::cout << "  <option selected value=\"33\">33</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"33\">33</option>" << std::endl;
    }
    if (numops2==34) {
      std::cout << "  <option selected value=\"34\">34</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"34\">34</option>" << std::endl;
    }
    if (numops2==35) {
      std::cout << "  <option selected value=\"35\">35</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"35\">35</option>" << std::endl;
    }
    if (numops2==36) {
      std::cout << "  <option selected value=\"36\">36</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"36\">36</option>" << std::endl;
    }
    if (numops2==37) {
      std::cout << "  <option selected value=\"37\">37</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"37\">37</option>" << std::endl;
    }
    if (numops2==38) {
      std::cout << "  <option selected value=\"38\">38</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"38\">38</option>" << std::endl;
    }
    if (numops2==39) {
      std::cout << "  <option selected value=\"39\">39</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"39\">39</option>" << std::endl;
    }
    if (numops2==40) {
      std::cout << "  <option selected value=\"40\">40</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"40\">40</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>40
    if (numops2==41) {
      std::cout << "  <option selected value=\"41\">41</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"41\">41</option>" << std::endl;
    }
    if (numops2==42) {
      std::cout << "  <option selected value=\"42\">42</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"42\">42</option>" << std::endl;
    }
    if (numops2==43) {
      std::cout << "  <option selected value=\"43\">43</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"43\">43</option>" << std::endl;
    }
    if (numops2==44) {
      std::cout << "  <option selected value=\"44\">44</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"44\">44</option>" << std::endl;
    }
    if (numops2==45) {
      std::cout << "  <option selected value=\"45\">45</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"45\">45</option>" << std::endl;
    }
    if (numops2==46) {
      std::cout << "  <option selected value=\"46\">46</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"46\">46</option>" << std::endl;
    }
    if (numops2==47) {
      std::cout << "  <option selected value=\"47\">47</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"47\">47</option>" << std::endl;
    }
    if (numops2==48) {
      std::cout << "  <option selected value=\"48\">48</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"48\">48</option>" << std::endl;
    }
    if (numops2==49) {
      std::cout << "  <option selected value=\"49\">49</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"49\">49</option>" << std::endl;
    }
    if (numops2==50) {
      std::cout << "  <option selected value=\"50\">50</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"50\">50</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>50
    if (numops2==51) {
      std::cout << "  <option selected value=\"51\">51</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"51\">51</option>" << std::endl;
    }
    if (numops2==52) {
      std::cout << "  <option selected value=\"52\">52</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"52\">52</option>" << std::endl;
    }
    if (numops2==53) {
      std::cout << "  <option selected value=\"53\">53</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"53\">53</option>" << std::endl;
    }
    if (numops2==54) {
      std::cout << "  <option selected value=\"54\">54</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"54\">54</option>" << std::endl;
    }
    if (numops2==55) {
      std::cout << "  <option selected value=\"55\">55</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"55\">55</option>" << std::endl;
    }
    if (numops2==56) {
      std::cout << "  <option selected value=\"56\">56</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"56\">56</option>" << std::endl;
    }
    if (numops2==57) {
      std::cout << "  <option selected value=\"57\">57</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"57\">57</option>" << std::endl;
    }
    if (numops2==58) {
      std::cout << "  <option selected value=\"58\">58</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"58\">58</option>" << std::endl;
    }
    if (numops2==59) {
      std::cout << "  <option selected value=\"59\">59</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"59\">59</option>" << std::endl;
    }
    if (numops2==60) {
      std::cout << "  <option selected value=\"60\">60</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"60\">60</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>60
    if (numops2==61) {
      std::cout << "  <option selected value=\"61\">61</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"61\">61</option>" << std::endl;
    }
    if (numops2==62) {
      std::cout << "  <option selected value=\"62\">62</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"62\">62</option>" << std::endl;
    }
    if (numops2==63) {
      std::cout << "  <option selected value=\"63\">63</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"63\">63</option>" << std::endl;
    }
    if (numops2==64) {
      std::cout << "  <option selected value=\"64\">64</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"64\">64</option>" << std::endl;
    }
    if (numops2==65) {
      std::cout << "  <option selected value=\"65\">65</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"65\">65</option>" << std::endl;
    }
    if (numops2==66) {
      std::cout << "  <option selected value=\"66\">66</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"66\">66</option>" << std::endl;
    }
    if (numops2==67) {
      std::cout << "  <option selected value=\"67\">67</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"67\">67</option>" << std::endl;
    }
    if (numops2==68) {
      std::cout << "  <option selected value=\"68\">68</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"68\">68</option>" << std::endl;
    }
    if (numops2==69) {
      std::cout << "  <option selected value=\"69\">69</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"69\">69</option>" << std::endl;
    }
    if (numops2==70) {
      std::cout << "  <option selected value=\"70\">70</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"70\">70</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>70
    if (numops2==71) {
      std::cout << "  <option selected value=\"71\">71</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"71\">71</option>" << std::endl;
    }
    if (numops2==72) {
      std::cout << "  <option selected value=\"72\">72</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"72\">72</option>" << std::endl;
    }
    if (numops2==73) {
      std::cout << "  <option selected value=\"73\">73</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"73\">73</option>" << std::endl;
    }
    if (numops2==74) {
      std::cout << "  <option selected value=\"74\">74</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"74\">74</option>" << std::endl;
    }
    if (numops2==75) {
      std::cout << "  <option selected value=\"75\">75</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"75\">75</option>" << std::endl;
    }
    if (numops2==76) {
      std::cout << "  <option selected value=\"76\">76</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"76\">76</option>" << std::endl;
    }
    if (numops2==77) {
      std::cout << "  <option selected value=\"77\">77</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"77\">77</option>" << std::endl;
    }
    if (numops2==78) {
      std::cout << "  <option selected value=\"78\">78</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"78\">78</option>" << std::endl;
    }
    if (numops2==79) {
      std::cout << "  <option selected value=\"79\">79</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"79\">79</option>" << std::endl;
    }
    if (numops2==80) {
      std::cout << "  <option selected value=\"80\">80</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"80\">80</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>80
    if (numops2==81) {
      std::cout << "  <option selected value=\"81\">81</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"81\">81</option>" << std::endl;
    }
    if (numops2==82) {
      std::cout << "  <option selected value=\"82\">82</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"82\">82</option>" << std::endl;
    }
    if (numops2==83) {
      std::cout << "  <option selected value=\"83\">83</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"83\">83</option>" << std::endl;
    }
    if (numops2==84) {
      std::cout << "  <option selected value=\"84\">84</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"84\">84</option>" << std::endl;
    }
    if (numops2==85) {
      std::cout << "  <option selected value=\"85\">85</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"85\">85</option>" << std::endl;
    }
    if (numops2==86) {
      std::cout << "  <option selected value=\"86\">86</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"86\">86</option>" << std::endl;
    }
    if (numops2==87) {
      std::cout << "  <option selected value=\"87\">87</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"87\">87</option>" << std::endl;
    }
    if (numops2==88) {
      std::cout << "  <option selected value=\"88\">88</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"88\">88</option>" << std::endl;
    }
    if (numops2==89) {
      std::cout << "  <option selected value=\"89\">89</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"89\">89</option>" << std::endl;
    }
    if (numops2==90) {
      std::cout << "  <option selected value=\"90\">90</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"90\">90</option>" << std::endl;
    }
#endif
#if NUMOPS_MAX>90
    if (numops2==91) {
      std::cout << "  <option selected value=\"91\">91</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"91\">91</option>" << std::endl;
    }
    if (numops2==92) {
      std::cout << "  <option selected value=\"92\">92</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"92\">92</option>" << std::endl;
    }
    if (numops2==93) {
      std::cout << "  <option selected value=\"93\">93</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"93\">93</option>" << std::endl;
    }
    if (numops2==94) {
      std::cout << "  <option selected value=\"94\">94</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"94\"94</option>" << std::endl;
    }
    if (numops2==95) {
      std::cout << "  <option selected value=\"95\">95</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"95\">95</option>" << std::endl;
    }
    if (numops2==96) {
      std::cout << "  <option selected value=\"96\">96</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"96\">96</option>" << std::endl;
    }
    if (numops2==97) {
      std::cout << "  <option selected value=\"97\">97</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"97\">97</option>" << std::endl;
    }
    if (numops2==98) {
      std::cout << "  <option selected value=\"98\">98</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"98\">98</option>" << std::endl;
    }
    if (numops2==99) {
      std::cout << "  <option selected value=\"99\">99</option>" << std::endl;
    } else {
      std::cout << "  <option value=\"99\">99</option>" << std::endl;
    }
#endif
    std::cout << "</select>&nbsp;&nbsp;" << std::endl;
    
    std::cout << "<INPUT type=\"submit\" id=\"submit_000\" onsubmit=\"return running('000')\">" << std::endl;
    std::cout << "<INPUT type=\"reset\">" << std::endl;
    std::cout << "</Form> <hr>" << std::endl;
    std::cout << "</center>" << std::endl;
    std::cout << "</td>" << std::endl;
    std::cout << "</tr>" << std::endl;
    std::cout << "<tr>" << std::endl;
    std::cout << "<td>" << std::endl;
    std::cout << "<a name=\"notice\"><H2 align=\"center\">NOTICE</H2></a>" << std::endl;
    std::cout << "<center>" << std::endl;
    std::cout << "<table border=0>" << std::endl;
    std::cout << "<tr><td width=160>" << std::endl;
    std::cout << "<font size=\"2\">" << std::endl;
    std::cout << "<P>" << std::endl;
    std::cout << "You may redistribute this program under the terms of the <a href=\"gpl.txt\">GPL</a>." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "Alternatively you may redistribute the functions" << std::endl;
    std::cout << "and subroutines of this program as an API under the" << std::endl;
    std::cout << "terms of the <a href=\"lgpl.txt\">LGPL</a>." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "</td>" << std::endl;
    std::cout << "<td>" << std::endl;
    std::cout << "<div style=\"width:720px;height:150px;overflow:scroll;border:2px solid #0000FF;\">" << std::endl;
    std::cout << "<font size=\"2\">" << std::endl;
    std::cout << "<table border=2>" << std::endl;
    std::cout << "<tr>" << std::endl;
    std::cout << "<th align=\"center\">GPL NOTICES</th>" << std::endl;
    std::cout << "<th align=\"center\">LGPL NOTICES</th></tr>" << std::endl;
    std::cout << "<tr><td><font size=\"2\">" << std::endl;
    std::cout << "This program is free software; you can redistribute it and/or" << std::endl;
    std::cout << "modify it under the terms of the GNU General Public License as" << std::endl;
    std::cout << "published by the Free Software Foundation; either version 2 of" << std::endl;
    std::cout << "(the License, or (at your option) any later version." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "This program is distributed in the hope that it will be useful," << std::endl;
    std::cout << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl;
    std::cout << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << std::endl;
    std::cout << "GNU General Public License for more details." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "You should have received a copy of the GNU General Public License" << std::endl;
    std::cout << "along with this program; if not, write to the Free Software" << std::endl;
    std::cout << "Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA" << std::endl;
    std::cout << "02111-1307  USA</font>" << std::endl;
    std::cout << "</td>" << std::endl;
    std::cout << "<td><font size=\"2\">" << std::endl;
    std::cout << "This library is free software; you can redistribute it and/or" << std::endl;
    std::cout << "modify it under the terms of the GNU Lesser General Public" << std::endl;
    std::cout << "License as published by the Free Software Foundation; either" << std::endl;
    std::cout << "version 2.1 of the License, or (at your option) any later version." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "This library is distributed in the hope that it will be useful," << std::endl;
    std::cout << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl;
    std::cout << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU" << std::endl;
    std::cout << "Lesser General Public License for more details." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "You should have received a copy of the GNU Lesser General Public" << std::endl;
    std::cout << "License along with this library; if not, write to the Free" << std::endl;
    std::cout << "Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston," << std::endl;
    std::cout << "MA  02110-1301  USA</font>" << std::endl;
    std::cout << "</td>" << std::endl;
    std::cout << "</tr>" << std::endl;
    std::cout << "</table>" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "<P>" << std::endl;
    std::cout << "<STRONG>" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Some of the software and documents included within this software package are the intellectual property of" << std::endl; 
    std::cout << "various parties, and placement in this package does not in anyway imply that any such rights have in any " << std::endl;
    std::cout << "way been waived or diminished." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "<P>" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "With respect to any software or documents for which a copyright exists, ALL RIGHTS ARE RESERVED TO THE" << std::endl;
    std::cout << "OWNERS OF SUCH COPYRIGHT." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "<P>" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Even though the authors of the various documents and software found here have made a good faith effort to" << std::endl; 
    std::cout << "ensure that the documents are correct and that the software performs according to its documentation, and " << std::endl;
    std::cout << "we would greatly appreciate hearing of any problems you may encounter, the programs and documents any " << std::endl;
    std::cout << "files created by the programs are provided **AS IS** without any warrantee as to correctness, " << std::endl;
    std::cout << "merchantability or fitness for any particular or general use." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "<P>" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "THE RESPONSIBILITY FOR ANY ADVERSE CONSEQUENCES FROM THE USE OF PROGRAMS OR DOCUMENTS OR ANY FILE OR FILES" << std::endl; 
    std::cout << "CREATED BY USE OF THE PROGRAMS OR DOCUMENTS LIES SOLELY WITH THE USERS OF THE PROGRAMS OR DOCUMENTS OR " << std::endl;
    std::cout << "FILE OR FILES AND NOT WITH AUTHORS OF THE PROGRAMS OR DOCUMENTS." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "</STRONG>" << std::endl;
    std::cout << "</font>" << std::endl;
    std::cout << "</div>" << std::endl;
    std::cout << "</td>" << std::endl;
    std::cout << "</tr>" << std::endl;
    std::cout << "</table>" << std::endl;
    std::cout << "</center>" << std::endl;
    std::cout << "<P>" << std::endl;
    std::cout << "<hr>" << std::endl;
    std::cout << "</font>" << std::endl;
    std::cout << "<a name=\"source\"></a>" << std::endl;
    std::cout << "<H2>Access to the source of LRL_WEB</H2>" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "This open source software is maintained on github:" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<center>" << std::endl;
    std::cout << "<a href=\"http://github.com/duck10/LatticeRepLib\">http://github.com/duck10/LatticeRepLib</a>" << std::endl;
    std::cout << "</center>" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "If you are interested in contributing, please make your own fork and writeup an issue when" << std::endl;
    std::cout << "you think you have something to suggest for a pull request." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<h2>History</h2>" << std::endl;
    std::cout << "<p>Human fascination with crystals has a long history. 105,000 years ago, someone had a collection " << std::endl;
    std::cout << "of calcite crystals (Iceland spar) <a href=\"#Wilkins2021\">[Wilkins <i>et al.</i> 2021]</a>. " << std::endl;
    std::cout << "Theophrastus (ca. 372-287 B.C.), a " << std::endl;
    std::cout << "student of Plato and successor to Aristotle, wrote the first known treatise on gems (“On Stones”) " << std::endl;
    std::cout << "<a href=\"#Wikipedia2022\">[Wikipedia contributors 2022]</a>." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Figure 1 notes a few key events in cataloging crystal properties. We start with <a href=\"#Kepler1611\">[Kepler 1611]</a> " << std::endl;
    std::cout << "(translated in <a href=\"#Kepler1966\">[Kepler <i>et al.</i> 1966]) and Steno (see <a href=\"#Authier2013\">[Authier 2013]</a> who conjectured on the structures " << std::endl;
    std::cout << "of crystals. <a name=\"Hauey1800\">[Ha&uuml;y 1800]</a> created the first catalog of minerals." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<center>" << std::endl;
    std::cout << "<a name=\"timeline\"><img src=/~yaya/images/timeline.jpg /></a>" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "Fig. 1. Some key dates in the history of modern crystallography" << std::endl;
    std::cout << "</center>" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "<a name=DeloneChars></a><h2>Table of Delone Characters</h2>" << std::endl;
    std::cout << "<P>" << std::endl;
    std::cout << "<center>" << std::endl;
    std::cout << "<img src=\"http://"+LRL_WEB_HOST+"/~"+LRL_WEB_USER+"/lrl_web/HorizontalDeloneGrid.svg\" alt=\"Table of Delone Characters\" />" << std::endl;
    std::cout << "</center>" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "<a name=NiggliFams></a><h2>Table of Niggli Lattice Families</h2>" << std::endl;
    std::cout << "<P>" << std::endl;
    std::cout << "<center>" << std::endl;
    std::cout << "<img src=\"http://"+LRL_WEB_HOST+"/~"+LRL_WEB_USER+"/lrl_web/NiggliGrid-2.svg\" alt=\"Niggli Lattice Families\" />" << std::endl;
    std::cout << "</center>" << std::endl;
    std::cout << "<h2>References to Sources for Information about the Calculations</h2>" << std::endl;
    std::cout << "<ol>" << std::endl;
    std::cout << "<li>Check Input: simply check the input for errors</li>" << std::endl;
    std::cout << "<li>compute Selling-reduced primitive cells: Delone/Delaunay/Selling reduction <a href=\"#Delaunay1933\">[Delaunay 1933]</a>" << std::endl;
    std::cout << "<a href=\"#Delone1975\">[Delone <i>et al.</i> 1975]</a> <a href=\"#Andrews2019a\">[Andrews <i>et al.</i> 2019a]</a></li>" << std::endl;
    std::cout << "<li>compute NCDist and CS6Dist distances <a href=\"#Andrews2014\">[Andrews and Bernstein 2014]</a>" << std::endl;
    std::cout << "<a href=\"#Andrews2019b\">[Andrews <i>et al.</i> 2019b]</a></li>" << std::endl;
    std::cout << "<li>Generate cells of a particular type or types</li>" << std::endl;
    std::cout << "<li>apply Lattice Matching algorithm to listed cells <a href=\"#Mighell2002\">[Mighell 2002]</a>" << std::endl;
    std::cout << "<a href=\"#Andrews2021\">[Andrews and Bernstein 2021]</a></li>" << std::endl;
    std::cout << "<li>compute Niggli-reduced primitive cells <a href=\"#Niggli1928\">[Niggli 1928]</a> <a href=\"#Roof1967\">[Roof 1967]</a></li>" << std::endl;
    std::cout << "<li>compute path between pairs of cells" << std::endl;
    std::cout << "<a href=\"#Andrews2023a\">[Andrews and Bernstein 2023a]</a></li>" << std::endl;
    std::cout << "<li>compute perturbed versions of input cells <a href=\"#Andrews2022\">[Andrews and Bernstein 2022]</a></li>" << std::endl;
    std::cout << "<li>apply S6 reflections to input cells <a href=\"#Andrews2019b\">[Andrews <i>et al.</i> 2019b]</a></li>" << std::endl;
    std::cout << "<li>apply Sella algorithm <a href=\"#Andrews2023b\">[Andrews <i>et al.</i> 2023b]</a></li>" << std::endl;
    std::cout << "<li>compute Bravais tetrahedron (B4) <a href=\"#Delone1975\">[Delone <i>et al.</i> 1975]</a></li>" << std::endl;
    std::cout << "<li>compute Selling-reduced complex cell presentation (C3) <a href=\"#Andrews2019b\">[Andrews <i>et al.</i> 2019b]</li>" << std::endl;
    std::cout << "<li>compute side-angle cells (a, b, c, &alpha;, &beta;, &gamma;)</li>" << std::endl;
    std::cout << "<li>compute raw Dirichlet cells (DC13)</li>" << std::endl;
    std::cout << "<li>computed sorted Dirichlet cells (DC) <a href=#Andrews2021>[Andrews and Bernstein 2021]</li>" << std::endl;
    std::cout << "<li>compute G6 version of cells (G6) <a href=#Andrews1988>[Andrews and Bernstein 1988]</a></li>" << std::endl;
    std::cout << "<li>compute Selling-reduced cells (S6)</li>" << std::endl;
    std::cout << "<li>compute unsorted Dirichlet cells (dc7unsrt) <a href=\"Bernstein2023\">[Bernstein <i>et al.</i> 2023]</a></li>" << std::endl;
    std::cout << "<li>compute volumes of listed cells</li>" << std::endl;
    std::cout << "</ol>" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"references\"></a>" << std::endl;
    std::cout << "<h2>References</h2>" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<div style=\"margin-bottom:-0.1em;margin-top:-0.1em;\">" << std::endl;
    std::cout << "<a name=\"Andrews1988\">[Andrews and Bernstein 1988]</a> L. C. Andrews and H. J. Bernstein (1988) Lattices and reduced cells as points in " << std::endl;
    std::cout << "6-space and selection of Bravais lattice type by projections. Acta Cryst., A44:6 1009 -- 1018." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Andrews2014\">[Andrews and Bernstein 2014]</a> L. C. Andrews and H. J. Bernstein (2014). " << std::endl;
    std::cout << "The geometry of Niggli reduction: BGAOL–embedding Niggli reduction and analysis of boundaries." << std::endl;
    std::cout << "J. Appl. Cryst., 47(1), 346 -- 359." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Andrews2021\">[Andrews and Bernstein 2021]</a> L. C. Andrews and H. J. Bernstein (2021). " << std::endl;
    std::cout << "DC 7, a very efficient lattice comparison metric. Acta Cryst. A77(a2), C809." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Andrews2022\">[Andrews and Bernstein 2022]</a> L. C. Andrews and H. J. Bernstein (2022)." << std::endl;
    std::cout << "Generating random unit cells.  J. Appl. Cryst. 55(4) 782 -- 786." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Andrews2023a\">[Andrews and Bernstein 2023a]</a> L. C. Andrews and H. J. Bernstein (2023).  " << std::endl;
    std::cout << "The Follower algorithm and a program using it to explore spaces, Cambridge Open Engage preprint" << std::endl;
    std::cout << "<a href=" << std::endl;
    std::cout << "\"https://web.archive.org/web/20230308145808id_/https://www.cambridge.org/engage/api-gateway/coe/assets/orp/resource/item/63f41c579da0bc6b3344e7f3/original/the-follower-algorithm-and-a-program-using-it-to-explore-spaces.pdf\">" << std::endl;
    std::cout << "https://web.archive.org/web/20230308145808id_/https://www.cambridge.org/engage/api-gateway/coe/assets/orp/resource/item/63f41c579da0bc6b3344e7f3/original/the-follower-algorithm-and-a-program-using-it-to-explore-spaces.pdf</a>" << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Andrews2019a\">[Andrews <i>et al.</i> 2019a]</a> L. C. Andrews, H. J. Bernstein, H. J. and N. K. Sauter (2019)." << std::endl;
    std::cout << "Selling reduction versus Niggli reduction for crystallographic lattices. Acta Cryst.  A75, 115 -- 120." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Andrews2019b\">[Andrews <i>et al.</i> 2019b]</a> L. C. Andrews, H. J. Bernstein and N. K.Sauter (2019)." << std::endl;
    std::cout << "A space for lattice representation and clustering. Acta Cryst. A75(3), 593 -- 599. " << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Andrews2023b\">[Andrews <i>et al.</i> 2023b]</a> L. C. Andrews, H. J. Bernstein and N. K. Sauter (2023)." << std::endl;
    std::cout << "SELLA - A Program for Determining Bravais Lattice Types. arXiv preprint" << std::endl;
    std::cout << "<a href=\"https://arxiv.org/pdf/2303.03122.pdf\">" << std::endl;
    std::cout << "https://arxiv.org/pdf/2303.03122.pdf</a>." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Authier2013\">[Authier 2013]</a> A. Authier  (2013). Early days of X-ray crystallography. OUP Oxford." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Bernstein2023\">[Bernstein <i>et al.</i> 2023]</a> H. J. Bernstein, L. C. Andrews and M. Xerri (2023)." << std::endl;
    std::cout << "An invertible seven-dimensional Dirichlet cell characterization of lattices. Acta Cryst. A79(4), 369 -- 380." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Delaunay1933\">[Delaunay 1933]</a> B. N. Delaunay (1933)." << std::endl;
    std::cout << "Neue Darstellung der geometrischen Kristallographie: Erste Abhandlung." << std::endl;
    std::cout << " Z. Krist. 84, 109 -- 149." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Delone1975\">[Delone <i>et al.</i> 1975]</a> B. N. Delone, R. V. Galiulin and M. I. Shtogrin, (1975)." << std::endl;
    std::cout << "On the Bravais types of lattices." << std::endl;
    std::cout << " J. Sov. Math. 4(1), 79 -- 156." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Hauey1800\">[Ha&uuml;y 1800]</a>  R. J. Ha&uuml;y (1800). Addition au m&eacute;moire sur l’arragonite: " << std::endl;
    std::cout << "ins&eacute;r&eacute; dans le tome XI des annnales (p. 241 et suiv.)." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Kepler1611\">[Kepler 1611]</a> Kepler, J. (1611). Strena Seude Niue Sexangula. Godefridum Tampach." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Kepler1966\">[Kepler <i>et al.</i> 1966]</a> J. Kepler,C. G. Hardie, B. J. Mason and L. L. Whyte  (1966). " << std::endl;
    std::cout << "The Six-cornered Snowflake.[Edited and Translated by Colin Hardie. With Essays by L. L. Whyte and B. J. Mason. " << std::endl;
    std::cout << "With Illustrations.] Lat. & Eng. Clarendon Press." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Mighell2002\">[Mighell 2002]</a> A. D. Mighell (2002)." << std::endl;
    std::cout << "Lattice Matching (LM) -- Prevention of Inadvertent Duplicate Publications of Crystal Structures." << std::endl;
    std::cout << "J. Res. Natl. Inst. Stand. Technol. 107(5), 425 – 430." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Niggli1928\">[Niggli 1928]</a> P. Niggli (1928). Krystallographische und Strukturtheoretische Grundbegriffe, " << std::endl;
    std::cout << "Handbuch der Experimentalphysik, Vol. 7, part 1. Akademische Verlagsgesellschaft, Leipzig." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Roof1967\">[Roof 1967]</a> R. B. Roof Jr, (1967). Theoretical Extension of the Reduced-Cell Concept in " << std::endl;
    std::cout << "in Crystallography (No. LA-4038). Los Alamos National Lab.(LANL), Los Alamos, NM (United States). " << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Wikipedia2022\">[Wikipedia contributors 2022]</a> Wikipedia contributors, (2022). Theophrastus — Wikipedia, the " << std::endl;
    std::cout << "free encyclopedia. [Online; accessed 17-October-2022]. https://en.wikipedia.org/w/index.php?title=Theophrastus&oldid=1114534722" << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Wilkins2021\">[Wilkins <i>et al.</i> 2021]</a> J. Wilkins, B. J. Schoville, R. Pickering, L. Gliganic, B. Collins," << std::endl;
    std::cout << "K. S.  Brown, J. von der Meden, W. Khumalo, M. C. Meyer, S. Maape, and A. F. Blackwood (2021). Innovative " << std::endl;
    std::cout << "Homo sapiens behaviours 105,000 years ago in a wetter Kalahari. Nature, 592(7853), 248 -- 252." << std::endl;
    std::cout << "</p><p>" << std::endl;
    std::cout << "<a name=\"Zimmermann1985\">[Zimmermann and Burzlaff 1985]</a> H. Zimmermann and H. Burzlaff. DELOS A computer program for the " << std::endl;
    std::cout << "determination of a unique conventional cell. Zeitschrift f&uuml;r Kristallographie, 170:241 246, 1985." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "</div>" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<hr />" << std::endl;
    std::cout << "Updated 17 April 2024." << std::endl;
    std::cout << "</font>" << std::endl;
 }
 
