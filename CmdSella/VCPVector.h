#ifndef VCPVECTOR_H
#define VCPVECTOR_H

#include <vector>
#include <functional>
#include "MatS6.h"
#include "EnhancedS6.h"

class VCPMatrixPair {
public:
   using VCPFunction = std::function<EnhancedS6(const EnhancedS6&)>;

   VCPMatrixPair(VCPFunction func, const MatS6& matrix);
   EnhancedS6 applyVCP(const EnhancedS6& s6) const;
   const MatS6& getMatrix() const;

private:
   VCPFunction vcpFunc;
   MatS6 mat;
};

class VCPVector {
public:
   VCPVector();
   EnhancedS6 applyVCP(size_t index, const EnhancedS6& s6) const;
   const MatS6& getMatrix(size_t index) const;

private:
   std::vector<VCPMatrixPair> pairs;
};

#endif // VCPVECTOR_H