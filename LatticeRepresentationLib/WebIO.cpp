#include "LRL_CreateFileName.h"
#include "WebIO.h"

#include <iostream>
#include <vector>
#include <cstdlib>

static std::string  StringFinder(const std::string& toFind, const std::string& defaultvalue, const std::vector<std::string>& args) {
   const auto f = std::find(args.begin(), args.end(), toFind);
   if (f == args.end()) return defaultvalue;
   return *(f + 1);
}

static bool BoolFinder(const std::string& toFind, const bool defaultvalue, const std::vector<std::string>& args) {
   const auto f = std::find(args.begin(), args.end(), toFind);
   if (f == args.end()) return defaultvalue;
   return *(f + 1) == "true";
}

std::ostream& operator<< (std::ostream& o, const WebIO& w) {
   //o << "; #### Bravais chain failure A\n";
   o << "\n";
   o << "; WebIO data content\n";
   o << ";\thost \t\t" << w.m_host << "\n";
   o << ";\trawprefix \t" << w.m_rawprefix << "\n";
   o << ";\thtmlprefix \t" << w.m_htmlprefix << "\n";
   o << ";\tusetimestamp \t" << w.m_usetimestamp << "\n";
   o << ";\tusehttps \t" << w.m_usehttps << "\n";
   o << ";\tusetarget \t" << w.m_usetarget << "\n";
   o << ";\tfileNameCount \t" << w.m_fileNameCount << "\n";
   o << ";\tblocksize \t" << w.m_blocksize << "\n";
   o << ";\tblockstart \t" << w.m_blockstart << "\n";
   o << "\n";

   for (const auto& name : w.m_FullfileNameList)
      o << ";\tm_FullfileNameList   \t" << name << "\n";
   o << "\n";

   for (const auto& name : w.m_basicfileNameList)
      o << ";\tm_basicfileNameList   \t" << name << "\n";
   o << "\n";

   for (const auto& name : w.m_FileNameList)
      o << ";\tm_FileNameList   \t" << name << "\n";

   std::cout << std::endl;
   return o;
}


bool containsSubstring(const std::string& str, const std::string& substring) {
   // Use string::find to check if substring exists within str
   return str.find(substring) != std::string::npos;
}

WebIO::WebIO(int argc, char* argv[], const std::string& progName, const size_t fileCount)
   : m_programName(progName)
   , m_fileNameCount(fileCount)
{
   std::vector<std::string> args;
   for (size_t i = 0; i < argc; ++i) {
      args.emplace_back(argv[i]);
   }

   const std::string searchString("--");
   // Use std::find_if to search for element containing the "--"
   auto it = find_if(args.begin(), args.end(),
      [&searchString](const std::string& element) {
         return containsSubstring(element, searchString); });

   m_hasWebInstructions = it != args.end();

   m_host = StringFinder("--host", m_host, args);
   m_rawprefix = StringFinder("--rawprefix", m_rawprefix, args);
   m_htmlprefix = StringFinder("--htmlprefix", m_htmlprefix, args);

   m_usetimestamp = BoolFinder("--usetimestamp", m_usetimestamp, args);
   m_usehttps = BoolFinder("--usehttps", m_usehttps, args);
   m_usetarget = BoolFinder("--usetimestamp", m_usetarget, args);

}

void WebIO::GetWebBlockSize(int argc, char* argv[]) {

   char * blocksizelimit;
   long lblocksizelimit;

   std::vector<std::string> args;
   for (size_t i = 0; i < argc; ++i) {
      args.emplace_back(argv[i]);
   }

   m_blockstart = std::stoul(StringFinder("--blockstart", std::to_string(m_blockstart), args));
   m_blocksize = std::stoul(StringFinder("--blocksize", std::to_string(m_blocksize), args));
   blocksizelimit = getenv(std::string("WEBIO_BLOCKSIZELIMIT").c_str());
   if (blocksizelimit != NULL) {
      lblocksizelimit=atol(blocksizelimit);
      if (lblocksizelimit < 0 ) lblocksizelimit = 0;
      if (m_blocksize > lblocksizelimit) m_blocksize = lblocksizelimit;
   }

   if (m_blockstart >= m_fileNameCount) {
      // nothing to do
      m_blockstart = 0;
      m_blocksize = 0;
   }
   else if (m_blockstart + m_blocksize > m_fileNameCount) {
      m_blocksize = m_fileNameCount - m_blockstart;
   }
   else {
      // accept the vaules of blockstart and blocksize
   }
}

void WebIO::CreateFilenamesAndLinks(const size_t n,
   const std::string& programName, const std::string& extension)
{
   m_basicfileNameList = LRL_CreateFileName::CreateListOfFilenames(n,
      programName, extension, m_usetimestamp, m_blockstart, m_blocksize);
   m_FileNameList = LRL_CreateFileName::CreateRawListOfFilenames(m_basicfileNameList, m_rawprefix);

   if (m_hasWebInstructions) {
      m_FullfileNameList = LRL_CreateFileName::CreateHTMLListOfFilenames(
         m_basicfileNameList, m_host, m_htmlprefix, m_usehttps, m_usetarget);
   }
   else {
      m_FullfileNameList = m_FileNameList;
   }
}

void WebIO::CompareHtmlFromWebIO(
   const std::string host,
   const std::string rawprefix,
   const std::string htmlprefix,
   const bool usetimestamp,
   const bool usehttps,
   const bool usetarget,
   const WebIO& webio) {
   if (host != webio.m_host) {
      std::cout << " host disagree" << std::endl;
      std::cout << host << "   " << webio.m_host << std::endl;
   }
   if (rawprefix != webio.m_rawprefix) std::cout << " rawprefix disagree" << std::endl;
   if (htmlprefix != webio.m_htmlprefix) std::cout << " htmlprefix disagree" << std::endl;
   if (usetimestamp != webio.m_usetimestamp) std::cout << " usetimestamp disagree" << std::endl;
   if (usehttps != webio.m_usehttps) std::cout << " usehttps disagree" << std::endl;
   if (usetarget != webio.m_usetarget)
   {
      std::cout << " usetarget disagree" << std::endl;
      std::cout << usetarget << "   " << webio.m_usetarget << std::endl;
   }
}
//  Graphical output SVG file = <a href="http://blondie.arcib.org:8083/~yaya/tmp/lrl_web_2948/PLT_2024-03-30.17_48_19_36e8e1_0.svg" target="_blank" >PLT_2024-03-30.17_48_19_36e8e1_0.svg</a><br />
// --usehttps false
// --host blondie.arcib.org:8083/~yaya/tmp/lrl_web_2948/
// --usetarget "_blank"

// --usetarget "_blank"// --usehttps false --host blondie.arcib.org:8083/~yaya/tmp/lrl_web_2948/ --usetarget "_blank"
