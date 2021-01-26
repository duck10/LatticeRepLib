#include "Faces.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"

std::string Letters(void) {
   return "V,G,D,S,P,A,B,C,I,F,R,C3,G6,S6,B4,D7,H";
}


void Cell_Faces::ReadCell() {
   const std::string letters = Letters();
   std::string lattice;
   std::vector<LRL_ReadLatticeData> cellDataList;
   LRL_ReadLatticeData rcd;
   rcd.read();
   lattice = rcd.GetLattice();
   if ((!lattice.empty()) && (static_cast<char>(letters.find(toupper(lattice[0]))) != std::string::npos)) {
      cellDataList.push_back(rcd);
   }
   else {
      throw;
   }
   m_cell = rcd.GetCell();
   const LRL_Cell_Degrees cd(m_cell);
   m_cart = Matrix_3x3::Cart(cd[0], cd[1], cd[2], cd[3], cd[4], cd[5]);
}

std::vector<DirichletFace> Tetrahedron() {
   std::vector<DirichletFace> vdf;
   vdf.push_back(DirichletFace(Vector_3(1, -1, 1), 1));
   vdf.push_back(DirichletFace(Vector_3(-1, -1, -1), 1));
   vdf.push_back(DirichletFace(Vector_3(1, 1, -1), 1));
   vdf.push_back(DirichletFace(Vector_3(-1, 1, 1), 1));
   return vdf;
}
std::vector<DirichletFace> RectangularParallelohedron() {
   std::vector<DirichletFace> vdf;
   vdf.push_back(DirichletFace(Vector_3(1, 0, 0), 1));
   vdf.push_back(DirichletFace(Vector_3(-1, 0, 0), 1));
   vdf.push_back(DirichletFace(Vector_3(0, 1, 0), 3.000));
   vdf.push_back(DirichletFace(Vector_3(0, -1, 0), 3));
   vdf.push_back(DirichletFace(Vector_3(0, 0, 1), 7.0001));
   vdf.push_back(DirichletFace(Vector_3(0, 0, -1), 7));

   return vdf;
}

std::vector<DirichletFace> Octahedron() {
   std::vector<DirichletFace> vdf;

   static const double delta = .0;
   const double distance = 1.0 + delta;

   const Vector_3 v1(-1, -1, 1);
   const Vector_3 v2(-1, 1, 1);
   const Vector_3 v3(1, -1, 1);
   const Vector_3 v4(1, 1, 1);
   const std::vector<Vector_3> vv({ v1,v2,v3,v4 });

   for (size_t i = 0; i < 4; ++i) {
      vdf.push_back(DirichletFace(vv[i], distance));
      vdf.push_back(DirichletFace(-vv[i], 0.5*distance));
   }

   return vdf;
}

std::vector<DirichletFace> Cube() {
   std::vector<DirichletFace> vdf;

   vdf.push_back(DirichletFace(Vector_3(1, 0, 0), 1));
   vdf.push_back(DirichletFace(Vector_3(-1, 0, 0), 1));
   vdf.push_back(DirichletFace(Vector_3(0, 1, 0), 1));
   vdf.push_back(DirichletFace(Vector_3(0, -1, 0), 1));
   vdf.push_back(DirichletFace(Vector_3( 0, 0, 1), 1));
   vdf.push_back(DirichletFace(Vector_3(0, 0, -1), 1));

   return vdf;
}

std::vector<DirichletFace> Cell_Faces::CreateFaces(const CNearTree<Vector_3>& tree) {
   std::vector<DirichletFace> vFaces;
   for (size_t i = 0; i < tree.size(); ++i) {
      DirichletFace face;
      const double normFace = tree[i].Norm();
      if (abs(normFace) > 1.0E-4) {
         face.SetNormal(tree[i] / normFace);
         face.SetDistance(normFace);
         vFaces.push_back(face);
      }
   }
   return vFaces;
}

DirichletFace::DirichletFace(const Vector_3& v3, const double d)
   : m_normal(v3/v3.Norm())
   , m_distance(d)
{ }

std::vector< DirichletFace> DirichletFace::ReadFaces() {
   std::vector< DirichletFace> v;
   v.push_back(DirichletFace(Vector_3(), 19191));
   return v;
}

//C********************************************************************** C
//SUBROUTINE SOLVE(A, B, C, V)
//DIMENSION A(4), B(4), C(4), V(3), AMAT(9)
//C----------------------------------------------------------------------C
//DO 1000 I = 1, 3
//CALL MAT(AMAT, A, B, C)
//DD = DET(AMAT)
//IF(ABS(DD).LT. 1.E-4) GO TO 9000
//AMAT(I) = A(4)
//AMAT(I + 3) = B(4)
//AMAT(I + 6) = C(4)
//V(I) = DET(AMAT) / DD
//1000 CONTINUE
//RETURN
//9000 DO 9100 I = 1, 3
//V(I) = 1000000.0
//9100 CONTINUE
//END
Intersection Intersection::FindIntersectionForThreeFaces(const DirichletFace& a, const DirichletFace& b, const DirichletFace& c)
{
   { // prelude
      const double cos1 = abs(a.GetNormal().Dot(b.GetNormal()));
      const double cos2 = abs(a.GetNormal().Dot(c.GetNormal()));
      const double cos3 = abs(b.GetNormal().Dot(c.GetNormal()));

      // is some pair of planes parallel?
      if ((abs(cos1) > 0.99999) || (abs(cos2) > 0.99999) || (abs(cos3) > 0.99999)) {
         return Intersection();
      }
   }

   const Matrix_3x3 const_m(Matrix_3x3(
      a.GetNormal()[0], a.GetNormal()[1], a.GetNormal()[2],
      b.GetNormal()[0], b.GetNormal()[1], b.GetNormal()[2],
      c.GetNormal()[0], c.GetNormal()[1], c.GetNormal()[2]));
   const double constDet = const_m.Det();

   if (abs(constDet) < 1.0E-4) {
      return Intersection();
   }

   Vector_3 v;
   Intersection intersection;
   { // the real calcularions
      Matrix_3x3 m;
      intersection.m_isValid = true;
      // Cramer's rule
      for (int i = 0; i < 3; ++i) {
         m = const_m;
         m[i] = a.GetDistance()/2;
         m[i + 3] = b.GetDistance()/2;
         m[i + 6] = c.GetDistance()/2;

         const double det = m.Det();
         v[i] = det / constDet;
      }
   }

   intersection.m_coordinates = v;
   return intersection;
}

DirichletFace DirichletFace::FindIntersectionForThreeFaces(const DirichletFace& a, const DirichletFace& b, const DirichletFace& c) {
   Matrix_3x3 m;
   Intersection intersection;

   const Matrix_3x3 const_m(Matrix_3x3(
      a.m_normal[0], b.m_normal[0], c.m_normal[0],
      a.m_normal[1], b.m_normal[1], c.m_normal[1],
      a.m_normal[2], b.m_normal[2], c.m_normal[2]));
   const double det = const_m.Det();
   if (abs(det) < 1.0E-4) {
      return DirichletFace(Vector_3(DBL_MAX, DBL_MAX, DBL_MAX), DBL_MAX);
   }

   Vector_3 v;
   for (int i = 0; i < 3; ++i) {
      m = const_m;

      m[i    ] = a.m_distance;
      m[i + 3] = b.m_distance;
      m[i + 6] = c.m_distance;


      const double dot1 = abs(a.GetNormal().Dot(b.GetNormal()));
      const double dot2 = abs(a.GetNormal().Dot(c.GetNormal()));
      const double dot3 = abs(b.GetNormal().Dot(c.GetNormal()));

      if (abs(m.Det()) < 1.0E-4) {
         return DirichletFace(Vector_3(DBL_MAX, DBL_MAX, DBL_MAX), DBL_MAX);
      }

      if (dot1 > 0.99999) {
         return DirichletFace(Vector_3(DBL_MAX, DBL_MAX, DBL_MAX), DBL_MAX);
      }

      if (dot2 > 0.99999) {
         return DirichletFace(Vector_3(DBL_MAX, DBL_MAX, DBL_MAX), DBL_MAX);
      }

      if (dot3 > 0.99999) {
         return DirichletFace(Vector_3(DBL_MAX, DBL_MAX, DBL_MAX), DBL_MAX);
      }


      v[i] = m.Det() / det;
   }
   return DirichletFace(v, v.Norm());
}

void DirichletFace::SeekFaces(const double rmax, const size_t iplane, const std::vector<DirichletFace>& faces) {

   Vector_3 sum(0.0, 0.0, 0.0);
   double temp(0.0);
   for (size_t i = 0; i < faces.size(); ++i) temp += faces[i].m_distance;
   temp /= double(faces.size());

   std::vector<Vector_3> corners;
   bool goodVector = true;
   for (size_t i1 = 0; i1 < faces.size(); ++i1) {
      for (size_t i2 = 0; i1+1 < faces.size(); ++i2) {
         for (size_t i3 = 0; i3 < faces.size(); ++i3) {
            const DirichletFace df = DirichletFace::FindIntersectionForThreeFaces(faces[i1], faces[i2], faces[i3]);
            for (size_t k = 0; k < i3; ++k) { // line   DO 9200
               const double d = df.GetNormal().Dot(faces[k].m_normal);
               if (d - faces[k].m_distance > 1.0E-5 * temp) {
                  goodVector = false;
                  break;
               }
            }
            if (goodVector) corners.push_back(df.GetNormal());
         }
      }
   }

   //compute the rmax if rmax is not yet determined
   if (rmax <= 0.0) throw("compute new rmax here and put it in the return face!!!!!!!!!");
   //return corners;
}
