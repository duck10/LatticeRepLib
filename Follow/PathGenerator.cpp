#include "PathGenerator.h"
#include "S6.h"
#include "G6.h"

template<typename T>
std::vector<T> PathGenerator::generatePath(const T& start, const T& end, int steps) {
   std::vector<T> path;
   path.reserve(steps);

   for (int i = 0; i < steps; ++i) {
      double t = static_cast<double>(i) / (steps - 1);
      path.push_back(interpolate(start, end, t));
   }

   return path;
}

template<typename T>
T PathGenerator::interpolate(const T& start, const T& end, double t) {
   T result;
   for (size_t i = 0; i < start.size(); ++i) {
      result[i] = start[i] + t * (end[i] - start[i]);
   }
   return result;
}
// Explicit instantiation for the types you'll use
template std::vector<S6> PathGenerator::generatePath<S6>(const S6&, const S6&, int);
template std::vector<G6> PathGenerator::generatePath<G6>(const G6&, const G6&, int);
