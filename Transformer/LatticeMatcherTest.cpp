//#include <iostream>
//#include <string>
//#include "LatticeMatcher.h"
//#include "LatticeCell.h"
//#include "LRL_Cell.h"
//#include "ProgramSetup.h"
//
//int main(int argc, char* argv[]) {
//
//   std::cout << "Lattice Matcher Test" << std::endl;
//   std::cout << "===================" << std::endl;
//
//   try {
//      // Create WebIO object for input handling
//      WebIO webio(argc, argv, "LatticeMatcher", 0);
//
//      // Create cells for testing
//      LRL_Cell sourceCell(10.0, 10.0, 10.0, 90.0, 90.0, 90.0);
//      LRL_Cell targetCell(10.0, 10.01, 10.02, 90.0, 90., 120.0);
//
//      LatticeCell sourceLatCell(sourceCell);
//      LatticeCell targetLatCell(targetCell);
//
//      // Display input cells
//      std::cout << "\nSource Cell: Cubic" << std::endl;
//      std::cout << "a = b = c = 10.0, α = β = γ = 90°" << std::endl;
//
//      std::cout << "\nTarget Cell: Hexagonal" << std::endl;
//      std::cout << "a = b = 10.0, c = 10.0, α = β = 90°, γ = 120°" << std::endl;
//
//      // Create transformer using the type alias for convenience
//      LatticeMatcherDefault matcher(sourceLatCell, targetLatCell);
//
//      // Find best transformation
//      std::cout << "\nFinding transformation..." << std::endl;
//      LatticeMatcherDefault::TransformationResult result = matcher.findBestTransformation();
//
//      // Display result
//      std::cout << "\n" << matcher.formatTransformation(result) << std::endl;
//
//      return 0;
//   }
//   catch (const std::exception& e) {
//      std::cerr << "Error: " << e.what() << std::endl;
//      return 1;
//   }
//}
