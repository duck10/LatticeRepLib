#include "SearchMatrixBuilder.h"
#include "TransformationMatrices.h"

#include <cmath>

namespace {

   // det=2:   U(2) alone -- full brute-force search, coefficient range +/-2.
   // det=3:   U(3) concatenated with HNF(3) (HNF(3)'s matrices already have
   //          every entry <=3, so they are already inside U(3)'s range;
   //          harmless but redundant, and small enough not to bother
   //          deduplicating).
   // det=4:   U(4) concatenated with HNF(4), same reasoning. This is the
   //          set that fixes the PvsICentering counterexample.
   // det=5,6: full brute force is not attempted -- U(1) (3,480 order-1
   //          unimodular matrices) composed with HNF(5)/HNF(6)
   //          individually, in BOTH multiplication orders (U*HNF and
   //          HNF*U -- matrix multiplication doesn't commute).
   //          KNOWN LIMITATION: not proven complete for det=5,6 -- an
   //          empirical improvement over HNF alone, not a proof.
   //
   // NOTE: det=3 and det=4's own internal concatenation (brute force +
   // HNF) is a one-time, small, fixed-size append (13 and 35 extra
   // matrices respectively) done ONCE when each vector is first built --
   // this is NOT the same kind of repeated/redundant copying that caused
   // the earlier bad_alloc crashes (that came from concatenating entire
   // multi-million-matrix vectors together into yet another vector of the
   // same size, done at the top level, and again per ratio-selection call).
   constexpr int kComposeUnimodularOrder = 1;  // gives exactly 3,480 matrices

   std::vector<Matrix_3x3> composeBothOrders(
      const std::vector<Matrix_3x3>& u,
      const std::vector<Matrix_3x3>& hnf) {
      std::vector<Matrix_3x3> result;
      result.reserve(u.size() * hnf.size() * 2);
      for (const auto& a : u)
         for (const auto& b : hnf)
            result.push_back(a * b);
      for (const auto& b : hnf)
         for (const auto& a : u)
            result.push_back(b * a);
      return result;
   }

   // Per-determinant caches, built once eagerly on first access via
   // ensureBuilt(). Each determinant's matrices exist in memory exactly
   // once -- no concatenated combined copies are stored anywhere.
   std::vector<Matrix_3x3> g_det1;
   std::vector<Matrix_3x3> g_det2;
   std::vector<Matrix_3x3> g_det3;
   std::vector<Matrix_3x3> g_det4;
   std::vector<Matrix_3x3> g_det5;
   std::vector<Matrix_3x3> g_det6;
   std::vector<Matrix_3x3> g_hnfAll;  // raw HNF(2..6), 177 matrices -- USEHNF=true path
   bool g_built = false;

   void ensureBuilt(const MultiTransformFinderControls& controls) {
      if (g_built) return;

      g_det1 = TransformationMatrices::generateUnimodularMatrices(controls.getMatrixOrder());
      g_hnfAll = TransformationMatrices::generateHNFMatrices({ 2, 3, 4, 5, 6 });

      const auto unimodular1 = TransformationMatrices::generateUnimodularMatrices(kComposeUnimodularOrder);

      g_det2 = TransformationMatrices::generateSupercellMatrices({ 2 }, 2);

      g_det3 = TransformationMatrices::generateSupercellMatrices({ 3 }, 3);
      const auto hnf3 = TransformationMatrices::generateHNFMatrices({ 3 });
      g_det3.insert(g_det3.end(), hnf3.begin(), hnf3.end());

      g_det4 = TransformationMatrices::generateSupercellMatrices({ 4 }, 4);
      const auto hnf4 = TransformationMatrices::generateHNFMatrices({ 4 });
      g_det4.insert(g_det4.end(), hnf4.begin(), hnf4.end());

      const auto hnf5 = TransformationMatrices::generateHNFMatrices({ 5 });
      g_det5 = composeBothOrders(unimodular1, hnf5);

      const auto hnf6 = TransformationMatrices::generateHNFMatrices({ 6 });
      g_det6 = composeBothOrders(unimodular1, hnf6);

      g_built = true;
   }

} // namespace

size_t totalMatrixCount(const MatrixGroups& groups) {
   size_t total = 0;
   for (const auto& g : groups) total += g.get().size();
   return total;
}

std::vector<Matrix_3x3> composeMatrixSets(
   const std::vector<Matrix_3x3>& lhs,
   const std::vector<Matrix_3x3>& rhs) {
   std::vector<Matrix_3x3> composed;
   composed.reserve(lhs.size() * rhs.size());
   for (const auto& u : lhs)
      for (const auto& s : rhs)
         composed.push_back(u * s);
   return composed;
}

const std::vector<Matrix_3x3>& getDet1Matrices(const MultiTransformFinderControls& controls) { ensureBuilt(controls); return g_det1; }
const std::vector<Matrix_3x3>& getDet2Matrices(const MultiTransformFinderControls& controls) { ensureBuilt(controls); return g_det2; }
const std::vector<Matrix_3x3>& getDet3Matrices(const MultiTransformFinderControls& controls) { ensureBuilt(controls); return g_det3; }
const std::vector<Matrix_3x3>& getDet4Matrices(const MultiTransformFinderControls& controls) { ensureBuilt(controls); return g_det4; }
const std::vector<Matrix_3x3>& getDet5Matrices(const MultiTransformFinderControls& controls) { ensureBuilt(controls); return g_det5; }
const std::vector<Matrix_3x3>& getDet6Matrices(const MultiTransformFinderControls& controls) { ensureBuilt(controls); return g_det6; }

MatrixGroups generateSearchMatrixGroups(const MultiTransformFinderControls& controls) {
   ensureBuilt(controls);

   const bool useHNF = controls.useHNF();

   if (controls.runModeEquivalent()) {
      return { g_det1 };
   }
   if (controls.runModeSuper()) {
      if (useHNF) return { g_hnfAll };
      return { g_det2, g_det3, g_det4, g_det5, g_det6 };
   }
   if (controls.runModeComposed()) {
      return { g_det2, g_det3, g_det4, g_det5, g_det6 };  // same set as SUPER/non-HNF
   }
   // controls.getRunMode() == RunMode::ALL
   if (useHNF) return { g_det1, g_hnfAll };
   return { g_det1, g_det2, g_det3, g_det4, g_det5, g_det6 };
}

MatrixGroups selectMatrixGroupsForRatio(
   const MultiTransformFinderControls& controls, double ratio) {
   const double halfWidth = 0.5 + controls.getRatioTolerance();

   MatrixGroups groups;
   groups.push_back(getDet1Matrices(controls));  // always included

   if (std::abs(2.0 - ratio) <= halfWidth) groups.push_back(getDet2Matrices(controls));
   if (std::abs(3.0 - ratio) <= halfWidth) groups.push_back(getDet3Matrices(controls));
   if (std::abs(4.0 - ratio) <= halfWidth) groups.push_back(getDet4Matrices(controls));
   if (std::abs(5.0 - ratio) <= halfWidth) groups.push_back(getDet5Matrices(controls));
   if (std::abs(6.0 - ratio) <= halfWidth) groups.push_back(getDet6Matrices(controls));

   return groups;
}
