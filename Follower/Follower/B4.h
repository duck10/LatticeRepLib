#ifndef B4_H
#define B4_H

#include "BasisBase.h"
#include "LRL_Vector3.h"

#include <vector>

   class LRL_Cell;
   class C3;
   class D7;
   class S6;
   class G6;

#include <ostream>
#include <string>

class B4 : private BasisBase<B4, Vector_3> {
public:

   friend std::ostream& operator<< (std::ostream&, const B4&);
   friend B4 operator* (const double d, const B4& dt);

   B4(void);
   B4(const C3& v6);
   B4(const B4& dt);
   B4(const G6& v6);
   B4(const S6& c);
   B4(const D7& v7);
   B4(const LRL_Cell& c);
   B4(const std::string& c);
   B4(const Vector_3& v0, const Vector_3& v1, const Vector_3& v2, const Vector_3& v3 );
   ~B4(void) {}

   B4& operator= (const B4& v);
   B4& operator= (const std::string& s);
   B4& operator= (const G6& v);
   B4& operator= (const C3& c3);
   B4& operator= (const S6& v);
   B4& operator= (const D7& v);
   B4& operator= (const LRL_Cell& v);
   B4& operator/= (const double d);
   B4& operator*= (const double d);
   B4 operator+ (const B4& dt) const;
   B4 operator- (const B4& dt) const;
   B4& operator+= (const B4& dt);
   B4& operator-= (const B4& dt);
   B4 operator* (const double d) const;
   B4 operator/ (const double d) const;
   B4 operator- (void) const; // unary
   bool operator== (const B4& b4) const;
   bool operator!= (const B4& b4) const;

   Vector_3  operator[] (const size_t n) const;
   Vector_3& operator[] (const size_t n);

   double DistanceBetween( const B4& v1, const B4& v2 );
   size_t size(void) const { return 4; }
   double norm(void) const;
   double norm(const B4& dt) const;
   double Norm() const { return norm(); }
   double Norm(const B4& dt) const { return norm(dt); }

   static B4 sort(const B4& dt);
   B4 sort(void) const;
   std::vector<Vector_3> GetVector(void) const { return m_vec; }
   void SetVector(const std::vector<Vector_3>& v) { m_vec = v; }
   typedef std::vector<Vector_3> C3Type;
   Vector_3* data() const { return const_cast<Vector_3* >(&m_vec[0]); }  //???????????????????????????????
   bool GetValid(void) const { return m_valid; }
   void SetValid(const bool b) { m_valid = b; }

   Vector_3 at(const size_t n) const { return m_vec[n]; }

   static B4 rand();
   static B4 randDeloneReduced();
   static B4 randDeloneUnreduced();
   static B4 rand(const double d);
   static B4 randDeloneReduced(const double d);
   static B4 randDeloneUnreduced(const double d);
   static std::string GetName(void) { return "B4, Delone Tetrahedron"; }

protected:
   std::vector<Vector_3> m_vec;
   bool m_valid;
};

#endif //  B4_H
