
#include <algorithm>
#include <cmath>
#include <iostream>     // std::cout, std::endl
#include <iomanip>      // std::setfill, std::setw
#include <string>
#include <utility>
#include <vector>

#include "Dirichlet.h"
#include "DirichletControls.h"
#include "DirichletProgramConstants.h"
#include "DirichletCellFaces.h"
#include "FileOperations.h"
#include "InputHandler.h"
#include "LatticeCell.h"
#include "ProgramSetup.h"
#include "WebIO.h"

int main(int argc, char* argv[]) {
   std::cout << "; Dirichlet (Voronoi) cells" << std::endl;

   WebIO webio(argc, argv, "Dirichlet", 0);

   DirichletControls controls;
   controls.setHasWebInput(webio.m_hasWebInstructions);
   const int initblockstart = 0;
   const int initblocksize = 20;
   const FileBlockProgramInput<DirichletControls> dc_setup("Dirichlet", initblockstart, initblocksize, controls);

   const size_t blockstart = dc_setup.getBlockStart();
   const size_t blocksize = dc_setup.getBlockSize();
   const size_t blockend = dc_setup.getBlockEnd();

   const std::vector<LatticeCell>& inputList = dc_setup.getInputList();

   if (controls.getShowControls()) {
      std::cout << controls << std::endl;
   }

   for (size_t whichCell = blockstart;
      whichCell < inputList.size() && whichCell < blockstart + blocksize; ++whichCell) {
      const std::string svg = Dirichlet::HandleOneCell(DirichletCell(inputList[whichCell]));
      dc_setup.writeOutputFile(svg, whichCell);
   }

   exit(0);
}

// 24 vertices in a Tetrakaidecahedron. It's a 14-sided polyhedron

/*
Bard.google.com says: (which is wrong for cubic)
Truncated Octahedron (Simple Cubic and FCC Lattices): 24 vertices
Rhombic Dodecahedron (BCC Lattice): 14 vertices
Hexagonal Prism (HCP Lattice): 14 vertices
Tetrakaidecahedron (Kelvin Cell, Diamond Lattice): 24 vertices

wikipedia
FCC The truncated octahedron has 14 faces (8 regular hexagons and 6 squares), 36 edges, and 24 vertices. 

the rhombic dodecahedron is a convex polyhedron with 12 congruent rhombic faces. It has 24 edges, and 14 vertices of 2 types.

Hexagonal Prism 12 vertices

i got 14 for FCC
i got 8 for hexagonal prism
i got 14 for random
i got 6 for P cubic
*/
