#include <cstddef>
#include <iostream>

#include "Generated_IT_Types.h"
#include"LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "G6.h"
#include "NCDist.h"
#include "Niggli.h"
#include "StoreResults.h"

StoreResults<std::string, std::string> store;
const size_t g_maxgen = 10000;



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
         << vfpg[i]->m_latticeType << "  " << vfpg[i]->m_character <<std::endl;
      for (size_t k = 0; k < cells.size(); ++k) {
         std::cout << "g " << cells[k] << (vfpg[i]->IsMember(cells[k]) ? " valid " : " invalid ") << "  " << k+1 << std::endl;
      }
      std::cout << std::endl;
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
