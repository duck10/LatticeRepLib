
#include "Delone.h"
#include "GeneratePairsOfTestData.h"
#include "GenerateRandomLattice.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ToString.h"
#include "StoreResults.h"


static int seed = 2;

std::pair<S6, S6> GeneratePairsOfTestData::ScalePairToSpecifedSeparation(const S6& s1, const S6& s2, const double factor) {
   double deltanorm = (s1 - s2).norm();
   if (deltanorm > 0.00001) {
      const double n1 = s1.norm();
      const double n2 = s2.norm();
   }
   if (deltanorm < 1.0E-5) deltanorm = 1.0;
   return std::make_pair(factor*s1 / deltanorm, factor*s2 / deltanorm);
}

S6 GeneratePairsOfTestData::DoReflection(const S6& s, const unsigned long n) {
   static const std::vector< S6(*)(const S6&)> refl = S6::SetRelectionFunctions();
   return refl[n](s);
}

std::pair<S6, S6> GeneratePairsOfTestData::GeneratePairsOfTestData::BinarySearchReduced(const S6& s1, const S6& s2, const int npass) {
   if (npass <= 0) {
      std::pair<S6, S6> pout = ScalePairToSpecifedSeparation(s1, s2, 5.0);
      const double d = (pout.first - pout.second).norm();
      return pout;
   }
   // norm doesn't work here because s2 is invalid !!!!!!!!!!
   // operator- doesn't work here because s2 is invalid !!!!!!!!!!
   S6 s6delta;
   for (unsigned long i = 0; i < 6; ++i) s6delta[i] = s2[i] - s1[i];
   S6 midpoint = s1 + 0.5 * s6delta;
   const bool bmid = LRL_Cell(midpoint).GetValid();
   midpoint.SetValid(bmid);

   const bool isReduced = midpoint.IsAllMinus();

   if (isReduced) {
      return BinarySearchReduced(midpoint, s2, npass - 1);
   }
   else {
      return BinarySearchReduced(s1, midpoint, npass - 1);
   }
}

S6 GeneratePairsOfTestData::DoRandomReflection(const S6& s) {
   return DoReflection(s, rand() & 23);
}

std::pair<S6, S6> GeneratePairsOfTestData::GeneratePair_Reduced_Other_ReducesFarAway(const unsigned long numberOfZeros) {
   static GenerateRandomLattice<S6> generator(seed);
   const S6 v1 = generator.randSellingReduced();
   const S6 v2 = generator.GenerateExtreme();

   const std::pair<S6, S6> output = BinarySearchReduced(v1, v2, 12);
   S6 out;
   if (Selling::Reduce(output.second, out)) {
      const int random = rand() & 23;
      return std::make_pair(output.first, DoRandomReflection(out));
   }
   return std::make_pair(S6(),S6());
}

std::string GeneratePairsOfTestData::FormatPairOfTestS6Vectors(const S6& s1, const S6& s2, const std::string& id) {
   return LRL_ToString("S ", s1, "    ", id) + " " + LRL_ToString("\nS ", s2, "    ", id, "\n");
}

void GeneratePairsOfTestData::MultiGeneratePair_Reduced_Other_ReducesFarAway(const unsigned long targetReducedUnreducedPairCount) {
   /*
   One point is generated. It is slightly modified, and it is reduced. If the reduced one is
   somewhat or more farther away than the modification, then the pair is kept and output.
   */
   StoreResults<int, std::string > store(5);
   store.SetTitle("Generate Pairs with one reduced and the other nearby but reduces far.");
   std::pair<S6, S6> output;

   for (unsigned long i = 0; i < targetReducedUnreducedPairCount; ++i) {
      output = GeneratePair_Reduced_Other_ReducesFarAway(0);
      const int rank = Rank(output.first);
      store.Store(rank, LRL_ToString("S", output.first, "\nS    ", output.second, "\n"));

      store.Store(rank, FormatPairOfTestS6Vectors(output.first, output.second, ""));
   }

   store.ShowResults();
}


int GeneratePairsOfTestData::Rank(const S6& s) {
   const double length = s.norm();
   double minS6 = DBL_MAX;;

   unsigned long nzero = 0;
   for (unsigned long i = 0; i < 6; ++i) {
      minS6 = std::abs(std::min(minS6, s[i]));
   }
   return (int)(log10(minS6));
}

void GeneratePairsOfTestData::GenerateRandomLatticesAndCatalogByReductionStepCount(const unsigned long n) {
   GenerateRandomLattice<S6> grl(19191);
   StoreResults<unsigned long, std::string> storeExtreme(3);
   storeExtreme.SetTitle("Extreme Unreduced S6 Generation -- >1000 cycles means reduction failed");
   storeExtreme.SetKeyLabel("cycles");
   StoreResults<unsigned long, std::string> storesimple(3);
   storesimple.SetTitle("S6 Selling Unreduced Generation -- >1000 cycles means reduction failed");
   storesimple.SetKeyLabel("cycles");
   S6 s6in, out;
   bool b;

   for (unsigned long i = 0; i < n; ++i) {
      s6in = grl.GenerateExtreme();
      if (s6in.GetValid()) {
         b = Selling::Reduce(s6in, out);
         storeExtreme.Store(Selling::GetCycles(), LRL_ToString(" ", s6in, " ", Selling::GetCycles(), " ", PositiveDistance(s6in)));
      }

      s6in = grl.randSellingUnreduced();
      LRL_Cell cell = LRL_Cell(s6in);
      if (cell.GetValid()) {
         double test = std::abs(39.4918 - s6in[0]);
         b = Selling::Reduce(s6in, out);
         LRL_Cell cellout = LRL_Cell(out);
         storesimple.Store(Selling::GetCycles(), LRL_ToString(" ", s6in, " ", Selling::GetCycles(), " ", PositiveDistance(s6in)));
      }

   }
   storesimple.ShowResultsByKeyAscending();
   std::cout << std::endl;
   storeExtreme.ShowResultsByKeyAscending();
}

double GeneratePairsOfTestData::PositiveDistance(const S6& s6) {
   double sum = 0;
   double sqsum = 0;
   for (unsigned long i = 0; i < 6; ++i) {
      sum += (s6[i] > 0.0) ? s6[i] * s6[i] : 0.0;
      sqsum += s6[i] * s6[i];
   }
   return sqrt(sum / sqsum);
}

static const std::string types[] = { std::string("C1 (cI)"), std::string("C3 (cF)"), std::string("C5 (cP)"), std::string("T1 (tI)"), std::string("t2 (tI)"), std::string("t5 (tP)"), std::string("R1 (rP)"), std::string("R3 (rP)"), std::string("O1A (oF)"), std::string("O1B (oI)"), std::string("O2 (oI)"), std::string("O3 (oI)"), std::string("O4 (oS)"), std::string("O5 (oP)"), std::string("M1A (mC)"), std::string("M1B (mC)"), std::string("M2B (mC)"), std::string("M2A (mC)"), std::string("M3 (mC)"), std::string("M4 (mP)"), std::string("A1 (aP)"), std::string("A2 (aP)"), std::string("A3 (aP)"), std::string("H4 (hP)") };
static const unsigned long avoidList[] = { 0,1,3,4,8,9 };

bool GeneratePairsOfTestData::IsGoodLatticeTypeForPerturbation(const unsigned long n) {
   static const std::vector<unsigned long> vAvoid(avoidList, avoidList + sizeof(avoidList) / sizeof(avoidList[0]));
   return std::find(vAvoid.begin(), vAvoid.end(), n) == vAvoid.end();
}

std::pair<S6, S6> GeneratePairsOfTestData::GenerateLatticeTypeExamplesNearDeloneTypes(const unsigned long n) {
   /*
   
   */
   StoreResults<int, std::string > store(5);
   store.SetTitle("Generation of perturbed Delone lattice types");
   store.SetKeyLabel("Delone type");
   static int seed = 1;
   static const std::vector<MatS6> vDeloneTypes = Delone::LoadLatticeTypeProjectors();

   std::pair<S6, S6> p;
   for (unsigned long ncycles = 0; ncycles < std::max(1UL, n); ++ncycles) {
      for (unsigned long i = 0; i < vDeloneTypes.size(); ++i) {
         if (!IsGoodLatticeTypeForPerturbation(i)) continue;
         p = OneLatticeType(vDeloneTypes[i]);
         const double pnorm = (p.first - p.second).norm();
         const bool b = LRL_Cell_Degrees(p.first).GetValid() && LRL_Cell_Degrees(p.second).GetValid();
         if (!b) {
            //std::cout << i << "  failed to find valid cell" << std::endl << vDeloneTypes[i] << std::endl;
         }
         else {
            //std::cout /*<< "in GenerateLatticeTypeExamplesNearDeloneTypes, valid = " << LRL_Cell_Degrees(p.first).GetValid() */ <<
            //   "   " << (p.first) << "       " << DoRandomReflection(p.second) << std::endl;
            //std::cout << "S   " << (p.first) << std::endl;
            //std::cout << "S   " << (p.second) << std::endl << std::endl;
            store.Store(i, GeneratePairsOfTestData::FormatPairOfTestS6Vectors(p.first, p.second, types[i]));
         }
      }
   }
   store.ShowResults();
   return p;
}

std::pair<S6, S6> GeneratePairsOfTestData::OneLatticeType(const MatS6& m) {
   bool again = true;
   unsigned long npass = 0;
   unsigned long maxPass = 100;
   std::pair<S6, S6> p;
   std::pair<S6, S6> pmaxDiff;
   S6 red;
   double pnorm;
   double maxdiff = -DBL_MAX;
   while (again && npass < maxPass) {
      p = OneLatticeAttempt(m);
      pnorm = (p.first - p.second).norm();
      Selling::Reduce(p.second, red);
      if ((p.first - red).norm() > maxdiff) {
         maxdiff = (p.first - red).norm();
         pmaxDiff = std::make_pair(p.first, red);
      }
      again = !LRL_Cell(p.first).GetValid();
      ++npass;
   }

   return pmaxDiff;
}

std::pair<S6, S6> GeneratePairsOfTestData::OneLatticeAttempt(const MatS6& m) {
   static const std::vector<MatS6> vDeloneTypes = Delone::LoadLatticeTypeProjectors();
   S6 test(GenerateStartingPoint(m));
   const S6 mod = Perturb(test, 0.01);
   const double originalDiff = (test - mod).norm();
   S6 out2;
   const bool b2 = Selling::Reduce(mod, out2);

   const double newDiff = (test - out2).norm();

   if (newDiff < 1.5*originalDiff) {
      test.SetValid(false);
   }
   return GeneratePairsOfTestData::ScalePairToSpecifedSeparation(test, mod, 1.0);
}

GenerateRandomLattice<S6> generator(seed);

S6 GeneratePairsOfTestData::Perturb(const S6& s, const double fraction) {
   bool again = true;
   static int seed = 11;
   static const std::vector<MatS6> vDeloneTypes = Delone::LoadLatticeTypeProjectors();
   S6 temp;

   unsigned long npass = 0;
   unsigned long maxPass = 100;
   LRL_Cell_Degrees cell(s);
   const double snorm = s.norm();
   while (again && npass < maxPass) {
      S6 modifier = generator.randSellingUnreduced();
      temp = s + fraction * modifier * snorm / modifier.norm();
      again = !LRL_Cell(temp).GetValid();
      ++npass;
   }
   if (npass >= maxPass) {
      temp.SetValid(false);
      //std::cout << "in Perturb attempt abandoned, npass = " << npass << std::endl;
   }
   bool b = temp.GetValid();
   return temp;
}

static int oneAttemptSeed = 1;

S6 GeneratePairsOfTestData::GenerateStartingPoint(const MatS6& m) {
   unsigned long npass = 0;
   unsigned long maxPass = 100;
   S6 test = m * generator.randSellingReduced();

   while (!LRL_Cell(test).GetValid() && npass < maxPass) {
      test = m * generator.randSellingReduced();
      ++npass;
   }

   if (!LRL_Cell(test).GetValid()) {
      test.SetValid(false);
      std::cout << "failed to generate a starting point in GenerateStartingPoint" << std::endl;
   }
   return test;
}

S6 AddOneToSingleScalar( const S6& s) {
   double mins = DBL_MAX;
   unsigned long minIndex;
   for (unsigned long i = 0; i < 6; ++i) {
      if (s[i] < mins) {
         mins = s[i];
         minIndex = i;
      }
   }
   S6 s6(s);
   s6[minIndex] += 1.0;
   return s6;
}

void GeneratePairsOfTestData::StoreOnePointIfValid(StoreResults<unsigned long, std::string >& store, const S6& s6in) {
   if (s6in.IsValid()) {
      S6 out;
      const bool b = Selling::Reduce(s6in, out);
      if (b && out.IsValid()) {
         const unsigned long ncycles = Selling::GetCycles();
         const std::string label = LRL_ToString(ncycles) + " Ex";
         store.Store(Selling::GetCycles(), GeneratePairsOfTestData::FormatPairOfTestS6Vectors(AddOneToSingleScalar(s6in), out, label));
      }
      else
         int i19191 = 19191;
   }
}

void GeneratePairsOfTestData::GeneratePairSamplesByHighReductionCycleCount(const unsigned long n) {
   /*
   Generate a random unreduced lattice (GenerateExtreme picks a point nearly invalid). Then
   the point is reduced, and the slightly modified point, and its unmodified reduced point
   are stored, using the number of cycles in Selling::Reduce that were required as the key.
   Finally, the result is trimmed such that only the highest 1/3 of the cycle counts are
   kept. They are then displayed.
   */
   GenerateRandomLattice<S6> grl(19191);
   StoreResults<unsigned long, std::string > store(8);
   store.SetKeyLabel("cycles");
   GeneratePairsOfTestData::GeneratePairSamplesByHighReductionCycleCount(1000000);
   store.SetTitle("GeneratePairSamplesByHighReductionCycleCount");

   for (unsigned long i = 0; i < n; ++i) {
      StoreOnePointIfValid(store, grl.GenerateExtreme());
   }

   // select the portion of the result that you want to display
   std::vector<unsigned long> v = store.GetKeys();
   for (unsigned long i = 0; i<(unsigned long)(double(v.size()*0.667)); ++i) store.erase(v[i]);
   store.ShowResultsByKeyDescending();
}
