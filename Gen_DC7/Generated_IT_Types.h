// From paciorek and bonin, 1992");
#ifndef GENERATED_IT_TYPES_H
#define GENERATED_IT_TYPES_H

#include <string>

#include "G6.h"
#include "MatG6.h"

const double g_delta = 1.0E-6;

static bool Approx(const double d1, const double d2, const double delta = g_delta) {
   return abs(d1 - d2) < delta;
}

static bool Approx(const G6& g1, const G6& g2, const double delta = g_delta) {
   return (g1 - g2).norm() < delta;
}


class IT_Lat_Char_Base {
public:
   virtual ~IT_Lat_Char_Base() {}
   IT_Lat_Char_Base();
   std::string m_name;
   std::string m_latticeType;
   std::string m_ITnumber;
   std::string m_G6_character;
   std::string m_DC7_character;
   double m_prjPrefix;
   std::string m_strProjector;
   MatG6 m_projector;  // from Paciorek & Bonin, 1992
   std::vector<G6> CreateCells(const size_t n = 0 );
   virtual std::string GetName() const { return m_name; }
   virtual std::string GetIT_Number() const { return m_ITnumber; }

   virtual bool IsMember(const G6& g) const { return true; }
   static G6 GeneratePerturbation(const G6& v, const double amountToPerturb);
   bool G4Positive(const G6& g) const { return g[3] > 0; }
   bool G4Negative(const G6& g) const { return g[3] <= 0; }
};

class IT_Lat_Char_1 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_1();
   bool IsMember(const G6& g) const { return G4Positive(g); }
};

class IT_Lat_Char_2 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_2();
   bool IsMember(const G6& g) const {return (G4Positive(g)) && (g[4] > 0.0); }
};

class IT_Lat_Char_3 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_3();
};

class IT_Lat_Char_4 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_4();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_4a : public IT_Lat_Char_4 {
public:
   IT_Lat_Char_4a() :IT_Lat_Char_4() { m_ITnumber = " 4a"; m_DC7_character = "[r,r,r,3r-3s,2r-s,2r-s,3r-s]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[4]<=0) && (g[0] >= -g[3]) && (-g[3] > g[0] / 2.0) && (g[3] <= 0.0); }
};
class IT_Lat_Char_4b : public IT_Lat_Char_4 {
public:
   IT_Lat_Char_4b() :IT_Lat_Char_4() { m_ITnumber = " 4b"; m_DC7_character = "[r,r,r,2r-s,2r-s,2r-s,s+2r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[4] <= 0) && (-g[3] <= g[0]/2.0) && (g[3] <=0.0); }
};

class IT_Lat_Char_5 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_5();
   bool IsMember(const G6& g) { return G4Negative(g); }
};

class IT_Lat_Char_6 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_6();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_6a : public IT_Lat_Char_6 {
public:
   IT_Lat_Char_6a() : IT_Lat_Char_6() { m_ITnumber = " 6a"; m_DC7_character = "[r,r,r,r,2r-2s,s+r,s+r]"; }
   bool IsMember(const G6& g) const { 
      //bool b1 = (g[0] / 2.0 >= -g[5] / 2.0);
      //double d1 = -g[5] / 2.0;
      //double d2 = g[0] / 3.0;
      //bool b2 = (-g[5] / 2.0 > g[0] / 3.0);
      //const bool b3 = d1 > d2;
      //return true;
      return (G4Negative(g)) && (g[0]/2.0>=-g[5]/2.0) && (-g[5]/2.0>g[0]/3.0) && Approx(g[3], g[4]);
   }
};

class IT_Lat_Char_6b : public IT_Lat_Char_6 {
public:
   IT_Lat_Char_6b() : IT_Lat_Char_6() { m_ITnumber = " 6b"; m_DC7_character = "[r,r,r,r,s,s+r,s+r,2r-2s]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (-g[5]/2.0 <= g[0]/3.0) && Approx(g[3], g[4]); }
};

class IT_Lat_Char_7 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_7();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_7a : public IT_Lat_Char_7 {
public:
   IT_Lat_Char_7a() : IT_Lat_Char_7(){ m_ITnumber = " 7a"; m_DC7_character = "[r,r,r,r,2r-2s,s+r,s+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[0] / 2.0 >= -g[3]/2.0) && (-g[3]/2.0 > g[0]/3.0) && Approx(g[3], g[4]); };
};
class IT_Lat_Char_7b : public IT_Lat_Char_7 {
public:
   IT_Lat_Char_7b() : IT_Lat_Char_7(){ m_ITnumber = " 7b"; m_DC7_character = "[r,r,r,r,s+r,s+r,2r-2s]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (-g[3]/2.0 <= g[0] / 3.0) && Approx(g[3], g[4]); };
};

class IT_Lat_Char_8 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_8();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_8a : public IT_Lat_Char_8 {
public:
   IT_Lat_Char_8a() : IT_Lat_Char_8() { m_ITnumber = " 8a"; m_DC7_character = "[r,r,r,r,t+s,2r-s,2r-t]"; }
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (-g[4] < -g[3]) && (-g[4] + 2 * (-g[3]) < 2 * g[0]); }
};
class IT_Lat_Char_8b : public IT_Lat_Char_8 {
public:
   IT_Lat_Char_8b() : IT_Lat_Char_8() { m_ITnumber = " 8b"; m_DC7_character = "[r,r,r,r,t+s,2r-t,2r-s]"; }
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (-g[4]>=-g[3]) && (2*(-g[4])-g[3]< 2*g[0]); }
};
class IT_Lat_Char_8c : public IT_Lat_Char_8 {
public:
   IT_Lat_Char_8c() : IT_Lat_Char_8() { m_ITnumber = " 8c"; m_DC7_character = "[r,r,r,r,2r-t,t+s,2r-s]"; }
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (-g[4]>=-g[3]) && ( 2*(-g[4])-g[3] >= 2*g[0]); }
};
class IT_Lat_Char_8d : public IT_Lat_Char_8 {
public:
   IT_Lat_Char_8d() : IT_Lat_Char_8() { m_ITnumber = " 8d"; m_DC7_character = "[r,r,r,r,2r-t,2r-s,t+s]"; }
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (-g[4]>=-g[3]) && (-g[4]-2*g[3]>=2*g[0]); }
};
class IT_Lat_Char_8e : public IT_Lat_Char_8 {
public:
   IT_Lat_Char_8e() : IT_Lat_Char_8() { m_ITnumber = " 8e"; m_DC7_character = "[r,r,r,r,2r-s,t+s,2r-t]"; }
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (-g[4]<-g[3]) && (-2*g[4]-g[3] < 2*g[0]); }
};
class IT_Lat_Char_8f : public IT_Lat_Char_8 {
public:
   IT_Lat_Char_8f() : IT_Lat_Char_8() { m_ITnumber = " 8f"; m_DC7_character = "[r,r,r,r,2r-s,2r-t,t+s]"; }
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (-g[4]<-g[3]) && (-2*g[4]-g[3]>=2*g[0]); }
};

class IT_Lat_Char_9 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_9();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_9a : public IT_Lat_Char_9 {
public:
   IT_Lat_Char_9a() : IT_Lat_Char_9() { m_ITnumber = " 9a"; m_DC7_character = "[r,r,r,3r,s,s,s]"; }
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (g[0] < g[2]/3.0); }
};
class IT_Lat_Char_9b : public IT_Lat_Char_9 {
public:
   IT_Lat_Char_9b() : IT_Lat_Char_9() { m_ITnumber = " 9b"; m_DC7_character = "[r,r,r,s,s,s,3r]"; }
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (g[2]/3.0 >= g[0]) && (g[0] < g[2]/3.0); }
};
class IT_Lat_Char_9c : public IT_Lat_Char_9 {
public:
   IT_Lat_Char_9c() : IT_Lat_Char_9() { m_ITnumber = " 9c"; m_DC7_character = "[r,r,r,s,s,s,s+r]"; }
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (g[0] >= g[2]/2.0); }
};

class IT_Lat_Char_10 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_10();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_10a : public IT_Lat_Char_10 {
public:
   IT_Lat_Char_10a() : IT_Lat_Char_10() { m_ITnumber = "10a"; m_DC7_character = "[r,r,s,-t+s+r,-t+s+r,2r-u,t+s+r]"; }
   bool IsMember(const G6& g) const { return (G4Positive(g)) && 
      (-g[3]+g[2]+g[0]< 2*g[0]-g[5]) && (2*g[0]-g[5] < g[3]+g[2]+g[0]) && Approx(g[3], g[4]); }
};
class IT_Lat_Char_10b : public IT_Lat_Char_10 {
public:
   IT_Lat_Char_10b() : IT_Lat_Char_10() { m_ITnumber = "10b"; m_DC7_character = "[r,r,s,2r-u,-t+s+r,-t+s+r,t+s+r]"; }
   bool IsMember(const G6& g) const { return (G4Positive(g)) && 
      (-g[3] + g[2] + g[0] > 2 * g[0] - g[5]) && (2 * g[0] - g[2]) && Approx(g[3], g[4]); }
};

class IT_Lat_Char_11 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_11();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_11a : public IT_Lat_Char_11 {
public:
   IT_Lat_Char_11a() : IT_Lat_Char_11() { m_ITnumber = "11a"; m_DC7_character = "[r,r,2r,2r,s,s+r,s+r]"; }
   bool IsMember(const G6& g) const { return (g[0] < g[2]/2.0); }
};
class IT_Lat_Char_11b : public IT_Lat_Char_11 {
public:
   IT_Lat_Char_11b() : IT_Lat_Char_11() { m_ITnumber = "11b"; m_DC7_character = "[r,r,s,2r,2r,s+r,s+r]"; }
   bool IsMember(const G6& g) const { return (g[0] >= g[2]/2.0); }
};

class IT_Lat_Char_12 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_12();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_12a : public IT_Lat_Char_12 {
public:
   IT_Lat_Char_12a() : IT_Lat_Char_12() { m_ITnumber = "12a"; m_DC7_character = "[r,r,r,s,s+r,s+r,s+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2] < 2.0*g[0]) && (g[5] != 0 ); }
};
class IT_Lat_Char_12b : public IT_Lat_Char_12 {
public:
   IT_Lat_Char_12b() : IT_Lat_Char_12() { m_ITnumber = "12b"; m_DC7_character = "[r,r,r,3r,s+r,s+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2] >= 2.0 * g[0]) && (g[5] != 0); }
};

class IT_Lat_Char_13 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_13();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_13a : public IT_Lat_Char_13 {
public:
   IT_Lat_Char_13a() : IT_Lat_Char_13() { m_ITnumber = "13a"; m_DC7_character = "[r,r,2r-t,s,s+r,s+r,s+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[0] < (g[2] - g[5])/2 ) && (g[5] != 0); }
};
class IT_Lat_Char_13b : public IT_Lat_Char_13 {
public:
   IT_Lat_Char_13b() : IT_Lat_Char_13() { m_ITnumber = "13b"; m_DC7_character = "[r,r,s,2r-t,s+r,s+r,s+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && ( (g[2]-g[5])/2 <=g[0]) && (g[5] != 0); }
};

class IT_Lat_Char_14 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_14();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_14a : public IT_Lat_Char_14 {
public:
   IT_Lat_Char_14a() : IT_Lat_Char_14() { m_ITnumber = "14a"; m_DC7_character = "[r,r,s,-t+s+r,-t+s+r,2r-u,t+s+r]"; }
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (-g[4]+g[2]+g[0] < 2*g[0]-g[5]) && 
      (2 * g[0] - g[5] < g[4]+g[2]+g[0]) && Approx(g[3], g[4]); }
};
class IT_Lat_Char_14b : public IT_Lat_Char_14 {
public:
   IT_Lat_Char_14b() : IT_Lat_Char_14() { m_ITnumber = "14b"; m_DC7_character = "[r,r,s,2r-u,-t+s+r,-t+s+r,t+s+r]"; }
   bool IsMember(const G6& g) const { return (G4Positive(g)) && 
      (-g[4] + g[2] + g[0] >= 2 * g[0] - g[5]) && (2 * g[0] - g[5] < g[2]) && Approx(g[3], g[4]); }
};

class IT_Lat_Char_15 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_15();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_15a : public IT_Lat_Char_15 {
public:
   IT_Lat_Char_15a() : IT_Lat_Char_15() { m_ITnumber = "15a"; m_DC7_character = "[r,r,s,s,s,s,2r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2] < 2*g[0]) && (g[5] == 0); }
};
class IT_Lat_Char_15b : public IT_Lat_Char_15 {
public:
   IT_Lat_Char_15b() : IT_Lat_Char_15() { m_ITnumber = "15b"; m_DC7_character = "[r,r,2r,2r,s,s,s]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2] >= g[0]) && (g[5] == 0); }
};

class IT_Lat_Char_16 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_16();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_16a : public IT_Lat_Char_16 {
public:
   IT_Lat_Char_16a() : IT_Lat_Char_16() { m_ITnumber = "16a"; m_DC7_character = "[r,r,s,s,2t,4r-2t,-t+s+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && 
      (3.0*g[0] <g[2] -g[3]) && (g[3]+g[2]+g[0] < -2*g[3]) && Approx(g[3], g[4]); }
};
class IT_Lat_Char_16b : public IT_Lat_Char_16 {
public:
   IT_Lat_Char_16b() : IT_Lat_Char_16() { m_ITnumber = "16b"; m_DC7_character = "[r,r,s,s,-t+s+r,-t+s+r,2t]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && 
      (3.0 * g[0] >= g[2] - g[3]) && (g[3] + g[2] + g[0] < -2*g[3]) && Approx(g[3], g[4]); }
};

class IT_Lat_Char_17 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_17();
   bool IsMember(const G6& g) const { return (G4Negative(g)) && Approx(2*g[0]+g[5]+g[3],-g[5]/2); }
};

class IT_Lat_Char_18 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_18();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_18a : public IT_Lat_Char_18 {
public:
   IT_Lat_Char_18a() : IT_Lat_Char_18() { m_ITnumber = "18a"; m_DC7_character = "[r,s,s,s,s,2s-r/2,2s-r/2]"; }
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (g[0] <=g[1]) && (g[1] < 5.0*g[0]/2.0) && Approx(g[4],g[5]); }
};
class IT_Lat_Char_18b : public IT_Lat_Char_18 {
public:
   IT_Lat_Char_18b(): IT_Lat_Char_18() { m_ITnumber = "18b"; m_DC7_character = "[r,s,s,s,s,s+r2,s+2r]"; };
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (g[1] >= 5.0*g[0]/2.0) && Approx(g[4], g[5]);; }
};

class IT_Lat_Char_19 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_19();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_19a : public IT_Lat_Char_19 {
public:
   IT_Lat_Char_19a() : IT_Lat_Char_19() { m_ITnumber = "19a"; m_DC7_character = "[]"; };
   bool IsMember(const G6& g) const { return  (G4Positive(g)) && (g[0] < (g[1]-g[3])/2.0) && Approx(g[5], g[4]); }
};
class IT_Lat_Char_19b : public IT_Lat_Char_19 {
public:
   IT_Lat_Char_19b() : IT_Lat_Char_19() { m_ITnumber = "19b"; m_DC7_character = "[]"; };
   bool IsMember(const G6& g) const { return (G4Positive(g)) && ((g[1] - g[3]) / 2.0 <= g[0]) && (g[0] < g[1]-g[3]) && Approx(g[5], g[4]); }
};

class IT_Lat_Char_20 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_20();
   bool IsMember(const G6& g) const { return Approx(g[5], g[4]); } //Roof says either +++ or ---
};

class IT_Lat_Char_21 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_21();
};

class IT_Lat_Char_22 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_22();
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[3] != 0); }
};

class IT_Lat_Char_23 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_23();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_23a : public IT_Lat_Char_23 {
public:
   IT_Lat_Char_23a() : IT_Lat_Char_23() { m_ITnumber = "23a"; m_DC7_character = "[r,s,s,s+r,s+r,s+r,s+r]"; };
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[0] < g[1] +g[3]) && (g[3] != 0); }
};
class IT_Lat_Char_23b : public IT_Lat_Char_23 {
public:
   IT_Lat_Char_23b() : IT_Lat_Char_23() { m_ITnumber = "23b"; m_DC7_character = "[r,s,s,2s-t,s+t,s+r,s+r]"; };
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[0] >= g[1] + g[3]) && (g[3] != 0); }
};

class IT_Lat_Char_24 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_24();
   bool IsMember(const G6& g) const { return (G4Negative(g)) && Approx(g[4],g[5]); }
};

class IT_Lat_Char_25 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_25();
   bool IsMember(const G6& g) const { return Approx(g[5], g[4]); } //Roof says either +++ or ---
};

class IT_Lat_Char_26 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_26();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_26a : public IT_Lat_Char_26 {
public:
   IT_Lat_Char_26a() : IT_Lat_Char_26() { m_ITnumber = "26a"; m_DC7_character = "[r,s,s,t,t,t+2r,t+2r]"; };
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (g[2] < g[1]+2*g[0]) && (5*g[0]/2 < g[1]) && Approx(g[5], g[4]); }
};
class IT_Lat_Char_26b : public IT_Lat_Char_26 {
public:
   IT_Lat_Char_26b() : IT_Lat_Char_26() { m_ITnumber = "26b"; m_DC7_character = "[r,s,s,t,t,s+2r<t+s-r/2]"; };
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (g[2] < g[1]+2*g[0]) && (g[1] <= 5*g[0]/2) && Approx(g[5], g[4]); }
};
class IT_Lat_Char_26c : public IT_Lat_Char_26 {
public:
   IT_Lat_Char_26c() : IT_Lat_Char_26() { m_ITnumber = "26c"; m_DC7_character = "[r,s,s,s+2r,t,t,t+2r]"; };
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (g[2] >= g[1]+2*g[0]) && (5*g[0]/2 < g[1]) && Approx(g[5], g[4]); }
};
class IT_Lat_Char_26d : public IT_Lat_Char_26 {
public:
   IT_Lat_Char_26d() : IT_Lat_Char_26() { m_ITnumber = "26d"; m_DC7_character = "[r,s,s,s+2r,t,t,t+s-r/2]"; };
   bool IsMember(const G6& g) const { return (G4Positive(g)) && (g[2] >= g[1]+2*g[0]) && (g[1] <=5*g[0]/2) && Approx(g[5], g[4]); }
};

class IT_Lat_Char_27 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_27();
   bool IsMember(const G6& g) const { return (G4Positive(g)) && ((g[0]-g[4]<1.0e-6) && ((g[0]-g[5])<1.0e-6)) && Approx(g[4],g[5]); }
};

class IT_Lat_Char_28 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_28();
   bool IsMember(const G6& g) const { return (G4Positive(g)) && Approx(g[3], g[5]/2); }
};

class IT_Lat_Char_29 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_29();
   bool IsMember(const G6& g) const { return (G4Positive(g)) && Approx(g[3], g[4] / 2); }
};

class IT_Lat_Char_30 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_30();
   bool IsMember(const G6& g) const { return (G4Positive(g)) && Approx(g[4], g[5] / 2); }
};

class IT_Lat_Char_32 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_32();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_32a : public IT_Lat_Char_32 {
public:
   IT_Lat_Char_32a() : IT_Lat_Char_32() { m_ITnumber = "32a"; m_DC7_character = "[r,s,t,s+r,s+r,t+r,t+r]"; };
   bool IsMember(const G6& g) const { return g[2] < g[1]+g[0]; }
};
class IT_Lat_Char_32b : public IT_Lat_Char_32 {
public:
   IT_Lat_Char_32b() : IT_Lat_Char_32() { m_ITnumber = "32b"; m_DC7_character = "[r,s,s+r,s+r,t,t+r,t+r]"; };
   bool IsMember(const G6& g) const { return g[2] >= g[1] + g[0]; }
};


#define r (g[0])
#define s (g[1])
#define t (g[2])
#define u (-g[3])

class IT_Lat_Char_33 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_33();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_33a : public IT_Lat_Char_33 {
public:
   IT_Lat_Char_33a() : IT_Lat_Char_33() { m_ITnumber = "33a"; m_DC7_character = "[r,s,t,s+r,s+r,-u+t+r,t+s]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[1]-g[4] < g[2]) && (g[2]<g[1]+g[0]) && (g[4] != 0); }
};
class IT_Lat_Char_33b : public IT_Lat_Char_33 {
public:
   IT_Lat_Char_33b() : IT_Lat_Char_33() { m_ITnumber = "33b"; m_DC7_character = "[r,s,t,-u+t+r,s+r,s+r,t+s]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2] <=g[1]-g[4]) && (g[4] != 0); }
};
class IT_Lat_Char_33c : public IT_Lat_Char_33 {
public:
   IT_Lat_Char_33c() : IT_Lat_Char_33() { m_ITnumber = "33c"; m_DC7_character = "[r,s,s+r,s+r,t,-u+t+r,t+s]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2] >=g[1]+g[0]) && (g[4] != 0); }
};

class IT_Lat_Char_34 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_34();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_34a : public IT_Lat_Char_34 {
public:
   IT_Lat_Char_34a() : IT_Lat_Char_34() { m_ITnumber = "34a";  m_DC7_character = "[r,s,t,s+r,s+r,t+r,t+r] HJB no u"; } // HJB no "u" 
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (t <r + s) && (r + u < s) && (g[4] != 0); }
};
class IT_Lat_Char_34b : public IT_Lat_Char_34 {
public:
   IT_Lat_Char_34b() : IT_Lat_Char_34() { m_ITnumber = "34b";  m_DC7_character = "[r,s,t,s+r,s+r,-u+t+s.t+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (t<r+s) && (t > r + u) && (r + u >= s) && (g[4] != 0); }
};
class IT_Lat_Char_34c : public IT_Lat_Char_34 {
public:
   IT_Lat_Char_34c() : IT_Lat_Char_34() { m_ITnumber = "34c";  m_DC7_character = "[r,s,t,-u+t+s,s+r,s+r,t+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (t < r+s) && (r+u>=t) && (g[4] != 0); }
};
class IT_Lat_Char_34d : public IT_Lat_Char_34 {
public:
   IT_Lat_Char_34d() : IT_Lat_Char_34() { m_ITnumber = "34d";  m_DC7_character = "[r,s,s+r,s+r,t,t+r,t+r] HJB no u"; } // HJB no "u" 
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (t >=r+s ) && (r+u <s) && (g[4] != 0); }
};
class IT_Lat_Char_34e : public IT_Lat_Char_34 {
public:
   IT_Lat_Char_34e() : IT_Lat_Char_34() { m_ITnumber = "34e";  m_DC7_character = "[r,s,s,s+r,s+r,t,-u+t+s,t+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (t>=r+s) && (r+u>=s) && (g[4] != 0); }
};

#undef r
#undef s
#undef t
#undef u

class IT_Lat_Char_35 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_35();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_35a : public IT_Lat_Char_35 {
public:
   IT_Lat_Char_35a() : IT_Lat_Char_35() { m_ITnumber = "35a";  m_DC7_character = "[r,s,t,-u+s+r,t+r,t+r,t+s]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2]<g[5]+g[1]+g[0]) && (g[2]+g[1]<-g[5]+g[1]+g[0]) && (g[5] != 0); }
};
class IT_Lat_Char_35b : public IT_Lat_Char_35 {
public:
   IT_Lat_Char_35b() : IT_Lat_Char_35() { m_ITnumber = "35b";  m_DC7_character = "[r,s,-u+s+r,t,t+r,t+r,t+s]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2]>=g[5]+g[1]+g[0]) && (g[2]+g[1] < -g[5]+g[2]+g[0]) && (g[5] != 0); }
};
class IT_Lat_Char_35c : public IT_Lat_Char_35 {
public:
   IT_Lat_Char_35c() : IT_Lat_Char_35() { m_ITnumber = "35c";  m_DC7_character = "[r,s,t,-u+s+r,u+s+r,t+r,t+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2] < g[5]+g[1]+g[0]) && (-g[5]+g[1]+g[0]>g[2]+g[0]) && (g[5] != 0); }
};
class IT_Lat_Char_35d : public IT_Lat_Char_35 {
public:
   IT_Lat_Char_35d() : IT_Lat_Char_35() { m_ITnumber = "35d";  m_DC7_character = "[r,s,-u+s+r,t,u+s+r,t+r,t+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (-g[5]+g[1]+g[0] >=g[2]) && (g[2] >= g[5]+g[1]+g[0]) && (-g[5]+g[1]+g[0] < g[2]+g[0]) && (g[5] != 0); }
};

class IT_Lat_Char_36 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_36();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_36a : public IT_Lat_Char_36 {
public:
   IT_Lat_Char_36a() : IT_Lat_Char_36() { m_ITnumber = "36a";  m_DC7_character = "[r,s,t,s+r,s+r,t+r,t+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2] < g[1]+g[0]) && (g[1] > 2*g[0]) && (g[4] != 0); }
};
class IT_Lat_Char_36b : public IT_Lat_Char_36 {
public:
   IT_Lat_Char_36b() : IT_Lat_Char_36() { m_ITnumber = "36b";  m_DC7_character = "[r,s,t,s+r,s+r,-u+t+s,t+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2]>= g[1]+g[0]) && (g[1] < 2*g[0]) && (g[4] != 0); }
};
class IT_Lat_Char_36c : public IT_Lat_Char_36 {
public:
   IT_Lat_Char_36c() : IT_Lat_Char_36() { m_ITnumber = "36c";  m_DC7_character = "[r,s,t,-u+t+s,s+r,s+r,t+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2]<g[1]+g[0]) && (g[1] >= 2*g[0]) && (g[4] != 0); }
};
class IT_Lat_Char_36d : public IT_Lat_Char_36 {
public:
   IT_Lat_Char_36d() : IT_Lat_Char_36() { m_ITnumber = "36d";  m_DC7_character = "[r,s,s+r,s+r,t,t+r,t+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2]>=g[1]+g[0]) && (g[1] >=g[0]) && (g[4] != 0); }
};

class IT_Lat_Char_37 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_37();
};

class IT_Lat_Char_38 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_38();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_38a : public IT_Lat_Char_38 {
public:
   IT_Lat_Char_38a() : IT_Lat_Char_38() { m_ITnumber = "38a";  m_DC7_character = "[r,s,s,t,t+r,t+r,t+s]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2] < 2*g[0]) && (g[5] != 0); }
};
class IT_Lat_Char_38b : public IT_Lat_Char_38 {
public:
   IT_Lat_Char_38b() : IT_Lat_Char_38() { m_ITnumber = "38b";  m_DC7_character = "[r,s,s,t,t+r,t+r,s+2r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (2*g[0]<g[2]) && (g[2]<g[1]+g[0]) && (g[5] != 0); }
};
class IT_Lat_Char_38c : public IT_Lat_Char_38 {
public:
   IT_Lat_Char_38c() : IT_Lat_Char_38() { m_ITnumber = "38c";  m_DC7_character = "[r,s,s,t,s+2r,t+r,t+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[1]+g[0] <= g[2]) && (g[2] < g[1]+2*g[0]) && (g[5] != 0); }
};
class IT_Lat_Char_38d : public IT_Lat_Char_38 {
public:
   IT_Lat_Char_38d() : IT_Lat_Char_38() { m_ITnumber = "38d";  m_DC7_character = "[r,s,s,s+2r,t,t+r,t+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[1]+2*g[0]<=g[2]) && (g[5] != 0); }
};

class IT_Lat_Char_39 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_39();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_40 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_40();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_40a : public IT_Lat_Char_40 {
public:
   IT_Lat_Char_40a() : IT_Lat_Char_40() { m_ITnumber = "40a"; m_DC7_character = "[r,s,t,s+r,s+r,t+r,t+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2] < g[1]+g[0]) && (g[3] != 0); }
};
class IT_Lat_Char_40b : public IT_Lat_Char_40 {
public:
   IT_Lat_Char_40b() : IT_Lat_Char_40() { m_ITnumber = "40b"; m_DC7_character = "[r,s,s+r,s=r,t,t+r,t+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2] >=g[1]+g[0]) && (g[3] != 0); }
};

class IT_Lat_Char_41 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_41();
};

class IT_Lat_Char_42 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_42();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_42a : public IT_Lat_Char_42 {
public:
   IT_Lat_Char_42a() : IT_Lat_Char_42() { m_ITnumber = "42a"; m_DC7_character = "[r,s,t,t,t,t,s+r]"; }
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2] < g[1] + g[0]) && (g[5] == 0); }
};
class IT_Lat_Char_42b : public IT_Lat_Char_42 {
public:
   IT_Lat_Char_42b() : IT_Lat_Char_42() { m_ITnumber = "42b"; m_DC7_character = "[r,s,s+r,s+r,t,t,t]"; };
   bool IsMember(const G6& g) const { return (G4Negative(g)) && (g[2] >= g[1] + g[0]) && (g[5] == 0); }
};


class IT_Lat_Char_43 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_43();
   bool IsMember(const G6& g) const { return Approx(g[0] + g[4], g[5] / 2); }
};



#endif  // GENERATED_IT_TYPES_H