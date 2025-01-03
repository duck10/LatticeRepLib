// CmdGenFeatures.h
#ifndef INPUTHANDLER_CMD_GEN_FEATURES_H
#define INPUTHANDLER_CMD_GEN_FEATURES_H

#include "BaseControlVariables.h"
#include "ControlFeature.h"


#include <string>
#include <vector>

class CmdGenControlVariables : public BaseControlVariables {
public:
   CmdGenControlVariables();
   void setupHandlers();
};

class GenCountControl : public ControlFeature {
public:
   GenCountControl();
   bool handleInput(const std::string& command, const std::string& value) override;
   std::string getFeatureState() const override;
   void writeToStream(std::ostream& os) const override;

   void setCount(size_t n);
   size_t getCount() const;

private:
   size_t count;
};

class GenTypeListControl : public ControlFeature {
public:
   GenTypeListControl();
   bool handleInput(const std::string& command, const std::string& value) override;
   std::string getFeatureState() const override;
   void writeToStream(std::ostream& os) const override;

   void addType(const std::string& type);
   std::string getTypeList() const;
   std::vector<std::string> getTypesAsVector() const;
   void clear();
   void setTypeList(const std::string& types);

private:
   std::string typeList;
};

#endif