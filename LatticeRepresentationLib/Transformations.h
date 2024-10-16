#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "S6.h"
#include "EnhancedS6.h"

class Transformations : public S6 {
public:
   Transformations();
   explicit Transformations(const EnhancedS6& s);

   const EnhancedS6& getEnhancedS6() const { return m_s; }
   EnhancedS6 operator()(const EnhancedS6& v) const;

private:
   EnhancedS6 m_s;
};


#endif // TRANSFORMATIONS_H