#ifndef INPUTHANDLER_CONTROL_FEATURES_H
#define INPUTHANDLER_CONTROL_FEATURES_H

#include <string>
#include <vector>
#include <algorithm>

class LatticeTypesControl {
private:
   static const std::vector<std::string> LATTICE_TYPE_LIST;

   static bool isValidType(const std::string& type) {
      return std::find(LATTICE_TYPE_LIST.begin(),
         LATTICE_TYPE_LIST.end(),
         type) != LATTICE_TYPE_LIST.end();
   }
};

class SingleLatticeTypeControl {
private:
   static const std::vector<std::string> SINGLE_LATTICE_TYPES;

   static bool isValidType(const std::string& type) {
      return std::find(SINGLE_LATTICE_TYPES.begin(),
         SINGLE_LATTICE_TYPES.end(),
         type) != SINGLE_LATTICE_TYPES.end();
   }
};

#endif // INPUTHANDLER_CONTROL_FEATURES_H
