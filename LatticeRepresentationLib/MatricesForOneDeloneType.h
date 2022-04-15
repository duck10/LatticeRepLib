#ifndef MATRICESFORONEDELONETYPE_H
#define MATRICESFORONEDELONETYPE_H

#include "MatS6.h"

#include <vector>

class MatricesForOneDeloneType {
   typedef std::vector<MatS6> vmat;

public:
   MatricesForOneDeloneType() {}
   MatricesForOneDeloneType(const vmat& prj, const vmat& perp, const vmat& toCanon)
      : m_prjs(prj)
      , m_perps(perp)
      , m_toCanons(toCanon)
   {}

   MatS6 GetPrj(const size_t n) const { return m_prjs[n]; }
   MatS6 GetPerp(const size_t n) const { return m_perps[n]; }
   MatS6 GetToCanon(const size_t n) const { return m_toCanons[n]; }
   std::vector<MatS6> GetPrjs(void) const { return m_prjs; }
   std::vector<MatS6> GetPerps(void) const { return m_perps; }
   std::vector<MatS6> GetToCanons(void) const { return m_toCanons; }

   size_t size(void) const { return m_prjs.size(); }

protected:
   vmat m_prjs;
   vmat m_perps;
   vmat m_toCanons;

};

#endif // MATRICESFORONEDELONETYPE_H