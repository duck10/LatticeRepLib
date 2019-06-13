#include "DeloneTypeList.h"

#include "LabeledSellaMatrices.h"
#include "MatG6.h"
#include "LRL_MaximaTools.h"
//#include "SanMatrix.h"
#include "LRL_inverse.h"
#include "SellaBuild.h"

#include <sstream>
#include <string>

static const std::vector<std::string> typelist{ "C1","C3","C5","R1","R3","T1","T2","T5","O1A","O1B","O2",/*"O3",*/"O4","O5","M1A","M1B","M2A"/*,"M2B"*//*,"M3"*/,"M4"/*,"A1","A2","A3"*/,"H4" };

std::vector<std::pair<std::string, std::string> > CreateBravaisTypeList() {
   std::vector<std::pair<std::string, std::string> > v;
   v.push_back(std::make_pair("C1", "cI"));
   v.push_back(std::make_pair("C3", "cF"));
   v.push_back(std::make_pair("C5", "cP"));
   v.push_back(std::make_pair("R1", "rP"));
   v.push_back(std::make_pair("R3", "rP"));
   v.push_back(std::make_pair("T1", "tI"));
   v.push_back(std::make_pair("T2", "tI"));
   v.push_back(std::make_pair("T5", "tP"));
   v.push_back(std::make_pair("O1A", "oF"));
   v.push_back(std::make_pair("O1B", "oI"));
   v.push_back(std::make_pair("O2", "oI"));
   v.push_back(std::make_pair("O3", "oI"));
   v.push_back(std::make_pair("O4", "oP"));
   v.push_back(std::make_pair("O5", "oP"));
   v.push_back(std::make_pair("M1A", "mC"));
   v.push_back(std::make_pair("M1B", "mC"));
   v.push_back(std::make_pair("M2A", "mC"));
   v.push_back(std::make_pair("M2B", "mC"));
   v.push_back(std::make_pair("M3", "mC"));
   v.push_back(std::make_pair("M4", "mP"));
   v.push_back(std::make_pair("A1", "aP"));
   v.push_back(std::make_pair("A2", "aP"));
   v.push_back(std::make_pair("A3", "aP"));
   v.push_back(std::make_pair("H4", "hP"));
   return v;
}

std::vector<std::pair<std::string, std::string> > CreateCharacterList() {
   std::vector<std::pair<std::string, std::string> > v;
   v.push_back(std::make_pair("C1", "rrr rrr"));
   v.push_back(std::make_pair("C3", "rr0 rr0"));
   v.push_back(std::make_pair("C5", "000 rrr"));
   v.push_back(std::make_pair("R1", "rrr sss"));
   v.push_back(std::make_pair("R3", "rr0 sr0"));
   v.push_back(std::make_pair("T1", "rrs rrs"));
   v.push_back(std::make_pair("T2", "rr0 rrs"));
   v.push_back(std::make_pair("T5", "000 rrs"));
   v.push_back(std::make_pair("O1A", "rrs rrt"));
   v.push_back(std::make_pair("O1B", "rst rst"));
   v.push_back(std::make_pair("O2", "rs0 srt"));
   v.push_back(std::make_pair("O3", "rs0 rs0"));
   v.push_back(std::make_pair("O4", "00r sst"));
   v.push_back(std::make_pair("O5", "000 rst"));
   v.push_back(std::make_pair("M1A", "rrs ttu"));
   v.push_back(std::make_pair("M1B", "rst rsu"));
   v.push_back(std::make_pair("M2A", "rs0 stu"));
   v.push_back(std::make_pair("M2B", "rs0 rst"));
   v.push_back(std::make_pair("M3", "rs0 ts0"));
   v.push_back(std::make_pair("M4", "00r stu"));
   v.push_back(std::make_pair("A1", "rst uvw"));
   v.push_back(std::make_pair("A2", "rs0 tuv"));
   v.push_back(std::make_pair("A3", "rs0 tu0"));
   v.push_back(std::make_pair("H4", "00r rrs"));
   return v;
}

std::vector<std::pair<std::string, std::string> > CreateE3CenteringMatrixList() {
   std::vector<std::pair<std::string, std::string> > v;
   v.push_back(std::make_pair("C1", "0 1 1  1 0 1  1 1 0"));
   v.push_back(std::make_pair("C3", "1 1 0 -1 1 0  1 1 2"));
   //v.push_back(std::make_pair("C5", "1 0 0  0 0 1  0 1 1"));
   v.push_back(std::make_pair("C5", "1 0 0  0 1 0  0 0 1"));
   v.push_back(std::make_pair("R1", "1 -1 0  0 1 -1  1 1 1"));
   v.push_back(std::make_pair("R3", "1 0 0  0 0 1  1 3 2"));
   v.push_back(std::make_pair("T1", "0 1 1  1 0 1  1 1 0"));
   v.push_back(std::make_pair("T2", "1 0 0  0 1 0  1 1 2"));
   v.push_back(std::make_pair("T5", "1 0 0  0 1 0  0 0 1"));
   //v.push_back(std::make_pair("T5", "1 0 0  0 0 1  0 1 1"));
   //v.push_back(std::make_pair("T5", "0 1 0  0 1 1  1 0 0"));
   v.push_back(std::make_pair("O1A", "1 1 0 -1 1 0  1 1 2"));
   v.push_back(std::make_pair("O1B", "0 1 1  1 0 1  1 1 0"));
   v.push_back(std::make_pair("O2", "1 0 0  0 1 0  1 1 2"));
   v.push_back(std::make_pair("O3", "0 1 1  1 0 1  1 1 0"));
   //v.push_back(std::make_pair("O3", "1 0 0  0 1 0  1 1 2"));
   //v.push_back(std::make_pair("O4", "2 1 0  0 1 0  0 0 1"));
   v.push_back(std::make_pair("O4", "1 -1 0  1 1 0  0 0 1"));
   v.push_back(std::make_pair("O5", "1 0 0  0 1 0  0 0 1"));
   //v.push_back(std::make_pair("O5", "1 0 0  0 0 1  0 1 1"));
   v.push_back(std::make_pair("M1A", "-1 -1 -1  1 -1 0  0 0 1"));
   v.push_back(std::make_pair("M1B", "0 1 1  1 1 0 -1 0 -1"));
   v.push_back(std::make_pair("M2A", "-1 -1 -2 0 1 0  1 0 0"));
   v.push_back(std::make_pair("M2B", "0 1 1  1 1 0 -1 0 -1"));
   //v.push_back(std::make_pair("M2B", "-1 -1 -1  1 -1 0  0 0 1"));
   v.push_back(std::make_pair("M3", "-1 -1 -2  0 1 0  1 0 0"));
   //v.push_back(std::make_pair("M3", "1 1 0  0 -1 -1 -1 0 -1"));
   v.push_back(std::make_pair("M4", "1 0 0  0 1 0  0 0 1"));
   v.push_back(std::make_pair("A1", "1 0 0  0 1 0  0 0 1"));
   v.push_back(std::make_pair("A2", "1 0 0  0 1 0  0 0 1"));
   v.push_back(std::make_pair("A3", "1 0 0  0 1 0  0 0 1"));
   v.push_back(std::make_pair("H4", "1 0 0  0 1 0  0 0 1"));
   return v;
}

std::vector<std::pair< std::string, std::vector<MatS6> > > CreateAllPrjs() {
   std::vector<LabeledSellaMatrices> vprj = LabeledSellaMatrices::CreateAllPrjs();
   std::vector<std::pair< std::string, std::vector<MatS6> > > v;
   for (unsigned long i = 0; i < vprj.size(); ++i) {
      v.push_back(std::make_pair(vprj[i].GetLabel(), vprj[i].GetMatrices()));
   }return v;
}

std::vector<std::pair< std::string, std::vector<MatS6> > > CreateAllPerps() {
   std::vector<LabeledSellaMatrices> vperp = LabeledSellaMatrices::CreateAllPerps();
   std::vector<std::pair< std::string, std::vector<MatS6> > > v;
   for (unsigned long i = 0; i < vperp.size(); ++i) {
      v.push_back(std::make_pair(vperp[i].GetLabel(), vperp[i].GetMatrices()));
   }return v;
}

std::vector<std::pair< std::string, std::vector<MatS6> > > CreateAllToCanaon() {
   SellaBuild sb;
   const std::vector<LabeledDeloneTypeMatrices>   sbmap = sb.Build();
   std::vector<std::pair< std::string, std::vector<MatS6> > > v;

   for (std::vector<LabeledDeloneTypeMatrices>::const_iterator it= sbmap.begin(); it!= sbmap.end(); ++it ) {
      LabeledDeloneTypeMatrices ldtm = *it;
      const std::string label = ldtm.GetLabel();
      v.push_back(std::make_pair(label, ldtm.GetTocanon(label)));
   }
   return v;
}

DeloneTypeList::DeloneTypeList()
{
   static const std::vector<std::pair<std::string, std::string> > characters = CreateCharacterList();
   static const std::vector<std::pair<std::string, std::string> > e3matrices = CreateE3CenteringMatrixList();
   static const std::vector<std::pair<std::string, std::string> > bravaisLatticeTypes = CreateBravaisTypeList();
   static const std::vector<std::pair<std::string, MatS6> >       centeringMatrices = CreateListOfCenteringMatrices();

   static const std::vector<std::pair<std::string, std::vector<MatS6> > >   prj = CreateAllPrjs();
   static const std::vector<std::pair<std::string, std::vector<MatS6> > >   perp = CreateAllPerps();
   static const std::vector<std::pair<std::string, std::vector<MatS6> > >   toCanon = CreateAllToCanaon();

   static std::vector<std::pair<std::string,MatricesForOneDeloneType> > matrices;
   if (matrices.empty()) {
      for (unsigned long i = 0; i < typelist.size(); ++i) {
         matrices.push_back(
            std::make_pair(typelist[i],
            MatricesForOneDeloneType(
            Getter(typelist[i], prj),
            Getter(typelist[i], perp),
            Getter(typelist[i], toCanon))));
      }
   }

   for (unsigned long i=0; i< typelist.size(); ++i ) {
      const DeloneType dt(typelist[i],
         Getter(typelist[i], bravaisLatticeTypes),
         Getter(typelist[i], characters),
         Getter(typelist[i], e3matrices),
         Getter(typelist[i], centeringMatrices),
         Getter(typelist[i], matrices));

      m_deloneTypes.push_back(dt);
         //std::cout << dt << std::endl;
   }
}

std::vector<double> DeloneTypeList::Make3dVector( const std::string& s) {
   std::istringstream istr(s);
   double d;
   std::vector<double> v;
   while (istr && !istr.eof()) {
      istr >> d;
      if (d == 0.0) d = 0.0;
      v.push_back(d);
   }
   if (v.size() !=  9) throw;
   return v;
}

std::pair<std::string, MatS6 > DeloneTypeList::CreateCenteringMatrix(const std::string& lattice, const std::string& threespaceMatrix) {
   return std::make_pair(lattice, MatS6::e3Tos6(Make3dVector(threespaceMatrix)));
}

std::vector<std::pair<std::string, MatS6> > DeloneTypeList::CreateListOfCenteringMatrices() {
   std::vector<std::pair<std::string, MatS6> > v;
   v.push_back(DeloneTypeList::CreateCenteringMatrix("C1", "0 1 1  1 0 1  1 1 0"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("C3", "1 1 0 -1 1 0  1 1 2"));
   //v.push_back(DeloneTypeList::CreateCenteringMatrix("C5", "1 0 0  0 0 1  0 1 1"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("C5", "1 0 0  0 1 0  0 0 1"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("R1", "1 -1 0  0 1 -1  1 1 1"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("R3", "1 0 0  0 0 1  1 3 2"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("T1", "0 1 1  1 0 1  1 1 0"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("T2", "1 0 0  0 1 0  1 1 2"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("T5", "1 0 0  0 1 0  0 0 1"));
   //v.push_back(DeloneTypeList::CreateCenteringMatrix("T5", "1 0 0  0 0 1  0 1 1"));
   //v.push_back(DeloneTypeList::CreateCenteringMatrix("T5", "0 1 0  0 1 1  1 0 0"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("O1A", "1 1 0 -1 1 0  1 1 2"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("O1B", "0 1 1  1 0 1  1 1 0"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("O2", "1 0 0  0 1 0  1 1 2"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("O3", "0 1 1  1 0 1  1 1 0"));
   //v.push_back(DeloneTypeList::CreateCenteringMatrix("O3", "1 0 0  0 1 0  1 1 2"));
   //v.push_back(DeloneTypeList::CreateCenteringMatrix("O4", "2 1 0  0 1 0  0 0 1"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("O4", "1 -1 0  1 1 0  0 0 1"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("O5", "1 0 0  0 1 0  0 0 1"));
   //v.push_back(DeloneTypeList::CreateCenteringMatrix("O5", "1 0 0  0 0 1  0 1 1"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("M1A", "-1 -1 -1  1 -1 0  0 0 1"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("M1B", "0 1 1  1 1 0 -1 0 -1"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("M2A", "-1 -1 -2 0 1 0  1 0 0"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("M2B", "0 1 1  1 1 0 -1 0 -1"));
   //v.push_back(DeloneTypeList::CreateCenteringMatrix("M2B", "-1 -1 -1  1 -1 0  0 0 1"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("M3", "-1 -1 -2  0 1 0  1 0 0"));
   //v.push_back(DeloneTypeList::CreateCenteringMatrix("M3", "1 1 0  0 -1 -1 -1 0 -1"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("M4", "1 0 0  0 1 0  0 0 1"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("A1", "1 0 0  0 1 0  0 0 1"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("A2", "1 0 0  0 1 0  0 0 1"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("A3", "1 0 0  0 1 0  0 0 1"));
   v.push_back(DeloneTypeList::CreateCenteringMatrix("H4", "1 0 0  0 1 0  0 0 1"));
   return v;
}

std::vector<std::pair<std::string,std::vector<double> > > DeloneTypeList::Make3dCenteringMatrices() {
   std::vector<std::pair<std::string, std::vector<double> > > v;
   v.push_back(std::make_pair("C1",  Make3dVector("0 1 1  1 0 1  1 1 0")));
   v.push_back(std::make_pair("C3",  Make3dVector("1 1 0 -1 1 0  1 1 2")));
   //v.push_back(std::make_pair("C5",  Make3dVector("1 0 0  0 0 1  0 1 1")));
   v.push_back(std::make_pair("C5",  Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("H4",  Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("R1",  Make3dVector("1 -1 0  0 1 -1  1 1 1")));
   v.push_back(std::make_pair("R3",  Make3dVector("1 0 0  0 0 1  1 3 2")));
   v.push_back(std::make_pair("T1",  Make3dVector("0 1 1  1 0 1  1 1 0")));
   v.push_back(std::make_pair("T2",  Make3dVector("1 0 0  0 1 0  1 1 2")));
   v.push_back(std::make_pair("T5",  Make3dVector("1 0 0  0 1 0  0 0 1")));
   //v.push_back(std::make_pair("T5",  Make3dVector("1 0 0  0 0 1  0 1 1")));
   //v.push_back(std::make_pair("T5",  Make3dVector("0 1 0  0 1 1  1 0 0")));
   v.push_back(std::make_pair("O1A", Make3dVector("1 1 0 -1 1 0  1 1 2")));
   v.push_back(std::make_pair("O1B", Make3dVector("0 1 1  1 0 1  1 1 0")));
   v.push_back(std::make_pair("O2",  Make3dVector("1 0 0  0 1 0  1 1 2")));
   v.push_back(std::make_pair("O3",  Make3dVector("0 1 1  1 0 1  1 1 0")));
   //v.push_back(std::make_pair("O3",  Make3dVector("1 0 0  0 1 0  1 1 2")));
   //v.push_back(std::make_pair("O4",  Make3dVector("2 1 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("O4",  Make3dVector("1 -1 0  1 1 0  0 0 1")));
   v.push_back(std::make_pair("O5",  Make3dVector("1 0 0  0 1 0  0 0 1")));
   //v.push_back(std::make_pair("O5",  Make3dVector("1 0 0  0 0 1  0 1 1")));
   v.push_back(std::make_pair("M1A", Make3dVector("-1 -1 -1  1 -1 0  0 0 1")));
   v.push_back(std::make_pair("M1B", Make3dVector("0 1 1  1 1 0 -1 0 -1")));
   v.push_back(std::make_pair("M2A", Make3dVector("-1 -1 -2 0 1 0  1 0 0")));
   v.push_back(std::make_pair("M2B", Make3dVector("0 1 1  1 1 0 -1 0 -1"))); //  ?????????????????????????
   //v.push_back(std::make_pair("M2B", Make3dVector("-1 -1 -1  1 -1 0  0 0 1"))); //  ?????????????????????????
   v.push_back(std::make_pair("M3",  Make3dVector("-1 -1 -2  0 1 0  1 0 0")));
   //v.push_back(std::make_pair("M3",  Make3dVector("1 1 0  0 -1 -1 -1 0 -1")));
   v.push_back(std::make_pair("M4",  Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("A1",  Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("A2",  Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("A3",  Make3dVector("1 0 0  0 1 0  0 0 1")));
   return v;
}

DeloneType DeloneTypeList::operator[] (const unsigned long n) const {
   return m_deloneTypes[n];
}

DeloneType DeloneTypeList::operator[] (const std::string& s) const {
   for (unsigned long i = 0; i < m_deloneTypes.size(); ++i)
      if (m_deloneTypes[i].GetName() == s) return m_deloneTypes[i];
   return DeloneType();
}
