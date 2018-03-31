
//#include "stdafx.h"

#include "LRL_StringTools.h"

#include <algorithm>
#include <cfloat>
#include <iostream>
#include <iterator>
#include <sstream>

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::string LRL_StringTools::strToupper(const std::string& s) {
   std::string ss;
   std::transform(s.begin(), s.end(), std::back_inserter(ss), toupper);
   return(ss);
}

std::vector<double> LRL_StringTools::FromString(const std::string& s) {
   std::istringstream istr(s);
   std::vector<double> t;
   double d;
   int i = 0;
   while (istr && !istr.eof()) {
      istr >> d;
      if (istr.eof() && d == DBL_MAX) break;
      t.push_back(d);
      ++i;
      d = DBL_MAX;
   }

   return t;
}

