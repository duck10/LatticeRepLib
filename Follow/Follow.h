#ifndef FOLLOW_H
#define FOLLOW_H

#include "FollowControls.h"
#include "GlitchDetector.h"
#include "Distance.h"
#include "Path.h"
#include "FollowInstance.h"

#include <memory>

class Follow {
public:
   explicit Follow(FollowControls& controls);

   bool processPerturbation(int trialNum, int perturbationNum, const FollowInstance& instance,
      const std::vector<LatticeCell>& cells);

   void PrintPathData(const Path& path) const;

   Path generatePath(const int trialNum, const int perturbationNum,
      const std::vector<LatticeCell>& perturbedPoints,
      const std::vector<LatticeCell>& cells) const;
   LatticeCell perturbVector(const LatticeCell& inputVector, const int perturbationIndex) const;

private:
   std::vector<Glitch> DetectGlitchesWithS6Data(
      const std::vector<double>& distances,
      const Path& path,
      const std::string& distanceType) const;


   void WriteSvgDistanceFile(const std::string& filename,
      const std::vector<std::vector<double>>& allDistances,
      const std::vector<std::unique_ptr<Distance>>& distfuncs,
      const std::vector<S6>& path,
      int trialNum, int perturbationNum);

   FollowControls& controls;
   std::vector<std::vector<double>> allDistances;  // Add this member
   GlitchDetector glitchDetector;
   std::vector<std::unique_ptr<Distance>> distfuncs;
};

#endif // FOLLOW_H

