#ifndef S6_BOUNDARY_H
#define S6_BOUNDARY_H

#include "S6.h"

   typedef S6(*s6fun)(const S6& s6);

class S6_Boundary /*: private BoundaryBase<S6, S6_operator> */ {

   friend std::ostream& operator<< (std::ostream&, const S6_Boundary&);

   class S6_operator {
   public:
      S6_operator() {}
      S6_operator(s6fun fun)
         : m_op(fun)
      {   }

      S6 operator*(const S6& s6) const
      {
         return m_op(s6);
      }

      s6fun m_op;
   };

public:
   S6_Boundary(const s6fun perp, const s6fun prj);
   const S6_operator& GetPerp(void) const { return m_perp; }
   const S6_operator& GetProjector(void) const { return m_prj; }

   std::string GetName(void)  const { return m_name; }
   std::string GetCondition(void)  const { return m_condition; }

   void  SetName(const std::string& name) { m_name = name; }
   void  SetCondition(const std::string& condition) { m_condition = condition; }


   static S6 Perp1(const S6& sin);
   static S6 Perp2(const S6& sin);
   static S6 Perp3(const S6& sin);
   static S6 Perp4(const S6& sin);
   static S6 Perp5(const S6& sin);
   static S6 Perp6(const S6& sin);

   static S6 Prj1(const S6& sin);
   static S6 Prj2(const S6& sin);
   static S6 Prj3(const S6& sin);
   static S6 Prj4(const S6& sin);
   static S6 Prj5(const S6& sin);
   static S6 Prj6(const S6& sin);

private:
   const S6_operator m_perp;
   const S6_operator m_prj;
   std::string m_name;
   std::string m_condition;

};



#endif  // S6_BOUNDARY_H
