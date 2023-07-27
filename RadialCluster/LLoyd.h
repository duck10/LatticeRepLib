#ifndef LLOYD_H
#define LLOYD_H

#include <vector>


#include "Vector_2.h"
#include "TNear.h"


class LloydStep {
public:
   LloydStep() {}
   void SetCentroids(const std::vector<Vector_2>& vc);
   //std::vector<Centroid> DoLloydStep(const CNearTree<T>& tree, const std::vector<Vector_2>& vdata);
   static std::vector<Vector_2> DoLloydStep(
      const std::vector<Vector_2>& centroids, const std::vector<Vector_2>& vdata);

   static std::vector<Vector_2> BuildCentroidList(const size_t n, const std::vector<Vector_2>& vv);

   static std::vector<Centroid> CentroidsFromVector_2(const std::vector<Vector_2>& vv);

private:
   //T m_cenroids;
   //size_t m_ncent;
};


#endif // LLOYD_H
