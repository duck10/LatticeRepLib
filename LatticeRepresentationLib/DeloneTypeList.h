#ifndef DELONETYPELIST_H
#define DELONETYPELIST_H

#include <algorithm>
#include <utility>
#include <vector>

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
   DeloneTypeList();

   DeloneType operator[] (const std::string& s) const;
   DeloneType operator[] (const size_t n) const;
   size_t size() const { return m_deloneTypes.size(); }
   static std::vector<std::pair<std::string, std::vector<double> > > Make3dCenteringMatrices();
   std::vector<std::tuple<double, S6, MatS6> > Fit( const std::string& type, const S6& s6 ) const;
   std::vector<std::tuple<double, S6, MatS6> > Fit( const S6& s6 ) const;
   static const std::vector<std::string> typelist;

protected:
   static std::vector<double> Make3dVector(const std::string& s);
   static std::pair<std::string, MatS6 > CreateCenteringMatrix(const std::string& lattice, const std::string& threespaceMatrix);
   static std::vector<std::pair<std::string, MatS6> > CreateListOfCenteringMatrices();

   std::vector< DeloneType> m_deloneTypes;
};

#endif  // DELONETYPELIST_H

