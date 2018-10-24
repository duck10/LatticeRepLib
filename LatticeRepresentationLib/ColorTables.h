#ifndef COLORTABLES_H
#define COLORTABLES_H

#include <string>

void GetCividisRGB(const unsigned long n, int& rreturn, int& greturn, int& breturn);
   std::string OrdinalToCividisHexString(const unsigned long n);

#endif // COLORTABLES_H
