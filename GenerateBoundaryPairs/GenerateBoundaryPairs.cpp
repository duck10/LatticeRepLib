// GenerateBoundaryPairs.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <cfloat>
#include <iomanip>
#include <iostream>
#include <utility>

#include "GenerateRandomLattice.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ToString.h"
#include "PairReporter.h"
#include "S6.h"
#include "StoreResults.h"

static std::pair<S6, S6> ScalePairToSpecifedSeparation(const S6& s1, const S6& s2, const double factor) {
   double deltanorm = (s1 - s2).norm();
   if (deltanorm > 0.00001) {
      const double n1 = s1.norm();
      const double n2 = s2.norm();
   }
   if (deltanorm < 1.0E-5) deltanorm = 1.0;
   return std::make_pair(factor*s1 / deltanorm, factor*s2 / deltanorm);
}

static std::pair<S6, S6> BinarySearchReduced(const S6& s1, const S6& s2, const int npass) {
   if (npass <= 0) {
      std::pair<S6,S6> pout = ScalePairToSpecifedSeparation(s1, s2, 5.0);
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

static int seed = 2;

static S6 DoReflection( const S6& s, const unsigned long n) {
   static const std::vector< S6(*)(const S6&)> refl = S6::SetRelectionFunctions();
   return refl[n](s);
}

static S6 DoRandomReflection( const S6& s) {
   return DoReflection(s, rand() & 23);
}

static std::pair<S6, S6> GeneratePair_Reduced_Unreduced(const unsigned long numberOfZeros) {
   static GenerateRandomLattice<S6> generator(seed);
   const S6 v1 = generator.randSellingReduced();
   const S6 v2 = generator.GenerateExtreme();

   const std::pair<S6, S6> output = BinarySearchReduced(v1, v2, 12);
   //std::cout << std::endl;
   //std::cout << "cell 1 " << LRL_Cell_Degrees(output.first) << std::endl;
   //std::cout << "cell 2 " << LRL_Cell_Degrees(output.second) << std::endl;
   //std::cout << "S6 1" << output.first << std::endl;
   //std::cout << "S6 2" << output.second << std::endl;
   S6 out;
   const bool b = Selling::Reduce(output.second, out);
   //std::cout << "reduced  2   " << out << std::endl;
   //std::cout << "G6   1   " << G6(output.first) << std::endl;
   //std::cout << "G6   2   " << G6(output.first) << std::endl;
   //std::cout << "G6 reduced  2   " << G6(output.second) << std::endl;
   //std::cout << "cell reduced  2   " << LRL_Cell_Degrees(output.second) << std::endl;
   //std::cout << "#1 to #2 " << std::fixed << std::setprecision(3) << (output.first - output.second).norm() << "  #1 to reduced #2 " << (output.first - out).norm() << std::endl << std::endl;
   const int random = rand() & 23;
   return std::make_pair(output.first, DoRandomReflection(out));
}

int Rank(const S6& s) {
   const double length = s.norm();
   double minS6 = DBL_MAX;;

   unsigned long nzero = 0;
   for (unsigned long i = 0; i < 6; ++i) {
      minS6 = std::abs(std::min(minS6, s[i]));
   }
   return (int)(log10(minS6)) ;
}

std::vector<MatS6> LoadLatticeTypeProjectors() {
   std::vector<MatS6> vDeloneTypes;
   vDeloneTypes.push_back(MatS6("1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1"));  //    K1 (cI)
   vDeloneTypes.push_back(MatS6("1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 0  1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 0"));  //    K3 (cF)
   vDeloneTypes.push_back(MatS6("0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 1 1 1  0 0 0 1 1 1  0 0 0 1 1 1"));  //    K5 (cP)
   vDeloneTypes.push_back(MatS6("1 1 0 1 1 0  1 1 0 1 1 0  0 0 1 0 0 1  1 1 0 1 1 0  1 1 0 1 1 0  0 0 1 0 0 1"));  //    Q1 (tI)
   vDeloneTypes.push_back(MatS6("1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 0  1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 1"));  //    Q2 (tI)
   vDeloneTypes.push_back(MatS6("0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 1"));  //    Q5 (tP)
   vDeloneTypes.push_back(MatS6("1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0  0 0 0 1 1 1  0 0 0 1 1 1  0 0 0 1 1 1"));  //    R1 (rP)
   vDeloneTypes.push_back(MatS6("1 0 1 0 1 0  0 0 0 0 0 0  1 0 1 0 1 0  0 0 0 1 0 0  1 0 1 0 1 0  0 0 0 0 0 0"));  //    R3 (rP)
   vDeloneTypes.push_back(MatS6("1 1 0 1 1 0  1 1 0 1 1 0  0 0 1 0 0 0  1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 1"));  //    O1A (oF)
   vDeloneTypes.push_back(MatS6("1 1 0 1 1 0  1 1 0 1 1 0  0 0 1 0 0 1  1 1 0 1 1 0  1 1 0 1 1 0  0 0 1 0 0 1"));  //    O1B (oI)
   vDeloneTypes.push_back(MatS6("1 0 0 0 1 0  0 1 0 1 0 0  0 0 0 0 0 0  0 1 0 1 0 0  1 0 0 0 1 0  0 0 0 0 0 0"));  //    O2 (oI)
   vDeloneTypes.push_back(MatS6("1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 0  1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 0"));  //    O3 (oI)
   vDeloneTypes.push_back(MatS6("0 0 0 0 0 0  0 0 0 0 0 0  0 0 1 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 1"));  //    O4 (oP)
   vDeloneTypes.push_back(MatS6("0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1"));  //    O5 (oP)
   vDeloneTypes.push_back(MatS6("1 1 0 0 0 0  1 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 1"));  //    M1A (mC)
   vDeloneTypes.push_back(MatS6("1 0 0 1 0 0  0 1 0 0 1 0  0 0 1 0 0 0  1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 1"));  //    M1B (mC)
   vDeloneTypes.push_back(MatS6("1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 0  1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 1"));  //    M2A (mC)
   vDeloneTypes.push_back(MatS6("1 0 0 0 0 0  0 1 0 1 0 0  0 0 0 0 0 0  0 1 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1"));  //    M2B (mC)
   vDeloneTypes.push_back(MatS6("1 0 0 0 0 0  0 1 0 0 1 0  0 0 0 0 0 0  0 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 0"));  //    M3 (mC)
   vDeloneTypes.push_back(MatS6("0 0 0 0 0 0  0 0 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1"));  //    M4 (mP)
   vDeloneTypes.push_back(MatS6("1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1"));  //    T1 (aP)
   vDeloneTypes.push_back(MatS6("1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1"));  //    T2 (aP)
   vDeloneTypes.push_back(MatS6("1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 0"));  //    T3 (aP)
   vDeloneTypes.push_back(MatS6("0 0 0 0 0 0  0 0 0 0 0 0  0 0 1 1 1 0  0 0 1 1 1 0  0 0 1 1 1 0  0 0 0 0 0 1"));  //    H4 (hP)

   return vDeloneTypes;
}

void TestLatticeTypes() {
   static int seed = 1;
   static const std::vector<MatS6> vDeloneTypes = LoadLatticeTypeProjectors();
   GenerateRandomLattice<S6> generator(seed);
   generator.randSellingReduced();  // try to get a random lattice that gives valid unit cells for all 
   const S6 temp = generator.randSellingReduced();

   for (unsigned long i = 0; i < vDeloneTypes.size(); ++i) {
      const S6 test = vDeloneTypes[i] * temp;
      const bool b = LRL_Cell_Degrees(test).GetValid();
      std::cout << LRL_Cell_Degrees(test) << std::endl;
   }
}

   GenerateRandomLattice<S6> generator(seed);

S6 Perturb(const S6& s, const double fraction) {
   bool again = true;
   static int seed = 11;
   static const std::vector<MatS6> vDeloneTypes = LoadLatticeTypeProjectors();
   S6 temp;

   unsigned long npass = 0;
   unsigned long maxPass = 100;
   LRL_Cell_Degrees cell(s);
   const double snorm = s.norm();
   while (again && npass < maxPass) {
      S6 modifier = generator.randSellingUnreduced();
      temp = s + fraction * modifier * snorm / modifier.norm() ;
      again = ! LRL_Cell(temp).GetValid();
      ++npass;
   }
   if (npass >= maxPass){
      temp.SetValid(false);
      std::cout << "in Perturb attempt abandoned, npass = " << npass << std::endl;
   }
   bool b = temp.GetValid();
   return temp;
}

   static int oneAttemptSeed = 1;

S6 GenerateStartingPoint(const MatS6& m) {
   unsigned long npass = 0;
   unsigned long maxPass = 100;
   S6 test = m*generator.randSellingReduced();

   while ( !LRL_Cell(test).GetValid() && npass < maxPass ) {
      test = m * generator.randSellingReduced();
      ++npass;
   }

   if ( ! LRL_Cell(test).GetValid()) {
      test.SetValid(false);
      std::cout << "failed to generate a starting point in GenerateStartingPoint" << std::endl;
   }
   return test;
}

std::pair<S6,S6> OneLatticeAttempt(const MatS6& m) {
   static const std::vector<MatS6> vDeloneTypes = LoadLatticeTypeProjectors();
   S6 test(GenerateStartingPoint(m));
   const S6 mod = Perturb(test, 0.01);
   const double originalDiff = (test - mod).norm();
   S6 out2;
   const bool b2 = Selling::Reduce(mod, out2);

   const double newDiff = (test-out2).norm();

   if (newDiff < 1.5*originalDiff){
      test.SetValid(false);
   }
   return ScalePairToSpecifedSeparation(test, mod, 1.0);
}

std::pair<S6, S6> OneLatticeType( const MatS6& m) {
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
      if ( (p.first-red).norm() > maxdiff) {
         maxdiff = (p.first - red).norm();
         pmaxDiff = std::make_pair(p.first, red);
      }
      again = !LRL_Cell(p.first).GetValid();
      ++npass;
   }

   return pmaxDiff;
}


static const std::string types[] = {std::string("K1 (cI)"), std::string("K3 (cF)"), std::string("K5 (cP)"), std::string("Q1 (tI)"), std::string("Q2 (tI)"), std::string("Q5 (tP)"), std::string("R1 (rP)"), std::string("R3 (rP)"), std::string("O1A (oF)"), std::string("O1B (oI)"), std::string("O2 (oI)"), std::string("O3 (oI)"), std::string("O4 (oP)"), std::string("O5 (oP)"), std::string("M1A (mC)"), std::string("M1B (mC)"), std::string("M2A (mC)"), std::string("M2B (mC)"), std::string("M3 (mC)"), std::string("M4 (mP)"), std::string("T1 (aP)"), std::string("T2 (aP)"), std::string("T3 (aP)"), std::string("H4 (hP)")};
static const unsigned long avoidList[] = {0,1,3,4,8,9};

bool IsGoodLatticeTypeForPerturbation(const unsigned long n) {
   static const std::vector<unsigned long> vAvoid(avoidList,avoidList+sizeof(avoidList)/sizeof(avoidList[0]));
   return std::find(vAvoid.begin(), vAvoid.end(), n) == vAvoid.end();
}

std::pair<S6, S6> GenerateLatticeTypeExamples( const unsigned long n) {
   StoreResults<int, std::string > store(20);
   store.SetKeyLabel("Delone type");
   static int seed = 1;
   static const std::vector<MatS6> vDeloneTypes = LoadLatticeTypeProjectors();

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
            //std::cout /*<< "in GenerateLatticeTypeExamples, valid = " << LRL_Cell_Degrees(p.first).GetValid() */ <<
            //   "   " << (p.first) << "       " << DoRandomReflection(p.second) << std::endl;
            //std::cout << "S   " << (p.first) << std::endl;
            //std::cout << "S   " << (p.second) << std::endl << std::endl;
            store.Store(i, LRL_ToString("S ", p.first, "\nS ", p.second, "    ", types[i], "\n"));
         }
      }
   }
   store.ShowResults();
   return p;
}

int main()
{
   GenerateLatticeTypeExamples(1200);
   StoreResults<int, std::string > store(5);
   PairReporter<S6, S6> output;

   for (unsigned long i = 0; i < 2000; ++i) {
      output = GeneratePair_Reduced_Unreduced(0);
      output.SetSeparator("     ");
      const int rank = Rank(output.GetFirst());
      store.Store(rank, LRL_ToString("S", output.GetFirst(), "\nS    ", output.GetSecond(), "\n"));

      std::cout << LRL_ToString("S", output.GetFirst(), "\nS    ", output.GetSecond(), "\n") << std::endl;
   }

   //store.ShowResults();
   return 0;
}


/*
Projectors for Delone types
K1 (cI)
1 1 1  1 1 1
1 1 1  1 1 1
1 1 1  1 1 1
1 1 1  1 1 1
1 1 1  1 1 1
1 1 1  1 1 1

K3(cF)
1 1 0  1 1 0
1 1 0  1 1 0
0 0 0  0 0 0
1 1 0  1 1 0
1 1 0  1 1 0
0 0 0  0 0 0

K5 (cP)
0 0 0  0 0 0
0 0 0  0 0 0
0 0 0  0 0 0
0 0 0  1 1 1
0 0 0  1 1 1
0 0 0  1 1 1

Q1 (tI)
1 1 0  1 1 0
1 1 0  1 1 0
0 0 1  0 0 1
1 1 0  1 1 0
1 1 0  1 1 0
0 0 1  0 0 1


Q2 (tI)
1 1 0  1 1 0
1 1 0  1 1 0
0 0 0  0 0 0
1 1 0  1 1 0
1 1 0  1 1 0
0 0 0  0 0 1

Q5 (tP)
0 0 0  0 0 0
0 0 0  0 0 0
0 0 0  0 0 0
0 0 0  1 1 0
0 0 0  1 1 0
0 0 0  0 0 1

R1 (rP)
1 1 1  0 0 0
1 1 1  0 0 0
1 1 1  0 0 0
0 0 0  1 1 1
0 0 0  1 1 1
0 0 0  1 1 1

R3 (rP)
1 0 1  0 1 0
0 0 0  0 0 0
1 0 1  0 1 0
0 0 0  1 0 0
1 0 1  0 1 0
0 0 0  0 0 0

O1A (oF)
1 1 0  1 1 0
1 1 0  1 1 0
0 0 1  0 0 0
1 1 0  1 1 0
1 1 0  1 1 0
0 0 0  0 0 1

O1B (oI)
1 1 0  1 1 0
1 1 0  1 1 0
0 0 1  0 0 1
1 1 0  1 1 0
1 1 0  1 1 0
0 0 1  0 0 1

O2 (oI)
1 0 0  0 1 0
0 1 0  1 0 0
0 0 0  0 0 0
0 1 0  1 0 0
1 0 0  0 1 0
0 0 0  0 0 0

O3 (oI)
1 0 0  0 1 0
0 1 0  1 0 0
0 0 0  0 0 0
1 0 0  0 1 0
0 1 0  1 0 0
0 0 0  0 0 0

O4 (oP)
0 0 0  0 0 0
0 0 0  0 0 0
0 0 1  0 0 0
0 0 0  1 1 0
0 0 0  1 1 0
0 0 0  0 0 1

O5 (oP)
0 0 0  0 0 0
0 0 0  0 0 0
0 0 0  0 0 0
0 0 0  1 0 0
0 0 0  0 1 0
0 0 0  0 010

M1A (mC)
1 1 0  0 0 0
1 1 0  0 0 0
0 0 1  0 0 0
0 0 0  1 1 0
0 0 0  1 1 0
0 0 0  0 0 1

M1B (mC)
1 0 0  1 0 0
0 1 0  0 1 0
0 0 1  0 0 0
1 0 0  1 0 0
0 1 0  0 1 0
0 0 0  0 0 1

M2A (mC)
1 0 0  1 0 0
0 1 0  0 1 0
0 0 0  0 0 0
1 0 0  1 0 0
0 1 0  0 1 0
0 0 0  0 0 1

M1B (mC)
1 0 0  1 0 0
0 1 0  0 1 0
0 0 0  0 0 0
0 1 0  0 1 0
0 0 0  0 1 0
0 0 0  0 0 1

M2B (mC)
1 0 0  0 0 0
0 1 0  1 0 0
0 0 0  0 0 0
0 1 0  1 0 0
0 0 0  0 1 0
0 0 0  0 0 1

M3 (mC)
1 0 0  0 0 0
0 1 0  0 1 0
0 0 0  0 0 0
0 0 0  1 0 0
0 1 0  0 1 0
0 0 0  0 0 0

M4 (mP)
0 0 0  0 0 0
0 0 0  0 0 0
0 0 1  0 0 0
0 0 0  1 0 0
0 0 0  0 1 0
0 0 0  0 0 1

T1 (aP)
1 0 0  0 0 0
0 1 0  0 0 0
0 0 1  0 0 0
0 0 0  1 0 0
0 0 0  0 1 0
0 0 0  0 0 1

T2 (aP)
1 0 0  0 0 0
0 1 0  0 0 0
0 0 0  0 0 0
0 0 0  1 0 0
0 0 0  0 1 0
0 0 0  0 0 1

T3 (aP)
1 0 0  0 0 0
0 1 0  0 0 0
0 0 0  0 0 0
0 0 0  1 0 0
0 0 0  0 1 0
0 0 0  0 0 0

H4 (hP)
0 0 0  0 0 0
0 0 0  0 0 0
0 0 1  1 1 0
0 0 1  1 1 0
0 0 1  1 1 0
0 0 0  0 0 1

vDeloneTypes.push_back(MatS6( "1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1" ));  //    K1 (cI)
vDeloneTypes.push_back(MatS6( "1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 0  1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 0" ));  //    K3 (cF)
vDeloneTypes.push_back(MatS6( "0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 1 1 1  0 0 0 1 1 1  0 0 0 1 1 1" ));  //    K5 (cP)
vDeloneTypes.push_back(MatS6( "1 1 0 1 1 0  1 1 0 1 1 0  0 0 1 0 0 1  1 1 0 1 1 0  1 1 0 1 1 0  0 0 1 0 0 1" ));  //    Q1 (tI)
vDeloneTypes.push_back(MatS6( "1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 0  1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 1" ));  //    Q2 (tI)
vDeloneTypes.push_back(MatS6( "0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 1" ));  //    Q5 (tP)
vDeloneTypes.push_back(MatS6( "1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0  0 0 0 1 1 1  0 0 0 1 1 1  0 0 0 1 1 1" ));  //    R1 (rP)
vDeloneTypes.push_back(MatS6( "1 0 1 0 1 0  0 0 0 0 0 0  1 0 1 0 1 0  0 0 0 1 0 0  1 0 1 0 1 0  0 0 0 0 0 0" ));  //    R3 (rP)
vDeloneTypes.push_back(MatS6( "1 1 0 1 1 0  1 1 0 1 1 0  0 0 1 0 0 0  1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 1" ));  //    O1A (oF)
vDeloneTypes.push_back(MatS6( "1 1 0 1 1 0  1 1 0 1 1 0  0 0 1 0 0 1  1 1 0 1 1 0  1 1 0 1 1 0  0 0 1 0 0 1" ));  //    O1B (oI)
vDeloneTypes.push_back(MatS6( "1 0 0 0 1 0  0 1 0 1 0 0  0 0 0 0 0 0  0 1 0 1 0 0  1 0 0 0 1 0  0 0 0 0 0 0" ));  //    O2 (oI)
vDeloneTypes.push_back(MatS6( "1 0 0 0 1 0  0 1 0 1 0 0  0 0 0 0 0 0  1 0 0 0 1 0  0 1 0 1 0 0  0 0 0 0 0 0" ));  //    O3 (oI)
vDeloneTypes.push_back(MatS6( "0 0 0 0 0 0  0 0 0 0 0 0  0 0 1 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 1" ));  //    O4 (oP)
vDeloneTypes.push_back(MatS6( "0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1" ));  //    O5 (oP)
vDeloneTypes.push_back(MatS6( "1 1 0 0 0 0  1 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 1" ));  //    M1A (mC)
vDeloneTypes.push_back(MatS6( "1 0 0 1 0 0  0 1 0 0 1 0  0 0 1 0 0 0  1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 1" ));  //    M1B (mC)
vDeloneTypes.push_back(MatS6( "1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 0  1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 1" ));  //    M2A (mC)
vDeloneTypes.push_back(MatS6( "1 0 0 0 0 0  0 1 0 1 0 0  0 0 0 0 0 0  0 1 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1" ));  //    M2B (mC)
vDeloneTypes.push_back(MatS6( "1 0 0 0 0 0  0 1 0 0 1 0  0 0 0 0 0 0  0 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 0" ));  //    M3 (mC)
vDeloneTypes.push_back(MatS6( "0 0 0 0 0 0  0 0 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1" ));  //    M4 (mP)
vDeloneTypes.push_back(MatS6( "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1" ));  //    T1 (aP)
vDeloneTypes.push_back(MatS6( "1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1" ));  //    T2 (aP)
vDeloneTypes.push_back(MatS6( "1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 0" ));  //    T3 (aP)
vDeloneTypes.push_back(MatS6( "0 0 0 0 0 0  0 0 0 0 0 0  0 0 1 1 1 0  0 0 1 1 1 0  0 0 1 1 1 0  0 0 0 0 0 1" ));  //    H4 (hP)



*/
