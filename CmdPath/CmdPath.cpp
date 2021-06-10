
#include <iostream>
#include <vector>

#include "FollowerConstants.h"
#include"LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "G6.h"
#include "Niggli.h"

class G6_Reduced : public G6 {
public:
   G6_Reduced() {}
   G6_Reduced(const G6& g6);
};

G6_Reduced::G6_Reduced(const G6& g6) {
   G6 g6out;
   m_valid = Niggli::Reduce( g6, g6out);
   m_vec = g6out.GetVector();
}

class S6_Reduced : public S6 {
   S6_Reduced() {}
   S6_Reduced(const S6& s6);
};

S6_Reduced::S6_Reduced(const S6& s6) {
   S6 s6out;
   m_valid = Selling::Reduce(s6, s6out);
   m_vec = s6out.GetVector();
}


std::vector<std::pair<S6, S6> > MakeS6SinglePointPath(const LRL_Cell& start, const LRL_Cell&target) {
   const size_t npath = FollowerConstants::globalStepsPerFrame;
   std::vector<std::pair<S6, S6> > path;
   for (size_t i = 0; i < npath + 1; ++i) {
      const S6 cellX = (double(i) * S6(target) + double(npath - i) * S6(start)) / double(npath);
      path.push_back(std::make_pair(cellX, target));
   }
   return path;
}

std::vector<std::pair<S6, S6> > MakeS6LinePath(const LRL_Cell& c1, const LRL_Cell& c2) {
   const std::vector<std::pair<S6, S6> > path1 = MakeS6SinglePointPath(c1, G6_Reduced(G6(c1)));
   const std::vector<std::pair<S6, S6> > path2 = MakeS6SinglePointPath(c2, G6_Reduced(G6(c2)));

   std::vector<std::pair<S6, S6> > path;
   for (size_t i = 0; i < path1.size(); ++i)
      path.push_back(std::make_pair(path1[i].first, path2[i].first));

   return path;
}

std::vector<std::pair<S6, S6> > MakeS6Line3Path(const LRL_Cell& c1, const LRL_Cell& c2, const LRL_Cell& c3) {
   const S6 initialMidPoint = (S6(c1) + S6(c2)) / 2.0;
   const std::vector<std::pair<S6, S6> > midPointPath = MakeS6SinglePointPath(initialMidPoint, c3);
   const S6 delta1 = S6(c1) - initialMidPoint;
   const S6 delta2 = S6(c2) - initialMidPoint;

   for ( size_t i=0; i<midPointPath.size(); ++i )
      std::cout << i << "  " << LRL_Cell_Degrees(midPointPath[i].first) << "  " << LRL_Cell_Degrees(midPointPath[i].second) << std::endl;

   std::vector<std::pair<S6, S6> > path;
   for (size_t i = 0; i < midPointPath.size(); ++i)
      path.push_back(std::make_pair(delta1+midPointPath[i].first, delta2+midPointPath[i].first));

   return path;
}

std::vector<std::pair<S6, S6> > CreatePath(const std::vector<LRL_ReadLatticeData>& cells, const size_t n1) {
   const size_t npath =FollowerConstants::globalStepsPerFrame;
   std::vector<std::pair<S6, S6> > path;
   if (FollowerConstants::globalFollowerMode == FollowerConstants::globalSinglePoint) {
      const S6 target = G6_Reduced(G6(cells[n1].GetCell()));
      path = MakeS6SinglePointPath(cells[n1].GetCell(),target);
   }
   else if (FollowerConstants::globalFollowerMode == FollowerConstants::globalChord) {
      path = MakeS6LinePath(cells[n1].GetCell(), cells[n1 + 1].GetCell());
   }
   else if (FollowerConstants::globalFollowerMode == FollowerConstants::globalChord3) {
      path = MakeS6Line3Path(cells[n1].GetCell(), cells[n1 + 1].GetCell(), cells[n1 + 2].GetCell());
   }
   else if (FollowerConstants::globalFollowerMode == FollowerConstants::globalTriangle) {
   };
   return path;
}

int main()
{ 
   FollowerConstants::globalStepsPerFrame = 20;
   FollowerConstants::globalFollowerMode = FollowerConstants::globalChord3;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::vector<std::pair<S6, S6> > path;
   std::cout << "; Path generator" << std::endl;
   for (size_t i = 0; i < inputList.size(); ++i) {
      path = CreatePath(inputList, i);
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalChord) ++i;
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalChord3) ++i, ++i;
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalTriangle) ++i, ++i;
   }


   for (size_t i = 0; i < path.size(); ++i) {
      std::cout << i + 1 << "  " << LRL_Cell_Degrees(path[i].first) << "    " << LRL_Cell_Degrees(path[i].second) << std::endl;
   }

}
