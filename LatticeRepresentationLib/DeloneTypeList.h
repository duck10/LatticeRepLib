#ifndef LATTICECENTERING_H
#define LATTICECENTERING_H

#include <algorithm>
#include <utility>
#include <vector>

#include "DeloneType.h"
#include "MatS6.h"

template< typename R>
auto Getter(const std::string& s, const R& r) {
   for (auto i = 0; i < r.size(); ++i) {
      const std::string test = r[i].first;
      if (test == s) {
         return r[i].second;
      }
   }
   throw;
   //return T();
}

class DeloneTypeList {
public:
   DeloneTypeList();

   std::vector<std::pair<char, MatS6> > GetMakePrimitiveMatrices();
   std::vector<std::pair<char, MatS6> > GetMakeCenteredMatrices();
   MatS6 findToPrimitive(const char c) const;
   MatS6 findToCentered(const char c) const;
   MatS6 findToPrimitive(const std::string& c) const;
   MatS6 findToCentered(const std::string& c) const;
   static std::vector<std::pair<std::string, std::vector<double> > > Make3dCenteringMatrices();
   DeloneType operator[] (const std::string& s);
   DeloneType operator[] (const unsigned long n);
   size_t size() { return m_types.size(); }
	static MatS6 MakeCentering(const MatS6& m);

   static std::pair<std::string, MatS6 > CreateCenteringMatrix(const std::string& lattice, const std::string& threespaceMatrix);
   static std::vector<std::pair<std::string, MatS6> > CreateListOfCenteringMatrices();

   std::string        GetCharacter(const std::string& type) const {return Getter(type, m_characters);}
   std::string        Gete3matrix(const std::string& type) const { return Getter(type, m_e3matrices); }
   std::string        GetBravaisLatticeType(const std::string& type) const { return Getter(type, m_bravaisLatticeTypes); }
   MatS6              GetCenteringMatrix(const std::string& type) const { return Getter(type, m_centeringMatrices); }
   std::vector<MatS6> GetPrjList(const std::string& type) const { return Getter(type, m_prjs); }
   std::vector<MatS6> GetPerpList(const std::string& type) const { return Getter(type, m_perps); }
   std::vector<MatS6> GetToCanonList(const std::string& type) const { return Getter(type, m_toCanons); }

protected:
   static std::vector<double> Make3dVector(const std::string& s);
   static const std::vector<std::string> m_typeList;

   std::vector<DeloneType> m_types;

   std::vector<std::pair< std::string, std::vector<MatS6> > > m_prjs;
   std::vector<std::pair< std::string, std::vector<MatS6> > > m_perps;
   std::vector<std::pair< std::string, std::vector<MatS6> > > m_toCanons;
   std::vector<std::pair<std::string, std::string> > m_characters;
   std::vector<std::pair<std::string, std::string> > m_e3matrices;
   std::vector<std::pair<std::string, std::string> > m_bravaisLatticeTypes;
   std::vector<std::pair<std::string, MatS6> > m_centeringMatrices;

};

#endif  // LATTICECENTERING_H

