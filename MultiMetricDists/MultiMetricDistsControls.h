#ifndef CMD_MULTI_METRIC_DISTS_CONTROLS_H
#define CMD_MULTI_METRIC_DISTS_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class MultiMetricDistsControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const MultiMetricDistsControls& ctrl) {
      os << "; CmdMultiMetricDistsControls\n";
      os << "; webRun " << ((ctrl.getHasWebInput()) ? "true" : "false") << std::endl;
      os << "; echo " << ((ctrl.getEcho()) ? "true" : "false") << std::endl;
      return os;
   }
};

#endif // CMD_MULTI_METRIC_DISTS_CONTROLS_H
