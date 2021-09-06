#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <utility>

#include "DC.h"
#include "Definitions.h"
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
std::vector<G6> samples;
StoreResults<std::string, std::string> store(numberOfExamplesToDisplayInStore);

const bool g_generateByPerturbation = false;

std::vector<G6> GenerateSamplesAndPerturbations(const size_t n) {
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
   return samples;
}

void FormatEigenValuesForStore(
   const std::vector<std::pair<double, std::vector<double> > >& eigenResults,
   const std::unique_ptr<IT_Lat_Char_Base>& vfpg,
   const std::vector<std::vector<double> > v,
   const std::vector<G6>& cells)
{
   std::string key;
   for (size_t i = 0; i < 7; ++i) {
      const double eigenvalue = eigenResults[v.size() - 1 - i].first;

      key = vfpg->m_ITnumber + " (" + vfpg->m_name + ")";
      std::string data = vfpg->m_ITnumber + " " + vfpg->m_name +
         "  " + vfpg->m_latticeType + " " + vfpg->m_G6_character + "  " + 
         vfpg->m_DC7_character + "\n";
      if (eigenvalue > 1.0 || eigenvalue == 0) {
         data += "eigenvalue " + LRL_ToString(eigenvalue) + " vector =  "
            + LRL_ToString(eigenResults[v.size() - 1 - i].second) + "\n";
      }
   }

   const std::string data = vfpg->m_ITnumber + " " + vfpg->m_name +
      "  " + vfpg->m_latticeType + " " + vfpg->m_G6_character + "  " + vfpg->m_DC7_character + "\n";
   if ( store.GetItemCount(key) == 0) store.Store(key, data);
   const size_t maxData = store.GetNmax();
   for (size_t i = 0; i < cells.size(); ++i) {
      if (store.GetItemCount(key) < 3) {
         const size_t n = store.GetItemCount(key);
         store.Store(key, LRL_ToString(cells[i], "\n", DC(cells[i]), "\n"));
      }
      else {
         store.Store(key, LRL_ToString(cells[i]));
      }
   }
}

void DC_SVD(const std::unique_ptr<IT_Lat_Char_Base>& vfpg, const std::vector<G6>& cells) {
   //if ( cells.size() < 10 ) std::cout << " cells count in DC_SVD " << cells.size() << std::endl;
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


   FormatEigenValuesForStore(eigenResults, vfpg, v, cells);





   ////std::sort(eigenResults.begin(), eigenResults.end());
   //std::string key = vfpg->m_ITnumber + " (" + vfpg->m_name + ")";
   //std::string data = vfpg->m_ITnumber + " " + vfpg->m_name +
   //   "  " + vfpg->m_latticeType + " " + vfpg->m_G6_character + "  " + vfpg->m_DC7_character + "\n";
   ////const double test = eigenResults[0].second[6];
   //////data += "largest eigenvalue " + LRL_ToString(eigenResults[v.size() - 1].first) + " vector =  "
   //////  + LRL_ToString(eigenResults[v.size() - 1].second);
   //////data += LRL_ToString("\n", "next eigenvalue ", eigenResults[v.size() - 2].first, "\n");
   ////for (size_t i = 0; i < 7; ++i) {
   ////   const double eigenvalue = eigenResults[v.size() - 1 - i].first;
   ////   if (eigenvalue > 1.0 || eigenvalue == 0 )
   ////   data += "eigenvalue " + LRL_ToString(eigenvalue) + " vector =  "
   ////      + LRL_ToString(eigenResults[v.size() - 1 - i].second) + "\n";
   ////}

   //store.Store(key, data);
   //const size_t maxData = store.GetNmax();
   //for (size_t i = 0; i < cells.size(); ++i) {
   //   if (store.GetItemCount(key) <3) {
   //      const size_t n = store.GetItemCount(key);
   //      store.Store(key, LRL_ToString(cells[i], "\n", DC(cells[i]), "\n"));
   //   }
   //   else {
   //      store.Store(key, LRL_ToString(cells[i]));
   //   }
   //}
}

void FormatDataForStore() {

}

void CreateListOfTypes(std::vector<std::unique_ptr<IT_Lat_Char_Base> >& vfpg) {

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
std::vector<G6> GenerateRandomSamples(const size_t n) {
   std::vector<G6> v;
   for (size_t i = 0; i < n; ++i) {
      v.push_back(G6().randDeloneUnreduced());
   }
   return v;
}

std::vector<G6> GenerateReducedRandomSamples(const size_t n) {
   std::vector<G6> v;
   for (size_t i = 0; i < n; ++i) {
      const G6 oneCell = G6().rand();
      G6 next = IT_Lat_Char_Base::GeneratePerturbation(oneCell, 0.1);
      G6 out;
      if ( i%2==0) const bool b = Niggli::Reduce(oneCell, out);
      if ( i%2==1) const bool b = Niggli::Reduce(oneCell, out);
      v.push_back(out);
   }
   return v;
}

std::pair<bool, G6> TestOneSample(const std::unique_ptr<IT_Lat_Char_Base>& vfpg, const G6& sample) {
   const MatG6& proj = vfpg->m_projector;
   const G6 g1 = proj * sample;
   G6 g2 = g1;
   const bool b = Niggli::Reduce(g1, g2);
   const G6 g3 = proj * g2;
   //std::cout << "input " << sample << std::endl;
   //std::cout << "projected " << g1 << std::endl;
   //std::cout << "Niggli_reduced " << g2 << std::endl;
   //std::cout << "projected_again " << g3 << std::endl << std::endl;
   //std::cout << "input " << LRL_Cell_Degrees(sample) << std::endl;
   //std::cout << "projected " << LRL_Cell_Degrees(g1) << std::endl;
   //std::cout << "Niggli_reduced " << LRL_Cell_Degrees(g2) << std::endl;
   //std::cout << "projected_again " << LRL_Cell_Degrees(g3) << std::endl << std::endl;
   const double d = (g3 - g2).norm();
   const bool test1 = (g3 - g2).norm() < 1.0E-6;
   const bool test2 = vfpg->IsMember(g3);;
   const int i19191 = 19191;

   return std::make_pair(test1 && test2, g3);
}

std::vector<G6> TestOneType(const std::unique_ptr<IT_Lat_Char_Base>& vfpg, const std::vector<G6>& samples) {
   const MatG6& proj = vfpg->m_strProjector;
   std::vector<G6> good;

   for (size_t i = 0; i < samples.size(); ++i) {
      const std::pair<bool, G6> out = TestOneSample(vfpg, samples[i]);
      if (out.first) {
         good.push_back(out.second);
      }
   }
   return good;
}

void ProcessTypes(const std::vector<std::unique_ptr<IT_Lat_Char_Base> >& vfpg, const std::vector<G6>& samples) {
   for (size_t i = 0; i <vfpg.size(); ++i) {
      const std::vector<G6> out = TestOneType(vfpg[i], samples);
         std::cout << "start type # SVD " << i << " " << vfpg[i]->m_ITnumber << std::endl;
      
      for (size_t k = 0; k < out.size(); ++k) {
         DC_SVD(vfpg[i], out);
         //std::cout << i << " " << vfpg[i]->GetName() << " " << out[k].first << " " << out[k].second << std::endl;
      }
   }
}

std::vector<G6> Reduce(const std::unique_ptr<IT_Lat_Char_Base>& vfpg, const std::vector<G6>& g6) {
   std::vector<G6> output;
   for (size_t i = 0; i < g6.size(); ++i) {
      const G6 projected = vfpg->m_projector * g6[i];
      G6 gout;
      const bool b1 = Niggli::Reduce(projected, gout);
      const bool b2 = (projected - gout).Norm() < 1.0E-5;
      const bool bproj = gout[3] > 0 && gout[4] > 0 && gout[5] > 0;
      if ( b1 && (projected - gout).Norm() < 1.0E-5 && vfpg->IsMember(gout) && gout.IsValid())
         output.push_back(gout);
   }
   return output;
}

std::vector<G6>  Generate(const std::vector<std::unique_ptr<IT_Lat_Char_Base> >& vfpg) {
   std::vector<G6> out;
   for (size_t i = 0; i < vfpg.size(); ++i) {
      const std::vector<G6> cells = Reduce(vfpg[i], samples);
      out.insert(out.begin(), cells.begin(), cells.end());
      DC_SVD(vfpg[i], cells);
   }
   return out;
}

void MonteCarloForOneType(const size_t n, const std::unique_ptr<IT_Lat_Char_Base>& vfpg) {
   //StoreResults<double, G6> monte(5);
   //StoreResults<double, std::string> DCstore;
   std::cout << "MonteCarloForOneType" << std::endl;
   for (size_t i = 0; i < n; ++i) {
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
      //monte.Store(key, g1);

      //const std::string examples = LRL_ToString("g6 ", g1,"\ng6 ",g2,"\n") +
      //   LRL_ToString("DC7 ", DC(g1), "\nDC7 ", DC(g2), "\n") +
      //   LRL_ToString("dnorm ", dnorm, "  gnorm ", gnorm, "\n");

      //DCstore.Store(key, examples);
   }
   //monte.ShowResults();
   //DCstore.ShowResults();
}

void TestProjectors() {
   //tested 2021-08-25 and found all projectors and prefixes are correct
   // two prefixes were incorrect, and a minus sign had been lost in case 24

   std::vector<std::unique_ptr<IT_Lat_Char_Base> > vfpg;
   CreateListOfTypes(vfpg);
   const G6 g = G6::rand();

   for (size_t i = 0; i < vfpg.size(); ++i) {
      const MatG6 projector = (vfpg[i]->m_projector);
      const G6 g1 = projector * g;
      const G6 g2 = projector * g1;
      const double d = g2.norm() / g1.norm();
      if (abs(d - 1.0) > 1.0e-3) {
         std::cout << i << std::endl << g1 << std::endl << g2 << std::endl;
         std::cout << "  d = " << d << vfpg[i]->m_ITnumber << " " << vfpg[i]->m_name << std::endl << std::endl;
      }
   }
   exit(0);
}

std::vector<G6> GenerationByPeturbation(const size_t n, const std::vector<std::unique_ptr<IT_Lat_Char_Base> >& vfpg) {
   std::cout << "; Generate perturbation only sample G6 data for search DC7 types" << std::endl;
   std::cout << "number of samples to generate " << n << std::endl;
   const std::vector<G6> samples = GenerateSamplesAndPerturbations(n);
   const std::vector<G6> out = Generate(vfpg);
   return out;
}

void GenerationByMonteCarlo(const size_t n, const std::vector<std::unique_ptr<IT_Lat_Char_Base> >& vfpg) {
   std::pair<G6, G6> samples;
   for (size_t i = 0; i < vfpg.size(); ++i) {
      MonteCarloForOneType(n, vfpg[i]);
   }
}

std::vector<G6> GenerationByRandomAndProjection(const size_t n, const std::vector<std::unique_ptr<IT_Lat_Char_Base> >& vfpg) {
   std::cout << "; Generate random and perturbed sample G6 data for search DC7 types" << std::endl;
   std::cout << "number of samples to generate " << n << std::endl;
   const std::vector<G6> samples = GenerateRandomSamples(n);
   ProcessTypes(vfpg, samples);
   return samples;
}

bool FilterForValidCell(const G6& g) {
   if (!g.IsValid()) return false;
   if (g[0] <= 0 || g[1] <= 0 || g[2] <= 0) return false;
   return LRL_Cell(g).IsValid();
}

std::vector<G6> FilterForValidCell(const std::vector<G6>& vg) {
   std::vector<G6> samples;
   for (size_t i = 0; i < vg.size(); ++i) {
      if (FilterForValidCell(vg[i])) samples.push_back(vg[i]);
   }
   return samples;
}

std::vector<G6> FilterListForType(const std::unique_ptr<IT_Lat_Char_Base>& vfpg, const std::vector <G6>& vg) {
   std::vector<G6> out;
   for (size_t i = 0; i < vg.size(); ++i) {
      const G6 projected = vfpg->m_projector * vg[i];
      G6 gout;
      const bool b1 = Niggli::Reduce(projected, gout);
      const bool b2 = (projected - gout).Norm() < 1.0E-5;
      const bool bproj = gout[3] > 0 && gout[4] > 0 && gout[5] > 0;
      if (b1 && (projected - gout).Norm() < 1.0E-5 && vfpg->IsMember(gout) && gout.IsValid())
         out.push_back(gout);
   }
   return out;
}


int main()
{
   std::vector<std::unique_ptr<IT_Lat_Char_Base> > vfpg;
   CreateListOfTypes(vfpg);

   const std::vector<G6> vred = GenerateReducedRandomSamples(g_maxgen);
   for (size_t i = 0; i < vfpg.size(); ++i) {
      const std::vector<G6> accepted = FilterListForType(vfpg[i], vred);
      DC_SVD(vfpg[i], accepted);
   }

   store.SetItemSeparator("==========================================================");
   store.ShowResultsByKeyAscending();
   exit(0);

   //TestProjectors();
   const bool noMonteCarlo = true;


   //const std::vector<G6> perturbationSamplesGenerationByPeturbation = GenerationByPeturbation(g_maxgen / 3, vfpg);
   ////GenerationByMonteCarlo(g_maxgen/3, vfpg);
   //const std::vector<G6> randomGenerationSamples = GenerationByRandomAndProjection(g_maxgen / 3, vfpg);

   //std::vector<G6> samples;
   //samples.insert(samples.end(), 
   //   perturbationSamplesGenerationByPeturbation.begin(), perturbationSamplesGenerationByPeturbation.end());
   //samples.insert(samples.end(), randomGenerationSamples.begin(), randomGenerationSamples.end());
   //for (size_t i = 0; i < vfpg.size(); ++i) {
   //   DC_SVD(vfpg[i], samples);
   //}

      if (noMonteCarlo) {
   }
   else {
//     CreateListOfSelectedTypes(vfpg);
     std::pair<G6, G6> samples;
     for (size_t i = 0; i < vfpg.size(); ++i) {
        MonteCarloForOneType(g_maxgen, vfpg[i]);
     }
   }
   {
   std::cout << "; Generate sample G6 data for search DC7 types" << std::endl;
   std::cout << "number of samples to generate " << g_maxgen << std::endl;
//   CreateListOfTypes(vfpg);
   const std::vector<G6> samples = GenerateRandomSamples(g_maxgen);
   ProcessTypes(vfpg, samples);
   }

   store.SetItemSeparator("==========================================================");
   store.ShowResultsByKeyAscending();

   exit(0);

}
