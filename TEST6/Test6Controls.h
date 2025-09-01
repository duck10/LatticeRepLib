#pragma warning(disable: 4101) // Visual Studio 

#ifndef MULTITRANSFORMFINDERCONTROLS_H
#define MULTITRANSFORMFINDERCONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"
#include <iostream>
#include <string>


class MultiTransformFinderControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const MultiTransformFinderControls& mtfc) {
      os << "; MultiTransformFinderTest6show details " << (mtfc.m_showDetails ? "true" : "false") << std::endl;

      return os;
   }


   MultiTransformFinderControls() {
      // Register handlers for control parameters
      InputHandler::registerHandler("SHOWDETAILS", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            setShowDetails(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("DETAILS", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            setShowDetails(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("TEST", 0.43,
         [this](const BaseControlVariables&, const std::string& value) {
            try {
               if (value.empty() || LRL_StringTools::strToupper(value) == "TEST") {
                  // Just "test" with no number - set to 0 (run all tests)
                  setTestNumber(0);
               } else {
                  // "test" followed by a number - parse and set
                  int testNum = std::stoi(value);
                  if (testNum < 0) {
                     // Negative numbers should show test list
                     setTestNumber(999);
                  } else {
                     setTestNumber(testNum);
                  }
               }
            }
            catch (const std::exception& e) {
               std::cout << "Warning: Invalid test number: " << value << ". Using default (all tests)." << std::endl;
               setTestNumber(0);  // Default to all tests
            }
         });

   }

   bool shouldShowDetails() const { return m_showDetails; }
   void setShowDetails(const bool b) { m_showDetails = b; }
   void setTestNumber(const int n) { m_testNumber = n; }
   int getTestNumber( ) const { return m_testNumber; }

   bool shouldRunTests() const { return m_testNumber >= 0; }
private:
   // NEW CLEAR PARAMETER NAMES
   bool m_showDetails = false;
   int m_testNumber = -1;
};

#endif // MULTITRANSFORMFINDERCONTROLS_H

