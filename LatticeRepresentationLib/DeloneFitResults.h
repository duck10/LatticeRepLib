#ifndef DELONEFITRESULTS_H
#define DELONEFITRESULTS_H

#include <iostream>
#include <string>

#include "MatS6.h"
#include "S6.h"


class DeloneFitResults {
public:
   friend std::ostream& operator<< (std::ostream&, const DeloneFitResults&);

   DeloneFitResults() {}
   DeloneFitResults(const double fit, const S6& bestApprox, const S6& perpv, const MatS6& toCanon);
   void SetZscore(const double score) { m_zscore = score; }
   void SetLatticeType(const std::string& s) { m_latticeType = s; }
   void SetDeloneType(const std::string& s) { m_DeloneType = s; }
   double GetRawFit(void) const { return m_rawFit; }
   void SetRawFit(const double fit) { m_rawFit = fit; }
   double GetZscore(void) const { return m_zscore; }
   S6 GetBestFit(void) const { return m_bestFit; }
   S6 GetDifference(void) const { return m_difference; }
   void SetDifference(const S6& s) { m_difference = s; }
   MatS6 GetToCanon(void) const { return m_toCanonicalDeloneType; }
   std::string GetLatticeType(void) const { return m_latticeType; }
   std::string GetDeloneType(void) const { return m_DeloneType; }
   void SetReductionMatrix(const MatS6& matrix) { m_reductionMatrix = matrix; }
   MatS6 GetReductionMatrix() const { return m_reductionMatrix; }
   void SetOriginalInput(const S6& s6) { m_originalInput = s6; }
   S6 GetOriginalInput() const { return m_originalInput; }
   void SetType(const std::string& s) { m_type = s; }
   std::string GetType() const { return m_type; }
   void SetGeneralType(const std::string& s) { m_generalLatticeType = s; }
   std::string GetGeneralType() const { return m_generalLatticeType; }

protected:
   std::string m_latticeType;
   std::string m_generalLatticeType;
   std::string m_DeloneType;
   std::string m_type;
   double m_rawFit;
   double m_zscore;
   S6 m_originalInput;
   S6 m_bestFit;
   S6 m_difference;
   MatS6 m_toCanonicalDeloneType;
   MatS6 m_reductionMatrix;
};

#endif  // DELONEFITRESULTS_H