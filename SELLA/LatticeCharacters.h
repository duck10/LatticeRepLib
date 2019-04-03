#ifndef LATTICECHARACTERS_H
#define LATTICECHARACTERS_H

#include <string>
#include <vector>

#include "MatS6.h"
#include "LabeledSellaMatrices.h"
#include "LabeledSellaMatricesForDeloneType.h"

class LatticeCharacters {
public:
	LatticeCharacters(){}
	LatticeCharacters(const std::string& deloneType, const std::string& bravaisType, const std::string& character);

   void AddToCenteredE3();
   void AddToCenteredS6();
   void AddToLabeledSellaMatricesForDeloneType( const LabeledSellaMatrices& lsm);



private:
	std::string m_deloneName;
	std::string m_bravaisType;
	std::string m_character;
	std::string m_toCentered_E3;
	MatS6       m_toCentered_S6;
	LabeledSellaMatricesForDeloneType m_matrices;



};


#endif  // LATTICECHARACTERS_H