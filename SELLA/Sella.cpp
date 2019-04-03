#include "Sella.h"


#include "GenerateRandomLattice.h"
#include "LatticeCharacters.h"
#include "LRL_ToString.h"
#include "LRL_MaximaTools.h"
#include "LRL_StringTools.h"
#include "LabeledSellaMatrices.h"
#include "Selling.h"
#include "StoreResults.h"

#include <cfloat>


const std::vector<LabeledSellaMatrices> Sella::projectors = LabeledSellaMatrices::CreateAllPrjs();
const std::vector<LabeledSellaMatrices> Sella::perps = LabeledSellaMatrices::CreateAllPerps();
std::vector <LatticeCharacters> Sella::m_latticeCharacters;


Sella::Sella()
{
   LatticeCharacters lc;
   lc = LatticeCharacters("C1", "cI", "rrr rrr");
   lc.AddToLabeledSellaMatricesForDeloneType(LabeledSellaMatrices::CreatePerps_C1());
   m_latticeCharacters.push_back(LatticeCharacters("C1", "cI", "rrr rrr"));
   m_latticeCharacters.push_back(LatticeCharacters("C3", "cF", "rr0 rr0"));
   m_latticeCharacters.push_back(LatticeCharacters("C5", "cP", "000 rrr"));
   m_latticeCharacters.push_back(LatticeCharacters("T1", "tI", "rrs rrs"));
   m_latticeCharacters.push_back(LatticeCharacters("T2", "tI", "rr0 rrs"));
   m_latticeCharacters.push_back(LatticeCharacters("T5", "tP", "000 rrs"));
   m_latticeCharacters.push_back(LatticeCharacters("R1", "rP", "rrr sss"));
   m_latticeCharacters.push_back(LatticeCharacters("R3", "rP", "r0r sr0"));
   m_latticeCharacters.push_back(LatticeCharacters("O1A", "oF", "rrs rrt"));
   m_latticeCharacters.push_back(LatticeCharacters("O1B", "oI", "rrs rrs"));
   m_latticeCharacters.push_back(LatticeCharacters("O2", "oI", "rs0 sr0"));
   m_latticeCharacters.push_back(LatticeCharacters("O3", "oI", "rs0 rs0"));
   m_latticeCharacters.push_back(LatticeCharacters("O4", "oP", "00r sst"));
   m_latticeCharacters.push_back(LatticeCharacters("O5", "oP", "000 rst"));
   m_latticeCharacters.push_back(LatticeCharacters("M1A", "mC", "rrs ttu"));
   m_latticeCharacters.push_back(LatticeCharacters("M1B", "mC", "rst rsu"));
   m_latticeCharacters.push_back(LatticeCharacters("M2A", "mC", "rs0 rst"));
   m_latticeCharacters.push_back(LatticeCharacters("M2B", "mC", "rs0 stu"));
   m_latticeCharacters.push_back(LatticeCharacters("M3", "mC", "rs0 ts0"));
   m_latticeCharacters.push_back(LatticeCharacters("M4", "mP", "00r stu"));
   m_latticeCharacters.push_back(LatticeCharacters("A1", "aP", "rst uvw"));
   m_latticeCharacters.push_back(LatticeCharacters("A2", "aP", "rs0 tuv"));
   m_latticeCharacters.push_back(LatticeCharacters("A3", "aP", "rs0 tu0"));
   m_latticeCharacters.push_back(LatticeCharacters("H4", "hP", "00r rrs"));
}

static int seed = 19191;
double Sella::TestOneType(const std::string& label, const S6 &s6, const std::vector<MatS6>& vm) {
   double best = DBL_MAX;
   unsigned long bestIndex;
   S6 bestS6;
   const double s6norm = s6.norm();
   for (unsigned long i = 0; i < vm.size(); ++i) {
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
      for (unsigned long i = 0; i < perps.size(); ++i) {
         const std::string label = perps[i].GetLabel();
         const double best = TestOneType(label, out, perps[i].GetMatrices());
         v.push_back(std::make_pair(label, best));
      }
   }
   return v;
}


std::pair<std::string, double>  Sella::GetBestFitForCrystalSystem(const std::string& type, const S6& s6) {
   std::string bestLabel;
   double bestFit = DBL_MAX;
   const char crystSystem = (LRL_StringTools::strToupper(type))[0];

   for (unsigned long i = 0; i < perps.size(); ++i) {
      const std::string currentLabel = perps[i].GetLabel();
      if (crystSystem == currentLabel[0]) {
         const std::vector<MatS6> mats = perps[i].GetMatrices();
         for (unsigned long mv = 0; mv < mats.size(); ++mv) {
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

   for (unsigned long i = 0; i < perps.size(); ++i) {
      const std::string currentLabel = perps[i].GetLabel();
      if (type == currentLabel) {
         const std::vector<MatS6> mats = perps[i].GetMatrices();
         for (unsigned long mv = 0; mv < mats.size(); ++mv) {
            const double fit = (mats[mv] * s6).norm();
            bestFit = std::min(fit, bestFit);
         }
         break; // we are only doing one type
      }
   }
   return bestFit;
}
