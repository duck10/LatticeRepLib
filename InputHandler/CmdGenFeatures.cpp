// CmdGenFeatures.cpp
#include "CmdGenFeatures.h"
#include "InputHandler.h"
#include <sstream>

// GenCountControl implementations
GenCountControl::GenCountControl() : count(0) {
   InputHandler::registerHandler("COUNT", 0.35,
      [this](BaseControlVariables&, const std::string& value) {
         setCount(std::stoul(value));
      });
}

bool GenCountControl::handleInput(const std::string& command, const std::string& value) {
   return false;  // Now handled via InputHandler
}

std::string GenCountControl::getFeatureState() const {
   std::ostringstream oss;
   oss << ";Generate Count: " << count << "\n";
   return oss.str();
}

void GenCountControl::writeToStream(std::ostream& os) const {
   os << getFeatureState();
}

void GenCountControl::setCount(size_t n) {
   count = n;
}

size_t GenCountControl::getCount() const {
   return count;
}

void GenTypeListControl::addType(const std::string& type) {
   if (!typeList.empty()) {
      typeList += " ";
   }
   typeList += " " + type;
}

std::string GenTypeListControl::getTypeList() const {
   return typeList;
}

std::vector<std::string> GenTypeListControl::getTypesAsVector() const {
   std::vector<std::string> types;
   std::istringstream iss(typeList);
   std::string type;
   while (iss >> type) {
      types.push_back(type);
   }
   return types;
}

void GenTypeListControl::clear() {
   typeList.clear();
}

CmdGenControlVariables::CmdGenControlVariables() {
   features.push_back(std::make_unique<GenCountControl>());
   features.push_back(std::make_unique<GenTypeListControl>());
}

void CmdGenControlVariables::setupHandlers() {
   InputHandler::clearHandlers();

   InputHandler::registerHandler("COUNT", 0.35,
      [](BaseControlVariables& cv, const std::string& value) {
         if (auto* countCtrl = cv.getFeature<GenCountControl>()) {
            countCtrl->setCount(std::stoul(value));
         }
      });

   InputHandler::registerHandler("TYPES", 0.35,
      [](BaseControlVariables& cv, const std::string& value) {
         if (auto* typeCtrl = cv.getFeature<GenTypeListControl>()) {
            typeCtrl->setTypeList(value);
         }
      });
}
