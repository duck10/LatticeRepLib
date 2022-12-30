#ifndef LabeledSellaMatricesForDeloneType_H
#define LabeledSellaMatricesForDeloneType_H

#include <string>
#include <vector>

#include "LabeledSellaMatrices.h"
#include "MatS6.h"
#include "S6_Ordinals.h"
#include "StoreResults.h"

class LabeledDeloneTypeMatrices {
public:
   LabeledDeloneTypeMatrices(){}
   std::vector<LabeledDeloneTypeMatrices>
      ProcessVectorMapToPerpsAndProjectors
      (const std::map<std::string, std::vector<S6> >& themap);

   size_t size() const { return m_perps.size(); }

   void WriteSellaMatrices(const std::vector<LabeledDeloneTypeMatrices>& matsForAllDeloneTypes) const;
   //void WriteSellaMatrices(const std::string& functionName, const std::string& label, const std::vector<MatS6>& mat) const;
   void FinalWriteSellaMatrices(const std::vector<LabeledDeloneTypeMatrices>& matsForAllDeloneTypes) const;
   std::string FinalWriteSellaMatrices(const std::string& functionName, const std::string& label, const std::vector<MatS6>& mat) const;
   std::string WriteMatrixTypes(const std::string& label,
      const std::string& functionHead,
      const std::string& matrixType,
      const std::vector<MatS6>& vm) const;

   std::string GetLabel(void) const { return m_label; }
   std::vector<MatS6> GetPrj(const std::string& s);
   std::vector<MatS6> GetPerp(const std::string& s);
   std::vector<MatS6> GetTocanon(const std::string& s);

private:

   static double Fraction(const double d, const S6& s);
   static size_t Index6(const size_t i, const size_t j);
   static MatS6 ProjectorFromVector(const std::string& label, const S6& s);
   static MatS6 ToCanon(const S6& s);
   static MatS6 ToCanon(const S6_Ordinals& s);
   bool AlreadyHasThisProjector(const MatS6& m, const LabeledDeloneTypeMatrices& lsm) const;

   std::string m_label;
   std::vector<MatS6> m_perps;
   std::vector<MatS6> m_prjs;
   std::vector<MatS6> m_toCanons;
};

#endif  // LabeledSellaMatricesForDeloneType_H