#include "stdafx.h"

#include "S6_Boundary.h"
#include "S6_BoundaryList.h"

std::vector<S6_Boundary> S6_BoundaryList::m_bounds = S6_BoundaryList::CalculateBoundaries();

S6_BoundaryList::S6_BoundaryList() {
   CalculateBoundaries();
}

std::vector<S6_Boundary> S6_BoundaryList::CalculateBoundaries() {
   if (m_bounds.size() != 0) return m_bounds;
   m_bounds.push_back(S6_Boundary(S6_Boundary::Perp1, S6_Boundary::Prj1));
   m_bounds.push_back(S6_Boundary(S6_Boundary::Perp2, S6_Boundary::Prj2));
   m_bounds.push_back(S6_Boundary(S6_Boundary::Perp3, S6_Boundary::Prj3));
   m_bounds.push_back(S6_Boundary(S6_Boundary::Perp4, S6_Boundary::Prj4));
   m_bounds.push_back(S6_Boundary(S6_Boundary::Perp5, S6_Boundary::Prj5));
   m_bounds.push_back(S6_Boundary(S6_Boundary::Perp6, S6_Boundary::Prj6));

   m_bounds[0].SetName("Sellin 1");
   m_bounds[1].SetName("Sellin 2");
   m_bounds[2].SetName("Sellin 3");
   m_bounds[3].SetName("Sellin 4");
   m_bounds[4].SetName("Sellin 5");
   m_bounds[5].SetName("Sellin 6");

   m_bounds[0].SetCondition("s1=0");
   m_bounds[1].SetCondition("s2=0");
   m_bounds[2].SetCondition("s3=0");
   m_bounds[3].SetCondition("s4=0");
   m_bounds[4].SetCondition("s5=0");
   m_bounds[5].SetCondition("s6=0");

   return m_bounds;
}