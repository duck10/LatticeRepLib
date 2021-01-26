#include "Investigate_S6_Boundaries.h"

std::vector<MatS6> m_refl = MatS6::GetReflections();
std::vector<MatS6> m_bounds6 = Investigate_S6_Boundaries::CreateS6ReductionMatrices();

Investigate_S6_Boundaries::Investigate_S6_Boundaries(const double radiusOfConfusion)
   : m_radiusOfConfusion(radiusOfConfusion)
   , m_tree()
{

}

std::vector<MatS6> Investigate_S6_Boundaries::CreateS6ReductionMatrices() {
   static const std::vector<std::pair<MatS6, MatS6> > redc = S6::SetUnreductionMatrices();
   std::vector<MatS6> v;
   for (size_t i = 0; i < redc.size(); ++i) {
      v.push_back(redc[i].first);
   }
   if (v.size() != 6) throw;
   return v;
}

void Investigate_S6_Boundaries::CreateS6ReductionMatrices24() {
   for ( size_t i=0; i<m_refl.size(); ++i)
   m_bounds24.push_back(m_bounds6[0]*m_refl[i]);
}

void Investigate_S6_Boundaries::Process(const S6& s) {

}