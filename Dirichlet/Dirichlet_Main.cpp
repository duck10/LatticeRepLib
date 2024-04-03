
#include <algorithm>
#include <cmath>
#include <iostream>     // std::cout, std::endl
#include <iomanip>      // std::setfill, std::setw
#include <string>
#include <utility>
#include <vector>

#include "Dirichlet.h"
#include "DirichletConstants.h"
#include "DirichletCellFaces.h"
#include "FileOperations.h"
#include "LRL_ReadLatticeData.h"
#include "ReadDirichletGlobalData.h"
#include "WebIO.h"

int main(int argc, char* argv[]) {
   LRL_ReadLatticeData reader;
   std::cout << "; Dirichlet (Voronoi) cells" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = reader.ReadLatticeData();
   std::cout << "; count of input cells " << inputList.size() << std::endl;

   WebIO webio(argc, argv, "Dirichlet", inputList.size());
   webio.GetWebBlockSize(argc, argv);
   webio.CreateFilenamesAndLinks(inputList.size(), "DC");

   const size_t& blockstart= webio.m_blockstart;
   const size_t& blocksize= webio.m_blocksize;

   const std::vector<std::string>& basicfileNameList = webio.m_basicfileNameList;
   const std::vector<std::string>& RawFileNameList = webio.m_FileNameList;
   const std::vector<std::string>& FullfileNameList = webio.m_FullfileNameList;

   std::cout << "; Dirichlet cell block start " << blockstart << std::endl;
   std::cout << "; Dirichlet cell block size " << blocksize << std::endl;

   for (size_t i = blockstart; i < (inputList.size()) && (i < blockstart + blocksize); ++i)
   {
      std::cout << "; Dirichlet graphics file(s) " <<
         i+1 << "  " << FullfileNameList[i - blockstart] << std::endl;
   }

   for (size_t whichCell = blockstart; 
       whichCell < inputList.size() && whichCell < blockstart+blocksize; ++whichCell) {
       const DirichletCell dc = DirichletCell(inputList[whichCell]);
       std::cout << inputList[whichCell].GetStrCell() << std::endl;
       Dirichlet::ListVertices(dc);
       const std::string svg = Dirichlet::HandleOneCell(dc);
       const std::string fileName = RawFileNameList[whichCell-blockstart];
       const std::string fullfileName = FullfileNameList[whichCell-blockstart];
       if (!svg.empty())
       {
           FileOperations::Write(fileName, svg);
           std::cout << "; Dirichlet graphics file " << fullfileName << std::endl;
       }
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
