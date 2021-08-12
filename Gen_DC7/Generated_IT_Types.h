// From paciorek and bonin, 1992");
#ifndef GENERATED_IT_TYPES_H
#define GENERATED_IT_TYPES_H

#include <string>

#include "G6.h"
#include "MatG6.h"

class IT_Lat_Char_Base {
public:
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

};

class IT_Lat_Char_1 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_1();
};

class IT_Lat_Char_2 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_2();
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

class IT_Lat_Char_5 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_5();
};

class IT_Lat_Char_6 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_6();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_7 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_7();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_8 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_8();
};

class IT_Lat_Char_9 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_9();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_10 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_10();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_11 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_11();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_11a : public IT_Lat_Char_11 {
public:
   IT_Lat_Char_11a() : IT_Lat_Char_11() { m_ITnumber = "11a"; m_DC7_character = "(r,r,2r,2r,s,s+r,s+r)"; }
   bool IsMember(const G6& g) const { return (g[0] < g[2]/2.0); }
};
class IT_Lat_Char_11b : public IT_Lat_Char_11 {
public:
   IT_Lat_Char_11b() : IT_Lat_Char_11() { m_ITnumber = "11b"; m_DC7_character = "(r,r,2s,2r,2r,s+r,s+r)"; }
   bool IsMember(const G6& g) const { return (g[0] >= g[2]/2.0); }
};

class IT_Lat_Char_12 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_12();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_13 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_13();
};

class IT_Lat_Char_14 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_14();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_15 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_15();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_16 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_16();
};

class IT_Lat_Char_17 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_17();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_18 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_18();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_19 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_19();
};

class IT_Lat_Char_20 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_20();
};

class IT_Lat_Char_21 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_21();
};

class IT_Lat_Char_22 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_22();
};

class IT_Lat_Char_23 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_23();
};

class IT_Lat_Char_24 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_24();
};

class IT_Lat_Char_25 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_25();
};

class IT_Lat_Char_26 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_26();
};

class IT_Lat_Char_27 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_27();
};

class IT_Lat_Char_28 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_28();
};

class IT_Lat_Char_29 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_29();
};

class IT_Lat_Char_30 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_30();
};

class IT_Lat_Char_32 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_32();
   bool IsMember(const G6& g) const = 0;
};

class IT_Lat_Char_34 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_34();
   bool IsMember(const G6& g) const = 0;
};


#define r (g[0])
#define s (g[1])
#define t (g[2])
#define u (-g[3])

class IT_Lat_Char_34a : public IT_Lat_Char_34 {
public:
   IT_Lat_Char_34a() : IT_Lat_Char_34() { m_ITnumber = "34a"; }
   bool IsMember(const G6& g) const { return (t <r + s) && (r + u < s); }
};
class IT_Lat_Char_34b : public IT_Lat_Char_34 {
public:
   IT_Lat_Char_34b() : IT_Lat_Char_34() { m_ITnumber = "34b"; }
   bool IsMember(const G6& g) const { return (t<r+s) && (t > t + u) && (r + u >= s); }
};
class IT_Lat_Char_34c : public IT_Lat_Char_34 {
public:
   IT_Lat_Char_34c() : IT_Lat_Char_34() { m_ITnumber = "34c"; }
   bool IsMember(const G6& g) const { return (t < r+s) && (r+u>=t); }
};
class IT_Lat_Char_34d : public IT_Lat_Char_34 {
public:
   IT_Lat_Char_34d() : IT_Lat_Char_34() { m_ITnumber = "34d"; }
   bool IsMember(const G6& g) const { return (t >=r+s ) && (r+u <s); }
};
class IT_Lat_Char_34e : public IT_Lat_Char_34 {
public:
   IT_Lat_Char_34e() : IT_Lat_Char_34() { m_ITnumber = "34e"; }
   bool IsMember(const G6& g) const { return (t>=r+s) && (r+u>=s); }
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

class IT_Lat_Char_36 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_36();
};

class IT_Lat_Char_33 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_33();
};

class IT_Lat_Char_37 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_37();
};

class IT_Lat_Char_38 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_38();
};

class IT_Lat_Char_39 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_39();
};

class IT_Lat_Char_40 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_40();
};

class IT_Lat_Char_41 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_41();
};

class IT_Lat_Char_42 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_42();
};

class IT_Lat_Char_43 : public IT_Lat_Char_Base {
public:
   IT_Lat_Char_43();
};



#endif  // GENERATED_IT_TYPES_H