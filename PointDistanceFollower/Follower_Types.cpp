#include "Follower_Types.h"
#include "G6.h"
#include "NCDist.h"
#include "R9.h"
#include "S6.h"

Follower_Type::Follower_Type (const Follower_Type_R9)
   : m_name("R9")
{
   m_SellingPath.clear();
   m_NiggliPath.clear();
   m_R9Path.clear();
}

//Follower_Type_R9::Follower_Type_R9()
//   : Follower_Type_Base<R9>()
//{
//   m_name = R9::GetName();
//}

MultiFollower Follower_Type_R9::CalculateDistances(const MultiFollower& mf) const {
   throw;
   //std::vector<double> vdist;
   //const std::vector<std::pair<G6, G6> >& m_Path(Follower_Type_Base<G6> m_NiggiReducedPath);
   ////const std::vector<std::pair<G6, G6> > secondPath(mf.GetSG6().GetSecondPath());

   //const std::pair<D13, D13> initial = Follower_Type_Base<D13>::m_Path[0];
   //const std::pair<S6, S6 > p1 = std::make_pair(S6(initial.first.GetCell()), S6((initial.second.GetCell())));
   //if (!S6::IsInvalidPair(p1)) {
   //   for (size_t i = 0; i < Follower_Type_Base<D13>::m_Path.size(); ++i) {
   //      const std::pair<D13, D13> point = Follower_Type_Base<D13>::m_Path[i];
   //      const double distance = (point.first - point.second).Norm();
   //      vdist.push_back(distance);
   //   }
   //}
   MultiFollower m(mf);
   //m.SetDistancesG6(vdist);
   return m;
}

void Follower_Type_R9::SetLatticePointChoiceForDistanceCalculation() {
   m_latticePointChoiceForDistanceCalculation = (
      FollowerConstants::globalFollowerMode == FollowerConstants::globalChord ||
      FollowerConstants::globalFollowerMode == FollowerConstants::globalSinglePoint) ?
      versusFirstPoint : versusCorrespondingPoint;
}
