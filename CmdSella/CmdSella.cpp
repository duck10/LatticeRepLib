
#include <iostream>

#include "DeloneFitResults.h"
#include "LRL_ReadLatticeData.h"
#include "LatticeConverter.h"
#include "Niggli.h"
#include "S6.h"
#include "MatS6.h"
#include "Sella.h"
#include "StoreResults.h"

static double g_maxDeltaForMatch = 0.02;
std::string selectBravaisCase = "";

std::vector<std::string> g_valueErrors;

std::vector<S6> GetInputSellingReducedVectors(const std::vector<LRL_ReadLatticeData>& input, std::vector<MatS6>& vmat) {
   std::vector<S6> v;
   LatticeConverter converter;
   MatS6 m;
   vmat.clear();

   for (size_t i = 0; i < input.size(); ++i) {
      const S6 s6 = converter.SellingReduceCell(input[i].GetLattice(), input[i].GetCell(), m);
      v.push_back(s6);
      vmat.push_back(m);
   }

   return v;
}

std::vector<S6> CreateS6Errors(const std::vector< S6>& vs) {
   std::vector<S6> out;
   for (size_t i = 0; i < vs.size(); ++i) {
      out.push_back(0.1 * vs[i]);
   }
   return out;
}

double DeltaInputBest(const DeloneFitResults& vDeloneFitResults) {
   const S6 input = vDeloneFitResults.GetOriginalInput();
   const S6 bestFit = vDeloneFitResults.GetBestFit();
   return (input - bestFit).norm();
}


std::map<std::string, double> g_BravaisMap;

std::map<std::string, double> CreateBasicBravaisMap() {
   std::map<std::string, double> bravaisMap;
   bravaisMap.insert(std::make_pair("aP", 0));

   return bravaisMap;
}

std::map<std::string, DeloneFitResults>  CreateMapForBestExamples(
   const std::vector<DeloneFitResults>& vDeloneFitResults)
{
   std::map<std::string, DeloneFitResults>  bravaisMap;

   for (size_t i = 0; i < vDeloneFitResults.size(); ++i) {
      std::string name = vDeloneFitResults[i].GetGeneralType();
      //if (name == "oC") name = "oS";
      const double& delta = vDeloneFitResults[i].GetDifference().norm();

      auto mapElement = bravaisMap.find(name);
      if (mapElement == bravaisMap.end())
         bravaisMap.insert(std::make_pair(name, vDeloneFitResults[i]));
      else
         if (delta < (*mapElement).second.GetDifference().norm())
            (*mapElement).second = vDeloneFitResults[i];
   }
   return bravaisMap;
}

std::vector<DeloneFitResults> FilterForBestExample(
   const std::vector<DeloneFitResults>& vDeloneFitResults)
{
   std::map<std::string, DeloneFitResults>  bravaisMap = CreateMapForBestExamples(vDeloneFitResults);

   std::vector<DeloneFitResults> out;
   for (auto i = bravaisMap.begin(); i != bravaisMap.end(); ++i) {
      out.push_back((*i).second);
   }
   return out;
}

std::map<std::string, double> GetBestOfEachBravaisType(
   const std::vector<DeloneFitResults>& vDeloneFitResults)
{

   std::map<std::string, DeloneFitResults> msdfr = CreateMapForBestExamples(vDeloneFitResults);
   std::map<std::string, double>  bravaisMap;
   //std::map<std::string, double>  bravaisMap = CreateBasicBravaisMap();

   //for (size_t i = 0; i < vDeloneFitResults.size(); ++i) {
   //   std::string name = vDeloneFitResults[i].GetGeneralType();
   //   //if (name == "oC") name = "oS";
   //   const double& delta = vDeloneFitResults[i].GetDifference().norm();

   //   auto mapElement = bravaisMap.find(name);
   //   if (mapElement == bravaisMap.end()) 
   //      bravaisMap.insert(std::make_pair(name, delta));
   //   else
   //      if (delta < (*mapElement).second)
   //         (*mapElement).second = delta;
   //}

   for (auto it = msdfr.begin(); it != msdfr.end(); ++it) {
      bravaisMap.insert(make_pair((*it).first, (*it).second.GetDifference().norm()));
   }
   return bravaisMap;
}

std::vector<std::vector<std::string> > CreateBravaisChains()
{
   std::vector<std::vector<std::string> > v;
   v.push_back({ "aP", "mP", "oP", "tP", "cP" });
   v.push_back({ "aP", "mP", "oS", "tP", "cP" });
   v.push_back({ "aP", "mP", "oS", "hP" });
   v.push_back({ "aP", "mS", "oS", "tP", "cP" });
   v.push_back({ "aP", "mS", "oS", "hP" });
   v.push_back({ "aP", "mS", "hR", "cP" });
   v.push_back({ "aP", "mS", "hR", "cF" });
   v.push_back({ "aP", "mS", "hR", "cI" });
   v.push_back({ "aP", "mS", "oF", "tI", "cI" });
   v.push_back({ "aP", "mS", "oI", "tI", "cI" });
   return v;
}

bool CheckOneBravaisChain(
   const std::vector<std::string>& bravaisChain, 
   const std::vector<DeloneFitResults>& v,
    std::map<std::string, double>& valueMap)
{

   bool okCheck = true;
   for (size_t i = 0; i < bravaisChain.size()-1; ++i)
   {
      const std::string name1 = bravaisChain[i];
      if (name1 == "aP") continue;
      const std::string name2 = bravaisChain[i+1];
      double value1 = valueMap[name1];
      double ddddd = sqrt(value1);
      double value2 = valueMap[name2];
      if (value1 < 1.0E-3) value1 = 0;
      if (value2 < 1.0E-3) value2 = 0;
      if (value2 - value1 < -0.0001)
      {
         okCheck = false;
         std::cout << "value error  " 
            << name1 << " " << value1 << " "
            << name2 << " " << value2 << std::endl;
      }
   }
   return okCheck;
}

bool CheckBravaisChains(const std::vector<DeloneFitResults>& v)
{
   //for (size_t i = 0; i < v.size(); ++i)
   //{
   //   std::cout << "CheckBravaisChains  DeloneFitResults= " << v[i].GetGeneralType() << " " << v[i].GetRawFit() << std::endl;
   //}


   std::map<std::string, double> valueMap = GetBestOfEachBravaisType(v);
   bool okCheck = true;
   static const std::vector<std::vector<std::string> > bravaisChains = CreateBravaisChains();
   for (size_t i = 0; i < bravaisChains.size() - 1; ++i)
   {
      if (!CheckOneBravaisChain(bravaisChains[i], v, valueMap)) {
         okCheck = false;
      }
   }
   return okCheck;
}

void NiggliMatchLatticeType(const DeloneFitResults& deloneFitResults) {
   const S6 bestFit = deloneFitResults.GetBestFit();
   double deltaInputBest = DeltaInputBest(deloneFitResults);
   if (deltaInputBest < 1.0E-6) deltaInputBest = 0.0;
   const std::string latticeType = deloneFitResults.GetType();
   const std::string latticeGeneral = deloneFitResults.GetGeneralType();

   const std::vector<std::shared_ptr<GenerateNiggliBase> >
      vglb = GenerateNiggliBase().Select(latticeGeneral);

   for (size_t i = 0; i < vglb.size(); ++i) {
      const std::shared_ptr<GenerateNiggliBase> pt = vglb[i];
      if (pt->GetBravaisType()[0] == 'a') continue;
      G6 probe;
      Niggli::Reduce(G6(bestFit), probe);
      const G6 perpV = pt->GetPerp() * probe;
      double d = perpV.norm();
      if (d < 1.0E-8) d = 0.0;
      if (d / probe.norm() > g_maxDeltaForMatch) continue;
      const G6 centered =  pt->GetToCenter() * probe;

      std::cout << latticeType.substr(1,1)+" " << LRL_Cell_Degrees(centered)
         << "  " << latticeType << "  " << deltaInputBest << "\n";
      break;
   }
}


int main(int argc, char* argv[])
{
   if (argc > 1) {
      selectBravaisCase = argv[1];
      if (argc > 2) {
         const double d = atof(argv[2]);
         if (d != 0.0) g_maxDeltaForMatch = atof(argv[2]);
      }
   }
   std::cout << ";SELLA\n";
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   std::vector<MatS6> reductionMatrices;

   const std::vector<S6> vLat = GetInputSellingReducedVectors(inputList, reductionMatrices);
   const std::vector<S6> errors = CreateS6Errors(vLat);

   //-----------------------------------------------------------------------------------
   std::vector<std::shared_ptr<GenerateDeloneBase> > sptest =
      GenerateDeloneBase().Select(selectBravaisCase);

   for (size_t lat = 0; lat < vLat.size(); ++lat) {
      const std::vector<DeloneFitResults> vDeloneFitResults = Sella().SellaFit(sptest, vLat[lat], errors[lat], reductionMatrices[lat]);


      const bool okCheck = CheckBravaisChains(vDeloneFitResults);
      if (!okCheck) {
         std::cout << "Bravais chain values check failed, input = " << inputList[lat].GetStrCell() << std::endl;
         g_valueErrors.push_back(inputList[lat].GetStrCell());
      }
      const std::vector<DeloneFitResults> vFilteredDeloneFitResults = FilterForBestExample(vDeloneFitResults);

      std::cout << "; " << inputList[lat].GetStrCell() << " input data" << std::endl;
      for (size_t kk = 0; kk < vFilteredDeloneFitResults.size(); ++kk) {
         const DeloneFitResults& dfr = vFilteredDeloneFitResults[kk];
         if (dfr.GetType()[0] == 'a') continue;
         //const double d = vDeloneFitResults[kk].GetRawFit() / vLat[lat].norm();
         if (vFilteredDeloneFitResults[kk].GetRawFit() / vLat[lat].norm() < g_maxDeltaForMatch) {
            //std::cout << dfr.GetType() << " " << LRL_Cell_Degrees(dfr.GetBestFit()) << std::endl;
            NiggliMatchLatticeType(vFilteredDeloneFitResults[kk]);
         }
      }
   }

   if (!g_valueErrors.empty()) {
      std::cout << "Bravais chain failures\n";
      for (size_t i = 0; i < g_valueErrors.size(); ++i) {
         std::cout << g_valueErrors[i] << std::endl;
      }
   }
}
