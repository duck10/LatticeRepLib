// PerturbCountControl.h
#ifndef PERTURB_COUNT_CONTROL_H
#define PERTURB_COUNT_CONTROL_H

#include "ControlFeature.h"
#include "BaseControlVariables.h"

// Control for number of perturbations
class PerturbCountControl : public ControlFeature {
private:
   size_t count{ 1 };  // Default to 1

public:
   void writeToStream(std::ostream& os) const override {
      os << ";Perturb Count: " << count << "\n";
   }

   void setCount(size_t n) { count = n; }
   size_t getCount() const { return count; }
};

// Control for perturbation amount
class PerturbAmountControl : public ControlFeature {
private:
   double perturbAmount{ 1.0 };  // Default to 1 part per thousand

public:
   void writeToStream(std::ostream& os) const override {
      os << ";Perturb Amount (PPT): " << perturbAmount << "\n";
   }

   void setAmount(double ppt) { perturbAmount = ppt; }
   double getAmount() const { return perturbAmount; }
};

class PerturbCountControlVariables : public BaseControlVariables {
public:
   PerturbCountControlVariables() {
      features.push_back(std::make_unique<PerturbCountControl>());
      features.push_back(std::make_unique<PerturbAmountControl>());
   }
};

#endif // PERTURB_COUNT_CONTROL_H
