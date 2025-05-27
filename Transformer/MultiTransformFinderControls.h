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
      os << "; distance threshold " << mtfc.m_distanceThreshold << std::endl;
      os << "; include identity matrix " << (mtfc.m_includeIdentityMatrix ? "true" : "false") << std::endl;
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

      InputHandler::registerHandler("DISTANCETHRESHOLD", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            setDistanceThreshold(std::stod(value));
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

   }

   // NEW CLEAR GETTERS
   bool shouldShowDetails() const { return m_showDetails; }
   bool shouldShowExtraTransformInfo() const { return m_showExtraTransformInfo; }
   int getMaxMatrixCount() const { return m_maxMatrixCount; }
   int getMatrixCombinationDepth() const { return m_matrixCombinationDepth; }
   int getMaxMatrixCoefficient() const { return m_maxMatrixCoefficient; }
   int getMaxTransformationsToShow() const { return m_maxTransformationsToShow; }
   double getMaxTransformationDistance() const { return m_maxTransformationDistance; }
   double getDistanceThreshold() const { return m_distanceThreshold; }
   bool shouldIncludeIdentityMatrix() const { return m_includeIdentityMatrix; }

   // NEW CLEAR SETTERS
   void setShowDetails(const bool b) { m_showDetails = b; }
   void setShowExtraTransformInfo(const bool b) { m_showExtraTransformInfo = b; }
   void setMaxMatrixCount(const int count) { m_maxMatrixCount = count; }
   void setMatrixCombinationDepth(const int depth) { m_matrixCombinationDepth = depth; }
   void setMaxMatrixCoefficient(const int coefficient) { m_maxMatrixCoefficient = coefficient; }
   void setMaxTransformationsToShow(const int count) { m_maxTransformationsToShow = count; }
   void setMaxTransformationDistance(const double distance) { m_maxTransformationDistance = distance; }
   void setDistanceThreshold(const double threshold) { m_distanceThreshold = threshold; }
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

private:
   // NEW CLEAR PARAMETER NAMES
   bool m_showDetails = false;
   bool m_showExtraTransformInfo = false;
   int m_maxMatrixCount = 2000;              // Was m_maxSearchLimit (default 1000 → 2000)
   int m_matrixCombinationDepth = 4;         // Was m_maxMatrixSearchDepth (default 3000 → 4!)
   int m_maxMatrixCoefficient = 2;           // Keep as-is
   int m_maxTransformationsToShow = 3;
   double m_maxTransformationDistance = 100.0;
   double m_distanceThreshold = 50.0;
   bool m_includeIdentityMatrix = true;
   // Niggli reduction parameters
   double m_niggliDelta = 1.0e-5;
   bool m_useNiggliReduction = true;
   // Other parameters
   std::string m_sortBy = "P3";
   int m_testNumber = 1000;
};

#endif // MULTITRANSFORMFINDERCONTROLS_H

