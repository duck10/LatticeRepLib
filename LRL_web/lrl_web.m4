changequote(`[[[',`]]]')dnl
define([[[LRLWEBHOST]]],[[[lrl_web_host]]])dnl
define([[[LRLWEBUSER]]],[[[lrl_web_user]]])dnl
define([[[LRLWEBCHECKINPUT]]],[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines Except for “END”, they can be combined in in any order. All these are case-insensitive. If a particular input lattice is invalid, it is rejected with a message.$1---  RANDOM: Random (valid) unit cell;$1---  Crystal lattice input: “A”, “B”, “C”, “P”, “R”, “F”, “I” followed by three axis lengths and three angles (in degrees);$1---  semicolon: lines beginning with a semicolon are treated as comments$1---  Vector Input: g (or v or g6) for G6 vectors; d (or d7) for D7 vectors; s (or s6) for S6, Delone/Selling scalars, C3 for C3 input (without parentheses or commas, “C” would be interpreted as a C-centered unit cell), u for unsorted Dirichlet 7-cells.$1---  END: ends the data input section$1$1Examples of unit cell inputs$1$1P 10 20 30 90 111 90$1G 100 400 900 0 -215.02 0$1S6 0 -107.51 0 7.51 -400 -792.49 ; this is a comment$1end]]]]]])dnl
ifdef([[[lrlwebhost]]],define([[[LRLWEBHOST]]],[[[lrlwebhost]]]))dnl
ifdef([[[lrlwebuser]]],define([[[LRLWEBUSER]]],[[[lrlwebuser]]]))dnl
ifdef([[[cgicpp]]],define([[[prefix]]],[[[std::cout << ]]]))dnl
ifdef([[[cgicpp]]],define([[[epilogue]]],[[[ << std::endl;]]]))dnl
ifdef([[[cgicpp]]],[[[#include <iostream>
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
#define NUMOPS_MAX 10
#define LRL_WEB_HOST std::string("]]]LRLWEBHOST[[[")
#define LRL_WEB_USER std::string("]]]LRLWEBUSER[[[")

using namespace std;
using namespace cgicc;

void  dumpList(const Cgicc& formData); 
void  process(const Cgicc& formData); 

std::string tmp_lrl_web;  //directory for processing
std::string myls;
int main(int argc, 
     char **argv)
{
   char buf[1024];
   std::string xbufstr;
   xbufstr=std::string("/home/")+LRL_WEB_USER+std::string("/public_html/cgi-bin/make_lrl_web_prefix.bash");
   if (do_exec_to_buffer(xbufstr.c_str(),buf,1024)!=0)
       exit(-1);
   tmp_lrl_web=std::string(buf);
   try {
      Cgicc cgi;

      // Send HTTP header
      std::cout << HTTPHTMLHeader() << std::endl;

      // Set up the HTML document" << std::endl;
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
      std::cout << "function twodig(myint) {" << std::endl;
      std::cout << "  if (myint<10) {" << std::endl;
      std::cout << "    return \"0\"+myint;" << std::endl;
      std::cout << "  } else {" << std::endl;
      std::cout << "    return \"\"+myint;" << std::endl;
      std::cout << "  }" << std::endl;
      std::cout << "}" << std::endl;
      std::cout << "" << std::endl;
      std::cout << "function setchaininput(row){" << std::endl;
      std::cout << "    let rownum=parseInt(row);" << std::endl;
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
      std::cout << "    if (document.getElementById(\"chain_\"+twodig(rownum)).value==\"chain_input\") {" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+twodig(priornum)+\"c\").style=\"display:inline\";" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+twodig(rownum)+\"b\").style=\"display:none\";" << std::endl;
      std::cout << "    } else {" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+twodig(rownum)+\"b\").style=\"display:inline\";" << std::endl;
      std::cout << "    }" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+twodig(rownum)).style=\"display:inline\";" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+twodig(rownum)+\"a\").style=\"display:inline\";" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+twodig(rownum)+\"c\").style=\"display:inline\";" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+twodig(rownum)+\"d\").style=\"display:inline\";" << std::endl;
      std::cout << "    return true;" << std::endl;
      std::cout << "}" << std::endl;
      std::cout << "" << std::endl;
      std::cout << "function changenumops(){" << std::endl;
      std::cout << "  var ii;" << std::endl;
      std::cout << "  let mynumops=parseInt(document.getElementById(\"numops\").value);" << std::endl;
      std::cout << "  if (mynumops < 1) mynumops=1;" << std::endl;
      std::cout << "  if (mynumops > "<<NUMOPS_MAX<<") mynumops="<<NUMOPS_MAX<<";" << std::endl;
      std::cout << "  document.getElementById(\"numops\").value=mynumops.toString();" << std::endl;
      std::cout << "  for (ii=1; ii<mynumops+1;ii++) {" << std::endl;
      std::cout << "    // alert(\"enable block_\"+twodig(ii));" << std::endl;
      std::cout << "    let mychain=document.getElementById(\"chain_\"+twodig(ii)).value;" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+twodig(ii)).style=\"display:inline\";" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+twodig(ii)+\"a\").style=\"display:inline\";" << std::endl;
      std::cout << "    if (mychain!=\"chain_input\") {" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+twodig(ii)+\"b\").style=\"display:inline\";" << std::endl;
      std::cout << "    } else {" << std::endl;
      std::cout << "      document.getElementById(\"block_\"+twodig(ii)+\"b\").style=\"display:none\";" << std::endl;
      std::cout << "    }" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+twodig(ii)+\"c\").style=\"display:inline\";" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+twodig(ii)+\"d\").style=\"display:inline\";" << std::endl;
      std::cout << "  }" << std::endl;
      std::cout << "  if (mynumops < "<<NUMOPS_MAX<<") {" << std::endl;
      std::cout << "  for (ii=mynumops+1; ii<"<<NUMOPS_MAX+1<<";ii++) {" << std::endl;
      std::cout << "    // alert(\"disable block_\"+twodig(ii));" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+twodig(ii)).style=\"display:none\";" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+twodig(ii)+\"a\").style=\"display:none\";" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+twodig(ii)+\"b\").style=\"display:none\";" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+twodig(ii)+\"c\").style=\"display:none\";" << std::endl;
      std::cout << "    document.getElementById(\"block_\"+twodig(ii)+\"d\").style=\"display:none\";" << std::endl;
      std::cout << "  }" << std::endl;
      std::cout << "  }" << std::endl;
      std::cout << "  return true;" << std::endl;
      std::cout << "" << std::endl;
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
      std::cout << "<BODY>" << std::endl;
      std::cout << "<font face=\"Arial,Helvetica,Times\" size=\"3\">" << std::endl;
      std::cout << "<hr />" << std::endl;
      std::cout << "<center>" << std::endl;
      std::cout << "Buffer: " << "'"+tmp_lrl_web+"'" <<std::endl;

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
 std::string plaintext2html(std::string & dst, std::string src)
{
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
          dst.append("<br/>");
          break;
        case('\r'):
          dst.append("<br/>");
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
    ofstream lrl_web_data_file;
    std::string lrl_web_data_data;
    std::string chain;
    std::string xactstr;
    size_t numops;
    size_t numop;
    std::string twodig_array[NUMOPS_MAX+1]={"00","01","02","03","04","05","06","07","08","09","10"
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
    std::cout << "<a href=\"#celticknot\">Celtic knots</a> by Susan Jones Davis, Blue Seahorse Studio, <br />" << std::endl;
    std::cout << "Sleeping Dragon line art image by Gordon Dylan Johnson, " << std::endl;
    std::cout << "<a href=\"https://openclipart.org/detail/226147/sleeping-dragon-line-art\">https://openclipart.org/detail/226147/sleeping-dragon-line-art</a></font></td>" << std::endl;
    std::cout << "</tr>" << std::endl;
    xactstr=std::string("<FORM method=POST ACTION=\"http://"+LRL_WEB_HOST+"/~");
    xactstr+=LRL_WEB_USER;
    xactstr+=std::string("/cgi-bin/lrl_web.cgi\">");
    std::cout << xactstr  << std::endl;
    std::cout << "<br />" << std::endl;
    std::cout << "Assorted tools to do various calculations for crystallographic lattices." << std::endl;
    std::cout << "<br />" << std::endl;
    std::cout << "<STRONG>" << std::endl;
    std::cout << "Please read the <a href=\"#notice\">NOTICE</a> below before use of this web page" << std::endl;
    std::cout << "</STRONG>" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"search\"></a>" << std::endl;
    std::cout << "<INPUT type=\"submit\">" << std::endl;
    std::cout << "<INPUT type=\"reset\">" << std::endl;
    std::cout << "<br />" << std::endl;
    std::cout << "<input type=hidden name=\"OutputStyle\" value=\"TEXT\" />" << std::endl;
    std::cout << "<table border=2>" << std::endl;
    std::cout << "<tr><td valign=top>" << std::endl;
    std::cout << "  <table>" << std::endl;
    std::cout << "  <tr>" << std::endl;
    std::cout << "  <td colspan=3 align=\"center\">" << std::endl;
    std::cout << "  <label for=\"numops\">Number of operation windows: </label>" << std::endl;
    std::cout << "  <select name=\"numops\" id=\"numops\" onchange=\"changenumops()\" size=\"1\">" << std::endl;
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
      std::cout << "  <option value=\"10\">20</option>" << std::endl;
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
    std::cout << "  </select>" << std::endl;
    std::cout << "  </td>" << std::endl;
    std::cout << "  </tr>" << std::endl;
    std::string currentoutput=string("");
    char * prevoutbuf = (char *)malloc(1);
    size_t prevoutbuflen = 1;
    prevoutbuf[0] = 0;
    for (numop=1; numop <=NUMOPS_MAX; numop++) {
      std::string chain;
      std::string operation;
      std::string selected;
      std::string active=std::string("\"display:inline\"");
      std::string lrl_web_data=string(tmp_lrl_web+"/lrl_web_data_"+twodig_array[numop]);
      std::string lrl_web_output=string(tmp_lrl_web+"/lrl_web_output_"+twodig_array[numop]);
      std::string lwd=string(tmp_lrl_web+"lwd_"+twodig_array[numop]);
      std::string prioroutput=currentoutput;
      std::string currentoutput=lrl_web_output;
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
      std::string at=std::string("");
      std::string path=std::string(tmp_lrl_web+"/lrl_web_data_"+twodig_array[numop]);
      if(string_to_file(at.c_str(), path.c_str(), lrl_web_data_data.c_str())) exit(-1);
      lrl_web_output_iter = formData.getElement("lrl_web_output"+twodig_array[numop]);
      operation_iter = formData.getElement("operation_"+twodig_array[numop]);
      if (operation_iter == formData.getElements().end()) {
        operation = "NoOp";
      } else {
        operation = operation_iter->getValue();
      }
      std::string oppath=std::string(tmp_lrl_web+"/operation_"+twodig_array[numop]);
      if(string_to_file(at.c_str(), oppath.c_str(), operation.c_str())) {
         std::cout << "<tr><td colspan=\"3\">string_to_file of"+tmp_lrl_web+"/operation_"+twodig_array[numop]+" failed</td></tr>"<<std::endl;
      }
      std::string xprocess_next_output=
        std::string("/home/")+LRL_WEB_USER+std::string("/public_html/cgi-bin/process_next_output.bash")+
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
      std::cout << "  <tr>" << std::endl;
      std::cout << "  <td>" << std::endl;
      std::cout << "  <div id=\"block_"+twodig_array[numop]+"\" style="+active+">" << std::endl; 
      std::cout << "  <label for=\"chain_"+twodig_array[numop]+"\">Source of data:</label><br/>" << std::endl;
      std::cout << "  <select name=\"chain_"+twodig_array[numop]+"\" id=\"chain_"+twodig_array[numop]+
          "\" size=\"1\" onchange=\"setchaininput('" << numop << "')\">" << std::endl;
      selected=chain.compare("new_input")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"new_input\">use new input</option>" << std::endl;
      selected=chain.compare("chain_input")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"chain_input\">use prior output</option>" << std::endl;
      std::cout << "  </select>" << std::endl;
      std::cout << "  <br />" << std::endl;
      std::cout << "  <br />" << std::endl;
      std::cout << "  <label for=\"submit_"+twodig_array[numop]+"\">Submit all data:</label><br/>" << std::endl;
      std::cout << "  <INPUT type=\"submit\">" << std::endl;
      std::cout << "  </div>" << std::endl;
      std::cout << "  </td>" << std::endl;
      std::cout << "  <td align=left>" << std::endl;
      std::cout << "  <div id=\"block_"+twodig_array[numop]+"a\" style="+active+">" << std::endl; 
      std::cout << "  <label for=\"operation_"+twodig_array[numop]+"\">Select an operation:</label><br/>" << std::endl;
      std::cout << "  <select name=\"operation_"+twodig_array[numop]+"\" id=\"operation_"+twodig_array[numop]+"\" size=\"19\">" << std::endl;
      selected=operation.compare("NoOp")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"NoOp\">Check Input</option>" << std::endl;
      selected=operation.compare("CmdDelone")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdDelone\">compute Selling-reduced primitive cells</option>" << std::endl;
      selected=operation.compare("CmdDists")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdDists\">compute NCDist and CS6Dist distances</option>" << std::endl;
      selected=operation.compare("CmdGen")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdGen\">Generate cells of a particular type or types</option>" << std::endl;
      selected=operation.compare("CmdLM")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdLM\">apply Lattice Matching algorithm to listed cells</option>" << std::endl;
      selected=operation.compare("CmdNiggli")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdNiggli\">compute Niggli-reduced primitive cells</option>" << std::endl;
      selected=operation.compare("CmdPath")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdPath\">compute path between pairs of cells</option>" << std::endl;
      selected=operation.compare("CmdPerturb")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdPerturb\">compute perturbed versions of input cells</option>" << std::endl;
      selected=operation.compare("CmdS6Refl")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdS6Refl\">apply S6 reflections to input cells</option>" << std::endl;
      selected=operation.compare("CmdSella")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdSella\"> apply Sella algorithm</option>" << std::endl;
      selected=operation.compare("CmdToB4")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToB4\"> compute Bravais tetrahedron (B4)</option>" << std::endl;
      selected=operation.compare("CmdToC3")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToC3\"> compute Selling-reduced complex cell presentation (C3)</option>" << std::endl;
      selected=operation.compare("CmdToCell")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToCell\"> compute side-angle cells (a, b, c, &alpha;, &beta;, &gamma;)</option>" << std::endl;
      selected=operation.compare("CmdToD13")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToD13\"> compute raw Dirichlet cells (DC13)</option>" << std::endl;
      selected=operation.compare("CmdToDC")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToDC\"> computed sorted Dirichlet cells (DC)</option>" << std::endl;
      selected=operation.compare("CmdToG6")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToG6\"> compute G6 version of cells</option>" << std::endl;
      selected=operation.compare("CmdToS6")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToS6\"> compute S6 version of cells</option>" << std::endl;
      selected=operation.compare("CmdToU")==0?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdToU\"> compute unsorted Dirichlet cells (dc7unsrt)</option>" << std::endl;
      selected=(operation.compare("CmdVolume")==0)?"selected ":"";
      std::cout << "  <option "+selected+"value=\"CmdVolume\"> compute volumes of listed cells</option>" << std::endl;
      std::cout << "  </select>" << std::endl;
      std::cout << "  </div>" << std::endl;
      std::cout << "  </td>" << std::endl;
      std::cout << "  <td align=left>" << std::endl;
      if (chain.compare("new_input")==0 || numop < 2) {
        std::cout << "  <div id=\"block_"+twodig_array[numop]+"b"+"\" style="+active+"> " << std::endl;
      } else {
        std::cout << "  <div id=\"block_"+twodig_array[numop]+"b"+"\" style=\"display:none\" >" <<std::endl;
      }
      std::cout << "  <label for=\"lrl_web_data_"+twodig_array[numop]+"\">Input data:</label><br />" << std::endl;
      std::cout << "  <textarea name=\"lrl_web_data_"+twodig_array[numop]+"\" id=\"lrl_web_data_"
          +twodig_array[numop]+"\" rows=\"9\" cols=\"100\">";
      if (lrl_web_data_iter==formData.getElements().end()) {
          lrl_web_data_data=std::string("end");
      } else { lrl_web_data_data=lrl_web_data_iter->getValue();
      }
      std::cout << lrl_web_data_data << std::endl;
      std::cout << "  </textarea>" << std::endl;
      std::cout << "  </div>" << std::endl;
      std::cout << "  <div id=\"block_"+twodig_array[numop]+"c"+"\" style="+active+">" << std::endl;
      std::cout << "  <br />" << std::endl;
      std::cout << "  <label for=\"lrl_web_output_"+twodig_array[numop]+"\">Tool Output:</label><br />" << std::endl;
      std::cout << "  <textarea name=\"lrl_web_output_"+twodig_array[numop]+"\" id=\"lrl_web_output_"+twodig_array[numop]+"\" rows=\"9\" cols=\"100\">" 
        << std::endl;
      string processed_output=string(std::string("/home/")+LRL_WEB_USER+std::string("/public_html/cgi-bin/do_exec_to_buffer.bash")+" "+lrl_web_output);
      if (outlen > 0) {
        size_t ip;
        char outputbuf[outlen+1];
        do_exec_to_buffer(processed_output.c_str(),outputbuf,outlen);
        outputbuf[outlen]=0;
        std::cout << std::string(outputbuf) << std::endl;
        std::cout << "end" << std::endl;
        if (prevoutbuflen > 0 && prevoutbuf ) {
           free(prevoutbuf);
        }
        prevoutbuf=(char*)malloc(outlen+1);
        for (ip=0; ip<outlen+1; ip++) prevoutbuf[ip]=outputbuf[ip];
        prevoutbuflen = outlen+1;
      }
      std::cout << "  </textarea>" << std::endl;
      std::cout << "  </div>" << std::endl;
      std::cout << "  <div id=\"block_"+twodig_array[numop]+"d"+"\" style="+active+">" << std::endl;
      std::cout << "  <br />" << std::endl;
      std::cout << "  <label for=\"lrl_web_help_"+twodig_array[numop]+"\">Tool Help:</label><br />" << std::endl;
      std::cout << "  <textarea name=\"lrl_web_help_"+twodig_array[numop]+"\" id=\"lrl_web_help_"+twodig_array[numop]+"\" readonly rows=\"9\" cols=\"100\" readonly`>" << std::endl;
      std::cout << "]]]LRLWEBCHECKINPUT([[[\n]]])[[[" << std::endl;
      std::cout << "" << std::endl;
      std::cout << "  </textarea>" << std::endl;
      std::cout << "  </div>" << std::endl;
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
    std::cout << "<INPUT type=\"submit\">" << std::endl;
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
    std::cout << "<div style=\"width:800px;height:150px;overflow:scroll;border:2px solid #0000FF;\">" << std::endl;
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
    std::cout << "This open sorce software is maintained on github:" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<center>" << std::endl;
    std::cout << "<a href=\"http://github.com/duck10/LatticeRepLib\">http://github.com/duck10/LatticeRepLib</a>" << std::endl;
    std::cout << "</center>" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "If you are interested in contributing, please make your own fork and writeup an issue when" << std::endl;
    std::cout << "you think you have something to suggest for a pull request." << std::endl;
    std::cout << "" << std::endl;
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
    std::cout << "<a name=\"timeline\"><img src=/~]]]LRLWEBUSER[[[/images/timeline.jpg /></a>" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "Fig. 1. Some key dates in the history of modern crystallography" << std::endl;
    std::cout << "</center>" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "<h2>References to Sources for Information about the Calculations</h2>" << std::endl;
    std::cout << "<ol>" << std::endl;
    std::cout << "<li>Check Input: simply check the input for errors</li>" << std::endl;
    std::cout << "<li>compute Selling-reduced primitive cells: Delone/Delaunay/Selling reduction <a href=\"#Delaunay1933\">[Delaunay 1933]</a>" << std::endl;
    std::cout << "<a href=\"#Delone1975\">[Delone <i>et al.</i> 1975]</a> <a href=\"#Andrews2019a\">[Andrews <i>et al.</i> 2019a]</a></li>" << std::endl;
    std::cout << "<li>compute NCDist and CS6Dist distances <a href=\"#Andrews2014\">[Andrews and Bernstein 2014]</a>" << std::endl;
    std::cout << "<a href=\"#Andrews2019b\">[Andrews <i>et al.</i> 2019b]</a></li>" << std::endl;
    std::cout << "<li>apply Lattice Matching algorithm to listed cells <a href=\"#Mighell2002\">[Mighell 2002]</a>" << std::endl;
    std::cout << "<a href=\"#Andrews2021\">[Andrews and Bernstein 2021]</a></li>" << std::endl;
    std::cout << "<li>compute Niggli-reduced primitive cells <a href=\"#Niggli1928\">[Niggli 1928]</a></li>" << std::endl;
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
    std::cout << "<a name=\"Andrews1988\">[Andrews and Bernstein 1988]</a> L. C. Andrews and H. J. Bernstein (1988) Lattices and reduced cells as points in " << std::endl;
    std::cout << "6-space and selection of Bravais lattice type by projections. Acta Cryst., A44:6 1009 -- 1018.<br />" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Andrews2014\">[Andrews and Bernstein 2014]</a> L. C. Andrews and H. J. Bernstein (2014). " << std::endl;
    std::cout << "The geometry of Niggli reduction: BGAOL–embedding Niggli reduction and analysis of boundaries." << std::endl;
    std::cout << "J. Appl. Cryst., 47(1), 346 -- 359." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Andrews2021\">[Andrews and Bernstein 2021]</a> L. C. Andrews and H. J. Bernstein (2021). " << std::endl;
    std::cout << "DC 7, a very efficient lattice comparison metric. Acta Cryst. A77(a2), C809." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Andrews2022\">[Andrews and Bernstein 2022]</a> L. C. Andrews and H. J. Bernstein (2022)." << std::endl;
    std::cout << "Generating random unit cells.  J. Appl. Cryst. 55(4) 782 -- 786." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Andrews2023a\">[Andrews and Bernstein 2023a]</a> L. C. Andrews and H. J. Bernstein (2023).  " << std::endl;
    std::cout << "The Follower algorithm and a program using it to explore spaces, Cambridge Open Engage preprint" << std::endl;
    std::cout << "<a href=" << std::endl;
    std::cout << "\"https://web.archive.org/web/20230308145808id_/https://www.cambridge.org/engage/api-gateway/coe/assets/orp/resource/item/63f41c579da0bc6b3344e7f3/original/the-follower-algorithm-and-a-program-using-it-to-explore-spaces.pdf\">" << std::endl;
    std::cout << "https://web.archive.org/web/20230308145808id_/https://www.cambridge.org/engage/api-gateway/coe/assets/orp/resource/item/63f41c579da0bc6b3344e7f3/original/the-follower-algorithm-and-a-program-using-it-to-explore-spaces.pdf</a>" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Andrews2019a\">[Andrews <i>et al.</i> 2019a]</a> L. C. Andrews, H. J. Bernstein, H. J. and N. K. Sauter (2019)." << std::endl;
    std::cout << "Selling reduction versus Niggli reduction for crystallographic lattices. Acta Cryst.  A75, 115 -- 120." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Andrews2019b\">[Andrews <i>et al.</i> 2019b]</a> L. C. Andrews, H. J. Bernstein and N. K.Sauter (2019)." << std::endl;
    std::cout << "A space for lattice representation and clustering. Acta Cryst. A75(3), 593 -- 599. " << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Andrews2023b\">[Andrews <i>et al.</i> 2023b]</a> L. C. Andrews, H. J. Bernstein and N. K. Sauter (2023)." << std::endl;
    std::cout << "SELLA - A Program for Determining Bravais Lattice Types. arXiv preprint" << std::endl;
    std::cout << "<a href=\"https://arxiv.org/pdf/2303.03122.pdf\">" << std::endl;
    std::cout << "https://arxiv.org/pdf/2303.03122.pdf</a>." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Authier2013\">[Authier 2013]</a> A. Authier  (2013). Early days of X-ray crystallography. OUP Oxford." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Bernstein2023\">[Bernstein <i>et al.</i> 2023]</a> H. J. Bernstein, L. C. Andrews and M. Xerri (2023)." << std::endl;
    std::cout << "An invertible seven-dimensional Dirichlet cell characterization of lattices. Acta Cryst. A79(4), 369 -- 380." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Delaunay1933\">[Delaunay 1933]</a> B. N. Delaunay (1933)." << std::endl;
    std::cout << "Neue Darstellung der geometrischen Kristallographie: Erste Abhandlung." << std::endl;
    std::cout << " Z. Krist. 84, 109 -- 149." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Delone1975\">[Delone <i>et al.</i> 1975]</a> B. N. Delone, R. V. Galiulin and M. I. Shtogrin, (1975)." << std::endl;
    std::cout << "On the Bravais types of lattices." << std::endl;
    std::cout << " J. Sov. Math. 4(1), 79 -- 156." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Hauey1800\">[Ha&uuml;y 1800]</a>  R. J. Ha&uuml;y (1800). Addition au m&eacute;moire sur l’arragonite: " << std::endl;
    std::cout << "ins&eacute;r&eacute; dans le tome XI des annnales (p. 241 et suiv.)." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Kepler1611\">[Kepler 1611]</a> Kepler, J. (1611). Strena Seude Niue Sexangula. Godefridum Tampach." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Kepler1966\">[Kepler <i>et al.</i> 1966]</a> J. Kepler,C. G. Hardie, B. J. Mason and L. L. Whyte  (1966). " << std::endl;
    std::cout << "The Six-cornered Snowflake.[Edited and Translated by Colin Hardie. With Essays by L. L. Whyte and B. J. Mason. " << std::endl;
    std::cout << "With Illustrations.] Lat. & Eng. Clarendon Press." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Mighell2002\">[Mighell 2002]</a> A. D. Mighell (2002)." << std::endl;
    std::cout << "Lattice Matching (LM) -- Prevention of Inadvertent Duplicate Publications of Crystal Structures." << std::endl;
    std::cout << "J. Res. Natl. Inst. Stand. Technol. 107(5), 425 – 430." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Niggli1928\">[Niggli 1928]</a> P. Niggli (1928). Krystallographische und Strukturtheoretische Grundbegriffe, " << std::endl;
    std::cout << "Handbuch der Experimentalphysik, Vol. 7, part 1. Akademische Verlagsgesellschaft, Leipzig." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Wikipedia2022\">[Wikipedia contributors 2022]</a> Wikipedia contributors, (2022). Theophrastus — Wikipedia, the " << std::endl;
    std::cout << "free encyclopedia. [Online; accessed 17-October-2022]. https://en.wikipedia.org/w/index.php?title=Theophrastus&oldid=1114534722" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Wilkins2021\">[Wilkins <i>et al.</i> 2021]</a> J. Wilkins, B. J. Schoville, R. Pickering, L. Gliganic, B. Collins," << std::endl;
    std::cout << "K. S.  Brown, J. von der Meden, W. Khumalo, M. C. Meyer, S. Maape, and A. F. Blackwood,  2021. Innovative " << std::endl;
    std::cout << "Homo sapiens behaviours 105,000 years ago in a wetter Kalahari. Nature, 592(7853), 248 -- 252." << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<a name=\"Zimmermann1985\">[Zimmermann and Burzlaff 1985]</a> H. Zimmermann and H. Burzlaff. DELOS A computer program for the " << std::endl;
    std::cout << "determination of a unique conventional cell. Zeitschrift f&uuml;r Kristallographie, 170:241 246, 1985." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "<p>" << std::endl;
    std::cout << "<hr />" << std::endl;
    std::cout << "Updated 31 October 2023." << std::endl;
    std::cout << "</font>" << std::endl;
 }
]]],
[[[<!DOCTYPE HTML>
<html>
<HEAD>
	<meta charset="utf-8">

 <script  type="text/javascript">
 const EventListenerMode = {capture: true};

function preventGlobalMouseEvents () {
  document.body.style['pointer-events'] = 'none';
  return true;
}

function restoreGlobalMouseEvents () {
  document.body.style['pointer-events'] = 'auto';
  return;
}

function mousemoveListener (e) {
  e.stopPropagation ();
  // do whatever is needed while the user is moving the cursor around
  return true;
}

function mouseupListener (e) {
  restoreGlobalMouseEvents ();
  document.removeEventListener ('mouseup',   mouseupListener,   EventListenerMode);
  document.removeEventListener ('mousemove', mousemoveListener, EventListenerMode);
  e.stopPropagation ();
  return true;
}

function captureMouseEvents (e) {
  preventGlobalMouseEvents ();
  document.addEventListener ('mouseup',   mouseupListener,   EventListenerMode);
  document.addEventListener ('mousemove', mousemoveListener, EventListenerMode);
  e.preventDefault ();
  e.stopPropagation ();
  return true;
}

function twodig(myint) {
  if (myint<10) {
    return "0"+myint;
  } else {
    return ""+myint;
  }
}

function setchaininput(row){
    let rownum=parseInt(row);
    if (rownum < 2) {
      let firstrow=document.getElementById("chain_01");
      if (!(firstrow.value==="new_input")){
        alert("Request for data prior to row 1 ignored");
      }
      firstrow.value="new_input";
      document.getElementById("block_01").style="display:inline";
      document.getElementById("block_01a").style="display:inline";
      document.getElementById("block_01b").style="display:inline";
      document.getElementById("block_01c").style="display:inline";
      document.getElementById("block_01d").style="display:inline";
      return true;
    } else if (rownum > 10) {
      alert("Request for data after row 10 ignored");
      return false;
    }
    let priornum=rownum-1;
    if (document.getElementById("chain_"+twodig(rownum)).value=="chain_input") {
      document.getElementById("block_"+twodig(priornum)+"c").style="display:inline";
      document.getElementById("block_"+twodig(priornum)+"d").style="display:inline";
      document.getElementById("block_"+twodig(rownum)+"b").style="display:none";
    } else {
      document.getElementById("block_"+twodig(rownum)+"b").style="display:inline"; 
    }
    document.getElementById("block_"+twodig(rownum)).style="display:inline";
    document.getElementById("block_"+twodig(rownum)+"a").style="display:inline";
    document.getElementById("block_"+twodig(rownum)+"c").style="display:inline";
    document.getElementById("block_"+twodig(rownum)+"d").style="display:inline";
    return true;
}

function changenumops(){
  var ii;
  let mynumops=parseInt(document.getElementById("numops").value);
  if (mynumops < 1) mynumops=1;
  if (mynumops > 10) mynumops=10;
  document.getElementById("numops").value=mynumops.toString();
  for (ii=1; ii<mynumops+1;ii++) {
    // alert("enable block_"+twodig(ii));
    let mychain=document.getElementById("chain_"+twodig(ii)).value;
    document.getElementById("block_"+twodig(ii)).style="display:inline";
    document.getElementById("block_"+twodig(ii)+"a").style="display:inline";
    if (mychain!="chain_input") {
      document.getElementById("block_"+twodig(ii)+"b").style="display:inline";
    } else {
      document.getElementById("block_"+twodig(ii)+"b").style="display:none";
    }
    document.getElementById("block_"+twodig(ii)+"c").style="display:inline";
    document.getElementById("block_"+twodig(ii)+"d").style="display:inline";
  }
  if (mynumops < 10) {
  for (ii=mynumops+1; ii<11;ii++) {
    // alert("disable block_"+twodig(ii));
    document.getElementById("block_"+twodig(ii)).style="display:none";
    document.getElementById("block_"+twodig(ii)+"a").style="display:none";
    document.getElementById("block_"+twodig(ii)+"b").style="display:none";
    document.getElementById("block_"+twodig(ii)+"c").style="display:none";
    document.getElementById("block_"+twodig(ii)+"d").style="display:none";
  }
  }
  return true;

}

function pfloat(pfield){
    // validate for non-negative float
    var charsAllowed="0123456789.+";
    var allowed;
    var plusfound;
    var dotfound;
    plusfound = 0;
    dotfound = 0;
    for(var i=0;i<pfield.value.length;i++){
        allowed=false;
        for(var j=0;j<charsAllowed.length;j++){
            if( pfield.value.charAt(i)==charsAllowed.charAt(j) ){ 
               allowed=true;
               if (j == 11) {
                 plusfound++;
                 allowed=false;
                 plusfound--;
                 break;
               } else if (j == 10) {
                 dotfound++;
                 if (dotfound > 1) {
                   allowed=false;
                   dotfound--;
                   break;
                 }
               } 
               break;
            }
        }
        if(allowed==false){ pfield.value = pfield.value.replace(pfield.value.charAt(i),""); i--; }
    }
    return true;
}

function gfloat(field){
    // validate for float   
    var charsAllowed="0123456789.+-";
    var allowed;
    var plusminusfound;
    var dotfound;
    plusminusfound = 0;
    dotfound = 0;
    otherfound = 0;
    for(var i=0;i<field.value.length;i++){       
        allowed=false;
        for(var j=0;j<charsAllowed.length;j++){
            if( field.value.charAt(i)==charsAllowed.charAt(j) ){ 
               allowed=true;
               if (j == 11 || j == 12) {
                 if (otherfound>0 || dotfound>0 || plusminusfound>0) {
                   allowed=false;
                 } else {
                 plusminusfound++;
                 }
               } else if (j == 10) {
                 if (dotfound>0) {
                   allowed=false;
                 } else {
                 dotfound++;
                 } 
               } else {
                 otherfound++;
               } 
            }
        }
        if(allowed==false){ field.value = field.value.replace(field.value.charAt(i),""); i--; }
    }
    return true;
}

</script>

<TITLE>
LRL_WEB Lattice Representation Library Tool Web Page
</TITLE> 
</HEAD> 


<BODY onload="changenumops()">
<font face="Arial,Helvetica,Times" size="3">
<hr />
<center>
<table border=0>
<tr>
<td align=left valign=top><a name="celticknots"><img src=http://]]]LRLWEBHOST[[[/~]]]LRLWEBUSER[[[/images/leftcorner.gif /></a></td>
<td width=150 rowspan="2">&nbsp;</td>
<td align=center rowspan="2"><img src="http://]]]LRLWEBHOST[[[/~]]]LRLWEBUSER[[[/images/dragon1a.gif" /></td>
<td width=150 rowspan="2">&nbsp;</td>
<td align=right valign=top><img src=http://]]]LRLWEBHOST[[[/~]]]LRLWEBUSER[[[/images/rightcorner.gif /></td>
</tr>
<tr> <td align=left valign=bottom><img src=http://]]]LRLWEBHOST[[[/~]]]LRLWEBUSER[[[/images/leftbottomcorner.gif /></td>
<td align=right valign=bottom><img src=http://]]]LRLWEBHOST[[[/~]]]LRLWEBUSER[[[/images/rightbottomcorner.gif /></td></tr>
</table>
<H2> LRL_WEB, Lattice Representation Library Tool Web Page</H2>
<br /> by
<br /> Lawrence C. Andrews, Herbert J. Bernstein, Ronin Institute for Independent Scholarship,
<br /><A HREF=mailto:lawrence.andrews@ronininstitute.org>lawrence.andrews@ronininstitute.org</A>
<a href=mailto:yayahjb@gmail.com>yayahjb@gmail.com</a><br />
<P>
<tr>
<td colspan=3 align=center><font size="-1">
<a href="#timeline">Timeline image</a> by Elizabeth Kincaid, Elizabeth Kincaid Watercolors, <br />
<a href="#celticknot">Celtic knots</a> by Susan Jones Davis, Blue Seahorse Studio, <br />
Sleeping Dragon line art image by Gordon Dylan Johnson, 
<a href="https://openclipart.org/detail/226147/sleeping-dragon-line-art">https://openclipart.org/detail/226147/sleeping-dragon-line-art</a></font></td>
</tr>
<FORM method=POST ACTION="http://]]]LRLWEBHOST[[[/~]]]LRLWEBUSER[[[/cgi-bin/lrl_web.cgi">
<br />
Assorted tools to do various calculations for crystallographic lattices.
<br />
<STRONG>
Please read the <a href="#notice">NOTICE</a> below before use of this web page
</STRONG>
<p>
<a name="search"></a>
<INPUT type="submit">
<INPUT type="reset">
<br />
<input type=hidden name="OutputStyle" value="TEXT" />
<table border=2>
<tr><td valign=top>
  <table>
  <tr>
  <td colspan=3 align="center">
  <label for="numops">Number of operation windows: </label>
  <select name="numops" id="numops" onchange="changenumops()" size="1">
  <option selected value="1">1</option>
  <option value="2">2</option>
  <option value="3">3</option>
  <option value="4">4</option>
  <option value="5">5</option>
  <option value="6">6</option>
  <option value="7">7</option>
  <option value="8">8</option>
  <option value="9">9</option>
  <option value="10">10</option>
  </select>
  </td>
  </tr>
  <tr>
  <td>
  <div id="block_01" style="display:inline"> 
  <label for="chain_01">Source of data:</label><br/>
  <select name="chain_01" id="chain_01" size="1" onchange="setchaininput('1')">
  <option selected value="new_input">use new input</option>
  <option value="chain_input">use prior output</option>
  </select>
  <br />
  <br />
  <label for=submit_01>Submit all data:</label>
  <INPUT type="submit">
  </div>
  </td>
  <td align=left>
  <div id="block_01a" style="display:inline"> 
  <label for="operation_01">Select an operation:</label><br/>
  <select name="operation_01" id="operation_01" size="19">
  <option value="NoOp">Check Input</option>
  <option value="CmdDelone">compute Selling-reduced primitive cells</option>
  <option value="CmdDists">compute NCDist and CS6Dist distances</option>
  <option value="CmdGen">Generate cells of a particular type or types</option>
  <option value="CmdLM">apply Lattice Matching algorithm to listed cells</option>
  <option selected value="CmdNiggli">compute Niggli-reduced primitive cells</option>
  <option value="CmdPath">compute path between pairs of cells</option>
  <option value="CmdPerturb">compute perturbed versions of input cells</option>
  <option value="CmdS6Refl">apply S6 reflections to input cells</option>
  <option value="CmdSella"> apply Sella algorithm</option>
  <option value="CmdToB4"> compute Bravais tetrahedron (B4)</option>
  <option value="CmdToC3"> compute Selling-reduced complex cell presentation (C3)</option>
  <option value="CmdToCell"> compute side-angle cells (a, b, c, &alpha;, &beta;, &gamma;)</option>
  <option value="CmdToD13"> compute raw Dirichlet cells (DC13)</option>
  <option value="CmdToDC"> computed sorted Dirichlet cells (DC)</option>
  <option value="CmdToG6"> compute G6 version of cells</option>
  <option value="CmdToS6"> compute S6 version of cells</option>
  <option value="CmdToU"> compute unsorted Dirichlet cells (dc7unsrt)</option>
  <option value="CmdVolume"> compute volumes of listed cells</option>
  </select>
  </div>
  </td>
  <td align=left>
  <div id="block_01b" style="display:inline"> 
  <label for="lrl_web_data_01">Input data:</label><br />
  <textarea name="lrl_web_data_01" id="lrl_web_data_01"  rows="9" cols="100">p 10 10 10 90 90 90
  end
  </textarea>
  </div>
  <div id="block_01c" style="display:inline">
  <br />
  <label for="lrl_web_output_01">Tool Output:</label><br />
  <textarea name="lrl_web_output_01" id="lrl_web_output_01" rows="9" cols="100">
  Press submit to process data
  </textarea>
  </div>
  <div id="block_01d" style="display:inline">
  <br />
  <label for="lrl_web_help_01">Tool Help:</label><br />
  <textarea name="lrl_web_help_01" id="lrl_web_help_01" rows="9" cols="100" readonly>
  ]]]LRLWEBCHECKINPUT([[[
]]])[[[
  </textarea>
  </div>
  </td>
  </tr>
  <tr>
  <td>
  <div id="block_02" style="display:none"> 
  <label for="chain_02">Source of data:</label><br/>
  <select name="chain_02" id="chain_02" size="1" onchange="setchaininput('2')">
  <option value="new_input">use new input</option>
  <option selected value="chain_input">use prior output</option>
  </select>
  <br />
  <br />
  <label for=submit_02>Submit all data:</label>
  <INPUT type="submit">
  </div>
  </td>
  <td align=left>
  <div id="block_02a" style="display:none"> 
  <label for="operation_02">Select an operation:</label><br/>
  <select name="operation_02" id="operation_02" size="19">
  <option selected value="NoOp">Check Input</option>
  <option value="CmdDelone">compute Selling-reduced primitive cells</option>
  <option value="CmdDists">compute NCDist and CS6Dist distances</option>
  <option value="CmdGen">Generate cells of a particular type or types</option>
  <option value="CmdLM">apply Lattice Matching algorithm to listed cells</option>
  <option value="CmdNiggli">compute Niggli-reduced primitive cells</option>
  <option value="CmdPath">compute path between pairs of cells</option>
  <option value="CmdPerturb">compute perturbed versions of input cells</option>
  <option value="CmdS6Refl">apply S6 reflections to input cells</option>
  <option value="CmdSella"> apply Sella algorithm</option>
  <option value="CmdToB4"> compute Bravais tetrahedron (B4)</option>
  <option value="CmdToC3"> compute Selling-reduced complex cell presentation (C3)</option>
  <option value="CmdToCell"> compute side-angle cells (a, b, c, &alpha;, &beta;, &gamma;)</option>
  <option value="CmdToD13"> compute raw Dirichlet cells (DC13)</option>
  <option value="CmdToDC"> computed sorted Dirichlet cells (DC)</option>
  <option value="CmdToG6"> compute G6 version of cells</option>
  <option value="CmdToS6"> compute S6 version of cells</option>
  <option value="CmdToU"> compute unsorted Dirichlet cells (dc7unsrt)</option>
  <option value="CmdVolume"> compute volumes of listed cells</option>
  </select>
  </div>
  </td>
  <td align=left>
  <div id="block_02b" style="display:none"> 
  <label for="lrl_web_data_02">Input data:</label><br />
  <textarea name="lrl_web_data_02" id="lrl_web_data_02" rows="9" cols="100">p 10 10 10 90 90 90
  end
  </textarea>
  </div>
  <div id="block_02c" style="display:none">
  <br />
  <label for="lrl_web_output_02">Tool Output:</label><br />
  <textarea name="lrl_web_output_02" id="lrl_web_output_02" rows="9" cols="100">
  Press submit to process data
  </textarea>
  </div>
  <div id="block_02d" style="display:inline">
  <br />
  <label for="lrl_web_help_02">Tool Help:</label><br />
  <textarea name="lrl_web_help_02" id="lrl_web_help_02" rows="9" cols="100" readonly>
    ]]]LRLWEBCHECKINPUT([[[
]]])[[[
  </textarea>
  </div>

  </td>
  </tr>
  <tr>
  <td>
  <div id="block_03" style="display:none"> 
  <label for="chain_03">Source of data:</label><br/>
  <select name="chain_03" id="chain_03" size="1" onchange="setchaininput('3')">
  <option value="new_input">use new input</option>
  <option selected value="chain_input">use prior output</option>
  </select>
  <br />
  <br />
  <label for=submit_03>Submit all data:</label>
  <INPUT type="submit">
  </div>
  </td>
  <td align=left>
  <div id="block_03a" style="display:none"> 
  <label for="operation_03">Select an operation:</label><br/>
  <select name="operation_03" id="operation_03" size="19">
  <option selected value="NoOp">Check Input</option>
  <option value="CmdDelone">compute Selling-reduced primitive cells</option>
  <option value="CmdDists">compute NCDist and CS6Dist distances</option>
  <option value="CmdGen">Generate cells of a particular type or types</option>
  <option value="CmdLM">apply Lattice Matching algorithm to listed cells</option>
  <option value="CmdNiggli">compute Niggli-reduced primitive cells</option>
  <option value="CmdPath">compute path between pairs of cells</option>
  <option value="CmdPerturb">compute perturbed versions of input cells</option>
  <option value="CmdS6Refl">apply S6 reflections to input cells</option>
  <option value="CmdSella"> apply Sella algorithm</option>
  <option value="CmdToB4"> compute Bravais tetrahedron (B4)</option>
  <option value="CmdToC3"> compute Selling-reduced complex cell presentation (C3)</option>
  <option value="CmdToCell"> compute side-angle cells (a, b, c, &alpha;, &beta;, &gamma;)</option>
  <option value="CmdToD13"> compute raw Dirichlet cells (DC13)</option>
  <option value="CmdToDC"> computed sorted Dirichlet cells (DC)</option>
  <option value="CmdToG6"> compute G6 version of cells</option>
  <option value="CmdToS6"> compute S6 version of cells</option>
  <option value="CmdToU"> compute unsorted Dirichlet cells (dc7unsrt)</option>
  <option value="CmdVolume"> compute volumes of listed cells</option>
  </select>
  </div>
  </td>
  <td align=left>
  <div id="block_03b" style="display:none"> 
  <label for="lrl_web_data_03">Input data:</label><br />
  <textarea name="lrl_web_data_03" id="lrl_web_data_03" rows="9" cols="100">p 10 10 10 90 90 90
  end
  </textarea>
  </div>
  <div id="block_03c" style="display:none">
  <br />
  <label for="lrl_web_output_03">Tool Output:</label><br />
  <textarea name="lrl_web_output_03" id="lrl_web_output_03" rows="9" cols="100">
  Press submit to process data
  </textarea>
  </div>
  <div id="block_03d" style="display:inline">
  <br />
  <label for="lrl_web_help_03">Tool Help:</label><br />
  <textarea name="lrl_web_help_03" id="lrl_web_help_03" rows="9" cols="100" readonly>
    ]]]LRLWEBCHECKINPUT([[[
]]])[[[
  </textarea>
  </div>

  </td>
  </tr>
  <tr>
  <td>
  <div id="block_04" style="display:none"> 
  <label for="chain_04">Source of data:</label><br/>
  <select name="chain_04" id="chain_04" size="1" onchange="setchaininput('4')">
  <option value="new_input">use new input</option>
  <option selected value="chain_input">use prior output</option>
  </select>
  <br />
  <br />
  <label for=submit_04>Submit all data:</label>
  <INPUT type="submit">
  </div>
  </td>
  <td align=left>
  <div id="block_04a" style="display:none"> 
  <label for="operation_04">Select an operation:</label><br/>
  <select name="operation_04" id="operation_04" size="19">
  <option selected value="NoOp">Check Input</option>
  <option value="CmdDelone">compute Selling-reduced primitive cells</option>
  <option value="CmdDists">compute NCDist and CS6Dist distances</option>
  <option value="CmdGen">Generate cells of a particular type or types</option>
  <option value="CmdLM">apply Lattice Matching algorithm to listed cells</option>
  <option value="CmdNiggli">compute Niggli-reduced primitive cells</option>
  <option value="CmdPath">compute path between pairs of cells</option>
  <option value="CmdPerturb">compute perturbed versions of input cells</option>
  <option value="CmdS6Refl">apply S6 reflections to input cells</option>
  <option value="CmdSella"> apply Sella algorithm</option>
  <option value="CmdToB4"> compute Bravais tetrahedron (B4)</option>
  <option value="CmdToC3"> compute Selling-reduced complex cell presentation (C3)</option>
  <option value="CmdToCell"> compute side-angle cells (a, b, c, &alpha;, &beta;, &gamma;)</option>
  <option value="CmdToD13"> compute raw Dirichlet cells (DC13)</option>
  <option value="CmdToDC"> computed sorted Dirichlet cells (DC)</option>
  <option value="CmdToG6"> compute G6 version of cells</option>
  <option value="CmdToS6"> compute S6 version of cells</option>
  <option value="CmdToU"> compute unsorted Dirichlet cells (dc7unsrt)</option>
  <option value="CmdVolume"> compute volumes of listed cells</option>
  </select>
  </div>
  </td>
  <td align=left>
  <div id="block_04b" style="display:none"> 
  <label for="lrl_web_data_04">Input data:</label><br />
  <textarea name="lrl_web_data_04" id="lrl_web_data_04" rows="9" cols="100">p 10 10 10 90 90 90
  end
  </textarea>
  </div>
  <div id="block_04c" style="display:none">
  <br />
  <label for="lrl_web_output_04">Tool Output:</label><br />
  <textarea name="lrl_web_output_04" id="lrl_web_output_04" rows="9" cols="100">
  Press submit to process data
  </textarea>
  </div>
  <div id="block_04d" style="display:inline">
  <br />
  <label for="lrl_web_help_04">Tool Help:</label><br />
  <textarea name="lrl_web_help_04" id="lrl_web_help_04" rows="9" cols="100" readonly>
   ]]]LRLWEBCHECKINPUT([[[
]]])[[[
  </textarea>
  </div>

  </td>
  </tr>
  <tr>
  <td>
  <div id="block_05" style="display:none"> 
  <label for="chain_05">Source of data:</label><br/>
  <select name="chain_05" id="chain_05" size="1" onchange="setchaininput('5')">
  <option value="new_input">use new input</option>
  <option selected value="chain_input">use prior output</option>
  </select>
  <br />
  <br />
  <label for=submit_05>Submit all data:</label>
  <INPUT type="submit">
  </div>
  </td>
  <td align=left>
  <div id="block_05a" style="display:none"> 
  <label for="operation_05">Select an operation:</label><br/>
  <select name="operation_05" id="operation_05" size="19">
  <option selected value="NoOp">Check Input</option>
  <option value="CmdDelone">compute Selling-reduced primitive cells</option>
  <option value="CmdDists">compute NCDist and CS6Dist distances</option>
  <option value="CmdGen">Generate cells of a particular type or types</option>
  <option value="CmdLM">apply Lattice Matching algorithm to listed cells</option>
  <option value="CmdNiggli">compute Niggli-reduced primitive cells</option>
  <option value="CmdPath">compute path between pairs of cells</option>
  <option value="CmdPerturb">compute perturbed versions of input cells</option>
  <option value="CmdS6Refl">apply S6 reflections to input cells</option>
  <option value="CmdSella"> apply Sella algorithm</option>
  <option value="CmdToB4"> compute Bravais tetrahedron (B4)</option>
  <option value="CmdToC3"> compute Selling-reduced complex cell presentation (C3)</option>
  <option value="CmdToCell"> compute side-angle cells (a, b, c, &alpha;, &beta;, &gamma;)</option>
  <option value="CmdToD13"> compute raw Dirichlet cells (DC13)</option>
  <option value="CmdToDC"> computed sorted Dirichlet cells (DC)</option>
  <option value="CmdToG6"> compute G6 version of cells</option>
  <option value="CmdToS6"> compute S6 version of cells</option>
  <option value="CmdToU"> compute unsorted Dirichlet cells (dc7unsrt)</option>
  <option value="CmdVolume"> compute volumes of listed cells</option>
  </select>
  </div>
  </td>
  <td align=left>
  <div id="block_05b" style="display:none"> 
  <label for="lrl_web_data_05">Input data:</label><br />
  <textarea name="lrl_web_data_05" id="lrl_web_data_05" rows="9" cols="100">p 10 10 10 90 90 90
  end
  </textarea>
  </div>
  <div id="block_05c" style="display:none">
  <br />
  <label for="lrl_web_output_05">Tool Output:</label><br />
  <textarea name="lrl_web_output_05" id="lrl_web_output_05" rows="9" cols="100">
  Press submit to process data
  </textarea>
  </div>
  <div id="block_05d" style="display:inline">
  <br />
  <label for="lrl_web_help_05">Tool Help:</label><br />
  <textarea name="lrl_web_help_05" id="lrl_web_help_05" rows="9" cols="100" readonly>
   ]]]LRLWEBCHECKINPUT([[[
]]])[[[
  </textarea>
  </div>

  </td>
  </tr>
  <tr>
  <td>
  <div id="block_06" style="display:none"> 
  <label for="chain_06">Source of data:</label><br/>
  <select name="chain_06" id="chain_06" size="1" onchange="setchaininput('6')">
  <option value="new_input">use new input</option>
  <option selected value="chain_input">use prior output</option>
  </select>
  <br />
  <br />
  <label for=submit_06>Submit all data:</label>
  <INPUT type="submit">
  </div>
  </td>
  <td align=left>
  <div id="block_06a" style="display:none"> 
  <label for="operation_06">Select an operation:</label><br/>
  <select name="operation_06" id="operation_06" size="19">
  <option selected value="NoOp">Check Input</option>
  <option value="CmdDelone">compute Selling-reduced primitive cells</option>
  <option value="CmdDists">compute NCDist and CS6Dist distances</option>
  <option value="CmdGen">Generate cells of a particular type or types</option>
  <option value="CmdLM">apply Lattice Matching algorithm to listed cells</option>
  <option value="CmdNiggli">compute Niggli-reduced primitive cells</option>
  <option value="CmdPath">compute path between pairs of cells</option>
  <option value="CmdPerturb">compute perturbed versions of input cells</option>
  <option value="CmdS6Refl">apply S6 reflections to input cells</option>
  <option value="CmdSella"> apply Sella algorithm</option>
  <option value="CmdToB4"> compute Bravais tetrahedron (B4)</option>
  <option value="CmdToC3"> compute Selling-reduced complex cell presentation (C3)</option>
  <option value="CmdToCell"> compute side-angle cells (a, b, c, &alpha;, &beta;, &gamma;)</option>
  <option value="CmdToD13"> compute raw Dirichlet cells (DC13)</option>
  <option value="CmdToDC"> computed sorted Dirichlet cells (DC)</option>
  <option value="CmdToG6"> compute G6 version of cells</option>
  <option value="CmdToS6"> compute S6 version of cells</option>
  <option value="CmdToU"> compute unsorted Dirichlet cells (dc7unsrt)</option>
  <option value="CmdVolume"> compute volumes of listed cells</option>
  </select>
  </div>
  </td>
  <td align=left>
  <div id="block_06b" style="display:none"> 
  <label for="lrl_web_data_06">Input data:</label><br />
  <textarea name="lrl_web_data_06" id="lrl_web_data_06" rows="9" cols="100">p 10 10 10 90 90 90
  end
  </textarea>
  </div>
  <div id="block_06c" style="display:none">
  <br />
  <label for="lrl_web_output_06">Tool Output:</label><br />
  <textarea name="lrl_web_output_06" id="lrl_web_output_06" rows="9" cols="100">
  Press submit to process data
  </textarea>
  </div>
  <div id="block_06d" style="display:inline">
  <br />
  <label for="lrl_web_help_06">Tool Help:</label><br />
  <textarea name="lrl_web_help_06" id="lrl_web_help_06" rows="9" cols="100" readonly>
   ]]]LRLWEBCHECKINPUT([[[
]]])[[[
  </textarea>
  </div>

  </td>
  </tr>
  <tr>
  <td>
  <div id="block_07" style="display:none"> 
  <label for="chain_07">Source of data:</label><br/>
  <select name="chain_07" id="chain_07" size="1" onchange="setchaininput('7')">
  <option value="new_input">use new input</option>
  <option selected value="chain_input">use prior output</option>
  </select>
  <br />
  <br />
  <label for=submit_07>Submit all data:</label>
  <INPUT type="submit">
  </div>
  </td>
  <td align=left>
  <div id="block_07a" style="display:none"> 
  <label for="operation_07">Select an operation:</label><br/>
  <select name="operation_07" id="operation_07" size="19">
  <option selected value="NoOp">Check Input</option>
  <option value="CmdDelone">compute Selling-reduced primitive cells</option>
  <option value="CmdDists">compute NCDist and CS6Dist distances</option>
  <option value="CmdGen">Generate cells of a particular type or types</option>
  <option value="CmdLM">apply Lattice Matching algorithm to listed cells</option>
  <option value="CmdNiggli">compute Niggli-reduced primitive cells</option>
  <option value="CmdPath">compute path between pairs of cells</option>
  <option value="CmdPerturb">compute perturbed versions of input cells</option>
  <option value="CmdS6Refl">apply S6 reflections to input cells</option>
  <option value="CmdSella"> apply Sella algorithm</option>
  <option value="CmdToB4"> compute Bravais tetrahedron (B4)</option>
  <option value="CmdToC3"> compute Selling-reduced complex cell presentation (C3)</option>
  <option value="CmdToCell"> compute side-angle cells (a, b, c, &alpha;, &beta;, &gamma;)</option>
  <option value="CmdToD13"> compute raw Dirichlet cells (DC13)</option>
  <option value="CmdToDC"> computed sorted Dirichlet cells (DC)</option>
  <option value="CmdToG6"> compute G6 version of cells</option>
  <option value="CmdToS6"> compute S6 version of cells</option>
  <option value="CmdToU"> compute unsorted Dirichlet cells (dc7unsrt)</option>
  <option value="CmdVolume"> compute volumes of listed cells</option>
  </select>
  </div>
  </td>
  <td align=left>
  <div id="block_07b" style="display:none"> 
  <label for="lrl_web_data_07">Input data:</label><br />
  <textarea name="lrl_web_data_07" id="lrl_web_data_07" rows="9" cols="100">p 10 10 10 90 90 90
  end
  </textarea>
  </div>
  <div id="block_07c" style="display:none">
  <br />
  <label for="lrl_web_output_07">Tool Output:</label><br />
  <textarea name="lrl_web_output_07" id="lrl_web_output_07" rows="9" cols="100">
  Press submit to process data
  </textarea>
  </div>
  <div id="block_07d" style="display:inline">
  <br />
  <label for="lrl_web_help_07">Tool Help:</label><br />
  <textarea name="lrl_web_help_07" id="lrl_web_help_07" rows="9" cols="100" readonly>
   ]]]LRLWEBCHECKINPUT([[[
]]])[[[
  </textarea>
  </div>

  </td>
  </tr>
  <tr>
  <td>
  <div id="block_08" style="display:none"> 
  <label for="chain_08">Source of data:</label><br/>
  <select name="chain_08" id="chain_08" size="1" onchange="setchaininput('8')">
  <option value="new_input">use new input</option>
  <option selected value="chain_input">use prior output</option>
  </select>
  <br />
  <br />
  <label for=submit_08>Submit all data:</label>
  <INPUT type="submit">
  </div>
  </td>
  <td align=left>
  <div id="block_08a" style="display:none"> 
  <label for="operation_08">Select an operation:</label><br/>
  <select name="operation_08" id="operation_08" size="19">
  <option selected value="NoOp">Check Input</option>
  <option value="CmdDelone">compute Selling-reduced primitive cells</option>
  <option value="CmdDists">compute NCDist and CS6Dist distances</option>
  <option value="CmdGen">Generate cells of a particular type or types</option>
  <option value="CmdLM">apply Lattice Matching algorithm to listed cells</option>
  <option value="CmdNiggli">compute Niggli-reduced primitive cells</option>
  <option value="CmdPath">compute path between pairs of cells</option>
  <option value="CmdPerturb">compute perturbed versions of input cells</option>
  <option value="CmdS6Refl">apply S6 reflections to input cells</option>
  <option value="CmdSella"> apply Sella algorithm</option>
  <option value="CmdToB4"> compute Bravais tetrahedron (B4)</option>
  <option value="CmdToC3"> compute Selling-reduced complex cell presentation (C3)</option>
  <option value="CmdToCell"> compute side-angle cells (a, b, c, &alpha;, &beta;, &gamma;)</option>
  <option value="CmdToD13"> compute raw Dirichlet cells (DC13)</option>
  <option value="CmdToDC"> computed sorted Dirichlet cells (DC)</option>
  <option value="CmdToG6"> compute G6 version of cells</option>
  <option value="CmdToS6"> compute S6 version of cells</option>
  <option value="CmdToU"> compute unsorted Dirichlet cells (dc7unsrt)</option>
  <option value="CmdVolume"> compute volumes of listed cells</option>
  </select>
  </div>
  </td>
  <td align=left>
  <div id="block_08b" style="display:none"> 
  <label for="lrl_web_data_08">Input data:</label><br />
  <textarea name="lrl_web_data_08" id="lrl_web_data_08" rows="9" cols="100">p 10 10 10 90 90 90
  end
  </textarea>
  </div>
  <div id="block_08c" style="display:none">
  <br />
  <label for="lrl_web_output_08">Tool Output:</label><br />
  <textarea name="lrl_web_output_08" id="lrl_web_output_08" rows="9" cols="100">
  Press submit to process data
  </textarea>
  </div>
  <div id="block_08d" style="display:inline">
  <br />
  <label for="lrl_web_help_08">Tool Help:</label><br />
  <textarea name="lrl_web_help_08" id="lrl_web_help_08" rows="9" cols="100" readonly>
    ]]]LRLWEBCHECKINPUT([[[
]]])[[[
  </textarea>
  </div>

  </td>
  </tr>
  <tr>
  <td>
  <div id="block_09" style="display:none"> 
  <label for="chain_09">Source of data:</label><br/>
  <select name="chain_09" id="chain_09" size="1" onchange="setchaininput('9')">
  <option value="new_input">use new input</option>
  <option selected value="chain_input">use prior output</option>
  </select>
  <br />
  <br />
  <label for=submit_09>Submit all data:</label>
  <INPUT type="submit">
  </div>
  </td>
  <td align=left>
  <div id="block_09a" style="display:none"> 
  <label for="operation_09">Select an operation:</label><br/>
  <select name="operation_09" id="operation_09" size="19">
  <option selected value="NoOp">Check Input</option>
  <option value="CmdDelone">compute Selling-reduced primitive cells</option>
  <option value="CmdDists">compute NCDist and CS6Dist distances</option>
  <option value="CmdGen">Generate cells of a particular type or types</option>
  <option value="CmdLM">apply Lattice Matching algorithm to listed cells</option>
  <option value="CmdNiggli">compute Niggli-reduced primitive cells</option>
  <option value="CmdPath">compute path between pairs of cells</option>
  <option value="CmdPerturb">compute perturbed versions of input cells</option>
  <option value="CmdS6Refl">apply S6 reflections to input cells</option>
  <option value="CmdSella"> apply Sella algorithm</option>
  <option value="CmdToB4"> compute Bravais tetrahedron (B4)</option>
  <option value="CmdToC3"> compute Selling-reduced complex cell presentation (C3)</option>
  <option value="CmdToCell"> compute side-angle cells (a, b, c, &alpha;, &beta;, &gamma;)</option>
  <option value="CmdToD13"> compute raw Dirichlet cells (DC13)</option>
  <option value="CmdToDC"> computed sorted Dirichlet cells (DC)</option>
  <option value="CmdToG6"> compute G6 version of cells</option>
  <option value="CmdToS6"> compute S6 version of cells</option>
  <option value="CmdToU"> compute unsorted Dirichlet cells (dc7unsrt)</option>
  <option value="CmdVolume"> compute volumes of listed cells</option>
  </select>
  </div>
  </td>
  <td align=left>
  <div id="block_09b" style="display:none"> 
  <label for="lrl_web_data_09">Input data:</label><br />
  <textarea name="lrl_web_data_09" id="lrl_web_data_09" rows="9" cols="100">p 10 10 10 90 90 90
  end
  </textarea>
  </div>
  <div id="block_09c" style="display:none">
  <br />
  <label for="lrl_web_output_09">Tool Output:</label><br />
  <textarea name="lrl_web_output_09" id="lrl_web_output_09" rows="9" cols="100">
  Press submit to process data
  </textarea>
  </div>
  <div id="block_09d" style="display:inline">
  <br />
  <label for="lrl_web_help_09">Tool Help:</label><br />
  <textarea name="lrl_web_help_09" id="lrl_web_help_09" rows="9" cols="100" readonly>
   ]]]LRLWEBCHECKINPUT([[[
]]])[[[
  </textarea>
  </div>

  </td> 
  </tr>
  <tr>
  <td>
  <div id="block_10" style="display:none"> 
  <label for="chain_10">Source of data:</label><br/>
  <select name="chain_10" id="chain_10" size="1" onchange="setchaininput('10')">
  <option value="new_input">use new input</option>
  <option selected value="chain_input">use prior output</option>
  </select>
  <br />
  <br />
  <label for=submit_10>Submit all data:</label>
  <INPUT type="submit">
  </div>
  </td>
  <td  align=left>
  <div id="block_10a" style="display:none"> 
  <label for="operation_10">Select an operation:</label><br/>
  <select name="operation_10" id="operation_10" size="19">
  <option selected value="NoOp">Check Input</option>
  <option value="CmdDelone">compute Selling-reduced primitive cells</option>
  <option value="CmdDists">compute NCDist and CS6Dist distances</option>
  <option value="CmdGen">Generate cells of a particular type or types</option>
  <option value="CmdLM">apply Lattice Matching algorithm to listed cells</option>
  <option value="CmdNiggli">compute Niggli-reduced primitive cells</option>
  <option value="CmdPath">compute path between pairs of cells</option>
  <option value="CmdPerturb">compute perturbed versions of input cells</option>
  <option value="CmdS6Refl">apply S6 reflections to input cells</option>
  <option value="CmdSella"> apply Sella algorithm</option>
  <option value="CmdToB4"> compute Bravais tetrahedron (B4)</option>
  <option value="CmdToC3"> compute Selling-reduced complex cell presentation (C3)</option>
  <option value="CmdToCell"> compute side-angle cells (a, b, c, &alpha;, &beta;, &gamma;)</option>
  <option value="CmdToD13"> compute raw Dirichlet cells (DC13)</option>
  <option value="CmdToDC"> computed sorted Dirichlet cells (DC)</option>
  <option value="CmdToG6"> compute G6 version of cells</option>
  <option value="CmdToS6"> compute S6 version of cells</option>
  <option value="CmdToU"> compute unsorted Dirichlet cells (dc7unsrt)</option>
  <option value="CmdVolume"> compute volumes of listed cells</option>
  </select>
  </div>
  </td>
  <td align=left>
  <div id="block_10b" style="display:none"> 
  <label for="lrl_web_data_10">Input data:</label><br />
  <textarea name="lrl_web_data_10" id="lrl_web_data_10" rows="9" cols="100">p 10 10 10 90 90 90
  end
  </textarea>
  </div>
  <div id="block_10c" style="display:none">
  <br />
  <label for="lrl_web_output_10">Tool Output:</label><br />
  <textarea name="lrl_web_output_10" id="lrl_web_output_10" rows="9" cols="100">
  Press submit to process data
  </textarea>
  </div>
  <div id="block_10d" style="display:inline">
  <br />
  <label for="lrl_web_help_10">Tool Help:</label><br />
  <textarea name="lrl_web_help_10" id="lrl_web_help_10" rows="9" cols="100" readonly>
   ]]]LRLWEBCHECKINPUT([[[
]]])[[[
  </textarea>
  </div>

  </td>
  </tr>
  </table>
</td>
<tr>
<td>
<center>
<INPUT type="hidden" NAME="Flush" VALUE="DUMMY">
<INPUT type="submit">
<INPUT type="reset">
</Form> <hr>
</center>
</td>
</tr>
<tr>
<td>
<a name="notice"><H2 align="center">NOTICE</H2></a>
<center>
<table border=0>
<tr><td width=160>
<font size="2">
<P>
You may redistribute this program under the terms of the <a href="gpl.txt">GPL</a>.
<p>
Alternatively you may redistribute the functions
and subroutines of this program as an API under the
terms of the <a href="lgpl.txt">LGPL</a>.
<p>
</td>
<td>
<div style="width:800px;height:150px;overflow:scroll;border:2px solid #0000FF;">
<font size="2">
<table border=2>
<tr>
<th align="center">GPL NOTICES</th>
<th align="center">LGPL NOTICES</th></tr>
<tr><td><font size="2">
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
(the License, or (at your option) any later version.
<p>
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
<p>
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307  USA</font>
</td>
<td><font size="2">
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
<p>
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.
<p>
You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free
Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
MA  02110-1301  USA</font>
</td>
</tr>
</table>


<P>
<STRONG>

Some of the software and documents included within this software package are the intellectual property of 
various parties, and placement in this package does not in anyway imply that any such rights have in any 
way been waived or diminished.

<P>

With respect to any software or documents for which a copyright exists, ALL RIGHTS ARE RESERVED TO THE 
OWNERS OF SUCH COPYRIGHT.

<P>

Even though the authors of the various documents and software found here have made a good faith effort to 
ensure that the documents are correct and that the software performs according to its documentation, and 
we would greatly appreciate hearing of any problems you may encounter, the programs and documents any 
files created by the programs are provided **AS IS** without any warrantee as to correctness, 
merchantability or fitness for any particular or general use.

<P>

THE RESPONSIBILITY FOR ANY ADVERSE CONSEQUENCES FROM THE USE OF PROGRAMS OR DOCUMENTS OR ANY FILE OR FILES 
CREATED BY USE OF THE PROGRAMS OR DOCUMENTS LIES SOLELY WITH THE USERS OF THE PROGRAMS OR DOCUMENTS OR 
FILE OR FILES AND NOT WITH AUTHORS OF THE PROGRAMS OR DOCUMENTS.

</STRONG>
</font>
</div>
</td>
</tr>
</table>
</center>
<P>
<hr>
</font>
<a name="source"></a>
<H2>Access to the source of LRL_WEB</H2>

This open sorce software is maintained on github:
<p>
<center>
<a href="http://github.com/duck10/LatticeRepLib">http://github.com/duck10/LatticeRepLib</a>
</center>
<p>
If you are interested in contributing, please make your own fork and writeup an issue when
you think you have something to suggest for a pull request.
<p>
<h2>History</h2>
<p>Human fascination with crystals has a long history. 105,000 years ago, someone had a collection 
of calcite crystals (Iceland spar) <a href="#Wilkins2021">[Wilkins <i>et al.</i> 2021]</a>. 
Theophrastus (ca. 372-287 B.C.), a 
student of Plato and successor to Aristotle, wrote the first known treatise on gems (“On Stones”) 
<a href="#Wikipedia2022">[Wikipedia contributors 2022]</a>.

Figure 1 notes a few key events in cataloging crystal properties. We start with <a href="#Kepler1611">[Kepler 1611]</a> 
(translated in <a href="#Kepler1966">[Kepler <i>et al.</i> 1966]) and Steno (see <a href="#Authier2013">[Authier 2013]</a> who conjectured on the structures 
of crystals. <a name="Hauey1800">[Ha&uuml;y 1800]</a> created the first catalog of minerals.
<p>
<center>
<a name="timeline"><img src=/~]]]LRLWEBUSER[[[/images/timeline.jpg /></a>
<p>
Fig. 1. Some key dates in the history of modern crystallography
</center>
<p>

<h2>References to Sources for Information about the Calculations</h2>
<ol>
<li>Check Input: simply check the input for errors</li>
<li>compute Selling-reduced primitive cells: Delone/Delaunay/Selling reduction <a href="#Delaunay1933">[Delaunay 1933]</a>
<a href="#Delone1975">[Delone <i>et al.</i> 1975]</a> <a href="#Andrews2019a">[Andrews <i>et al.</i> 2019a]</a></li>
<li>compute NCDist and CS6Dist distances <a href="#Andrews2014">[Andrews and Bernstein 2014]</a>
<a href="#Andrews2019b">[Andrews <i>et al.</i> 2019b]</a></li>
<li>Generate cells of a particular type or types</li>
<li>apply Lattice Matching algorithm to listed cells <a href="#Mighell2002">[Mighell 2002]</a>
<a href="#Andrews2021">[Andrews and Bernstein 2021]</a></li>
<li>compute Niggli-reduced primitive cells <a href="#Niggli1928">[Niggli 1928]</a></li>
<li>compute path between pairs of cells
<a href="#Andrews2023a">[Andrews and Bernstein 2023a]</a></li>
<li>compute perturbed versions of input cells <a href="#Andrews2022">[Andrews and Bernstein 2022]</a></li>
<li>apply S6 reflections to input cells <a href="#Andrews2019b">[Andrews <i>et al.</i> 2019b]</a></li>
<li>apply Sella algorithm <a href="#Andrews2023b">[Andrews <i>et al.</i> 2023b]</a></li>
<li>compute Bravais tetrahedron (B4) <a href="#Delone1975">[Delone <i>et al.</i> 1975]</a></li>
<li>compute Selling-reduced complex cell presentation (C3) <a href="#Andrews2019b">[Andrews <i>et al.</i> 2019b]</li>
<li>compute side-angle cells (a, b, c, &alpha;, &beta;, &gamma;)</li>
<li>compute raw Dirichlet cells (DC13)</li>
<li>computed sorted Dirichlet cells (DC) <a href=#Andrews2021>[Andrews and Bernstein 2021]</li>
<li>compute G6 version of cells (G6) <a href=#Andrews1988>[Andrews and Bernstein 1988]</a></li>
<li>compute Selling-reduced cells (S6)</li>
<li>compute unsorted Dirichlet cells (dc7unsrt) <a href="Bernstein2023">[Bernstein <i>et al.</i> 2023]</a></li>
<li>compute volumes of listed cells</li>
</ol>

<p>
<a name="references"></a>
<h2>References</h2>
<p>
<a name="Andrews1988">[Andrews and Bernstein 1988]</a> L. C. Andrews and H. J. Bernstein (1988) Lattices and reduced cells as points in 
6-space and selection of Bravais lattice type by projections. Acta Cryst., A44:6 1009 -- 1018.<br />
<p>
<a name="Andrews2014">[Andrews and Bernstein 2014]</a> L. C. Andrews and H. J. Bernstein (2014). 
The geometry of Niggli reduction: BGAOL–embedding Niggli reduction and analysis of boundaries.
J. Appl. Cryst., 47(1), 346 -- 359.
<p>
<a name="Andrews2021">[Andrews and Bernstein 2021]</a> L. C. Andrews and H. J. Bernstein (2021). 
DC 7, a very efficient lattice comparison metric. Acta Cryst. A77(a2), C809.
<p>
<a name="Andrews2022">[Andrews and Bernstein 2022]</a> L. C. Andrews and H. J. Bernstein (2022).
Generating random unit cells.  J. Appl. Cryst. 55(4) 782 -- 786.
<p>
<a name="Andrews2023a">[Andrews and Bernstein 2023a]</a> L. C. Andrews and H. J. Bernstein (2023).  
The Follower algorithm and a program using it to explore spaces, Cambridge Open Engage preprint
<a href=
"https://web.archive.org/web/20230308145808id_/https://www.cambridge.org/engage/api-gateway/coe/assets/orp/resource/item/63f41c579da0bc6b3344e7f3/original/the-follower-algorithm-and-a-program-using-it-to-explore-spaces.pdf">
https://web.archive.org/web/20230308145808id_/https://www.cambridge.org/engage/api-gateway/coe/assets/orp/resource/item/63f41c579da0bc6b3344e7f3/original/the-follower-algorithm-and-a-program-using-it-to-explore-spaces.pdf</a>
<p>
<a name="Andrews2019a">[Andrews <i>et al.</i> 2019a]</a> L. C. Andrews, H. J. Bernstein, H. J. and N. K. Sauter (2019).
Selling reduction versus Niggli reduction for crystallographic lattices. Acta Cryst.  A75, 115 -- 120.
<p>
<a name="Andrews2019b">[Andrews <i>et al.</i> 2019b]</a> L. C. Andrews, H. J. Bernstein and N. K.Sauter (2019).
A space for lattice representation and clustering. Acta Cryst. A75(3), 593 -- 599. 
<p>
<a name="Andrews2023b">[Andrews <i>et al.</i> 2023b]</a> L. C. Andrews, H. J. Bernstein and N. K. Sauter (2023).
SELLA - A Program for Determining Bravais Lattice Types. arXiv preprint
<a href="https://arxiv.org/pdf/2303.03122.pdf">
https://arxiv.org/pdf/2303.03122.pdf</a>.
<p>
<a name="Authier2013">[Authier 2013]</a> A. Authier  (2013). Early days of X-ray crystallography. OUP Oxford.
<p>
<a name="Bernstein2023">[Bernstein <i>et al.</i> 2023]</a> H. J. Bernstein, L. C. Andrews and M. Xerri (2023).
An invertible seven-dimensional Dirichlet cell characterization of lattices. Acta Cryst. A79(4), 369 -- 380.
<p>
<a name="Delaunay1933">[Delaunay 1933]</a> B. N. Delaunay (1933).
Neue Darstellung der geometrischen Kristallographie: Erste Abhandlung.
 Z. Krist. 84, 109 -- 149.
<p>
<a name="Delone1975">[Delone <i>et al.</i> 1975]</a> B. N. Delone, R. V. Galiulin and M. I. Shtogrin, (1975).
On the Bravais types of lattices.
 J. Sov. Math. 4(1), 79 -- 156.
<p>
<a name="Hauey1800">[Ha&uuml;y 1800]</a>  R. J. Ha&uuml;y (1800). Addition au m&eacute;moire sur l’arragonite: 
ins&eacute;r&eacute; dans le tome XI des annnales (p. 241 et suiv.).
<p>
<a name="Kepler1611">[Kepler 1611]</a> Kepler, J. (1611). Strena Seude Niue Sexangula. Godefridum Tampach.
<p>
<a name="Kepler1966">[Kepler <i>et al.</i> 1966]</a> J. Kepler,C. G. Hardie, B. J. Mason and L. L. Whyte  (1966). 
The Six-cornered Snowflake.[Edited and Translated by Colin Hardie. With Essays by L. L. Whyte and B. J. Mason. 
With Illustrations.] Lat. & Eng. Clarendon Press.
<p>
<a name="Mighell2002">[Mighell 2002]</a> A. D. Mighell (2002).
Lattice Matching (LM) -- Prevention of Inadvertent Duplicate Publications of Crystal Structures.
J. Res. Natl. Inst. Stand. Technol. 107(5), 425 – 430.
<p>
<a name="Niggli1928">[Niggli 1928]</a> P. Niggli (1928). Krystallographische und Strukturtheoretische Grundbegriffe, 
Handbuch der Experimentalphysik, Vol. 7, part 1. Akademische Verlagsgesellschaft, Leipzig.
<p>
<a name="Wikipedia2022">[Wikipedia contributors 2022]</a> Wikipedia contributors, (2022). Theophrastus — Wikipedia, the 
free encyclopedia. [Online; accessed 17-October-2022]. https://en.wikipedia.org/w/index.php?title=Theophrastus&oldid=1114534722
<p>
<a name="Wilkins2021">[Wilkins <i>et al.</i> 2021]</a> J. Wilkins, B. J. Schoville, R. Pickering, L. Gliganic, B. Collins,
K. S.  Brown, J. von der Meden, W. Khumalo, M. C. Meyer, S. Maape, and A. F. Blackwood,  2021. Innovative 
Homo sapiens behaviours 105,000 years ago in a wetter Kalahari. Nature, 592(7853), 248 -- 252.
<p>
<a name="Zimmermann1985">[Zimmermann and Burzlaff 1985]</a> H. Zimmermann and H. Burzlaff. DELOS A computer program for the 
determination of a unique conventional cell. Zeitschrift f&uuml;r Kristallographie, 170:241 246, 1985.

<p>
<hr />
Updated 31 October 2023.
</font>
</body>
</html>]]])

