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
#include "LatticeCell.h"
#include "PathPoint.h"

class Follow {
private:
   ControlVariables& controlVars;
   GlitchDetector glitchDetector;
   std::vector<std::unique_ptr<Distance>> distfuncs;
   std::vector<std::string> filenames;
   std::vector<LatticeCell> inputVectors;

   void processTrial(int trialNum);
   void processPerturbation(int trialNum, int perturbationNum, const std::vector<LatticeCell>& perturbedPoints);
   LatticeCell perturbVector(const LatticeCell& inputVector, const int perturbationIndex) const;
   void PrintDistanceData(const Path& path);
   Path generatePath(const int trialNum, int perturbationNum, const std::vector<LatticeCell>& perturbedPoints);
   Path generatePointPath(const LatticeCell& startPoint) const;
   Path generateLinePath(const LatticeCell& startPoint, const LatticeCell& endPoint) const;
   Path generateChordPath(const LatticeCell& point1, const LatticeCell& point2) const;
   Path generateChord3Path(const LatticeCell& mobile1, const LatticeCell& mobile2, const LatticeCell& target) const;
   Path generateTrianglePath(const LatticeCell& point1, const LatticeCell& point2, const LatticeCell& point3) const;

public:
   Follow(ControlVariables& cv);
   void run(const std::vector<std::string>& filenames, const std::vector<LatticeCell>& inputVectors);
   void run(const std::vector<std::string>& filenames, const std::vector<LatticeCell>& inputVectors, 
        size_t blockstart, size_t blocksize);
   void processAllTrials();
};

#endif // FOLLOW_H

