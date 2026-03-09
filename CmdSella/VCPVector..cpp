#include "VCPVector.h"
#include "S6.h"

VCPMatrixPair::VCPMatrixPair(VCPFunction func, const MatS6& matrix)
   : vcpFunc(func), mat(matrix) {}

EnhancedS6 VCPMatrixPair::applyVCP(const EnhancedS6& s6) const {
   EnhancedS6 result = vcpFunc(s6);
   result.setTransformMatrix(mat * s6.getTransformMatrix());
   return result;
}

const MatS6& VCPMatrixPair::getMatrix() const {
   return mat;
}

VCPVector::VCPVector() {
   // Implement all six VCP functions
   pairs.emplace_back(
      [](const EnhancedS6& s) {
         EnhancedS6 result(s);
         result[0] = 0.0;
         const MatS6 reflectionMatrix("-1 0 0 0 0 0    1 1 0 0 0 0    1 0 0 0 1 0    -1 0 0 1 0 0    1 0 1 0 0 0    1 0 0 0 0 1");
         result.setTransformMatrix(reflectionMatrix * s.getTransformMatrix());
         return result;
      },
      MatS6("-1 0 0 0 0 0    1 1 0 0 0 0    1 0 0 0 1 0    -1 0 0 1 0 0    1 0 1 0 0 0    1 0 0 0 0 1")
   );
   pairs.emplace_back(
      [](const EnhancedS6& s) {
         EnhancedS6 result(s);
         result[1] = 0.0;
         const MatS6 reflectionMatrix("1 1 0 0 0 0    0 -1 0 0 0 0    0 1 0 1 0 0    0 1 1 0 0 0    0 -1 0 0 1 0    0 1 0 0 0 1");
         result.setTransformMatrix(reflectionMatrix * s.getTransformMatrix());
         return result;
      },
      MatS6("1 1 0 0 0 0    0 -1 0 0 0 0    0 1 0 1 0 0    0 1 1 0 0 0    0 -1 0 0 1 0    0 1 0 0 0 1")
   );
   pairs.emplace_back(
      [](const EnhancedS6& s) {
         EnhancedS6 result(s);
         result[2] = 0.0;
         const MatS6 reflectionMatrix("1 0 1 0 0 0    0 0 1 1 0 0    0 0 -1 0 0 0    0 1 1 0 0 0    0 0 1 0 1 0    0 0 -1 0 0 1");
         result.setTransformMatrix(reflectionMatrix * s.getTransformMatrix());
         return result;
      },
      MatS6("1 0 1 0 0 0    0 0 1 1 0 0    0 0 -1 0 0 0    0 1 1 0 0 0    0 0 1 0 1 0    0 0 -1 0 0 1")
   );
   pairs.emplace_back(
      [](const EnhancedS6& s) {
         EnhancedS6 result(s);
         result[3] = 0.0;
         const MatS6 reflectionMatrix("1 0 0 -1 0 0    0 0 1 1 0 0    0 1 0 1 0 0    0 0 0 -1 0 0    0 0 0 1 1 0    0 0 0 1 0 1");
         result.setTransformMatrix(reflectionMatrix * s.getTransformMatrix());
         return result;
      },
      MatS6("1 0 0 -1 0 0    0 0 1 1 0 0    0 1 0 1 0 0    0 0 0 -1 0 0    0 0 0 1 1 0    0 0 0 1 0 1")
   );
   pairs.emplace_back(
      [](const EnhancedS6& s) {
         EnhancedS6 result(s);
         result[4] = 0.0;
         const MatS6 reflectionMatrix("0 0 1 0 1 0    0 1 0 0 -1 0    1 0 0 0 1 0    0 0 0 1 1 0    0 0 0 0 -1 0    0 0 0 0 1 1");
         result.setTransformMatrix(reflectionMatrix * s.getTransformMatrix());
         return result;
      },
      MatS6("0 0 1 0 1 0    0 1 0 0 -1 0    1 0 0 0 1 0    0 0 0 1 1 0    0 0 0 0 -1 0    0 0 0 0 1 1")
   );
   pairs.emplace_back(
      [](const EnhancedS6& s) {
         EnhancedS6 result(s);
         result[5] = 0.0;
         const MatS6 reflectionMatrix("0 1 0 0 0 1    1 0 0 0 0 1    0 0 1 0 0 -1    0 0 0 1 0 1    0 0 0 0 1 1    0 0 0 0 0 -1");
         result.setTransformMatrix(reflectionMatrix * s.getTransformMatrix());
         return result;
      },
      MatS6("0 1 0 0 0 1    1 0 0 0 0 1    0 0 1 0 0 -1    0 0 0 1 0 1    0 0 0 0 1 1    0 0 0 0 0 -1")
   );
}

EnhancedS6 VCPVector::applyVCP(size_t index, const EnhancedS6& s6) const {
   if (index < pairs.size()) {
      return pairs[index].applyVCP(s6);
   }
   throw std::out_of_range("VCP index out of range");
}

const MatS6& VCPVector::getMatrix(size_t index) const {
   if (index < pairs.size()) {
      return pairs[index].getMatrix();
   }
   throw std::out_of_range("VCP index out of range");
}
