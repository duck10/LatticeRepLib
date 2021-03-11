#ifndef LRL_StringTools_H
#define LRL_StringTools_H
#include <string>
#include <vector>

class LRL_StringTools
{
public:

   static std::string strToupper(const std::string& s);
   static std::vector<double> FromString(const std::string& s);

   static std::string ConcatanateStrings(const std::vector<std::string>& sinput);


};

#endif  // LRL_StringTools_H
