#ifndef CMD_SAUC_CONTROLS_H
#define CMD_SAUC_CONTROLS_H

#include "BaseControlVariables.h"
#include "BlockUtils.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"
#include "DistanceTypes.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cstddef>
#include <atomic>
#include <vector>

// Default constants
constexpr double DEFAULT_A = 100.0;
constexpr double DEFAULT_B = 100.0;
constexpr double DEFAULT_C = 100.0;
constexpr double DEFAULT_ALPHA = 90.0;
constexpr double DEFAULT_BETA = 90.0;
constexpr double DEFAULT_GAMMA = 90.0;
constexpr double DEFAULT_RANGEA = 1.0;
constexpr double DEFAULT_RANGEB = 1.0;
constexpr double DEFAULT_RANGEC = 1.0;
constexpr double DEFAULT_RANGEALPHA = 1.0;
constexpr double DEFAULT_RANGEBETA = 1.0;
constexpr double DEFAULT_RANGEGAMMA = 1.0;
const std::string DEFAULT_SPACE = "DC7UNSRT";
constexpr int DEFAULT_SAUCMETRIC = 7;
/*#define DEFAULT_BLOCKSIZE 100*/

// Forward declare class instead of defining it in the header
class CmdSaucSearchOptions;

// Define full CmdSaucControls class here
class CmdSaucControls : public BaseControlVariables {
public:
    friend std::ostream& operator<< (std::ostream& os, const CmdSaucControls& ctrl);
    
    static std::atomic<bool> g_running;
    static std::atomic<bool> g_in_input;
    
    CmdSaucControls();
    
    // Public accessors
    size_t getBlockStart() const;
    size_t getBlockSize() const;
    std::string getState() const;
    int getsimilarity() const;
    int getalgorithm() const;
    double getRangeA() const;
    double getRangeB() const;
    double getRangeC() const;
    double getRangeAlpha() const;
    double getRangeBeta() const;
    double getRangeGamma() const;
    double getsaucSphereRange() const;

private:
   std::string HandleSpace(const std::string& st) const {
      static const std::string validNumbers("1 2 3 4 5 6 7 ");
      static const std::vector<std::string>
         validNames({ "L1", "L2", "NCDist", "V7", "D7", "S6", "DC7UNSRT", "DC7U"});
      {
         if (validNumbers.find(st) != std::string::npos) {
            return validNames[std::stoi(st)];
         }
         else {
            for (const auto& name : validNames) {
               if (name == st) return st;
            }
            return "ERROR in SPACE ASSIGMENT";
         }
      }
   }

private:
   size_t blocksize = 0;
   size_t blockstart = 0;

   double RangeA = DEFAULT_RANGEA;
   double RangeB = DEFAULT_RANGEB;
   double RangeC = DEFAULT_GAMMA;
   double RangeAlpha = DEFAULT_RANGEALPHA;
   double RangeBeta = DEFAULT_RANGEBETA;
   double RangeGamma = DEFAULT_RANGEBETA;

   double saucSphereRange = 50.0;
   int SAUCMETRIC = DEFAULT_SAUCMETRIC;
   std::string SPACE = std::string(DEFAULT_SPACE);

   bool saucNearest = false;
   bool saucSphere = true;
   bool saucRange = false;
};


#endif // CMD_SAUC_CONTROLS_H
