#ifndef DISTANCECALCULATOR_h
#define DISTANCECALCULATOR_h
#include <string>
#include <vector>

enum class FollowerMode {
   POINT,
   LINE,
   CHORD,
   CHORD3,
   TRIANGLE
};

//class ControlVariables {
//public:
//   ControlVariables()
//      : followerMode(FollowerMode::POINT)
//      , trials(1)
//      , numFollowerPoints(100)
//      , perturbationAmount(0.1)
//      , filePrefix("follow_output")
//   {}
//
//   // Control parameters
//   FollowerMode followerMode;
//   int trials;
//   int numFollowerPoints;
//   double perturbationAmount;
//
//   // File handling
//   std::string filePrefix;
//   std::vector<std::string> filenames;
//
//   // Distance calculation flags
//   bool useCS6Dist = true;
//   bool useS6Dist = true;
//   bool useNCDist = true;
//   bool useCS6Dist_C = true;
//};

#endif // DISTANCECALCULATOR_h
