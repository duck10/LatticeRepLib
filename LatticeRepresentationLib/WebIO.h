#ifndef WEBIO_H
#define WEBIO_H

#include <algorithm>
#include <sstream>
#include <string>
#include <utility>

class WebIO {
public:
   friend std::ostream& operator<< (std::ostream& o, const WebIO& bcf);
   WebIO(int argc, char* argv[], const std::string& progName, const size_t fileCount);
   std::string RecreateFileNames(const std::string& name);
   void GetWebBlockSize(int argc, char* argv[]);
   void CompareHtmlFromWebIO(
      const std::string host,
      const std::string rawprefix,
      const std::string htmlprefix,
      const bool usetimestamp,
      const bool usehttps,
      const bool usetarget,
      const WebIO& webio);

public:
   bool m_hasWebInstructions;
   std::string m_host = "";
   std::string m_rawprefix = "";
   std::string m_htmlprefix = "";
   bool m_usetimestamp = true;
   bool m_usehttps = false;
   bool m_usetarget = true;
   const size_t m_fileNameCount;
   size_t m_blocksize = 20;
   size_t m_blockstart = 0;

};

#endif // WEBIO_H