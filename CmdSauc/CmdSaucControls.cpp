// CmdSaucControls.cpp
#include "CmdSaucControls.h"  // Always include your own header first
#include "LRL_StringTools.h"

//class CmdSaucSearchOptions {
//public:
//
//    CmdSaucSearchOptions(){
//        A = DEFAULT_A;
//        B = DEFAULT_B;
//        C = DEFAULT_C;
//        Alpha = DEFAULT_ALPHA;
//        Beta = DEFAULT_BETA; 
//        Gamma = DEFAULT_GAMMA; 
//        RangeA = DEFAULT_RANGEA;
//        RangeB = DEFAULT_RANGEB;
//        RangeC = DEFAULT_RANGEC;
//        RangeAlpha = DEFAULT_RANGEALPHA;
//        RangeBeta = DEFAULT_RANGEBETA;
//        RangeGamma = DEFAULT_RANGEGAMMA;
//        saucSphereRange = 50.;
//        SPACE = std::string(DEFAULT_SPACE);
//        SAUCMETRIC = DEFAULT_SAUCMETRIC;
//        saucNearest = false;
//        saucSphere = true;
//        saucRange = false;
//    }
//};
//
//CmdSaucSearchOptions __CSSO__ = CmdSaucSearchOptions();

// Static member initialization
std::atomic<bool> CmdSaucControls::g_running(false);
std::atomic<bool> CmdSaucControls::g_in_input(false);

// Constructor
CmdSaucControls::CmdSaucControls() {
    InputHandler::registerHandler("BLOCKSIZE", 0.35,
                                  [this](BaseControlVariables&, const std::string& value) {
        blocksize = std::stoul(value);
    });

    InputHandler::registerHandler("BLOCKSTART", 0.35,
                                  [this](BaseControlVariables&, const std::string& value) {
        blockstart = std::stoul(value);
    });

    InputHandler::registerHandler("SPACE", 0.2,
                                  [this](BaseControlVariables&, const std::string& value) {
          SPACE = HandleSpace(LRL_StringTools::strToupper(value));
    });

    InputHandler::registerHandler("RANGEA", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        RangeA = std::stod(value);
    });

    InputHandler::registerHandler("RANGEB", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        RangeB = std::stod(value);
    });

    InputHandler::registerHandler("RANGEC", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        RangeC = std::stod(value);
    });

    InputHandler::registerHandler("RANGEALPHA", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        RangeAlpha = std::stod(value);
    });

    InputHandler::registerHandler("RANGEBETA", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        RangeBeta = std::stod(value);
    });

    InputHandler::registerHandler("RANGEGAMMA", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
        RangeGamma = std::stod(value);
    });

    InputHandler::registerHandler("RANGESPHERE", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
          saucSphereRange = std::stod(value);
    });

    InputHandler::registerHandler("NEAREST", 0.2,
                                  [this](BaseControlVariables&, const std::string& value) {
        saucNearest = (value == "1"
                                || LRL_StringTools::strToupper(value) == "TRUE"
                                || value.empty());
        if (saucNearest) {
            saucSphere = !saucNearest;
            saucRange = !saucNearest;
        }
    });

    InputHandler::registerHandler("SPHERE", 0.2,
                                  [this](BaseControlVariables&, const std::string& value) {
        saucSphere = (value == "1"
                               || LRL_StringTools::strToupper(value) == "TRUE"
                               || value.empty());
        if (saucSphere) {
            saucNearest = !saucSphere;
            saucRange = !saucSphere;
        }
    });

    InputHandler::registerHandler("RANGE", 0.2,
                                  [this](BaseControlVariables&, const std::string& value) {
        saucRange = (value == "1"
                              || LRL_StringTools::strToupper(value) == "TRUE"
                              || value.empty());
        if (saucRange) {
            saucSphere = !saucRange;
            saucNearest = !saucRange;
        }
    });

    InputHandler::registerHandler("SAUCMETRIC", 0.95,
                                  [this](BaseControlVariables&, const std::string& value) {
          saucSphereRange = std::stoi(value);
    });

    if ((!saucSphere) && (!saucRange) && (!saucNearest)) 
        saucNearest = true;
}

// Accessor methods
size_t CmdSaucControls::getBlockStart() const { 
    return blockstart; 
}

size_t CmdSaucControls::getBlockSize() const { 
    return blocksize; 
}

std::string CmdSaucControls::getState() const {
    std::ostringstream oss;
    oss
    << ";Web run: " << ((this->getHasWebInput()) ? "true" : "false") << "\n"
    << ";Blockstart: " << blockstart << "\n"
    << ";Blocksize: " << blocksize << "\n"
    << ";Sauc Metric: " << SAUCMETRIC << "\n";
    oss << "\n\n";
    return oss.str();
}

int CmdSaucControls::getsimilarity() const {
    /* Similarity -- 1 through 3 for Nearest, Sphere, or Range */
    if (saucNearest) {
        return 1;
    } else if (saucSphere) {
        return 2;
    } else {
        return 3;
    }
}

int CmdSaucControls::getalgorithm() const {
    return SAUCMETRIC;
}

double CmdSaucControls::getRangeA() const {
    return RangeA;
}

double CmdSaucControls::getRangeB() const {
    return RangeB;
}

double CmdSaucControls::getRangeC() const {
    return RangeC;
}

double CmdSaucControls::getRangeAlpha() const {
    return RangeAlpha;
}

double CmdSaucControls::getRangeBeta() const {
    return RangeBeta;
}

double CmdSaucControls::getRangeGamma() const {
    return RangeGamma;
}

double CmdSaucControls::getsaucSphereRange() const {
    return saucSphereRange;
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const CmdSaucControls& ctrl) {
    os << ctrl.getState();
    return os;
}
