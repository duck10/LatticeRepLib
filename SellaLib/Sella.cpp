#include "Sella.h"

#include "GenerateRandomLattice.h"
#include "DeloneTypeList.h"
#include "DeloneType.h"
#include "FaceCrossedArrangements.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_MaximaTools.h"
#include "LRL_StringTools.h"
#include "LabeledSellaMatrices.h"
#include "LabeledDeloneTypeMatrices.h"
#include "Selling.h"

#include <cfloat>
#include <iostream>

Sella::Sella(const bool /*useDynamicMatrices*/ /*=false*/)
   : m_projectors(LabeledSellaMatrices::projectors)
   , m_perps(LabeledSellaMatrices::perps)
{
   // Dynamic matrices injected externally via SetPerps()/SetProjectors()
   // from CmdSella, which has access to SellaBuild
}




static int seed = 19191;
double Sella::TestOneType(const std::string& label, const S6& s6, const std::vector<MatS6>& vm) {
   double best = DBL_MAX;
   size_t bestIndex;
   S6 bestS6;
   const double s6norm = s6.norm();
   for (size_t i = 0; i < vm.size(); ++i) {
      S6 prp = vm[i] * s6;
      if (best > prp.norm()) {
         best = prp.norm();
         bestIndex = i;
         bestS6 = vm[i] * s6;
      }
   }
   return best;
}

std::vector<std::pair<std::string, double> > Sella::GetVectorOfFits(const S6& s6) {
   S6 out;
   std::vector<std::pair<std::string, double> > v;
   const double s6norm = s6.norm();
   if (!s6.IsValid() && s6norm > 1.0E-4) return v;

   const bool b = Selling::Reduce(s6, out);
   if (b) {
      const double n1 = s6.norm();
      const double n2 = out.norm();
      // CHANGED: use m_perps instead of LabeledSellaMatrices::perps
      for (size_t i = 0; i < m_perps.size(); ++i) {
         const std::string label = m_perps[i].GetLabel();
         const double best = TestOneType(label, out, m_perps[i].GetMatrices());
         v.emplace_back(std::make_pair(label, best));
      }
   }
   return v;
}

std::pair<std::string, double>  Sella::GetBestFitForCrystalSystem(const std::string& type, const S6& s6) {
   std::string bestLabel;
   double bestFit = DBL_MAX;
   const char crystSystem = (LRL_StringTools::strToupper(type))[0];

   // CHANGED: use m_perps instead of LabeledSellaMatrices::perps
   for (size_t i = 0; i < m_perps.size(); ++i) {
      const std::string currentLabel = m_perps[i].GetLabel();
      if (crystSystem == currentLabel[0]) {
         const std::vector<MatS6> mats = m_perps[i].GetMatrices();
         for (size_t mv = 0; mv < mats.size(); ++mv) {
            const S6 projected = mats[mv] * s6;
            //std::cout << s6 << std::endl;
            //std::cout << projected << std::endl;
            //std::cout << LRL_MaximaTools::MaximaFromMat(mats[0]) << std::endl;
            //std::cout << LRL_MaximaTools::MaximaFromMat(projectors[i].GetMatrices()[0]) << std::endl;
            const double fit = projected.norm();
            if (fit < bestFit) {
               bestFit = fit;
               bestLabel = currentLabel;
            }
         }
      }
   }
   if (bestFit < 1.0E-10) bestFit = 0.0;
   return std::make_pair(bestLabel, bestFit);
}

double Sella::GetFitForDeloneType(const std::string& type, const S6& s6) {
   double bestFit = DBL_MAX;

   // CHANGED: use m_perps instead of LabeledSellaMatrices::perps
   for (size_t i = 0; i < m_perps.size(); ++i) {
      const std::string currentLabel = m_perps[i].GetLabel();
      if (type == currentLabel) {
         const std::vector<MatS6> mats = m_perps[i].GetMatrices();
         for (size_t mv = 0; mv < mats.size(); ++mv) {
            const double fit = (mats[mv] * s6).norm();
            bestFit = std::min(fit, bestFit);
         }
         break; // we are only doing one type
      }
   }
   return bestFit;
}

// Appends the FaceCrossed projectors/perps for types that have them (O1B, O2
// so far). The lists are built once and cached, since SellaFitXXXXXX is
// called for every remediation candidate.
// Appends the missing face arrangements (white paper v4, Appendix B) for
// this type; does nothing for the 13 types that need none.
static void AppendFaceCrossed(const std::string& name,
   std::vector<MatS6>& prjs, std::vector<MatS6>& perps) {
   const FaceCrossedMatrices& fc = GetFaceCrossedMatrices(name);
   prjs.insert(prjs.end(), fc.prjs.begin(), fc.prjs.end());
   perps.insert(perps.end(), fc.perps.begin(), fc.perps.end());
}

// ClampToCone -- sets every positive component of a projected vector to 0.
// All members of a letter group share one projected value, so this is
// min(0, group mean) per group: the nearest point of the arrangement's
// subspace that lies inside the reduced cone (s_i <= 0). Returns true if
// anything changed. A Selling-reduced input never changes; only
// non-reduced inputs (Remediation's candidates, which may lie up to
// 0.10 * |s6| outside the cone) can.
static bool ClampToCone(S6& v) {
   bool changed = false;
   for (size_t i = 0; i < 6; ++i) {
      if (v[i] > 0.0) {
         v[i] = 0.0;
         changed = true;
      }
   }
   return changed;
}

// SellaFitXXXXXX -- best fit of s6 to one Delone type over the type's
// projector list (plus the face arrangements when s_useFaceCrossed).
// With s_useConeFit (CONEFIT, default on, 2026-11-06) each projection is
// clamped to the reduced cone before the distance is taken, so every
// reported fit is the distance to a genuine reduced form of the type.
// Without it, a candidate outside the cone could project to a point
// outside the cone -- a vector with the type's zero/equality pattern that
// is not a lattice of that type -- and report a fit that is too small.
// When nothing is clamped the fit is perps[i] * s6, exactly as before.
DeloneFitResults Sella::SellaFitXXXXXX(
   const std::shared_ptr<GenerateDeloneBase>& sptype,
   const S6& s6) {

   const std::string name = sptype->GetName();
   size_t nBest = 0;
   double bestFit = DBL_MAX;
   std::vector<MatS6> perps = sptype->GetSellaPerps();
   std::vector<MatS6> prjs = sptype->GetSellaProjectors();
   if (s_useFaceCrossed) AppendFaceCrossed(name, prjs, perps);

   if (prjs.size() != perps.size()) {
      std::cout << name << "  there are not the same number of lattice types in prjs"
         "as in perps  " << __FILE__ << std::endl;
      throw("prj count != perp count");
   }

   int errors = 0;

   for (size_t i = 0; i < prjs.size(); ++i) {
      if (prjs.size() != perps.size()) {
         ++errors;
         std::cout << "prjs" << prjs[i].GetName() << " " << prjs.size() << std::endl;
         std::cout << "perps" << perps[i].GetName() << " " << perps.size() << std::endl << std::endl;
      }
   }

   if (errors != 0) throw("prjs[i] count != perps[i] count");

   S6 bestv = prjs[0] * s6;
   S6 smallestPerp = perps[0] * s6;
   for (size_t i = 0; i < perps.size(); ++i) {
      S6 projected = prjs[i] * s6;
      const bool clamped = s_useConeFit && ClampToCone(projected);
      const S6 perpv = clamped ? S6(s6 - projected) : S6(perps[i] * s6);
      const double testFit = perpv.norm();

      if (bestFit > testFit && LRL_Cell_Degrees(projected).IsValid()) {
         nBest = i;
         bestFit = testFit;
         bestv = projected;
         smallestPerp = perpv;
      }
   }
   if (bestFit < 1.0E-8) bestFit = 0.0;
   const MatS6 toCanonicalDeloneType/* = sptypes[nBest]->GetToCanon(nBest)*/;

   DeloneFitResults temp(bestFit, bestv, smallestPerp, MatS6().unit());
   temp.SetLatticeType(sptype->GetBravaisLatticeGeneral());
   temp.SetDeloneType(sptype->GetBravaisType());
   temp.SetGeneralType(name);
   temp.SetBavaisType(sptype->GetBravaisType());

   return temp;
}

double Sella::Zscore(const S6& s6, const S6& sigmas, const MatS6& reductionMatrix)
{
   const double zscore = s6.Norm() / (MatS6::Inverse(reductionMatrix) * sigmas).Norm();
   return (zscore < 1.0E-6) ? 0.0 : zscore;
}

std::vector<DeloneFitResults> Sella::SellaFit(
   const std::string& selectBravaisCase,
   const S6& s6,
   const S6& errors,
   const MatS6& reductionMatrix) {

   std::vector<DeloneFitResults> vDeloneFitResults;
   static const std::vector<std::shared_ptr<GenerateDeloneBase> > sptypes =
      GenerateDeloneBase().Select(selectBravaisCase);

   for (size_t i = 0; i < sptypes.size(); ++i) {
      DeloneFitResults fit = SellaFitXXXXXX(sptypes[i], s6);
      //std::cout << "; best fit in SellaFit S6 " << fit.GetBestFit() << "   " << fit.GetRawFit() << " " << sptypes[i]->GetName() << std::endl;
      const double zscore = Zscore(s6 - fit.GetBestFit(), errors, reductionMatrix) * sqrt(sptypes[i]->GetFreeParams());
      fit.SetZscore(zscore);
      fit.SetLatticeType(sptypes[i]->GetBravaisType());
      fit.SetDeloneType(sptypes[i]->GetBravaisType());
      fit.SetReductionMatrix(reductionMatrix);
      fit.SetBavaisType(sptypes[i]->GetBravaisType());
      fit.SetGeneralType(sptypes[i]->GetBravaisLatticeGeneral());
      fit.SetGeneralType(sptypes[i]->GetName());

      fit.SetDifference(s6 - fit.GetBestFit());
      fit.SetOriginalInput(s6);
      vDeloneFitResults.push_back(fit);
   }
   return vDeloneFitResults;
}
