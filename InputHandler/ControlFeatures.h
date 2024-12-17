// ControlFeatures.h
#ifndef CONTROL_FEATURES_H
#define CONTROL_FEATURES_H

#include "ControlFeature.h"
#include <string>
#include <set>
#include <vector>

// Feature for block processing
class BlockControl : public ControlFeature {
private:
   size_t blockstart{ 0 };
   size_t blocksize{ 20 };

public:
   void writeToStream(std::ostream& os) const override;
   void setBlockStart(size_t start);
   void setBlockSize(size_t size);
   size_t getBlockStart() const;
   size_t getBlockSize() const;
};

// Feature for multiple lattice type selection
class LatticeTypesControl : public ControlFeature {
private:
   std::set<std::string> enabledTypes;
   static const std::vector<std::string> VALID_TYPES;

public:
   LatticeTypesControl();
   void writeToStream(std::ostream& os) const override;
   void enableType(const std::string& type);
   void disableType(const std::string& type);

private:
   bool isValidType(const std::string& type) const;
};

// Feature for single lattice type selection
class SingleLatticeTypeControl : public ControlFeature {
private:
   std::string selectedType{ "P" };
   static const std::vector<std::string> VALID_TYPES;

public:
   void writeToStream(std::ostream& os) const override;
   void setType(const std::string& type);
   std::string getType() const;

private:
   bool isValidType(const std::string& type) const;
};

// Feature for output size control
class OutputSizeControl : public ControlFeature {
private:
   size_t outputElements{ 100 };  // Default to 100 elements

public:
   void writeToStream(std::ostream& os) const override;
   void setOutputElements(size_t count);
   size_t getOutputElements() const;
};

#endif // CONTROL_FEATURES_H
