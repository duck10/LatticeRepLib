#ifndef COLORTABLES_H
#define COLORTABLES_H

#include <string>

void GetCividisRGB(const unsigned long n, int& rreturn, int& greturn, int& breturn);
   std::string OrdinalToCividisHexString(const unsigned long n);

   static const std::string BASIC_COLORS[] = { "red", "lightblue", "turquoise", "slategrey",
                     "orange", "blueviolet", "coral", "saddlebrown", "blue", "pink", "violet",
                     "deeppink", "mediumvioletred", "tomato", "greenyellow", "olive" };


#endif // COLORTABLES_H
