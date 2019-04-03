#include "S6_Ordinals.h"

S6 S6_Ordinals::InitializeOrdinals() {
   S6 s;
   for (auto i = 0; i < 6; ++i) s[i] = double(i);
   return s;
}

S6_Ordinals::S6_Ordinals()
   : S6()
   , m_ordinals()
{
   m_ordinals = InitializeOrdinals();

}

S6_Ordinals::S6_Ordinals(const S6& s)
   : S6(s)
   , m_ordinals(InitializeOrdinals())
{}

S6_Ordinals::S6_Ordinals(const S6_Ordinals& so)
   : S6(so)
   , m_ordinals(so.m_ordinals)
{}

S6_Ordinals operator* (const MatS6& m, const S6_Ordinals& soin) {
   S6_Ordinals so = m * S6(soin);
   so.m_ordinals = m * soin.m_ordinals;
   return so;
}

MatS6 S6_Ordinals::MatrixFromOrdinals(const S6_Ordinals& s) {
   return MatrixFromOrdinals(s.m_ordinals);
}

MatS6 S6_Ordinals::MatrixFromOrdinals(const S6& s) {
   MatS6 m(MatS6().Zero());
   for ( auto i=0; i<6; ++i ) {
      m[6 * i + int(s[i])] = 1.0;   
   }
   return m;
}
