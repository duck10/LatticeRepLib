#ifndef PATH_H
#define PATH_H

#include "LatticeCell.h"
#include <ostream>
#include <utility>
#include <vector>

//Path generatePath(...)
//Path generatePointPath(...)
//Path generateLinePath(...)
//Path generateChordPath(...)
//Path generateChord3Path(...)
//Path generateTrianglePath(...)
//
//These are all path generation functions that operate on LatticeCell inputs and use controls only for the number of points.They should be moved to Path.cpp as static methods:

class Path : public std::vector<std::pair<LatticeCell, LatticeCell>> {
public:
   friend std::ostream& operator<<(std::ostream& os, const Path& path) {
      os << "; the path" << std::endl;
      int count = 0;
      for (const auto& [point1, point2] : path) {
         const S6 p1(point1.getCell());
         const S6 p2(point2.getCell());
         const std::string invalid1 = (p1.IsValid()) ? "" : " invalid";
         const std::string invalid2 = (p2.IsValid()) ? "" : " invalid";
         os << "; " << point1.getLatticeType() << " " << p1 << invalid1
            << "\n; " << point2.getLatticeType() << " " << p2 << invalid2
            << ";--------  end of path -----------------  index " << count << "\n\n";

         ++count;
      }
      return os;
   }

   static Path generatePointPath(const LatticeCell& point, int numPoints);
   static Path generateLinePath(const LatticeCell& start, const LatticeCell& end, int numPoints);
   static Path generateChordPath(const LatticeCell& p1, const LatticeCell& p2, int numPoints);
   static Path generateChord3Path(const LatticeCell& m1, const LatticeCell& m2,
      const LatticeCell& target, int numPoints);
   static Path generateTrianglePath(const LatticeCell& p1, const LatticeCell& p2,
      const LatticeCell& p3, int numPoints);
   static Path generateSplinePath( const int numPoints,  const std::vector<LatticeCell>& cells);

   static bool PathPointIsValid(const S6& p) {
      return(LRL_Cell(p).IsValid());
   }

};

#endif // PATH_H
