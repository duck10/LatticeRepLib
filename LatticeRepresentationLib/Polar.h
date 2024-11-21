#ifndef POLAR_H
#define POLAR_H

#include "LRL_Cell_Degrees.h"
#include "Vector_2.h"


class Polar {
public:
   friend std::ostream& operator<< (std::ostream&, const Polar&);

   Polar();
   Polar(const Vector_2& v1, const Vector_2& v2, const Vector_2& v3);
   static size_t size() { return 3; }

   bool CheckValid();
   //bool CheckValid(const Polar& p);
   static bool CheckValid(const LRL_Cell& cell);
   static bool CheckValid(const double a, const double b, const double c,
      const double alpha, const double beta, const double gamma);


   static Polar_2 ConvertUnitSphereProjectionToLengthAndAngle(const Vector_2& v);
   static LRL_Cell_Degrees ConvertPolarToCell(const Polar& p);

   template<typename T>
   Polar(const T& t) {
      m_cell = t;
      m_vec.resize(3);
      for (size_t i = 0; i < 3; ++i) {
         m_vec[i] = Vector_2(m_cell[i] * cos(m_cell[i + 3]), m_cell[i] * sin(m_cell[i + 3]));
         if (abs(m_vec[i][0]) < 1.0E-8) m_vec[i][0] = 0.0;
         if (abs(m_vec[i][1]) < 1.0E-8) m_vec[i][1] = 0.0;
      }
   }

   static Polar rand();
   static Polar RandWork();
   Vector_2 operator[] (const size_t n) const {
      return m_vec[n];
   }
   Vector_2& operator[](const size_t n) { return m_vec[n]; }

   operator LRL_Cell() const;

private:
   std::vector<Vector_2> m_vec;
   LRL_Cell m_cell;
};

#endif // POLAR_H
