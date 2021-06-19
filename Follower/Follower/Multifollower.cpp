#include "MultiFollower.h"

#include <cmath>
#include <vector>

#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "D7Dist.h"
#include "Delone.h"
#include "LRL_MinMaxTools.h"
#include "LRL_Path.h"
#include "NCDist.h"
#include "Niggli.h"
#include "D7.h"
#include "G6.h"
#include "OutlierFinder.h"
#include "S6.h"
#include "S6Dist.h"
#include "V7Dist.h"

MultiFollower::MultiFollower(const std::vector<std::pair<S6, S6> > & inputPath)
   //: m_s6path(inputPath)
   //, m_g6path(Converter<G6, std::vector<std::pair<G6, G6> >, Niggli>(inputPath))
   //, m_d7path(Converter<D7, std::vector<std::pair<D7, D7> >, Delone>(inputPath))
   //, m_cspath(inputPath)
   //, m_v7path( Converter<G6, std::vector<std::pair<G6, G6> >, Niggli>( inputPath ))
   //, m_lmpath( inputPath )
   //, m_dcpath( Converter<DC, std::vector<std::pair<DC, DC> >, Niggli>( inputPath))
   //, m_d13path(Converter<D13, std::vector<std::pair<D13, D13> >, Niggli>(inputPath))

{
                                          m_s6path = m_SellingReducedPath; // always generate an S6 path !!!
   if (FollowerConstants::IsEnabled("G6"))m_g6path = m_NiggiReducedPath;
   if (FollowerConstants::IsEnabled("CS"))m_cspath = m_SellingReducedPath;
   if (FollowerConstants::IsEnabled("LM"))m_lmpath = m_SellingReducedPath;
   if (FollowerConstants::IsEnabled("V7"))m_v7path = m_NiggiReducedPath;
   if (FollowerConstants::IsEnabled("DC"))m_dcpath = Converter<DC, std::vector<std::pair<DC, DC> >, Niggli>(inputPath);
   if (FollowerConstants::IsEnabled("DC"))m_dcpath = m_DCPath;
   //if (FollowerConstants::IsEnabled("DC13"))m_d13path = m_NiggiReducedPath;
}

LRL_Path<S6> MultiFollower::GetS6(void) const {
   return m_s6path;
}

LRL_Path<G6> MultiFollower::GetG6(void) const {
   return m_g6path;
}

LRL_Path<D7> MultiFollower::GetD7(void) const {
   return m_d7path;
}

LRL_Path<S6> MultiFollower::GetCS(void) const {
   return m_cspath;
}

LRL_Path<G6> MultiFollower::GetV7( void ) const {
   return m_v7path;
}

LRL_Path<S6> MultiFollower::GetLM(void) const {
   return m_lmpath;
}

LRL_Path<DC> MultiFollower::GetDC(void) const {
   return m_dcpath;
}

//LRL_Path<D13> MultiFollower::GetD13(void) const {
//   return m_d13path;
//}

MultiFollower MultiFollower::CalculateDistancesS6(const MultiFollower& mf) const {
   MultiFollower m(mf);
   S6 out;
   std::vector<double> vdist;
   const std::vector<std::pair<S6, S6> >& path(m_SellingReducedPath);
   //const std::vector<std::pair<S6, S6> > secondPath(mf.GetS6().GetSecondPath());
   S6Dist s6dist( 1 );
   if (!S6::IsInvalidPair( path[0] )) {
      for (size_t i = 0; i < path.size( ); ++i) {
         double distance = s6dist.DistanceBetween( path[i].first, path[i].second );
         if (S6::IsInvalidPair( path[i] )) distance = -1.0;
         vdist.push_back( distance );
      }
   }
   m.SetDistancesS6( vdist );
   return m;
}

MultiFollower MultiFollower::CalculateDistancesG6(const MultiFollower& mf) const {
   MultiFollower m(mf);
   G6 out;

   std::vector<double> vdist;
   const std::vector<std::pair<G6, G6> >& path( m_NiggiReducedPath );
   for (size_t i = 0; i < path.size( ); ++i) {
      double distance = NCDist( path[i].first.data( ), path[i].second.data( ) );
      if (S6::IsInvalidPair( path[i] )) distance = -1.0;
      vdist.push_back( distance );
   }
   m.SetDistancesG6( vdist );
   return m;
}

MultiFollower MultiFollower::CalculateDistancesV7(const MultiFollower& mf) const {
   MultiFollower m( mf );
   G6 out;
   std::vector<double> vdist;
   const std::vector<std::pair<G6, G6> > path( mf.GetG6( ).GetPath( ) );
   for (size_t i = 0; i < path.size( ); ++i) {

      const G6 firstPoint = (m_latticePointChoiceForDistanceCalculation == versusFirstPoint) ?
         path[0].second : path[i].first;

      double distance = V7Dist( firstPoint, path[i].second );
      if (S6::IsInvalidPair( path[i] )) distance = -1.0;
      vdist.push_back( distance );
   }
   m.SetDistancesV7( vdist );
   return m;
}

MultiFollower MultiFollower::CalculateDistancesD7(const MultiFollower& mf) const {
   MultiFollower m(mf);
   D7 out;
   std::vector<double> vdist;
   const std::vector<std::pair<D7, D7> > path( mf.GetD7( ).GetPath( ) );
   for (size_t i = 0; i < path.size( ); ++i) {
      double distance = D7Dist( path[i].first.data( ), path[i].second.data( ) );
      if (S6::IsInvalidPair( path[i] )) distance = -1.0;
      vdist.push_back( distance );
   }
   m.SetDistancesD7( vdist );
   return m;
}

MultiFollower MultiFollower::CalculateDistancesCS( const MultiFollower& mf ) const {
   MultiFollower m( mf );
   S6 out;
   std::vector<double> vdist;
   const std::vector<std::pair<S6, S6> > path( m_SellingReducedPath);
   for (size_t i = 0; i < path.size( ); ++i) {
      double distance = CS6Dist( path[i].first.data( ), path[i].second.data( ) );
      if (S6::IsInvalidPair( path[i] )) distance = -1.0;
      vdist.push_back( distance );
   }
   m.SetDistancesCS( vdist );
   return m;
}

void CheckTrees( const CNearTree<MatS6>& treea, const CNearTree<MatS6>& treeb) {
   std::cout << "in CheckTrees tree sizes " << treea.size() << "  " << treeb.size() << std::endl;
   if (treea.size() != treeb.size()) throw;
   for (size_t i = 0; i < treea.size(); ++i)
      if (treeb.NearestNeighbor(0.1, treea[i]) == treeb.end()) throw;
}

MultiFollower MultiFollower::CalculateDistancesLM( const MultiFollower& mf ) const {
   MultiFollower m( mf );
   S6 out;

   LM lm(1, 3.1);
   {
   }


   std::vector<double> vdist;
   const std::vector<std::pair<S6, S6> > path( mf.GetS6( ).GetPath( ) );
   S6Dist s6dist( 1 );
   for (size_t i = 0; i < path.size( ); ++i) {
      const double distance1 = (S6::IsInvalidPair(path[i]) || !path[i].second.IsValid()) ? -1.0 :
         lm.DistanceBetween(path[0].second, path[i].second);
      //const double distance2 = (S6::IsInvalidPair(path[i]) || !path[i].second.IsValid()) ? -1.0 :
      //   lm.DistanceBetween(path[i].second, path[0].second);
      ////std::cout << i << " CalculateDistancesLM " << lm.GetBestMatch() << std::endl;
      //vdist.push_back(std::min(distance1, distance2));
      vdist.push_back(distance1);
   }
   m.SetDistancesLM( vdist );
   return m;
}

MultiFollower MultiFollower::CalculateDistancesDC(const MultiFollower& mf) const {
   MultiFollower m(mf);
   S6 out;
   std::vector<double> vdist;
   const std::vector<std::pair<S6, S6> >& path = m_SellingReducedPath;
   S6Dist s6dist(1);
   if (!S6::IsInvalidPair(path[0])) {
      for (size_t i = 0; i < path.size(); ++i) {
         double distance = -19191.0;
         if (FollowerConstants::globalFollowerMode == FollowerConstants::globalSinglePoint) distance = DC().DistanceBetween(DC(path[i].first), DC(path[i].second));
         if (FollowerConstants::globalFollowerMode == FollowerConstants::globalChord) distance = DC().DistanceBetween(DC(path[i].first), DC(path[i].second));
         if (FollowerConstants::globalFollowerMode == FollowerConstants::globalLine) distance = DC().DistanceBetween(DC(path[i].first), DC(path[i].second));
         if (FollowerConstants::globalFollowerMode == FollowerConstants::globalChord3) distance = DC().DistanceBetween(DC(path[i].first), DC(path[i].second));
         if (FollowerConstants::globalFollowerMode == FollowerConstants::globalTriangle) distance = DC().DistanceBetween(DC(path[i].first), DC(path[i].second));
         vdist.push_back(distance);
      }
   }
   m.SetDistancesDC(vdist);
   return m;
}

MultiFollower MultiFollower::CalculateDistancesD13(const MultiFollower& mf) const {
   MultiFollower m(mf);
   throw;
   return m;
}

void MultiFollower::SetLatticePointChoiceForDistanceCalculation() {
   m_latticePointChoiceForDistanceCalculation = (
      FollowerConstants::globalFollowerMode == FollowerConstants::globalChord ||
      FollowerConstants::globalFollowerMode == FollowerConstants::globalSinglePoint) ?
      versusFirstPoint : versusCorrespondingPoint;
}

MultiFollower MultiFollower::GenerateAllDistances(void) {
   MultiFollower m(*this);
   if (FollowerConstants::IsEnabled("S6")) m.m_s6path = m_SellingReducedPath;
   if (FollowerConstants::IsEnabled("G6")) m.m_g6path = m_NiggiReducedPath;
   if (FollowerConstants::IsEnabled("CS")) m.m_cspath = m_SellingReducedPath;
   if (FollowerConstants::IsEnabled("LM")) m.m_lmpath = m_SellingReducedPath;
   if (FollowerConstants::IsEnabled("V7")) m.m_v7path = m_NiggiReducedPath;
   if (FollowerConstants::IsEnabled("DC")) m.m_dcpath = Converter<DC, std::vector<std::pair<DC, DC> >, Niggli>(m_SellingReducedPath);
   if (FollowerConstants::IsEnabled("DC")) m.m_dcpath = m_DCPath;


   SetLatticePointChoiceForDistanceCalculation();
   m.SetLatticePointChoiceForDistanceCalculation();

   m.SetComputeStartTime();

   {
      m.m_v7path.SetComputeStartTime();
      if (FollowerConstants::IsEnabled("V7")) m = CalculateDistancesV7(m);
      const double computetime = std::clock() - m.m_v7path.GetComputeStartTime();
      m.m_v7path.SetTime2ComputeFrame(computetime);
      m.SetComputeTime("V7", computetime);
   }
   {
      m.m_cspath.SetComputeStartTime();
      if (FollowerConstants::IsEnabled("CS")) m = CalculateDistancesCS(m);
      const double computetime = std::clock() - m.m_cspath.GetComputeStartTime();
      m.m_cspath.SetTime2ComputeFrame(computetime);
      m.SetComputeTime("CS", computetime);
   }
   {
      m.m_s6path.SetComputeStartTime();
      if (FollowerConstants::IsEnabled("S6")) m = CalculateDistancesS6(m);
      const double computetime = std::clock() - m.m_s6path.GetComputeStartTime();
      m.m_s6path.SetTime2ComputeFrame(computetime);
      m.SetComputeTime("S6", computetime);
   }
   {
      m.m_g6path.SetComputeStartTime();
      if (FollowerConstants::IsEnabled("G6")) m = CalculateDistancesG6(m);
      const double computetime = std::clock() - m.m_g6path.GetComputeStartTime();
      m.m_g6path.SetTime2ComputeFrame(computetime);
      m.SetComputeTime("G6", computetime);
   }
   {
      m.m_d7path.SetComputeStartTime();
      if (FollowerConstants::IsEnabled("D7")) m = CalculateDistancesD7(m);
      const double computetime = std::clock() - m.m_d7path.GetComputeStartTime();
      m.m_d7path.SetTime2ComputeFrame(computetime);
      m.SetComputeTime("D7", computetime);
   }
   {
      m.m_lmpath.SetComputeStartTime();
      if (FollowerConstants::IsEnabled("LM")) m = CalculateDistancesLM(m);
      const double computetime = std::clock() - m.m_lmpath.GetComputeStartTime();
      m.m_lmpath.SetTime2ComputeFrame(computetime);
      m.SetComputeTime("LM", computetime);
   }
   {
      m.m_dcpath.SetComputeStartTime();
      if (FollowerConstants::IsEnabled("DC")) m = CalculateDistancesDC(m);
      const double computetime = std::clock() - m.m_dcpath.GetComputeStartTime();
      m.m_dcpath.SetTime2ComputeFrame(computetime);
      m.SetComputeTime("DC", computetime);
   }
   //{
   //   m.m_d13path.SetComputeStartTime();
   //   if (FollowerConstants::IsEnabled("D13")) m = CalculateDistancesD13(m);
   //   const double computetime = std::clock() - m.m_d13path.GetComputeStartTime();
   //   m.m_d13path.SetTime2ComputeFrame(computetime);
   //   m.SetComputeTime("D13", computetime);
   //}

   m.SetTime2ComputeFrame(std::clock() - m.GetComputeStartTime());

   m.GetPathS6( ).SetGlitches( m.DetermineOutliers( m.GetPathS6( ).GetDistances( ) ) );
   m.GetPathG6( ).SetGlitches( m.DetermineOutliers( m.GetPathG6( ).GetDistances( ) ) );
   m.GetPathD7( ).SetGlitches( m.DetermineOutliers( m.GetPathD7( ).GetDistances( ) ) );
   m.GetPathCS( ).SetGlitches( m.DetermineOutliers( m.GetPathCS( ).GetDistances( ) ) );
   m.GetPathV7( ).SetGlitches( m.DetermineOutliers( m.GetPathV7( ).GetDistances( ) ) );
   m.GetPathLM( ).SetGlitches( m.DetermineOutliers( m.GetPathLM( ).GetDistances( ) ) );
   m.GetPathDC().SetGlitches(m.DetermineOutliers(m.GetPathDC().GetDistances()));
   //m.GetPathD13().SetGlitches(m.DetermineOutliers(m.GetPathD13().GetDistances()));

   return m;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::set<size_t> MultiFollower::DetermineIfSomeDeltaIsTooLarge(const std::vector<double>& distances) const
/*-------------------------------------------------------------------------------------*/
{
   const double maximumDistance = *std::max_element(distances.begin(), distances.end());
   std::set<size_t> glitches(DetermineOutliers(distances));

   return(glitches);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::set<size_t> MultiFollower::DetermineOutliers(const std::vector<double> distanceList) const
/*-------------------------------------------------------------------------------------*/
{
   std::set<size_t> glitches;
   if (distanceList.empty()) return glitches;
   OutlierFinder of(distanceList);
   std::vector<std::pair<double, double> > steps = of.FindDiscontinuities(FollowerConstants::globalPercentChangeToDetect);

   for (size_t i = 0; i < steps.size(); ++i) {
      glitches.insert(size_t(steps[i].first) + 1UL);
   }
   return glitches;
}

template<typename T>
std::pair<double, double> GetPathMinMax(const LRL_Path<T>& path) {
   std::pair<double, double> p(std::make_pair(DBL_MAX, 0));
   const std::vector<double> pdist = path.GetDistances( );
   if (!path.GetDistances().empty()) {
      p = std::make_pair(path.GetMin(), path.GetMax());
      if (p.first > 0.0) {
         const std::vector<double> distances = path.GetDistances();
         double distmin = DBL_MAX;
         for (size_t i = 0; i < distances.size(); ++i) {
            if (distances[i] >= 0.0)
               distmin = std::min(distmin, distances[i]);
         }
         p.first = distmin;
      }
   }
   return p;
}

std::pair<double, double> MultiFollower::GetMinMax(void) const {
   std::pair<double, double> p;
   const std::pair<double, double> pS6 = GetPathMinMax(m_s6path);
   const std::pair<double, double> pG6 = GetPathMinMax(m_g6path);
   const std::pair<double, double> pD7 = GetPathMinMax(m_d7path);
   const std::pair<double, double> pCS = GetPathMinMax(m_cspath);
   const std::pair<double, double> pV7 = GetPathMinMax( m_v7path );
   const std::pair<double, double> pLM = GetPathMinMax(m_lmpath);
   const std::pair<double, double> pDC = GetPathMinMax(m_dcpath);
   //const std::pair<double, double> pD13 = GetPathMinMax(m_d13path);

   const double minp = std::min(minNC(pS6.first, pG6.first, pD7.first, pCS.first, pV7.first, pLM.first), 
      pDC.first);
   const double maxp = std::max(maxNC(pS6.second, pG6.second, pD7.second, pCS.second, pV7.second, pLM.second),
     pDC.second);
   return std::make_pair(minp, maxp);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const double MultiFollower::GetTime2ComputerFrame(void) const {
   return(m_seconds2ComputerFrame);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void MultiFollower::SetTime2ComputeFrame(const double computeTime) {
   m_seconds2ComputerFrame = computeTime;
}

bool MultiFollower::HasGlitches(void) const {
   return m_cspath.HasGlitches() || m_d7path.HasGlitches() || m_s6path.HasGlitches() || m_g6path.HasGlitches() || m_lmpath.HasGlitches();
}

void MultiFollower::SetComputeTime(const std::string& name, const double time) {
   m_lineDescription.SetComputeTime(name, time);
}

void MultiFollower::SetInputVectors(const std::vector<S6>& v) {
   m_inputVectors.clear();
   for (size_t i = 0; i < v.size(); ++i) {
      m_inputVectors += LRL_ToString(v[i], '\n');;
   }
}