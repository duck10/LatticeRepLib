#include "S6_Ordinals.h"

std::vector<double> S6_Ordinals::SetOrdinals() {
   std::vector<double> v;
   for (auto i = 0; i < 6; ++i) v[i] = double(i);
   return v;
}

S6_Ordinals::S6_Ordinals()
   : S6()
   , m_ordinals(SetOrdinals())
{}

S6_Ordinals::S6_Ordinals(const S6& s)
   : S6(s)
   , m_ordinals(SetOrdinals())
{}

S6_Ordinals::S6_Ordinals(const S6_Ordinals& so)
   : S6(so)
   , m_ordinals(so.m_ordinals)
{}

S6_Ordinals operator* (const MatS6& m, const S6_Ordinals& soin) {
   S6_Ordinals so = m * S6(soin);
   so.m_ordinals = m * soin.m_ordinals;
}