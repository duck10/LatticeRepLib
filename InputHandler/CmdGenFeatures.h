// CmdGenFeatures.h
#ifndef CMDGEN_FEATURES_H
#define CMDGEN_FEATURES_H

#include "ControlFeature.h"
#include "BaseControlVariables.h"
#include <string>
#include <sstream>
#include <vector>

// Control for number of objects to generate
class GenCountControl : public ControlFeature {
private:
   size_t count{ 0 };  // Default to 0 so we can tell if there was input

public:
   void writeToStream(std::ostream& os) const override;
   void setCount(size_t n);
   size_t getCount() const;
};

// Control for type list
class GenTypeListControl : public ControlFeature {
private:
   std::string typeList;

public:
   void writeToStream(std::ostream& os) const override;
   void setTypeList(const std::string& types);
   void addType(const std::string& type);
   std::string getTypeList() const;
   std::vector<std::string> getTypesAsVector() const;
   void clear();
};

class CmdGenControlVariables : public BaseControlVariables {
public:
   CmdGenControlVariables();
   static void setupHandlers();
};

#endif // CMDGEN_FEATURES_H