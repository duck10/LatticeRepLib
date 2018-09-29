#ifndef NULLOPS_H
#define NULLOPS_H

#include <string>

class NullOps {
public:
   template<typename T>
   static bool Reduce(const T& in, T& out) { out = in; return true; }

   template<typename TVEC, typename TMAT>
   static bool Reduce(const TVEC& in, TMAT& m, TVEC& out) { out = in; return true; }

   static unsigned long GetCycles() { return 0; }
   static std::string GetName() { return "NullOps"; }

};
#endif // NULLOPS_H


