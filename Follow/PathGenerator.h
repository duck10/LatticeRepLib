#ifndef PATH_GENERATOR_H
#define PATH_GENERATOR_H

#include "Cell.h"
#include "Distance.h"

#include <vector>

class PathGenerator {
public:
   template<typename T>
   static std::vector<T> generatePath(const T& start, const T& end, int steps);

private:
   template<typename T>
   static T interpolate(const T& start, const T& end, double t);
};

#endif // PATH_GENERATOR_H
