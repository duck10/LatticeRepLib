#include "CmdSaucControls.h"

// External CmdSaucSearchOptions instance
extern CmdSaucSearchOptions __CSSO__;

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
