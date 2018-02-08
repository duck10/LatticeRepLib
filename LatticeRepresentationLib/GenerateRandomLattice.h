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
      S6 s6(randDeloneReduced());

      const double choice = m_rhrand.urand();
      // the constants are chosen from results for 
      // random generation of valid cells !!!!!!!!!!!!!!!!
      // but they are somewhat random and can be adjusted.
      if (choice < 0.055) return T(s6);
      else if (choice < 0.68) return T(RandomUnreduceOne(s6));
      else if (choice < 0.995) return T(RandomUnreduceTwo(s6));
      else return T(RandomUnreduceThree(s6));
   }

   S6 randDeloneReduced() {
      S6 s6;
      for (unsigned long i = 0; i < 6; ++i)
         s6[i] = -m_rhrand.urand() * LRL_Cell::randomLatticeNormalizationConstantSquared;
      s6.SetValid(true);
      return s6;
   }

   T Generate(const double d) {
      return T(d * rand() / LRL_Cell::randomLatticeNormalizationConstantSquared);
   }

   T Perturb(const T& t) { return T(S6(t)*m_perturb); }

   void SetSeed(const int n) { m_seed = n; }
   void PerturbBy(const double perturb) { m_perturb = perturb; }

private:

   unsigned long CountPositive(const S6& s6) {
      unsigned long sum = 0;
      for (unsigned long i = 0; i < 6; ++i) sum += (s6[i] > 0.0) ? 1 : 0;
      return sum;
   }

   S6 RandomUnreduceOne(const S6& s6) {
      const double choice = 12.0 * m_rhrand.urand();

      if (choice < 1) return S6::Unreduce11(s6);
      else if (choice < 2) return S6::Unreduce12(s6);
      else if (choice < 3) return S6::Unreduce21(s6);
      else if (choice < 4) return S6::Unreduce22(s6);
      else if (choice < 5) return S6::Unreduce31(s6);
      else if (choice < 6) return S6::Unreduce32(s6);
      else if (choice < 7) return S6::Unreduce41(s6);
      else if (choice < 8) return S6::Unreduce42(s6);
      else if (choice < 9) return S6::Unreduce51(s6);
      else if (choice < 10) return S6::Unreduce52(s6);
      else if (choice < 11) return S6::Unreduce61(s6);
      return S6::Unreduce62(s6);
   }

   S6 RandomUnreduceTwo(const S6& s6) {
      S6 s(s6);
      while (CountPositive(s) < 2)
         s = RandomUnreduceOne(s);
      return s;
   }

   S6 RandomUnreduceThree(const S6& s6) {
      S6 s(s6);
      while (CountPositive(s) < 3) {
         s = RandomUnreduceOne(s);
         if (!LRL_Cell(s).GetValid()) s = S6("-1 -1 -1 -1 -1 -1");
      }
      return s;
   }


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