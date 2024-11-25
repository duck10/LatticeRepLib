#include "FollowUtils.h"
#include <random>



G6 perturbVector(const G6& vector, double perturbBy) {
   static std::random_device rd;
   static std::mt19937 gen(rd());
   static std::normal_distribution<> d(0, perturbBy);

   G6 perturbedVector = vector;
   for (size_t i = 0; i < 6; ++i) {
      perturbedVector[i] += d(gen);
   }
   return perturbedVector;
}
