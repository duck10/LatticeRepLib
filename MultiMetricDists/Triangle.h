#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>


#include "LRL_ReadLatticeData.h"

class Triangle {

public:
   Triangle(const std::vector<LRL_ReadLatticeData>& cells);

   size_t Test(void);
   size_t Test(const LRL_ReadLatticeData& rcd1);
   size_t Test(const LRL_ReadLatticeData& rcd1,  const LRL_ReadLatticeData& rcd2);

   bool TestTriangle(const LRL_ReadLatticeData& rcd1, const LRL_ReadLatticeData& rcd2, const LRL_ReadLatticeData& rcd3);


private:
   const std::vector<LRL_ReadLatticeData> m_triangles;
};

#endif  // TRIANGLE_H

