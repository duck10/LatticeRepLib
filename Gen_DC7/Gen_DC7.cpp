#include <cstddef>
#include <iomanip>
#include <iostream>

#include "DC.h"
#include "Generated_IT_Types.h"
#include"LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "G6.h"
#include "NCDist.h"
#include "Niggli.h"
#include "svd.h"


const size_t g_maxgen = 10000;

bool IsClose(const double a, const double b) {
   return abs(a - b) < 1.0E-7;
}

std::string IdentifyMultiples(const double r, const double x) {
   std::string ret;

   if (IsClose(r, x)) ret = "r";
   else if (IsClose(r, 2 * x)) ret = "2r";
   else if (IsClose(r, 3 * x)) ret = "3r";
   else if (IsClose(r, 4 * x)) ret = "4r";
   else if (IsClose(r, 4.0 / 3.0 * x)) ret = "4/3r";
   return ret;
}

std::string IdentifyMultiplesOf_r(const std::vector<double>&dc) {
      std::vector<std::string> v(7);
   
   v[0] = "r";
   for (size_t i = 1; i < dc.size(); ++i) {
      v[i] = IdentifyMultiples(dc[0], dc[i]);
   }
   return "(" + v[0] + "," + v[1] + "," + v[2] + "," + v[3] + "," + v[4] + "," + v[5] + "," + v[6] + ")";
}

std::string IdentifyDC7Elements(const std::vector<double>& dc) {
   std::string s;
   s = IdentifyMultiplesOf_r(dc);
   return s;
}



void DC_SVD(const std::vector<G6>& cells) {

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
      //std::cout << "eigenvalue " << std::setw(5) << std::right << w[i] << "    \t vector  \t";
      for (size_t k = 0; k < v.size(); ++k) {
         //std::cout << v[k][i] << " ";
         vscalars.push_back(v[k][i]);
      }
      //std::cout << std::endl;
      eigenResults.push_back(std::make_pair(w[i], vscalars));

   }

   std::sort(eigenResults.begin(), eigenResults.end());
      std::string xxxx = IdentifyDC7Elements(eigenResults[6].second);
   std::cout << "largest eigenvalue " << eigenResults[v.size()-1].first << " vector =  " 
      << LRL_ToString(eigenResults[v.size() - 1].second);
   std::cout << " next eigenvalue " << eigenResults[v.size() - 2].first << "  " << xxxx <<std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
}

void CreateListOfTypes(std::vector<std::unique_ptr<IT_Lat_Char_Base> >& vfpg) {
   std::cout << "CreateListOfTypes" << std::endl;
   std::unique_ptr<IT_Lat_Char_Base> fpg;

   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_34a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_34b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_34c)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_34d)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_34e)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_3)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_1)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_5)));

   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_11a)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_11b)));
   vfpg.push_back(std::move(std::unique_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_21)));

}

void Generate(std::vector<std::unique_ptr<IT_Lat_Char_Base> >& vfpg) {
   for (size_t i = 0; i < vfpg.size(); ++i) {
      const std::vector<G6> cells = vfpg[i]->CreateCells(10);
      std::cout << "; case " << vfpg[i]->GetName() << " " << vfpg[i]->GetIT_Number() << " " 
         << vfpg[i]->m_latticeType << "  " << vfpg[i]->m_G6_character <<
         "  " << vfpg[i]->m_DC7_character << std::endl;

      if (!cells.empty()) DC_SVD(cells);
   }

}

int main()
{

   std::cout << "; Generate sample G6 data for search DC7 types" << std::endl;

   std::vector<std::unique_ptr<IT_Lat_Char_Base> > vfpg;
   CreateListOfTypes(vfpg);
   Generate(vfpg);;

   exit(0);

}
