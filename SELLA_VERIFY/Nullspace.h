#ifndef NULLSPACE_H
#define NULLSPACE_H

#include "LRL_Vector3.h"
#include "S6.h"

class Nullspace {
public:

   static void FillZeros(const S6& s6in, const double cirilepart, const std::vector<unsigned long>& vzeros);
   static std::vector<unsigned long> FindQuartets(const S6& s6);
   static void FindNonZeroTriplets(const S6& s6, std::vector<std::vector<unsigned long> >& triplets);
   static std::vector<unsigned long> FindZeros(const S6& s6);
   static std::vector<std::pair<unsigned long, unsigned long> > FindEqualNonZeroPairs(const S6& s6);
   static int FindNullspace(const S6& s6, std::vector<unsigned long>&  quartets,
       std::vector<std::vector<unsigned long> >& triplets,
       std::vector<std::pair<unsigned long, unsigned long> >&  pairs,
       std::vector<unsigned long>&  zeros);
   static S6 MakeCircle(const S6& s6, const Vector_3& v);
   static void Test_CenteredNullspace2And3(
      const std::string& deloneVariety,
      const std::string& latticeCentering,
      const S6& s6cell,
      const std::vector<Vector_3>& v);
   static int GetNullspaceDimension(const S6& s6);
};





#endif  // NULLSPACE_H