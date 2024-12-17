// ControlFeatures.cpp
#include "ControlFeatures.h"
#include <algorithm>

// Static member initialization
const std::vector<std::string> LatticeTypesControl::VALID_TYPES = { "P", "I", "F", "C", "A" };
const std::vector<std::string> SingleLatticeTypeControl::VALID_TYPES = { "P", "I", "F", "C", "A" };

// BlockControl implementation
void BlockControl::writeToStream(std::ostream& os) const {
   os << ";Block Start: " << blockstart << "\n"
      << ";Block Size: " << blocksize << "\n";
}

void BlockControl::setBlockStart(size_t start) { blockstart = start; }
void BlockControl::setBlockSize(size_t size) { blocksize = size; }
size_t BlockControl::getBlockStart() const { return blockstart; }
size_t BlockControl::getBlockSize() const { return blocksize; }

// LatticeTypesControl implementation
LatticeTypesControl::LatticeTypesControl() {
   enabledTypes.insert("P");  // Default to primitive
}

void LatticeTypesControl::writeToStream(std::ostream& os) const {
   os << ";Enabled Lattice Types: ";
   for (const auto& type : enabledTypes) {
      os << type << " ";
   }
   os << "\n";
}

void LatticeTypesControl::enableType(const std::string& type) {
   if (isValidType(type)) enabledTypes.insert(type);
}

void LatticeTypesControl::disableType(const std::string& type) {
   enabledTypes.erase(type);
}

bool LatticeTypesControl::isValidType(const std::string& type) const {
   return std::find(VALID_TYPES.begin(), VALID_TYPES.end(), type) != VALID_TYPES.end();
}

// SingleLatticeTypeControl implementation
void SingleLatticeTypeControl::writeToStream(std::ostream& os) const {
   os << ";Selected Lattice Type: " << selectedType << "\n";
}

void SingleLatticeTypeControl::setType(const std::string& type) {
   if (isValidType(type)) selectedType = type;
}

std::string SingleLatticeTypeControl::getType() const {
   return selectedType;
}

bool SingleLatticeTypeControl::isValidType(const std::string& type) const {
   return std::find(VALID_TYPES.begin(), VALID_TYPES.end(), type) != VALID_TYPES.end();
}

// OutputSizeControl implementation
void OutputSizeControl::writeToStream(std::ostream& os) const {
   os << ";Output Elements: " << outputElements << "\n";
}

void OutputSizeControl::setOutputElements(size_t count) {
   outputElements = count;
}

size_t OutputSizeControl::getOutputElements() const {
   return outputElements;
}