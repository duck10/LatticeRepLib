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
   , allDistances(std::vector<std::vector<double>>())
{
   distfuncs = DistanceFactory::createEnabledDistances(controls.getDistanceTypes());
}

void Follow::PrintPathData(const Path& path) const {
   if (controls.shouldPrintDistanceData()) {
      std::cout << path << std::endl;
   }
}


std::vector<Glitch> Follow::DetectGlitchesWithS6Data(
   const std::vector<double>& distances,
   const Path& path,
   const std::string& distanceType) const {
   auto glitches = Glitch::DetectGlitches(distances, controls.getGlitchThreshold(), 5);

   for (auto& g : glitches) {
      const size_t idx = g.index;
      const auto& [point1, point2] = path[idx];
      g.value = distances[idx];
      g.atPoint = S6Pair(static_cast<S6>(point1.getCell()), static_cast<S6>(point2.getCell()));

      if (idx > 0) {
         const auto& [before1, before2] = path[idx - 1];
         g.beforePoint = S6Pair(static_cast<S6>(before1.getCell()), static_cast<S6>(before2.getCell()));
      }
      else {
         g.beforePoint = g.atPoint;
      }

      if (idx < path.size() - 1) {
         const auto& [after1, after2] = path[idx + 1];
         g.afterPoint = S6Pair(static_cast<S6>(after1.getCell()), static_cast<S6>(after2.getCell()));
      }
      else {
         g.afterPoint = g.atPoint;
      }

      g.distanceType = distanceType;
   }
   return glitches;
}


bool Follow::processPerturbation(int trialNum,
   int perturbationNum, const FollowInstance& instance,
   const std::vector<LatticeCell>& cells) {
   distfuncs = DistanceFactory::createEnabledDistances(controls.getDistanceTypes());
   if (distfuncs.empty()) {
      std::cout << "; No distance types enabled - please enable at least one type" << std::endl;
      return false;
   }

   std::string curfilename = instance.GetRawFileName();
   const Path path = generatePath(trialNum, perturbationNum, instance.GetFollowSeed(), cells);
   if (path.empty()) {
      std::cout << "; Failed to generate valid path" << std::endl;
      return false;
   }
   if (curfilename.empty()) {
      std::cout << "; Empty output filename" << std::endl;
      return false;
   }

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
            pathDists.emplace_back(-19191.0);  // This is happening at index 28
         }
      }
      allDistances.push_back(pathDists);
   }

   if (allDistances.empty()) {
      std::cout << "; Error, no paths created\n";
      return false;
   }

   std::vector<Glitch> allGlitches;

   if (controls.isGlitchDetectionEnabled()) {
      for (size_t distIdx = 0; distIdx < allDistances.size(); ++distIdx) {
         auto glitches = DetectGlitchesWithS6Data(allDistances[distIdx], path, distfuncs[distIdx]->getName());
         allGlitches.insert(allGlitches.end(), glitches.begin(), glitches.end());
      }
   }

   if (!controls.shouldShowOnlyGlitches() || !allGlitches.empty()) {
      std::ofstream svgfile(curfilename);
      if (svgfile.is_open()) {
         SvgPlotWriter writer(svgfile, controls);
         if (controls.isGlitchDetectionEnabled()) {
            writer.setGlitches(allGlitches);
         }
         std::cout << "; Follow graphics file(s) "
            << instance.GetFullFileName() << std::endl;

         writer.writePlot(allDistances, distfuncs, trialNum, perturbationNum);
      }
      else {
         std::cout << "Warning: Unable to open output file" << std::endl;
         return false;
      }
      return true;
   }
}

Path Follow::generatePath(const int trialNum, const int perturbationNum,
   const std::vector<LatticeCell>& perturbedPoints,
   const std::vector<LatticeCell>& cells) const {

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
      return Path::generateSplinePath(numPoints, cells);
   default:
      return Path();
   }
}

LatticeCell Follow::perturbVector(const LatticeCell& inputVector, const int perturbationIndex) const {
   if (perturbationIndex == 0) {
      return inputVector;
   }

   const S6 randomVector = S6(Polar::rand());
   const S6 inputS6 = S6(inputVector.getCell());

   const double dotProduct = inputS6.Dot(randomVector);
   const double inputNormSquared = inputS6.Dot(inputS6);
   const S6 orthogonalVector = randomVector - (dotProduct / inputNormSquared) * inputS6;
   const double d = orthogonalVector.Dot(inputS6);

   const double inputNorm = std::sqrt(inputNormSquared);
   const double orthogonalNorm = std::sqrt(orthogonalVector.Dot(orthogonalVector));
   const double scaleFactor = inputNorm * controls.getPerturbBy() / orthogonalNorm;
   const S6 scaledOrthogonalVector = scaleFactor * orthogonalVector;

   return LatticeCell(G6(inputS6 + scaledOrthogonalVector), inputVector.getLatticeType());
}

std::vector<FollowInstance> Follow::CreateFollowInstanceList(
   const FollowControls& controls,
   const std::vector<LatticeCell>& cells)
{
   std::vector<FollowInstance> instances;
   const size_t vectorsNeeded = controls.getVectorsPerTrial();
   const size_t numCompleteSets = cells.size() / vectorsNeeded;
   instances.reserve(numCompleteSets * controls.getPerturbations());  // Pre-allocate space

   for (size_t i = 0; i < numCompleteSets; ++i) {
      std::vector<LatticeCell> starter;
      starter.reserve(vectorsNeeded);  // Pre-allocate space

      const size_t baseIndex = i * vectorsNeeded;
      for (size_t j = 0; j < vectorsNeeded; ++j) {
         starter.emplace_back(cells[baseIndex + j]);
      }

      // Handle unperturbed case
      instances.emplace_back(starter, static_cast<int>(i), 0);

      // Handle perturbations
      for (size_t kk = 1; kk < controls.getPerturbations(); ++kk) {
         auto perturbedCells = FollowInstance::Perturb(starter, controls.getPerturbBy());
         instances.emplace_back(std::move(perturbedCells),
            static_cast<int>(i),
            static_cast<int>(kk));
      }
   }
   return instances;
}

void Follow::assignFilenamesToInstances(
   const std::vector<std::string>& rawNames,
   const std::vector<std::string>& basicNames,
   const std::vector<std::string>& fullNames,
   std::vector<FollowInstance>& instances)
{
   for (size_t i = 0; i < instances.size() && i < rawNames.size(); ++i) {
      instances[i].AddFileNames(rawNames[i], basicNames[i], fullNames[i]);
   }
}

void Follow::processInstances(
   const std::vector<FollowInstance>& instances,
   const size_t blockStart,
   const size_t blockEnd,
   FollowControls& controls,
   const std::vector<LatticeCell>& cells)
{
   Follow follow(controls);
   for (size_t i = blockStart; i < blockEnd && i < instances.size(); ++i) {
      if (follow.processPerturbation(instances[i].GetTrial(),
         instances[i].GetPerturbation(),
         instances[i], cells))
      {
         if ( !controls.shouldShowOnlyGlitches())
         {
            std::cout << "; Follow graphics file(s) "
               << i << "  " << instances[i].GetFullFileName() << std::endl;
         }
      }
   }
}
