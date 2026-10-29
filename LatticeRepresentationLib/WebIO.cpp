#pragma warning(disable: 4996)


#include "LRL_CreateFileName.h"
#include "WebIO.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

static std::string StringFinder(const std::string& toFind, const std::string& defaultvalue, const std::vector<std::string>& args) {
   const auto f = std::find(args.begin(), args.end(), toFind);
   if (f == args.end() || (f + 1) == args.end()) return defaultvalue;
   return *(f + 1);
}

static bool BoolFinder(const std::string& toFind, const bool defaultvalue, const std::vector<std::string>& args) {
   const auto f = std::find(args.begin(), args.end(), toFind);
   if (f == args.end() || (f + 1) == args.end()) return defaultvalue;
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

   o << std::endl;
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
   for (size_t i = 0; i < static_cast<size_t>(argc); ++i) {
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
   if (m_hasWebInstructions) m_usetimestamp = true;

   m_usehttps = BoolFinder("--usehttps", m_usehttps, args);

   // NOTE: reads --usetimestamp, almost certainly meant --usetarget.
   // Not changed (Oct 2026) until the web front end's arguments are checked;
   // fixing it changes link behavior if the front end sends --usetimestamp false
   // or --usetarget with a value other than "true".
   m_usetarget = BoolFinder("--usetimestamp", m_usetarget, args);

}

void WebIO::GetWebBlockSize(int argc, char* argv[]) {

   char* blocksizelimit;
   long lblocksizelimit;

   std::vector<std::string> args;
   for (size_t i = 0; i < static_cast<size_t>(argc); ++i) {
      args.emplace_back(argv[i]);
   }

   m_blockstart = std::stoul(StringFinder("--blockstart", std::to_string(m_blockstart), args));
   m_blocksize = std::stoul(StringFinder("--blocksize", std::to_string(m_blocksize), args));
   blocksizelimit = getenv(std::string("WEBIO_BLOCKSIZELIMIT").c_str());
   if (blocksizelimit != NULL) {
      lblocksizelimit = atol(blocksizelimit);
      if (lblocksizelimit < 0) lblocksizelimit = 0;
      if (m_blocksize > static_cast<size_t>(lblocksizelimit)) m_blocksize = static_cast<size_t>(lblocksizelimit);
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

// Final authority on block size and start (Oct 2026). Call after any values
// from stdin controls have been copied in.
// Web run:     the number of output FILES is capped at WEB_MAX_BLOCKSIZE;
//              WEBIO_BLOCKSIZELIMIT can only lower that cap, never raise it.
//              With filesPerItem files per item, the item count is capped at
//              (file cap) / filesPerItem, whole items only. filesPerItem = 0
//              means the items write no files, so no file cap applies.
// Desktop run: no cap.
// Both:        the block is clamped to itemCount. The comparison is written
//              as size > count - start (not start + size > count) so huge
//              values cannot wrap around past the check.
void WebIO::ApplyBlockLimits(const size_t itemCount, const bool isWebRun,
   const size_t filesPerItem) {
   if (isWebRun && filesPerItem > 0) {
      size_t fileLimit = WEB_MAX_BLOCKSIZE;
      const char* const env = std::getenv("WEBIO_BLOCKSIZELIMIT");
      if (env != nullptr) {
         const long envLimit = std::atol(env);
         const size_t envSize = (envLimit < 0) ? 0 : static_cast<size_t>(envLimit);
         fileLimit = std::min(fileLimit, envSize);
      }
      const size_t itemLimit = fileLimit / filesPerItem;
      if (m_blocksize > itemLimit) m_blocksize = itemLimit;
   }

   if (m_blockstart >= itemCount) {
      m_blockstart = 0;
      m_blocksize = 0;
   }
   else if (m_blocksize > itemCount - m_blockstart) {
      m_blocksize = itemCount - m_blockstart;
   }
}

void WebIO::CreateFilenamesAndLinks(const size_t n,
   const std::string& programName, const std::string& extension)
{
   std::string safeName = programName;
   if (!IsSafeFileNameComponent(safeName)) {
      std::cerr << ";Warning: invalid file name prefix '" << programName
         << "' replaced by '" << m_programName << "'" << std::endl;
      safeName = m_programName;
   }

   m_basicfileNameList = LRL_CreateFileName::CreateListOfFilenames(n,
      safeName, extension, m_usetimestamp, m_blockstart, m_blocksize);
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