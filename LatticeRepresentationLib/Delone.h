#ifndef DELAUNAY_H
#define DELAUNAY_H

#include "S6.h"
#include "B4.h"
#include "D7.h"
#include "MatD7.h"
#include "Selling.h"
#include <set>
class D7;
class MatG6;
class MatD7;

class Delone {

public:
   template<typename TVEC>
   static bool Reduce(const TVEC& d, TVEC& dd) {
      S6 s6out;
      const S6 s6in(d);
      const bool b = Selling::Reduce(s6in, s6out);
      MatD7 mat;
      D7 d7(s6out);
      dd = S6(sort(d7, mat));

      return b;
   }

   template<typename TVEC, typename TMAT>
   static bool Reduce(const TVEC& d, TMAT& m, TVEC& dd) {
      MatS6 mS6;
      S6 s6out;
      const bool b = Reduce(S6(d), mS6, s6out, 0.0);
      MatD7 mD7(mS6);
      dd = S6(sort(D7(s6out), mD7));
      m = mD7;
      return b;
   }

   static bool Reduce(const S6& d, MatS6& m, S6& dd, const double delta);
   static bool Reduce(const S6& d, S6& dd);
   static bool Reduce(const D7& d, D7& dd);
   static bool Reduce(const S6& d, MatS6& m, S6& dd);
   static D7 sort(const D7& d, MatD7& m);
   static bool IsReduced(const G6& v);
   static bool IsReduced(const G6& v, const double delta);
   static bool IsDelone( const D7& v, const double delta = 1.0E-6 );
   static std::vector<MatD7> GetD7Reflections();
   static std::vector<MatS6> LoadLatticeTypeProjectors();

private:
   static int GenMaxMinKey(const D7& d);
   static unsigned long FindRefl(const unsigned long key, const D7& random, std::set<unsigned long>& sr);
};

#endif   //  DELAUNAY_H

