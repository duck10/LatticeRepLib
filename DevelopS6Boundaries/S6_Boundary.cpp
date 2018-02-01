#include "stdafx.h"

#include "S6_Boundary.h"

std::ostream& operator<< (std::ostream& o, const S6_Boundary& v) {
   o << "Boundary ------------------------------------ " << v.GetName() << std::endl;
   o << "Condition: " << v.GetCondition() << std::endl << std::endl;

   return o;
}

S6_Boundary::S6_Boundary(const s6fun perp, const s6fun prj)
   : m_perp(perp)
   , m_prj(prj)
{}

S6 S6_Boundary::Perp1(const S6& sin) { return S6(sin[0], 0,      0,      0,      0,      0);      }
S6 S6_Boundary::Perp2(const S6& sin) { return S6(0,      sin[1], 0,      0,      0,      0);      }
S6 S6_Boundary::Perp3(const S6& sin) { return S6(0,      0,      sin[2], 0,      0,      0);      }
S6 S6_Boundary::Perp4(const S6& sin) { return S6(0,      0,      0,      sin[3], 0,      0);      }
S6 S6_Boundary::Perp5(const S6& sin) { return S6(0,      0,      0,      0,      sin[4], 0);      }
S6 S6_Boundary::Perp6(const S6& sin) { return S6(0,      0,      0,      0,      0,      sin[5]); }

S6 S6_Boundary::Prj1(const S6& sin) { return S6(0,      sin[1], sin[2], sin[3], sin[4], sin[5]); }
S6 S6_Boundary::Prj2(const S6& sin) { return S6(sin[0], 0,      sin[2], sin[3], sin[4], sin[5]); }
S6 S6_Boundary::Prj3(const S6& sin) { return S6(sin[0], sin[1], 0,      sin[3], sin[4], sin[5]); }
S6 S6_Boundary::Prj4(const S6& sin) { return S6(sin[0], sin[1], sin[2], 0,      sin[4], sin[5]); }
S6 S6_Boundary::Prj5(const S6& sin) { return S6(sin[0], sin[1], sin[2], sin[3], 0,      sin[5]); }
S6 S6_Boundary::Prj6(const S6& sin) { return S6(sin[0], sin[1], sin[2], sin[3], sin[4], 0); }


