#include <cstddef>
#include <iomanip>
#include <iostream>

#include "DC.h"
#include "Generated_IT_Types.h"
#include"LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "G6.h"
#include "NCDist.h"
#include "Niggli.h"
#include "Selling.h"
#include "StoreResults.h"
#include "svd.h"

StoreResults<std::string, std::string> store(5);
const size_t g_maxgen = 1000000;
std::vector<G6> samples;

const bool g_generateByPerturbation = false;

void GenerateSamplesAndPerturbations(const size_t n) {
   const G6 oneSample = G6().randDeloneUnreduced();
   samples.push_back(oneSample);
   for (size_t i = 0; i < n; ++i) {
      if (g_generateByPerturbation) {
         if ( i==0) std::cout <<"Generate by perturbation" << std::endl;
         samples.push_back(IT_Lat_Char_Base::GeneratePerturbation(oneSample, 0.1));
      }
      else {
         if (i == 0) std::cout << "Generate by random cells" << std::endl;
         const G6 newSample = G6().rand();
         samples.push_back(newSample);
      }
   }
}

bool IsClose(const double a, const double b) {
   return abs(a - b) < 1.0E-7;
}

void DC_SVD(const std::unique_ptr<IT_Lat_Char_Base>& vfpg, const std::vector<G6>& cells) {

   if (cells.empty()) {
      store.Store(vfpg->m_ITnumber + " (" + vfpg->m_name + ")", "No valid cases found");
      return;
   }
   std::vector<std::vector<double> > a;
   std::vector<std::vector<double> > v;
   std::vector<double> w;

   for (size_t i = 0; i < cells.size(); ++i) {
      const std::vector<double> d = DC(cells[i]).GetVector();
      a.push_back(d);
   }

   w.resize(a[0].size());
   v.resize(a.size());

   svdcmp(a, w, v);

   std::vector<std::pair<double, std::vector<double> > > eigenResults;
   std::vector<double> vscalars;
   for (size_t i = 0; i < v.size(); ++i) {
      vscalars.clear();
      for (size_t k = 0; k < v.size(); ++k) {
         vscalars.push_back(v[k][i]);
      }
      eigenResults.push_back(std::make_pair(w[i], vscalars));

   }

   std::sort(eigenResults.begin(), eigenResults.end());
   std::string key = vfpg->m_ITnumber + " (" + vfpg->m_name + ")";
   std::string data = vfpg->m_ITnumber + " " + vfpg->m_name +
      "  " + vfpg->m_latticeType + " " + vfpg->m_G6_character + "  " + vfpg->m_DC7_character + "\n";
   const double test = eigenResults[0].second[6];
   data += "largest eigenvalue " + LRL_ToString(eigenResults[v.size() - 1].first) + " vector =  "
     + LRL_ToString(eigenResults[v.size() - 1].second);
   data += LRL_ToString("\n", "next eigenvalue ", eigenResults[v.size() - 2].first, "\n");

   store.Store(key, data);
   const size_t maxData = store.GetNmax();
   for (size_t i = 0; i < cells.size(); ++i) {
      if (store.GetItemCount(key) <3) {
         const size_t n = store.GetItemCount(key);
         store.Store(key, LRL_ToString(cells[i], "\n", DC(cells[i]), "\n"));
      }
      else {
         store.Store(key, LRL_ToString(cells[i]));
      }
   }
}

void CreateListOfTypes(std::vector<std::unique_ptr<IT_Lat_Char_Base> >& vfpg) {
   std::unique_ptr<IT_Lat_Char_Base> fpg;

   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_1)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_2)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_3)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_4a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_4b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_5)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_6a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_6b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_7a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_7b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_8a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_8b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_8c)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_8d)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_8e)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_8f)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_9a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_9b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_9c)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_10a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_10b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_11a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_11b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_12a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_12b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_13a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_13b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_14a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_14b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_15a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_15b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_16a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_16b)));
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_17))); NA
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_18a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_18b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_19a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_19b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_20)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_21)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_22)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_23a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_23b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_24)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_25)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_26a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_26b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_26c)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_26d)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_27)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_28)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_29)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_30)));
   // IT31 is triclinic
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_32a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_32b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_33a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_33b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_33c)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_34a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_34b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_34c)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_34d)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_34e)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_35a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_35b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_35c)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_35d)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_36a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_36b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_36c)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_36d)));
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_37))); NA
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_38a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_38b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_38c)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_38d)));
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_39))); NA
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_40a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_40b)));
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_41))); NA
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_42))); NA
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_43))); NA
   // IT44 is triclinic


}

void CreateListOfSelectedTypes(std::vector<std::unique_ptr<IT_Lat_Char_Base> >& vfpg) {
   std::unique_ptr<IT_Lat_Char_Base> fpg;


   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_4b)));
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_6a)));
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_6b)));
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_8c)));
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_8d)));
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_8f)));
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_16a)));
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_36c)));
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_37))); NA
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_39))); NA
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_41))); NA
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_42))); NA
   //vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_43))); NA
   // IT44 is triclinic


}

std::vector<G6> Reduce(const std::unique_ptr<IT_Lat_Char_Base>& vfpg, const std::vector<G6>& g6) {
   std::vector<G6> output;
   for (size_t i = 0; i < g6.size(); ++i) {
      const G6 projected = vfpg->m_projector * g6[i];
      G6 gout;
      const bool b = Niggli::Reduce(projected, gout);
      const bool bproj = gout[3] > 0 && gout[4] > 0 && gout[5] > 0;
      if ( b && (projected - gout).Norm() < 1.0E-5 && vfpg->IsMember(gout) && gout.IsValid())
         output.push_back(gout);
   }
   return output;
}

void Generate(const std::vector<std::unique_ptr<IT_Lat_Char_Base> >& vfpg) {
   for (size_t i = 0; i < vfpg.size(); ++i) {
      const std::vector<G6> cells = Reduce(vfpg[i], samples);
      DC_SVD(vfpg[i], cells);
   }

}

void MonteCarloForOneType(const std::unique_ptr<IT_Lat_Char_Base>& vfpg) {
   StoreResults<double, G6> monte(5);
   StoreResults<double, std::string> DCstore;
   for (size_t i = 0; i < 2000000; ++i) {
      G6 g1 = G6::rand();
      G6 gout;
      const bool b = Niggli::Reduce(g1, gout);
      g1 = vfpg->m_projector * gout;
      g1 *= 100.0 / g1.norm();
      if (!vfpg->IsMember(gout))continue;
      const G6 g2 = IT_Lat_Char_Base::GeneratePerturbation(g1, 0.01);
      const double d = (DC(g1) - DC(g2)).Norm();
      const double gnorm = (g1 - g2).norm() / g1.Norm();
      const double dnorm = d / DC(g1).Norm();
      //std::cout << i << "  " << d << "  " << d / DC(g1).Norm() << std::endl;
      //std::cout << i << "  " << g1.norm() << "  " << (g1-g2).norm() / g1.Norm() << std::endl << std::endl;
      //std::cout << i << "  " << gnorm << "  " << dnorm << "  " << dnorm / gnorm << std::endl;
      const double key = double(int(100.0 * dnorm / gnorm)) / 100.0;
      monte.Store(key, g1);

      const std::string examples = LRL_ToString("g6 ", g1,"\ng6 ",g2,"\n") +
         LRL_ToString("DC7 ", DC(g1), "\nDC7 ", DC(g2), "\n") +
         LRL_ToString("dnorm ", dnorm, "  gnorm ", gnorm, "\n");

      DCstore.Store(key, examples);
   }
   //monte.ShowResults();
   DCstore.ShowResults();
   exit(0);
}


int main()
{
   const bool noMonteCarlo = true;

      std::vector<std::unique_ptr<IT_Lat_Char_Base> > vfpg;
   if (noMonteCarlo) {
      std::cout << "; Generate sample G6 data for search DC7 types" << std::endl;
      CreateListOfTypes(vfpg);
      GenerateSamplesAndPerturbations(g_maxgen);
      Generate(vfpg);
   }
   else {
     CreateListOfSelectedTypes(vfpg);
     std::pair<G6, G6> samples;
     for (size_t i = 0; i < vfpg.size(); ++i) {
        MonteCarloForOneType(vfpg[i]);
     }
   }

   store.ShowResultsByKeyAscending();

   exit(0);

}
