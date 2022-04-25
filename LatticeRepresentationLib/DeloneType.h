#ifndef DELONETYPE_H
#define DELONETYPE_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "MatS6.h"
#include "LabeledSellaMatrices.h"
#include "LabeledDeloneTypeMatrices.h"
#include "MatricesForOneDeloneType.h"


class DeloneFitResults {
public:
   friend std::ostream& operator<< (std::ostream&, const DeloneFitResults&);

   DeloneFitResults(const double fit, const S6& bestApprox, const S6& perpv, const MatS6& toCanon);
   void SetZscore(const double score) { m_zscore = score; }
   void SetLatticeType(const std::string& s) { m_latticeType = s; }
   double GetRawFit(void) const { return m_rawFit; }
   double GetZscore(void) const { return m_zscore; }
   S6 GetBestFit(void) const { return m_bestFit; }
   S6 GetDifference(void) const { return m_difference; }
   void SetDifference(const S6& s) { m_difference = s; }
   MatS6 GetToCanon(void) const { return m_toCanonicalDeloneType; }
   std::string GetLatticeType(void) const { return m_latticeType; }
   void SetReductionMatrix(const MatS6& matrix) { m_reductionMatrix = matrix; }
   MatS6 GetReductionMatrix() const { return m_reductionMatrix; }
   void SetOriginalInput(const S6& s6) { m_originalInput = s6; }
   S6 GetOriginalInput() const {return m_originalInput;}
   void SetType(const std::string& s) { m_type = s; }
   std::string GetType() const { return m_type; }
   void SetGeneralType(const std::string& s) { m_generalLatticeType = s; }
   std::string GetGeneralType() const { return m_generalLatticeType; }

protected:
   std::string m_latticeType;
   std::string m_generalLatticeType;
   std::string m_type;
   double m_rawFit;
   double m_zscore;
   S6 m_originalInput;
   S6 m_bestFit;
   S6 m_difference;
   MatS6 m_toCanonicalDeloneType;
   MatS6 m_reductionMatrix;
};

class DeloneType {
public:
   friend std::ostream& operator<< (std::ostream&, const DeloneType&);

   DeloneType() {}
   //DeloneType(const std::string& deloneType, const std::string& bravaisType, const std::string& character, const std::pair<std::string, MatS6 >&);
   DeloneType(
      const std::string& deloneType,
      const std::string& bravaisType,
      const std::string& character,
      const std::string& toCentered_E3,
      const MatS6& ToCentered,
      const int m_freePsrams,
      const MatricesForOneDeloneType& vm);

   DeloneFitResults GetFit(const std::string& name, const S6& s6, const MatS6& reductionMatrix) const;

   std::string GetName(void) const { return m_deloneName; }
   std::string GetBravaisType(void) const { return m_bravaisType; }
   std::string GetCharacter(void) const { return m_character; }
   std::string GetToCenteredString(void) const { return m_toCentered_E3; }
   MatS6 GetToCentered(void) const { return m_toCentered_S6; }
   MatricesForOneDeloneType GetFamilyOfMattrices() const  { return m_matrices; }
   std::vector<MatS6> GetPrjs( void ) const { return m_matrices.GetPrjs( ); }
   std::vector<MatS6> GetPerps( void ) const { return m_matrices.GetPerps( ); }

private:
   std::string m_deloneName;
   std::string m_bravaisType;
   std::string m_character;
   std::string m_toCentered_E3;
   MatS6       m_toCentered_S6;
   int m_freePsrams;

   MatricesForOneDeloneType m_matrices;


};


#endif  // DELONETYPE_H