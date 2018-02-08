#ifndef GENERATERANDOMLATTICE_H
#define GENERATERANDOMLATTICE_H

#include "LRL_Cell.h"
#include "rhrand.h"
#include "S6.h"

template<typename T = S6>
class GenerateRandomLattice {

public:
   GenerateRandomLattice(const int seed)
      : m_seed(seed)
      , m_perturb(1.0)
   {}

   T Generate(void) {
      S6 s6(S6::randDeloneReduced());

      const double choice = m_rhrand.urand();
      // the constants are chosen from results for 
      // random generation of valid cells !!!!!!!!!!!!!!!!
      // but they are somewhat random and can be adjusted.
      if (choice < 0.055) return T(s6);
      else if (choice < 0.68) return T(S6::RandomUnreduceOne(s6));
      else if (choice < 0.995) return T(S6::RandomUnreduceTwo(s6));
      else return T(S6::RandomUnreduceThree(s6));
   }

   T Generate(const double d) {
      return T(d * rand() / LRL_Cell::randomLatticeNormalizationConstantSquared);
   }

   T Perturb(const T& t) { return T(S6(t)*m_perturb); }

   void SetSeed(const int n) { m_seed = n; }
   void PerturbBy(const double perturb) { m_perturb = perturb; }

private:


   S6 S6_randDeloneReduced() {
      S6 s6;
      for (unsigned long i = 0; i < 6; ++i)
         s6[i] = -m_rhrand.urand() * LRL_Cell::randomLatticeNormalizationConstantSquared;
      s6.m_valid = true;
      return s6;
   }

   int m_seed;
   double m_perturb;
   RHrand m_rhrand;
};

#endif  //  GENERATERANDOMLATTICE_H