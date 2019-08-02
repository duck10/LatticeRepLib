#include "Sella.h"


#include "GenerateRandomLattice.h"
#include "DeloneTypeList.h"
#include "DeloneType.h"
#include "LRL_ToString.h"
#include "LRL_MaximaTools.h"
#include "LRL_StringTools.h"
#include "LabeledSellaMatrices.h"
#include "LabeledDeloneTypeMatrices.h"
#include "Selling.h"
#include "StoreResults.h"

#include <cfloat>


std::vector <DeloneType> Sella::m_latticeCharacters;

Sella::Sella()
{
   LabeledDeloneTypeMatrices labeledPerType;
   DeloneType dt;
   std::string label;
   label = "C1";
}

static int seed = 19191;
double Sella::TestOneType(const std::string& label, const S6 &s6, const std::vector<MatS6>& vm) {
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
      for (size_t i = 0; i < LabeledSellaMatrices::perps.size(); ++i) {
         const std::string label = LabeledSellaMatrices::perps[i].GetLabel();
         const double best = TestOneType(label, out, LabeledSellaMatrices::perps[i].GetMatrices());
         v.push_back(std::make_pair(label, best));
      }
   }
   return v;
}


std::pair<std::string, double>  Sella::GetBestFitForCrystalSystem(const std::string& type, const S6& s6) {
   std::string bestLabel;
   double bestFit = DBL_MAX;
   const char crystSystem = (LRL_StringTools::strToupper(type))[0];

   for (size_t i = 0; i < LabeledSellaMatrices::perps.size(); ++i) {
      const std::string currentLabel = LabeledSellaMatrices::perps[i].GetLabel();
      if (crystSystem == currentLabel[0]) {
         const std::vector<MatS6> mats = LabeledSellaMatrices::perps[i].GetMatrices();
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

   for (size_t i = 0; i < LabeledSellaMatrices::perps.size(); ++i) {
      const std::string currentLabel = LabeledSellaMatrices::perps[i].GetLabel();
      if (type == currentLabel) {
         const std::vector<MatS6> mats = LabeledSellaMatrices::perps[i].GetMatrices();
         for (size_t mv = 0; mv < mats.size(); ++mv) {
            const double fit = (mats[mv] * s6).norm();
            bestFit = std::min(fit, bestFit);
         }
         break; // we are only doing one type
      }
   }
   return bestFit;
}
