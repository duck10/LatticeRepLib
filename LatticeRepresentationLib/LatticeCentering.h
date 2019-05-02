#ifndef LATTICECENTERING_H
#define LATTICECENTERING_H

#include <algorithm>
#include <utility>
#include <vector>

#include "MatS6.h"

class LatticeCentering {
public:
   LatticeCentering();

   std::vector<std::pair<char, MatS6> > GetMakePrimitiveMatrices();
   std::vector<std::pair<char, MatS6> > GetMakeCenteredMatrices();
   MatS6 findToPrimitive(const char c) const;
   MatS6 findToCentered(const char c) const;
   MatS6 findToPrimitive(const std::string& c) const;
   MatS6 findToCentered(const std::string& c) const;
   static std::vector<std::pair<std::string, std::vector<double> > > Make3dCenteringMatrices();
   std::pair<std::string, std::vector<double> >operator[] (const std::string& s);
   std::pair<std::string, std::vector<double> >operator[] (const unsigned long n);
   size_t size() { return m_3dMatricesToCentered.size(); }
	static MatS6 MakeCentering(const MatS6& m);

   static std::pair<std::string, MatS6 > CreateCenteringMatrix(const std::string& lattice, const std::string& threespaceMatrix);


protected:
   static std::vector<std::pair<char, MatS6> > MakeUncenterMatrices();
   static std::vector<std::pair<char, MatS6> > MakeCenteringMatrices();
   static const std::vector<std::pair<char, MatS6> > m_ToPrimitive;
   static const std::vector<std::pair<char, MatS6> > m_ToCentered;
   static std::vector<double> Make3dVector(const std::string& s);
   std::vector<std::pair<std::string, std::vector<double> > > m_3dMatricesToCentered;
};

#endif  // LATTICECENTERING_H

