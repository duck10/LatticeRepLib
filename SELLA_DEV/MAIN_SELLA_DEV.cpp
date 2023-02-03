// SELLA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <set>
#include <string>

//#include "LabeledSellaMatrices.h"
//#include "DeloneTypeList.h"
//#include "LatticeConverter.h"
//#include "LRL_CoordinateConversionMatrices.h"
//#include "LRL_ReadLatticeData.h"
//#include "LRL_StringTools.h"
//#include "LRL_ToString.h"
//#include "LRL_Vector3.h"
//#include "MatG6.h"
//#include "rhrand.h"
//#include "S6.h"
#include "SellaBuild.h"
//#include "Selling.h"
#include "S6BoundaryTransforms.h"


int main()
{
   const S6 s6(-100, -100, -100, 0, 0, 0);
   //SellaBuild sb;
   //const  std::set<S6> setS6 = sb.Xpand3("xxx",s6, MatS6());
   //std::cout << std::endl;
   //for (auto it = setS6.begin(); it != setS6.end(); ++it) std::cout << *it << std::endl;
   //std::cout << "setS6 size  " << setS6.size() << std::endl;
   //S6BoundaryMatricesZero smz;
   //const std::vector<MatS6> vz = smz.GetVector();

   //S6BoundaryMatricesOne smo(0);
   //const std::vector<MatS6> vo = smo.GetVector();

   ////for (size_t i = 0; i < vo.size(); ++i)
   ////{
   ////   std::cout << vo[i] << std::endl << std::endl;
   ////}

   //S6BoundaryMatricesTwo smtwo(2, 3);
   //const std::vector<MatS6> vt23 = smtwo.GetVector();

   //S6BoundaryMatricesThree smthr(2, 3, 5);
   //const std::vector<MatS6> vt235 = smthr.GetVector();
   //
   //const S6 s("1 2 3 4 5 6");
   //for ( size_t i = 0; i < vt235.size(); ++i)
   //{
   //   std::cout << vt235[i] * s << std::endl;
   //}
   //exit(0);

   SellaBuild().Build();
   exit(0);

}
