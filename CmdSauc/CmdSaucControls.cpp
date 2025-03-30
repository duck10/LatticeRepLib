// CmdSaucControls.cpp
#include "CmdSaucControls.h"  // Always include your own header first

class CmdSaucSearchOptions {
public:
    double blocksize, blockstart;
    double A, B, C, Alpha, Beta, Gamma;
    double RangeA, RangeB, RangeC, RangeAlpha, RangeBeta, RangeGamma;
    double saucSphereRange;
    int SAUCMETRIC;
    std::string SPACE;
    bool saucNearest;
    bool saucSphere;
    bool saucRange;

    CmdSaucSearchOptions(){
        A = DEFAULT_A;
        B = DEFAULT_B;
        C = DEFAULT_C;
        Alpha = DEFAULT_ALPHA;
        Beta = DEFAULT_BETA; 
        Gamma = DEFAULT_GAMMA; 
        RangeA = DEFAULT_RANGEA;
        RangeB = DEFAULT_RANGEB;
        RangeC = DEFAULT_RANGEC;
        RangeAlpha = DEFAULT_RANGEALPHA;
        RangeBeta = DEFAULT_RANGEBETA;
        RangeGamma = DEFAULT_RANGEGAMMA;
        saucSphereRange = 50.;
        SPACE = std::string(DEFAULT_SPACE);
        SAUCMETRIC = DEFAULT_SAUCMETRIC;
        saucNearest = false;
        saucSphere = true;
        saucRange = false;
    }
};

CmdSaucSearchOptions __CSSO__ = CmdSaucSearchOptions();

// Static member initialization
std::atomic<bool> CmdSaucControls::g_running(false);
std::atomic<bool> CmdSaucControls::g_in_input(false);

// Constructor
CmdSaucControls::CmdSaucControls() {
    InputHandler::registerHandler("BLOCKSIZE", 0.35,
                                  [this](BaseControlVariables&, const std::string& value) {
        setBlockSize(std::stoul(value));
    });

    InputHandler::registerHandler("BLOCKSTART", 0.35,
                                  [this](BaseControlVariables&, const std::string& value) {
        setBlockStart(std::stoul(value));
    });

    InputHandler::registerHandler("SHOW", .5,
                                  [this](BaseControlVariables&, const std::string& value) {
        showControls = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
    });

    InputHandler::registerHandler("SPACE", 0.2,
                                  [this](BaseControlVariables&, const std::string& value) {
        setSpace(value);
    });

    InputHandler::registerHandler("A", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        setCellEdgeA(std::stod(value));
    });

    InputHandler::registerHandler("B", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        setCellEdgeB(std::stod(value));
    });

    InputHandler::registerHandler("C", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        setCellEdgeC(std::stod(value));
    });

    InputHandler::registerHandler("ALPHA", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        setCellAngleAlpha(std::stod(value));
    });

    InputHandler::registerHandler("BETA", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        setCellAngleBeta(std::stod(value));
    });

    InputHandler::registerHandler("GAMMA", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        setCellAngleGamma(std::stod(value));
    });

    InputHandler::registerHandler("RANGEA", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        setCellRangeA(std::stod(value));
    });

    InputHandler::registerHandler("RANGEB", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        setCellRangeB(std::stod(value));
    });

    InputHandler::registerHandler("RANGEC", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        setCellRangeC(std::stod(value));
    });

    InputHandler::registerHandler("RANGEALPHA", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        setCellRangeAlpha(std::stod(value));
    });

    InputHandler::registerHandler("RANGEBETA", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        setCellRangeBeta(std::stod(value));
    });

    InputHandler::registerHandler("RANGEGAMMA", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        setCellRangeGamma(std::stod(value));
    });

    InputHandler::registerHandler("RANGESPHERE", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        setCellRangeSphere(std::stod(value));
    });

    InputHandler::registerHandler("NEAREST", 0.2,
                                  [this](BaseControlVariables&, const std::string& value) {
        __CSSO__.saucNearest = (value == "1"
                                || LRL_StringTools::strToupper(value) == "TRUE"
                                || value.empty());
        if (__CSSO__.saucNearest) {
            __CSSO__.saucSphere = !__CSSO__.saucNearest;
            __CSSO__.saucRange = !__CSSO__.saucNearest;
        }
    });

    InputHandler::registerHandler("SPHERE", 0.2,
                                  [this](BaseControlVariables&, const std::string& value) {
        __CSSO__.saucSphere = (value == "1"
                               || LRL_StringTools::strToupper(value) == "TRUE"
                               || value.empty());
        if (__CSSO__.saucSphere) {
            __CSSO__.saucNearest = !__CSSO__.saucSphere;
            __CSSO__.saucRange = !__CSSO__.saucSphere;
        }
    });

    InputHandler::registerHandler("RANGE", 0.2,
                                  [this](BaseControlVariables&, const std::string& value) {
        __CSSO__.saucRange = (value == "1"
                              || LRL_StringTools::strToupper(value) == "TRUE"
                              || value.empty());
        if (__CSSO__.saucRange) {
            __CSSO__.saucSphere = !__CSSO__.saucRange;
            __CSSO__.saucNearest = !__CSSO__.saucRange;
        }
    });

    InputHandler::registerHandler("SAUCMETRIC", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        setSaucMetric(std::stod(value));
    });

    if ((!__CSSO__.saucSphere) && (!__CSSO__.saucRange) && (!__CSSO__.saucNearest)) 
        __CSSO__.saucNearest = true;
}

// Accessor methods
size_t CmdSaucControls::getBlockStart() const { 
    return __CSSO__.blockstart; 
}

size_t CmdSaucControls::getBlockSize() const { 
    return __CSSO__.blocksize; 
}

std::string CmdSaucControls::getState() const {
    std::ostringstream oss;
    oss
    << ";Web run: " << ((this->getHasWebInput()) ? "true" : "false") << "\n"
    << ";Blockstart: " << __CSSO__.blockstart << "\n"
    << ";Blocksize: " << __CSSO__.blocksize << "\n"
    << ";Sauc Metric: " << __CSSO__.SAUCMETRIC << "\n";
    oss << "\n\n";
    return oss.str();
}

int CmdSaucControls::getsimilarity() const {
    bool saucNearest = __CSSO__.saucNearest;
    bool saucSphere  = __CSSO__.saucSphere;
    bool saucRange   = __CSSO__.saucRange;
    /* Similarity -- 1 through 3 for Nearest, Sphere, or Range */
    int similarity;
    if (saucNearest) {
        similarity = 1;
    } else if (saucSphere) {
        similarity = 2;
    } else {
        similarity = 3;
    }
    return similarity;
}

int CmdSaucControls::getalgorithm() const {
    return __CSSO__.SAUCMETRIC;
}

double CmdSaucControls::getRangeA() const {
    return __CSSO__.RangeA;
}

double CmdSaucControls::getRangeB() const {
    return __CSSO__.RangeB;
}

double CmdSaucControls::getRangeC() const {
    return __CSSO__.RangeC;
}

double CmdSaucControls::getRangeAlpha() const {
    return __CSSO__.RangeAlpha;
}

double CmdSaucControls::getRangeBeta() const {
    return __CSSO__.RangeBeta;
}

double CmdSaucControls::getRangeGamma() const {
    return __CSSO__.RangeGamma;
}

double CmdSaucControls::getsaucSphereRange() const {
    return __CSSO__.saucSphereRange;
}


// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const CmdSaucControls& ctrl) {
    os << ctrl.getState();
    return os;
}

void setCellEdgeA(const double value) {
    __CSSO__.A = value;
}

void setCellEdgeB(const double value) {
    __CSSO__.B = value;
}

void setCellEdgeC(const double value) {
    __CSSO__.C = value;
}

void setCellAngleAlpha(const double value) {
    __CSSO__.Alpha = value;
}

void setCellAngleBeta(const double value) {
    __CSSO__.Beta = value;
}

void setCellAngleGamma(const double value) {
    __CSSO__.Gamma = value;
}

void setBlockSize(const unsigned long value) {
    __CSSO__.blocksize = value;
}

void setBlockStart(const unsigned long value) {
    __CSSO__.blockstart = value;
}

void setSpace(std::string value) {
    __CSSO__.SPACE = value;
}

void setCellRangeA(const double value) {
    __CSSO__.RangeA = value;
}

void setCellRangeB(const double value) {
    __CSSO__.RangeB = value;
}

void setCellRangeC(const double value) {
    __CSSO__.RangeC = value;
}

void setCellRangeAlpha(const double value) {
    __CSSO__.RangeAlpha = value;
}

void setCellRangeBeta(const double value) {
    __CSSO__.RangeBeta = value;
}

void setCellRangeGamma(const double value) {
    __CSSO__.RangeGamma = value;
}

void setCellRangeSphere(const double value) {
    __CSSO__.saucSphereRange = value;
}

void setSaucMetric(const double value) {
    __CSSO__.SAUCMETRIC = (int)(floor(value+0.5));
}

