#ifndef LATTICECONVERTER_H
#define LATTICECONVERERT_H

#include <string>

#include "Cell.h"

class LatticeConverter {
   enum eOutputType { etext, emaxima };
private:
   eOutputType m_OutputType;

   void TextOutput(const std::string& label, const std::string& lattice, const Cell& cell) const;
   void MaximaOutput(const std::string& label, const std::string& lattice, const Cell& cell) const;

public:
   LatticeConverter(const eOutputType type = etext);

   void SetOutputMaxima(void);
   void SetOutputText(void);
   void Output(const std::string& label, const std::string& lattice, const Cell& cell) const;
   Cell NiggliReduceCell(const std::string& lattice, const Cell& cell);
   void NiggliReducedOutput(const std::string& label, const std::string& lattice, const Cell& cell);
   Cell DeloneReduceCell(const std::string& lattice, const Cell& cell);
   void DeloneReducedOutput(const std::string& label, const std::string& lattice, const Cell& cell);
};
#endif //LATTICECONVERTER_H
