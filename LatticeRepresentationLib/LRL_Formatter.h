#ifndef LRL_FORMATTER_H
#define LRL_FORMATTER_H

#include <string>
#include <sstream>
#include <vector>
#include "LatticeCell.h"
#include "S6.h"
#include "G6.h"
#include "D7.h"
#include "Path.h"

// Base formatter interface
class FormatterBase {
public:
   virtual ~FormatterBase() = default;

   // Format a lattice cell based on the specific representation
   virtual std::string formatLattice(const LatticeCell& cell) const = 0;

   // Format a path (implementation in cpp file)
   std::string formatPath(const Path& path) const;
};

// Template formatter for any lattice type
template<typename LatticeType>
class LatticeFormatter : public FormatterBase {
public:
   std::string formatLattice(const LatticeCell& cell) const override {
      const LatticeType lattice = cell.getCell();
      std::stringstream ss;

      ss << cell.getLatticeType() << " ";

      // Use the size() method to format the correct number of elements
      for (size_t i = 0; i < LatticeType::size(); ++i) {
         ss << lattice[i];
         if (i < LatticeType::size() - 1) {
            ss << " ";
         }
      }

      return ss.str();
   }
};

// Type aliases for specific lattice formatters
using G6Formatter = LatticeFormatter<G6>;
using S6Formatter = LatticeFormatter<S6>;
using D7Formatter = LatticeFormatter<D7>;

// Output manager that works with any formatter
template<typename Formatter>
class LRL_Output {
private:
   Formatter formatter;
   std::ostream& out;

public:
   explicit LRL_Output(std::ostream& stream = std::cout)
      : out(stream) {}

   // Write a comment line
   void writeComment(const std::string& comment) {
      out << "; " << comment << std::endl;
   }

   // Format a lattice to string without writing
   std::string formatLattice(const LatticeCell& cell) const {
      return formatter.formatLattice(cell);
   }

   // Write a lattice cell normally
   void writeLattice(const LatticeCell& cell) {
      out << formatLattice(cell) << std::endl;
   }

   // Write a group of cells
   void writeCells(const std::vector<LatticeCell>& cells) {
      for (const auto& cell : cells) {
         writeLattice(cell);
      }
   }

   // Write a path - simplified to use formatter's formatPath
   void writePath(const Path& path) {
      writeComment("the path");
      out << formatter.formatPath(path);
   }

   // Get the underlying stream
   std::ostream& getStream() {
      return out;
   }
};

#endif // LRL_FORMATTER_H

