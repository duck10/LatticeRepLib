#ifndef THETA_INSENSITIVE_H
#define THETA_INSENSITIVE_H

#include "Theta.h"
#include <algorithm>
#include <string>

// ------------------------------------------------------------------
//  One global instance with sensible default weights
// ------------------------------------------------------------------
static ThetaMatch<std::string> gTheta;

// ------------------------------------------------------------------
//  Case-insensitive wrapper
// ------------------------------------------------------------------
inline std::string toLower(std::string s)
{
   std::transform(s.begin(), s.end(), s.begin(), ::tolower);
   return s;
}

inline double thetaDistance(const std::string& a,
   const std::string& b,
   ThetaMatch<std::string>& theta = gTheta)
{
   return theta(toLower(a), toLower(b));
}

// ------------------------------------------------------------------
//  Simple yes/no match with a user threshold
// ------------------------------------------------------------------
inline bool isClose(const std::string& a,
   const std::string& b,
   double threshold = 0.20,
   ThetaMatch<std::string>& theta = gTheta)
{
   return thetaDistance(a, b, theta) <= threshold;
}

#endif // THETA_INSENSITIVE_H
