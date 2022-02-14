
#include <cstdio>
#include <string>

#include "Definitions.h"
#include "Generated_IT_Types.h"
#include "LRL_Cell.h"
#include "MatG6.h"

 bool IT_Lat_Char_Base::Approx(const double d1, const double d2) {
   return abs(d1 - d2) < g_delta;
}

 bool IT_Lat_Char_Base::Approx(const double d1, const double d2, const double d3) {
   return Approx(d1, d2) && Approx(d2, d3);
}

 bool IT_Lat_Char_Base::Approx(const G6& g1, const G6& g2) {
   return (g1 - g2).norm() < g_delta;
}


IT_Lat_Char_Base::IT_Lat_Char_Base()
   : m_name("BASE")
   , m_latticeType("BASE")
   , m_ITnumber("BASE")
   , m_G6_character("BASE")
   , m_DC7_character("DC7_character NA")
   , m_prjPrefix(-19191.0E19)
   , m_strProjector("BASE")
   , m_projector("BASE")  // from Paciorek & Bonin, 1992
{
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// add some small random perturbation
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
G6 IT_Lat_Char_Base::GeneratePerturbation(const G6& v, const double amountToPerturb) {
   G6 modVector = G6::rand();
   modVector = v.norm() * modVector / modVector.norm();
   return v + amountToPerturb * modVector;
}

IT_Lat_Char_1::IT_Lat_Char_1() : IT_Lat_Char_Base() {
   m_name = "44C";
   m_latticeType = "cF";
   m_ITnumber = " 1";
   m_G6_character =  "[r, r, r, r, r, r]";
   m_DC7_character = "[r,r,r,r,r,r,2r]";
   m_prjPrefix = 1.0/6.0;
   m_strProjector = "1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.333333 0.333333 0.333333 0.333333 0.333333 0.333333
   0.333333 0.333333 0.333333 0.333333 0.333333 0.333333
   0.333333 0.333333 0.333333 0.333333 0.333333 0.333333
   0.333333 0.333333 0.333333 0.333333 0.333333 0.333333
   0.333333 0.333333 0.333333 0.333333 0.333333 0.333333
   0.333333 0.333333 0.333333 0.333333 0.333333 0.333333 ;
   */
} // end of class IT_Lat_Char_1 constructor

IT_Lat_Char_2::IT_Lat_Char_2() : IT_Lat_Char_Base() {
   m_name = "49C";
   m_latticeType = "hR";
   m_ITnumber = " 2";
   m_G6_character =  "[r, r, r, s, s, s]";
   m_DC7_character = "[r,r,r,2r-s,2-s,3r-s]";
   m_prjPrefix = 0.333333;
   m_strProjector = "1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0  0 0 0 1 1 1  0 0 0 1 1 1  0 0 0 1 1 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.333333 0.333333 0.333333 0 0 0
   0.333333 0.333333 0.333333 0 0 0
   0.333333 0.333333 0.333333 0 0 0
   0 0 0 0.333333 0.333333 0.333333
   0 0 0 0.333333 0.333333 0.333333
   0 0 0 0.333333 0.333333 0.333333 ;
   */
} // end of class IT_Lat_Char_2 constructor

IT_Lat_Char_3::IT_Lat_Char_3() : IT_Lat_Char_Base() {
   m_name = "44A";
   m_latticeType = "cP";
   m_ITnumber = " 3";
   m_G6_character =  "[r, r, r, 0, 0, 0]";
   m_DC7_character = "[r,r,r,2r,2r,2r,2r]";
   m_prjPrefix = 1.0/3.0;
   m_strProjector = "1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.333333 0.333333 0.333333 0 0 0
   0.333333 0.333333 0.333333 0 0 0
   0.333333 0.333333 0.333333 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_Lat_Char_3 constructor

IT_Lat_Char_4::IT_Lat_Char_4() : IT_Lat_Char_Base() {
   m_name = "49D";
   m_latticeType = "hR";
   m_ITnumber = " 4";
   m_G6_character =  "[r, r, r, -s, -s, -s]";
   m_prjPrefix = 0.333333;
   m_strProjector = "1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0  0 0 0 1 1 1  0 0 0 1 1 1  0 0 0 1 1 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.333333 0.333333 0.333333 0 0 0
   0.333333 0.333333 0.333333 0 0 0
   0.333333 0.333333 0.333333 0 0 0
   0 0 0 0.333333 0.333333 0.333333
   0 0 0 0.333333 0.333333 0.333333
   0 0 0 0.333333 0.333333 0.333333 ;
   */
} // end of class IT_Lat_Char_4 constructor

IT_Lat_Char_5::IT_Lat_Char_5() : IT_Lat_Char_Base() {
   m_name = "44B";
   m_latticeType = "cI";
   m_ITnumber = " 5";
   m_G6_character = "(3r, 3r, 3r, -2r, -2r, -2r]";
   m_DC7_character = "[r,r,r,r,4r/3,4r/3,4r/3]";
   m_prjPrefix = 0.025641;
   m_strProjector = "9 9 9 -6 -6 -6  9 9 9 -6 -6 -6  9 9 9 -6 -6 -6 -6 -6 -6 4 4 4 -6 -6 -6 4 4 4 -6 -6 -6 4 4 4";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   -0.153846 -0.153846 -0.153846 0.102564 0.102564 0.102564
   -0.153846 -0.153846 -0.153846 0.102564 0.102564 0.102564
   -0.153846 -0.153846 -0.153846 0.102564 0.102564 0.102564 ;
   */
} // end of class IT_Lat_Char_5 constructor

IT_Lat_Char_6::IT_Lat_Char_6() : IT_Lat_Char_Base() {
   m_name = "45D";
   m_latticeType = "tI";
   m_ITnumber = " 6";
   m_G6_character =  "[r, r, r, -r+s, -r+s, -2s]";
   m_prjPrefix = 0.0384615;
   m_strProjector = "6 6 6 -4 -4 -4  6 6 6 -4 -4 -4  6 6 6 -4 -4 -4   -4 -4 -4 7 7 -6  -4 -4 -4 7 7 -6  -4 -4 -4 -6 -6 20";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   -0.153846 -0.153846 -0.153846 0.269231 0.269231 -0.230769
   -0.153846 -0.153846 -0.153846 0.269231 0.269231 -0.230769
   -0.153846 -0.153846 -0.153846 -0.230769 -0.230769 0.769231 ;
   */
} // end of class IT_Lat_Char_6 constructor

IT_Lat_Char_7::IT_Lat_Char_7() : IT_Lat_Char_Base() {
   m_name = "45D";
   m_latticeType = "tI";
   m_ITnumber = " 7";
   m_G6_character =  "[r, r, r, 2s, -r -s, -r -s]";
   m_prjPrefix = 0.0384615;
   m_strProjector = "6 6 6 -4 -4 -4  6 6 6 -4 -4 -4  6 6 6 -4 -4 -4 -4 -4 -4 20 -6 -6 -4 -4 -4 -6 7 7 -4 -4 -4 -6 7 7";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   -0.153846 -0.153846 -0.153846 0.769231 -0.230769 -0.230769
   -0.153846 -0.153846 -0.153846 -0.230769 0.269231 0.269231
   -0.153846 -0.153846 -0.153846 -0.230769 0.269231 0.269231 ;
   */
} // end of class IT_Lat_Char_7 constructor

IT_Lat_Char_8::IT_Lat_Char_8() : IT_Lat_Char_Base() {
   m_name = "52A";
   m_latticeType = "oI";
   m_ITnumber = " 8";
   m_G6_character = "[r, r, r, -s, -t, -2r+s+t]";
   m_prjPrefix = 1.0/13.0;
   m_strProjector = "3 3 3 -2 -2 -2   3 3 3 -2 -2 -2   3 3 3 -2 -2 -2   -2 -2 -2 10 -3 -3   -2 -2 -2 -3 10 -3   -2 -2 -2 -3 -3 10";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   0.230769 0.230769 0.230769 -0.153846 -0.153846 -0.153846
   -0.153846 -0.153846 -0.153846 0.769231 -0.230769 -0.230769
   -0.153846 -0.153846 -0.153846 -0.230769 0.769231 -0.230769
   -0.153846 -0.153846 -0.153846 -0.230769 -0.230769 0.769231 ;
   */
} // end of class IT_Lat_Char_8 constructor

IT_Lat_Char_9::IT_Lat_Char_9() : IT_Lat_Char_Base() {
   m_name = "49B";
   m_latticeType = "hR";
   m_ITnumber = " 9";
   m_G6_character = "[r, r, s, r, r, r]";
   m_prjPrefix = 0.2;
   m_strProjector = "1 1 0 1 1 1  1 1 0 1 1 1  0 0 5 0 0 0  1 1 0 1 1 1  1 1 0 1 1 1  1 1 0 1 1 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.2 0.2 0 0.2 0.2 0.2
   0.2 0.2 0 0.2 0.2 0.2
   0 0 1 0 0 0
   0.2 0.2 0 0.2 0.2 0.2
   0.2 0.2 0 0.2 0.2 0.2
   0.2 0.2 0 0.2 0.2 0.2 ;
   */
} // end of class IT_Lat_Char_9 constructor

IT_Lat_Char_10::IT_Lat_Char_10() : IT_Lat_Char_Base() {
   m_name = "55A";
   m_latticeType = "mC";
   m_ITnumber = "10";
   m_G6_character = "[r, r, s, t, t, u]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 1 0 0 0 0  1 1 0 0 0 0  0 0 2 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 2";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.5 0.5 0 0 0 0
   0.5 0.5 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0.5 0.5 0
   0 0 0 0.5 0.5 0
   0 0 0 0 0 1 ;
   */
} // end of class IT_Lat_Char_10 constructor

IT_Lat_Char_11::IT_Lat_Char_11() : IT_Lat_Char_Base() {
   m_name = "45A";
   m_latticeType = "tP";
   m_ITnumber = "11";
   m_G6_character = "[r, r, s, 0,0,0]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 1 0 0 0 0  1 1 0 0 0 0  0 0 2 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.5 0.5 0 0 0 0
   0.5 0.5 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_Lat_Char_11 constructor

IT_Lat_Char_12::IT_Lat_Char_12() : IT_Lat_Char_Base() {
   m_name = "48A";
   m_latticeType = "hP";
   m_ITnumber = "12";
   m_G6_character = "[r, r, s, 0, 0, -r]";
   m_prjPrefix = 0.333333;
   m_strProjector = "1 1 0 0 0 -1  1 1 0 0 0 -1  0 0 3 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0 -1 -1 0 0 0 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.333333 0.333333 0 0 0 -0.333333
   0.333333 0.333333 0 0 0 -0.333333
   0 0 1 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   -0.333333 -0.333333 0 0 0 0.333333 ;
   */
} // end of class IT_Lat_Char_12 constructor

IT_Lat_Char_13::IT_Lat_Char_13() : IT_Lat_Char_Base() {
   m_name = "50D";
   m_latticeType = "oC";
   m_ITnumber = "13";
   m_G6_character = "[r, r, s, 0, 0, -t]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 1 0 0 0 0  1 1 0 0 0 0  0 0 2 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 2";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.5 0.5 0 0 0 0
   0.5 0.5 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 1 ;
   */
} // end of class IT_Lat_Char_13 constructor

IT_Lat_Char_14::IT_Lat_Char_14() : IT_Lat_Char_Base() {
   m_name = "55A";
   m_latticeType = "mC";
   m_ITnumber = "14";
   m_G6_character = "[r, r, s, t, t, u]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 1 0 0 0 0  1 1 0 0 0 0  0 0 2 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 2";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.5 0.5 0 0 0 0
   0.5 0.5 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0.5 0.5 0
   0 0 0 0.5 0.5 0
   0 0 0 0 0 1 ;
   */
} // end of class IT_Lat_Char_14 constructor

IT_Lat_Char_15::IT_Lat_Char_15() : IT_Lat_Char_Base() {
   m_name = "45C";
   m_latticeType = "tI";
   m_ITnumber = "15";
   m_G6_character = "[r, r, s, -r, -r, 0]";
   m_prjPrefix = 0.25;
   m_strProjector = "1 1 0 -1 -1 0  1 1 0 -1 -1 0  0 0 4 0 0 0  -1 -1 0 1 1 0  -1 -1 0 1 1 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.25 0.25 0 -0.25 -0.25 0
   0.25 0.25 0 -0.25 -0.25 0
   0 0 1 0 0 0
   -0.25 -0.25 0 0.25 0.25 0
   -0.25 -0.25 0 0.25 0.25 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_Lat_Char_15 constructor

IT_Lat_Char_16::IT_Lat_Char_16() : IT_Lat_Char_Base() {
   m_name = "51A";
   m_latticeType = "oF";
   m_ITnumber = "16";
   m_G6_character = "r, r, s, -t, -t, -2r -2t]";
   m_prjPrefix = 0.1;
   m_strProjector = "3 3 0 -2 -2 -2  3 3 0 -2 -2 -2  0 0 10 0 0 0  -2 -2 0 3 3 -2  -2 -2 0 3 3 -2  -2 -2 0 -2 -2 8";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.3 0.3 0 -0.2 -0.2 -0.2
   0.3 0.3 0 -0.2 -0.2 -0.2
   0 0 1 0 0 0
   -0.2 -0.2 0 0.3 0.3 -0.2
   -0.2 -0.2 0 0.3 0.3 -0.2
   -0.2 -0.2 0 -0.2 -0.2 0.8 ;
   */
} // end of class IT_Lat_Char_16 constructor

IT_Lat_Char_17::IT_Lat_Char_17() : IT_Lat_Char_Base() {
   m_name = "57B";
   m_latticeType = "mC";
   m_ITnumber = "17";
   m_G6_character = "[r, r, s, t, u, -2r -t -u]";
   m_prjPrefix = 0.1;
   m_strProjector = "3 3 0 -2 -2 -2  3 3 0 -2 -2 -2  0 0 10 0 0 0 -2 -2 0 8 -2 -2 -2 -2 0 -2 8 -2 -2 -2 0 -2 -2 8";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.3 0.3 0 -0.2 -0.2 -0.2
   0.3 0.3 0 -0.2 -0.2 -0.2
   0 0 1 0 0 0
   -0.2 -0.2 0 0.8 -0.2 -0.2
   -0.2 -0.2 0 -0.2 0.8 -0.2
   -0.2 -0.2 0 -0.2 -0.2 0.8 ;
   */
} // end of class IT_Lat_Char_17 constructor

IT_Lat_Char_18::IT_Lat_Char_18() : IT_Lat_Char_Base() {
   m_name = "45E";
   m_latticeType = "tI";
   m_ITnumber = "18";
   m_G6_character = "(2r, s, s, r, 2r, 2r]";
   m_prjPrefix = 0.0384615;
   m_strProjector = "8 0 0 4 8 8  0 13 13 0 0 0  0 13 13 0 0 0  4 0 0 2 4 4  8 0 0 4 8 8  8 0 0 4 8 8";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.307692 0 0 0.153846 0.307692 0.307692
   0 0.5 0.5 0 0 0
   0 0.5 0.5 0 0 0
   0.153846 0 0 0.0769231 0.153846 0.153846
   0.307692 0 0 0.153846 0.307692 0.307692
   0.307692 0 0 0.153846 0.307692 0.307692 ;
   */
} // end of class IT_Lat_Char_18 constructor

IT_Lat_Char_19::IT_Lat_Char_19() : IT_Lat_Char_Base() {
   m_name = "52B";
   m_latticeType = "oI";
   m_ITnumber = "19";
   m_G6_character = "[r, s, s, t, r, r]";
   m_prjPrefix = 0.166667;
   m_strProjector = "2 0 0 0 2 2  0 3 3 0 0 0  0 3 3 0 0 0  0 0 0 6 0 0  2 0 0 0 2 2  2 0 0 0 2 2";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.333333 0 0 0 0.333333 0.333333
   0 0.5 0.5 0 0 0
   0 0.5 0.5 0 0 0
   0 0 0 1 0 0
   0.333333 0 0 0 0.333333 0.333333
   0.333333 0 0 0 0.333333 0.333333 ;
   */
} // end of class IT_Lat_Char_19 constructor

IT_Lat_Char_20::IT_Lat_Char_20() : IT_Lat_Char_Base() {
   m_name = "55B";
   m_latticeType = "mC";
   m_ITnumber = "20";
   m_G6_character = "[r, s, s, t, u, u]";
   m_prjPrefix = 0.5;
   m_strProjector = "2 0 0 0 0 0  0 1 1 0 0 0  0 1 1 0 0 0  0 0 0 2 0 0  0 0 0 0 1 1  0 0 0 0 1 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 1 0 0 0 0 0
   0 0.5 0.5 0 0 0
   0 0.5 0.5 0 0 0
   0 0 0 1 0 0
   0 0 0 0 0.5 0.5
   0 0 0 0 0.5 0.5 ;
   */
} // end of class IT_Lat_Char_20 constructor

IT_Lat_Char_21::IT_Lat_Char_21() : IT_Lat_Char_Base() {
   m_name = "45B";
   m_latticeType = "tP";
   m_ITnumber = "21";
   m_G6_character = "[r, s, s, 0, 0, 0]";
   m_DC7_character = "[r,s,s,s+r,s+r,s+r,d+r]";
   m_prjPrefix = 0.5;
   m_strProjector = "2 0 0 0 0 0  0 1 1 0 0 0  0 1 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 1 0 0 0 0 0
   0 0.5 0.5 0 0 0
   0 0.5 0.5 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_Lat_Char_21 constructor

IT_Lat_Char_22::IT_Lat_Char_22() : IT_Lat_Char_Base() {
   m_name = "48B";
   m_latticeType = "hP";
   m_ITnumber = "22";
   m_G6_character = "[r, s, s, -s, 0, 0]";
   m_DC7_character = "[r, s, s, s, s + r, s + r, s + r]";
   m_prjPrefix = 1.0/3.0;
   m_strProjector = "3 0 0 0 0 0  0 1 1 -1 0 0  0 1 1 -1 0 0  0 -1 -1 1 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 1.5 0 0 0 0 0
   0 0.5 0.5 -0.5 0 0
   0 0.5 0.5 -0.5 0 0
   0 -0.5 -0.5 0.5 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_Lat_Char_22 constructor

IT_Lat_Char_23::IT_Lat_Char_23() : IT_Lat_Char_Base() {
   m_name = "50E";
   m_latticeType = "oC";
   m_ITnumber = "23";
   m_G6_character = "[r, s, s, -t, 0, 0]";
   m_prjPrefix = 0.5;
   m_strProjector = "2 0 0 0 0 0  0 1 1 0 0 0  0 1 1 0 0 0  0 0 0 2 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 1 0 0 0 0 0
   0 0.5 0.5 0 0 0
   0 0.5 0.5 0 0 0
   0 0 0 1 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_Lat_Char_23 constructor

IT_Lat_Char_24::IT_Lat_Char_24() : IT_Lat_Char_Base() {
   m_name = "49E";
   m_latticeType = "hR";
   m_ITnumber = "24";
   m_G6_character = "(3r, s, s, -s, -s+r, -2r, -2r]";
   m_DC7_character = "[r,s,s,s,s+r/3,s+r/3,s+r/3]";
   m_prjPrefix = 1.0/53.0;
   m_strProjector = "27 3 3 6 -18 -18  3 18 18 -17 -2 -2  3 18 18 -17 -2 -2  6 -17 -17 19 -4 -4  -18 -2 -2 -4 12 12  -18 -2 -2 -4 12 12";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.509434 0.0566038 0.0566038 0.113208 -0.339623 -0.339623
   0.0566038 0.339623 0.339623 -0.320755 -0.0377358 -0.0377358
   0.0566038 0.339623 0.339623 -0.320755 -0.0377358 -0.0377358
   0.113208 -0.320755 -0.320755 0.358491 -0.0754717 -0.0754717
   0.339623 -0.0377358 -0.0377358 -0.0754717 0.226415 0.226415
   -0.339623 -0.0377358 -0.0377358 -0.0754717 0.226415 0.226415 ;
   */
} // end of class IT_Lat_Char_24 constructor

IT_Lat_Char_25::IT_Lat_Char_25() : IT_Lat_Char_Base() {
   m_name = "55B";
   m_latticeType = "mC";
   m_ITnumber = "25";
   m_G6_character = "[r, s, s, t, u, u]";
   m_prjPrefix = 0.5;
   m_strProjector = "2 0 0 0 0 0  0 1 1 0 0 0  0 1 1 0 0 0  0 0 0 2 0 0  0 0 0 0 1 1  0 0 0 0 1 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 1 0 0 0 0 0
   0 0.5 0.5 0 0 0
   0 0.5 0.5 0 0 0
   0 0 0 1 0 0
   0 0 0 0 0.5 0.5
   0 0 0 0 0.5 0.5 ;
   */
} // end of class IT_Lat_Char_25 constructor

IT_Lat_Char_26::IT_Lat_Char_26() : IT_Lat_Char_Base() {
   m_name = "51B";
   m_latticeType = "oF";
   m_ITnumber = "26";
   m_G6_character = "(2r, s, t, r, 2r, 2r]";
   m_prjPrefix = 0.0769231;
   m_strProjector = "4 0 0 2 4 4  0 13 0 0 0 0  0 0 13 0 0 0  2 0 0 1 2 2  4 0 0 2 4 4  4 0 0 2 4 4";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.307692 0 0 0.153846 0.307692 0.307692
   0 1 0 0 0 0
   0 0 1 0 0 0
   0.153846 0 0 0.0769231 0.153846 0.153846
   0.307692 0 0 0.153846 0.307692 0.307692
   0.307692 0 0 0.153846 0.307692 0.307692 ;
   */
} // end of class IT_Lat_Char_26 constructor

IT_Lat_Char_27::IT_Lat_Char_27() : IT_Lat_Char_Base() {
   m_name = "57C";
   m_latticeType = "mC";
   m_ITnumber = "27";
   m_G6_character = "[r, s, t, u, r, r]";
   m_prjPrefix = 0.333333;
   m_strProjector = "1 0 0 0 1 1  0 3 0 0 0 0  0 0 3 0 0 0  0 0 0 3 0 0  1 0 0 0 1 1  1 0 0 0 1 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.333333 0 0 0 0.333333 0.333333
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 1 0 0
   0.333333 0 0 0 0.333333 0.333333
   0.333333 0 0 0 0.333333 0.333333 ;
   */
} // end of class IT_Lat_Char_27 constructor

IT_Lat_Char_28::IT_Lat_Char_28() : IT_Lat_Char_Base() {
   m_name = "56A";
   m_latticeType = "mC";
   m_ITnumber = "28";
   m_G6_character = "[r, s, t, u, r, 2u]";
   m_prjPrefix = 0.1;
   m_strProjector = "5 0 0 0 5 0  0 10 0 0 0 0  0 0 10 0 0 0  0 0 0 2 0 4  5 0 0 0 5 0  0 0 0 4 0 8";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.5 0 0 0 0.5 0
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0.2 0 0.4
   0.5 0 0 0 0.5 0
   0 0 0 0.4 0 0.8 ;
   */
} // end of class IT_Lat_Char_28 constructor

IT_Lat_Char_29::IT_Lat_Char_29() : IT_Lat_Char_Base() {
   m_name = "56C";
   m_latticeType = "mC";
   m_ITnumber = "29";
   m_G6_character = "[r, s, t, u, 2u, r]";
   m_prjPrefix = 0.1;
   m_strProjector = "5 0 0 0 0 5  0 10 0 0 0 0  0 0 10 0 0 0  0 0 0 2 4 0  0 0 0 4 8 0  5 0 0 0 0 5";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.5 0 0 0 0 0.5
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0.2 0.4 0
   0 0 0 0.4 0.8 0
   0.5 0 0 0 0 0.5 ;
   */
} // end of class IT_Lat_Char_29 constructor

IT_Lat_Char_30::IT_Lat_Char_30() : IT_Lat_Char_Base() {
   m_name = "56B";
   m_latticeType = "mC";
   m_ITnumber = "30";
   m_G6_character = "[r, s, t, s, u, 2u]";
   m_prjPrefix = 0.1;
   m_strProjector = "10 0 0 0 0 0  0 5 0 5 0 0  0 0 10 0 0 0  0 5 0 5 0 0  0 0 0 0 2 4  0 0 0 0 4 8";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 1 0 0 0 0 0
   0 0.5 0 0.5 0 0
   0 0 1 0 0 0
   0 0.5 0 0.5 0 0
   0 0 0 0 0.2 0.4
   0 0 0 0 0.4 0.8 ;
   */
} // end of class IT_Lat_Char_30 constructor

IT_Lat_Char_32::IT_Lat_Char_32() : IT_Lat_Char_Base() {
   m_name = "50C";
   m_latticeType = "oP";
   m_ITnumber = "32";
   m_G6_character = "[r, s, t, 0, 0, 0]";
   m_prjPrefix = 1;
   m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 1 0 0 0 0 0
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_Lat_Char_32 constructor

IT_Lat_Char_33::IT_Lat_Char_33() : IT_Lat_Char_Base() {
   m_name = "53A";
   m_latticeType = "mP";
   m_ITnumber = "33";
   m_G6_character = "[r, s, t, 0, u, 0]";
   m_prjPrefix = 1;
   m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 1 0 0 0 0 0
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0 0 0
   0 0 0 0 1 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_Lat_Char_33 constructor


// NOTE: Pacioric and Bonin seem to have reversed 34/35
// Also Andrews&Bernstein, 1988 
// what does the IT say?


IT_Lat_Char_35::IT_Lat_Char_35() : IT_Lat_Char_Base() {
   m_name = "53C";
   m_latticeType = "mP";
   m_ITnumber = "35";
   m_G6_character = "[r, s, t, 0, 0, -u]";
   m_prjPrefix = 1;
   m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 1 0 0 0 0 0
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 1 ;
   */
} // end of class IT_Lat_Char_35 constructor

IT_Lat_Char_34::IT_Lat_Char_34() : IT_Lat_Char_Base() {
   m_name = "53B";
   m_latticeType = "mP";
   m_ITnumber = "34";
   m_G6_character = "[r, s, t, -u, 0, 0]";
   m_prjPrefix = 1;
   m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 1 0 0 0 0 0
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 1 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_Lat_Char_34 constructor

IT_Lat_Char_36::IT_Lat_Char_36() : IT_Lat_Char_Base() {
   m_name = "50A";
   m_latticeType = "oC";
   m_ITnumber = "36";
   m_G6_character =  "[r,s, t, 0, -r, 0]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 0 0 0 -1 0  0 2 0 0 0 0  0 0 2 0 0 0  0 0 0 0 0 0  -1 0 0 0 1 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.5 0 0 0 -0.5 0
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0 0 0
   -0.5 0 0 0 0.5 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_Lat_Char_36 constructor

IT_Lat_Char_37::IT_Lat_Char_37() : IT_Lat_Char_Base() {
   m_name = "54C";
   m_latticeType = "mC";
   m_ITnumber = "37";
   m_G6_character =  "[r, s, t, u, -r, 0]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 0 0 0 -1 0  0 2 0 0 0 0  0 0 2 0 0 0  0 0 0 2 0 0 -1 0 0 0 1 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.5 0 0 0 -0.5 0
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 1 0 0
   -0.5 0 0 0 0.5 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_Lat_Char_37 constructor

IT_Lat_Char_38::IT_Lat_Char_38() : IT_Lat_Char_Base() {
   m_name = "50B";
   m_latticeType = "oC";
   m_ITnumber = "38";
   m_G6_character =  "[r, s, t, 0, 0, -r]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 0 0 0 0 -1  0 2 0 0 0 0  0 0 2 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0 -1 0 0 0 0 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.5 0 0 0 0 -0.5
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   -0.5 0 0 0 0 0.5 ;
   */
} // end of class IT_Lat_Char_38 constructor

IT_Lat_Char_39::IT_Lat_Char_39() : IT_Lat_Char_Base() {
   m_name = "54A";
   m_latticeType = "mC";
   m_ITnumber = "39";
   m_G6_character =  "[r, s, t, u, 0, -r]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 0 0 0 0 -1  0 2 0 0 0 0  0 0 2 0 0 0  0 0 0 2 0 0  0 0 0 0 0 0 -1 0 0 0 0 1";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.5 0 0 0 0 -0.5
   0 1 0 0 0 0
   0 0 1 0 0 0
   0 0 0 1 0 0
   0 0 0 0 0 0
   -0.5 0 0 0 0 0.5 ;
   */
} // end of class IT_Lat_Char_39 constructor

IT_Lat_Char_40::IT_Lat_Char_40() : IT_Lat_Char_Base() {
   m_name = "50F";
   m_latticeType = "oC";
   m_ITnumber = "40";
   m_G6_character =  "[r, s, t, -s, 0, 0]";
   m_prjPrefix = 0.5;
   m_strProjector = "2 0 0 0 0 0  0 1 0 -1 0 0  0 0 2 0 0 0  0 -1 0 1 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 1 0 0 0 0 0
   0 0.5 0 -0.5 0 0
   0 0 1 0 0 0
   0 -0.5 0 0.5 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_Lat_Char_40 constructor

IT_Lat_Char_41::IT_Lat_Char_41() : IT_Lat_Char_Base() {
   m_name = "54B";
   m_latticeType = "mC";
   m_ITnumber = "41";
   m_G6_character =  "[r, s, t, -s, u, 0]";
   m_prjPrefix = 0.5;
   m_strProjector = "2 0 0 0 0 0  0 1 0 -1 0 0  0 0 2 0 0 0  0 -1 0 1 0 0  0 0 0 0 2 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 1 0 0 0 0 0
   0 0.5 0 -0.5 0 0
   0 0 1 0 0 0
   0 -0.5 0 0.5 0 0
   0 0 0 0 1 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_Lat_Char_41 constructor

IT_Lat_Char_42::IT_Lat_Char_42() : IT_Lat_Char_Base() {
   m_name = "52C";
   m_latticeType = "oI";
   m_ITnumber = "42";
   m_G6_character =  "[r, s, t, -s, -r, 0]";
   m_prjPrefix = 0.5;
   m_strProjector = "1 0 0 0 -1 0  0 1 0 -1 0 0  0 0 2 0 0 0  0 -1 0 1 0 0 -1 0 0 0 1 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.5 0 0 0 -0.5 0
   0 0.5 0 -0.5 0 0
   0 0 1 0 0 0
   0 -0.5 0 0.5 0 0
   -0.5 0 0 0 0.5 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_Lat_Char_42 constructor

IT_Lat_Char_43::IT_Lat_Char_43() : IT_Lat_Char_Base() {
   m_name = "57A";
   m_latticeType = "mC";
   m_ITnumber = "43";
   m_G6_character =  "[r, s, t, -s -u, -r -u, 2u]";
   m_prjPrefix = 0.05;
   m_strProjector = "11 1 0 1 -9 -4  1 11 0 -9 1 -4  0 0 20 0 0 0  1 -9 0 11 1 -4 -9 1 0 1 11 -4 -4 -4 0 -4 -4 16";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.55 0.05 0 0.05 -0.45 -0.2
   0.05 0.55 0 -0.45 0.05 -0.2
   0 0 1 0 0 0
   0.05 -0.45 0 0.55 0.05 -0.2
   -0.45 0.05 0 0.05 0.55 -0.2
   -0.2 -0.2 0 -0.2 -0.2 0.8 ;
   */
} // end of class IT_Lat_Char_43 constructor
