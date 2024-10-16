#ifndef LabeledSellaMatricesForDeloneType_H
#define LabeledSellaMatricesForDeloneType_H

#include <string>
#include <vector>

#include "EnhancedS6.h"
#include "S6.h"
#include "LabeledSellaMatrices.h"
#include "MatS6.h"
#include "S6_Ordinals.h"
#include "StoreResults.h"
#include "Transformations.h"

class LabeledDeloneTypeMatrices {
public:
   LabeledDeloneTypeMatrices() {}
   std::vector<LabeledDeloneTypeMatrices>
   ProcessVectorMapToPerpsAndProjectors
   ( const std::map<std::string, std::vector<S6> >& themap);
   std::vector<LabeledDeloneTypeMatrices>
      ProcessVectorMapToPerpsAndProjectors
   ( const std::map<std::string, std::vector<EnhancedS6> >& xfmap);

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
   //static MatS6 ProjectorFromVector(const std::string& label, const S6& s);
   //static MatS6 ProjectorFromVector(const std::string& label,
      //const Transformations& xf);
   static MatS6 ToCanon(const S6& s);
   static MatS6 ToCanon(const S6_Ordinals& s);
   static MatS6 ToCanon(const EnhancedS6& s);
   bool AlreadyHasThisProjector(const MatS6& m, const LabeledDeloneTypeMatrices& lsm) const;

   std::string m_label;
   std::vector<MatS6> m_perps;
   std::vector<MatS6> m_prjs;
   std::vector<MatS6> m_toCanons;

   std::vector<LabeledDeloneTypeMatrices> MapToMatrices(const std::map<std::string, std::vector<EnhancedS6>>& t) const;


   template <typename T>
   std::vector<LabeledDeloneTypeMatrices> MapToMatrices(const T& t) const {
      std::vector<LabeledDeloneTypeMatrices> vlsm;
      static const MatS6 unitM = MatS6().unit();
      for (auto it = t.begin(); it != t.end(); ++it) {
         const auto& p = *it;
         LabeledDeloneTypeMatrices lsm;
         const std::string& label = p.first;
         const auto& type = p.second;
         const auto& typepart = type[0];
         //const auto& mat = typepart.GetTransformMatrix();
         lsm.m_label = label;
         for (size_t i = 0; i < p.second.size(); ++i) {
            const auto s6 = p.second[i].GetVector();
            const MatS6 prj = ProjectorFromVector(label,s6);
            if (!AlreadyHasThisProjector(prj, lsm)) {
               lsm.m_prjs.push_back(ProjectorFromVector(label, p.second[i]));
               lsm.m_perps.push_back(unitM - ProjectorFromVector(label, p.second[i]));
               //MatS6 fromCanon = p.second[i].getTransformMatrix();
               //const int i19191 = 19191;
               //lsm.m_toCanons.push_back(MatS6::Inverse(fromCanon));
               //lsm.m_toCanons.push_back(MatS6::Inverse(p.second[i].getTransformMatrix()));
               // LCA -- NEED TO MAKE ToCanon work for Transformations !!!!!!!!!!!!!!!!!!
            }
         }

         vlsm.push_back(lsm);
      }
      return vlsm;
   }

   template <typename T>
   MatS6 ProjectorFromVector(const std::string& label, const T& t) const {
      MatS6 m;
      m = m.Zero();
      const S6 s = S6::ReZeroScalars(S6(t));

      for (size_t j = 0; j < 6; ++j) {
         for (size_t k = j; k < 6; ++k) {
            const double thisFrac = Fraction(s[k], s);
            m[Index6(j, k)] = (abs(s[j] - s[k]) < 1.0E-5 && abs(s[k]) > 1.0E-5) ? thisFrac : 0.0;
            m[Index6(k, j)] = m[Index6(j, k)];
         }
      }
      return m;
   }

};

#endif  // LabeledSellaMatricesForDeloneType_H
