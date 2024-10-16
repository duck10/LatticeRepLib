#ifndef S_ORDINALS_H
#define S_ORDINALS_H

#include <vector>

#include "MatS6.h"
#include "EnhancedS6.h"

class S6_Ordinals : public EnhancedS6 {
public:
   S6_Ordinals();
   S6_Ordinals(const EnhancedS6& s);
   S6_Ordinals(const S6_Ordinals& so);
   EnhancedS6 InitializeOrdinals();
   static MatS6 MatrixFromOrdinals(const S6_Ordinals& s);
   static MatS6 MatrixFromOrdinals(const EnhancedS6& s);

   friend S6_Ordinals operator* (const MatS6& m, const S6_Ordinals& so);

   EnhancedS6 m_ordinals;
};

#endif // S_ORDINALS_H