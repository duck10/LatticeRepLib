#ifndef CMDP3REDUCECONTROLS_H
#define CMDP3REDUCECONTROLS_H

#include <iostream>

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdP3ReduceControls : public BaseControlVariables {
public:
   CmdP3ReduceControls()  {}

   bool shouldShowControls() const { return m_showControls; }

   friend std::ostream& operator<<(std::ostream& os, const CmdP3ReduceControls& c) {
      os << "; CmdP3Reduce Controls\n";
      os << "; Show controls: " << c.shouldShowControls() << "\n";
      return os;
   }

private:
   bool m_showControls = false;
};

#endif  // CMDP3REDUCECONTROLS_H
