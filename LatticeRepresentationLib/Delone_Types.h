#ifndef DELONE_TYPES_H
#define DELONE_TYPES_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "MatricesForOneDeloneType.h"
#include "MatS6.h"


class Delone_Type {
public:
   friend std::ostream& operator<< (std::ostream&, const Delone_Type&);

   Delone_Type() {}
   //Delone_Type(const std::string& deloneType, const std::string& bravaisType, const std::string& character, const std::pair<std::string, MatS6 >&);
   Delone_Type(
      const std::string& deloneType,
      const std::string& bravaisType,
      const std::string& character,
      const std::string& toCentered_E3,
      const MatS6& ToCentered,
      const int freeParams);

   //DeloneFitResults GetFit(const S6& s6) const;

   std::string GetName(void) const { return m_deloneName; }
   std::string GetBravaisType(void) const { return m_bravaisType; }
   std::string GetCharacter(void) const { return m_character; }
   std::string GetToCenteredString(void) const { return m_toCentered_E3; }
   MatS6 GetToCentered(void) const { return m_toCentered_S6; }
   MatricesForOneDeloneType GetFamilyOfMattrices() const { return m_matrices; }
   std::vector<MatS6> GetPrjs(void) const { return m_matrices.GetPrjs(); }
   std::vector<MatS6> GetPerps(void) const { return m_matrices.GetPerps(); }

private:
   std::string m_deloneName;
   std::string m_bravaisType;
   std::string m_character;
   std::string m_toCentered_E3;
   MatS6       m_toCentered_S6;
   int m_freePsrams;

   MatricesForOneDeloneType m_matrices;


};

class Delone_Type_C1 : public Delone_Type{
   Delone_Type_C1();
};

#endif // DELONE_TYPES_H
