#include "FollowerConstants.h"
#include "FollowerPathGenerator.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ToString.h"

#include "Selling.h"


FollowerPoint::FollowerPoint(const size_t n, const S6& s1) {
   m_input.push_back(s1);

   S6 reducedProbe;
   S6 reducedNext;
   m_result.clear();
   Selling::Reduce(s1, reducedProbe);
   for (size_t step = 0; step < n; ++step) {
      const double t(double(step) / double(n - 1));
      S6 next = double(1.0 - t) * s1 + t * reducedProbe;
      const bool b2 = Selling::Reduce(next, reducedNext);
      m_result.push_back(std::make_pair(next, reducedProbe));
   }
}

std::pair<std::string, std::string> FollowerPathGenerator::GetLineEnds(const std::vector<std::pair<S6, S6> >& v) const {
   const size_t n = v.size();
   const S6& s1f = v[0].first;
   const S6& s2f = v[0].second;
   const S6& s1s = v[n - 1].first;
   const S6& s2s = v[n - 1].second;
   const std::string s1 = "start, line1 " + LRL_ToString(LRL_Cell_Degrees(s1s), "\n  end, line1, ", LRL_Cell_Degrees(s1f), "\n");
   const std::string s2 = "start, line2 " + LRL_ToString(LRL_Cell_Degrees(s2s), "\n  end, line2, ", LRL_Cell_Degrees(s2f), "\n");
   return std::make_pair(s1, s2);
}

std::unique_ptr<FollowerPathGenerator> FollowerPathGenerator::PointerToFollowerPathType(
   const CellInputData& cell1, const CellInputData& cell2, const CellInputData& cell3) {
   std::unique_ptr<FollowerPathGenerator> fpg;
   const size_t npoints = FollowerConstants::globalStepsPerFrame;

   if (FollowerConstants::globalFollowerMode == FollowerConstants::globalTriangle) {
      std::unique_ptr<FollowerTriangle> fpgT(new FollowerTriangle(npoints, cell1.GetCell(), cell2.GetCell(), cell3.GetCell()));
      fpg = std::move(fpgT);
   }
   else if (FollowerConstants::globalFollowerMode == FollowerConstants::globalChord3) {
      std::unique_ptr<FollowerChord3> fpgC3(new FollowerChord3(npoints, cell1.GetCell(), cell2.GetCell(), cell3.GetCell()));
      fpg = std::move(fpgC3);
   }
   else if (FollowerConstants::globalFollowerMode == FollowerConstants::globalChord) {
      std::unique_ptr<FollowerChord> fpgC(new FollowerChord(npoints, cell1.GetCell(), cell2.GetCell()));
      fpg = std::move(fpgC);
   }
   else if (FollowerConstants::globalFollowerMode == FollowerConstants::globalLine) {
      std::unique_ptr<FollowerLine> fpgL(new FollowerLine(npoints, cell1.GetCell(), cell2.GetCell()));
      fpg = std::move(fpgL);
   }
   else {// globalSinglePoint
      std::unique_ptr<FollowerPoint> fpgP(new FollowerPoint(npoints, cell1.GetCell()));
      fpg = std::move(fpgP);
   }
   fpg->SetName(fpg->GetName());

   return fpg;
}

std::string  FollowerPathGenerator::GetFirstAndLastPointsAsString() const {
   const std::string name = GetName();
   const std::vector<std::pair<S6, S6> > path = GetPath();
   const std::pair<std::string, std::string> sp = GetLineEnds(path);
   return sp.first + sp.second;
}


FollowerLine::FollowerLine(const size_t n, const S6& s1, const S6& s2) {
   m_input.push_back(s1);
   m_input.push_back(s2);

   m_result = GenerateFollowerPointList(n, s1, s2);
}

FollowerChord::FollowerChord(const size_t n, const S6& s1, const S6& s2) {
   m_input.push_back(s1);
   m_input.push_back(s2);

   const S6 midpoint = (s2 + s1) / 2.0;
   const S6 pointToPointDelta = s1 - midpoint;
   S6 endpoint;
   const bool b = Selling::Reduce(midpoint, endpoint);
   std::vector<std::pair<S6, S6> > midlist = GenerateFollowerPointList(n, midpoint, endpoint);
   m_result.clear();
   for (size_t i = 0; i < midlist.size(); ++i) {
      m_result.push_back(std::make_pair(midlist[i].first + pointToPointDelta, midlist[i].first - pointToPointDelta));
   }
}

FollowerChord3::FollowerChord3(const size_t n, const S6& s1, const S6& s2, const S6& s3) {
   m_input.push_back(s1);
   m_input.push_back(s2);
   m_input.push_back(s3);
   const S6 midpoint = (s2 + s1) / 2.0;
   const S6 pointToPointDelta = (s2 - s1) / 2.0;
   const std::vector<std::pair<S6, S6> > midlist = GenerateFollowerPointList(n, midpoint, s3);
   m_result.clear();
   for (size_t i = 0; i < midlist.size(); ++i) {
      m_result.push_back(std::make_pair(midlist[i].first + pointToPointDelta, midlist[i].first - pointToPointDelta));
   }
}

FollowerTriangle::FollowerTriangle(const size_t n, const S6& s1, const S6& s2, const S6& s3) {
   m_input.push_back(s1);
   m_input.push_back(s2);
   m_input.push_back(s3);

   S6 focus3;
   const bool b3 = Selling::Reduce(s3, focus3);
   std::vector<std::pair<S6, S6> >ptlist1 = GenerateFollowerPointList(n, s1, focus3);
   std::vector<std::pair<S6, S6> >ptlist2 = GenerateFollowerPointList(n, s2, focus3);
   m_result.clear();
   for (size_t i = 0; i < ptlist1.size(); ++i) {
      m_result.push_back(std::make_pair(ptlist1[i].first, ptlist2[i].first));
   }
}

std::vector<std::pair<S6, S6> > FollowerPathGenerator::GenerateFollowerPointList(const size_t n, const S6& s1, const S6& s2) {
   std::vector<std::pair<S6, S6> > result;
   const S6 pathDelta = (s2 - s1) / double(n - 1);

   for (size_t i = 0; i < n; ++i) {
      const S6 next = s1 + double(i) * pathDelta;
      result.push_back(std::make_pair(next, s2));
   }
   return result;
}

std::string FollowerPathGenerator::GetName() const { return "FollowerPathGenerator"; }

void FollowerPathGenerator::OutputPath()const {
   std::cout << GetName() << " input size " << GetInput().size() << std::endl;
   auto path = GetPath();
   for (size_t i = 0; i < size(); ++i) {
      std::cout << path[i].first << "   " << path[i].second << std::endl;
   }
   std::cout << std::endl;
}

std::ostream& operator<< (std::ostream& o, const FollowerPathGenerator& dt) {
   o << dt.GetName() << " input size " << dt.GetInput().size() << std::endl;
   const std::vector<S6> inputList = dt.GetInput();
   for (size_t i = 0; i < inputList.size(); ++i) {
      o << '\t' << inputList[i] << std::endl;
   }

   o << "   Output" << std::endl;
   const std::vector<std::pair<S6,S6> > path = dt.GetPath();
   for (size_t i = 0; i < dt.size(); ++i) {
      o << '\t' << path[i].first << "   " << path[i].second << std::endl;
   }

   return o;
}
