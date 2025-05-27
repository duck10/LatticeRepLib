


#include "LRL_StringTools.h"

#include <algorithm>
#include <cfloat>
#include <iostream>
#include <iterator>
#include <sstream>

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool LRL_StringTools::not_space(const char c) {
   return(c != ' ');
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool LRL_StringTools::space(const char c) {
   return(c == ' ');
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::string LRL_StringTools::strToupper(const std::string& s) {
   std::string ss(s);
   for (size_t i = 0; i < ss.length(); ++i)
      ss[i] = static_cast<char>(toupper(ss[i]));
   return(ss);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::string LRL_StringTools::strLower(const std::string& s) {
   std::string ss(s);
   for (size_t i = 0; i < ss.length(); ++i)
      ss[i] = static_cast<char>(tolower(ss[i]));
   return(ss);
}

std::vector<double> LRL_StringTools::FromString(const std::string& s) {
   std::istringstream istr(s);
   std::vector<double> t;
   double d;
   int i = 0;
   while (istr && !istr.eof()) {
      istr >> d;
      if (istr.eof() && d == DBL_MAX) break;
      t.push_back(d);
      ++i;
      d = DBL_MAX;
   }

   return t;
}


std::string LRL_StringTools::ConcatanateStrings(const std::vector<std::string>& sinput) {
   std::string s;
   for (size_t face = 0; face < sinput.size(); ++face)
      s += sinput[face];
   return s;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::vector<std::string> LRL_StringTools::SplitBetweenBlanks(const std::string& s) {
   std::vector<std::string> str;
   std::string::const_iterator i = s.begin();

   while (i != s.end())
   {
      //look for the next non-blank
      i = std::find_if(i, s.end(), not_space);
      const std::string::const_iterator i2 = std::find_if(i, s.end(), space);
      str.push_back(std::string(i, i2));
      i = i2;
   }

   return(str);
}

std::string LRL_StringTools::indentLines(const std::string& input, int numSpaces) {
   std::stringstream inputStream(input);
   std::stringstream outputStream;
   std::string line;
   std::string prefix(numSpaces, ' '); // Create a string with numSpaces blanks

   while (std::getline(inputStream, line)) {
      outputStream << prefix << line << '\n';
   }

   return outputStream.str();
}