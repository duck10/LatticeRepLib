#ifndef D13_H
#define D13_H

#include <vector>

#include "LRL_Cell.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"

class D13 {

   const static std::vector<Vector_3> vertices;
public:
   D13() {}

   template<typename T>
   D13(const T& t) {
      const LRL_Cell cell(t);
      const Matrix_3x3 cartesian(cell.Cart());
      //v13.resize(13);
      //for (size_t i = 0; i < vertices.size(); ++i) {
      //   v13[i] = (cartesian * vertices[i]).Norm();
      //}
   }

   std::vector<Vector_3> ComputeIndices();

   std::vector<double> GetAreas() const { return areas; }

   std::vector<std::string> GetStringIndices();
   std::vector<Vector_3> GetIndices();

   std::string GetIndex(const size_t n);

private:
   std::vector<Vector_3> indices;
   std::vector<std::string> sIndices;
   std::vector<double> areas;

   //void BuildIndices();
};

#endif  // D13_h
