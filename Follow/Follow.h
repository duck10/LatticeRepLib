#ifndef FOLLOW_H
#define FOLLOW_H

#include "ControlVariables.h"
#include "Distance.h"
#include "DistanceFactory.h"
#include "GlitchDetector.h"
#include "LinearAxis.h"
#include "S6.h"
#include "G6.h"
#include "Polar.h"

#include <vector>
#include <memory>
#include <string>

using PathPoint = std::pair<S6, S6>;
using Path = std::vector<PathPoint>;

class Follow {
public:
   Follow(ControlVariables& cv);
   void run(const std::vector<std::string>& filenames, const std::vector<G6>& inputVectors);

private:
   ControlVariables& controlVars;
   std::vector<std::unique_ptr<Distance>> distances;
   GlitchDetector glitchDetector;
   std::vector<G6> inputVectors;
   std::vector<std::string> filenames;  // Add this line if it's not already there

   void processAllTrials();
   void processTrial(int trialNum);
   void processPerturbation(int trialNum, int perturbationNum, const std::vector<S6>& perturbedPoints);
   void PrintDistanceData(const Path& path);
   S6 perturbVector(const S6& inputVector, int perturbationIndex);

   Path generatePath(const int trialNum, int perturbationNum, const std::vector<S6>& perturbedPoints);
   Path generatePointPath(const S6& startPoint);
   Path generateLinePath(const S6& startPoint, const S6& endPoint);
   Path generateChordPath(const S6& startPoint, const S6& endPoint);
   Path generateChord3Path(const S6& mobile1, const S6& mobile2, const S6& target);
   Path generateTrianglePath(const S6& point1, const S6& point2, const S6& point3);

   };


#endif // FOLLOW_H

