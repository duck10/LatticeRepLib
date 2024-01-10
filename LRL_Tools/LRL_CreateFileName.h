#ifndef LRL_CREATEFILENAME_H
#define LRL_CREATEFILENAME_H

#include <string>
#include <vector>

class LRL_CreateFileName
{
public:
   static std::string Create(const std::string& prefix, const std::string& extension, const bool includeTimestamp = true);
   static std::string Create(const std::string& prefix, const std::string& suffix, 
      const std::string& extension, const bool includeTimestamp = true);
   static std::vector<std::string> CreateListOfFilenames( const int nFiles, 
      const std::string& prefix,
      const std::string& extension, const bool includeTimestamp = true);
};


#endif  //  LRL_CREATEFILENAME_H
