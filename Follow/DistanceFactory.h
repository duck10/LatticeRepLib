#ifndef DISTANCE_FACTORY_H
#define DISTANCE_FACTORY_H

#include "Distance.h"
#include "DistanceTypes.h"

#include <memory>
#include <vector>

namespace DistanceFactory {
   std::vector<std::unique_ptr<Distance>> createEnabledDistances(const DistanceTypes& types);
}

#endif // DISTANCE_FACTORY_H