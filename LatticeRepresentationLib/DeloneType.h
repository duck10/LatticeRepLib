#ifndef DELONETYPE_H
#define DELONETYPE_H

#include <string>
#include <vector>

#include "MatS6.h"
#include "LabeledSellaMatrices.h"
#include "LabeledSellaMatricesForDeloneType.h"

class DeloneType {
public:
	DeloneType(){}
	DeloneType(const std::string& deloneType, const std::string& bravaisType, const std::string& character, const std::pair<std::string, MatS6 >& );
   void InsertMatricesForDeloneType(const std::string& s);
   void AddToCenteredE3();
   void AddToCenteredS6();
   void AddToLabeledSellaMatricesForDeloneType(
      const std::string& label,
      const LabeledSellaMatrices& prjs,
      const LabeledSellaMatrices& perps,
      const LabeledSellaMatrices& toCanons,
      const LabeledSellaMatrices& toCentereds);



private:
	std::string m_deloneName;
	std::string m_bravaisType;
	std::string m_character;
	std::string m_toCentered_E3;
	MatS6       m_toCentered_S6;
	LabeledSellaMatricesForDeloneType m_matrices;



};


#endif  // DELONETYPE_H