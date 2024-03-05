

#include "LRL_CreateFileName.h"
#include "LRL_DataToSVG.h"

#include <ctime>
#include <cstdio>
#include <iomanip>
#include <thread>
 
#ifdef __unix__
#include <unistd.h>
#else
#pragma warning( disable: 4996 )
#endif

///////////////////////////////////////////////////////////////////////
// Create a unique file name from the date and time
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/*static*/ std::string LRL_CreateFileName::Create(const std::string& prefix, const std::string& suffix,
   const std::string& extension, const bool includeTimestamp/* = true*/) 
{
   //const std::string test = Create(prefix, "") + suffix + "." + extension;
   //return test;


   const time_t now = static_cast<long int>(time(NULL));
   char buft[256];
   const int len = sizeof(buft);
   buft[0] = '\0';
   if (includeTimestamp) strftime(buft, sizeof(buft), "%Y-%m-%d.%X", localtime(&now));



   for (int i = 0; i < len; ++i)
   {
      // blanks are irritating and colons are illegal
      if (buft[i] == ' ' || buft[i] == ':') buft[i] = '_';
   }

   return(prefix + "_" + std::string(buft) + "_" + suffix + "." + extension);


}

/*static*/ std::string LRL_CreateFileName::Create( const std::string& prefix,
   const std::string& extension, const bool includeTimestamp/*=true*/ )
//---------------------------------------------------------------------
{
   const time_t now = static_cast<long int>(time(NULL));
   char buft[256];
   const int len = sizeof(buft);
   buft[0] = '\0';
   if (includeTimestamp) strftime(buft, sizeof(buft), "%Y-%m-%d.%X", localtime(&now));

   for (int i = 0; i < len; ++i)
   {
      // blanks are irritating and colons are illegal
      if (buft[i] == ' ' || buft[i] == ':') buft[i] = '_';
   }

   return(prefix + "_" + std::string(buft) + "." + extension);
}

/*static*/ std::vector<std::string> LRL_CreateFileName::CreateListOfFilenames(
   const size_t nFiles,
   const std::string& prefix,
   const std::string& extension, 
   const bool includeTimestamp /*= true*/,
   const size_t block_start /*= 0*/,
   const size_t block_size /*= 20*/)
{
   std::vector<std::string> out;
   const int pid = getpid();

   for (size_t i = block_start; (i < nFiles) && (i < block_start+block_size); ++i) {
      std::ostringstream o;
      o << std::hex << pid;
      o << "_";
      o <<i;
      const std::string filename = LRL_CreateFileName::Create
      (prefix,o.str(), extension, includeTimestamp);
      out.emplace_back(filename);
   }
   return out;
}

/*static*/ std::vector<std::string> LRL_CreateFileName::CreateRawListOfFilenames(
    const std::vector<std::string>& basicFileNameList,
    const std::string& raw_prefix)
{
    std::vector<std::string> out;

    for (size_t i=0; i < basicFileNameList.size(); ++i) {
       out.emplace_back(raw_prefix+basicFileNameList[i]);
    }
    return out;
}
    

/*static*/ std::vector<std::string> LRL_CreateFileName::CreateHTMLListOfFilenames(
    const std::vector<std::string>& basicFileNameList,
    const std::string& host,
    const std::string& html_prefix,
    const bool use_https /*=false*/,
    const bool use_target /*=true*/ )
{
    std::string filename;
    std::vector<std::string> out;

    if (host.compare(std::string(""))==0) {
      for (size_t i=0; i < basicFileNameList.size(); ++i) {
          filename=basicFileNameList[i];
          out.emplace_back(std::string("<a href=\"")
              + html_prefix+filename
              + (use_target?std::string("\" target=\"_blank\""):
                  std::string(""))
              + std::string(">")+filename+std::string("</a>"));
      }
    } else {
      std::string host_string;
      if (use_https) {
          host_string=std::string("<a href=\"https://")+host+std::string("/");
      } else {
          host_string=std::string("<a href=\"http://")+host+std::string("/");
      }
      for(size_t i=0; i < basicFileNameList.size(); ++i) {
         filename=basicFileNameList[i];
         out.emplace_back(host_string
              + html_prefix+filename
              + (use_target?std::string("\" target=\"_blank\""):
                  std::string(""))
              + std::string(">")+filename+std::string("</a>"));
      }

    }

    return out;
}
    
