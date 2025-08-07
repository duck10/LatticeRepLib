#ifndef P3_SPECIAL_GENERATOR_H
#define P3_SPECIAL_GENERATOR_H

#include "P3.h"
#include "P3Utilities.h"
#include "Polar.h"

class P3SpecialGenerator {
public:
   explicit P3SpecialGenerator(double perturbationMagnitude = 0.0)
      : m_counter(0), m_perturbationMagnitude(perturbationMagnitude) {
   }

   void SetPerturbationMagnitude(double mag) {
      m_perturbationMagnitude = mag;
   }

   P3 Next() {
      const int type = m_counter % m_totalTypes;
      ++m_counter;

      P3 base = GenerateByType(type);
      return PerturbP3(base, m_perturbationMagnitude);
   }

   static std::pair<P3, P3> GenerateRandomPair(const double magnitude) {
      const LRL_Cell baseCell = P3(Polar::rand());
      P3 p3Base(baseCell);
      p3Base = 100.0 * p3Base / p3Base.norm();
      const P3 p3Test = P3(baseCell);
      if (!p3Test.GetValid() || !baseCell.IsValid()) {
         const int i19191 = 19191;
      }
      const P3 perturbedP3 = PerturbP3(P3(baseCell), magnitude);
      const P3 diff = p3Test - perturbedP3;
      const LRL_Cell cellPerturbed(perturbedP3);
      return{ p3Test, perturbedP3 };
   }


   int Count() const { return m_counter; }

private:
   int m_counter;
   double m_perturbationMagnitude;
   RHrand m_rng;
   static constexpr int m_totalTypes = 11;

   P3 GenerateByType(int type) const {
      switch (type) {
      case 0: return GenerateRawRandom();
      case 1: return GenerateZeroBoth(0);
      case 2: return GenerateZeroFirst(1);
      case 3: return GenerateZeroSecond(2);
      case 4: return GenerateEqualFirsts();
      case 5: return GenerateEqualSeconds();
      case 6: return GenerateEqualFirstSecondSame(0);
      case 7: return GenerateEqualFirstSecondSame(1);
      case 8: return GenerateEqualFirstSecondSame(2);
      case 9: return GenerateEqualFirstSecondCross();
      case 10:return GenerateEqualSecondSecondCross();
      default: return GenerateRawRandom();
      }
   }

   P3 GenerateRawRandom() const {
      return P3(Polar::rand());
   }

   P3 GenerateZeroBoth(int index) const {
      P3 p = P3(Polar::rand());
      p[index].first = 0.0;
      p[index].second = 0.0;
      return p;
   }

   P3 GenerateZeroFirst(int index) const {
      P3 p = P3(Polar::rand());
      p[index].first = 0.0;
      return p;
   }

   P3 GenerateZeroSecond(int index) const {
      P3 p = P3(Polar::rand());
      p[index].second = 0.0;
      return p;
   }

   P3 GenerateEqualFirsts() const {
      P3 p = P3(Polar::rand());
      double val = p[0].first;
      p[1].first = val;
      p[2].first = val;
      return p;
   }

   P3 GenerateEqualSeconds() const {
      P3 p = P3(Polar::rand());
      double val = p[0].second;
      p[1].second = val;
      p[2].second = val;
      return p;
   }

   P3 GenerateEqualFirstSecondSame(int index) const {
      P3 p = P3(Polar::rand());
      double val = p[index].first;
      p[index].second = val;
      return p;
   }

   P3 GenerateEqualFirstSecondCross() const {
      P3 p = P3(Polar::rand());
      double val = p[0].first;
      p[1].second = val;
      p[2].second = val;
      return p;
   }

   P3 GenerateEqualSecondSecondCross() const {
      P3 p = P3(Polar::rand());
      double val = p[0].second;
      p[1].second = val;
      p[2].second = val;
      return p;
   }
};

#endif // P3_SPECIAL_GENERATOR_H
