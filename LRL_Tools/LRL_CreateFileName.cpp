

#include "LRL_CreateFileName.h"
#include "LRL_DataToSVG.h"

#include <ctime>
#include <cstdio>


#pragma warning( disable: 4996 )
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

std::vector<std::string> LRL_CreateFileName::CreateListOfFilenames(const int nFiles,
   const std::string& prefix,
   const std::string& extension, const bool includeTimestamp)
{
   std::vector<std::string> out;

   for (size_t i = 0; i < nFiles; ++i) {
      const std::string filename = LRL_CreateFileName::Create
      (prefix, LRL_DataToSVG(i + 1), extension, includeTimestamp);
      out.emplace_back(filename);
   }
   return out;
}
