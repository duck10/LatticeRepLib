#ifndef FOLLOW_H
#define FOLLOW_H

#include "FollowControls.h"
#include "GlitchDetector.h"
#include "Distance.h"
#include "Path.h"
#include "FollowInstance.h"

class Follow {
public:
   explicit Follow(FollowControls& controls);

   bool processPerturbation(int trialNum, int perturbationNum, const FollowInstance& instance);
   void PrintPathData(const Path& path) const;

private:
   Path generatePath(const int trialNum, int perturbationNum, const std::vector<LatticeCell>& perturbedPoints);
   LatticeCell perturbVector(const LatticeCell& inputVector, const int perturbationIndex) const;

private:
   FollowControls& controls;
   GlitchDetector glitchDetector;
   std::vector<std::unique_ptr<Distance>> distfuncs;
};

#endif // FOLLOW_H

