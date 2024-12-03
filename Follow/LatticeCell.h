#ifndef LATTICECELL_H
#define LATTICECELL_H

#include <string>

#include "G6.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"


class LatticeCell {
private:
   G6 cell;
   std::string latticeType;  // "P", "A", "B", "C", "F", "I", "H"

public:
   LatticeCell(const G6& c, const std::string& type = "P")
      : cell(c), latticeType(type) {}

   G6 getNiggliReducedCell() const {
      G6 primitive = toPrimitive();
      // Apply your existing Niggli reduction
      return primitive;  // Should be Niggli reduced
   }

   const G6& getCell() const { return cell; }
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

   operator S6() const {
      return (this->getCell());
   }


   operator S6() {

   }

};

#endif // LATTICECELL_H