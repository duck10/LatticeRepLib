#ifndef LATTICECELL_H
#define LATTICECELL_H

#include <string>

#include "G6.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "Niggli.h"


class LatticeCell {
private:
   G6 cell;

   std::string latticeType;  // "P", "A", "B", "C", "F", "I", "H"
   std::string inputtext;

public:
   LatticeCell(const G6& c, const std::string& type = "P", const std::string& inputReadIn = "")
      : cell(c)
      , latticeType(type)
   , inputtext(inputReadIn)
   {}

   G6 getNiggliReducedCell() const {
      G6 primitive = toPrimitive();
      G6  out;
      Niggli::Reduce(primitive, out);
      return out;  // Should be Niggli reduced
   }

   const LRL_Cell getCell() const {
      return LRL_Cell(cell); 
   }

   std::string getLatticeType() const { return latticeType; }


   G6 toPrimitive() const {
      if (latticeType == "P")
      {
         return cell;
      }
      else
      {
         const LRL_Cell cellin(this->getCell());
         LRL_Cell pcell = LatticeConverter::MakePrimitiveCell(latticeType, cellin);
         return pcell;
      }
   }

   std::string GetInput() const {
      return inputtext;
   }

   operator S6() const {
      return (this->getCell());
   }


   operator S6() {
      return (this->getCell());

   }

};

#endif // LATTICECELL_H