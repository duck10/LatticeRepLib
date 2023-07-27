#include "D7.h"
#include "D7Dist.h"
//#include "D7Dist_.hpp"
#include "LRL_MinMaxTools.h"
#include "Triangle.h"

#include <algorithm>

//#include "stdafx.h"

Triangle::Triangle(const std::vector<LRL_ReadLatticeData>& cells) 
   : m_triangles( cells )
{
}

size_t Triangle::Test(void)
{
   std::cout << "Triangle violations, followed by the edge lengths (negative is GOOD)" << std::endl;
   size_t violations = 0;
   if (m_triangles.size() < 3) return 0;
   for (size_t i0 = 0; i0 < m_triangles.size(); ++i0)
      violations = Test(m_triangles[i0]);
   return violations;
}

size_t Triangle::Test(const LRL_ReadLatticeData& rcd1)
{
   size_t violations = 0;
   for (size_t i1 = 0; i1 < m_triangles.size(); ++i1)
      violations = Test(rcd1, m_triangles[i1]);
   return violations;
}

size_t Triangle::Test(const LRL_ReadLatticeData& rcd1, const LRL_ReadLatticeData& rcd2)
{
   size_t violations = 0;
   for (size_t i2 = 0; i2 < m_triangles.size(); ++i2) {
      const bool isViolated = TestTriangle(rcd1, rcd2, m_triangles[i2]);
      violations += isViolated ? 1 : 0;
   }
   return violations;
}

bool Triangle::TestTriangle(const LRL_ReadLatticeData & rcd1, const LRL_ReadLatticeData & rcd2, const LRL_ReadLatticeData & rcd3)
{
   const D7 d1 = rcd1.GetCell();
   const D7 d2 = rcd2.GetCell();
   const D7 d3 = rcd3.GetCell();

   double* a1 = d1.data();
   double* a2 = d2.data();
   double* a3 = d3.data();
   const double dists0 = D7Dist(a1,a2);
   const double dists1 = D7Dist(a1,a3);
   const double dists2 = D7Dist(a2,a3);

   const double triangleInequality = dists0 + dists1 + dists2 - 2.0*maxNC(dists0,dists1,dists2);
   if (triangleInequality / maxNC(dists0, dists1, dists2) < 0.0001 && triangleInequality < -1.0E-5) {
      std::cout << triangleInequality;
      ////std::cout << "              " << triangleInequality / minNC(dists0, dists1, dists2);
      std::cout << "        " << dists0 << "  " << dists1 << "  " << dists2;
      std::cout << ((triangleInequality >=0.0) ? "  VIOLATION" : "") << std::endl;
   }

   return triangleInequality < 0.0;
}
