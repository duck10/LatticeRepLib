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
#define DEFAULT_A 100.0
#define DEFAULT_B 100.0
#define DEFAULT_C 100.0
#define DEFAULT_ALPHA 90.0
#define DEFAULT_BETA 90.0
#define DEFAULT_GAMMA 90.0
#define DEFAULT_RANGEA 1.0
#define DEFAULT_RANGEB 1.0
#define DEFAULT_RANGEC 1.0
#define DEFAULT_RANGEALPHA 1.0
#define DEFAULT_RANGEBETA 1.0
#define DEFAULT_RANGEGAMMA 1.0
#define DEFAULT_SPACE "DC7UNSRT"
#define DEFAULT_SAUCMETRIC 7
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
    
};


#endif // CMD_SAUC_CONTROLS_H
