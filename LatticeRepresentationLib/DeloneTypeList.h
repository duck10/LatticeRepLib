#ifndef LATTICECENTERING_H
#define LATTICECENTERING_H

#include <algorithm>
#include <utility>
#include <vector>

#include "DeloneType.h"
#include "MatS6.h"

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


protected:
   std::vector<std::pair<char, MatS6> > MakeUncenterMatrices();
   std::vector<std::pair<char, MatS6> > MakeCenteringMatrices();
   static std::vector<double> Make3dVector(const std::string& s);
   std::vector<DeloneType> m_types;

   //static const std::vector<std::pair<char, MatS6> > m_ToPrimitive;
   //static const std::vector<std::pair<char, MatS6> > m_ToCentered;
   //std::vector<std::pair<std::string, std::vector<double> > > m_3dMatricesToCentered;
};

#endif  // LATTICECENTERING_H

