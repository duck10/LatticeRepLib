#ifndef ROOFFIGURES_H
#define ROOFFIGURES_H

#include <string>
#include <vector>

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_Vector3.h"

static std::vector<Vector_3> MV(const Matrix_3x3& m, const std::vector<Vector_3>& vv) {
   std::vector<Vector_3> out;
   for (const auto& v : vv) {
      out.emplace_back(m * v);
   }
   return out;
}

static std::vector<Vector_3> MV( const std::vector<Vector_3>& vv, const Matrix_3x3& m) {
   return MV(m, vv);
}

static std::vector<Vector_3> Translate(const Vector_3& vin, const std::vector<Vector_3>& vv) {
   std::vector<Vector_3> out;
   for (const auto& v : vv) {
      out.emplace_back(vin + v);
   }
   return vv;
}

static std::vector<Vector_3> Translate( const std::vector<Vector_3>& vv, const Vector_3& vin) {
   return Translate(vin, vv);;
}

class BaseBodyCentered {
public:

};

class BaseFaceCentered {
public:

};

class BaseCell {
public:
   friend std::ostream& operator<< (std::ostream&, const BaseCell&);

   BaseCell(const LRL_Cell& cell);
   std::string DrawCell() const;
public:
   std::vector<Vector_3> m_corners;
   LRL_Cell m_cell;
   Matrix_3x3 m_cart;

public:
   std::string m_name;
   Matrix_3x3 m_orient;
   std::vector<Vector_3> m_placedCorners;
};

class Figure_44A : public BaseCell {
public:
   Figure_44A();
};


#endif // ROOFFIGURES_H


