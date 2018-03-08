#ifndef PROGRESSSTEP_H
#define PROGRESSSTEP_H


#include <vector>

#include "ProgressLine.h"
#include "ProgressPoint.h"

class ProgressStep {
public:
   ProgressStep();

   template<typename T>
   void AddType() {
      m_step.push_back(T());
   }

private:
   std::vector<ProgressLine> m_step;
   bool m_validToAddMoreLatticeTypes;
};
#endif  //  PROGRESSSTEP_H

