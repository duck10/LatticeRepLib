#include "LRL_Formatter.h"

std::string FormatterBase::formatPath(const Path& path) const {
   std::stringstream ss;

   int count = 0;
   for (const auto& [point1, point2] : path) {
      const S6 p1(point1.getCell());
      const S6 p2(point2.getCell());
      const std::string invalid1 = (p1.IsValid()) ? "" : " invalid";
      const std::string invalid2 = (p2.IsValid()) ? "" : " invalid";

      ss << formatLattice(point1) << invalid1 << std::endl;
      ss << formatLattice(point2) << invalid2 << std::endl;
      ss << "; --------  end of path -----------------  index " << count++ << std::endl << std::endl;
   }
   return ss.str();
}

