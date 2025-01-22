#ifndef CMDGEN_CONTROLS_H
#define CMDGEN_CONTROLS_H

#include "BaseControlVariables.h"
#include "CmdGenControls.h"
#include "CmdGenUtils.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"

#include <iostream>
#include <ostream>

class CmdGenControls : public BaseControlVariables {
public:

   friend std::ostream& operator<< (std::ostream& os, const CmdGenControls& cgc) {
      os << ";CmdGen controls\n";
      os << ";number of cells of each type to generate: " << cgc.generateCount << std::endl;
      os << ";raw types to generate: " << cgc.types << ":";
      for (const auto& t : cgc.selectedTypes) {
         os << " " << t << std::endl;
      }
      return os;
   }

   CmdGenControls() {
      InputHandler::clearHandlers();

      InputHandler::registerHandler("COUNT", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            generateCount = CmdGenUtils::validatePointCount(stoi(value));
         });


      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.size()==0);
         }
      );

      InputHandler::registerHandler("TYPE", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            std::istringstream iss(value);
            std::string token;
            while (iss >> token) {
               const std::string type = CmdGenUtils::validateTypes(token);
               selectedTypes.insert(value);
            }
         });
   }

   int getCount() const { return generateCount; }
   void setCount(const int n) { generateCount = n; }

   std::set<std::string> getTypes() const { return selectedTypes; }
   void addType(const std::string& s) { selectedTypes.insert(s); }

   std::vector<std::string> getTypesAsVector() const {
      std::vector<std::string> out;
      for (const auto& s : selectedTypes) {
         out.emplace_back(s);
      }
      return out;
   }
   std::string getTypesAsString() const {
      std::string out;
      for (const auto& s : selectedTypes) {
         out += s + " ";
      }
      return out;
   }

   bool getWebRun() const { return webRun; }
   void setWebRun(const bool webrun) { webRun = webrun; }


   size_t getBlockStart() const { return 0; }
   size_t getBlockSize() const { return 0; }
   std::string getPrefix() const { return "error"; }

private:
   int generateCount = 5;
   std::string types;
   std::set<std::string> selectedTypes;
   bool showControls = false;
   bool webRun = false;
};
#endif // CMDGEN_CONTROLS_H