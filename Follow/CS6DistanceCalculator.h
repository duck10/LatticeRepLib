#ifndef CS6DISTANCECALCULATOR_H
#define CS6DISTANCECALCULATOR_H

#include "DistanceCalculator.h"
#include "CS6Dist.h"
#include "CS6Dist.cpp"

class CS6DistanceCalculator : public DistanceCalculator {
public:
   double calculateDistance(const S6& cell1, const S6& cell2) const override {
      return CS6Dist(cell1, cell2);
   }

   std::string getName() const override {
      return "CS6 Distance";
   }

   std::string getColor() const override {
      return "#FF0000";  // Red
   }
};

#endif // CS6DISTANCECALCULATOR_H
