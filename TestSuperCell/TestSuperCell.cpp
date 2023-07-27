// TestSuperCell.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include "G6.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ToString.h"
#include "LRL_MaximaTools.h"
#include "MatG6.h"
#include "Niggli.h"

int main()
{
   std::vector<MatG6> v;
   MatG6 m1("1 0 0 0 0 0  0 1 0 0 0 0  0 0 4 0 0 0  0 0 0 2 0 0  0 0 0 0 2 0  0 0 0 0 0 1");
   MatG6 m2("1 0 0 0 0 0  0 1 1 1 0 0  0 0 4 0 0 0  0 0 4 2 0 0  0 0 0 0 2 0  0 0 0 0 1 1");
   MatG6 m3("1 0 1 0 1 0  0 1 0 0 0 0  0 0 4 0 0 0  0 0 0 2 0 0  0 0 4 0 2 0  0 0 0 1 0 1");
   MatG6 m4("1 0 1 0 1 0  0 1 1 1 0 0  0 0 4 0 0 0  0 0 4 2 0 0  0 0 4 0 2 0  0 0 2 1 1 1");
   MatG6 m5("1 0 0 0 0 0  0 4 0 0 0 0  0 0 1 0 0 0  0 0 0 2 0 0  0 0 0 0 1 0  0 0 0 0 0 2");
   MatG6 m6("1 1 0 0 0 1  0 4 0 0 0 0  0 0 1 0 0 0  0 0 0 2 0 0  0 0 0 1 1 0  0 4 0 0 0 2");
   MatG6 m7("4 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 2 0  0 0 0 0 0 2");
   v.push_back(m1);
   v.push_back(m2);
   v.push_back(m3);
   v.push_back(m4);
   v.push_back(m5);
   v.push_back(m6);
   v.push_back(m7);

   const LRL_Cell cell("10 13 17 65 75 86");
   std::cout << " input  " << LRL_Cell_Degrees(cell) << "   V= " << cell.Volume() << std::endl;
   G6 out;
   Niggli::Reduce(G6(cell), out);
   std::cout << LRL_Cell_Degrees(LRL_Cell(out)) << "   V= " << LRL_Cell(out).Volume() << "   reduced" << std::endl << std::endl << std::endl << std::endl;
   
   
   for (size_t i = 0; i < v.size(); ++i) {
      std::cout << "cell # " << i << std::endl;
      const LRL_Cell cell2 = LRL_Cell(v[i] * G6(cell));
      std::cout << LRL_Cell_Degrees(cell2) << "   V= " << cell2.Volume() << std::endl;
      G6 out;
      Niggli::Reduce(v[i] * G6(cell), out);
      std::cout << LRL_Cell_Degrees(LRL_Cell(out)) << "   V= " << LRL_Cell(out).Volume() << "  Niggli  reduced" << std::endl << std::endl;
   }

   std::cout << std::endl << std::endl;

   for (size_t i = 0; i < v.size(); ++i) {
      std::cout << i << "   " << LRL_MaximaTools::MaximaFromMat(v[i]) << std::endl;;
   }

    return 0;
}

