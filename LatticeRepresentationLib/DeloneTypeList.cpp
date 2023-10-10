#include "DeloneTypeList.h"

#include "LabeledSellaMatrices.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_MaximaTools.h"
//#include "SanMatrix.h"
#include "LRL_inverse.h"
#include "MatG6.h"
#include "SellaBuild.h"
//
#include <set>
#include <sstream>
#include <string>
 #include <vector>
//
const std::vector<std::string> DeloneTypeList::typelist{ "C1","C3","C5","R1","R3","T1","T2","T5","O1A","O1B","O2",/*"O3",*/"O4","O4B","O5","M1A","M1B","M2A"/*,"M2B"*//*,"M3"*/,"M4"/*,"A1","A2","A3"*/,"H4" };

std::vector<std::pair<std::string, std::string> > DeloneTypeList::CreateBravaisTypeList() {
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
   v.push_back(std::make_pair("O4", "oS"));
   //v.push_back(std::make_pair("O4B", "oS"));
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

std::vector<std::pair<std::string, std::string> > DeloneTypeList::CreateCharacterList() {
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
   //v.push_back(std::make_pair("O4B", "0r0 sst"));
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

std::vector<std::pair<std::string, std::string> > DeloneTypeList::CreateE3CenteringMatrixList() {
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

std::vector<std::pair< std::string, std::vector<MatS6> > > DeloneTypeList::CreateAllPrjs() {
   LabeledSellaMatrices lsm;
   std::vector<LabeledSellaMatrices> vprj = lsm.GetPrjs();
   std::vector<std::pair< std::string, std::vector<MatS6> > > v;
   for (size_t i = 0; i < vprj.size(); ++i) {
      v.push_back(std::make_pair(vprj[i].GetLabel(), vprj[i].GetMatrices()));
   }
   return v;
}

std::vector<std::pair< std::string, std::vector<MatS6> > > DeloneTypeList::CreateAllPerps() {
   LabeledSellaMatrices lsm;
   std::vector<LabeledSellaMatrices> vperp = lsm.GetPerps();
   std::vector<std::pair< std::string, std::vector<MatS6> > > v;
   for (size_t i = 0; i < vperp.size(); ++i) {
      v.push_back(std::make_pair(vperp[i].GetLabel(), vperp[i].GetMatrices()));
   }
   return v;
}

std::vector<std::pair< std::string, std::vector<MatS6> > > DeloneTypeList::CreateAllToCanon() {
   LabeledSellaMatrices lsm;
   std::vector<LabeledSellaMatrices> vToCanon = lsm.GetToCanons( );
   std::vector<std::pair< std::string, std::vector<MatS6> > > v;
   for (size_t i = 0; i < vToCanon.size( ); ++i) {
      v.push_back( std::make_pair( vToCanon[i].GetLabel( ), vToCanon[i].GetMatrices( ) ) );
   }
   return v;
}

DeloneTypeList::DeloneTypeList(const bool build)
{
   static const std::vector<std::pair<std::string, std::string> > characters = CreateCharacterList( );
   static const std::vector<std::pair<std::string, std::string> > e3matrices = CreateE3CenteringMatrixList( );
   static const std::vector<std::pair<std::string, std::string> > bravaisLatticeTypes = CreateBravaisTypeList( );
   static const std::vector<std::pair<std::string, MatS6> >       centeringMatrices = CreateListOfCenteringMatrices( );
   static const std::vector<std::pair<std::string, int> > freeParams = CreateListOfFreeParams( );

   static const std::vector<std::pair<std::string, std::vector<MatS6> > >   prj = CreateAllPrjs( );
   static const std::vector<std::pair<std::string, std::vector<MatS6> > >   perp = CreateAllPerps( );
   static const std::vector<std::pair<std::string, std::vector<MatS6> > >   toCanon = CreateAllToCanon( );




   static std::vector<std::pair<std::string, MatricesForOneDeloneType> > matrices;

   if (build) {
      if (matrices.empty( )) {
         for (size_t i = 0; i < typelist.size( ); ++i) {
            matrices.push_back(
               std::make_pair( typelist[i],
                  MatricesForOneDeloneType(
                     Getter( typelist[i], prj ),
                     Getter( typelist[i], perp ),
                     Getter( typelist[i], toCanon ) ) ) );
         }
      }
   }

   for (size_t i = 0; i < typelist.size( ); ++i) {
      const DeloneType dt( typelist[i],
         Getter( typelist[i], bravaisLatticeTypes ),
         Getter( typelist[i], characters ),
         Getter( typelist[i], e3matrices ),
         Getter( typelist[i], centeringMatrices ),
         Getter( typelist[i], freeParams ),
         Getter( typelist[i], matrices ) );
      if (typelist[i][0] == 'C' || typelist[i] == "C3")
      m_deloneTypes.push_back( dt );
         //std::cout << dt << std::endl;
   }
   //else {
   //   for (size_t i = 0; i < typelist.size( ); ++i) {
   //      //const DeloneType dt( typelist[i],
   //      //   Getter( typelist[i], bravaisLatticeTypes ),
   //      //   Getter( typelist[i], characters ),
   //      //   Getter( typelist[i], e3matrices ),
   //      //   Getter( typelist[i], centeringMatrices ),
   //      //   Getter( typelist[i], matrices ) );
   //      //-------------------------------------
   //      ////std::string m_deloneName;
   //      ////std::string m_bravaisType;
   //      ////std::string m_character;
   //      ////std::string m_toCentered_E3;
   //      ////MatS6       m_toCentered_S6;

   //      ////MatricesForOneDeloneType m_matrices;
   //      //-------------------------------------
   //      Getter( typelist[i], bravaisLatticeTypes );
   //      const DeloneType dt( typelist[i], );
   //      Getter( typelist[i], characters );
   //      Getter( typelist[i], e3matrices );
   //      Getter( typelist[i], centeringMatrices );
   //         Getter( typelist[i], matrices );

   //      m_deloneTypes.push_back( dt );
   //         //std::cout << dt << std::endl;
   //   }
   //}
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
   //std::cout << threespaceMatrix << std::endl;
   //std::cout << std::endl << MatS6::e3Tos6( Make3dVector( threespaceMatrix ) ) << std::endl << std::endl;
   return std::make_pair(lattice, MatS6::e3Tos6(Make3dVector(threespaceMatrix)));
}

std::vector<std::pair<std::string, int > > DeloneTypeList::CreateListOfFreeParams( ) {
   std::vector<std::pair<std::string, int > > v;
   v.push_back( std::make_pair( "C1", 1 ) );
   v.push_back( std::make_pair( "C3", 1 ) );
   v.push_back( std::make_pair( "C5", 1 ) );
   v.push_back( std::make_pair( "R1", 2 ) );
   v.push_back( std::make_pair( "R3", 2 ) );
   v.push_back( std::make_pair( "T1", 2 ) );
   v.push_back( std::make_pair( "T2", 2 ) );
   v.push_back( std::make_pair( "T5", 2 ) );
   v.push_back( std::make_pair( "O1A", 3 ) );
   v.push_back( std::make_pair( "O1B", 3 ) );
   v.push_back( std::make_pair( "O2", 3 ) );
   v.push_back( std::make_pair( "O3", 3 ) );
   v.push_back( std::make_pair( "O4", 3 ) );
   v.push_back( std::make_pair( "O5", 3 ) );
   v.push_back( std::make_pair( "M1A", 4 ) );
   v.push_back( std::make_pair( "M1B", 4 ) );
   v.push_back( std::make_pair( "M2A", 4 ) );
   v.push_back( std::make_pair( "M2B", 4 ) );
   v.push_back( std::make_pair( "M3", 4 ) );
   v.push_back( std::make_pair( "M4", 4 ) );
   v.push_back( std::make_pair( "A1", 5 ) );
   v.push_back( std::make_pair( "A2", 5 ) );
   v.push_back( std::make_pair( "A3", 5 ) );
   v.push_back( std::make_pair( "H4", 2 ) );
   return v;
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

std::vector<std::pair<std::string, std::vector<double> > > DeloneTypeList::Make3dCenteringMatrices() {
   std::vector<std::pair<std::string, std::vector<double> > > v;
   v.push_back(std::make_pair("C1", Make3dVector("0 1 1  1 0 1  1 1 0")));
   v.push_back(std::make_pair("C3", Make3dVector("1 1 0 -1 1 0  1 1 2")));
   v.push_back(std::make_pair("C5",  Make3dVector("1 0 0  0 0 1  0 1 1")));
   v.push_back(std::make_pair("C5", Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("H4", Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("R1", Make3dVector("1 -1 0  0 1 -1  1 1 1")));
   v.push_back(std::make_pair("R3", Make3dVector("1 0 0  0 0 1  1 3 2")));
   v.push_back(std::make_pair("T1", Make3dVector("0 1 1  1 0 1  1 1 0")));
   v.push_back(std::make_pair("T2", Make3dVector("1 0 0  0 1 0  1 1 2")));
   v.push_back(std::make_pair("T5", Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("T5",  Make3dVector("1 0 0  0 0 1  0 1 1")));
   v.push_back(std::make_pair("T5",  Make3dVector("0 1 0  0 1 1  1 0 0")));
   v.push_back(std::make_pair("O1A", Make3dVector("1 1 0 -1 1 0  1 1 2")));
   v.push_back(std::make_pair("O1B", Make3dVector("0 1 1  1 0 1  1 1 0")));
   v.push_back(std::make_pair("O2", Make3dVector("1 0 0  0 1 0  1 1 2")));
   v.push_back(std::make_pair("O3", Make3dVector("0 1 1  1 0 1  1 1 0")));
   v.push_back(std::make_pair("O3",  Make3dVector("1 0 0  0 1 0  1 1 2")));
   v.push_back(std::make_pair("O4",  Make3dVector("2 1 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("O4", Make3dVector("1 -1 0  1 1 0  0 0 1")));
   v.push_back(std::make_pair("O5", Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("O5",  Make3dVector("1 0 0  0 0 1  0 1 1")));
   v.push_back(std::make_pair("M1A", Make3dVector("-1 -1 -1  1 -1 0  0 0 1")));
   v.push_back(std::make_pair("M1B", Make3dVector("0 1 1  1 1 0 -1 0 -1")));
   v.push_back(std::make_pair("M2A", Make3dVector("-1 -1 -2 0 1 0  1 0 0")));
   v.push_back(std::make_pair("M2B", Make3dVector("0 1 1  1 1 0 -1 0 -1"))); //  ?????????????????????????
   v.push_back(std::make_pair("M2B", Make3dVector("-1 -1 -1  1 -1 0  0 0 1"))); //  ?????????????????????????
   v.push_back(std::make_pair("M3", Make3dVector("-1 -1 -2  0 1 0  1 0 0")));
   v.push_back(std::make_pair("M3",  Make3dVector("1 1 0  0 -1 -1 -1 0 -1")));
   v.push_back(std::make_pair("M4", Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("A1", Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("A2", Make3dVector("1 0 0  0 1 0  0 0 1")));
   v.push_back(std::make_pair("A3", Make3dVector("1 0 0  0 1 0  0 0 1")));
   return v;
}

DeloneType DeloneTypeList::operator[] (const size_t n) const {
   return m_deloneTypes[n];
}

bool find_range(const char c, char start, char end) {
   for (char x = start; x < end; ++x) {
      if (x == c) return true;
   }
   return false;
}

DeloneType DeloneTypeList::operator[] (const std::string& s) const {
   for (size_t i = 0; i < m_deloneTypes.size(); ++i)
      if (m_deloneTypes[i].GetName() == s) return m_deloneTypes[i];
   return DeloneType();
}

double Zscore(const S6& s6, const S6& sigmas, const MatS6& reductionMatrix)
{
   const double zscore = s6.Norm() / (reductionMatrix * sigmas).Norm();
   return (zscore < 1.0E-6) ? 0.0 : zscore;
}

double DeloneTypeList::GetFreeParams(const std::string& s) {
   static const std::vector<std::pair< std::string, int> > freeparams = CreateListOfFreeParams();
   size_t foundName = 0;
   for (size_t i = 0; i < freeparams.size(); ++i) {
      if (freeparams[i].first == s) {
         foundName = freeparams[i].second;
         break;
      }
   }
   return double(foundName);
}


std::vector<DeloneFitResults> DeloneTypeList::Fit( const std::string& type, const S6& s6, const S6& sig, const MatS6& reductionMatrix ) const {
   std::vector<DeloneFitResults> v;
   //for (size_t i = 0; i < m_deloneTypes.size( ); ++i) {
   //   const std::string name = m_deloneTypes[i].GetName( );
   //   if (type.empty( ) || name.find( type ) != std::string::npos) {  // LCA make type UC
   //      DeloneFitResults fit = m_deloneTypes[i].GetFit(name, s6, reductionMatrix);
   //      
   //      const double zscore = Zscore( s6-fit.GetBestFit(), sig, reductionMatrix ) * sqrt( GetFreeParams( name ) );
   //      std::cout << s6 << "---------------------" << std::endl;
   //      std::cout << fit.GetBestFit() << std::endl;
   //      std::cout << sig << std::endl;
   //      std::cout << name << std::endl;
   //      std::cout << GetFreeParams(name) << std::endl;
   //      fit.SetZscore( zscore );
   //      fit.SetLatticeType( name );
   //      fit.SetReductionMatrix(reductionMatrix);
   //      fit.SetOriginalInput(s6);
   //      v.push_back( fit );
   //      //std::cout << fit << std::endl << std::endl;
   //      //std::cout << m_deloneTypes[i].GetName() << " in DeloneTypeList::Fit " << std::endl
   //      //   << MatS6().Inverse(reductionMatrix) * fit.GetBestFit() << std::endl
   //      //   << LRL_Cell_Degrees(MatS6().Inverse(reductionMatrix) * fit.GetBestFit()) << std::endl;
   //      //std::cout << s6 << std::endl << fit.GetBestFit() << std::endl;
   //   }
   //}
   return v;
}

std::vector<DeloneFitResults>  DeloneTypeList::Fit( const S6& s6, const S6& sig, const MatS6& reductionMatrix ) const {
   return Fit( "", s6, sig, reductionMatrix );
}

