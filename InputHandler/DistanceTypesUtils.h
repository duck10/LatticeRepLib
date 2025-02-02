#ifndef INPUTHANDLER_DISTANCE_TYPES_UTILS_H
#define INPUTHANDLER_DISTANCE_TYPES_UTILS_H

#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <sstream>
#include <iostream>

namespace DistanceTypesUtils {
   // Move definition to header
   static inline const std::vector<std::string> VALID_TYPES = {
       "CS", "NC", "V7", "R9", "RI", "U", "DC7U"
   };

   inline bool isValidType(const std::string& type) {
      std::string upperType = type;
      std::transform(upperType.begin(), upperType.end(), upperType.begin(), ::toupper);
      return std::find(VALID_TYPES.begin(), VALID_TYPES.end(), upperType) != VALID_TYPES.end();
   }

   inline std::set<std::string> validateTypes(const std::string& value) {
      std::istringstream iss(value);
      std::string type;
      std::set<std::string> validTypes;
      bool anyValid = false;

      while (iss >> type) {
         if (isValidType(type)) {
            validTypes.insert(type);
            anyValid = true;
         }
         else {
            std::cerr << ";Warning: Invalid distance type '" << type << "'" << std::endl;
         }
      }

      if (!anyValid) {
         std::cerr << ";Warning: No valid types provided, using defaults CS NC" << std::endl;
         validTypes.insert("CS");
         validTypes.insert("NC");
      }
      return validTypes;
   }
}

#endif
