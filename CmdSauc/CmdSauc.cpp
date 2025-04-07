//CmdSauc.cpp

/* Program to process a list is lattices through Sauc, the search for alternate
   unit cells, taking each lattice in the form centering, a, b, c, alpha, beta, gamma
   and making it into a call to the sauc cgi-bin script with arguments:

     Centering -- the lattice centering a, A, b, B, c, C, i, I, f, F, p, P, h, H, r, or R
     RangeA, RangeB, RangeC, RangeAlpha, RangeBeta, RangeGamma -- ranges in Angstroms and Degrees
     RangeSphere -- the size of a search sphere for similarity 2
     Algorithm -- 1 through 7 for L1, L2, NCDist, V7, D7, S6, or DC7unsrt (default)
     Similarity -- 1 through 3 for Nearest, Sphere, or Range
     SortbyFam -- yes to sort by family
     UsePercent -- yes to use RangeSphere as a percentage instead of as Angstroms

*/

#include <iostream>

#include "CmdSaucControls.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_Cell_Degrees.h"
#include "ProgramSetup.h"
#include "WebIO.h"

void print_sauc(const std::string& lattice, const LRL_Cell_Degrees& mycell, const CmdSaucControls& controls) {

   const int similarity = controls.getsimilarity();
   const int algorithm = controls.getalgorithm();
   const double RangeA = controls.getRangeA();
   const double RangeB = controls.getRangeB();
   const double RangeC = controls.getRangeC();
   const double RangeAlpha = controls.getRangeAlpha();
   const double RangeBeta = controls.getRangeBeta();
   const double RangeGamma = controls.getRangeGamma();
   const double saucSphereRange = controls.getsaucSphereRange();

   std::cout
      << ";<a href=\"http://blondie.arcib.org:8083/~yaya/cgi-bin/sauc-1.2.1.csh?OutputStyle=TEXT&"
      << "Centering=" << lattice
      << "&Algorithm=" << algorithm
      << "&A=" << mycell[0]
      << "&B=" << mycell[1]
      << "&C=" << mycell[2]
      << "&Alpha=" << mycell[3]
      << "&Beta=" << mycell[4]
      << "&Gamma=" << mycell[5]
      << "&NumHits=" << 50
      << "&Similarity=" << similarity
      << "&RangeSphere=" << saucSphereRange
      << "&UsePercent=" << "yes"
      << "&SortbyFam=" << "yes"
      << "&RangeA=" << RangeA
      << "&RangeAlpha=" << RangeAlpha
      << "&RangeB=" << RangeB
      << "&RangeBeta=" << RangeBeta
      << "&RangeC=" << RangeC
      << "&RangeGamma=" << RangeGamma
      << "&Flush=" << "DUMMY"
      << "\" target=\"_blank\""
      << " > sauc search </a>"
      << std::endl;

}

int main(int argc, char* argv[]) {
   std::cout << "; Sauc" << std::endl;
   WebIO webio(argc, argv, "CmdSauc", 0);

   try {
      CmdSaucControls controls;
      const BasicProgramInput<CmdSaucControls> dc_setup("CmdSauc", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      const std::string latticeTypes("aAbBcCiIfFpPhHrR");

      if (controls.getShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {
         const size_t n = latticeTypes.find(input.getLatticeType());

         std::string lattice;
         if (n == std::string::npos) lattice = "P";
         else lattice = std::string(input.getLatticeType());

         const LRL_Cell_Degrees mycell = LRL_Cell_Degrees(input.getCell());

         std::cout << lattice << " " << mycell << std::endl;
         print_sauc(lattice, mycell, controls);
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}



