#pragma warning(disable: 4101) // Visual Studio 

#ifndef MULTITRANSFORMFINDERCONTROLS_H
#define MULTITRANSFORMFINDERCONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"
#include <iostream>
#include <string>

/*
definitions on 2025-05-25
# Complete Control Commands Reference

## Matrix Generation Controls

### MAXMATRICES [integer]
**Purpose:** Maximum number of transformation matrices to generate
**Default:** 2000
**Range:** 100-10000
**Example:** `MAXMATRICES 1500`
**What it controls:** Total matrix count limit to prevent excessive computation

### COMBINATIONDEPTH [1-4]
**Purpose:** How many levels of matrix combinations to generate
**Default:** 4
**Values:**
- **1:** Basic matrices only (~23 matrices)
- **2:** + Permutation×Shear combinations (~200 matrices)
- **3:** + Shear×Shear combinations (~1000 matrices)
- **4:** + Triple combinations (~2000+ matrices)
**Example:** `COMBINATIONDEPTH 3`
**What it controls:** Complexity/thoroughness of matrix search

### MAXCOEFFICIENT [integer]
**Purpose:** Largest integer value allowed in generated matrix elements
**Default:** 2
**Range:** 1-5
**Example:** `MAXCOEFFICIENT 3`
**What it controls:** Range of matrix element values (-3 to +3 for coefficient=3)

## Display Controls

### MAXTRANSFORMS [integer]
**Purpose:** Number of best transformations to display in results
**Default:** 3
**Range:** 1-20
**Example:** `MAXTRANSFORMS 5`
**What it controls:** How many solutions are shown in output

### DETAILS [true/false]
**Purpose:** Show detailed debugging information during search
**Default:** false
**Example:** `DETAILS true` or just `DETAILS`
**What it controls:** Verbose output including matrix generation details

## Quality Controls

### MAXDISTANCE [decimal]
**Purpose:** Maximum allowed P3 distance for valid transformations
**Default:** 100.0
**Example:** `MAXDISTANCE 50.0`
**What it controls:** Filter out poor-quality transformations

### DISTANCETHRESHOLD [decimal]
**Purpose:** Threshold for considering transformations "good enough"
**Default:** 50.0
**Example:** `DISTANCETHRESHOLD 25.0`
**What it controls:** Quality cutoff for transformation acceptance

### SORTBY [P3|S6]
**Purpose:** Metric used to rank transformation quality
**Default:** P3
**Example:** `SORTBY S6`
**What it controls:** Whether to sort by P3 distance or S6 angle

## Niggli Reduction Controls

### USENIGGLI [true/false]
**Purpose:** Enable/disable Niggli reduction of cells before comparison
**Default:** true
**Example:** `USENIGGLI false`
**What it controls:** Whether cells are normalized before transformation search

### NIGGLIDELTA [decimal]
**Purpose:** Tolerance for Niggli reduction convergence
**Default:** 1e-05
**Example:** `NIGGLIDELTA 1e-06`
**What it controls:** Precision of Niggli reduction algorithm

### NIGGLITOLERANCE [decimal]
**Purpose:** Alternative name for NIGGLIDELTA
**Example:** `NIGGLITOLERANCE 1e-06`

## Advanced Controls

### INCLUDEIDENTITY [true/false]
**Purpose:** Whether to include identity matrix in search
**Default:** true
**Example:** `INCLUDEIDENTITY false`
**What it controls:** Include/exclude trivial identity transformation

### TEST [integer]
**Purpose:** Run built-in test cases
**Default:** 1000
**Example:** `TEST 500`
**What it controls:** Number of test cases to run (if testing mode enabled)

## Common Usage Patterns

### Fast Search (Small Problems)
```
MAXMATRICES 500
COMBINATIONDEPTH 2
MAXCOEFFICIENT 2
MAXTRANSFORMS 3
```

### Thorough Search (Challenging Cases)
```
MAXMATRICES 3000
COMBINATIONDEPTH 4
MAXCOEFFICIENT 3
MAXTRANSFORMS 5
DETAILS true
```

### Debug Mode
```
DETAILS true
SHOWTRANSFORMINFO true
MAXTRANSFORMS 10
USENIGGLI false
```

### Conservative Search (Fast)
```
COMBINATIONDEPTH 2
MAXCOEFFICIENT 1
MAXMATRICES 200
```
*/

class MultiTransformFinderControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const MultiTransformFinderControls& mtfc) {
      os << "; MultiTransformFinderControls\n";
      os << "; show details " << (mtfc.m_showDetails ? "true" : "false") << std::endl;
      os << "; show extra transform info " << (mtfc.m_showExtraTransformInfo ? "true" : "false") << std::endl;
      os << "; max matrix count " << mtfc.m_maxMatrixCount << std::endl;
      os << "; matrix combination depth " << mtfc.m_matrixCombinationDepth << std::endl;
      os << "; max matrix coefficient " << mtfc.m_maxMatrixCoefficient << std::endl;
      os << "; max transformations to show " << mtfc.m_maxTransformationsToShow << std::endl;
      os << "; max transformation distance " << mtfc.m_maxTransformationDistance << std::endl;
      os << "; success threshold " << mtfc.m_successThreshold << std::endl;
      os << "; include identity matrix " << (mtfc.m_includeIdentityMatrix ? "true" : "false") << std::endl;
      os << "; use niggli reduction " << (mtfc.m_useNiggliReduction ? "true" : "false") << std::endl;
      os << "; niggli delta " << mtfc.m_niggliDelta << std::endl;
      os << "; comparison mode for multi input " << mtfc.m_comparisonMode << std::endl;

      os << "; test strategy " << mtfc.m_testStrategy << std::endl;
      os << "; test all strategies " << (mtfc.m_testAll ? "true" : "false") << std::endl;
      os << "; test guided methods " << (mtfc.m_testGuided ? "true" : "false") << std::endl;
      os << "; test bidirectional " << (mtfc.m_testBidirectional ? "true" : "false") << std::endl;
      os << "; test multistage " << (mtfc.m_testMultiStage ? "true" : "false") << std::endl;
      os << "; test aggressive " << (mtfc.m_testAggressive ? "true" : "false") << std::endl;
      os << "; cloud radius " << mtfc.m_cloudRadius << std::endl;
      os << "; cloud steps " << mtfc.m_cloudSteps << std::endl;
      os << "; guidance type " << mtfc.m_guidanceType << std::endl;
      os << "; restart attempts " << mtfc.m_restartAttempts << std::endl;
      os << "; max iterations " << mtfc.m_maxIterations << std::endl;
      os << "; early stop " << (mtfc.m_earlyStop ? "true" : "false") << std::endl;
      os << "; compare strategies " << (mtfc.m_compareStrategies ? "true" : "false") << std::endl;
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

      InputHandler::registerHandler("SHOWTRANSFORMINFO", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            setShowExtraTransformInfo(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      // NEW CLEAR PARAMETER HANDLERS
      InputHandler::registerHandler("MAXMATRICES", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            try {
               setMaxMatrixCount(std::stoi(value));
            }
            catch (const std::exception& e) {
               std::cout << "Warning: max matrices: " << value << ". Using default." << std::endl;
            }
         });
      InputHandler::registerHandler("COMBINATIONDEPTH", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            setMatrixCombinationDepth(std::stoi(value));
         });

      InputHandler::registerHandler("MAXTRANSFORMS", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            setMaxTransformationsToShow(std::stoi(value));
         });

      InputHandler::registerHandler("MAXDISTANCE", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            setMaxTransformationDistance(std::stod(value));
         });

      InputHandler::registerHandler("INCLUDEIDENTITY", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            setIncludeIdentityMatrix(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("SORTBY", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            std::string upperValue = LRL_StringTools::strToupper(value);
            if (upperValue == "S6" || upperValue == "P3") {
               setSortBy(upperValue);
            }
            else {
               std::cout << "Warning: Unknown sort option: " << value << ". Using default (P3)." << std::endl;
               setSortBy("P3");
            }
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
      InputHandler::registerHandler("MAXCOEFFICIENT", 0.44,
         [this](const BaseControlVariables&, const std::string& value) {
            try {
               setMaxMatrixCoefficient(std::stoi(value));
            }
            catch (const std::exception& e) {
               std::cout << "Warning: Invalid coefficient value: " << value << ". Using default." << std::endl;
            }
         });

      // Also add an alias
      InputHandler::registerHandler("COEFFICIENT", 0.44,
         [this](const BaseControlVariables&, const std::string& value) {
            try {
               setMaxMatrixCoefficient(std::stoi(value));
            }
            catch (const std::exception& e) {
               std::cout << "Warning: Invalid coefficient value: " << value << ". Using default." << std::endl;
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

      // Strategy Selection Controls
      InputHandler::registerHandler("TESTSTRATEGY", 0.50,
         [this](const BaseControlVariables&, const std::string& value) {
            std::string upperValue = LRL_StringTools::strToupper(value);
            if (upperValue == "ALL" || upperValue == "DIRECT" || upperValue == "STANDARD" ||
               upperValue == "NIGGLI" || upperValue == "GUIDED" || upperValue == "BIDIRECTIONAL" ||
               upperValue == "MULTISTAGE" || upperValue == "AGGRESSIVE") {
               setTestStrategy(upperValue);
            }
            else {
               std::cout << "Warning: Unknown test strategy: " << value << ". Using default (ALL)." << std::endl;
               setTestStrategy("ALL");
            }
         });

      InputHandler::registerHandler("TESTALL", 0.51,
         [this](const BaseControlVariables&, const std::string& value) {
            setTestAll(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("TESTGUIDED", 0.52,
         [this](const BaseControlVariables&, const std::string& value) {
            setTestGuided(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("TESTBIDIRECTIONAL", 0.53,
         [this](const BaseControlVariables&, const std::string& value) {
            setTestBidirectional(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("TESTMULTISTAGE", 0.54,
         [this](const BaseControlVariables&, const std::string& value) {
            setTestMultiStage(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("TESTAGGRESSIVE", 0.55,
         [this](const BaseControlVariables&, const std::string& value) {
            setTestAggressive(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      // Guided Cloud Method Controls
      InputHandler::registerHandler("CLOUDRADIUS", 0.56,
         [this](const BaseControlVariables&, const std::string& value) {
            try {
               setCloudRadius(std::stod(value));
            }
            catch (const std::exception& e) {
               std::cout << "Warning: Invalid cloud radius: " << value << ". Using default." << std::endl;
            }
         });

      InputHandler::registerHandler("CLOUDSTEPS", 0.57,
         [this](const BaseControlVariables&, const std::string& value) {
            try {
               setCloudSteps(std::stoi(value));
            }
            catch (const std::exception& e) {
               std::cout << "Warning: Invalid cloud steps: " << value << ". Using default." << std::endl;
            }
         });

      InputHandler::registerHandler("GUIDANCETYPE", 0.58,
         [this](const BaseControlVariables&, const std::string& value) {
            std::string upperValue = LRL_StringTools::strToupper(value);
            if (upperValue == "GEOMETRIC" || upperValue == "DISTANCE" || upperValue == "HYBRID") {
               setGuidanceType(upperValue);
            }
            else {
               std::cout << "Warning: Unknown guidance type: " << value << ". Using default (GEOMETRIC)." << std::endl;
               setGuidanceType("GEOMETRIC");
            }
         });

      InputHandler::registerHandler("RESTARTATTEMPTS", 0.59,
         [this](const BaseControlVariables&, const std::string& value) {
            try {
               setRestartAttempts(std::stoi(value));
            }
            catch (const std::exception& e) {
               std::cout << "Warning: Invalid restart attempts: " << value << ". Using default." << std::endl;
            }
         });

      // Advanced Testing Controls
      InputHandler::registerHandler("MAXITERATIONS", 0.60,
         [this](const BaseControlVariables&, const std::string& value) {
            try {
               setMaxIterations(std::stoi(value));
            }
            catch (const std::exception& e) {
               std::cout << "Warning: Invalid max iterations: " << value << ". Using default." << std::endl;
            }
         });

      InputHandler::registerHandler("EARLYSTOP", 0.61,
         [this](const BaseControlVariables&, const std::string& value) {
            setEarlyStop(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("COMPARESTRATEGIES", 0.62,
         [this](const BaseControlVariables&, const std::string& value) {
            setCompareStrategies(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      // === THREE THRESHOLD REGISTRATION ===
      InputHandler::registerHandler("SUCCESSTHRESHOLD", 0.1,
         [this](const BaseControlVariables&, const std::string& value) {
            setSuccessThreshold(std::stod(value));
         });

      InputHandler::registerHandler("CONVERGENCETHRESHOLD", 1e-6,
         [this](const BaseControlVariables&, const std::string& value) {
            setConvergenceThreshold(std::stod(value));
         });

      InputHandler::registerHandler("IMPROVEMENTTHRESHOLD", 1e-8,
         [this](const BaseControlVariables&, const std::string& value) {
            setImprovementThreshold(std::stod(value));
         });

      // === THRESHOLD REGISTRATIONS ===
      InputHandler::registerHandler("SUCCESSTHRESHOLD", 0.1,
         [this](const BaseControlVariables&, const std::string& value) {
            setSuccessThreshold(std::stod(value));
         });

      InputHandler::registerHandler("CONVERGENCETHRESHOLD", 1e-6,
         [this](const BaseControlVariables&, const std::string& value) {
            setConvergenceThreshold(std::stod(value));
         });

      InputHandler::registerHandler("IMPROVEMENTTHRESHOLD", 1e-8,
         [this](const BaseControlVariables&, const std::string& value) {
            setImprovementThreshold(std::stod(value));
         });

      // === ALGORITHM CONTROL REGISTRATIONS ===
      InputHandler::registerHandler("SHOWPROGRESS", false,
         [this](const BaseControlVariables&, const std::string& value) {
            setShowProgress(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("INCLUDESTEPHISTORY", false,
         [this](const BaseControlVariables&, const std::string& value) {
            setIncludeStepHistory(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("MAXITERATIONS", 1000,
         [this](const BaseControlVariables&, const std::string& value) {
            setMaxIterations(std::stoi(value));
         });

      InputHandler::registerHandler("SHOWDETAILEDSTEPS", false,
         [this](const BaseControlVariables&, const std::string& value) {
            setShowDetailedSteps(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });
   }

   bool shouldShowDetails() const { return m_showDetails; }
   bool shouldShowExtraTransformInfo() const { return m_showExtraTransformInfo; }
   int getMaxMatrixCount() const { return m_maxMatrixCount; }
   int getMatrixCombinationDepth() const { return m_matrixCombinationDepth; }
   int getMaxMatrixCoefficient() const { return m_maxMatrixCoefficient; }
   int getMaxTransformationsToShow() const { return m_maxTransformationsToShow; }
   double getMaxTransformationDistance() const { return m_maxTransformationDistance; }
   bool shouldIncludeIdentityMatrix() const { return m_includeIdentityMatrix; }
   void setShowDetails(const bool b) { m_showDetails = b; }
   void setShowExtraTransformInfo(const bool b) { m_showExtraTransformInfo = b; }
   void setMaxMatrixCount(const int count) { m_maxMatrixCount = count; }
   void setMatrixCombinationDepth(const int depth) { m_matrixCombinationDepth = depth; }
   void setMaxMatrixCoefficient(const int coefficient) { m_maxMatrixCoefficient = coefficient; }
   void setMaxTransformationsToShow(const int count) { m_maxTransformationsToShow = count; }
   void setMaxTransformationDistance(const double distance) { m_maxTransformationDistance = distance; }
   void setIncludeIdentityMatrix(const bool b) { m_includeIdentityMatrix = b; }
   void setTestNumber(const int n) { m_testNumber = n; }

   bool shouldSortByP3() const { return m_sortBy == "P3"; }
   std::string getSortBy() const { return m_sortBy; }
   void setSortBy(const std::string& sortBy) { m_sortBy = sortBy; }

   double getNiggliDelta() const { return m_niggliDelta; }
   void setNiggliDelta(double delta) { m_niggliDelta = delta; }

   bool shouldUseNiggliReduction() const { return m_useNiggliReduction; }
   void setUseNiggliReduction(bool use) { m_useNiggliReduction = use; }

   bool shouldRunTests() const { return m_testNumber != 0; }
   bool shouldRunComparisonMode() const { return m_comparisonMode; }
   void setComparisonMode(bool mode) { m_comparisonMode = mode; }
   bool shouldOutputCsv() const { return m_csvOutput; }
   void setCsvOutput(bool csv) { m_csvOutput = csv; }

   bool shouldShowControls() const { return this->showControls; }


   // Strategy Selection
   std::string getTestStrategy() const { return m_testStrategy; }
   void setTestStrategy(const std::string& strategy) { m_testStrategy = strategy; }
   bool shouldTestAll() const { return m_testAll; }
   void setTestAll(bool test) { m_testAll = test; }
   bool shouldTestGuided() const { return m_testGuided; }
   void setTestGuided(bool test) { m_testGuided = test; }
   bool shouldTestBidirectional() const { return m_testBidirectional; }
   void setTestBidirectional(bool test) { m_testBidirectional = test; }
   bool shouldTestMultiStage() const { return m_testMultiStage; }
   void setTestMultiStage(bool test) { m_testMultiStage = test; }
   bool shouldTestAggressive() const { return m_testAggressive; }
   void setTestAggressive(bool test) { m_testAggressive = test; }

   // Guided Cloud Parameters
   double getCloudRadius() const { return m_cloudRadius; }
   void setCloudRadius(double radius) { m_cloudRadius = radius; }
   int getCloudSteps() const { return m_cloudSteps; }
   void setCloudSteps(int steps) { m_cloudSteps = steps; }
   std::string getGuidanceType() const { return m_guidanceType; }
   void setGuidanceType(const std::string& type) { m_guidanceType = type; }
   int getRestartAttempts() const { return m_restartAttempts; }
   void setRestartAttempts(int attempts) { m_restartAttempts = attempts; }

   // Advanced Testing
   int getMaxIterations() const { return m_maxIterations; }
   void setMaxIterations(int iterations) { m_maxIterations = iterations; }
   bool shouldEarlyStop() const { return m_earlyStop; }
   void setEarlyStop(bool stop) { m_earlyStop = stop; }
   bool shouldCompareStrategies() const { return m_compareStrategies; }
   void setCompareStrategies(bool compare) { m_compareStrategies = compare; }

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

   // === IMPROVEMENT THRESHOLD ===
   // Used for internal algorithm stopping criteria
   // Default: 1e-8 (when algorithm stops improving)
   void setImprovementThreshold(double threshold) {
      m_improvementThreshold = threshold;
   }
   double getImprovementThreshold() const {
      return m_improvementThreshold;
   }

   public:



      // === ALGORITHM CONTROL SETTINGS ===

      // Show progress messages during matching
      void setShowProgress(bool value) {
         m_showProgress = value;
      }
      bool showProgress() const {
         return m_showProgress;
      }

      // Include detailed step history in results
      void setIncludeStepHistory(bool value) {
         m_includeStepHistory = value;
      }
      bool includeStepHistory() const {
         return m_includeStepHistory;
      }

      // Show detailed step information
      void setShowDetailedSteps(bool value) {
         m_showDetailedSteps = value;
      }
      bool showDetailedSteps() const {
         return m_showDetailedSteps;
      }

private:
   // NEW CLEAR PARAMETER NAMES
   bool m_showDetails = false;
   bool m_showExtraTransformInfo = false;
   int m_maxMatrixCount = 2000;              // Was m_maxSearchLimit (default 1000 → 2000)
   int m_matrixCombinationDepth = 4;         // Was m_maxMatrixSearchDepth (default 3000 → 4!)
   int m_maxMatrixCoefficient = 2;           // Keep as-is
   int m_maxTransformationsToShow = 3;
   double m_maxTransformationDistance = 100.0;
   bool m_includeIdentityMatrix = true;
   // Niggli reduction parameters
   double m_niggliDelta = 1.0e-5;
   bool m_useNiggliReduction = true;
   // Other parameters
   std::string m_sortBy = "P3";
   int m_testNumber = 1000;
   bool m_comparisonMode = false;
   bool m_csvOutput = false;

   // Test Strategy Controls
   std::string m_testStrategy = "ALL";
   bool m_testAll = true;
   bool m_testGuided = true;
   bool m_testBidirectional = true;
   bool m_testMultiStage = true;
   bool m_testAggressive = true;

   // Guided Cloud Method Parameters
   double m_cloudRadius = 2.0;
   int m_cloudSteps = 100;
   std::string m_guidanceType = "GEOMETRIC";
   int m_restartAttempts = 5;

   // Advanced Testing Parameters
   int m_maxIterations = 1000;
   bool m_earlyStop = false;
   bool m_compareStrategies = true;

   // === THREE THRESHOLD SYSTEM ===
   double m_successThreshold = 0.1;       // For determining if match is successful
   double m_convergenceThreshold = 1.e-6;   // For bidirectional algorithm convergence  
   double m_improvementThreshold = 1.e-6;   // For internal algorithm stopping criteria

   // === ALGORITHM CONTROL VARIABLES ===
   // === THRESHOLDS ===
   bool m_showProgress = false;            // Display progress messages
   bool m_includeStepHistory = false;      // Record detailed step history
   bool m_showDetailedSteps = false;       // Show detailed step information
};

#endif // MULTITRANSFORMFINDERCONTROLS_H

