#ifndef LABELEDVECTORBOUNDARIES_H
#define LABELEDVECTORBOUNDARIES_H

#include "D7_ClassifySubboundaries.h"
#include "MaximaTools.h"

template<typename TVEC, typename TMAT>
class LabeledVectorBoundaries {
public:
   friend std::ostream& operator<< (std::ostream& o, const LabeledVectorBoundaries& lvb) {
      const std::pair<std::string, std::string> insideBoundaries =
         SummarizeBoundaries(lvb.m_inside, lvb.m_windowSize);
      const std::pair<std::string, std::string> outsideBoundaries =
         SummarizeBoundaries(TVEC(Delone::Reduce(S6(lvb.m_outside))), lvb.m_windowSize);


      o << lvb.m_strMatrix << std::endl;
      o << MaximaTools::MaximaFromString(LRL_ToString(lvb.m_inside)) << "  " << insideBoundaries.second << std::endl;
      o << lvb.m_outside << "  " << outsideBoundaries.second << std::endl;


      return o;
   }

   static std::pair<std::string, std::string> SummarizeBoundaries(const TVEC& v, const double delta) {
      const std::vector<std::pair<std::string, std::string> > bounds = v.ClassifyVector(delta);
      std::string boundList;
      std::string subboundList;
      for (unsigned long i = 0; i < bounds.size(); ++i) {
         boundList += bounds[i].first + " ";
         subboundList += bounds[i].second + " ";
      }
      return std::make_pair(boundList, subboundList);
   }

   LabeledVectorBoundaries(const TVEC& inside, const TVEC& outside, const TMAT& mat, const double windowSize)
      : m_inside(inside)
      , m_outside(outside)
      , m_strMatrix(MaximaTools::MaximaFromMat(mat))
      , m_windowSize(windowSize)
   { }

   std::string GetName() const {
      return m_strMatrix;
   }

   TVEC GetOutside() const { return m_outside; }

private:
   const TVEC m_inside;
   const TVEC m_outside;
   const std::string m_strMatrix;
   const double m_windowSize;
};

#endif  // LABELEDVECTORBOUNDARIES_H