#ifndef FOLLOW_H
#define FOLLOW_H

#include "ControlVariables.h"
#include "Distance.h"
#include "DistanceFactory.h"
#include "GlitchDetector.h"
#include "LinearAxis.h"
#include "PathPoint.h"
#include "S6.h"
#include "G6.h"
#include "Polar.h"

#include <vector>
#include <memory>
#include <string>

class Follow {
public:
   Follow(ControlVariables& cv);
   void run(const std::vector<std::string>& filenames, const std::vector<G6>& inputVectors);

private:
   ControlVariables& controlVars;
   std::vector<std::unique_ptr<Distance>> distfuncs;
   GlitchDetector glitchDetector;
   std::vector<G6> inputVectors;
   std::vector<std::string> filenames;  // Add this line if it's not already there

   void processAllTrials();
   void processTrial(int trialNum);
   void processPerturbation(int trialNum, int perturbationNum, const std::vector<S6>& perturbedPoints);
   void PrintDistanceData(const Path& path);
   S6 perturbVector(const S6& inputVector, int perturbationIndex) const;

   Path generatePath(const int trialNum, int perturbationNum, const std::vector<S6>& perturbedPoints);
   Path generatePointPath(const S6& startPoint) const;
   Path generateLinePath(const S6& startPoint, const S6& endPoint) const;
   Path generateChordPath(const S6& startPoint, const S6& endPoint) const;
   Path generateChord3Path(const S6& mobile1, const S6& mobile2, const S6& target) const;
   Path generateTrianglePath(const S6& point1, const S6& point2, const S6& point3) const;

   };


#endif // FOLLOW_H

