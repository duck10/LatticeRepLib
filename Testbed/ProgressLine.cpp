
#include "ProgressLine.h"
#include "ProgressPoint.h"
#include "ProgressPointBase.h"
#include "ProgressStep.h"
#include "S6.h"

ProgressLine::ProgressLine() {
}

template<typename T>
void ProgressLine::AddStep(  ) {
   T point;
   point.m_name = T::GetName();
   m_line.push_back(point);
}