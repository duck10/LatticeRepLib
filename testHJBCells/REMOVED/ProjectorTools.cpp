#include <list>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>

//#include "NC_Boundary.h"
#include "D7.h"
#include "G6.h"
#include "Mat66.h"
#include "ProjectorTools.h"

//mat66 ProjectorTools::ConvertStringToPerp(const std::string& s) {
//   G6 v(s);
//   return v.VV_Transpose() / G6::dot(v, v);
//}
//
//mat66 ProjectorTools::ConvertStringToProjector(const std::string& s) {
//   G6 v(s);
//   return mat66::Eye() - v.VV_Transpose() / G6::dot(v, v);
//}
//
//ProjectorTools::ProjectorTools(void) {
//   InitCleanupList();
//}
//
//ProjectorTools::~ProjectorTools(void) {
//   m_vCleanupList.clear();
//}

//-----------------------------------------------------------------------------
// Name: InitCleanupList()
// Description: make a list of a lot of rational numbers this will be used 
//              to "clean up" the iterated projectors to correct for 
//              floating point approximations (if any)
//
//              NOTE: if you expand the size of this list, be careful, because
//              you may need to decrease the hard-coded constant 1.0E-4 in
//              function CleanupMatrix_SetNearZeroToZero
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
size_t ProjectorTools::InitCleanupList(void) {
   const long MAXDENOMINATOR = 30L;
   m_vCleanupList.push_back(0.0);

   for (int inumerator = 1; inumerator <= MAXDENOMINATOR - 1; ++inumerator) {
      for (int idenominator = inumerator + 1; idenominator <= MAXDENOMINATOR;
            ++idenominator) {
         m_vCleanupList.push_back(double(inumerator) / double(idenominator));
         m_vCleanupList.push_back(-double(inumerator) / double(idenominator));
      }
   }

   return (m_vCleanupList.size());
}

//-----------------------------------------------------------------------------
// Name: CleanupMatrix_SetNearZeroToZero()
// Description: make a list of a lot of rational numbers this will be used 
//              to "clean up" the iterated projectors to correct for 
//              floating point approximations (if any)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//mat66 ProjectorTools::CleanupMatrix_SetNearZeroToZero(
//      const mat66& projectorInput) const {
//   mat66 projector(projectorInput);
//   const size_t n = m_vCleanupList.size();
//   for (int iproj = 0; iproj < 36; ++iproj) {
//      for( size_t i = 0; i < n; ++i) {
//         if (fabs(projector[iproj] - m_vCleanupList[i]) < 1.0E-4) {
//            projector[iproj] = m_vCleanupList[i];
//            break;
//         }
//      }
//   }
//   return projector;
//}
//
///*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//double ProjectorTools::MatrixNorm(const mat66& m) {
//   double d = 0.0;
//   for (int i = 0; i < 36; ++i)
//      d += m[i] * m[i];
//   return d;
//}

///*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//mat66 ProjectorTools::ComputeProjectorFromVector(const G6& v) {
//   const double dotProduct = G6::dot(v, v);
//   const mat66 m = v.VV_Transpose();
//   return mat66::Eye() - m / dotProduct;
//}
//
///*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//std::vector<mat66> ProjectorTools::GeneratePrincipalProjectors(
//      const std::vector<mat66>& perps) {
//   std::vector<mat66> projectors;
//   for( size_t i = 0; i < perps.size(); ++i)
//      projectors.push_back(mat66::Eye() - perps[i]);
//   return projectors;
//}
//
//double ProjectorTools::MinimumDistanceToBoundary(const G6& v) {
//   // this is NCDist function "minbddist"
//   return minNC(
//         minNC(v[1] - v[0], v[2] - v[1], v[2] - v[0])
//               / std::sqrt(2.0),
//
//         minNC(std::fabs(v[3]), std::fabs(v[4]), std::fabs(v[5])),
//
//         minNC(std::fabs(v[0] - std::fabs(v[4])),
//               std::fabs(v[0] - std::fabs(v[5])),
//               std::fabs(v[1] - std::fabs(v[3])),
//               std::fabs(v[1] - std::fabs(v[5])),
//               std::fabs(v[2] - std::fabs(v[3])),
//               std::fabs(v[2] - std::fabs(v[4]))) / std::sqrt(2.0),
//
//         minNC(std::fabs(v[0] + v[1] + v[3] + v[4] + v[5]),
//               std::fabs(v[0] + v[1] - v[3] - v[4] + v[5]),
//               std::fabs(v[0] + v[1] - v[3] + v[4] - v[5]),
//               std::fabs(v[0] + v[1] + v[3] - v[4] - v[5]))
//               / std::sqrt(5.0));
//}

bool ProjectorTools::BothEssentiallySameSign(const G6& v1,
      const G6& v2) {
   return (ProjectorTools::EssentiallyPlus(v1)
         && ProjectorTools::EssentiallyPlus(v2))
         || (ProjectorTools::EssentiallyNegative(v1)
               && ProjectorTools::EssentiallyNegative(v2));
}

G6 ProjectorTools::ConvertArrayToG6(const double a[6]) {
   G6 v;
   v[0] = a[0];
   v[1] = a[1];
   v[2] = a[2];
   v[3] = a[3];
   v[4] = a[4];
   v[5] = a[5];
   return v;
}

void ProjectorTools::ConvertG6ToArray(const G6 & v, double a[6]) {
   a[0] = v[0];
   a[1] = v[1];
   a[2] = v[2];
   a[3] = v[3];
   a[4] = v[4];
   a[5] = v[5];
}

D7 ProjectorTools::ConvertArrayToD7(const double a[]) {
   D7 v;
   v[0] = a[0];
   v[1] = a[1];
   v[2] = a[2];
   v[3] = a[3];
   v[4] = a[4];
   v[5] = a[5];
   v[6] = a[6];
   return v;
}

void ProjectorTools::ConvertD7ToArray(const D7 & v, double a[7]) {
   a[0] = v[0];
   a[1] = v[1];
   a[2] = v[2];
   a[3] = v[3];
   a[4] = v[4];
   a[5] = v[5];
   a[6] = v[6];
}

//double ProjectorTools::DistanceToClosestBoundary( const G6& v, std::vector<NC_Boundary>& bounds ) {
//   double minD = 0.0;
//   for( size_t i=0; i<bounds.size(); ++i ) minD = std::min(minD, bounds[i].Distance( v ));
//    return minD;
//}
//
//std::vector<NC_Boundary> ProjectorTools::CreateBoundaryList( void ) {
//   std::vector<NC_Boundary> b;
//   b.push_back( NC_Boundary() );
//   b.push_back( NC_1() );
//
//   return b;
//}
