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
      os << "; MultiTransformFinderControls\n";
      os << "; show details " << (mtfc.m_showDetails ? "true" : "false") << std::endl;
      os << "; niggli delta " << mtfc.m_niggliDelta << std::endl;
      os << "; comparison mode for multi input " << mtfc.m_comparisonMode << std::endl;

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

      InputHandler::registerHandler("NIGGLIDELTA", 0.42,
         [this](const BaseControlVariables&, const std::string& value) {
            try {
               setNiggliDelta(std::stod(value));
            }
            catch (const std::exception& e) {
               std::cout << "Warning: Invalid Niggli delta value: " << value << ". Using default." << std::endl;
            }
         });

      InputHandler::registerHandler("NIGGLITOLERANCE", 0.43,
         [this](const BaseControlVariables&, const std::string& value) {
            try {
               setNiggliDelta(std::stod(value));
            }
            catch (const std::exception& e) {
               std::cout << "Warning: Invalid Niggli tolerance value: " << value << ". Using default." << std::endl;
            }
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

      InputHandler::registerHandler("MATRIXORDER", 0.43,
         [this](const BaseControlVariables&, const std::string& value) {
            try {
               if (value.empty() || LRL_StringTools::strToupper(value) == "MATRIXORDER") {
                  // Just "order" with no number - set to 1
                  setMatrixOrder(1);
               } else {
                  // "test" followed by a number - parse and set
                  int order = std::stoi(value);
                  if (order < 1 || order > 4) {
                     setMatrixOrder(1);
                  } else {
                     setMatrixOrder(order);
                  }
               }
            }
            catch (const std::exception& e) {
               std::cout << "Warning: Invalid matrix order: " << value << ". Using default (1)." << std::endl;
               setMatrixOrder(1);  // Default to all tests
            }
         });

      InputHandler::registerHandler("COMPARISONMODE", 0.45,
         [this](const BaseControlVariables&, const std::string& value) {
            setComparisonMode(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("CSVOUTPUT", 0.46,
         [this](const BaseControlVariables&, const std::string& value) {
            setCsvOutput(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

   }

   bool shouldShowDetails() const { return m_showDetails; }
   void setShowDetails(const bool b) { m_showDetails = b; }
   void setTestNumber(const int n) { m_testNumber = n; }
   int getTestNumber( ) const { return m_testNumber; }

   double getNiggliDelta() const { return m_niggliDelta; }
   void setNiggliDelta(double delta) { m_niggliDelta = delta; }

   bool shouldRunTests() const { return m_testNumber >= 0; }
   bool shouldRunComparisonMode() const { return m_comparisonMode; }
   void setComparisonMode(bool mode) { m_comparisonMode = mode; }
   bool shouldOutputCsv() const { return m_csvOutput; }
   void setCsvOutput(bool csv) { m_csvOutput = csv; }

   int getMatrixOrder() const { return m_unimodularOrder; }
   void setMatrixOrder(const int n) { m_unimodularOrder = n; }

private:
   // NEW CLEAR PARAMETER NAMES
   bool m_showDetails = false;
   // Niggli reduction parameters
   double m_niggliDelta = 1.0e-5;
   // Other parameters
   int m_testNumber = -1;
   bool m_comparisonMode = true;
   bool m_csvOutput = false;
   int m_unimodularOrder = 1;


   bool m_showProgress = true;
};

#endif // MULTITRANSFORMFINDERCONTROLS_H

