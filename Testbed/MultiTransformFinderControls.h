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
      os << "; max matrix coefficient " << mtfc.m_maxMatrixCoefficient << std::endl;
      os << "; success threshold " << mtfc.m_successThreshold << std::endl;
      os << "; use niggli reduction " << (mtfc.m_useNiggliReduction ? "true" : "false") << std::endl;
      os << "; niggli delta " << mtfc.m_niggliDelta << std::endl;

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

      // Niggli reduction handlers
      InputHandler::registerHandler("USENIGGLIREDUCTION", 0.40,
         [this](const BaseControlVariables&, const std::string& value) {
            setUseNiggliReduction(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("USENIGGLI", 0.41,
         [this](const BaseControlVariables&, const std::string& value) {
            setUseNiggliReduction(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
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
               setTestNumber(std::stoi(value));
            }
            catch (const std::exception& e) {
               std::cout << "Warning: test number: " << value << ". Using default." << std::endl;
            }
         });

      // Also add an alias
      InputHandler::registerHandler("CSVOUTPUT", 0.46,
         [this](const BaseControlVariables&, const std::string& value) {
            setCsvOutput(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

   }

   void setShowDetails(const bool b) { m_showDetails = b; }
   void setTestNumber(const int n) { m_testNumber = n; }
   int getTestNumber( ) const { return m_testNumber; }

   double getNiggliDelta() const { return m_niggliDelta; }
   void setNiggliDelta(double delta) { m_niggliDelta = delta; }

   bool shouldUseNiggliReduction() const { return m_useNiggliReduction; }
   void setUseNiggliReduction(bool use) { m_useNiggliReduction = use; }

   bool shouldRunTests() const { return m_testNumber != 0; }
   bool shouldOutputCsv() const { return m_csvOutput; }
   void setCsvOutput(bool csv) { m_csvOutput = csv; }

   void setSuccessThreshold(double threshold) {
      m_successThreshold = threshold;
   }
   double getSuccessThreshold() const {
      return m_successThreshold;
   }

   // === CONVERGENCE THRESHOLD ===  
// Used for bidirectional algorithm convergence detection
// Default: 1e-6 (when forward/backward searches meet)
   void setConvergenceThreshold(double threshold) {
      m_convergenceThreshold = threshold;
   }
   double getConvergenceThreshold() const {
      return m_convergenceThreshold;
   }

private:
   // NEW CLEAR PARAMETER NAMES
   bool m_showDetails = false;
   int m_maxMatrixCoefficient = 2;           // Keep as-is
   // Niggli reduction parameters
   double m_niggliDelta = 1.0e-5;
   bool m_useNiggliReduction = true;
   // Other parameters
   int m_testNumber = 0;
   bool m_csvOutput = false;

   // === THREE THRESHOLD SYSTEM ===
   double m_successThreshold = 0.1;       // For determining if match is successful
   double m_convergenceThreshold = 1.e-6;   // For bidirectional algorithm convergence  
};

#endif // MULTITRANSFORMFINDERCONTROLS_H

