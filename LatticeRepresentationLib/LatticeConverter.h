#ifndef LATTICECONVERTER_H
#define LATTICECONVERTER_H
#include <string>
#include "LRL_Cell.h"
#include <vector>

class MatG6;
class MatS6;

class LatticeConverter {
   enum eOutputType { etext, emaxima };
private:
   eOutputType m_OutputType;
public:
   LatticeConverter(const eOutputType type = etext);
   void SetOutputMaxima(void);
   void SetOutputText(void);
   static LRL_Cell NiggliReduceCell(const std::string& lattice, const LRL_Cell& cell);
   static LRL_Cell DeloneReduceCell(const std::string& lattice, const LRL_Cell& cell);
   static LRL_Cell SellingReduceCell(const std::string& lattice, const LRL_Cell& cell);
   static LRL_Cell MakePrimitiveCell(const std::string& lattice, const LRL_Cell& cell);
   static LRL_Cell NiggliReduceCell(const std::string& lattice, const LRL_Cell& cell, MatG6& mat);
   static LRL_Cell SellingReduceCell(const std::string& lattice, const LRL_Cell& cell, MatS6& mat);
};
#endif //LATTICECONVERTER_H
