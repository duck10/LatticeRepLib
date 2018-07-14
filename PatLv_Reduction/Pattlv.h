#ifndef PATTLV_H
#define PATTLV_H

#include "G6.h"
#include "S6.h"

class Pattlv {
public:
   static bool Reduce();
   static bool ReduceS6ToNiggli(const S6& si, G6& g6out);

};


#endif  //  PATTLV_H
