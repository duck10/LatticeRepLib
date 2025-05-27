#ifndef LRL_StringTools_H
#define LRL_StringTools_H
#include <string>
#include <vector>

class LRL_StringTools
{
public:

   static std::string strToupper(const std::string& s);
   static std::string strLower(const std::string& s);
   static std::vector<double> FromString(const std::string& s);

   static std::string ConcatanateStrings(const std::vector<std::string>& sinput);
   static std::vector<std::string> SplitBetweenBlanks(const std::string& s);

   static bool not_space(const char c);
   static bool space(const char c);

   static std::string indentLines(const std::string& input, int numSpaces);
};

#endif  // LRL_StringTools_H
