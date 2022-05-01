#ifndef DELONETYPELIST_H
#define DELONETYPELIST_H

#include <algorithm>
#include <utility>
#include <vector>

#include "DeloneFitResults.h"
#include "DeloneType.h"
#include "MatS6.h"

template< typename R>
auto Getter(const std::string& s, const R& r) {
   for (size_t i = 0; i < r.size(); ++i) {
      const std::string test = r[i].first;
      if (test == s) {
         return r[i].second;
      }
   }
   throw;
}

class DeloneTypeList {
public:
   DeloneTypeList(const bool build=true);
   DeloneType operator[] (const std::string& s) const;
   DeloneType operator[] (const size_t n) const;
   size_t size() const { return m_deloneTypes.size(); }
   std::vector<DeloneType> GetAllTypes() const { return m_deloneTypes; }
   static std::vector<std::pair<std::string, std::vector<double> > > Make3dCenteringMatrices();
   std::vector<DeloneFitResults> Fit( const std::string& type, const S6& s6, const S6& sig, const MatS6& m ) const;
   std::vector<DeloneFitResults> Fit( const S6& s6, const S6& sig, const MatS6& m ) const;
   static const std::vector<std::string> typelist;
   static std::vector<std::pair<std::string, std::vector<MatS6> > > CreateAllPrjs();
   static std::vector<std::pair<std::string, std::vector<MatS6> > > CreateAllPerps();
   static std::vector<std::pair<std::string, std::vector<MatS6> > > CreateAllToCanon();
   static std::vector<std::pair<std::string, std::string> > CreateE3CenteringMatrixList();
   static std::vector<std::pair<std::string, std::string> > CreateCharacterList();
   static std::vector<std::pair<std::string, std::string> > CreateBravaisTypeList();

protected:
   static double GetFreeParams( const std::string& s );
   static std::vector<double> Make3dVector(const std::string& s);
   static std::pair<std::string, MatS6 > CreateCenteringMatrix(const std::string& lattice, const std::string& threespaceMatrix);
   static std::vector<std::pair<std::string, MatS6> > CreateListOfCenteringMatrices();
   static std::vector<std::pair<std::string, int> > CreateListOfFreeParams( );
   std::vector< DeloneType> m_deloneTypes;
};

#endif  // DELONETYPELIST_H

