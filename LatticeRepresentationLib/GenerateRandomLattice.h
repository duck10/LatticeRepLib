#ifndef GENERATERANDOMLATTICE_H
#define GENERATERANDOMLATTICE_H

#include "LRL_Cell.h"
#include "LRL_MinMaxTools.h"
#include "rhrand.h"
#include "S6.h"

template<typename T = S6>
class GenerateRandomLattice {

public:
   GenerateRandomLattice(const int seed)
      : m_seed(seed)
      , m_perturb(1.0)
   {
      m_rhrand.srandom(seed);
   }

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
   
   LRL_Cell RandCell(const double minEdgeA, const double maxEdgeA) {
      return RandCell(minEdgeA, maxEdgeA, minEdgeA, maxEdgeA, minEdgeA, maxEdgeA);
   }

   LRL_Cell RandCell(const double minEdgeA, const double maxEdgeA, 
                     const double minEdgeB, const double maxEdgeB, 
                     const double minEdgeC, const double maxEdgeC) {
      LRL_Cell c;
      Prepare2CellElements(minEdgeA, maxEdgeA, 0, c);
      Prepare2CellElements(minEdgeB, maxEdgeB, 1, c);
      Prepare2CellElements(minEdgeC, maxEdgeC, 2, c);
      PrepareAngles(c[3], c[4], c[5]);
      c.SetValid(true);
      return c;
   }

private:

   void GenerateThreeRandomFractionsThatSumToOne( RHrand& rhrand, double& r1, double& r2, double& r3 ) {
      const double a1 = rhrand.urand();
      const double a2 = rhrand.urand();
      const double a3 = rhrand.urand();
      const double sum = a1 + a2 + a3;
      r1 = a1 / sum;
      r2 = a2 / sum;
      r3 = a3 / sum;
   }

   void PrepareAngles( double& a3, double& a4, double& a5 ) {
      static const double thirtyDegrees = 30.0 / 180.0 * 4 * atan(1.0);
      static const double sixtyDegrees = 2.0*thirtyDegrees;
      static const double ninetyDegrees = 3.0*thirtyDegrees;
      static const double oneeightyDegrees = 6.0*thirtyDegrees;
      static const double threesixtyDegrees = 12.0*thirtyDegrees;
      static const double tenDegrees = thirtyDegrees / 3.0;
      static const double oneDegree = thirtyDegrees / 30.0;

      static RHrand r;
      double totalAngles = 0.0;
      while ( totalAngles < 2.0*tenDegrees)
         totalAngles = r.urand() * threesixtyDegrees;
      double r1;
      double r2;
      double r3;

      a3 = DBL_MAX;

      while (a3 > oneeightyDegrees || a4 > oneeightyDegrees || a5 > oneeightyDegrees || a3 + a4 + a5 > 2.0*oneeightyDegrees ||
         (a3 + a4 + a5 - 2.0*maxNC(a3, a4, a5) < 0.0)  || 
         a3 < oneDegree || a4 < oneDegree || a5 < oneDegree)
      {
         GenerateThreeRandomFractionsThatSumToOne(r, r1, r2, r3);
         a3 = r1 * totalAngles;
         a4 = r2 * totalAngles;
         a5 = r3 * totalAngles;
      }
      return;
   }

   void Prepare2CellElements(const double minEdge, const double maxEdge, const unsigned long i, LRL_Cell& c) {
      static RHrand r;
      const double range = std::fabs(minEdge - maxEdge);
      const double d1 = r.urand();
      c[i] = range * d1 + minEdge;
   }

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