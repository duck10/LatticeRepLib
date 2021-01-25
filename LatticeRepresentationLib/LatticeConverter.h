#ifndef LATTICECONVERTER_H
#define LATTICECONVERTER_H

#include <string>

#include "LRL_Cell.h"
#include "MatD7.h"

#include <vector>

class LatticeConverter {
   enum eOutputType { etext, emaxima };
private:
   eOutputType m_OutputType;

   void TextOutput  (const std::string& label, const std::string& lattice, const LRL_Cell& cell) const;
   void MaximaOutput(const std::string& label, const std::string& lattice, const LRL_Cell& cell) const;

public:
   LatticeConverter(const eOutputType type = etext);

   void SetOutputMaxima(void);
   void SetOutputText(void);
   void Output(const std::string& label, const std::string& lattice, const LRL_Cell& cell) const;
   static LRL_Cell NiggliReduceCell( const std::string& lattice, const LRL_Cell& cell );
   void NiggliReducedOutput( const std::string& label, const std::string& lattice, const LRL_Cell& cell );
   static LRL_Cell DeloneReduceCell( const std::string& lattice, const LRL_Cell& cell );
   static LRL_Cell SellingReduceCell( const std::string& lattice, const LRL_Cell& cell );
   void DeloneReducedOutput(const std::string& label, const std::string& lattice, const LRL_Cell& cell);
   void SellingReducedOutput(const std::string& label, const std::string& lattice, const LRL_Cell& cell);
   static LRL_Cell MakePrimitiveCell( const std::string& lattice, const LRL_Cell& cell );

   static LRL_Cell NiggliReduceCell ( const std::string& lattice, const LRL_Cell& cell, MatG6& mat );
   static LRL_Cell DeloneReduceCell ( const std::string& lattice, const LRL_Cell& cell, MatD7& mat );
   static LRL_Cell SellingReduceCell( const std::string& lattice, const LRL_Cell& cell, MatS6& mat );
   static LRL_Cell MakePrimitiveCell( const std::string& lattice, const LRL_Cell& cell, MatG6& mat );
   static LRL_Cell MakePrimitiveCell( const std::string& lattice, const LRL_Cell& cell, MatD7& mat );
   static LRL_Cell MakePrimitiveCell( const std::string& lattice, const LRL_Cell& cell, MatS6& mat );
};
#endif //LATTICECONVERTER_H
