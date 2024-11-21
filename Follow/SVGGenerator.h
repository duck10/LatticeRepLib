#ifndef SVG_GENERATOR_H
#define SVG_GENERATOR_H

#include "Cell.h"
#include "ControlVariables.h"
#include "S6.h"
#include <map>
#include <vector>
#include <memory>
#include <string>

class SVGGenerator {
public:
   static std::string generateSVG(const std::map<std::string, std::pair<std::vector<S6>, std::vector<double>>>& results,
      const ControlVariables& controlVars,
      const std::vector<std::unique_ptr<Cell>>& cells);

private:
   static std::string getCurrentDate();
   static std::string getElapsedTime();
   static std::pair<double, double> findMinMax(const std::map<std::string, std::pair<std::vector<S6>, std::vector<double>>>& results);
   static size_t findMaxSteps(const std::map<std::string, std::pair<std::vector<S6>, std::vector<double>>>& results);
};

#endif // SVG_GENERATOR_H