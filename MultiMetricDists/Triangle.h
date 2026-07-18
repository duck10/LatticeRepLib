#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>


#include "LRL_Cell.h"

class Triangle {

public:
   Triangle(const std::vector<LRL_Cell>& cells);

   size_t Test(void);
   size_t Test(const LRL_Cell& rcd1);
   size_t Test(const LRL_Cell& rcd1, const LRL_Cell& rcd2);

   bool TestTriangle(const LRL_Cell& rcd1, const LRL_Cell& rcd2, const LRL_Cell& rcd3);


private:
   const std::vector<LRL_Cell> m_triangles;
};

#endif  // TRIANGLE_H
