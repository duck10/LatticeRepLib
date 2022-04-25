#ifndef BRAVAISHEIRARCHY_H
#define BRAVAISHEIRARCHY_H

#include "LRL_ReadLatticeData.h"
#include "S6.h"

#include <algorithm>
#include <string>
#include <vector>
#include <utility>

class BravaisHeirarchy {
public:
   BravaisHeirarchy();

   static std::string ProduceSVG(
      const LRL_ReadLatticeData& input, const S6& reducedCell,
      const std::vector<std::pair<std::string, double> >& scores);
   static std::string BoilerPlate_1();
   static std::string BoilerPlate_2();
   static std::string ScoreLabels(const std::vector<std::pair<std::string, double> >& scores);

};

#endif // BRAVAISHEIRARCHY_H