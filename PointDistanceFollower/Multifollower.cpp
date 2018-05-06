#include "MultiFollower.h"

#include <vector>

#include "D7Dist.h"
#include "Delone.h"
#include "LRL_MinMaxTools.h"
#include "LRL_Path.h"
#include "NCDist.h"
#include "Niggli.h"
#include "D7.h"
#include "G6.h"
#include "S6.h"
#include "S6Dist.h"

MultiFollower::MultiFollower(const std::vector<std::pair<S6, S6> > & inputPath)
   : m_s6path(inputPath)
   , m_g6path (Converter<G6, std::vector<std::pair<G6, G6> >, Niggli>(inputPath))
   , m_d7path (Converter<D7, std::vector<std::pair<D7, D7> >, Delone>(inputPath))
   , m_cspath (inputPath)

{
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


MultiFollower MultiFollower::CalculateDistancesS6(const MultiFollower& mf) {
   MultiFollower m(mf);
   S6 out;
   std::vector<double> vdist;
   const std::vector<std::pair<S6, S6> > path(mf.GetS6().GetPath());
   S6Dist s6dist(1);
   for ( unsigned long i=0; i<path.size(); ++i ){
     vdist.push_back( s6dist.DistanceBetween(path[0].second, path[i].second));
   }
   m.SetDistancesS6(vdist);
   return m;
}

MultiFollower MultiFollower::CalculateDistancesG6(const MultiFollower& mf) {
   MultiFollower m(mf);
   G6 out;
   std::vector<double> vdist;
   const std::vector<std::pair<G6, G6> > path(mf.GetG6().GetPath());
   for (unsigned long i = 0; i<path.size(); ++i) {
      vdist.push_back(NCDist(path[0].second.data(), path[i].second.data()));
   }
   m.SetDistancesG6(vdist);
   return m;
}

MultiFollower MultiFollower::CalculateDistancesD7(const MultiFollower& mf) {
   MultiFollower m(mf);
   D7 out;
   std::vector<double> vdist;
   const std::vector<std::pair<D7, D7> > path(mf.GetD7().GetPath());
   for (unsigned long i = 0; i<path.size(); ++i) {
      vdist.push_back(D7Dist(path[0].second.data(), path[i].second.data()));
   }
   m.SetDistancesD7(vdist);
   return m;
}


MultiFollower MultiFollower::CalculateDistancesCS(const MultiFollower& mf) {
   MultiFollower m(mf);
   S6 out;
   std::vector<double> vdist;
   const std::vector<std::pair<S6, S6> > path(mf.GetCS().GetPath());
   for (unsigned long i = 0; i<path.size(); ++i) {
      vdist.push_back(CS6Dist(path[0].second.data(), path[i].second.data()));
   }
   m.SetDistancesCS(vdist);
   return m;
}

MultiFollower MultiFollower::GenerateAllDistances(void) {
   MultiFollower m(*this);
   m.SetComputeStartTime();
   m = CalculateDistancesS6(m);
   m = CalculateDistancesG6(m);
   m = CalculateDistancesD7(m);
   m = CalculateDistancesCS(m);
   m.SetTime2ComputeFrame(m.GetComputeStartTime() - std::clock());
   return m;
}

template<typename T>
std::pair<double,double> GetPathMinMax( const LRL_Path<T>& path ) {
   std::pair<double, double> p(std::make_pair(DBL_MAX, 0));
   if ( !path.GetDistances().empty()) {
      p = std::make_pair(path.GetMin(), path.GetMax());
   }
   return p;
}

std::pair<double, double> MultiFollower::GetMinMax(void) const {
   std::pair<double, double> p;
   const std::pair<double, double> pS6 = GetPathMinMax(m_s6path);
   const std::pair<double, double> pG6 = GetPathMinMax(m_g6path);
   const std::pair<double, double> pD7 = GetPathMinMax(m_d7path);
   const std::pair<double, double> pCS = GetPathMinMax(m_cspath);

   const double minp = minNC(pS6.first, pG6.first, pD7.first, pCS.first);
   const double maxp = maxNC(pS6.second, pG6.second, pD7.second, pCS.second);
   return std::make_pair(minp,maxp);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const double MultiFollower::GetTime2ComputerFrame(void) const {
   return(m_seconds2ComputerFrame);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void MultiFollower::SetTime2ComputeFrame(const double computeTime) {
   m_seconds2ComputerFrame = computeTime;
}
