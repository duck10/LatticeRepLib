#ifndef GENERATELATTICETYPEEXAMPLES_H
#define GENERATELATTICETYPEEXAMPLES_H

#include <string>
#include <vector>

#include "G6.h"
#include "MatG6.h"
#include "rhrand.h"
#include "S6.h"

static RHrand rhrand;

template<typename T>
T CreateUnitOrthogonalComponent(const T& t) {
   // assume t is not null and rand is not parallel to t
   const T base = t / t.norm();
   T temp;
   for (size_t i = 0; i < 6; ++i) temp[i] = rhrand.urand() - 0.5;
   temp /= temp.norm();
   const double d = dot(VecN(base.GetVector()), VecN(temp.GetVector()));
   T ortho = temp - d * base;
   ortho.SetValid(true);
   return ortho / ortho.norm();
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// add some small random perturbation
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
template<typename T>
T PerturbVector(const T& v, const double amountToPerturb) {
   const T out = v + amountToPerturb * CreateUnitOrthogonalComponent(v);
   return out;
}

template<typename T>
T PerturbVector(const double amountToPerturb, const T& v) {
   const T ortho = amountToPerturb * CreateUnitOrthogonalComponent(v);
   const T out = v + ortho;
   return out;
}

static std::vector<std::string> GetUpwardBravaisSymmetry(const std::string& s) {
   //Here's the up-going connections according to Grimmer, 2015

   //   aP->mP, mS
   //   mp->oP, oS
   //   mS->oS, oF, oI, hR
   //   oP->tP
   //   oS->tP, hP
   //   oF->tI
   //   oI->tI
   //   tP->cP
   //   hP->NA
   //   hR->cP, cF, cI
   //   tI->cF, cI
   if (s == "aP")  return std::vector<std::string> {"mP", "mS"};
   else if (s == "mP")  return std::vector<std::string> {"oP", "oS"};
   else if (s == "mS")  return std::vector<std::string> {"oS", "oF", "oI", "hR"};
   else if (s == "oP")  return std::vector<std::string> {"tP"};
   else if (s == "oS")  return std::vector<std::string> {"tP", "hP"};
   else if (s == "oF")  return std::vector<std::string> {"tI"};
   else if (s == "oI")  return std::vector<std::string> {"tI"};
   else if (s == "tP")  return std::vector<std::string> {"cP"};
   //else if ( s == "hP") return std::vector<std::string>();
   else if (s == "hR")  return std::vector<std::string> {"cP", "cF", "cI"};
   else if (s == "tI")  return std::vector<std::string> {"cF", "cI"};
   else return std::vector<std::string> {"mP", "mS"};
}

class GenerateLatticesBase {
public:
   friend std::ostream& operator<< (std::ostream&, const GenerateLatticesBase&);
   GenerateLatticesBase();
   ~GenerateLatticesBase() {}
   virtual bool IsMember(const G6& g) const { g; return true; }
   std::string GetName(void) const { return m_name; }
   std::string GetBravaisType(void) const { return m_bravaisType; }
   std::string GetCharacter(void) const { return m_character; }
   int GetFreeParams(void) const { return m_freeParams; }
   std::string GetStringProjector(void) const { return m_strProjector; }
   G6 GeneratePerturbation(const G6& v, const double amountToPerturb);
   template<typename T>
   static T VecAdd(const T& t1, const T& t2) {
      T tx(t1);
      tx.insert(tx.end(), t2.begin(), t2.end());
      return tx;
   }

protected:
   std::string m_name; // either Delone name (C5) or Niggli name (55B)
   std::string m_bravaisLatticeGeneral; // like oS, or mS for face centered
   std::string m_bravaisType;
   std::string m_character;
   int m_freeParams;
   double m_prjPrefix;
   std::string m_strProjector;
};


class GenerateNiggliBase : public GenerateLatticesBase {
public:
   friend std::ostream& operator<< (std::ostream&, const GenerateNiggliBase&);
   GenerateNiggliBase() {}
   virtual ~GenerateNiggliBase() {}
   virtual MatG6 GetPrj(void) const { return m_projector; }
   virtual MatG6 GetPerp(void) const { return MatG6().unit() - m_projector; }
   std::string GetITNumber(void) const { return m_ITnumber; }
   std::string GetBravaisLatticeGeneral() const { return m_bravaisLatticeGeneral; }
   std::vector<std::shared_ptr<GenerateNiggliBase> >
      Select(const std::string& s = "") const;
   //std::string GetITNum() const { return m_ITnumber; }
   std::string GetXtalType(const std::string& s) const { return m_bravaisType; }
   MatG6 GetToCenter() const { return m_toCentered; }
   static std::vector<G6>
      Generate(const std::string& s, const size_t n = 1);
   virtual bool IsMember(const G6& g) const { return true; }
   bool IsTypeI(const G6& g) const;
   bool IsTypeII(const G6& g) const;

protected:
   static bool Approx(const double d1, const double d2);
   static bool Approx(const double d1, const double d2, const double d3);
   static bool Approx(const G6& g1, const G6& g2);
   bool G4Positive(const G6& g) const { return g[3] > 0; }
   bool G4Negative(const G6& g) const { return g[3] <= 0; }

   std::string m_toCentered_E3;
   std::string m_ITnumber;
   double m_prjPrefix;
   MatG6 m_projector;
   MatG6 m_toCentered; // International Tables Table 3.1.3.1
};



class IT_1 : public GenerateNiggliBase {
public:
   IT_1();
}
;
class IT_2 : public GenerateNiggliBase {
public:
   IT_2();
   bool IsMember(const G6& g) const {
      return IsTypeI(g);
   }
}
;
class IT_3 : public GenerateNiggliBase {
public:
   IT_3();
}
;
class IT_4 : public GenerateNiggliBase {
public:
   IT_4();
   bool IsMember(const G6& g) const {
      return IsTypeII(g);
   }
}
;
class IT_5 : public GenerateNiggliBase {
public:
   IT_5();
}
;
class IT_6 : public GenerateNiggliBase {
public:
   IT_6();
}
;
class IT_7 : public GenerateNiggliBase {
public:
   IT_7();
}
;
class IT_8 : public GenerateNiggliBase {
public:
   IT_8();
}
;
class IT_9 : public GenerateNiggliBase {
public:
   IT_9();
}
;
class IT_10 : public GenerateNiggliBase {
public:
   IT_10();
}
;
class IT_11 : public GenerateNiggliBase {
public:
   IT_11();
}
;
class IT_12 : public GenerateNiggliBase {
public:
   IT_12();
}
;
class IT_13 : public GenerateNiggliBase {
public:
   IT_13();
}
;
class IT_14 : public GenerateNiggliBase {
public:
   IT_14();
}
;
class IT_15 : public GenerateNiggliBase {
public:
   IT_15();
}
;
class IT_16 : public GenerateNiggliBase {
public:
   IT_16();
}
;
class IT_17 : public GenerateNiggliBase {
public:
   IT_17();
}
;
class IT_18 : public GenerateNiggliBase {
public:
   IT_18();
}
;
class IT_19 : public GenerateNiggliBase {
public:
   IT_19();
}
;
class IT_20 : public GenerateNiggliBase {
public:
   IT_20();
   bool IsMember(const G6& g) const {
      return IsTypeI(g);
   }
}
;
class IT_21 : public GenerateNiggliBase {
public:
   IT_21();
}
;
class IT_22 : public GenerateNiggliBase {
public:
   IT_22();
}
;
class IT_23 : public GenerateNiggliBase {
public:
   IT_23();
}
;
class IT_24 : public GenerateNiggliBase {
public:
   IT_24();
}
;
class IT_25 : public GenerateNiggliBase {
public:
   IT_25();
   bool IsMember(const G6& g) const {
      return IsTypeII(g);
   }
}
;
class IT_26 : public GenerateNiggliBase {
public:
   IT_26();
}
;
class IT_27 : public GenerateNiggliBase {
public:
   IT_27();
}
;
class IT_28 : public GenerateNiggliBase {
public:
   IT_28();
}
;
class IT_29 : public GenerateNiggliBase {
public:
   IT_29();
}
;
class IT_30 : public GenerateNiggliBase {
public:
   IT_30();
}
;
class IT_31 : public GenerateNiggliBase {
public:
   IT_31();
   bool IsMember(const G6& g) const {
      return IsTypeI(g);
   }
}
;
class IT_32 : public GenerateNiggliBase {
public:
   IT_32();
}
;
class IT_33 : public GenerateNiggliBase {
public:
   IT_33();
}
;
class IT_34 : public GenerateNiggliBase {
public:
   IT_34();
}
;
class IT_35 : public GenerateNiggliBase {
public:
   IT_35();
}
;
class IT_36 : public GenerateNiggliBase {
public:
   IT_36();
}
;
class IT_37 : public GenerateNiggliBase {
public:
   IT_37();
}
;
class IT_38 : public GenerateNiggliBase {
public:
   IT_38();
}
;
class IT_39 : public GenerateNiggliBase {
public:
   IT_39();
}
;
class IT_40 : public GenerateNiggliBase {
public:
   IT_40();
}
;
class IT_41 : public GenerateNiggliBase {
public:
   IT_41();
}
;
class IT_42 : public GenerateNiggliBase {
public:
   IT_42();
}
;
class IT_43 : public GenerateNiggliBase {
public:
   IT_43();
}
;
class IT_44 : public GenerateNiggliBase {
public:
   IT_44();
   bool IsMember(const G6& g) const {
      return IsTypeII(g);
   }
}
;


class GenerateDeloneBase : public GenerateLatticesBase {
public:
   friend std::ostream& operator<< (std::ostream&, const GenerateDeloneBase&);
   friend std::ostream& operator<< (std::ostream&, const GenerateDeloneBase*);
   GenerateDeloneBase() {}; // LCA
   virtual ~GenerateDeloneBase() {}
   virtual MatS6 GetPrj(void) const { return m_projector; }
   virtual MatS6 GetPerp(void) const { return MatS6().unit() - m_projector; }

   std::string GetBravaisLatticeGeneral() const { return m_bravaisLatticeGeneral; }
   std::vector<std::shared_ptr<GenerateNiggliBase> >
      Select(const std::string& s = "") const;
   std::string GetXtalType(const std::string& s) { return m_bravaisType; }
   static std::vector<S6>
      Generate(const std::string& s, const size_t n = 1);

protected:
   double m_prjPrefix;
   MatS6 m_projector;
};

class Delone_C1 : public GenerateDeloneBase {
public:
   Delone_C1();
   ~Delone_C1() {}
};

class Delone_C3 : public GenerateDeloneBase {
public:
   Delone_C3();
   ~Delone_C3() {}
};

class Delone_C5 : public GenerateDeloneBase {
public:
   Delone_C5();
   ~Delone_C5() {}
};

class Delone_T1 : public GenerateDeloneBase {
public:
   Delone_T1();
  ~ Delone_T1() {}
};

class Delone_T2 : public GenerateDeloneBase {
public:
   Delone_T2();
   ~Delone_T2() {}
};

class Delone_T5 : public GenerateDeloneBase {
public:
   Delone_T5();
   ~Delone_T5() {}
};

class Delone_R1 : public GenerateDeloneBase {
public:
   Delone_R1();
  ~ Delone_R1() {}
};

class Delone_R3 : public GenerateDeloneBase {
public:
   Delone_R3();
   ~Delone_R3() {}
};

class Delone_O1A : public GenerateDeloneBase {
public:
   Delone_O1A();
  ~ Delone_O1A() {}
};

class Delone_O1B : public GenerateDeloneBase {
public:
   Delone_O1B();
   ~Delone_O1B() {}
};
class Delone_O2 : public GenerateDeloneBase {
public:
   Delone_O2();
   ~Delone_O2() {}
};
class Delone_O3 : public GenerateDeloneBase {
public:
   Delone_O3();
   ~Delone_O3() {}
};
class Delone_O4 : public GenerateDeloneBase {
public:
   Delone_O4();
   ~Delone_O4() {}
};
class Delone_O5 : public GenerateDeloneBase {
public:
   Delone_O5();
   ~Delone_O5() {}
};
class Delone_M1A : public GenerateDeloneBase {
public:
   Delone_M1A();
   ~Delone_M1A() {}
};
class Delone_M1B : public GenerateDeloneBase {
public:
   Delone_M1B();
   ~Delone_M1B() {}
};
class Delone_M2B : public GenerateDeloneBase {
public:
   Delone_M2B();
   ~Delone_M2B() {}
};
class Delone_M2A : public GenerateDeloneBase {
public:
   Delone_M2A();
  ~ Delone_M2A() {}
};
class Delone_M3 : public GenerateDeloneBase {
public:
   Delone_M3();
   ~Delone_M3() {}
};
class Delone_M4 : public GenerateDeloneBase {
public:
   Delone_M4();
   ~Delone_M4() {}
};
class Delone_A1 : public GenerateDeloneBase {
public:
   Delone_A1();
   ~Delone_A1() {}
};
class Delone_A2 : public GenerateDeloneBase {
public:
   Delone_A2();
   ~Delone_A2() {}
};
class Delone_A3 : public GenerateDeloneBase {
public:
   Delone_A3();
   ~Delone_A3() {}
};
class Delone_H4 : public GenerateDeloneBase {
public:
   Delone_H4();
   ~Delone_H4() {}
};


class GenerateLatticeTypeExamples
{
public:
   GenerateLatticeTypeExamples();
   ~GenerateLatticeTypeExamples() {}
   static std::vector<std::shared_ptr<GenerateNiggliBase> > CreateListOfNiggliTypes();
   static std::vector<std::shared_ptr<GenerateDeloneBase> > CreateListOfDeloneTypes();
};


#endif // GENERATELATTICETYPEEXAMPLES_H
