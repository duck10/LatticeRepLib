#include "DistanceFactory.h"
#include "Distance.h"
#include <memory>

namespace DistanceFactory {
   std::vector<std::unique_ptr<Distance>> createEnabledDistances(const DistanceTypes& types) {
      std::vector<std::unique_ptr<Distance>> distances;

      if (types.isTypeEnabled("CS")) {
         distances.push_back(std::make_unique<CS6Distance>());
      }
      if (types.isTypeEnabled("NC")) {
         distances.push_back(std::make_unique<NCDistance>());
      }
      if (types.isTypeEnabled("V7")) {
         distances.push_back(std::make_unique<V7Distance>());
      }
      if (types.isTypeEnabled("RI")) {
         distances.push_back(std::make_unique<RIDistance>());
      }
      if (types.isTypeEnabled("U") || types.isTypeEnabled("DC7U")) {
         distances.push_back(std::make_unique<DC7uDistance>());
      }
      if (types.isTypeEnabled("P3") || types.isTypeEnabled("P3")) {
         distances.push_back(std::make_unique<P3Distance>());
      }

      return distances;
   }
}