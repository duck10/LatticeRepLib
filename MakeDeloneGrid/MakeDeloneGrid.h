#ifndef MAKEDELONEGRID_H
#define MAKEDELONEGRID_H


class BasicDeloneRectangle {
public:
   int xcoord;
   int ycoord;
   std::string m_deloneName;
   std::string m_bravaisType;
   std::string m_generalType;
   std::string m_character;

   BasicDeloneRectangle() {}

   std::string DeloneOut() {
      std::stringstream  o;
      o << xcoord << "\n";
      o << ycoord << "\n";
      o << m_deloneName << "\n";
      o << m_bravaisType << "\n";
      o << m_generalType << "\n";
      o << m_character;
      return o.str();
   }  // end class BasicDeloneRectangle

   std::string MakeSVG() {
      std::stringstream  o;
      o << "BasicDeloneRectangle:: MakeSVG make a rectangle\n";
      return o.str();
   }
};

class Digit_1 : public BasicDeloneRectangle {
public:
   Digit_1() {}
   std::string MakeSVG() {
      std::stringstream  o;
      o << "Digit_1::MakeSVG output \n";
      return o.str();
   }
};

class Digit_2 : public BasicDeloneRectangle {
public:
   Digit_2() {}
   std::string MakeSVG() {
      std::stringstream  o;
      o << "Digit_2::MakeSVG output \n";
      return o.str();
   }
};

class Digit_3 : public BasicDeloneRectangle {
public:
   Digit_3() {}
   std::string MakeSVG() {
      std::stringstream  o;
      o << "Digit_3::MakeSVG output \n";
      return o.str();
   }
};

class Digit_4 : public BasicDeloneRectangle {
public:
   Digit_4() {}
   std::string MakeSVG() {
      std::stringstream  o;
      o << "Digit_4::MakeSVG output \n";
      return o.str();
   }
};

class Digit_5 : public BasicDeloneRectangle {
public:
   Digit_5() {}
   std::string MakeSVG() {
      std::stringstream  o;
      o << "Digit_5::MakeSVG output \n";
      return o.str();
   }
};

class Letter_C : public BasicDeloneRectangle {
public:
   Letter_C() {}
   Letter_C(const int x, const int y) {
      xcoord = x;
      ycoord = y;
   }
};

class Letter_T : public BasicDeloneRectangle {
public:
   Letter_T() {}
   Letter_T(const int x, const int y) {
      xcoord = x;
      ycoord = y;
   }
};

class Letter_R : public BasicDeloneRectangle {
public:
   Letter_R() {}
   Letter_R(const int x, const int y) {
      xcoord = x;
      ycoord = y;
   }
};

class Letter_O : public BasicDeloneRectangle {
public:
   Letter_O() {}
   Letter_O(const int x, const int y) {
      xcoord = x;
      ycoord = y;
   }
};

class Letter_M : public BasicDeloneRectangle {
public:
   Letter_M() {}
   Letter_M(const int x, const int y) {
      xcoord = x;
      ycoord = y;
   }
};

class Letter_A : public BasicDeloneRectangle {
public:
   Letter_A() {}
   Letter_A(const int x, const int y) {
      xcoord = x;
      ycoord = y;
   }
};

class Letter_H : public BasicDeloneRectangle {
public:
   Letter_H() {}
   Letter_H(const int x, const int y) {
      xcoord = x;
      ycoord = y;
   }
};

class GrayRectangle : public BasicDeloneRectangle {
public:
   GrayRectangle() {}
   GrayRectangle(const int x, const int y) {
      xcoord = x;
      ycoord = y;
   }
};  // end class GrayRectangle;

class BlankRectangle : public BasicDeloneRectangle {
public:
   BlankRectangle() {}
   BlankRectangle(const int x, const int y) {
      xcoord = x;
      ycoord = y;
   }
};  // end class BlankRectangle;

class C1 : public BasicDeloneRectangle {
public:
   C1() {}
   C1(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "C1";
      m_bravaisType = "cI";
      m_generalType = "cI";
      m_character = "(rrr rrr)";
   }
};  // end class C1;

class C3 : public BasicDeloneRectangle {
public:
   C3() {}
   C3(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "C3";
      m_bravaisType = "cF";
      m_generalType = "cF";
      m_character = "(rr0 rr0)";
   }
};  // end class C3;

class C5 : public BasicDeloneRectangle {
public:
   C5() {}
   C5(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "C5";
      m_bravaisType = "cP";
      m_generalType = "cP";
      m_character = "(000 rrr)";
   }
};  // end class C5;

class T1 : public BasicDeloneRectangle {
public:
   T1() {}
   T1(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "T1";
      m_bravaisType = "tI";
      m_generalType = "tI";
      m_character = "(rrs rrs)";
   }
};  // end class T1;

class T2 : public BasicDeloneRectangle {
public:
   T2() {}
   T2(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "T2";
      m_bravaisType = "tI";
      m_generalType = "tI";
      m_character = "(rr0 rrs)";
   }
};  // end class T2;

class T5 : public BasicDeloneRectangle {
public:
   T5() {}
   T5(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "T5";
      m_bravaisType = "tP";
      m_generalType = "tP";
      m_character = "(000 rrs)";
   }
};  // end class T5;

class R1 : public BasicDeloneRectangle {
public:
   R1() {}
   R1(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "R1";
      m_bravaisType = "hR";
      m_generalType = "hR";
      m_character = "(rrr sss)";
   }
};  // end class R1;

class R3 : public BasicDeloneRectangle {
public:
   R3() {}
   R3(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "R3";
      m_bravaisType = "hR";
      m_generalType = "hR";
      m_character = "(rr0 sr0) ";
   }
};  // end class R3;

class O1A : public BasicDeloneRectangle {
public:
   O1A() {}
   O1A(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "O1A";
      m_bravaisType = "oF";
      m_generalType = "oF";
      m_character = "(rrs rrt)";
   }
};  // end class O1A;

class O2 : public BasicDeloneRectangle {
public:
   O2() {}
   O2(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "O2";
      m_bravaisType = "oI";
      m_generalType = "oI";
      m_character = "(rs0 srt)";
   }
};  // end class O2;

class O3 : public BasicDeloneRectangle {
public:
   O3() {}
   O3(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "O3";
      m_bravaisType = "oI";
      m_generalType = "oI";
      m_character = "(rs0 rs0)";
   }
};  // end class O3;

class O4 : public BasicDeloneRectangle {
public:
   O4() {}
   O4(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "O4";
      m_bravaisType = "oS";
      m_generalType = "oS";
      m_character = "(00r sst)";
   }
};  // end class O4;

class O5 : public BasicDeloneRectangle {
public:
   O5() {}
   O5(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "O5";
      m_bravaisType = "oP";
      m_generalType = "oP";
      m_character = "(000 rst)";
   }
};  // end class O5;

class O1B : public BasicDeloneRectangle {
public:
   O1B() {}
   O1B(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "O1B";
      m_bravaisType = "oI";
      m_generalType = "oI";
      m_character = "(rst rst)";
   }
};  // end class O1B;

class M1A : public BasicDeloneRectangle {
public:
   M1A() {}
   M1A(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "M1A";
      m_bravaisType = "mC";
      m_generalType = "mS";
      m_character = "(rrs ttu)";
   }
};  // end class M1A;

class M2A : public BasicDeloneRectangle {
public:
   M2A() {}
   M2A(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "M2A";
      m_bravaisType = "mC";
      m_generalType = "mS";
      m_character = "(rs0 stu)";
   }
};  // end class M2A;

class M3 : public BasicDeloneRectangle {
public:
   M3() {}
   M3(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "M3";
      m_bravaisType = "mC";
      m_generalType = "mS";
      m_character = "(rs0 ts0)";
   }
};  // end class M3;

class M4 : public BasicDeloneRectangle {
public:
   M4() {}
   M4(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "M4";
      m_bravaisType = "mP";
      m_generalType = "mP";
      m_character = "(00r stu)";
   }
};  // end class M4;

class M1B : public BasicDeloneRectangle {
public:
   M1B() {}
   M1B(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "M1B";
      m_bravaisType = "mC";
      m_generalType = "mS";
      m_character = "(rst rsu)";
   }
};  // end class M1B;

class M2B : public BasicDeloneRectangle {
public:
   M2B() {}
   M2B(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "M2B";
      m_bravaisType = "mC";
      m_generalType = "mS";
      m_character = "(rs0 rst)";
   }
};  // end class M2B;

class A1 : public BasicDeloneRectangle {
public:
   A1() {}
   A1(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "A1";
      m_bravaisType = "aP";
      m_generalType = "aP";
      m_character = "(rst uvw)";
   }
};  // end class A1;

class A2 : public BasicDeloneRectangle {
public:
   A2() {}
   A2(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "A2";
      m_bravaisType = "aP";
      m_generalType = "aP";
      m_character = "(rs0 tuv)";
   }
};  // end class A2;

class A3 : public BasicDeloneRectangle {
public:
   A3() {}
   A3(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "A3";
      m_bravaisType = "aP";
      m_generalType = "aP";
      m_character = "(rs0 tu0)";
   }
};  // end class A3;

class H4 : public BasicDeloneRectangle {
public:
   H4() {}
   H4(const int x, const int y) {
      xcoord = x;
      ycoord = y;
      m_deloneName = "H4";
      m_bravaisType = "hP";
      m_generalType = "hP";
      m_character = "(00r rrs)";
   }
};  // end class H4;

class AllDeloneTypes {
public:
   AllDeloneTypes() {
   }

   void SetVertical() {

   }

   void SetHorizontal() {

   }

private:
   C1 c1;
   C3 c3;
   C5 c5;
   T1 t1;
   T2 t2;
   T5 t5;
   R1 r1;
   R3 r3;
   O1A o1a;
   O2 o2;
   O3 o3;
   O4 o4;
   O5 o5;
   O1B o1b;
   M1A m1a;
   M2A m2a;
   M3 m3;
   M4 m4;
   M1B m1b;
   M2B m2b;
   A1 a1;
   A2 a2;
   A3 a3;
   H4 h4;
};


#endif  // MAKEDELONEGRID_H
