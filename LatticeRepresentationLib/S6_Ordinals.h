#ifndef S_ORDINALS_H
#define S_ORDINALS_H

#include <vector>

#include "S6.h"

class S6_Ordinals : public S6 {
public:
   S6_Ordinals();
   S6_Ordinals(const S6& s);
   S6_Ordinals(const S6_Ordinals& so);
   std::vector<double> SetOrdinals();

   friend S6_Ordinals operator* (const MatS6& m, const S6_Ordinals& so);

   S6 m_ordinals;

};

#endif // S_ORDINALS_H