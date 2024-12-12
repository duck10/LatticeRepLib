#ifndef FOLLOW_H
#define FOLLOW_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

#include "ControlVariables.h"
#include "DistanceFactory.h"
#include "GlitchDetector.h"
#include "FollowInstance.h"
#include "LatticeCell.h"
#include "PathPoint.h"

class Follow {
private:
   ControlVariables& controlVars;
   GlitchDetector glitchDetector;
   std::vector<std::unique_ptr<Distance>> distfuncs;
   std::vector<std::string> filenames;
   std::vector<LatticeCell> inputVectors;

   LatticeCell perturbVector(const LatticeCell& inputVector, const int perturbationIndex) const;
   void PrintDistanceData(const Path& path);
   Path generatePath(const int trialNum, int perturbationNum, const std::vector<LatticeCell>& perturbedPoints);
   Path generatePointPath(const LatticeCell& startPoint) const;
   Path generateLinePath(const LatticeCell& startPoint, const LatticeCell& endPoint) const;
   Path generateChordPath(const LatticeCell& point1, const LatticeCell& point2) const;
   Path generateChord3Path(const LatticeCell& mobile1, const LatticeCell& mobile2, const LatticeCell& target) const;
   Path generateTrianglePath(const LatticeCell& point1, const LatticeCell& point2, const LatticeCell& point3) const;

public:
   explicit Follow(ControlVariables& cv);
   void processPerturbation(int trialNum, int perturbationNum, const FollowInstance& followStart);
};

#endif // FOLLOW_H

