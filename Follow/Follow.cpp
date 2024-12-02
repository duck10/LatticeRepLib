#pragma warning (disable: 4996)

#include "Follow.h"
#include "GlitchDetector.h"
#include "LinearAxis.h"
#include "Niggli.h"
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
void Follow::run(const std::vector<std::string>& filenames, const std::vector<G6>& inputVectors) {
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
   std::vector<S6> startingPoints;
   const int vectorsPerTrial = controlVars.getVectorsPerTrial();

   // Get the starting points for this trial
   for (int i = 0; i < vectorsPerTrial; ++i) {
      startingPoints.emplace_back(S6(inputVectors[trialNum * vectorsPerTrial + i]));
   }

   for (int perturbation = 0; perturbation < controlVars.perturbations; ++perturbation) {
      std::vector<S6> perturbedPoints;
      for (const auto& point : startingPoints) {
         perturbedPoints.push_back(perturbVector(point, perturbation));
      }
      processPerturbation(trialNum, perturbation, perturbedPoints);
   }
}

void Follow::PrintDistanceData(const Path& path) {
   if (controlVars.printDistanceData)
   {
      std::cout << "; the path" << std::endl;
      for (const auto& point : path) {
         const S6& pnt1(point.first);
         const S6& pnt2(point.second);
         const std::string invalid1 = (pnt1.IsValid()) ? "" : " invalid";
         const std::string invalid2 = (pnt2.IsValid()) ? "" : " invalid";
         std::cout << "; S6 " << pnt1 << invalid1 << "\n; S6 "
            << pnt2 << invalid2 << std::endl << std::endl;
      }
   }
   std::cout << ";--------  end of path ----------------- \n";

}

bool PathPointIsValid(const S6& p) {
   return(LRL_Cell(p).IsValid());
}

void Follow::processPerturbation(int trialNum, int perturbationNum, const std::vector<S6>& perturbedPoints) {
   const Path path = generatePath(trialNum, perturbationNum, perturbedPoints);
   controlVars.updatePathStart( perturbedPoints);
   controlVars.updatePath(path);
   if (path.empty()) return;

   if (controlVars.printDistanceData) {
      PrintDistanceData(path);
   }

   std::vector<std::vector<double>> allDistances;

   // Just calculate the distfuncs without trying to copy the Distance objects
   for (const auto& dist : distfuncs) {
      std::vector<double> pathDists;
      pathDists.reserve(path.size());
      for (const auto& [point1, point2] : path) {
         if (PathPointIsValid(point1) && PathPointIsValid(point2)) {
            pathDists.emplace_back(dist->dist(G6(point1), G6(point2)));
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

Path Follow::generatePath(const int trialNum, int perturbationNum, const std::vector<S6>& perturbedPoints) {
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

Path Follow::generatePointPath(const S6& startPoint) const {
   G6 niggliReduced;
   Niggli::Reduce(G6(startPoint), niggliReduced);
   const S6 endPoint(niggliReduced);

   Path path;
   path.reserve(controlVars.numFollowerPoints);

   // Go from startPoint to niggliReduced and back
   for (int i = 0; i < controlVars.numFollowerPoints; ++i) {
      const double t = i / static_cast<double>(controlVars.numFollowerPoints - 1);
      S6 currentPoint = startPoint * (1.0 - t) + endPoint * t;
      if (!PathPointIsValid(currentPoint)) currentPoint = MakeInvalidS6();
      path.emplace_back(currentPoint, endPoint);
   }
   return path;
}

Path Follow::generateLinePath(const S6& startPoint, const S6& endPoint) const {
   Path path;
   path.reserve(controlVars.numFollowerPoints);

   for (int i = 0; i < controlVars.numFollowerPoints; ++i) {
      const double t = i / static_cast<double>(controlVars.numFollowerPoints - 1);
      S6 currentPoint = startPoint * (1.0 - t) + endPoint * t;
      if (!PathPointIsValid(currentPoint)) currentPoint = MakeInvalidS6();
      path.emplace_back(currentPoint, endPoint);
   }

   return path;
}


Path Follow::generateChordPath(const S6& point1, const S6& point2) const {
   G6 reduced1G6, reduced2G6;
   Niggli::Reduce(G6(point1), reduced1G6);
   Niggli::Reduce(G6(point2), reduced2G6);
   const S6 reduced1(reduced1G6);
   const S6 reduced2(reduced2G6);

   Path path;
   path.reserve(controlVars.numFollowerPoints);

   for (int i = 0; i < controlVars.numFollowerPoints; ++i) {
      const double t = i / static_cast<double>(controlVars.numFollowerPoints - 1);
      S6 mobile1 = point1 * (1.0 - t) + reduced1 * t;
      S6 mobile2 = point2 * (1.0 - t) + reduced2 * t;
      if (!PathPointIsValid(mobile1)) mobile1 = MakeInvalidS6();
      if (!PathPointIsValid(mobile2)) mobile2 = MakeInvalidS6();
      path.emplace_back(mobile1, mobile2);
   }

   return path;
}

Path Follow::generateChord3Path(const S6& mobile1, const S6& mobile2, const S6& target) const {
   S6 initialDirection = mobile2 - mobile1;
   const double separation = initialDirection.Norm();
   initialDirection = initialDirection * (1.0 / separation);
   const S6 initialMidpoint = (mobile1 + mobile2) * 0.5;

   Path path;
   path.reserve(controlVars.numFollowerPoints);

   for (int i = 0; i < controlVars.numFollowerPoints; ++i) {
      const double t = i / static_cast<double>(controlVars.numFollowerPoints - 1);
      S6 currentMidpoint = initialMidpoint * (1.0 - t) + target * t;
      S6 point1 = currentMidpoint - initialDirection * (separation * 0.5);
      S6 point2 = currentMidpoint + initialDirection * (separation * 0.5);
      if (!PathPointIsValid(point1)) point1 = MakeInvalidS6();
      if (!PathPointIsValid(point2)) point2 = MakeInvalidS6();
      path.emplace_back(point1, point2);
   }

   return path;
}

Path Follow::generateTrianglePath(const S6& point1, const S6& point2, const S6& point3) const {
   Path path;
   path.reserve(controlVars.numFollowerPoints);

   for (int i = 0; i < controlVars.numFollowerPoints; ++i) {
      const double t = i / static_cast<double>(controlVars.numFollowerPoints - 1);
      S6 mobile1 = point1 * (1.0 - t) + point3 * t;
      S6 mobile2 = point2 * (1.0 - t) + point3 * t;
      if (!PathPointIsValid(mobile1)) mobile1 = MakeInvalidS6();
      if (!PathPointIsValid(mobile2)) mobile2 = MakeInvalidS6();
      path.emplace_back(mobile1, mobile2);
   }

   return path;
}

S6 Follow::perturbVector(const S6& inputVector, const int perturbationIndex) const {
   if (perturbationIndex == 0) {
      return inputVector;  // No perturbation for the first case
   }

   // Generate a random vector
   const S6 randomVector = S6(Polar::rand());

   // Compute the component of randomVector orthogonal to inputVector
   const double dotProduct = inputVector.Dot(randomVector);
   const double inputNormSquared = inputVector.Dot(inputVector);
   const S6 orthogonalVector = randomVector - (dotProduct / inputNormSquared) * inputVector;

   // Compute norms
   const double inputNorm = std::sqrt(inputNormSquared);
   const double orthogonalNorm = std::sqrt(orthogonalVector.Dot(orthogonalVector));

   // Scale the orthogonal vector
   const double scaleFactor = inputNorm * controlVars.perturbBy / orthogonalNorm;
   const S6 scaledOrthogonalVector = scaleFactor * orthogonalVector;

   // Return the perturbed vector
   return inputVector + scaledOrthogonalVector;
}
