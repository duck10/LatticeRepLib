#ifndef NULLSPACE_H
#define NULLSPACE_H

#include "LRL_Vector3.h"
#include "S6.h"

class Nullspace {
public:

   static void FillZeros(const S6& s6in, const double circlepart, const std::vector<size_t>& vzeros);
   static std::vector<size_t> FindQuartets(const S6& s6);
   static void FindNonZeroTriplets(const S6& s6, std::vector<std::vector<size_t> >& triplets);
   static std::vector<size_t> FindZeros(const S6& s6);
   static std::vector<std::pair<size_t, size_t> > FindEqualNonZeroPairs(const S6& s6);
   static int FindNullspace(const S6& s6, std::vector<size_t>&  quartets,
       std::vector<std::vector<size_t> >& triplets,
       std::vector<std::pair<size_t, size_t> >&  pairs,
       std::vector<size_t>&  zeros);
   static S6 MakeCircle(const S6& s6, const Vector_3& v);
   static void Test_CenteredNullspace2And3(
      const std::string& deloneVariety,
      const std::string& latticeCentering,
      const S6& s6cell,
      const std::vector<Vector_3>& v);
   static int GetNullspaceDimension(const S6& s6);
};





#endif  // NULLSPACE_H