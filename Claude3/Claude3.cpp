#include <algorithm>
#include <cmath>
#include <limits>
#include <sstream>

#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "S6.h"
#include "G6.h"
#include "MatS6.h"
#include "LRL_ReadLatticeData.h"
#include "S6BoundaryTransforms.h"

class CS6Dist_C {
public:
   CS6Dist_C()
      : m_debugOutput(false)
   {}

   double DistanceBetween(const S6& s6vec1, const S6& s6vec2) const {
      double dist = calculateReflectionDistance(s6vec1, s6vec2);

      if (m_debugOutput) {
         std::cout << "\nStarting vectors:" << std::endl;
         std::cout << "v1: " << s6vec1 << std::endl;
         std::cout << "v2: " << s6vec2 << std::endl;
         std::cout << "Initial distance: " << dist << std::endl;
      }

      // Try single boundary paths
      for (size_t boundary = 0; boundary < 6; ++boundary) {
         const double newDist = processTMB(s6vec1, s6vec2, boundary, dist, true);
         if (newDist < dist) {
            dist = newDist;
         }
      }

      // Try double boundary paths
      for (size_t firstBoundary = 0; firstBoundary < 6; ++firstBoundary) {
         const double newDist = processTMB(s6vec1, s6vec2, firstBoundary, dist, false);
         if (newDist < dist) {
            dist = newDist;
         }
      }

      return dist;
   }

   double DistanceInG6(const G6& g6vec1, const G6& g6vec2) const {
      return DistanceBetween(S6(g6vec1), S6(g6vec2));
   }

   void enableDebugOutput(bool enable) noexcept { m_debugOutput = enable; }
   [[nodiscard]] bool isDebugEnabled() const noexcept { return m_debugOutput; }

private:
   const std::vector<MatS6> m_reflections = MatS6::GetReflections();
   bool m_debugOutput;

   double calculateReflectionDistance(const S6& v1, const S6& v2) const {
      double minDist = (v1 - v2).Norm();

      for (const auto& refl : m_reflections) {
         const S6 reflected = refl * v2;
         const double dist = (v1 - reflected).Norm();
         minDist = std::min(minDist, dist);
      }

      return minDist;
   }

   static S6 createMirrorPoint(const S6& v1, const S6& v2, size_t boundary) {
      const double alpha = calculateAlpha(v1[boundary], v2[boundary]);
      S6 mirror = v1 + alpha * (v2 - v1);
      mirror[boundary] = 0.0;
      return mirror;
   }

   static double calculateAlpha(double comp1, double comp2) {
      const double dist1 = std::abs(comp1);
      const double dist2 = std::abs(comp2);

      if (dist1 + dist2 > dist1 * 1.0E-10) {
         const double alpha = dist1 / (dist1 + dist2);
         return std::min(std::max(alpha, 0.0), 1.0);
      }
      return 0.0;
   }

   double processPathThroughMirrorPoints(const S6& s1, const S6& s2,
      const S6& firstMirror, const S6& secondMirror,
      bool isSingleBoundary) const {
      double bestPath = std::numeric_limits<double>::max();

      for (const auto& refl : m_reflections) {
         const S6 reflectedMirror = refl * (isSingleBoundary ? firstMirror : secondMirror);
         const double pathDist = isSingleBoundary ?
            (s1 - reflectedMirror).Norm() + (reflectedMirror - s2).Norm() :
            (s1 - firstMirror).Norm() + (firstMirror - reflectedMirror).Norm() +
            (reflectedMirror - s2).Norm();

         bestPath = std::min(bestPath, pathDist);
      }
      return bestPath;
   }

   double processTMB(const S6& s1, const S6& s2,
      size_t firstBoundary,
      double currentDist,
      bool singleBoundaryOnly) const {
      const S6 firstMirror = createMirrorPoint(s1, s2, firstBoundary);
      double minDist = currentDist;
      if (singleBoundaryOnly)
      {
         minDist = tryPathWithMirrors(s1, s2, firstMirror, firstMirror,
            firstBoundary, firstBoundary, minDist);
      }
      else
      {
         for (size_t secondBoundary = 0; secondBoundary < 6; ++secondBoundary) {
            if ( secondBoundary != firstBoundary ) {
               const S6 secondMirror = createMirrorPoint(firstMirror, s2, secondBoundary);
               minDist = tryPathWithMirrors(s1, s2, firstMirror, secondMirror,
                  firstBoundary, secondBoundary, minDist);
            }
         }
      }

      return minDist;
   }


   double tryPathWithMirrors(const S6& s1, const S6& s2,
      const S6& mirror1, const S6& mirror2,
      size_t boundary1, size_t boundary2,
      double currentDist) const {
      const double pathDist = processPathThroughMirrorPoints(s1, s2, mirror1,
         mirror2,
         boundary1 == boundary2);
      if (pathDist < currentDist && m_debugOutput) {
         if (boundary1 == boundary2) {
            std::cout << "Better single boundary path at " << boundary1
               << ": " << pathDist << std::endl;
         }
         else {
            std::cout << "Better two-boundary path through " << boundary1
               << " and " << boundary2 << ": " << pathDist << std::endl;
         }
      }
      return std::min(currentDist, pathDist);
   }


}; // end of CS6Dist_C



static void PrintArrayOfResults(const std::vector<std::vector<std::string>>& arr) {
   std::cout << std::endl << "output arr" << std::endl;

   for (const auto& oa : arr) {
      std::string oa1 = oa[0].c_str();
      std::string oa2 = oa[1].c_str();
      std::string oa3 = oa[2].c_str();
      oa1.resize(36, ' ');
      oa2.resize(36, ' ');
      oa3.resize(36, ' ');
      if (oa1.empty() && oa2.empty() && oa3.empty())
      {
         break;
      }

      std::cout << oa1;
      std::cout << "  " << oa2;
      std::cout << "  " << oa3;
      std::cout << std::endl;
   }
}

static void FillPrintArray(std::vector<std::vector<std::string>>& arr, const double d_Claude, const double d_CS6Dist,
   const std::pair<S6, S6>& tests) {
      std::ostringstream os;
   os << "\t CS6Dist_C " << d_Claude << "  CS6Dist " << d_CS6Dist;
   std::string o = os.str();
   std::cout << "S6 " << tests.first << std::endl;
   std::cout << "S6 " << tests.second << std::endl;
   std::cout << o << std::endl;
   o.resize(36, ' ');

   int pos = 0;
   const double diff = (d_Claude - d_CS6Dist) / d_Claude;
   if ((std::abs(diff) < 1.0E-2) || abs(d_Claude + d_CS6Dist) < 1.0e-4) pos = 1;
   else if (diff < 0)  pos = 2;

   // look for a blank space in arr
   for (auto& ar : arr) {
      if (ar[pos].empty()) 
      {
         ar[pos] = o;
         break;
      }
   }
}

int main() {

   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   S6 origin;
   Selling::Reduce(inputList[0].GetCell(), origin);

   //const int ntest = 20;

   const size_t ntest = inputList.size() - 1;
   std::vector<std::vector<std::string>> arr(ntest, std::vector<std::string>(3));

   std::vector<std::pair<S6, S6>> tests;

   CS6Dist_C cs6d; 
   cs6d.enableDebugOutput(false);
   for ( size_t i=0; i<ntest; ++i )
   {
      cs6d.enableDebugOutput(false);

      //S6 r1 = S6::rand();
      //S6 r2 = S6::rand();
      S6 r1 = inputList[i].GetCell();
      S6 r2 = inputList[i+1].GetCell();
      Selling::Reduce(r1,r1);
      Selling::Reduce(r2,r2);
      const double d_CS6DistA = CS6Dist(r1.data(), r2.data());
      const double d_ClaudeA = cs6d.DistanceBetween(r1, r2);
      const double d_CS6DistB = CS6Dist(origin.data(), r2.data());
      const double d_ClaudeB = cs6d.DistanceBetween(origin, r2);

      FillPrintArray( arr, d_ClaudeB, d_CS6DistB, {r1,r2});
   }

   PrintArrayOfResults(arr);
}