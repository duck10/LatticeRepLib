#include "LatticeCharacters.h"

LatticeCharacters::LatticeCharacters(const std::string& deloneType, 
	const std::string& bravaisType, const std::string& character)
	: m_deloneName(deloneType)
	, m_bravaisType(bravaisType)
	, m_character(character)
	, m_toCentered_E3()
	, m_toCentered_S6()
	, m_matrices()
{
}

void LatticeCharacters::AddToLabeledSellaMatricesForDeloneType(const LabeledSellaMatrices& lsm) {
   m_matrices.push_back(lsm);
}
