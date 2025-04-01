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

// Helper function declaration
bool caseInsensitiveCompare(const std::string& str1, const std::string& str2);

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

void setCellEdgeA(const double);
void setCellEdgeB(const double);
void setCellEdgeC(const double);
void setCellAngleAlpha(const double);
void setCellAngleBeta(const double);
void setCellAngleGamma(const double);
void setBlockSize(const unsigned long);
void setBlockStart(const unsigned long);
void setSpace(std::string);
void setCellRangeA(const double);
void setCellRangeB(const double);
void setCellRangeC(const double);
void setCellRangeAlpha(const double);
void setCellRangeBeta(const double);
void setCellRangeGamma(const double);
void setCellRangeSphere(const double);
void setSaucMetric(const double);

class CmdSaucSearchOptions {
public:
   size_t blocksize = 0;      ;
   size_t blockstart = 0;
   double A = DEFAULT_A;
   double B = DEFAULT_B;
   double C = DEFAULT_C;
   double Alpha = DEFAULT_ALPHA;
   double Beta = DEFAULT_BETA;
   double Gamma = DEFAULT_GAMMA;

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

   CmdSaucSearchOptions() = default;
};

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
    
};


#endif // CMD_SAUC_CONTROLS_H
