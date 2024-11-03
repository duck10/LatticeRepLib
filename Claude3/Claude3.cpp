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
      : m_boundaryTransforms(S6BoundaryTransforms::generateFlat24BoundaryTransforms()),
      m_debugOutput(false)
   {}

   struct DiagnosticInfo {
      double norm1;
      double norm2;
      double maxComp1;
      double minComp1;
      double maxComp2;
      double minComp2;
      double sumAbs1;
      double sumAbs2;
      int numNearZero1;
      int numNearZero2;
      double maxRatio;  // ratio of corresponding components
      double minRatio;
      double normRatio;
   };

   double DistanceBetween(const S6& s6vec1, const S6& s6vec2) const {
      if (m_debugOutput) {
         printDiagnostics(s6vec1, s6vec2);
      }

      // Initial distance considering reflections
      double dist = calculateReflectionDistance(s6vec1, s6vec2);

      if (m_debugOutput) {
         std::cout << "\nStarting distance: " << dist << std::endl;
      }

      // Try each transformation on each vector
      for (const auto& transform : m_boundaryTransforms) {
         // Transform first vector
         const S6 trans1 = transform * s6vec1;
         const double dist1 = calculateReflectionDistance(trans1, s6vec2);
         if (dist1 < dist) {
            dist = dist1;
            if (m_debugOutput) {
               std::cout << "Better distance after transform v1: " << dist << std::endl;
               printDiagnostics(trans1, s6vec2);
            }
         }

         // Transform second vector
         const S6 trans2 = transform * s6vec2;
         const double dist2 = calculateReflectionDistance(s6vec1, trans2);
         if (dist2 < dist) {
            dist = dist2;
            if (m_debugOutput) {
               std::cout << "Better distance after transform v2: " << dist << std::endl;
               printDiagnostics(s6vec1, trans2);
            }
         }

         // Try transforming both vectors
         const double dist3 = calculateReflectionDistance(trans1, trans2);
         if (dist3 < dist) {
            dist = dist3;
            if (m_debugOutput) {
               std::cout << "Better distance after transform both: " << dist << std::endl;
               printDiagnostics(trans1, trans2);
            }
         }
      }

      return dist;
   }

private:
   const std::vector<MatS6> m_boundaryTransforms;
   bool m_debugOutput;

   DiagnosticInfo calculateDiagnostics(const S6& v1, const S6& v2) const {
      DiagnosticInfo info;
      constexpr double nearZeroThreshold = 1.0E-10;

      // Calculate norms and related values
      info.norm1 = v1.Norm();
      info.norm2 = v2.Norm();
      info.normRatio = info.norm1 / info.norm2;

      // Initialize component stats
      info.maxComp1 = std::numeric_limits<double>::lowest();
      info.minComp1 = std::numeric_limits<double>::max();
      info.maxComp2 = std::numeric_limits<double>::lowest();
      info.minComp2 = std::numeric_limits<double>::max();
      info.sumAbs1 = 0.0;
      info.sumAbs2 = 0.0;
      info.numNearZero1 = 0;
      info.numNearZero2 = 0;
      info.maxRatio = -std::numeric_limits<double>::max();
      info.minRatio = std::numeric_limits<double>::max();

      // Calculate component-wise statistics
      for (size_t i = 0; i < 6; ++i) {
         const double abs1 = std::abs(v1[i]);
         const double abs2 = std::abs(v2[i]);

         info.sumAbs1 += abs1;
         info.sumAbs2 += abs2;

         info.maxComp1 = std::max(info.maxComp1, abs1);
         info.minComp1 = std::min(info.minComp1, abs1);
         info.maxComp2 = std::max(info.maxComp2, abs2);
         info.minComp2 = std::min(info.minComp2, abs2);

         if (abs1 < nearZeroThreshold) ++info.numNearZero1;
         if (abs2 < nearZeroThreshold) ++info.numNearZero2;

         if (abs2 > nearZeroThreshold) {
            const double ratio = abs1 / abs2;
            info.maxRatio = std::max(info.maxRatio, ratio);
            info.minRatio = std::min(info.minRatio, ratio);
         }
      }

      return info;
   }

   void printDiagnostics(const S6& v1, const S6& v2) const {
      auto info = calculateDiagnostics(v1, v2);

      std::cout << "\nDiagnostic Information:" << std::endl;
      std::cout << "Vector 1: " << v1 << std::endl;
      std::cout << "Vector 2: " << v2 << std::endl;
      std::cout << "Norms: " << info.norm1 << ", " << info.norm2
         << " (ratio: " << info.normRatio << ")" << std::endl;
      std::cout << "Max components: " << info.maxComp1 << ", " << info.maxComp2 << std::endl;
      std::cout << "Min components: " << info.minComp1 << ", " << info.minComp2 << std::endl;
      std::cout << "Sum of absolutes: " << info.sumAbs1 << ", " << info.sumAbs2 << std::endl;
      std::cout << "Near-zero components: " << info.numNearZero1
         << ", " << info.numNearZero2 << std::endl;
      std::cout << "Component ratios: " << info.minRatio
         << " to " << info.maxRatio << std::endl;
      std::cout << std::endl;
   }

   double calculateReflectionDistance(const S6& v1, const S6& v2) const {
      double minDist = (v1 - v2).Norm();

      static const auto& reflections = MatS6::GetReflections();
      for (const auto& refl : reflections) {
         const S6 transformed = refl * v2;
         const double dist = (v1 - transformed).Norm();
         minDist = std::min(minDist, dist);
      }

      return minDist;
   }

public:
   void enableDebugOutput(bool enable) noexcept { m_debugOutput = enable; }
   [[nodiscard]] bool isDebugEnabled() const noexcept { return m_debugOutput; }
};




int main() {

   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   //const int ntest = 20;
   const int ntest = inputList.size()-1;
   std::vector<std::string> line(3);
   std::vector < std::vector<std::string>> arr;
   for (size_t i = 0; i < ntest; ++i) {
      arr.emplace_back(line);
   }

   std::vector<std::pair<S6, S6>> tests;

   CS6Dist_C cs6d; 
   cs6d.enableDebugOutput(false);
   for ( size_t i=0; i<ntest; ++i )
   {
      cs6d.enableDebugOutput(false);

      //S6 r1 = S6::rand();
      //S6 r2 = S6::rand();
      const S6 r1 = inputList[i].GetCell();
      const S6 r2 = inputList[i+1].GetCell();
      tests.emplace_back(std::make_pair(r1,r2));
      const double dddd = CS6Dist(r1.data(), r2.data());
       
      std::string blanks;
      if ((abs(dddd - 233.076) < .01 || (abs(dddd - 242.339) < .01)))
      {
         cs6d.enableDebugOutput(false);

         std::cout << "S6 " << tests[i].first << std::endl;
         std::cout << "S6 " << tests[i].second << std::endl;
         std::cout << " CS6Dist = " << dddd << std::endl;
      }
      const double d = cs6d.DistanceBetween(r1, r2);
      const double diff = (d - dddd) / d;

      std::ostringstream os;
      os <</* blanks << */"  CS6Dist_C " << d << "  CS6Dist " << dddd;
      //if ((abs(dddd - 38.9353) < .001 || (abs(dddd - 57.7191) < .001)))
         //{
      //   std::cout <</* blanks << */"  CS6Dist_C " << d << "  CS6Dist " << dddd;
      //}
      const std::string o = os.str();
      std::cout << "S6 " << tests[i].first << std::endl;
      std::cout << "S6 " << tests[i].second << std::endl;
      std::cout << o << std::endl;

      int pos = 0;
      if ((std::abs(diff) < 1.0E-2) || abs(d+dddd) < 1.0e-4) pos = 1;
      else if (diff < 0)  pos = 2;

      // look for a blank space in arr
      for (size_t i = 0; i < ntest; ++i) {
         if (arr[i][pos].empty()) {
            arr[i][pos] = o;
            arr[i][pos].resize(36, ' ');
            break;
         }
      }
   }

   std::cout << "output arr" << std::endl;

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