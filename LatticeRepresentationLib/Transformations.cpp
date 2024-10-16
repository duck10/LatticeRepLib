#include "EnhancedS6.h"
#include "Transformations.h"


Transformations::Transformations() : m_s() {}

Transformations::Transformations(const EnhancedS6& s) : m_s(s) {}

EnhancedS6 Transformations::operator()(const EnhancedS6& v) const {
   // Implement the transformation logic here
    // This should apply the transformation stored in m_s to v
   EnhancedS6 result = v;
   result.setTransformMatrix(m_s.getTransformMatrix() * v.getTransformMatrix());
   // Apply any necessary changes to the S6 part of result
   return result;
}


//EnhancedS6 Transformations::operator()(const EnhancedS6& v) const {
//   // Implement the transformation logic here
//   // Make sure to handle both the S6 part and the transformation matrix
//   EnhancedS6 result/* =  transformation logic */;
//   return result;
//}