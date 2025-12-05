// Example usage of IsNiggli_KG.h

//#include "IsNiggli_KG.h"
#include "KrivyGruber.h"
#include "G6.h"
#include "IsNiggli_KG.h"
#include "LRL_Cell_Degrees.h"
#include <iostream>
#include "G6_Validity_Checker.h"

int main() {
   G6_Validity::RunTests(std::cout);

   // Test case that fails both algorithms
   const G6 testCell(7.560, 11.560, 11.560, 11.560, 2.890, 5.780);  // KG result
   const G6 testCell2(7.560, 11.560, 11.560, 11.560, 5.780, 2.890); // AB result 
   const double tolerance = 0.0;
    
   std::cout << "Testing KG reduced cell:" << std::endl;
   std::cout << "G6: " << testCell << std::endl;
   std::cout << "Cell: " << LRL_Cell_Degrees(testCell) << std::endl;
   std::cout << "tolerance " << tolerance << std::endl;
   
   std::vector<NiggliViolation> violations;
   bool isNiggli = IsNiggli_KG::Check(testCell, violations, tolerance);
   
   std::cout << "\nIs Niggli reduced? " << (isNiggli ? "YES" : "NO") << std::endl;
   IsNiggli_KG::PrintViolations(violations);
   
   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "Testing AB reduced cell:" << std::endl;
   std::cout << "G6: " << testCell2 << std::endl;
   std::cout << "Cell: " << LRL_Cell_Degrees(testCell2) << std::endl;
   
   violations.clear();
   isNiggli = IsNiggli_KG::Check(testCell2, violations, tolerance);
   
   std::cout << "\nIs Niggli reduced? " << (isNiggli ? "YES" : "NO") << std::endl;
   IsNiggli_KG::PrintViolations(violations);
   
   return 0;
}
