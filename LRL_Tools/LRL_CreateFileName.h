#ifndef LRL_CREATEFILENAME_H
#define LRL_CREATEFILENAME_H

#include <string>
#include <vector>

#include <cctype>
#include <string>

// True only for a non-empty, short name built from letters, digits, '_' and '-'.
// Rejects path separators, "..", quotes, angle brackets, and anything else
// that could escape a directory or break out of an HTML attribute.
inline bool IsSafeFileNameComponent(const std::string& s) {
   if (s.empty() || s.size() > 64) return false;
   for (const char c : s) {
      const unsigned char u = static_cast<unsigned char>(c);
      if (!(std::isalnum(u) || c == '_' || c == '-')) return false;
   }
   return true;
}

class LRL_CreateFileName
{
public:
   static std::string Create(const std::string& prefix, const std::string& extension,
      const bool includeTimestamp = true);

   static std::string Create(const std::string& prefix, const std::string& suffix, 
      const std::string& extension, const bool includeTimestamp = true);

   static std::vector<std::string> CreateListOfFilenames(
      const size_t nFiles,
      const std::string& prefix,
      const std::string& extension, 
      const bool includeTimestamp = true,
      const size_t block_start = 0,
      const size_t block_size = 20);

   static std::vector<std::string> CreateRawListOfFilenames(
      const std::vector<std::string>& basicFileNameList,
      const std::string& raw_prefix);

   static std::vector<std::string> CreateHTMLListOfFilenames(
      const std::vector<std::string>& basicFileNameList,
      const std::string& host,                                                            
      const std::string& html_prefix,
      const bool use_https = false,
      const bool use_target = true);

};


#endif  //  LRL_CREATEFILENAME_H
