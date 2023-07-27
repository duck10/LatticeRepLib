#include "LLoyd.h"
#include "rhrand.h"

#include <cmath>

std::pair<double, double> GetDataMin(const std::vector<Vector_2>& v) {
   std::pair<double, double> p(DBL_MAX, DBL_MAX);
   for (size_t i = 0; i < v.size(); ++i) {
      if (p.first > v[i][0] && p.first > 0) p.first = v[i][0];
      if (p.second > v[i][1] && p.second > 0) p.second = v[i][1];
   }
   return p;
}

std::pair<double, double> GetDataMax(const std::vector<Vector_2>& v) {
   std::pair<double, double> p(-DBL_MAX, -DBL_MAX);
   for (size_t i = 0; i < v.size(); ++i) {
      if (p.first < v[i][0]) p.first = v[i][0];
      if (p.second < v[i][1]) p.second = v[i][1];
   }
   return p;
}

std::vector<Vector_2> CreateCentroidList(const size_t nstart, const std::vector<Vector_2>& vdata)
{
   const std::pair<double, double> maxes = GetDataMax(vdata);
   const std::pair<double, double> mins = GetDataMin(vdata);
   std::cout << "x range " << mins.first << " " << maxes.first << std::endl;
   std::cout << "x range " << mins.second << " " << maxes.second << std::endl;

   const double xrange = maxes.first - mins.first;
   const double yrange = maxes.second - mins.second;

   RHrand ran;
   for (size_t i = 0; i < 20; ++i) ran.urand();

   std::vector<Vector_2> out;

   for (size_t i = 0; i < nstart; ++i) {
      const double x = xrange*ran.urand() + mins.first;
      const double y = yrange * ran.urand() + mins.second;
      out.emplace_back(Vector_2(x, y));
      std::cout << "x/y " << x << " " << y << std::endl;
   }

   return out;
}




std::vector<Vector_2> LloydStep::DoLloydStep(
   const std::vector<Vector_2>& centroids, const std::vector<Vector_2>& vdata)
{
   std::vector<Vector_2> centroidList = centroids;
   size_t nstart = centroids.size();
   if (centroids.empty()) {
      nstart = 25;
      centroidList = BuildCentroidList(nstart, vdata);
   }

   const CNearTree<Vector_2> centroidTree(centroidList);
   for (size_t i = 0; i < centroidTree.size(); ++i) {
      std::cout << i << " " << centroidTree[i] << std::endl;
   }

   std::vector<Vector_2> out(nstart,Vector_2(0.,0.));
   std::vector<size_t> counts(nstart);

   std::cout << "tree size " << centroidTree.size() << std::endl;
   for (size_t i = 0; i < vdata.size(); ++i) {
      auto xxx = centroidTree.NearestNeighbor(DBL_MAX, vdata[i]);
      const size_t npos = xxx.get_position();
      counts[npos] += 1;
      out[npos] += vdata[i];
   }

   for (size_t i = 0; i < counts.size(); ++i) {
      out[i] /= double(counts[i]);
      std::cout << " counts " << i << " " << counts[i] << 
         "   " << out[i] << std::endl;
   }


   std::vector<Vector_2> out2;
   for (size_t i = 0; i < counts.size(); ++i) {
      std::cout << " Counts[i] " << counts[i] << std::endl;
      if (counts[i] > 2 && counts[i] > 0.05*vdata.size()) {
         out2.emplace_back(out[i]);
      }
   }

   for (size_t i = 0; i < out2.size(); ++i) {
      std::cout << " outs[i] " << i << " " <<
         "   " << out2[i] << std::endl;
   }

   return out2;
}


std::vector<Vector_2> LloydStep::BuildCentroidList(const size_t n, const std::vector<Vector_2>& vv)
{
   RHrand ran;
   for (size_t i = 0; i < 20; ++i) ran.urand(); // dummy to get urand started

   std::vector<Vector_2> vcentroids;

   for (size_t i = 0; i < std::min(size_t(100), std::min(n,vv.size())); ++i) {
      const size_t index = size_t(ran.urand() * n);
      if ( index <= vv.size()){
         std::cout << "index " << index << std::endl;
         std::cout << "vv[index] " << vv[index] << std::endl << std::endl;
         if (vv[index][0] > 0)
            vcentroids.emplace_back(vv[index]);
      }
   }
   return vcentroids;
}
