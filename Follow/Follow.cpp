#pragma warning (disable: 4996)

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

#include "PathPoint.h"

Follow::Follow(ControlVariables& cv) : controlVars(cv), glitchDetector() {
   distfuncs = DistanceFactory::createEnabledDistances(controlVars);
}
void Follow::run(const std::vector<std::string>& filenames, const std::vector<LatticeCell>& inputVectors) {
   this->filenames = filenames;
   this->inputVectors = inputVectors;

   int vectorsPerTrial = controlVars.getVectorsPerTrial();
   int numTrials = int(inputVectors.size()) / vectorsPerTrial;

   for (int trial = 0; trial < numTrials; ++trial) {
      processTrial(trial);
   }
}

void Follow::processAllTrials() {
   const int vectorsPerTrial = controlVars.getVectorsPerTrial();
   const int numTrials = int(inputVectors.size()) / vectorsPerTrial;

   for (int trial = 0; trial < numTrials; ++trial) {
      processTrial(trial);
   }
}

void Follow::processTrial(int trialNum) {
   std::vector<LatticeCell> startingPoints;
   const int vectorsPerTrial = controlVars.getVectorsPerTrial();

   // Get the starting points for this trial
   for (int i = 0; i < vectorsPerTrial; ++i) {
      startingPoints.emplace_back(inputVectors[trialNum * vectorsPerTrial + i]);
   }

   for (int perturbation = 0; perturbation < controlVars.perturbations; ++perturbation) {
      std::vector<LatticeCell> perturbedPoints;
      for (const auto& point : startingPoints) {
         perturbedPoints.push_back(perturbVector(point, perturbation));
      }
      processPerturbation(trialNum, perturbation, perturbedPoints);
   }
}

void Follow::PrintDistanceData(const Path& path) {
   if (controlVars.printDistanceData) {
      std::cout << "; the path" << std::endl;
      for (const auto& [point1, point2] : path) {
         const S6 p1(point1.getCell());
         const S6 p2(point2.getCell());
         const std::string invalid1 = (p1.IsValid()) ? "" : " invalid";
         const std::string invalid2 = (p2.IsValid()) ? "" : " invalid";
         std::cout << "; " << point1.getLatticeType() << " " << p1 << invalid1
            << "\n; " << point2.getLatticeType() << " " << p2 << invalid2 << std::endl << std::endl;
      }
   }
   std::cout << ";--------  end of path ----------------- \n";
}

bool PathPointIsValid(const S6& p) {
   return(LRL_Cell(p).IsValid());
}

void Follow::processPerturbation(int trialNum, int perturbationNum, const std::vector<LatticeCell>& perturbedPoints) {
   const Path path = generatePath(trialNum, perturbationNum, perturbedPoints);
   controlVars.updatePathStart(perturbedPoints);
   controlVars.updatePath(path);
   if (path.empty()) return;

   if (controlVars.printDistanceData) {
      PrintDistanceData(path);
   }

   std::vector<std::vector<double>> allDistances;
   for (const auto& dist : distfuncs) {
      std::vector<double> pathDists;
      pathDists.reserve(path.size());
      for (const auto& [point1, point2] : path) {
         if (PathPointIsValid(S6(point1.getCell())) && PathPointIsValid(S6(point2.getCell()))) {
            pathDists.emplace_back(dist->dist(point1.getCell(), point2.getCell()));
         }
         else {
            pathDists.emplace_back(-19191.0);
         }
      }
      allDistances.push_back(pathDists);
   }

   std::ofstream svg(controlVars.filenames[trialNum * controlVars.perturbations + perturbationNum]);
   if (svg.is_open()) {
      SvgPlotWriter writer(svg, controlVars, glitchDetector);
      writer.writePlot(allDistances, distfuncs, trialNum, perturbationNum);
   }
   else {
      std::cout << "unable to open output file" << std::endl;
   }
}

Path Follow::generatePath(const int trialNum, int perturbationNum, const std::vector<LatticeCell>& perturbedPoints) {
   switch (controlVars.followerMode) {
   case FollowerMode::POINT:
      return generatePointPath(perturbedPoints[0]);
   case FollowerMode::LINE:
      return generateLinePath(perturbedPoints[0], perturbedPoints[1]);
   case FollowerMode::CHORD:
      return generateChordPath(perturbedPoints[0], perturbedPoints[1]);
   case FollowerMode::CHORD3:
      return generateChord3Path(perturbedPoints[0], perturbedPoints[1], perturbedPoints[2]);
   case FollowerMode::TRIANGLE:
      return generateTrianglePath(perturbedPoints[0], perturbedPoints[1], perturbedPoints[2]);
   default:
      return Path();
   }
}

static S6 MakeInvalidS6() {
   return { 19191.111111111111, 0, 0, 0, 0, 0 };
}

Path Follow::generatePointPath(const LatticeCell& startPoint) const {
   // Convert to primitive before Niggli reduction
   G6 primitive = startPoint.toPrimitive();
   G6 niggliReduced;
   Niggli::Reduce(primitive, niggliReduced);
   const S6 startS6(startPoint.getCell());
   const S6 endS6(niggliReduced);
   const std::string latticeType = startPoint.getLatticeType();

   Path path;
   path.reserve(controlVars.numFollowerPoints);

   for (int i = 0; i < controlVars.numFollowerPoints; ++i) {
      const double t = i / static_cast<double>(controlVars.numFollowerPoints - 1);
      S6 currentS6 = startS6 * (1.0 - t) + endS6 * t;
      if (!PathPointIsValid(currentS6)) currentS6 = MakeInvalidS6();

      path.emplace_back(LatticeCell(G6(currentS6), latticeType),
         LatticeCell(G6(endS6), latticeType));
   }
   return path;
}

Path Follow::generateLinePath(const LatticeCell& startPoint, const LatticeCell& endPoint) const {
   const S6 startS6(startPoint.getCell());
   const S6 endS6(endPoint.getCell());
   const std::string startType = startPoint.getLatticeType();
   const std::string endType = endPoint.getLatticeType();

   Path path;
   path.reserve(controlVars.numFollowerPoints);

   for (int i = 0; i < controlVars.numFollowerPoints; ++i) {
      const double t = i / static_cast<double>(controlVars.numFollowerPoints - 1);
      S6 currentS6 = startS6 * (1.0 - t) + endS6 * t;
      if (!PathPointIsValid(currentS6)) currentS6 = MakeInvalidS6();
      path.emplace_back(LatticeCell(G6(currentS6), startType), LatticeCell(G6(endS6), endType));
   }
   return path;
}


Path Follow::generateChordPath(const LatticeCell& point1, const LatticeCell& point2) const {
   G6 primitive1 = point1.toPrimitive();
   G6 primitive2 = point2.toPrimitive();
   G6 reduced1G6, reduced2G6;
   Niggli::Reduce(primitive1, reduced1G6);
   Niggli::Reduce(primitive2, reduced2G6);

   const S6 p1S6(point1.getCell());
   const S6 p2S6(point2.getCell());
   const S6 reduced1(reduced1G6);
   const S6 reduced2(reduced2G6);
   const std::string type1 = point1.getLatticeType();
   const std::string type2 = point2.getLatticeType();

   Path path;
   path.reserve(controlVars.numFollowerPoints);

   for (int i = 0; i < controlVars.numFollowerPoints; ++i) {
      const double t = i / static_cast<double>(controlVars.numFollowerPoints - 1);
      S6 mobile1 = p1S6 * (1.0 - t) + reduced1 * t;
      S6 mobile2 = p2S6 * (1.0 - t) + reduced2 * t;
      if (!PathPointIsValid(mobile1)) mobile1 = MakeInvalidS6();
      if (!PathPointIsValid(mobile2)) mobile2 = MakeInvalidS6();
      path.emplace_back(LatticeCell(G6(mobile1), type1), LatticeCell(G6(mobile2), type2));
   }
   return path;
}

Path Follow::generateChord3Path(const LatticeCell& mobile1, const LatticeCell& mobile2, const LatticeCell& target) const {
   const S6 m1S6(mobile1.getCell());
   const S6 m2S6(mobile2.getCell());
   S6 initialDirection = m2S6 - m1S6;
   const double separation = initialDirection.Norm();
   initialDirection = initialDirection * (1.0 / separation);
   const S6 initialMidpoint = (m1S6 + m2S6) * 0.5;
   const S6 targetS6(target.getCell());
   const std::string type1 = mobile1.getLatticeType();
   const std::string type2 = mobile2.getLatticeType();

   Path path;
   path.reserve(controlVars.numFollowerPoints);

   for (int i = 0; i < controlVars.numFollowerPoints; ++i) {
      const double t = i / static_cast<double>(controlVars.numFollowerPoints - 1);
      S6 currentMidpoint = initialMidpoint * (1.0 - t) + targetS6 * t;
      S6 point1 = currentMidpoint - initialDirection * (separation * 0.5);
      S6 point2 = currentMidpoint + initialDirection * (separation * 0.5);
      if (!PathPointIsValid(point1)) point1 = MakeInvalidS6();
      if (!PathPointIsValid(point2)) point2 = MakeInvalidS6();
      path.emplace_back(LatticeCell(G6(point1), type1), LatticeCell(G6(point2), type2));
   }
   return path;
}

Path Follow::generateTrianglePath(const LatticeCell& point1, const LatticeCell& point2, const LatticeCell& point3) const {
   const S6 p1S6(point1.getCell());
   const S6 p2S6(point2.getCell());
   const S6 p3S6(point3.getCell());
   const std::string type1 = point1.getLatticeType();
   const std::string type2 = point2.getLatticeType();

   Path path;
   path.reserve(controlVars.numFollowerPoints);

   for (int i = 0; i < controlVars.numFollowerPoints; ++i) {
      const double t = i / static_cast<double>(controlVars.numFollowerPoints - 1);
      S6 mobile1 = p1S6 * (1.0 - t) + p3S6 * t;
      S6 mobile2 = p2S6 * (1.0 - t) + p3S6 * t;
      if (!PathPointIsValid(mobile1)) mobile1 = MakeInvalidS6();
      if (!PathPointIsValid(mobile2)) mobile2 = MakeInvalidS6();
      path.emplace_back(LatticeCell(G6(mobile1), type1), LatticeCell(G6(mobile2), type2));
   }
   return path;
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

   // Scale the orthogonal vector
   const double inputNorm = std::sqrt(inputNormSquared);
   const double orthogonalNorm = std::sqrt(orthogonalVector.Dot(orthogonalVector));
   const double scaleFactor = inputNorm * controlVars.perturbBy / orthogonalNorm;
   const S6 scaledOrthogonalVector = scaleFactor * orthogonalVector;

   // Return perturbed vector with same lattice type
   return LatticeCell(G6(inputS6 + scaledOrthogonalVector), inputVector.getLatticeType());
}

