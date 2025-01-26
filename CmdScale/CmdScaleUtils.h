#ifndef CMD_SCALE_UTILS
#define CMD_SCALE_UTILS

#include <algorithm>
#include <string>

#include "LRL_StringTools.h"

namespace CmdScaleUtils
{
   std::string verifyScaleType(const std::string& s) {
      static const std::string allowedTypes("S6 V VOLUME");
      const std::string upper = LRL_StringTools::strToupper(s);
      const auto result = std::search(allowedTypes.begin(), allowedTypes.end(), 
         upper.begin(), upper.end());
      if (result == allowedTypes.end()) {
         return "VOLUME";
      }
      else {
         return upper;
      }
   }
}


#endif  // CMD_SCALE_UTILS