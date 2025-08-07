#pragma warning (disable: 4189) // Visual Studio -- local variable is initialized but not referenced

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "B4.h"
#include "Matrix_3x3.h"
#include "P3.h"
#include "P3_Reduce.h"
#include "Polar.h"
#include "P3Dist.h"
#include "TransformationMatrices.h"
#include "G6.h"
#include "S6.h"
#include "P3Utilities.h"
#include "GenerateLatticeTypeExamples.h"
#include "P3SpecialGenerator.h"

#include "StoreResults.h"  // Already included elsewhere, if needed

#include "LRL_Vector3.h"
#include "BoundaryExplorer.h"
#include "TransistionCase.h"
#include "StableTransitionCase.h"

template<typename TKEY, typename TDATA>
void ShowScalarDistanceStats(const StoreResults<TKEY, TDATA>& store, std::function<double(const TDATA&)> extractDistance) {
   std::vector<TKEY> keys = store.GetKeys();
   std::cout << "\nKey-wise Scalar Distance Summary\n";
   std::cout << "---------------------------------\n";
   std::cout << "Key                             Count    Min      Max      Avg\n";

   for (const TKEY& key : keys) {
      std::vector<std::pair<TKEY, TDATA>> samples = store.GetResult(key);
      if (samples.empty()) continue;

      double minVal = std::numeric_limits<double>::max();
      double maxVal = std::numeric_limits<double>::lowest();
      double total = 0.0;

      for (const auto& entry : samples) {
         double val = extractDistance(entry.second);
         minVal = std::min(minVal, val);
         maxVal = std::max(maxVal, val);
         total += val;
      }
      double avgVal = total / samples.size();
      std::cout << std::setw(30) << std::left << key
         << "  " << std::setw(6) << samples.size()
         << "  " << std::setw(7) << std::fixed << std::setprecision(3) << minVal
         << "  " << std::setw(7) << maxVal
         << "  " << std::setw(7) << avgVal << "\n";
   }
}


LRL_Cell GetCellFromBaseVectors(const Vector_3& a, const Vector_3& b, const Vector_3& c) {
   return { a.norm(),b.norm(),c.norm(), Angle(b,c),Angle(a,c),Angle(a,b) };
}

//static S6 PerturbS6(const S6& s6, const double perturbationDelta) {
//   //GenerateLatticeTypeExamples();
//   const S6 unitOrthogonal = CreateUnitOrthogonalComponent(s6);  // norm = 1.0
//   const S6 unitInput = s6 / s6.norm();
//   return 1000.0 * (unitInput + perturbationDelta * unitOrthogonal);
//}

P3 ApplyMatrixToP3(const P3& p3, const Matrix_3x3& m) {
   return P3(m * LRL_Cell(p3));
}


 StoreResults<std::string, TransitionCase> edgeTransitionStore(5);
 StoreResults<std::string, StableTransitionCase> stableResultStore(5);

int main() {
   const int numTrials = 2000;
   const double perturbationMagnitude = 0.05;

   for (int trialID = 0; trialID < numTrials; ++trialID) {
      if (trialID == 525) {
         const int i19191 = 19191;
      }
      const std::pair<P3, P3> perturbedPair = P3SpecialGenerator::GenerateRandomPair(perturbationMagnitude);
      BoundaryExplorer explorer(perturbationMagnitude);  // Your trial handler
      explorer.RunTrial(perturbedPair);
   }

   std::cout << "perturbation magnitude " << perturbationMagnitude << std::endl;

   stableResultStore.SetTitle("Cases with small final distances");
   stableResultStore.ShowResults();

   ShowScalarDistanceStats<std::string, TransitionCase>(
      edgeTransitionStore,
      [](const TransitionCase& tc) { return tc.summary.finalDistance; });

   edgeTransitionStore.ShowResults();

   return 0;
}