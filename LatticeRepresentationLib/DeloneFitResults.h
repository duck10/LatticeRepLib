#ifndef DELONEFITRESULTS_H
#define DELONEFITRESULTS_H

#include <iostream>
#include <string>

#include "MatS6.h"
#include "S6.h"


class DeloneFitResults {
public:
   friend std::ostream& operator<< (std::ostream& os, const DeloneFitResults& dfr) {
      os << "; DeloneFitResults" << std::endl;
      os << ";   general type    " << dfr.m_generalLatticeType << std::endl;
      os << ";   Bravais type    " << dfr.m_type << std::endl;
      os << ";   Delone type     " << dfr.m_DeloneType << std::endl;
      os << ";   lattice type    " << dfr.m_latticeType << std::endl;
      os << ";   raw fit         " << dfr.m_rawFit << std::endl;
      os << ";   Zscore          " << dfr.m_zscore << std::endl;
      os << ";   best fit S6     " << dfr.m_bestS6Fit << std::endl;
      os << ";   difference      " << dfr.m_S6Difference << std::endl;
      os << ";   original input  " << dfr.m_originalS6Input << std::endl;
      os << ";   winning cand.   " << dfr.m_winningCandidate << std::endl;
      os << ";   reduction mat   " << dfr.m_reductionMatrix << std::endl;
      os << ";   toCanon mat     " << dfr.m_toCanonicalDeloneType << std::endl;
      return os;
   }

   DeloneFitResults() {}
   DeloneFitResults(const double fit, const S6& bestApprox, const S6& perpv, const MatS6& toCanon);
   void SetZscore(const double score) { m_zscore = score; }
   void SetLatticeType(const std::string& s) { m_latticeType = s; }
   void SetDeloneType(const std::string& s) { m_DeloneType = s; }
   double GetRawFit(void) const { return m_rawFit; }
   void SetRawFit(const double fit) { m_rawFit = fit; }
   double GetZscore(void) const { return m_zscore; }
   S6 GetBestFit(void) const { return m_bestS6Fit; }
   S6 GetDifference(void) const { return m_S6Difference; }
   void SetDifference(const S6& s) { m_S6Difference = s; }
   MatS6 GetToCanon(void) const { return m_toCanonicalDeloneType; }
   std::string GetLatticeType(void) const { return m_latticeType; }
   std::string GetDeloneType(void) const { return m_DeloneType; }
   void SetReductionMatrix(const MatS6& matrix) { m_reductionMatrix = matrix; }
   MatS6 GetReductionMatrix() const { return m_reductionMatrix; }
   void SetOriginalInput(const S6& s6) { m_originalS6Input = s6; }
   S6 GetOriginalInput() const { return m_originalS6Input; }
   void SetBavaisType(const std::string& s) { m_type = s; }
   std::string GetBravaisType() const { return m_type; }
   void SetGeneralType(const std::string& s) { m_generalLatticeType = s; }
   std::string GetGeneralType() const { return m_generalLatticeType; }
   void SetWinningCandidate(const S6& s6) { m_winningCandidate = s6; }
   S6 GetWinningCandidate() const { return m_winningCandidate; }

protected:
   std::string m_latticeType;
   std::string m_generalLatticeType;
   std::string m_DeloneType;
   std::string m_type;
   double m_rawFit;
   double m_zscore;
   S6 m_originalS6Input;
   S6 m_winningCandidate;
   S6 m_bestS6Fit;
   S6 m_S6Difference;
   MatS6 m_toCanonicalDeloneType;
   MatS6 m_reductionMatrix;
};

#endif  // DELONEFITRESULTS_H