#include <algorithm>
#include <iostream>     // std::cout, std::endl
#include <string>
#include <vector>

#include "ColorTables.h"
#include "Nullspace.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "Sella.h"


void Nullspace::FillZeros(const S6& s6in, const double circlepart, const std::vector<size_t>& vzeros ) {
   S6 s6(s6in);
}

std::vector<size_t> Nullspace::FindQuartets(const S6& s6) {
   size_t i, k;
   double vi;
   bool foundQuartet = false;
   for (i = 0; i < 3; ++i) {
      int count = 0;
      vi = s6[i];
      for (k = i; k < 6; ++k) {
         if (s6[k] == vi) ++count;
         if (count == 4) {
            foundQuartet = true;
            break;
         }
      }
      if (foundQuartet) break;
   }

   std::vector<size_t> vout;
   if (foundQuartet) {
      for (i = 0; i < 6; ++i) {
         if (vi == s6[i]) vout.push_back(i);
      }
   }
   if (vout.size() != 4) vout.clear();

   return vout;
}

void Nullspace::FindNonZeroTriplets(const S6& s6, std::vector<std::vector<size_t> >& triplets) {
   int count = 0;
   size_t i, k;
   double vi= -DBL_MAX;
   for (i = 0; i < 3; ++i) {
      for (k = i + 1; k < 6; ++k) {
         if (s6[k] != 0 && s6[k] == s6[i]) {
            ++count;
            vi = s6[i];
         }
         if (count == 3) break;
      }
   }

   std::vector<size_t> vout;
   for (i = 0; i < 6; ++i) {
      if (vi == s6[i]) vout.push_back(i);
   }

   if (vout.size() == 3) {
      triplets.push_back(vout);
      S6 temp(s6);
      for (size_t kk = 0; kk < vout.size(); ++kk) temp[vout[kk]] = 19191UL + kk + temp.norm();
      FindNonZeroTriplets(temp, triplets);
   }
}


std::vector<size_t> Nullspace::FindZeros(const S6& s6) {
   std::vector<size_t> v;
   for (size_t i = 0; i < 6; ++i)
      if (abs(s6[i]) < 1.0E-10) v.push_back(i);
   return v;
}

std::vector<std::pair<size_t,size_t> > Nullspace::FindEqualNonZeroPairs(const S6& s6) {
   std::vector<std::pair<size_t, size_t> > v;
   for (size_t i = 0; i < 5; ++i)
      for (size_t k = i + 1; k < 6; ++k)
         if ( abs(s6[i]) > 1.0E-10 && abs(s6[i] -s6[k]) < 1.0E-6) v.push_back(std::make_pair(i,k));
   return v;
}


int Nullspace::FindNullspace(const S6& s6, std::vector<size_t>&  quartets,
   std::vector<std::vector<size_t> >& triplets,
   std::vector<std::pair<size_t, size_t> >&  pairs,
   std::vector<size_t>&  zeros) {
   quartets = FindQuartets(s6);
   if (quartets.empty()) {
      FindNonZeroTriplets(s6, triplets);
      if (triplets.empty())
         pairs = FindEqualNonZeroPairs(s6);
   }
   zeros = FindZeros(s6);
   const int dimension = (quartets.empty() ? 0 : 3) + int(triplets.size() * 2 + pairs.size() + zeros.size());
   return dimension;
}

S6 Nullspace::MakeCircle( const S6& s6, const Vector_3& v) {
   std::vector<size_t>  quartets;
   std::vector<std::vector<size_t> > triplets;
   std::vector<std::pair<size_t, size_t> >  pairs;
   std::vector<size_t>  zeros;

   FindNullspace(s6, quartets, triplets, pairs, zeros);
   size_t vpos = 0;
   S6 circle("0 0 0  0 0 0");
   for (size_t i = 0; i < zeros.size(); ++i) {
      circle[zeros[i]] = v[vpos];
      ++vpos;
   }

   for (size_t i = 0; i < pairs.size(); ++i) {
      circle[pairs[i].first] = v[vpos]/2.0;
      circle[pairs[i].second] = -v[vpos]/2.0;
      ++vpos;
   }

   // NOTE: triplets only occur for R lattices, and there can be one triplet or one plus zeros or for H and then one plus two zeros
   // rquires 4-D input if there are two
   vpos = 0;
   for (size_t i = 0; i < triplets.size(); ++i) {
      circle[triplets[i][0]] = v[0 + vpos] / 3.0;
      circle[triplets[i][1]] = v[1 + vpos] / 3.0;
      circle[triplets[i][2]] = -(v[0 + vpos] + v[1 + vpos]) / 3.0;
      vpos += 2;
   }

   // quartets only occur for O1A (primitive cell) and T2, and then only one is possible
   for (size_t i = 0; i < quartets.size(); ++i) {
      circle[quartets[0]] = v[0] / 4.0;
      circle[quartets[1]] = v[1] / 4.0;
      circle[quartets[2]] = v[2] / 4.0;
      circle[quartets[3]] = -(v[0] + v[1] + v[2]) / 4.0;
   }

   return circle;
}

int Nullspace::GetNullspaceDimension(const S6& s6) {
   std::vector<size_t>  quartets;
   std::vector<std::vector<size_t> > triplets;
   std::vector<std::pair<size_t, size_t> >  pairs;
   std::vector<size_t>  zeros;
   const int dimension = FindNullspace(s6, quartets, triplets, pairs, zeros);
   return dimension;
}


void Nullspace::Test_CenteredNullspace2And3(
   const std::string& deloneVariety,
   const std::string& latticeCentering,
   const S6& s6cell,
   const std::vector<Vector_3>& v) {
   Sella sella;
   //expect a vector_3 file from DataGen Circle, z=0
   const S6 s6 = s6cell;
   const double radius = v[0].Norm();
   std::cout << std::endl << deloneVariety << "  " << latticeCentering << std::endl << std::endl;
   const int dimension = GetNullspaceDimension(s6);

   std::vector<double> vbest;
   for (unsigned int i = 0; i < v.size(); ++i)
   {
      const S6 circle = MakeCircle(s6, v[i]);
      const S6 primitive = LatticeConverter::SellingReduceCell(latticeCentering, s6 + circle);
      const double best = sella.GetFitForDeloneType(deloneVariety, primitive);
      vbest.push_back(best);
   }
   auto p = std::minmax_element(vbest.begin(), vbest.end());
   const double minbest = *p.first;
   const double maxbest = *p.second;
   const double limit = std::max(1.0, maxbest);
   ColorRange cr(minbest, maxbest);

   std::cout << "# Title Delone variety and input centering " << deloneVariety << "  "  << latticeCentering << std::endl;
   std::cout << "# Radius of confusion " << radius << std::endl;
   std::cout << "# min best Sella values " << minbest << std::endl;
   std::cout << "# max best Sella values " << maxbest << std::endl;

   for (unsigned int i = 0; i < v.size(); ++i)
   {
      const double best = vbest[i];
      const int ci = cr.ColorIndex(best);
      std::cout << v[i][0] << "," << v[i][1] << "," << v[i][2] << "," << best << "," << std::endl;
      //std::cout << v[i][0] << "," << v[i][1] << "," << v[i][2] << std::endl;
   }
}
