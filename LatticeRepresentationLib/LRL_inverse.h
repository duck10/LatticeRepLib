/* inverse.h  basic Gaussian elimination version */
/* inverse.c  basic Gaussian elimination version */
/* http://userpages.umbc.edu/~squire/f14-455/download/inverse.c */

#ifndef LRL_INVERSE
#define LRL_INVERSE

#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
   void inverse( int n, double A [ ], double AA [ ] );

#ifdef __cplusplus
}
#endif 

template<typename T>
T Inverse(const T& min) {
   T m(min);
   std::vector<double> arout(m.size());
   inverse(min.GetRowDim(), min.GetVector().data(), arout.data());
   m.SetVector(arout);
   return m;
}

#endif // LRL_INVERSE