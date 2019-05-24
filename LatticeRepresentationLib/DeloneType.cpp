#include "DeloneType.h"

DeloneType::DeloneType(const std::string& deloneType, 
	const std::string& bravaisType, const std::string& character, const std::pair<std::string, MatS6 >& fromCanon)
	: m_deloneName(deloneType)
	, m_bravaisType(bravaisType)
	, m_character(character)
	, m_toCentered_E3()
	, m_toCentered_S6(fromCanon.second)
{
}

void DeloneType::InsertMatricesForDeloneType(const std::string& s) {
   //(*this).m_prjs = GetPrj(s);
   //(*this).m_perps = GetPerp(s);
   //(*this).m_toCanons = GetTocanon(s);
}
