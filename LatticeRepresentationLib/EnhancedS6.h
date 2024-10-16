#ifndef ENHANCED_S6_H
#define ENHANCED_S6_H

#include "S6.h"
#include "MatS6.h"

class EnhancedS6 : public S6 {
public:
   friend std::ostream& operator<< (std::ostream&, const EnhancedS6&);
   EnhancedS6();
   EnhancedS6(const S6& s6);
   EnhancedS6(const S6& s6, const MatS6& matrix);
   EnhancedS6(const EnhancedS6& other);
   EnhancedS6& operator=(const EnhancedS6& other);
   friend EnhancedS6 operator*(const MatS6& m, const EnhancedS6& s);

   const S6& GetS6() const { return *this; }
   EnhancedS6 operator*(const MatS6& mat) const;

   const MatS6& getTransformMatrix() const { return transformMatrix; }
   void setTransformMatrix(const MatS6& matrix) { transformMatrix = matrix; }
   static std::vector<EnhancedS6(*)(const EnhancedS6&)> SetReflectionFunctions();
   static EnhancedS6 Reflection01(const EnhancedS6& s);
   static EnhancedS6 Reflection02(const EnhancedS6& s);
   static EnhancedS6 Reflection03(const EnhancedS6& s);
   static EnhancedS6 Reflection04(const EnhancedS6& s);
   static EnhancedS6 Reflection05(const EnhancedS6& s);
   static EnhancedS6 Reflection06(const EnhancedS6& s);
   static EnhancedS6 Reflection07(const EnhancedS6& s);
   static EnhancedS6 Reflection08(const EnhancedS6& s);
   static EnhancedS6 Reflection09(const EnhancedS6& s);
   static EnhancedS6 Reflection10(const EnhancedS6& s);
   static EnhancedS6 Reflection11(const EnhancedS6& s);
   static EnhancedS6 Reflection12(const EnhancedS6& s);
   static EnhancedS6 Reflection13(const EnhancedS6& s);
   static EnhancedS6 Reflection14(const EnhancedS6& s);
   static EnhancedS6 Reflection15(const EnhancedS6& s);
   static EnhancedS6 Reflection16(const EnhancedS6& s);
   static EnhancedS6 Reflection17(const EnhancedS6& s);
   static EnhancedS6 Reflection18(const EnhancedS6& s);
   static EnhancedS6 Reflection19(const EnhancedS6& s);
   static EnhancedS6 Reflection20(const EnhancedS6& s);
   static EnhancedS6 Reflection21(const EnhancedS6& s);
   static EnhancedS6 Reflection22(const EnhancedS6& s);
   static EnhancedS6 Reflection23(const EnhancedS6& s);
   static EnhancedS6 Reflection24(const EnhancedS6& s);

private:
   MatS6 transformMatrix;
};

#endif // ENHANCED_S6_H
