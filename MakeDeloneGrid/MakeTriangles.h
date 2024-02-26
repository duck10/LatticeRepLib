#ifndef MAKETRIANGLES_H
#define MAKETRIANGLES_H

#include "LRL_Vector3.h"

#include <map>
#include <string>
#include <vector>


static double slashHalfHeight = .08;
static double slashWidth = 2.0;
static double slashShift = 2.0;

class Slashes {
public:
   // Declaration
   enum class Color { Red, Green, Blue };

   // Initialisation
   Color col = Color::Red;

   enum class eSlashCount { none, one, two, three, zero };
   enum class eTetrahedronSide { P, Q, R, S, T, U };
   eTetrahedronSide esides;
   eSlashCount ecounts;

   void MakeSlashes();

   std::pair<Vector_3, Vector_3> OneSlashAC() const;
   std::vector<std::pair<Vector_3, Vector_3>> TwoSlashs();
   std::vector<std::pair<Vector_3, Vector_3>> ThreeSlashs();

   std::string  CharacterToSlashes(const std::string& crtr);
   Matrix_3x3 MatForSide(const eTetrahedronSide& side) const;
   void BuildSlashMap();

   // the posiions in the crtr list are: P,Q,R,S,T,Y OR s1,s2,s3,s4,s5,s6
   std::map<std::pair< eTetrahedronSide, eSlashCount>, std::string > slashMap;

   Vector_3 SideLabelToShiftToMidpoint(const eTetrahedronSide& side);
   std::string ConvertZeroToSVG(const eTetrahedronSide& side, const std::pair<Vector_3, Vector_3>& one);
   std::string ConvertSlashToSVG(const eTetrahedronSide& side, const std::pair<Vector_3, Vector_3>& one);
   std::string ConvertSlashToSVG(const eTetrahedronSide& side,
      const std::vector<std::pair<Vector_3, Vector_3> >& mult);

   //}

   std::string GetSlashes(const std::string& chracter);
};


class Triangles {
public:

   Triangles();
   std::vector<Vector_3> MakeBasicElements();
   static double Rescale(const double x);
   static Vector_3 Rescale(const Vector_3& x);
   static std::vector<Vector_3> Rescale(const std::vector<Vector_3>& vvec);
   std::string MakeTrianglePolyline(const std::vector<Vector_3>& vvec);
   std::string MakeBasicTriangle();

   void MakeSlashes();

   std::vector<Vector_3> OneSlashAC();
   std::vector<Vector_3> TwoSlashs();
   std::vector<Vector_3> ThreeSlashs();

   void MakeTrianglePointsAndMatrices();

public:
   Matrix_3x3 mat30 ;
   Matrix_3x3 mat60 ;
   Matrix_3x3 mat90 ;
   Matrix_3x3 matm30;
   Matrix_3x3 matm60;

   Vector_3 A;
   Vector_3 C;
   Vector_3 B;
   Vector_3 D;

   Vector_3 midAB;
   Vector_3 midAC;
   Vector_3 midBC;
                 ;
   Vector_3 midAD;
   Vector_3 midBD;
   Vector_3 midCD;


};
#endif // !MAKETRIANGLES_H

