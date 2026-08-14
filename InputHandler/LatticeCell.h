#ifndef LATTICECELL_H
#define LATTICECELL_H

#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "G6.h"
#include "C3.h"
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
   {
   }

   LatticeCell(const std::string& type, const G6& c, const std::string& inputReadIn = "")
      : cell(c)
      , latticeType(type)
      , inputtext(inputReadIn)
   {
   }

   // Single-string constructor -- accepts either
   //   "P 10 10 10 90 90 90"   (leading lattice designator)
   // or
   //   "10 10 10 90 90 90"     (no designator -- P is assumed)
   // Six cell parameters (a, b, c, alpha, beta, gamma) are expected either
   // way; LRL_Cell's own string constructor does the actual numeric parse.
   LatticeCell(const std::string& s)
      : inputtext(s)
   {
      static const std::set<std::string> designators = {
         "P", "A", "B", "C", "F", "I", "H"
      };

      std::istringstream iss(s);
      std::vector<std::string> tokens;
      std::string tok;
      while (iss >> tok) tokens.push_back(tok);

      latticeType = "P";
      size_t startIdx = 0;
      if (!tokens.empty() && designators.count(tokens[0])) {
         latticeType = tokens[0];
         startIdx = 1;
      }

      std::string cellTokens;
      for (size_t i = startIdx; i < tokens.size(); ++i) {
         cellTokens += tokens[i];
         if (i + 1 < tokens.size()) cellTokens += " ";
      }
      cell = LRL_Cell(cellTokens);
   }

   LatticeCell(const B4& b4) {
      cell = LRL_Cell(b4);
      latticeType = "P";
   }

   friend std::ostream& operator<< (std::ostream& o, const LatticeCell& c) {
      o << c.getLatticeType() << " ";
      o << c.getCell() << std::endl;
      return o;
   }

   G6 getNiggliReducedCell() const {
      G6 primitive = toPrimitive();
      G6  out;
      Niggli::Reduce(primitive, out);
      return out;  // Should be Niggli reduced
   }

   const LRL_Cell getCell() const {
      return LRL_Cell(cell);
   }

   const LRL_Cell GetCell() const {
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

   static size_t size() { return 6; }

   std::string GetInput() const {
      return inputtext;
   }

   std::string GetInputLine() const {
      return inputtext;
   }

   std::string getInput() const {
      return inputtext;
   }

   std::string getInputLine() const {
      return inputtext;
   }

   operator S6() const {
      return (this->getCell());
   }


   operator S6() {
      return (this->getCell());

   }

   bool operator==(const LatticeCell& c) const {
      const LatticeCell& a(*this);
      return (a.getCell() == c.getCell() && (a.getLatticeType() == c.getLatticeType()));
   }

};

#endif // LATTICECELL_H
