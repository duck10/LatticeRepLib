#ifndef DELONETYPE_H
#define DELONETYPE_H

#include <string>
#include <vector>

#include "MatS6.h"
#include "LabeledSellaMatrices.h"
#include "LabeledDeloneTypeMatrices.h"

class DeloneType {
public:
	DeloneType(){}
	DeloneType(const std::string& deloneType, const std::string& bravaisType, const std::string& character, const std::pair<std::string, MatS6 >& );
   void InsertMatricesForDeloneType(const std::string& s);
   void AddToCenteredE3();
   void AddToCenteredS6();

   std::string GetName(void) const { return m_deloneName; }
   std::string GetBravaisType(void) const { return m_bravaisType;}
   std::string GetCharacter(void) const  { return m_character; }
   std::string GetToCenteredString(void) const  { return m_toCentered_E3;}
   MatS6 GetToCentered(void) const { return m_toCentered_S6; }

   MatS6 GetPrj(const unsigned long n) const { return m_prjs[n]; }
   MatS6 GetPerp(const unsigned long n) const { return m_perps[n]; }
   MatS6 GetToCanon(const unsigned long n) const { return m_toCanons[n]; }



private:
	std::string m_deloneName;
	std::string m_bravaisType;
	std::string m_character;
	std::string m_toCentered_E3;
	MatS6       m_toCentered_S6;

   std::vector<MatS6> m_prjs;
   std::vector<MatS6> m_perps;
   std::vector<MatS6> m_toCanons;


};


#endif  // DELONETYPE_H