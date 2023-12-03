
#include <cstdio>
#include <set>
#include <string>

#include "GenerateLatticeTypeExamples.h"

#include "Niggli.h"
#include "LRL_Cell.h"
#include "MatG6.h"
#include "LRL_StringTools.h"

std::ostream& operator<< (std::ostream& o, const GenerateLatticesBase&) {
   o << "operator<< for GenerateLatticesBase" << std::endl;
   return o;
}


std::ostream& operator<< (std::ostream& o, const GenerateNiggliBase& g)
{
   o << "Niggli/Roof name " << g.m_name << std::endl; // either Delone name (C5) or Niggli name (55B)
   o << "General Bravais type " << g.m_bravaisLatticeGeneral << std::endl; // like oS, or mS for face centered
   o << "Bravais lattice type " << g.m_bravaisType << std::endl;
   o << "IT# " << g.m_ITnumber << std::endl;
   o << "Lattice character " << g.m_character << std::endl;
   o << "Number of free parameters " << g.m_freeParams << std::endl;
   o << "Projector premultiplier " << g.m_prjPrefix << std::endl;
   o << "Projector as a string [" << g.m_strProjector << "]" << std::endl;
   o << "Projector " << g.m_projector << std::endl;
   return o;
}



static const double g_delta = 1.0E-6;

 bool GenerateNiggliBase::Approx(const double d1, const double d2) {
   return abs(d1 - d2) < g_delta;
}

 bool GenerateNiggliBase::Approx(const double d1, const double d2, const double d3) {
   return Approx(d1, d2) && Approx(d2, d3);
}

 bool GenerateNiggliBase::Approx(const G6& g1, const G6& g2) {
   return (g1 - g2).norm() < g_delta;
}

MatG6  MatrixE3ToG6(const Matrix_3x3& m) {
   return MatG6(MatS6::e3Tos6(Matrix_3x3(m).GetVector()));
}

bool GenerateNiggliBase::IsTypeI(const G6& g) const {
   return (g[3] <= 0) && (g[4] <= 0) && (g[5] <= 0);
}

bool GenerateNiggliBase::IsTypeII(const G6& g) const {
   //std::cout << g << std::endl;
   //std::cout << (g[3] < 0) << std::endl;
   //std::cout << (g[4] < 0) << std::endl;
   //std::cout << (g[5] < 0) << std::endl;
   return (g[3] > 0) && (g[4] > 0) && (g[5] > 0);
}

GenerateLatticesBase::GenerateLatticesBase()
//: m_name("BASE")
//, m_bravaisType("BASE")
//, m_ITnumber("BASE")
//, m_character("BASE")
 //,m_DC7_character("DC7_character NA")
//, m_prjPrefix(-19191.0E19)
//, m_strProjector("BASE")
//, m_projector("BASE")  // from Paciorek & Bonin, 1992
{
}

IT_1::IT_1() : GenerateNiggliBase() {
   m_name = "44C";
   m_bravaisLatticeGeneral = "cF";
   m_bravaisType = "cF";
   m_ITnumber = "1";
   m_freeParams = 1;
   m_character = "[r, r, r, r, r, r]";
   //m_DC7_character = "[r,r,r,r,r,r,2r]";
   m_prjPrefix = 1.0 / 6.0;
   m_strProjector = "1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 -1 1 1 1 -1 -1 1 1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.333333 0.333333 0.333333 0.333333 0.333333 0.333333
   0.333333 0.333333 0.333333 0.333333 0.333333 0.333333
   0.333333 0.333333 0.333333 0.333333 0.333333 0.333333
   0.333333 0.333333 0.333333 0.333333 0.333333 0.333333
   0.333333 0.333333 0.333333 0.333333 0.333333 0.333333
   0.333333 0.333333 0.333333 0.333333 0.333333 0.333333 ;
   */
} // end of class IT_1 constructor

std::vector<G6>
GenerateNiggliBase::Generate(const std::string& s, const size_t n/* = 1*/)
{
   const std::vector<std::shared_ptr<GenerateNiggliBase> >
      vglb = GenerateNiggliBase().Select(s);
   std::vector<G6> out;

   for (size_t i = 0; i < n; ++i) {
      for (size_t k = 0; k < vglb.size(); ++k) {
         const S6 dummyS6(S6().randDeloneReduced());
         G6 g6red;
         const bool b = Niggli::Reduce(dummyS6, g6red);
         const std::shared_ptr<GenerateNiggliBase> ptNiggli = vglb[k];
         const G6 prj = ptNiggli->GetPrj() * g6red;

         if (LRL_Cell(prj).IsValid()) {
            out.push_back(prj);
            //   std::cout
            //      << ptNiggli->GetITNum() << " "
            //      << ptNiggli->GetName() << " "
            //      << ptNiggli->GetBravaisType() << " "
            //      << prj << std::endl;
            //}
         }
         std::cout << std::endl;
      }
   }
   return out;
}


IT_2::IT_2() : GenerateNiggliBase() {
   m_name = "49C";
   m_bravaisLatticeGeneral = "hR";
   m_bravaisType = "hR";
   m_ITnumber = "2";
   m_freeParams = 2;
   m_character = "[r, r, r, s, s, s]";
   //m_DC7_character = "[r,r,r,2r-s,2-s,3r-s]";
   m_prjPrefix = 1.0/3.0;
   m_strProjector = "1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0  0 0 0 1 1 1  0 0 0 1 1 1  0 0 0 1 1 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 -1 0 -1 0 1 -1 -1 -1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.333333 0.333333 0.333333 0 0 0
   0.333333 0.333333 0.333333 0 0 0
   0.333333 0.333333 0.333333 0 0 0
   0 0 0 0.333333 0.333333 0.333333
   0 0 0 0.333333 0.333333 0.333333
   0 0 0 0.333333 0.333333 0.333333 ;
   */
} // end of class IT_2 constructor

IT_3::IT_3() : GenerateNiggliBase() {
   m_name = "44A";
   m_bravaisLatticeGeneral = "cP";
   m_bravaisType = "cP";
   m_ITnumber = "3";
   m_freeParams = 1;
   m_character = "[r, r, r, 0, 0, 0]";
   //m_DC7_character = "[r,r,r,2r,2r,2r,2r]";
   m_prjPrefix = 1.0 / 3.0;
   m_strProjector = "1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 0 0 0 1 0 0 0 1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.333333 0.333333 0.333333 0 0 0
   0.333333 0.333333 0.333333 0 0 0
   0.333333 0.333333 0.333333 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_3 constructor

IT_4::IT_4() : GenerateNiggliBase() {
   m_name = "49D";
   m_bravaisLatticeGeneral = "hR";
   m_bravaisType = "hR";
   m_ITnumber = "4";
   m_freeParams = 2;
   m_character = "[r, r, r, -s, -s, -s]";
   m_prjPrefix = 1.0/3.0;
   m_strProjector = "1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0  0 0 0 1 1 1  0 0 0 1 1 1  0 0 0 1 1 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 -1 0 -1 0 1 -1 -1 -1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.333333 0.333333 0.333333 0 0 0
   0.333333 0.333333 0.333333 0 0 0
   0.333333 0.333333 0.333333 0 0 0
   0 0 0 0.333333 0.333333 0.333333
   0 0 0 0.333333 0.333333 0.333333
   0 0 0 0.333333 0.333333 0.333333 ;
   */
} // end of class IT_4 constructor

IT_5::IT_5() : GenerateNiggliBase() {
   m_name = "44B";
   m_bravaisLatticeGeneral = "cI";
   m_bravaisType = "cI";
   m_ITnumber = "5";
   m_freeParams = 1;
   m_character = "[3r, 3r, 3r, -2r, -2r, -2r]";
   //m_DC7_character = "[r,r,r,r,4r/3,4r/3,4r/3]";
   m_prjPrefix = 1.0/39.;
   m_strProjector = "9 9 9 -6 -6 -6  9 9 9 -6 -6 -6  9 9 9 -6 -6 -6 -6 -6 -6 4 4 4 -6 -6 -6 4 4 4 -6 -6 -6 4 4 4";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 0 1 1 1 0 0 1 1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   -0.153846 -0.153846 -0.153846 0.102564 0.102564 0.102564
   -0.153846 -0.153846 -0.153846 0.102564 0.102564 0.102564
   -0.153846 -0.153846 -0.153846 0.102564 0.102564 0.102564 ;
   */
} // end of class IT_5 constructor

IT_6::IT_6() : GenerateNiggliBase() {
   m_name = "45D";
   m_bravaisLatticeGeneral = "tI";
   m_bravaisType = "tI";
   m_ITnumber = "6";
   m_freeParams = 2;
   m_character = "[r, r, r, -r+s, -r+s, -2s]";
   m_prjPrefix = 1.0/26.0;
   m_strProjector = "6 6 6 -4 -4 -4  6 6 6 -4 -4 -4  6 6 6 -4 -4 -4   -4 -4 -4 7 7 -6  -4 -4 -4 7 7 -6  -4 -4 -4 -6 -6 20";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "0 1 1 1 0 1 1 1 0 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   -0.153846 -0.153846 -0.153846 0.269231 0.269231 -0.230769
   -0.153846 -0.153846 -0.153846 0.269231 0.269231 -0.230769
   -0.153846 -0.153846 -0.153846 -0.230769 -0.230769 0.769231 ;
   */
} // end of class IT_6 constructor

IT_7::IT_7() : GenerateNiggliBase() {
   m_name = "45D";
   m_bravaisLatticeGeneral = "tI";
   m_bravaisType = "tI";
   m_ITnumber = "7";
   m_freeParams = 2;
   m_character = "[r, r, r, 2s, -r -s, -r -s]";
   m_prjPrefix = 1.0/26.0;
   m_strProjector = "6 6 6 -4 -4 -4  6 6 6 -4 -4 -4  6 6 6 -4 -4 -4 -4 -4 -4 20 -6 -6 -4 -4 -4 -6 7 7 -4 -4 -4 -6 7 7";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 0 1 1 1 0 0 1 1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   -0.153846 -0.153846 -0.153846 0.769231 -0.230769 -0.230769
   -0.153846 -0.153846 -0.153846 -0.230769 0.269231 0.269231
   -0.153846 -0.153846 -0.153846 -0.230769 0.269231 0.269231 ;
   */
} // end of class IT_7 constructor

IT_8::IT_8() : GenerateNiggliBase() {
   m_name = "52A";
   m_bravaisLatticeGeneral = "oI";
   m_bravaisType = "oI";
   m_ITnumber = "8";
   m_freeParams = 3;
   m_character = "[r, r, r, -s, -t, -2r+s+t]";
   m_prjPrefix = 1.0 / 13.0;
   m_strProjector = "3 3 3 -2 -2 -2   3 3 3 -2 -2 -2   3 3 3 -2 -2 -2   -2 -2 -2 10 -3 -3   -2 -2 -2 -3 10 -3   -2 -2 -2 -3 -3 10";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = " -1 -1 0 -1 0 -1 0 -1 -1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   -0.153846 -0.153846 -0.153846 0.769231 -0.230769 -0.230769
   -0.153846 -0.153846 -0.153846 -0.230769 0.769231 -0.230769
   -0.153846 -0.153846 -0.153846 -0.230769 -0.230769 0.769231 ;
   */
} // end of class IT_8 constructor

IT_9::IT_9() : GenerateNiggliBase() {
   m_name = "49B";
   m_bravaisLatticeGeneral = "hR";
   m_bravaisType = "hR";
   m_ITnumber = "9";
   m_freeParams = 2;
   m_character = "[r, r, s, r, r, r]";
   m_prjPrefix = 0.2;
   m_strProjector = "1 1 0 1 1 1  1 1 0 1 1 1  0 0 5 0 0 0  1 1 0 1 1 1  1 1 0 1 1 1  1 1 0 1 1 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 0 0 -1 1 0 -1 -1 3 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.2 0.2 0 0.2 0.2 0.2
   0.2 0.2 0 0.2 0.2 0.2
   0 0 1 0 0 0
   0.2 0.2 0 0.2 0.2 0.2
   0.2 0.2 0 0.2 0.2 0.2
   0.2 0.2 0 0.2 0.2 0.2 ;
   */
} // end of class IT_9 constructor

IT_10::IT_10() : GenerateNiggliBase() {
   m_name = "55A";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_ITnumber = "10";
   m_freeParams = 4;
   m_character = "[r, r, s, t, t, u]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 1 0 0 0 0  1 1 0 0 0 0  0 0 2 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 2";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 1 0 1 -1 0 0 0 -1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.5 0.5 0 0 0 0
   0.5 0.5 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0.5 0.5 0
   0 0 0 0.5 0.5 0
   0 0 0 0 0 1 ;
   */
} // end of class IT_10 constructor

IT_11::IT_11() : GenerateNiggliBase() {
   m_name = "45A";
   m_bravaisLatticeGeneral = "tP";
   m_bravaisType = "tP";
   m_ITnumber = "11";
   m_freeParams = 2;
   m_character = "[r, r, s, 0,0,0]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 1 0 0 0 0  1 1 0 0 0 0  0 0 2 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 0 0 0 1 0 0 0 1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.5 0.5 0 0 0 0
   0.5 0.5 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_11 constructor

IT_12::IT_12() : GenerateNiggliBase() {
   m_name = "48A";
   m_bravaisLatticeGeneral = "hP";
   m_bravaisType = "hP";
   m_ITnumber = "12";
   m_freeParams = 2;
   m_character = "[r, r, s, 0, 0, -r]";
   m_prjPrefix = 1.0 / 3.0;
   m_strProjector = "1 1 0 0 0 -1  1 1 0 0 0 -1  0 0 3 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0 -1 -1 0 0 0 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 0 0 0 1 0 0 0 1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.333333 0.333333 0 0 0 -0.333333
   0.333333 0.333333 0 0 0 -0.333333
   0 0 1 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   -0.333333 -0.333333 0 0 0 0.333333 ;
   */
} // end of class IT_12 constructor

IT_13::IT_13() : GenerateNiggliBase() {
   m_name = "50D";
   m_bravaisLatticeGeneral = "oS";
   m_bravaisType = "oC";
   m_ITnumber = "13";
   m_freeParams = 3;
   m_character = "[r, r, s, 0, 0, -t]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 1 0 0 0 0  1 1 0 0 0 0  0 0 2 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 2";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 1 0 -1 1 0 0 0 1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.5 0.5 0 0 0 0
   0.5 0.5 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 1 ;
   */
} // end of class IT_13 constructor

IT_14::IT_14() : GenerateNiggliBase() {
   m_name = "55A";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_ITnumber = "14";
   m_freeParams = 4;
   m_character = "[r, r, s, t, t, u]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 1 0 0 0 0  1 1 0 0 0 0  0 0 2 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 2";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 1 0 -1 1 0 0 0 1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.5 0.5 0 0 0 0
   0.5 0.5 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0.5 0.5 0
   0 0 0 0.5 0.5 0
   0 0 0 0 0 1 ;
   */
} // end of class IT_14 constructor

IT_15::IT_15() : GenerateNiggliBase() {
   m_name = "45C";
   m_bravaisLatticeGeneral = "tI";
   m_bravaisType = "tI";
   m_ITnumber = "15";
   m_freeParams = 2;
   m_character = "[r, r, s, -r, -r, 0]";
   m_prjPrefix = 0.25;
   m_strProjector = "1 1 0 -1 -1 0  1 1 0 -1 -1 0  0 0 4 0 0 0  -1 -1 0 1 1 0  -1 -1 0 1 1 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 0 0 0 1 0 1 1 2 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.25 0.25 0 -0.25 -0.25 0
   0.25 0.25 0 -0.25 -0.25 0
   0 0 1 0 0 0
   -0.25 -0.25 0 0.25 0.25 0
   -0.25 -0.25 0 0.25 0.25 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_15 constructor

IT_16::IT_16() : GenerateNiggliBase() {
   m_name = "51A";
   m_bravaisLatticeGeneral = "oF";
   m_bravaisType = "oF";
   m_ITnumber = "16";
   m_freeParams = 3;
   m_character = "r, r, s, -t, -t, -2r -2t]";
   m_prjPrefix = 0.1;
   m_strProjector = "3 3 0 -2 -2 -2  3 3 0 -2 -2 -2  0 0 10 0 0 0  -2 -2 0 3 3 -2  -2 -2 0 3 3 -2  -2 -2 0 -2 -2 8";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = " -1 -1 0 1 -1 0 1 1 2 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.3 0.3 0 -0.2 -0.2 -0.2
   0.3 0.3 0 -0.2 -0.2 -0.2
   0 0 1 0 0 0
   -0.2 -0.2 0 0.3 0.3 -0.2
   -0.2 -0.2 0 0.3 0.3 -0.2
   -0.2 -0.2 0 -0.2 -0.2 0.8 ;
   */
} // end of class IT_16 constructor

IT_17::IT_17() : GenerateNiggliBase() {
   m_name = "57B";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_ITnumber = "17";
   m_freeParams = 4;
   //m_character = "[r, r, s, t, u, -2r -t -u]";
   m_character = "[r, r, s, -2t, -2u, -2r -t -u]"; // corrected LCA 2021-10-30
   m_prjPrefix = 0.1;
   m_strProjector = "3 3 0 -2 -2 -2  3 3 0 -2 -2 -2  0 0 10 0 0 0 -2 -2 0 8 -2 -2 -2 -2 0 -2 8 -2 -2 -2 0 -2 -2 8";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 -1 0 1 1 0 -1 0 -1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.3 0.3 0 -0.2 -0.2 -0.2
   0.3 0.3 0 -0.2 -0.2 -0.2
   0 0 1 0 0 0
   -0.2 -0.2 0 0.8 -0.2 -0.2
   -0.2 -0.2 0 -0.2 0.8 -0.2
   -0.2 -0.2 0 -0.2 -0.2 0.8 ;
   */
} // end of class IT_17 constructor

IT_18::IT_18() : GenerateNiggliBase() {
   m_name = "45E";
   m_bravaisLatticeGeneral = "tI";
   m_bravaisType = "tI";
   m_ITnumber = "18";
   m_freeParams = 2;
   m_character = "[2r, s, s, r, 2r, 2r]";
   m_prjPrefix = 1.0/26.0;
   m_strProjector = "8 0 0 4 8 8  0 13 13 0 0 0  0 13 13 0 0 0  4 0 0 2 4 4  8 0 0 4 8 8  8 0 0 4 8 8";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "0 -1 1   1 -1 -1   1 0 0 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.307692 0 0 0.153846 0.307692 0.307692
   0 0.5 0.5 0 0 0
   0 0.5 0.5 0 0 0
   0.153846 0 0 0.0769231 0.153846 0.153846
   0.307692 0 0 0.153846 0.307692 0.307692
   0.307692 0 0 0.153846 0.307692 0.307692 ;
   */
} // end of class IT_18 constructor

IT_19::IT_19() : GenerateNiggliBase() {
   m_name = "52B";
   m_bravaisLatticeGeneral = "oI";
   m_bravaisType = "oI";
   m_ITnumber = "19";
   m_freeParams = 3;
   m_character = "[r, s, s, t, r, r]";
   m_prjPrefix = 1.0/6.0;
   m_strProjector = "2 0 0 0 2 2  0 3 3 0 0 0  0 3 3 0 0 0  0 0 0 6 0 0  2 0 0 0 2 2  2 0 0 0 2 2";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = " -1 0 0   0 -1 1   -1 1 1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.333333 0 0 0 0.333333 0.333333
   0 0.5 0.5 0 0 0
   0 0.5 0.5 0 0 0
   0 0 0 1 0 0
   0.333333 0 0 0 0.333333 0.333333
   0.333333 0 0 0 0.333333 0.333333 ;
   */
} // end of class IT_19 constructor

IT_20::IT_20() : GenerateNiggliBase() {
   m_name = "55B";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_ITnumber = "20";
   m_freeParams = 4;
   m_character = "[r, s, s, t, u, u]";
   m_prjPrefix = 0.5;
   m_strProjector = "2 0 0 0 0 0  0 1 1 0 0 0  0 1 1 0 0 0  0 0 0 2 0 0  0 0 0 0 1 1  0 0 0 0 1 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "0 1 1 0 1 -1 -1 0 0 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 1 0 0 0 0 0
   0 0.5 0.5 0 0 0
   0 0.5 0.5 0 0 0
   0 0 0 1 0 0
   0 0 0 0 0.5 0.5
   0 0 0 0 0.5 0.5 ;
   */
} // end of class IT_20 constructor

IT_21::IT_21() : GenerateNiggliBase() {
   m_name = "45B";
   m_bravaisLatticeGeneral = "tP";
   m_bravaisType = "tP";
   m_ITnumber = "21";
   m_freeParams = 2;
   m_character = "[r, s, s, 0, 0, 0]";
   //m_DC7_character = "[r,s,s,s+r,s+r,s+r,d+r]";
   m_prjPrefix = 0.5;
   m_strProjector = "2 0 0 0 0 0  0 1 1 0 0 0  0 1 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "0 1 0 0 0 1 1 0 0 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 1 0 0 0 0 0
   0 0.5 0.5 0 0 0
   0 0.5 0.5 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_21 constructor

IT_22::IT_22() : GenerateNiggliBase() {
   m_name = "48B";
   m_bravaisLatticeGeneral = "hP";
   m_bravaisType = "hP";
   m_ITnumber = "22";
   m_freeParams = 2;
   m_character = "[r, s, s, -s, 0, 0]";
   //m_DC7_character = "[r, s, s, s, s + r, s + r, s + r]";
   m_prjPrefix = 1.0 / 3.0;
   m_strProjector = "3 0 0 0 0 0  0 1 1 -1 0 0  0 1 1 -1 0 0  0 -1 -1 1 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "0 1 0 0 0 1 1 0 0 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 1.5 0 0 0 0 0
   0 0.5 0.5 -0.5 0 0
   0 0.5 0.5 -0.5 0 0
   0 -0.5 -0.5 0.5 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_22 constructor

IT_23::IT_23() : GenerateNiggliBase() {
   m_name = "50E";
   m_bravaisLatticeGeneral = "oS";
   m_bravaisType = "oC";
   m_ITnumber = "23";
   m_freeParams = 3;
   m_character = "[r, s, s, -t, 0, 0]";
   m_prjPrefix = 0.5;
   m_strProjector = "2 0 0 0 0 0  0 1 1 0 0 0  0 1 1 0 0 0  0 0 0 2 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "0 1 1 0 -1 1 1 0 0 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 1 0 0 0 0 0
   0 0.5 0.5 0 0 0
   0 0.5 0.5 0 0 0
   0 0 0 1 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_23 constructor

IT_24::IT_24() : GenerateNiggliBase() {
   m_name = "49E";
   m_bravaisLatticeGeneral = "hR";
   m_bravaisType = "hR";
   m_ITnumber = "24";
   m_freeParams = 2;
   m_character = "[3r, s, s, -s, -s+r, -2r, -2r]";
   //m_DC7_character = "[r,s,s,s,s+r/3,s+r/3,s+r/3]";
   m_prjPrefix = 1.0 / 53.0;
   m_strProjector = "27 3 3 6 -18 -18  3 18 18 -17 -2 -2  3 18 18 -17 -2 -2  6 -17 -17 19 -4 -4  -18 -2 -2 -4 12 12  -18 -2 -2 -4 12 12";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 2 1 0 -1 1 1 0 0 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.509434 0.0566038 0.0566038 0.113208 -0.339623 -0.339623
   0.0566038 0.339623 0.339623 -0.320755 -0.0377358 -0.0377358
   0.0566038 0.339623 0.339623 -0.320755 -0.0377358 -0.0377358
   0.113208 -0.320755 -0.320755 0.358491 -0.0754717 -0.0754717
   0.339623 -0.0377358 -0.0377358 -0.0754717 0.226415 0.226415
   -0.339623 -0.0377358 -0.0377358 -0.0754717 0.226415 0.226415 ;
   */
} // end of class IT_24 constructor

IT_25::IT_25() : GenerateNiggliBase() {
   m_name = "55B";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_ITnumber = "25";
   m_freeParams = 4;
   m_character = "[r, s, s, -t, -u, -u]";
   m_prjPrefix = 0.5;
   m_strProjector = "2 0 0 0 0 0  0 1 1 0 0 0  0 1 1 0 0 0  0 0 0 2 0 0  0 0 0 0 1 1  0 0 0 0 1 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "0 1 1 0 -1 1 1 0 0 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 1 0 0 0 0 0
   0 0.5 0.5 0 0 0
   0 0.5 0.5 0 0 0
   0 0 0 1 0 0
   0 0 0 0 0.5 0.5
   0 0 0 0 0.5 0.5 ;
   */
} // end of class IT_25 constructor

IT_26::IT_26() : GenerateNiggliBase() {
   m_name = "51B";
   m_bravaisLatticeGeneral = "oF";
   m_bravaisType = "oF";
   m_ITnumber = "26";
   m_freeParams = 3;
   m_character = "[2r, s, t, r, 2r, 2r]";
   m_prjPrefix = 1.0/13.0;
   m_strProjector = "4 0 0 2 4 4  0 13 0 0 0 0  0 0 13 0 0 0  2 0 0 1 2 2  4 0 0 2 4 4  4 0 0 2 4 4";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 0 0   -1 2 0   -1 0 2 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.307692 0 0 0.153846 0.307692 0.307692
   0 1 0 0 0 0
   0 0 1 0 0 0
   0.153846 0 0 0.0769231 0.153846 0.153846
   0.307692 0 0 0.153846 0.307692 0.307692
   0.307692 0 0 0.153846 0.307692 0.307692 ;
   */
} // end of class IT_26 constructor

IT_27::IT_27() : GenerateNiggliBase() {
   m_name = "57C";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_ITnumber = "27";
   m_freeParams = 4;
   m_character = "[r, s, t, u, r, r]";
   m_prjPrefix = 1.0/3.0;
   m_strProjector = "1 0 0 0 1 1  0 3 0 0 0 0  0 0 3 0 0 0  0 0 0 3 0 0  1 0 0 0 1 1  1 0 0 0 1 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "-1 2 0   -1 0 0   0 -1 1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.333333 0 0 0 0.333333 0.333333
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 1 0 0
   0.333333 0 0 0 0.333333 0.333333
   0.333333 0 0 0 0.333333 0.333333 ;
   */
} // end of class IT_27 constructor

IT_28::IT_28() : GenerateNiggliBase() {
   m_name = "56A";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_ITnumber = "28";
   m_freeParams = 4;
   m_character = "[r, s, t, u, r, 2u]";
   m_prjPrefix = 0.1;
   m_strProjector = "5 0 0 0 5 0  0 10 0 0 0 0  0 0 10 0 0 0  0 0 0 2 0 4  5 0 0 0 5 0  0 0 0 4 0 8";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "-1 0 0   -1 0 2  0 1 0 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.5 0 0 0 0.5 0
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0.2 0 0.4
   0.5 0 0 0 0.5 0
   0 0 0 0.4 0 0.8 ;
   */
} // end of class IT_28 constructor

IT_29::IT_29() : GenerateNiggliBase() {
   m_name = "56C";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_ITnumber = "29";
   m_freeParams = 4;
   m_character = "[r, s, t, u, 2u, r]";
   m_prjPrefix = 0.1;
   m_strProjector = "5 0 0 0 0 5  0 10 0 0 0 0  0 0 10 0 0 0  0 0 0 2 4 0  0 0 0 4 8 0  5 0 0 0 0 5";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 0 0 1 -2 0 0 0 -1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.5 0 0 0 0 0.5
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0.2 0.4 0
   0 0 0 0.4 0.8 0
   0.5 0 0 0 0 0.5 ;
   */
} // end of class IT_29 constructor

IT_30::IT_30() : GenerateNiggliBase() {
   m_name = "56B";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_ITnumber = "30";
   m_freeParams = 4;
   m_character = "[r, s, t, s, u, 2u]";
   m_prjPrefix = 0.1;
   m_strProjector = "10 0 0 0 0 0  0 5 0 5 0 0  0 0 10 0 0 0  0 5 0 5 0 0  0 0 0 0 2 4  0 0 0 0 4 8";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "0 1 0 0 1 -2 -1 0 0 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 1 0 0 0 0 0
   0 0.5 0 0.5 0 0
   0 0 1 0 0 0
   0 0.5 0 0.5 0 0
   0 0 0 0 0.2 0.4
   0 0 0 0 0.4 0.8 ;
   */
} // end of class IT_30 constructor

IT_31::IT_31() : GenerateNiggliBase() {
   m_name = "58B";
   m_bravaisType = "aP";
   m_bravaisLatticeGeneral = "aP";
   m_ITnumber = "31";
   m_freeParams = 6;
   m_character = "[r, s, t, u, v, w]";
   m_prjPrefix = 1.0;
   m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 0 0 0 1 0 0 0 1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
} // end of class IT_Lat_Char_31 constructor

IT_32::IT_32() : GenerateNiggliBase() {
   m_name = "50C";
   m_bravaisLatticeGeneral = "oP";
   m_bravaisType = "oP";
   m_ITnumber = "32";
   m_freeParams = 3;
   m_character = "[r, s, t, 0, 0, 0]";
   m_prjPrefix = 1;
   m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 0 0 0 1 0 0 0 1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 1 0 0 0 0 0
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_32 constructor

IT_33::IT_33() : GenerateNiggliBase() {
   m_name = "53A";
   m_bravaisLatticeGeneral = "mP";
   m_bravaisType = "mP";
   m_ITnumber = "33";
   m_freeParams = 4;
   m_character = "[r, s, t, 0, u, 0]";
   m_prjPrefix = 1;
   m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 0 0 0 1 0 0 0 1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 1 0 0 0 0 0
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0 0 0
   0 0 0 0 1 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_33 constructor


// NOTE: Pacioric and Bonin seem to have reversed 34/35
// Also Andrews&Bernstein, 1988 
// what does the IT say?


IT_35::IT_35() : GenerateNiggliBase() {
   m_name = "53C";
   m_bravaisLatticeGeneral = "mP";
   m_bravaisType = "mP";
   m_ITnumber = "35";
   m_freeParams = 4;
   m_character = "[r, s, t, 0, 0, -u]";
   m_prjPrefix = 1;
   m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 0 0 1  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "0 -1 0 -1 0 0 0 0 -1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 1 0 0 0 0 0
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 1 ;
   */
} // end of class IT_35 constructor

IT_34::IT_34() : GenerateNiggliBase() {
   m_name = "53B";
   m_bravaisLatticeGeneral = "mP";
   m_bravaisType = "mP";
   m_ITnumber = "34";
   m_freeParams = 4;
   m_character = "[r, s, t, -u, 0, 0]";
   m_prjPrefix = 1;
   m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "-1 0 0  0 0 -1  0 -1 0";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 1 0 0 0 0 0
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 1 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_34 constructor

IT_36::IT_36() : GenerateNiggliBase() {
   m_name = "50A";
   m_bravaisLatticeGeneral = "oS";
   m_bravaisType = "oC";
   m_ITnumber = "36";
   m_freeParams = 3;
   m_character = "[r,s, t, 0, -r, 0]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 0 0 0 -1 0  0 2 0 0 0 0  0 0 2 0 0 0  0 0 0 0 0 0  -1 0 0 0 1 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 0 0 -1 0 -2 0 1 0 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.5 0 0 0 -0.5 0
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0 0 0
   -0.5 0 0 0 0.5 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_36 constructor

IT_37::IT_37() : GenerateNiggliBase() {
   m_name = "54C";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_ITnumber = "37";
   m_freeParams = 4;
   m_character = "[r, s, t, u, -r, 0]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 0 0 0 -1 0  0 2 0 0 0 0  0 0 2 0 0 0  0 0 0 2 0 0 -1 0 0 0 1 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 0 2 1 0 0 0 1 0 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.5 0 0 0 -0.5 0
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 1 0 0
   -0.5 0 0 0 0.5 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_37 constructor

IT_38::IT_38() : GenerateNiggliBase() {
   m_name = "50B";
   m_bravaisLatticeGeneral = "oS";
   m_bravaisType = "oC";
   m_ITnumber = "38";
   m_freeParams = 3;
   m_character = "[r, s, t, 0, 0, -r]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 0 0 0 0 -1   0 2 0 0 0 0   0 0 2 0 0 0   0 0 0 0 0 0   0 0 0 0 0 0  -1 0 0 0 0 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "-1 0 0   1 2 0  0 0 -1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.5 0 0 0 0 -0.5
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   -0.5 0 0 0 0 0.5 ;
   */
} // end of class IT_38 constructor

IT_39::IT_39() : GenerateNiggliBase() {
   m_name = "54A";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_ITnumber = "39";
   m_freeParams = 4;
   m_character = "[r, s, t, u, 0, -r]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 0 0 0 0 -1  0 2 0 0 0 0  0 0 2 0 0 0  0 0 0 2 0 0  0 0 0 0 0 0  -1 0 0 0 0 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = " -1 -2 0  -1 0 0  0 0 -1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.5 0 0 0 0 -0.5
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 1 0 0
   0 0 0 0 0 0
   -0.5 0 0 0 0 0.5 ;
   */
} // end of class IT_39 constructor

IT_40::IT_40() : GenerateNiggliBase() {
   m_name = "50F";
   m_bravaisLatticeGeneral = "oS";
   m_bravaisType = "oC";
   m_ITnumber = "40";
   m_freeParams = 3;
   m_character = "[r, s, t, -s, 0, 0]";
   m_prjPrefix = 0.5;
   m_strProjector = "2 0 0 0 0 0  0 1 0 -1 0 0  0 0 2 0 0 0  0 -1 0 1 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "0 -1 0 0 1 2 -1 0 0 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 1 0 0 0 0 0
   0 0.5 0 -0.5 0 0
   0 0 1 0 0 0
   0 -0.5 0 0.5 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_40 constructor

IT_41::IT_41() : GenerateNiggliBase() {
   m_name = "54B";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_ITnumber = "41";
   m_freeParams = 4;
   m_character = "[r, s, t, -s, u, 0]";
   m_prjPrefix = 0.5;
   m_strProjector = "2 0 0 0 0 0  0 1 0 -1 0 0  0 0 2 0 0 0  0 -1 0 1 0 0  0 0 0 0 2 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "0 -1 -2 0 -1 0 -1 0 0 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 1 0 0 0 0 0
   0 0.5 0 -0.5 0 0
   0 0 1 0 0 0
   0 -0.5 0 0.5 0 0
   0 0 0 0 1 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_41 constructor

IT_42::IT_42() : GenerateNiggliBase() {
   m_name = "52C";
   m_bravaisLatticeGeneral = "oI";
   m_bravaisType = "oI";
   m_ITnumber = "42";
   m_freeParams = 3;
   m_character = "[r, s, t, -s, -r, 0]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 0 0 0 -1 0  0 1 0 -1 0 0  0 0 2 0 0 0  0 -1 0 1 0 0 -1 0 0 0 1 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = " -1 0 0 0 -1 0 1 1 2 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.5 0 0 0 -0.5 0
   0 0.5 0 -0.5 0 0
   0 0 1 0 0 0
   0 -0.5 0 0.5 0 0
   -0.5 0 0 0 0.5 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_42 constructor

IT_43::IT_43() : GenerateNiggliBase() {
   m_name = "57A";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mI";
   m_ITnumber = "43";
   m_freeParams = 4;
   m_character = "[r, s, t, -s -u, -r -u, 2u]";
   m_prjPrefix = 1.0/20.0;
   m_strProjector = "11 1 0 1 -9 -4  1 11 0 -9 1 -4  0 0 20 0 0 0  1 -9 0 11 1 -4 -9 1 0 1 11 -4 -4 -4 0 -4 -4 16";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = " -1 0 0 -1 -1 -2 0 -1 0 ";
   //m_toCentered_E3 = " 1 0 0  1 1 2  0 11 "; from Roof
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
   /*
      m_projector = 0.55 0.05 0 0.05 -0.45 -0.2
   0.05 0.55 0 -0.45 0.05 -0.2
   0 0 1 0 0 0
   0.05 -0.45 0 0.55 0.05 -0.2
   -0.45 0.05 0 0.05 0.55 -0.2
   -0.2 -0.2 0 -0.2 -0.2 0.8 ;
   */
} // end of class IT_43 constructor

IT_44::IT_44() : GenerateNiggliBase() {
   m_name = "58B";
   m_bravaisType = "aP";
   m_bravaisLatticeGeneral = "aP";
   m_ITnumber = "44";
   m_freeParams = 6;
   m_character = "[r, s, t, -u, -v, -w]";
   m_prjPrefix = 1.0;
   m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   m_toCentered_E3 = "1 0 0 0 1 0 0 0 1 ";
   m_toCentered = MatrixE3ToG6(m_toCentered_E3);
} // end of class IT_Lat_Char_44 constructor


Delone_C1::Delone_C1() : GenerateDeloneBase() {
   m_name = "C1";
   m_bravaisLatticeGeneral = "cI";
   m_bravaisType = "cI";
   m_strProjector = "1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1";
   m_character = "rrr rrr";
   m_freeParams = 1;
   m_prjPrefix = 1.0 / 6.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_C1().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_C1().GetMatrices();
}

Delone_C3::Delone_C3() {
   m_name = "C3";
   m_bravaisLatticeGeneral = "cF";
   m_bravaisType = "cF";
   m_strProjector = "1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 0  1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 0";
   m_character = "rr0 rr0";
   m_freeParams = 1;
   m_prjPrefix = 1.0 / 4.0 ;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_C3().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_C3().GetMatrices();
}

Delone_C5::Delone_C5() {
   m_name = "C5";
   m_bravaisLatticeGeneral = "cP";
   m_bravaisType = "cP";
   m_strProjector = "0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 1 1 1  0 0 0 1 1 1  0 0 0 1 1 1";
   m_character = "000 rrr";
   m_freeParams = 1;
   m_prjPrefix = 1.0/3.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_C5().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_C5().GetMatrices();
}

Delone_T1::Delone_T1() {
   m_name = "T1";
   m_bravaisLatticeGeneral = "tI";
   m_bravaisType = "tI";
   m_strProjector = "1 1 0 1 1 0  1 1 0 1 1 0  0 0 2 0 0 2  1 1 0 1 1 0  1 1 0 1 1 0  0 0 2 0 0 2";
   m_character = "rrs rrs";
   m_freeParams = 2;
   m_prjPrefix = 1.0 / 4.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_T1().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_T1().GetMatrices();
}

Delone_T2::Delone_T2() {
   m_name = "T2";
   m_bravaisLatticeGeneral = "tI";
   m_bravaisType = "tI";
   m_strProjector = "1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 0  1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 4";
   m_character = "rr0 rrs";
   m_freeParams = 2;
   m_prjPrefix = 1.0 / 4.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_T2().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_T2().GetMatrices();
}

Delone_T5::Delone_T5() {
   m_name = "T5";
   m_bravaisLatticeGeneral = "tP";
   m_bravaisType = "tP";
   m_strProjector = "0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 2";
   m_character = "000 rrs";
   m_freeParams = 2;
   m_prjPrefix = 1.0 / 2.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_T5().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_T5().GetMatrices();
}

Delone_R1::Delone_R1() {
   m_name = "R1";
   m_bravaisLatticeGeneral = "hR";
   m_bravaisType = "hR";
   m_strProjector = "1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0  0 0 0 1 1 1  0 0 0 1 1 1  0 0 0 1 1 1";
   m_character = "rrr sss";
   m_freeParams = 2;
   m_prjPrefix = 1.0 / 3.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_R1().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_R1().GetMatrices();
}

Delone_R3::Delone_R3() {
   m_name = "R3";
   m_bravaisLatticeGeneral = "hR";
   m_bravaisType = "hR";
   m_strProjector = "1 1 0 0 1 0  1 1 0 0 1 0  0 0 0 0 0 0  0 0 0 3 0 0  1 1 0 0 1 0  0 0 0 0 0 0";
   m_character = "rr0 sr0";
   m_freeParams = 2;
   m_prjPrefix = 1.0 / 3.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_R3().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_R3().GetMatrices();
}

Delone_O1A::Delone_O1A() {
   m_name = "O1A";
   m_bravaisLatticeGeneral = "oF";
   m_bravaisType = "oF";
   m_strProjector = "1 1 0 1 1 0  1 1 0 1 1 0  0 0 4 0 0 0  1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 4";
   m_character = "rrs rrt";
   m_freeParams = 3;
   m_prjPrefix = 1.0 / 4.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_O1A().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_O1A().GetMatrices();
}

Delone_O1B::Delone_O1B() {
   m_name = "O1B";
   m_bravaisLatticeGeneral = "oI";
   m_bravaisType = "oI";
   m_strProjector = "1 0 0 1 0 0  0 1 0 0 1 0  0 0 1 0 0 1  1 0 0 1 0 0  0 1 0 0 1 0  0 0 1 0 0 1";
   m_character = "rst rst";
   m_freeParams = 3;
   m_prjPrefix = 1.0 / 2.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_O1B().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_O1B().GetMatrices();
}

Delone_O2::Delone_O2() {
   m_name = "O2";
   m_bravaisLatticeGeneral = "oI";
   m_bravaisType = "oI";
   m_strProjector = "1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 0  0 1 0 0 1 0  1 0 0 1 0 0  0 0 0 0 0 2";
   m_character = "rs0 srt";
   m_freeParams = 3;
   m_prjPrefix = 1.0 / 2.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_O2().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_O2().GetMatrices();
}

Delone_O3::Delone_O3() {
   m_name = "O3";
   m_bravaisLatticeGeneral = "oI";
   m_bravaisType = "oI";
   m_strProjector = "1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 0  1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 0";
   m_character = "rs0 rs0";
   m_freeParams = 2;
   m_prjPrefix = 1.0 / 2.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_O3().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_O3().GetMatrices();
}

Delone_O4::Delone_O4() {
   m_name = "O4";
   m_bravaisLatticeGeneral = "oS";
   m_bravaisType = "oC";
   m_strProjector = "0 0 0 0 0 0  0 0 0 0 0 0  0 0 2 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 2";
   m_character = "00r sst";
   m_freeParams = 3;
   m_prjPrefix = 1.0 / 2.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_O4().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_O4().GetMatrices();
}

//Delone_O4B::Delone_O4B() {
//   m_name = "O4B";
//   m_bravaisLatticeGeneral = "oS";
//   m_bravaisType = "oC";
//   m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 0  0 0 0 1 0 0  0 1 0 0 0 0  0 0 0 0 0 0";
//   m_character = "rs0 tso";
//   m_freeParams = 3;
//   m_prjPrefix = 1.0 / 2.0;
//   m_projector = m_prjPrefix * MatS6(m_strProjector);
//
//   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_O4().GetMatrices();
//   m_sellaPerps = LabeledSellaMatrices::CreatePerps_O4().GetMatrices();
//}

Delone_O5::Delone_O5() {
   m_name = "O5";
   m_bravaisLatticeGeneral = "oP";
   m_bravaisType = "oP";
   m_strProjector = "0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0";
   m_character = "000 rst";
   m_freeParams = 3;
   m_prjPrefix = 1.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_O5().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_O5().GetMatrices();
}

Delone_M1A::Delone_M1A() {
   m_name = "M1A";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_strProjector = "1 1 0 0 0 0  1 1 0 0 0 0  0 0 2 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 2";
   m_character = "rrs ttu";
   m_freeParams = 4;
   m_prjPrefix = 1.0 / 2.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_M1A().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_M1A().GetMatrices();
}

Delone_M1B::Delone_M1B() {
   m_name = "M1B";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_strProjector = "1 0 0 1 0 0  0 1 0 0 1 0  0 0 2 0 0 0  1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 2";
   m_character = "rst rsu";
   m_freeParams = 4;
   m_prjPrefix = 1.0 / 2.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_M1B().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_M1B().GetMatrices();
}

Delone_M2A::Delone_M2A() {
   m_name = "M2A";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_strProjector = "2 0 0 0 0 0  0 1 0 1 0 0  0 0 0 0 0 0  0 1 0 1 0 0  0 0 0 0 2 0  0 0 0 0 0 2";
   m_character = "rs0 stu";
   m_freeParams = 4;
   m_prjPrefix = 1.0 / 2.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_M2A().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_M2A().GetMatrices();
}

Delone_M2B::Delone_M2B() {
   m_name = "M2B";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_strProjector = "1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 0  1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 2";
   m_character = "rs0 rst";
   m_freeParams = 3;
   m_prjPrefix = 1.0 / 2.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_M2B().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_M2B().GetMatrices();
}

Delone_M3::Delone_M3() {
   m_name = "M3";
   m_bravaisLatticeGeneral = "mS";
   m_bravaisType = "mC";
   m_strProjector = "2 0 0 0 0 0  0 1 0 0 1 0  0 0 0 0 0 0  0 0 0 2 0 0  0 1 0 0 1 0  0 0 0 0 0 0";
   m_character = "rs0 ts0";
   m_freeParams = 3;
   m_prjPrefix = 1.0 / 2.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_M3().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_M3().GetMatrices();
}

Delone_M4::Delone_M4() {
   m_name = "M4";
   m_bravaisLatticeGeneral = "mP";
   m_bravaisType = "mP";
   m_strProjector = "0 0 0 0 0 0  0 0 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1";
   m_character = "00r stu";
   m_freeParams = 4;
   m_prjPrefix = 1.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_M4().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_M4().GetMatrices();
}

Delone_H4::Delone_H4() {
   m_name = "H4";
   m_bravaisLatticeGeneral = "hP";
   m_bravaisType = "hP";
   m_strProjector = "0 0 0 0 0 0  0 0 0 0 0 0  0 0 1 1 1 0  0 0 1 1 1 0  0 0 1 1 1 0  0 0 0 0 0 3";
   m_character = "00r rrs";
   m_freeParams = 2;
   m_prjPrefix = 1.0 / 3.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_H4().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_H4().GetMatrices();
}

Delone_A1::Delone_A1() {
   m_name = "A1";
   m_bravaisLatticeGeneral = "aP";
   m_bravaisType = "aP";
   m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1";
   m_character = "rst uvw";
   m_freeParams = 6;
   m_prjPrefix = 1.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_A1().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_A1().GetMatrices();
}

Delone_A2::Delone_A2() {
   m_name = "A2";
   m_bravaisLatticeGeneral = "aP";
   m_bravaisType = "aP";
   m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1";
   m_character = "rs0 tuv";
   m_freeParams = 6;
   m_prjPrefix = 1.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_A2().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_A2().GetMatrices();
}

Delone_A3::Delone_A3() {
   m_name = "A3";
   m_bravaisLatticeGeneral = "aP";
   m_bravaisType = "aP";
   m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 0";
   m_character = "rs0 tu0";
   m_freeParams = 6;
   m_prjPrefix = 1.0;
   m_projector = m_prjPrefix * MatS6(m_strProjector);

   m_sellaProjectors = LabeledSellaMatrices::CreatePrjs_A3().GetMatrices();
   m_sellaPerps = LabeledSellaMatrices::CreatePerps_A3().GetMatrices();
}

std::ostream& operator<< (std::ostream& o, const GenerateDeloneBase* gdb) {
   o << *gdb << std::endl;
   return o;
}

std::ostream& operator<< (std::ostream& o, const GenerateDeloneBase& gdb) {
   //o << gdb.GetName() << " " << gdb.GetBravaisType() << " " 
   //   << "     (" << gdb.GetCharacter() << ")";
   //o << std::endl << gdb.GetPrefix() << " * [";
   //o << gdb.GetStringProjector() << "]";
   //return o;
   o << "$#begin{matrix} $" << gdb.GetName()
      << "$ ## $" << gdb.GetBravaisType()
      << "$ ## $(" << gdb.GetCharacter() << ")$"
      << std::endl;
   o << "#end{matrix} $" << std::endl;
   o << "%---------------------------" << std::endl;

   o << "$#begin{matrix} " << std::endl;
   o << "&$" << gdb.GetPrefix() << "~*~$ ##" << std::endl;
   o << "#end{matrix} $" << std::endl;
   o << "%---------------------------" << std::endl;
   o << "$#begin{bmatrix}" << std::endl;
   o << gdb.GetPrj() / gdb.GetPrefix() << std::endl;;
   o << "#end{bmatrix}$ ##" << std::endl;
   return o;

}

std::vector<std::shared_ptr<GenerateNiggliBase> >
GenerateLatticeTypeExamples::CreateListOfNiggliTypes()
{
   std::vector<std::shared_ptr<GenerateNiggliBase> > vglb;
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_1));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_2));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_3));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_4));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_5));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_6));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_7));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_8));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_9));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_10));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_11));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_12));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_13));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_14));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_15));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_16));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_17));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_18));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_19));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_20));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_21));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_22));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_23));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_24));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_25));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_26));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_27));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_28));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_29));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_30));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_31));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_32));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_33));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_34));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_35));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_36));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_37));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_38));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_39));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_40));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_41));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_42));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_43));
   vglb.push_back(std::shared_ptr<GenerateNiggliBase>(new IT_44));

   return vglb;
}

std::vector<std::shared_ptr<GenerateDeloneBase> >
GenerateLatticeTypeExamples::CreateListOfDeloneTypes()
{
   std::vector<std::shared_ptr<GenerateDeloneBase> > vglb;

   const bool includeUninterestingTypes = true;
   const bool includeTriclinic = true;

   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_C1));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_C3));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_C5));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_R1));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_R3));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_T1));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_T2));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_T5));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_O1A));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_O1B));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_O2));
   if (includeUninterestingTypes) vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_O3));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_O4));
   //vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_O4B));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_O5));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_M1A));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_M1B));
   if (includeUninterestingTypes) vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_M2B));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_M2A));
   if (includeUninterestingTypes) vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_M3));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_M4));
   if (includeTriclinic) vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_A1));
   if (includeTriclinic) vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_A2));
   if (includeTriclinic) vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_A3));
   vglb.push_back(std::shared_ptr<GenerateDeloneBase>(new Delone_H4));
   return vglb;
}

static std::string numbers("0123456789");
static std::string xtals("ctromha");
static std::string deloneTypes("CTROMHA");

std::vector<std::shared_ptr<GenerateNiggliBase> >
RetrieveByITNumber(const std::string& s,
   const std::vector<std::shared_ptr<GenerateNiggliBase> >& v) {
   std::vector<std::shared_ptr<GenerateNiggliBase> > out;
   for (size_t i = 0; i < v.size(); ++i) {
      if (s == v[i]->GetITNumber()) out.push_back(v[i]);
   }
   return out;
}

template<typename T>
std::vector<std::shared_ptr<T> >
RetrieveByName(const std::string& s,
   const std::vector<std::shared_ptr<T> >& v) {
   std::vector<std::shared_ptr<T> > out;
   for (size_t i = 0; i < v.size(); ++i) {
      const std::string name = v[i]->GetName();
      if (s == v[i]->GetName()) out.push_back(v[i]);
   }
   return out;
}

template<typename T>
std::vector<std::shared_ptr<T> >
RetrieveByXtalType(const std::string& s,
   const std::vector<std::shared_ptr<T> >& v) {
   std::vector<std::shared_ptr<T> > out;
   for (size_t i = 0; i < v.size(); ++i) {
      if (s.length() == 1 && s[0] == v[i]->GetXtalType(s)[0]) out.push_back(v[i]);
      else if (s == v[i]->GetXtalType(s)) out.push_back(v[i]);
      //std::cout << "in RetrieveByXtalType " << s << " " << v[i]->GetXtalType(s) << std::endl;
      if (!out.size() && out.size() < 4) {
         const int i19191 = 19191;
      }
   }
   return out;
}

template<typename T>
std::vector<std::shared_ptr<T> >
RetrieveByGeneralBravaisType(const std::string& s,
   const std::vector<std::shared_ptr<T> >& v) {
   std::vector<std::shared_ptr<T> > out;
   for (size_t i = 0; i < v.size(); ++i) {
      const std::string type = v[i]->GetBravaisLatticeGeneral();
      if ( type == s ) 
      {
         out.emplace_back(v[i]);;
      }
   }
   return out;
}

template<typename T>
std::vector<std::shared_ptr<T> >
RetrieveByDeloneName(const std::string& s,
   const std::vector<std::shared_ptr<T> >& v) {
   std::vector<std::shared_ptr<T> > out;
   for (size_t i = 0; i < v.size(); ++i) {
      const std::string type = v[i]->GetBravaisLatticeGeneral();
      if (s.length() == 2 && s == v[i]->GetName()) out.push_back(v[i]);
      if (s.length() == 1 && s[0] == v[i]->GetName()[0]) out.push_back(v[i]);
      if (!out.size() && out.size() < 4) {
         const int i19191 = 19191;
      }
   }
   return out;
}


std::vector<std::shared_ptr<GenerateNiggliBase> >
GenerateNiggliBase::Select(const std::string& s/* = ""*/) const {
   static const std::vector<std::shared_ptr<GenerateNiggliBase> > vglb =
      GenerateLatticeTypeExamples::CreateListOfNiggliTypes();

   if (s.empty()) {
      return vglb;
   }
   else if (s.size() == 3) {
      return RetrieveByName(s, vglb);
   }
   else if (numbers.find(s[0]) != std::string::npos) {
      return RetrieveByITNumber(s, vglb);
   }
   else if (s.size() == 2 && s[1] == 'S') {
      return RetrieveByGeneralBravaisType(s, vglb);
   }
   else if (xtals.find(LRL_StringTools::strLower(s)[0]) != std::string::npos) {
      return RetrieveByXtalType(s, vglb);
   }
   else {
      return vglb;
   }


   return vglb;

}

std::vector<std::shared_ptr<GenerateDeloneBase> >
GenerateDeloneBase::Select(const std::string& s/* = ""*/) const {

  static const std::vector<std::shared_ptr<GenerateDeloneBase> > vglb =
      GenerateLatticeTypeExamples::CreateListOfDeloneTypes();

   if (s.empty()) {
      return vglb;
   }
   else if (s.size() == 3) {
      return RetrieveByName(s, vglb);
   }
   else if (deloneTypes.find(s[0]) != std::string::npos) {
      return RetrieveByDeloneName(s, vglb);
   }
   else if (s.size() == 1 && xtals.find(s[0]) == std::string::npos) {
      return RetrieveByGeneralBravaisType(s, vglb);
   }
   else if (s.size() == 2 && s[1] == 'S') {
      return RetrieveByGeneralBravaisType(s, vglb);
   }
   else if (xtals.find(s[0]) != std::string::npos) {
      return RetrieveByXtalType(s, vglb);
   }
   else {
      return vglb;
   }


   return vglb;

}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// add some small random perturbation
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
G6 GenerateLatticesBase::GeneratePerturbation(const G6& v, const double amountToPerturb) {
   G6 modVector = G6::rand();
   modVector = v.norm() * modVector / modVector.norm();
   return v + amountToPerturb * modVector;
}


