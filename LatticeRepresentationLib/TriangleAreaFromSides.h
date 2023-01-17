#ifndef TRIANGLEAREAFROMSIDES_H
#define TRIANGLEAREAFROMSIDES_H

#include <cmath>

static double TriangleAreaFromSides(const double a, const double b, const double c) {
   const double s = (a + b + c) / 2.0; // s is half the perimeter
   const double trialValue = s * (s - a) * (s - b) * (s - c); // Heron's formula
   const double sign = (trialValue >= 0.0) ? 1.0 : -1.0;
   return sign * std::sqrt(std::abs(trialValue));
}



#endif // TRIANGLEAREAFROMSIDES_H
