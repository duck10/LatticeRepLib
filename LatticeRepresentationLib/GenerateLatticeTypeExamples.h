#ifndef GENERATELATTICETYPEEXAMPLES_H
#define GENERATELATTICETYPEEXAMPLES_H

#include <string>
#include <vector>

#include "G6.h"
#include "MatG6.h"
#include "S6.h"

template<typename T>
T GeneratePerturbation(const T& v, const double amountToPerturb) {
   return (1.0 + amountToPerturb) * v;
}

class GenerateLatticesBase {
public:
   GenerateLatticesBase() {}
   ~GenerateLatticesBase() {}
   virtual bool IsMember(const G6& g) const { g; return true; }
   std::string GetName(void) const { return m_name; }
   std::string GetBravaisType(void) const { return m_bravaisType; }
   std::string GetCharacter(void) const { return m_character; }
   int GetFreeParams(void) const { return m_freeParams; }
   std::string GetStringProjector(void) const { return m_strProjector; }
protected:
   std::string m_name; // either Delone name (C5) or Niggli name (55B)
   std::string m_bravaisLatticeGeneral; // like oS, or mS for face centered
   std::string m_bravaisType;
   std::string m_character;
   int m_freeParams;
   std::string m_strProjector;
};


class GenerateNiggliBase : public GenerateLatticesBase {
public:
   friend std::ostream& operator<< (std::ostream&, const GenerateNiggliBase&);
   GenerateNiggliBase() {}
   ~GenerateNiggliBase() {}
   virtual MatG6 GetPrj(void) const { return m_projector; }
   virtual MatG6 GetPerp(void) const { return MatG6().unit() - m_projector; }
   std::string GetITNumber(void) const { return m_ITnumber; }
   std::string GetBravaisLatticeGeneral() const { return m_bravaisType; }

protected:
   std::string m_toCentered_E3;
   MatG6  m_toCentered;
   std::string m_ITnumber;
   double m_prjPrefix;
   MatG6 m_projector;
};

//class GenerateDeloneBase : public GenerateLatticesBase {
//public:
//   GenerateDeloneBase();
//   ~GenerateDeloneBase() {}
//   virtual std::vector<MatS6> GetPrj(void) const;
//   virtual std::vector<MatS6> GetPerp(void) const;
//};

class GenerateLatticeTypeExamples
{
   GenerateLatticeTypeExamples();
   ~GenerateLatticeTypeExamples() {}
};

class IT_3 : public GenerateNiggliBase {
public:
   IT_3();
};


#endif // GENERATELATTICETYPEEXAMPLES_H
