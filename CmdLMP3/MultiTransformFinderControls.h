#pragma warning(disable: 4101) // Visual Studio

#ifndef MULTITRANSFORMFINDERCONTROLS_H
#define MULTITRANSFORMFINDERCONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"

#include <iostream>
#include <string>
#include <vector>


class MultiTransformFinderControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const MultiTransformFinderControls& mtfc) {
      os << "; CmdLMP3 controls:\n";
      os << ";   details        " << (mtfc.m_showDetails ? "true" : "false") << "\n";
      os << ";   csv output     " << (mtfc.m_csvOutput ? "true" : "false") << "\n";
      os << ";   mode           " << mtfc.getModeString()
         << "  (ALL=equivalent+supercell, SUPER=supercell only, EQUIVALENT=same lattice only)\n";
      os << ";   useHNF         " << (mtfc.m_useHNF ? "true" : "false")
         << "  (55 HNF matrices vs ~360K brute-force)\n";
      os << ";   niggli delta   " << mtfc.m_niggliDelta << "\n";
      return os;
   }

   MultiTransformFinderControls() {
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
            try { setNiggliDelta(std::stod(value)); }
            catch (...) { std::cout << "; Warning: Invalid Niggli delta value: " << value << "\n"; }
         });

      InputHandler::registerHandler("NIGGLITOLERANCE", 0.43,
         [this](const BaseControlVariables&, const std::string& value) {
            try { setNiggliDelta(std::stod(value)); }
            catch (...) { std::cout << "; Warning: Invalid Niggli tolerance value: " << value << "\n"; }
         });

      InputHandler::registerHandler("USESELLING", 0.43,
         [this](const BaseControlVariables&, const std::string& value) {
            try { setUseSellingReduction(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty()); }
            catch (...) { std::cout << "; Warning: Invalid UseSelling value: " << value << "\n"; }
         });

      // TEST with no argument or TEST 0 runs all tests.
      // Any negative number is treated as 0 (run all) with a warning.
      // TEST n (n > 0) runs only test n.
      InputHandler::registerHandler("TEST", 0.43,
         [this](const BaseControlVariables&, const std::string& value) {
            m_runTests = true;
            if (value.empty() || LRL_StringTools::strToupper(value) == "TEST") {
               m_testNumber = 0;
            } else {
               try {
                  const int n = std::stoi(value);
                  if (n < 0) {
                     std::cout << "; Warning: Negative test number " << n
                        << " treated as 0 (run all tests)\n";
                     m_testNumber = 0;
                  } else {
                     m_testNumber = n;
                  }
               }
               catch (...) {
                  std::cout << "; Warning: Invalid test number: " << value
                     << " -- running all tests\n";
                  m_testNumber = 0;
               }
            }
         });

      InputHandler::registerHandler("MATRIXORDER", 0.43,
         [this](const BaseControlVariables&, const std::string& value) {
            try {
               if (value.empty() || LRL_StringTools::strToupper(value) == "MATRIXORDER")
                  setMatrixOrder(1);
               else {
                  const int order = std::stoi(value);
                  setMatrixOrder((order < 1 || order > 4) ? 1 : order);
               }
            }
            catch (...) {
               std::cout << "; Warning: Invalid matrix order: " << value << "\n";
               setMatrixOrder(1);
            }
         });

      InputHandler::registerHandler("CSVOUTPUT", 0.46,
         [this](const BaseControlVariables&, const std::string& value) {
            setCsvOutput(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      // ------------------------------------------------------------------
      // MODE: restrict matrix search to a subset.
      //   MODE ALL         search det=1 and det=2,3,4 (default)
      //   MODE SUPER       search det=2,3,4 only
      //   MODE EQUIVALENT  search det=1 only
      // ------------------------------------------------------------------
      InputHandler::registerHandler("MODE", 0.45,
         [this](const BaseControlVariables&, const std::string& value) {
            const std::string v = LRL_StringTools::strToupper(value);
            if (v == "SUPER")
               m_runMode = RunMode::SUPER;
            else if (v == "EQUIVALENT")
               m_runMode = RunMode::EQUIVALENT;
            else if (v == "ALL" || v.empty())
               m_runMode = RunMode::ALL;
            else
               std::cout << "; Warning: MODE must be ALL, SUPER, or EQUIVALENT -- ignoring \""
               << value << "\"\n";
         });

      // ------------------------------------------------------------------
      // USEHNF: use Hermite Normal Form matrices for supercell matching.
      // HNF is the minimal complete set (55 matrices for orders 2,3,4)
      // for primitive P cells (Santoro & Mighell, Acta Cryst. A28, 1972).
      // May miss relationships for non-standard cell orientations.
      // ------------------------------------------------------------------
      InputHandler::registerHandler("USEHNF", 0.45,
         [this](const BaseControlVariables&, const std::string& value) {
            m_useHNF = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });
   }

   // -----------------------------------------------------------------------
   // Run mode enum
   // -----------------------------------------------------------------------
   enum class RunMode { ALL, SUPER, EQUIVALENT };

   // -----------------------------------------------------------------------
   // Accessors
   // -----------------------------------------------------------------------
   bool shouldShowDetails()        const { return m_showDetails; }
   void setShowDetails(bool b) { m_showDetails = b; }

   // m_runTests is set by the TEST handler; m_testNumber = 0 means run all.
   bool shouldRunTests()           const { return m_runTests; }
   int  getTestNumber()            const { return m_testNumber; }
   void setTestNumber(int n) { m_runTests = true; m_testNumber = (n < 0) ? 0 : n; }

   double getNiggliDelta()         const { return m_niggliDelta; }
   void   setNiggliDelta(double d) { m_niggliDelta = d; }

   bool shouldOutputCsv()          const { return m_csvOutput; }
   void setCsvOutput(bool b) { m_csvOutput = b; }

   int  getMatrixOrder()           const { return m_unimodularOrder; }
   void setMatrixOrder(int n) { m_unimodularOrder = n; }

   void setUseSellingReduction(bool b) { m_useSellingReduction = b; }
   bool getUseSellingReduction()   const { return m_useSellingReduction; }

   bool    useHNF()                const { return m_useHNF; }
   RunMode getRunMode()            const { return m_runMode; }
   bool    runModeSuper()          const { return m_runMode == RunMode::SUPER; }
   bool    runModeEquivalent()     const { return m_runMode == RunMode::EQUIVALENT; }
   std::string getModeString()     const {
      if (m_runMode == RunMode::SUPER)      return "SUPER";
      if (m_runMode == RunMode::EQUIVALENT) return "EQUIVALENT";
      return "ALL";
   }

private:
   bool    m_showDetails = false;
   double  m_niggliDelta = 1.0e-5;
   bool    m_runTests = false;   // set true only when TEST keyword is seen
   int     m_testNumber = 0;       // 0 = run all tests; >0 = specific test
   bool    m_csvOutput = false;
   int     m_unimodularOrder = 1;
   bool    m_useSellingReduction = false;
   RunMode m_runMode = RunMode::ALL;
   bool    m_useHNF = false;
};

#endif // MULTITRANSFORMFINDERCONTROLS_H