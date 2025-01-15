#pragma warning (disable: 4996)

#include "DistanceFactory.h"
#include "Follow.h"
#include "GlitchDetector.h"
#include "LinearAxis.h"
#include "Niggli.h"
#include "Polar.h"
#include "SvgPlotWriter.h"

#include <algorithm>
#include <cmath>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

#include "PathPoint.h"

Follow::Follow(FollowControls& followControls)
   : controls(followControls)
   , glitchDetector()
{
   distfuncs = DistanceFactory::createEnabledDistances(controls.getDistanceTypes());
}

void Follow::PrintPathData(const Path& path) const {
   if (controls.shouldPrintDistanceData()) {
      std::cout << path << std::endl;
   }
}

bool Follow::processPerturbation(int trialNum, int perturbationNum, const FollowInstance& instance) {
   distfuncs = DistanceFactory::createEnabledDistances(controls.getDistanceTypes());
   if (distfuncs.empty()) {
      std::cerr << "; No distance types enabled - please enable at least one type" << std::endl;
      return false;  // Exit before creating file
   }

   std::string curfilename = instance.GetRawFileName();
   const Path path = generatePath(trialNum, perturbationNum, instance.GetFollowSeed());
   if (path.empty()) {
      std::cerr << "; Failed to generate valid path" << std::endl;
      return false;
   }
   if (curfilename.empty()) {
      std::cerr << "; Empty output filename" << std::endl;
      return false;
   }
   if (path.empty()) return false;
   if (curfilename.empty()) return false;

   if (controls.shouldPrintDistanceData()) {
      PrintPathData(path);
   }

   std::vector<std::vector<double>> allDistances;
   for (const auto& dist : distfuncs) {
      std::vector<double> pathDists;
      pathDists.reserve(path.size());
      for (const auto& [point1, point2] : path) {
         if (Path::PathPointIsValid(S6(point1.getCell())) && Path::PathPointIsValid(S6(point2.getCell()))) {
            pathDists.emplace_back(dist->dist(static_cast<S6>(point1.getCell()),
               static_cast<S6>(point2.getCell())));
         }
         else {
            pathDists.emplace_back(-19191.0);
         }
      }
      allDistances.push_back(pathDists);
   }

   if (allDistances.empty()) {
      std::cerr << "; Error, no paths created\n";
      return false;
   }

   std::ofstream svgfile(curfilename);
   if (svgfile.is_open()) {
      SvgPlotWriter writer(svgfile, controls, glitchDetector);
      writer.writePlot(allDistances, distfuncs, trialNum, perturbationNum);
   }
   else {
      std::cerr << ";Warning: Unable to open output file" << std::endl;
      return false;
   }
   return true;
}

Path Follow::generatePath(const int trialNum, int perturbationNum, const std::vector<LatticeCell>& perturbedPoints) {

   const int numPoints = controls.getNumPoints();
   switch (controls.getMode()) {
   case FollowerMode::POINT:
      return Path::generatePointPath(perturbedPoints[0], numPoints);
   case FollowerMode::LINE:
      return Path::generateLinePath(perturbedPoints[0], perturbedPoints[1], numPoints);
   case FollowerMode::CHORD:
      return Path::generateChordPath(perturbedPoints[0], perturbedPoints[1], numPoints);
   case FollowerMode::CHORD3:
      return Path::generateChord3Path(perturbedPoints[0], perturbedPoints[1], perturbedPoints[2], numPoints);
   case FollowerMode::TRIANGLE:
      return Path::generateTrianglePath(perturbedPoints[0], perturbedPoints[1], perturbedPoints[2], numPoints);
   case FollowerMode::SPLINE:
      return Path::generateSplinePath(numPoints);
   default:
      return Path();
   }
}

LatticeCell Follow::perturbVector(const LatticeCell& inputVector, const int perturbationIndex) const {
   if (perturbationIndex == 0) {
      return inputVector;  // No perturbation for first case
   }

   const S6 randomVector = S6(Polar::rand());
   const S6 inputS6 = S6(inputVector.getCell());

   // Compute orthogonal component
   const double dotProduct = inputS6.Dot(randomVector);
   const double inputNormSquared = inputS6.Dot(inputS6);
   const S6 orthogonalVector = randomVector - (dotProduct / inputNormSquared) * inputS6;
   const double d = orthogonalVector.Dot(inputS6);

   // Scale the orthogonal vector
   const double inputNorm = std::sqrt(inputNormSquared);
   const double orthogonalNorm = std::sqrt(orthogonalVector.Dot(orthogonalVector));
   const double scaleFactor = inputNorm * controls.getPerturbBy() / orthogonalNorm;
   const S6 scaledOrthogonalVector = scaleFactor * orthogonalVector;

   // Return perturbed vector with same lattice type
   return LatticeCell(G6(inputS6 + scaledOrthogonalVector), inputVector.getLatticeType());
}
