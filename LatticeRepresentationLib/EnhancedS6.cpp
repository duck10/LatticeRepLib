#include "EnhancedS6.h"
#include "S6.h"

#include <iomanip>


EnhancedS6::EnhancedS6()
   : S6(), transformMatrix(MatS6::Eye()) {}

EnhancedS6::EnhancedS6(const S6& s6)
   : S6(s6), transformMatrix(MatS6::Eye()) {}

EnhancedS6::EnhancedS6(const S6& s6, const MatS6& matrix)
   : S6(s6), transformMatrix(matrix) {}

EnhancedS6::EnhancedS6(const EnhancedS6& other)
   : S6(other), transformMatrix(other.transformMatrix) {}

EnhancedS6& EnhancedS6::operator=(const EnhancedS6& other) {
   if (this != &other) {
      S6::operator=(other);
      transformMatrix = other.transformMatrix;
   }
   return *this;
}

EnhancedS6 operator*(const MatS6& m, const EnhancedS6& s) {
   S6 newS6 = m * static_cast<const S6&>(s);
   MatS6 newMatrix = m * s.getTransformMatrix();
   return EnhancedS6(newS6, newMatrix);
}


EnhancedS6 EnhancedS6::Reflection01(const EnhancedS6& s) {
   S6 reflected = S6::Reflection01(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection02(const EnhancedS6& s) {
   S6 reflected = S6::Reflection02(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection03(const EnhancedS6& s) {
   S6 reflected = S6::Reflection03(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection04(const EnhancedS6& s) {
   S6 reflected = S6::Reflection04(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection05(const EnhancedS6& s) {
   S6 reflected = S6::Reflection05(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection06(const EnhancedS6& s) {
   S6 reflected = S6::Reflection06(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection07(const EnhancedS6& s) {
   S6 reflected = S6::Reflection07(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection08(const EnhancedS6& s) {
   S6 reflected = S6::Reflection08(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection09(const EnhancedS6& s) {
   S6 reflected = S6::Reflection09(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection10(const EnhancedS6& s) {
   S6 reflected = S6::Reflection10(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection11(const EnhancedS6& s) {
   S6 reflected = S6::Reflection11(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection12(const EnhancedS6& s) {
   S6 reflected = S6::Reflection12(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection13(const EnhancedS6& s) {
   S6 reflected = S6::Reflection13(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection14(const EnhancedS6& s) {
   S6 reflected = S6::Reflection14(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection15(const EnhancedS6& s) {
   S6 reflected = S6::Reflection15(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection16(const EnhancedS6& s) {
   S6 reflected = S6::Reflection16(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection17(const EnhancedS6& s) {
   S6 reflected = S6::Reflection17(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection18(const EnhancedS6& s) {
   S6 reflected = S6::Reflection18(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection19(const EnhancedS6& s) {
   S6 reflected = S6::Reflection19(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection20(const EnhancedS6& s) {
   S6 reflected = S6::Reflection20(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection21(const EnhancedS6& s) {
   S6 reflected = S6::Reflection21(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection22(const EnhancedS6& s) {
   S6 reflected = S6::Reflection22(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection23(const EnhancedS6& s) {
   S6 reflected = S6::Reflection23(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

EnhancedS6 EnhancedS6::Reflection24(const EnhancedS6& s) {
   S6 reflected = S6::Reflection24(static_cast<const S6&>(s));
   return EnhancedS6(reflected, s.getTransformMatrix());
}

std::vector<EnhancedS6(*)(const EnhancedS6&)> EnhancedS6::SetReflectionFunctions() {
   return {
   &EnhancedS6::Reflection01,
   &EnhancedS6::Reflection02,
   &EnhancedS6::Reflection03,
   &EnhancedS6::Reflection04,
   &EnhancedS6::Reflection05,
   &EnhancedS6::Reflection06,
   &EnhancedS6::Reflection07,
   &EnhancedS6::Reflection08,
   &EnhancedS6::Reflection09,
   &EnhancedS6::Reflection10,
   &EnhancedS6::Reflection11,
   &EnhancedS6::Reflection12,
   &EnhancedS6::Reflection13,
   &EnhancedS6::Reflection14,
   &EnhancedS6::Reflection15,
   &EnhancedS6::Reflection16,
   &EnhancedS6::Reflection17,
   &EnhancedS6::Reflection18,
   &EnhancedS6::Reflection19,
   &EnhancedS6::Reflection20,
   &EnhancedS6::Reflection21,
   &EnhancedS6::Reflection22,
   &EnhancedS6::Reflection22,
   &EnhancedS6::Reflection24 };
}


std::ostream& operator<< (std::ostream& o, const EnhancedS6& dc) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(5);
   const size_t n = dc.size();
   for (size_t i = 0; i < n; ++i) {
      const double d = dc[i];
      o << std::setw(9) << d << " ";
   }
   std::cout << std::endl;

   const MatS6& m = dc.getTransformMatrix();
   for (size_t i = 0; i < m.size(); i+=6) {
      for (size_t kk = 0; kk < 6; ++kk) {
         std::cout << m[i + kk] << " ";
      }
      std::cout << std::endl;
   }
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

EnhancedS6 EnhancedS6::operator*(const MatS6& mat) const {
   S6 newS6 = mat * static_cast<const S6&>(*this);
   MatS6 newMatrix = mat * transformMatrix;
   return EnhancedS6(newS6, newMatrix);
}