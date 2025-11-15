#include <iostream>

#include "GenerateCenteredCells.h"
#include "ProgramSetup.h"

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LatticeConverter.h"
#include "LatticeCell.h"
#include "LRL_inverse.h"

#include <map>
#include <vector>

static MatG6 Inverse(const MatG6& min) {
   MatG6 m(min);
   std::vector<double> arout(36);
   inverse(6, min.GetVector().data(), arout.data());
   m.SetVector(arout);
   return m;
}


std::map<std::string, MatG6> primitiveToCentered = {
    {"P", MatG6::Eye()},
    {"I", MatG6("1 0 0 0 0 0    0 1 0 0 0 0    0 0 1 0 0 0    0 1 0 0 0 0    1 0 0 0 0 0    0 0 0 0 0 1")}, // placeholder: needs proper I-centering
    {"A", MatG6("1 0 0 0 0 0    0 1 0 0 0 0    0 0 1 0 0 0    0 1 0 0 0 0    0 0 0 0 1 0    0 0 0 0 0 1")}, // placeholder
    {"B", MatG6("1 0 0 0 0 0    0 1 0 0 0 0    0 0 1 0 0 0    0 0 0 0 0 1    1 0 0 0 0 0    0 0 0 0 1 0")}, // placeholder
    {"C", MatG6("1 0 0 0 0 0    0 1 0 0 0 0    0 0 1 0 0 0    0 0 0 0 1 0    0 0 0 0 0 1    1 0 0 0 0 0")}, // placeholder
    {"F", MatG6("0.5 0.5 0 0 0 0    0.5 0 0.5 0 0 0    0 0.5 0.5 0 0 0    0 0 0 0.5 0.5 0    0 0 0 0 0.5 0.5    0 0 0 0.5 0 0.5")}, // verified for face-centering
    {"R", MatG6("1 1 1 1 -1 -1    4 1 1 1 2 2    1 4 1 -2 -1 2    -4 -4 2 -1 1 -5    2 -4 2 -1 -2 1    -4 2 2 2 1 1") * (1.0 / 9.0)} // rhombohedral as hex
};



std::vector<LatticeCell> ConvertPrimitiveToCentered(const G6& g6_primitive) {
   std::vector<LatticeCell> result;
   const std::string centerings = "IABCFR";

   result.emplace_back(g6_primitive, "P"); // include primitive

   for (char sym : centerings) {
      MatG6 M = LRL_Cell::G6MakePrimitiveMatrix(std::string(1, sym));
      const MatG6 Minv = Inverse(M);
      const LRL_Cell inv = Minv * g6_primitive;
      result.emplace_back(inv, std::string(1, sym));
   }

   return result;
}


int main() {
   std::cout << "; GENERATE_CENTERED_CELLS" << std::endl;

   try {
      GenerateCenteredCellsControls controls;
      const BasicProgramInput<GenerateCenteredCellsControls> dc_setup("TEMPLATE", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {

         const std::string lattice(input.getLatticeType());
         const LRL_Cell primitive = LatticeConverter::MakePrimitiveCell(lattice, input.getCell());

         const std::vector<LatticeCell> vout = ConvertPrimitiveToCentered(primitive);




         for (const auto& v : vout) {
            std::cout << v.getLatticeType() << " " << LRL_Cell_Degrees(v.getCell()) << std::endl;
         }
      }
      return 0;
   }
   catch (const std::exception& e) {
      std::cout << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}
