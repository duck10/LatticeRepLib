#include "Vec_N_Tools.h"

#include <cmath>
#include <cfloat>
#include <iostream>
#include <sstream>

std::vector<double> Vec_N_Tools::FromString(const std::string& s) {
   std::istringstream istr(s);
   std::vector<double> t;
   double d;
   int i = 0;
   while ( istr && ! istr.eof() ) {
      istr >> d;
      if ( istr.eof( )  && d == DBL_MAX ) break;
      t.push_back( d );
      ++i;
      d = DBL_MAX;
   }

   return t;
}

