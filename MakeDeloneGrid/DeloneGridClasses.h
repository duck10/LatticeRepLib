#ifndef DELONEGRIDCLASSES_H
#define DELONEGRIDCLASSES_H

#include <iostream>
#include <string>
#include <vector>

class BasicDeloneRectangle {
public:
   std::string m_deloneName;
   std::string m_bravaisType;
   std::string m_generalType;
   std::string m_character;
   std::string m_textName;

public:
   BasicDeloneRectangle();
   std::string MakeSVG();
   
   };

class Digit_1 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const Digit_1&);
   Digit_1() { m_textName = "1"; }
};

class Digit_2 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const Digit_2&);
   Digit_2() { m_textName = "2"; }
};

class Digit_3 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const Digit_3&);
   Digit_3() { m_textName = "3"; }
};

class Digit_4 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const Digit_4&);
   Digit_4() { m_textName = "4"; }
};

class Digit_5 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const Digit_5&);
   Digit_5() { m_textName = "5"; }
};

class Letter_C : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const Letter_C&);
   Letter_C() { m_textName = "C"; }
};

class Letter_T : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const Letter_T&);
   Letter_T() { m_textName = "T"; }
};

class Letter_R : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const Letter_R&);
   Letter_R() { m_textName = "R"; }
};

class Letter_O : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const Letter_O&);
   Letter_O() { m_textName = "O"; }
};

class Letter_M : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const Letter_M&);
   Letter_M() { m_textName = "M"; }
};

class Letter_A : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const Letter_A&);
   Letter_A() { m_textName = "A"; }
};

class Letter_H : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const Letter_H&);
   Letter_H() { m_textName = "H"; }
};

class GrayRectangle : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const GrayRectangle&);
   GrayRectangle() {}
   std::string MakeSVG();
};  // end class GrayRectangle;

class BlankRectangle : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const BlankRectangle&);
   BlankRectangle() {
      m_deloneName  = "";
      m_bravaisType = "";
      m_generalType = "";
      m_character   = "";
   }
};  // end class BlankRectangle;

class C1 : public BlankRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const C1&);
   C1() {
      m_deloneName = "C1";
      m_bravaisType = "cI";
      m_generalType = "cI";
      m_character = "(rrr rrr)";
   }
};  // end class C1;

class C3 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const C3&);
   C3()  {
      m_deloneName = "C3";
      m_bravaisType = "cF";
      m_generalType = "cF";
      m_character = "(rr0 rr0)";
   }
};  // end class C3;

class C5 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const C5&);
   C5() {
      m_deloneName = "C5";
      m_bravaisType = "cP";
      m_generalType = "cP";
      m_character = "(000 rrr)";
   }
};  // end class C5;

class T1 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const T1&);
   T1() {
      m_deloneName = "T1";
      m_bravaisType = "tI";
      m_generalType = "tI";
      m_character = "(rrs rrs)";
   }
};  // end class T1;

class T2 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const T2&);
   T2() {
      m_deloneName = "T2";
      m_bravaisType = "tI";
      m_generalType = "tI";
      m_character = "(rr0 rrs)";
   }
};  // end class T2;

class T5 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const T5&);
   T5() {
      m_deloneName = "T5";
      m_bravaisType = "tP";
      m_generalType = "tP";
      m_character = "(000 rrs)";
   }
};  // end class T5;

class R1 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const R1&);
   R1() {
      m_deloneName = "R1";
      m_bravaisType = "hR";
      m_generalType = "hR";
      m_character = "(rrr sss)";
   }
};  // end class R1;

class R3 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const R3&);
   R3() {
      m_deloneName = "R3";
      m_bravaisType = "hR";
      m_generalType = "hR";
      m_character = "(rr0 sr0) ";
   }
};  // end class R3;

class O1A : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const O1A&);
   O1A() {
      m_deloneName = "O1A";
      m_bravaisType = "oF";
      m_generalType = "oF";
      m_character = "(rrs rrt)";
   }
};  // end class O1A;

class O2 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const O2&);
   O2() {
      m_deloneName = "O2";
      m_bravaisType = "oI";
      m_generalType = "oI";
      m_character = "(rs0 srt)";
   }
};  // end class O2;

class O3 : public GrayRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const O3&);
   O3() {
      m_deloneName = "O3";
      m_bravaisType = "oI";
      m_generalType = "oI";
      m_character = "(rs0 rs0)";
   }
};  // end class O3;

class O4 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const O4&);
   O4() {
      m_deloneName = "O4";
      m_bravaisType = "oS";
      m_generalType = "oS";
      m_character = "(00r sst)";
   }
};  // end class O4;

class O5 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const O5&);
   O5() {
      m_deloneName = "O5";
      m_bravaisType = "oP";
      m_generalType = "oP";
      m_character = "(000 rst)";
   }
};  // end class O5;

class O1B : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const O1B&);
   O1B() {
      m_deloneName = "O1B";
      m_bravaisType = "oI";
      m_generalType = "oI";
      m_character = "(rst rst)";
   }
};  // end class O1B;

class M1A : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const M1A&);
   M1A() {
      m_deloneName = "M1A";
      m_bravaisType = "mC";
      m_generalType = "mS";
      m_character = "(rrs ttu)";
   }
};  // end class M1A;

class M2A : public BasicDeloneRectangle {
   public:
   friend std::ostringstream& operator<< (std::ostringstream&, const M2A&);
   M2A() {
      m_deloneName = "M2A";
      m_bravaisType = "mC";
      m_generalType = "mS";
      m_character = "(rs0 stu)";
   }
};  // end class M2A;

class M3 : public GrayRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const M3&);
   M3() {
      m_deloneName = "M3";
      m_bravaisType = "mC";
      m_generalType = "mS";
      m_character = "(rs0 ts0)";
   }
};  // end class M3;

class M4 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const M4&);
   M4() {
      m_deloneName = "M4";
      m_bravaisType = "mP";
      m_generalType = "mP";
      m_character = "(00r stu)";
   }
};  // end class M4;

class M1B : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const M1B&);
   M1B() {
      m_deloneName = "M1B";
      m_bravaisType = "mC";
      m_generalType = "mS";
      m_character = "(rst rsu)";
   }
};  // end class M1B;

class M2B : public GrayRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const M2B&);
   M2B() {
      m_deloneName = "M2B";
      m_bravaisType = "mC";
      m_generalType = "mS";
      m_character = "(rs0 rst)";
   }
};  // end class M2B;

class A1 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const A1&);
   A1() {
      m_deloneName = "A1";
      m_bravaisType = "aP";
      m_generalType = "aP";
      m_character = "(rst uvw)";
   }
};  // end class A1;

class A2 : public GrayRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const A2&);
   A2() {
      m_deloneName = "A2";
      m_bravaisType = "aP";
      m_generalType = "aP";
      m_character = "(rs0 tuv)";
   }
};  // end class A2;

class A3 : public GrayRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const A3&);
   A3() {
      m_deloneName = "A3";
      m_bravaisType = "aP";
      m_generalType = "aP";
      m_character = "(rs0 tu0)";
   }
};  // end class A3;

class H4 : public BasicDeloneRectangle {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const H4&);
   H4() {
      m_deloneName = "H4";
      m_bravaisType = "hP";
      m_generalType = "hP";
      m_character = "(00r rrs)";
   }
};  // end class H4;

class AllDeloneTypes {
public:
   AllDeloneTypes();

   void SetVertical();

   void SetHorizontal();

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

class BasicDeloneRows {

public:
   std::vector<std::string> m_row;
};

class NumericRow : public BasicDeloneRows {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const NumericRow&);

   NumericRow() {
      m_row.emplace_back(BasicDeloneRectangle().MakeSVG());

      std::cout << Digit_1().MakeSVG() << std::endl;;




      m_row.emplace_back(Digit_1().MakeSVG());
      m_row.emplace_back(Digit_2().MakeSVG());
      m_row.emplace_back(Digit_3().MakeSVG());
      m_row.emplace_back(Digit_4().MakeSVG());
      m_row.emplace_back(Digit_5().MakeSVG());
   }

};

class CRow : public BasicDeloneRows {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const CRow&);
   CRow() {
      m_row.emplace_back(Letter_C().MakeSVG());
      m_row.emplace_back(C1().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
      m_row.emplace_back(C3().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
      m_row.emplace_back(C5().MakeSVG());
   }

private:
};

class TRow : public BasicDeloneRows {
public:

   TRow() {
      m_row.emplace_back(Letter_T().MakeSVG());
      m_row.emplace_back(T1().MakeSVG());
      m_row.emplace_back(T2().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
      m_row.emplace_back(T5().MakeSVG());
   }

private:
};

class RRow : public BasicDeloneRows {
public:

   RRow() {
      m_row.emplace_back(Letter_R().MakeSVG());
      m_row.emplace_back(R1().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
      m_row.emplace_back(R3().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
   }

private:
};

class O1Row : public BasicDeloneRows {
public:

   O1Row() {
      m_row.emplace_back(Letter_O().MakeSVG());
      m_row.emplace_back(O1A().MakeSVG());
      m_row.emplace_back(O2().MakeSVG());
      m_row.emplace_back(O3().MakeSVG());
      m_row.emplace_back(O4().MakeSVG());
      m_row.emplace_back(O5().MakeSVG());
   }

private:
};

class O2Row : public BasicDeloneRows {
public:

   O2Row() {
      m_row.emplace_back(Letter_O().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
   }
};

class M1Row : public BasicDeloneRows {
public:

   M1Row() {
      m_row.emplace_back(Letter_M().MakeSVG());
      m_row.emplace_back(M1A().MakeSVG());
      m_row.emplace_back(M2A().MakeSVG());
      m_row.emplace_back(M3().MakeSVG());
      m_row.emplace_back(M4().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
   }

private:
};

class M2Row : public BasicDeloneRows {
public:

   M2Row() {
      m_row.clear();
      m_row.emplace_back(Letter_M().MakeSVG());
      m_row.emplace_back(M1B().MakeSVG());
      m_row.emplace_back(M2B().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
   }

private:
};

class ARow : public BasicDeloneRows {
public:

   ARow() {
      m_row.emplace_back(Letter_A().MakeSVG());
      m_row.emplace_back(A1().MakeSVG());
      m_row.emplace_back(A2().MakeSVG());
      m_row.emplace_back(A2().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
   }

private:
};

class HRow : public BasicDeloneRows {
public:

   HRow() {
      m_row.emplace_back(Letter_H().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
      m_row.emplace_back(H4().MakeSVG());
      m_row.emplace_back(BlankRectangle().MakeSVG());
   }

private:
};

class DeloneRows : public BasicDeloneRows {
public:
   friend std::ostringstream& operator<< (std::ostringstream&, const DeloneRows&);
   DeloneRows() {
      m_row.emplace_back(CRow());
      m_row.emplace_back(TRow());
      m_row.emplace_back(RRow());
      m_row.emplace_back(O1Row());
      m_row.emplace_back(O2Row());
      m_row.emplace_back(M1Row());
      m_row.emplace_back(M2Row());
      m_row.emplace_back(ARow());
      m_row.emplace_back(HRow());
   }
   std::vector<BasicDeloneRows> m_row;
};


#endif  // DELONEGRIDCLASSES_H

