#pragma warning(disable: 4101) // Visual Studio

#ifndef MULTITRANSFORMFINDERCONTROLS_H
#define MULTITRANSFORMFINDERCONTROLS_H

#include "BaseControlVariables.h"
#include "HelpLMP3.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>


class MultiTransformFinderControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const MultiTransformFinderControls& mtfc) {
      os << "; CmdLMP3 controls:\n";
      os << ";   details        " << (mtfc.m_showDetails ? "true" : "false") << "\n";
      //os << ";   csv output     " << (mtfc.m_csvOutput ? "true" : "false") << "\n";
      os << ";   mode           " << mtfc.getModeString()
         << "  (ALL=equivalent+supercell concatenated, SUPER=supercell only,\n"
         << ";                  EQUIVALENT=same lattice only, COMPOSED=equivalent*supercell composed)\n";
      os << ";   useHNF         " << (mtfc.m_useHNF ? "true" : "false")
         << "  (55 HNF matrices vs ~360K brute-force)\n";
      os << ";   niggli delta   " << mtfc.m_niggliDelta << "\n";
      os << ";   run tests      " << (mtfc.m_runTests ? "true" : "false");
      if (!mtfc.m_testNumbers.empty()) {
         os << "  tests:";
         for (int n : mtfc.m_testNumbers) os << " " << n;
      }
      os << "\n";
      os << ";   matrix element max " << mtfc.m_unimodularOrder << "\n";
      os << ";   displaymode    " << mtfc.getDisplayModeString()
         << "  (OLD=primitive, NEW=conventional frame, BOTH=show both)\n";
      os << ";   threshold      " << mtfc.m_conventionalThreshold
         << "  (max decentered P3 distance for NEW display, % of reference P3 norm)\n";
      os << ";   ratio tolerance " << mtfc.m_ratioTolerance
         << "  (confidence bar in [0,0.5] for including a 2nd-nearest determinant; higher = stricter/faster)\n";
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

      // TEST with no argument runs all tests.
      // TEST n (n > 0) adds test n to the list of tests to run.
      // TEST n1 n2 n3 ... adds multiple tests in one line.
      // TEST can also be repeated on separate lines; each occurrence adds to
      // the accumulated list rather than replacing it.
      // Any negative number is ignored with a warning.
      // If no valid test number is ever given, all tests run (m_testNumbers stays empty).
      InputHandler::registerHandler("TEST", 0.43,
         [this](const BaseControlVariables&, const std::string& value) {
            m_runTests = true;
            if (value.empty() || LRL_StringTools::strToupper(value) == "TEST") {
               return;  // no numbers given on this line -- run all (unless other TEST lines added numbers)
            }
            std::istringstream iss(value);
            std::string token;
            while (iss >> token) {
               try {
                  const int n = std::stoi(token);
                  if (n < 0) {
                     std::cout << "; Warning: Negative test number " << n << " ignored\n";
                  }
                  else if (n == 0) {
                     // TEST 0 means run all; clear any accumulated list
                     m_testNumbers.clear();
                  }
                  else {
                     m_testNumbers.push_back(n);
                  }
               }
               catch (...) {
                  std::cout << "; Warning: Invalid test number: " << token << " -- ignored\n";
               }
            }
         });

      //InputHandler::registerHandler("CSVOUTPUT", 0.46,
      //   [this](const BaseControlVariables&, const std::string& value) {
      //      setCsvOutput(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
      //   });

      // ------------------------------------------------------------------
      // MODE: restrict matrix search to a subset.
      //   MODE ALL         search det=1 and det=2..6, concatenated (default)
      //   MODE SUPER       search det=2..6 only
      //   MODE EQUIVALENT  search det=1 only
      //   MODE COMPOSED    search det=1 and det=2..6, COMPOSED (u * s for
      //                    every pair) rather than concatenated
      // ------------------------------------------------------------------
      InputHandler::registerHandler("MODE", 0.45,
         [this](const BaseControlVariables&, const std::string& value) {
            const std::string v = LRL_StringTools::strToupper(value);
            if (v == "SUPER")
               m_runMode = RunMode::SUPER;
            else if (v == "EQUIVALENT")
               m_runMode = RunMode::EQUIVALENT;
            else if (v == "COMPOSED")
               m_runMode = RunMode::COMPOSED;
            else if (v == "ALL" || v.empty())
               m_runMode = RunMode::ALL;
            else
               std::cout << "; Warning: MODE must be ALL, SUPER, EQUIVALENT, or COMPOSED -- ignoring \""
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

      // ------------------------------------------------------------------
      // DISPLAYMODE: choose which display(s) to show for each match.
      //   DISPLAYMODE OLD    primitive-space display only (default)
      //   DISPLAYMODE NEW    conventional-frame display only
      //   DISPLAYMODE BOTH   show both displays
      // ------------------------------------------------------------------
      InputHandler::registerHandler("DISPLAYMODE", 0.45,
         [this](const BaseControlVariables&, const std::string& value) {
            const std::string v = LRL_StringTools::strToupper(value);
            if (v == "NEW")
               m_displayMode = DisplayMode::NEW;
            else if (v == "BOTH")
               m_displayMode = DisplayMode::BOTH;
            else if (v == "OLD" || v.empty())
               m_displayMode = DisplayMode::OLD;
            else
               std::cout << "; Warning: DISPLAYMODE must be OLD, NEW, or BOTH -- ignoring \""
               << value << "\"\n";
         });

      // ------------------------------------------------------------------
      // THRESHOLD: max decentered P3 distance, as a percentage of the
      // reference cell's P3 norm, for which the conventional-frame (NEW)
      // display is shown. Above this, NEW display falls back to the
      // primitive-frame match with a short note explaining why.
      //
      // A percentage (not an absolute Angstrom distance) is used because
      // the same absolute P3 mismatch means something different for a
      // small cell than a large one; comparing against the reference's
      // own P3 norm keeps the cutoff scale invariant. Default 2.0 matches
      // the existing GOOD quality tier.
      // ------------------------------------------------------------------
      InputHandler::registerHandler("THRESHOLD", 0.45,
         [this](const BaseControlVariables&, const std::string& value) {
            try { setConventionalThreshold(std::stod(value)); }
            catch (...) { std::cout << "; Warning: Invalid THRESHOLD value: " << value << "\n"; }
         });

      // ------------------------------------------------------------------
      // RATIOTOLERANCE: a confidence bar (in [0, 0.5]) for whether a
      // second, non-nearest determinant is also searched for a given
      // reference/mobile primitive-cell volume ratio r. Modeling a
      // determinant d's likelihood of being correct as L(distance) =
      // max(0, 1 - distance) -- 100% at distance 0, 50% at distance 0.5
      // (an exact tie between neighbors, e.g. r=3.5), 0% at distance >=1 --
      // d is included whenever L(|d - r|) >= RATIOTOLERANCE, which reduces
      // to the fixed comparison |d - r| <= 1 - RATIOTOLERANCE. The single
      // nearest determinant is always included automatically (its
      // likelihood is always >= 50% >= RATIOTOLERANCE, since RATIOTOLERANCE
      // is capped at 0.5). Default 0.20: only search a second determinant
      // if it has at least a 20% chance of being the right one.
      //
      // NOTE: larger values here mean a STRICTER (narrower, faster)
      // search -- the opposite sense from a plain widening margin. 0.5
      // is the strictest useful value (never search a second candidate
      // unless it's tied with the nearest); 0 is the loosest (search any
      // determinant within distance 1.0 of r).
      // ------------------------------------------------------------------
      InputHandler::registerHandler("RATIOTOLERANCE", 0.45,
         [this](const BaseControlVariables&, const std::string& value) {
            try { setRatioTolerance(std::stod(value)); }
            catch (...) { std::cout << "; Warning: Invalid RATIOTOLERANCE value: " << value << "\n"; }
         });

      InputHandler::registerHandler("HELP", 0.30,
         [this](const BaseControlVariables&, const std::string&) {
            ShowHelp();
         });

      InputHandler::registerHandler("?", 0.30,
         [this](const BaseControlVariables&, const std::string&) {
            ShowHelp();
         });

   }

   // -----------------------------------------------------------------------
   // Run mode enum
   // -----------------------------------------------------------------------
   // COMPOSED searches the same two matrix sets as ALL (det=1 unimodular,
   // det=2..6 supercell) but combines them by composition (u * s, for every
   // u in the unimodular set and s in the supercell set) rather than
   // concatenation. This is a different search space, not a modifier on
   // ALL -- hence its own RunMode value rather than an independent flag.
   enum class RunMode { ALL, SUPER, EQUIVALENT, COMPOSED };

   // -----------------------------------------------------------------------
   // Display mode enum
   // -----------------------------------------------------------------------
   enum class DisplayMode { OLD, NEW, BOTH };

   // -----------------------------------------------------------------------
   // Accessors
   // -----------------------------------------------------------------------
   bool shouldShowDetails()        const { return m_showDetails; }
   void setShowDetails(bool b) { m_showDetails = b; }

   // m_runTests is set by the TEST handler; an empty m_testNumbers list means run all.
   bool shouldRunTests()           const { return m_runTests; }
   const std::vector<int>& getTestNumbers() const { return m_testNumbers; }
   void addTestNumber(int n) { m_runTests = true; if (n > 0) m_testNumbers.push_back(n); }

   double getNiggliDelta()         const { return m_niggliDelta; }
   void   setNiggliDelta(double d) { m_niggliDelta = d; }

   //bool shouldOutputCsv()          const { return m_csvOutput; }
   //void setCsvOutput(bool b) { m_csvOutput = b; }

   int  getMatrixOrder()           const { return m_unimodularOrder; }
   void setMatrixOrder(int n) { m_unimodularOrder = n; }

   void setUseSellingReduction(bool b) { m_useSellingReduction = b; }
   bool getUseSellingReduction()   const { return m_useSellingReduction; }

   bool    useHNF()                const { return m_useHNF; }
   RunMode getRunMode()            const { return m_runMode; }
   bool    runModeSuper()          const { return m_runMode == RunMode::SUPER; }
   bool    runModeEquivalent()     const { return m_runMode == RunMode::EQUIVALENT; }
   bool    runModeComposed()       const { return m_runMode == RunMode::COMPOSED; }
   std::string getModeString()     const {
      if (m_runMode == RunMode::SUPER)      return "SUPER";
      if (m_runMode == RunMode::EQUIVALENT) return "EQUIVALENT";
      if (m_runMode == RunMode::COMPOSED)   return "COMPOSED";
      return "ALL";
   }

   DisplayMode getDisplayMode()    const { return m_displayMode; }
   bool    showOldDisplay()        const { return m_displayMode == DisplayMode::OLD || m_displayMode == DisplayMode::BOTH; }
   bool    showNewDisplay()        const { return m_displayMode == DisplayMode::NEW || m_displayMode == DisplayMode::BOTH; }
   std::string getDisplayModeString() const {
      if (m_displayMode == DisplayMode::NEW)  return "NEW";
      if (m_displayMode == DisplayMode::BOTH) return "BOTH";
      return "OLD";
   }

   double getConventionalThreshold()       const { return m_conventionalThreshold; }
   void   setConventionalThreshold(double d) { m_conventionalThreshold = d; }

   double getRatioTolerance()               const { return m_ratioTolerance; }
   void   setRatioTolerance(double d) { m_ratioTolerance = d; }

private:
   bool    m_showDetails = false;
   double  m_niggliDelta = 1.0e-5;
   bool    m_runTests = false;   // set true only when TEST keyword is seen
   std::vector<int> m_testNumbers;  // empty = run all tests; otherwise run these specific tests
   //bool    m_csvOutput = false;
   int     m_unimodularOrder = 1;
   bool    m_useSellingReduction = false;
   RunMode m_runMode = RunMode::ALL;
   bool    m_useHNF = false;
   DisplayMode m_displayMode = DisplayMode::NEW;
   double  m_conventionalThreshold = 2.0;  // percent of referenceP3Norm, matches GOOD tier
   double  m_ratioTolerance = 0.20;  // confidence bar in [0,0.5] for including a 2nd-nearest determinant (see RATIOTOLERANCE)

};

#endif // MULTITRANSFORMFINDERCONTROLS_H
