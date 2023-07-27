#include "FollowerPathGenerator.h"

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
