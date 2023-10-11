#ifndef BRAVAISHEIRARCHY_H
#define BRAVAISHEIRARCHY_H

#include "DeloneFitResults.h"
#include "LRL_ReadLatticeData.h"
#include "S6.h"

#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <utility>

class BravaisHeirarchy {
public:
   BravaisHeirarchy();
   static bool CheckOneBravaisChain(
      const std::vector<std::string>& bravaisChain,
      const std::vector<DeloneFitResults>& v,
      std::map<std::string, double>& valueMap,
      std::vector<std::string>& errorList);

   static bool CheckBravaisChains(const std::vector<DeloneFitResults>& v);
   static std::vector<std::vector<std::string> > CreateBravaisChains();
   static std::map<std::string, double> GetBestOfEachBravaisType(
      const std::vector<DeloneFitResults>& vDeloneFitResults);
   static std::map<std::string, DeloneFitResults>  CreateMapForBestExamples(
      const std::vector<DeloneFitResults>& vDeloneFitResults);

   static std::string ProduceSVG(
      const LRL_ReadLatticeData& input, const S6& reducedCell,
      const std::vector<std::pair<std::string, double> >& scores);
   static std::string BoilerPlate_1();
   static std::string BoilerPlate_2();
   static std::string ScoreLabels(const std::vector<std::pair<std::string, double> >& scores);

};

#endif // BRAVAISHEIRARCHY_H