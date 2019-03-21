#include "LatticeCentering.h"

#include "MatG6.h"
#include "SanMatrix.h"

#include <sstream>

const std::vector<std::pair<char, MatS6> > LatticeCentering::m_ToPrimitive = LatticeCentering::MakeUncenterMatrices();;
const std::vector<std::pair<char, MatS6> > LatticeCentering::m_ToCentered = LatticeCentering::MakeCenteringMatrices();

std::vector<std::pair<char, MatS6> > LatticeCentering::MakeUncenterMatrices() {
   if (m_ToPrimitive.empty()) {
      std::vector<std::pair<char, MatS6> >v;
      v.push_back(std::make_pair('P', MatG6::Eye()));
      v.push_back(std::make_pair('I', MatG6("1 0 0 0 0 0    0 1 0 0 0 0    .25 .25 .25 .25 .25 .25   0 1 0 .5 0 .5   1 0 0 0 .5 .5   0 0 0 0 0 1"))); // for monoclinic, assumes b unique
      v.push_back(std::make_pair('A', MatG6("1 0 0 0 0 0   0 1 0 0 0 0   0 .25 .25 .25 0 0    0 1 0 .5 0 0   0 0 0 0 .5 .5   0 0 0 0 0 1"))); // for monoclinic, assumes b unique
      v.push_back(std::make_pair('B', MatG6("1 0 0 0 0 0   0 1 0 0 0 0   .25 0 .25 0 .25 0   0 0 0 .5 0 .5   1 0 0 0 .5 0   0 0 0 0 0 1"))); // for monoclinic, assumes c unique
      v.push_back(std::make_pair('C', MatG6("1 0 0 0 0 0   .25 .25 0 0 0 .25   0 0 1 0 0 0    0 0 0 .5 .5 0   0 0 0 0 1 0   1 0 0 0 0 .5"))); // for monoclinic, assumes b unique
      v.push_back(std::make_pair('F', MatG6(".25 .25 0 0 0 .25     .25 0 .25 0 .25 0     0 .25 .25 .25  0 0    0 0 .5 .25 .25 .25     0 .5 0 .25 .25 .25     .5 0 0 .25 .25 .25")));
      v.push_back(std::make_pair('R', (1.0 / 9.0)* MatG6("1 1 1 1 -1 -1    4 1 1  1  2  2     1  4  1  -2  -1  2     -4  -4  2  -1  1  -5     2  -4  2  -1  -2  1     -4  2  2  2  1  1")));
      v.push_back(std::make_pair('H', MatG6::Eye()));
      return v;
   }
   else
   return m_ToPrimitive;
}

MatS6 LatticeCentering::MakeCentering(const MatS6& m) {
   const CMatrix cm(m);
   const CMatrix inv = cm.Inverse();
   const MatS6 m6(inv.GetVector());
   return m6;
}

std::vector<std::pair<char, MatS6> > LatticeCentering::MakeCenteringMatrices() {
   if (m_ToPrimitive.empty()) MakeUncenterMatrices();

   std::vector<std::pair<char, MatS6> > v;
   for ( unsigned long i=0; i< m_ToPrimitive.size(); ++i )
      v.push_back(std::make_pair(m_ToPrimitive[i].first, MakeCentering(m_ToPrimitive[i].second)));

   return v;
}

LatticeCentering::LatticeCentering()
   : m_3dMatricesToCentered(Make3dCenteringMatrices())

{
}

std::vector<std::pair<char, MatS6> > LatticeCentering::GetMakePrimitiveMatrices() {
   if (m_ToPrimitive.empty()) throw;
   return m_ToPrimitive;
}

std::vector<std::pair<char, MatS6> > LatticeCentering::GetMakeCenteredMatrices() {
   if (m_ToCentered.empty()) throw;
   return m_ToCentered;
}

MatS6 LatticeCentering::findToPrimitive(const char c) const {
   for (unsigned long i = 0; i < m_ToPrimitive.size(); ++i)
      if (c == m_ToPrimitive[i].first) return m_ToPrimitive[i].second;
   throw;
}

MatS6 LatticeCentering::findToCentered(const char c) const {
   for (unsigned long i = 0; i < m_ToCentered.size(); ++i)
      if (c == m_ToCentered[i].first) return m_ToCentered[i].second;
   throw;
}
MatS6 LatticeCentering::findToPrimitive(const std::string& c) const {
   for (unsigned long i = 0; i < m_ToPrimitive.size(); ++i)
      if (c[0] == m_ToPrimitive[i].first) return m_ToPrimitive[i].second;
   throw;
}

MatS6 LatticeCentering::findToCentered(const std::string& c) const {
   for (unsigned long i = 0; i < m_ToCentered.size(); ++i)
      if (c[0] == m_ToCentered[i].first) return m_ToCentered[i].second;
   throw;
}

std::vector<double> LatticeCentering::Make3dVector( const std::string& s) {
   std::istringstream istr(s);
   double d;
   std::vector<double> v;
   while (istr && !istr.eof()) {
      istr >> d;
      v.push_back(d);
   }
   if (v.size() !=  9) throw;
   return v;
}

std::vector<std::pair<std::string,std::vector<double> > > LatticeCentering::Make3dCenteringMatrices() {
   std::vector<std::pair<std::string, std::vector<double> > > v;
   v.push_back(std::make_pair("C1",  Make3dVector("0 1 1  1 0 1  1 1 0")));
   v.push_back(std::make_pair("C3",  Make3dVector("1 1 0 -1 1 0  1 1 2")));
   v.push_back(std::make_pair("C5",  Make3dVector("1 0 0  0 0 1  0 1 1")));
   v.push_back(std::make_pair("C5",  Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("H4",  Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("R1",  Make3dVector("1 -1 0  0 1 -1  1 1 1")));
   v.push_back(std::make_pair("R3",  Make3dVector("1 0 0  0 0 1  1 3 2")));
   v.push_back(std::make_pair("T1",  Make3dVector("0 1 1  1 0 1  1 1 0")));
   v.push_back(std::make_pair("T2",  Make3dVector("1 0 0  0 1 0  1 1 2")));
   v.push_back(std::make_pair("T5",  Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("T5",  Make3dVector("1 0 0  0 0 1  0 1 1")));
   v.push_back(std::make_pair("T5",  Make3dVector("0 1 0  0 1 1  1 0 0")));
   v.push_back(std::make_pair("O1A", Make3dVector("1 1 0 -1 1 0  1 1 2")));
   v.push_back(std::make_pair("O1B", Make3dVector("0 1 1  1 0 1  1 1 0")));
   v.push_back(std::make_pair("O2",  Make3dVector("1 0 0  0 1 0  1 1 2")));
   v.push_back(std::make_pair("O3",  Make3dVector("0 1 1  1 0 1  1 1 0")));
   v.push_back(std::make_pair("O3",  Make3dVector("1 0 0  0 1 0  1 1 2")));
   v.push_back(std::make_pair("O4",  Make3dVector("2 1 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("O4",  Make3dVector("1 -1 0  1 1 0  0 0 1")));
   v.push_back(std::make_pair("O5",  Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("O5",  Make3dVector("1 0 0  0 0 1  0 1 1")));
   v.push_back(std::make_pair("M1A", Make3dVector("-1 -1 -1  1 -1 0  0 0 1")));
   v.push_back(std::make_pair("M1B", Make3dVector("0 1 1  1 1 0 -1 0 -1")));
   v.push_back(std::make_pair("M2A", Make3dVector("-1 -1 -2 0 1 0  1 0 0")));
   v.push_back(std::make_pair("M2B", Make3dVector("0 1 1  1 1 0 -1 0 -1")));
   v.push_back(std::make_pair("M2B", Make3dVector("-1 -1 -1  1 -1 0  0 0 1")));
   v.push_back(std::make_pair("M3",  Make3dVector("-1 -1 -2  0 1 0  1 0 0")));
   v.push_back(std::make_pair("M3",  Make3dVector("1 1 0  0 -1 -1 -1 0 -1")));
   v.push_back(std::make_pair("M4",  Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("A1",  Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("A2",  Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("A3",  Make3dVector("1 0 0  0 1 0  0 0 1")));

   return v;
}

std::pair<std::string, std::vector<double> > LatticeCentering::operator[] (const unsigned long n) {
   return m_3dMatricesToCentered[n];
}

std::pair<std::string, std::vector<double> > LatticeCentering::operator[] (const std::string& s) {
   for (unsigned long i = 0; i < m_3dMatricesToCentered.size(); ++i)
      if (m_3dMatricesToCentered[i].first == s) return m_3dMatricesToCentered[i];
   return std::pair<std::string, std::vector<double> >();
}