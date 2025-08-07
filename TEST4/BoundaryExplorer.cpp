#include "BoundaryExplorer.h"
#include "FractureTagger.h"
#include "P3.h"
#include "P3SpecialGenerator.h"
#include "P3Utilities.h"
#include "ScalarTraits.h"
#include "StoreResults.h"
#include "StableTransitionCase.h"
#include "TransistionCase.h"

#include <iomanip>
#include <algorithm>
#include <numeric>
#include <string>

extern StoreResults<std::string, TransitionCase> edgeTransitionStore;
extern StoreResults<std::string, StableTransitionCase> stableResultStore;

static const Matrix_3x3 identity{ 1,0,0, 0,1,0, 0,0,1 };


std::string GetTransformationSignature(const P3& reduced) {
   const std::vector<double> vec = reduced.GetFlatVector();
   const double epsilon = 1e-5;

   std::string signature;

   // Sign pattern
   signature += "S:";
   for (const double val : vec) {
      if (std::abs(val) < epsilon) signature += "0";
      else if (val > 0) signature += "+";
      else signature += "-";
   }

   // Near-zero count
   int nearZeroCount = 0;
   for (const double val : vec) {
      if (std::abs(val) < epsilon) ++nearZeroCount;
   }
   signature += "|Z:" + std::to_string(nearZeroCount);

   // Permutation class (sorted index pattern)
   std::vector<std::pair<double, int>> indexed;
   for (size_t i = 0; i < vec.size(); ++i) {
      indexed.emplace_back(vec[i], static_cast<int>(i));
   }
   std::sort(indexed.begin(), indexed.end());

   signature += "|P:";
   for (const auto& pair : indexed) {
      signature += std::to_string(pair.second);
   }

   // Optional: symmetry hint (e.g. repeated values)
   std::map<int, int> valueBuckets;
   for (const double val : vec) {
      int bucket = static_cast<int>(val / epsilon);  // crude binning
      valueBuckets[bucket]++;
   }

   int repeatedBuckets = 0;
   for (const auto& kv : valueBuckets) {
      if (kv.second > 1) ++repeatedBuckets;
   }
   signature += "|R:" + std::to_string(repeatedBuckets);

   return signature;
}

std::string MakeStableKey(int binA, int binB, double distance) {
   std::ostringstream oss;
   oss << "TRANSFORM[bin" << binA << "→bin" << binB << "]";
   oss << ":CLASS[Stable]";
   return oss.str();
}

static int GetDistanceBin(const double finalDistance) {
   return static_cast<int>(std::log10(finalDistance * 1000));  // Tunable scale
}

static int GetDistanceBin(const P3& a, const P3& b) {
   return static_cast<int>(std::log10((a - b).norm() * 1000));  // Tunable scale
}

void BoundaryExplorer::RunTrial(const P3& baseCell, const P3& perturbedCell) const {
   const LRL_Cell cellBase(baseCell);
   const LRL_Cell cellPerturbed(perturbedCell);

   const P3 reducedBase(ReduceCellScalar(baseCell));
   const P3 reducedPert(ReduceCellScalar(LRL_Cell(perturbedCell)));


  const ReducedResult testBase = P3_Reduce::MatrixAwareReduce(baseCell);
  const ReducedResult testPerturbed = P3_Reduce::MatrixAwareReduce(perturbedCell);

  if (baseCell != reducedBase) {
     const bool test = perturbedCell == testPerturbed.reduced;
     const int i19191 = 19191;
  }
  if (reducedPert!= testPerturbed.reduced)
  {
     std::cout << " reduce by ReduceCellScalar  " << reducedBase << std::endl;
     std::cout << " reduce by MatrixAwareReduce " << testBase.reduced << std::endl << std::endl;
     std::cout << " reduce by ReduceCellScalar  " << LRL_Cell_Degrees(reducedBase) << std::endl;
     std::cout << " reduce by MatrixAwareReduce " << LRL_Cell_Degrees(testBase.reduced) << std::endl << std::endl;
     const int i19191 = 19191;
  }

  if (testBase.transformation != identity || testPerturbed.transformation != identity) {
     const int i19191 = 19191;
  }
  if (testBase.reduced !=  testPerturbed.reduced) {
     const int i19191 = 19191;
  }


   const LRL_Cell cellReducedBase(reducedBase);
   const LRL_Cell cellReducedPerturbed(reducedPert);

   const int binSeverity = GetDistanceBin(reducedBase, reducedPert);
   const int binA = GetDistanceBin(reducedBase.norm());
   const int binB = GetDistanceBin(reducedPert.norm());

   FractureSummary summary;
   summary.initialDistance = P3::EucledianDistance(baseCell, perturbedCell);
   summary.finalDistance = P3::EucledianDistance(reducedBase, reducedPert);

   const double fractureThreshold = 0.1;
   summary.fractureDetected = (summary.finalDistance > fractureThreshold);
   summary.repairSucceeded = !summary.fractureDetected;
   summary.note = "Fracture check: threshold = 0.1";

   std::string diagnosticTag = FractureTagger::Tag(summary);



   if (!perturbedCell.GetValid()) {
      diagnosticTag += "_InvalidInput";
   }

   int binKey = GetDistanceBin(summary.finalDistance);

   {
      TransitionCase tc;
      tc.baseCell = baseCell;
      tc.perturbedCell = perturbedCell;
      tc.reducedBase = reducedBase;
      tc.reducedPerturbed = reducedPert;
      tc.binBase = binA;
      tc.binPerturbed = binB;
      tc.crossedBoundary = (binA != binB);
      tc.summary = summary;

      const ScalarTraits traits = AnalyzeScalarTraits(reducedBase);
      tc.typeLabel = traits.Tag();  // or tc.tag = traits.Tag();

      const std::string key = traits.Tag() + "_" + std::to_string(tc.binBase) + "->" + std::to_string(tc.binPerturbed);

      const P3 p3temp(ReduceCellScalar(LRL_Cell(perturbedCell)));
      tc.originalVector= p3temp;  // Before reduction
      tc.finalVector = reducedPert;                                   // After reduction
      edgeTransitionStore.Store(tc.key, tc);

   }

   {
      static const Matrix_3x3 mdummy(19191, 19191, 19191, 19191, 19191, 19191, 19191, 19191, 19191);
      StableTransitionCase entry = { binA, binB, summary.finalDistance, mdummy, "Stable" };
      std::string key = MakeStableKey(binA, binB, summary.finalDistance);
      stableResultStore.Store(key, entry);
   }

}
