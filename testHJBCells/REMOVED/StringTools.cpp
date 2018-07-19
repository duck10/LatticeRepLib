#include "StringTools.h"

#include <algorithm>
#include <iterator>

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::string StringTools::strToupper(const std::string& s) {
   std::string ss;
   std::transform(s.begin(), s.end(), std::back_inserter(ss), toupper);
   return(ss);
}
