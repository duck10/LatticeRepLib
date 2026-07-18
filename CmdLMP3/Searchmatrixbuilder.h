#ifndef SEARCHMATRIXBUILDER_H
#define SEARCHMATRIXBUILDER_H

#include "MultiTransformFinderControls.h"
#include "Matrix_3x3.h"

#include <functional>
#include <vector>

// ---------------------------------------------------------------------------
// MatrixGroups -- a non-owning list of references to existing, already-
// cached matrix vectors (e.g. {det1, det3, det4} for a ratio-narrowed
// search). NOTHING here is ever concatenated or copied into a new combined
// vector: callers iterate across each referenced group in turn. This is
// deliberate -- an earlier version of this file assembled combined vectors
// by concatenation (e.g. composedSuper = det2 ++ det3 ++ det4 ++ det5 ++
// det6, then allComposed = det1 ++ composedSuper), which meant the same
// ~8.78M matrices were stored three times over in static memory, and every
// call to the ratio-based selector made yet another full copy on top of
// that. That redundant copying (and the reallocate-and-copy overhead of
// growing those vectors via repeated .insert() with no .reserve()) is what
// caused intermittent bad_alloc crashes at higher RATIOTOLERANCE values.
// Each determinant's matrices now exist in memory exactly once.
// ---------------------------------------------------------------------------
using MatrixGroupRef = std::reference_wrapper<const std::vector<Matrix_3x3>>;
using MatrixGroups = std::vector<MatrixGroupRef>;

// Total matrix count across all groups (sum of each group's size) -- for
// display/logging, without needing to actually iterate the matrices.
size_t totalMatrixCount(const MatrixGroups& groups);

// ---------------------------------------------------------------------------
// generateSearchMatrixGroups -- the full search set for a given RunMode/
// USEHNF combination, as a list of groups to iterate (never concatenated).
// Everything is built once, eagerly, the first time any function in this
// file is called -- regardless of which RunMode/USEHNF combination made
// that first call -- and simply selected/referenced afterward. This
// matters: a single process (e.g. one run of the flexible test suite) may
// call these with several different RunMode/USEHNF combinations, and each
// must get the right, fully-computed set every time, not something built
// lazily (and only once) for whichever combination happened to be
// requested first.
// ---------------------------------------------------------------------------
MatrixGroups generateSearchMatrixGroups(const MultiTransformFinderControls& controls);

// Named helper (not a lambda): the Cartesian product of two matrix sets,
// u * s for every u in lhs, s in rhs. Used internally when building the
// det=5,6 composed sets; exposed here in case other composition needs
// arise.
std::vector<Matrix_3x3> composeMatrixSets(
   const std::vector<Matrix_3x3>& lhs,
   const std::vector<Matrix_3x3>& rhs);

// ---------------------------------------------------------------------------
// Per-determinant accessors -- references to the SAME cached vectors
// generateSearchMatrixGroups() draws from. Calling any of these forces the
// one-time construction if it hasn't happened yet.
// ---------------------------------------------------------------------------
const std::vector<Matrix_3x3>& getDet1Matrices(const MultiTransformFinderControls& controls);
const std::vector<Matrix_3x3>& getDet2Matrices(const MultiTransformFinderControls& controls);
const std::vector<Matrix_3x3>& getDet3Matrices(const MultiTransformFinderControls& controls);
const std::vector<Matrix_3x3>& getDet4Matrices(const MultiTransformFinderControls& controls);
const std::vector<Matrix_3x3>& getDet5Matrices(const MultiTransformFinderControls& controls);
const std::vector<Matrix_3x3>& getDet6Matrices(const MultiTransformFinderControls& controls);

// ---------------------------------------------------------------------------
// selectMatrixGroupsForRatio -- assembles a (non-owning) list of groups
// plausible for a given reference/mobile volume ratio. A determinant d is
// included whenever |d - ratio| <= 0.5 + controls.getRatioTolerance() (see
// the RATIOTOLERANCE keyword). det=1 is ALWAYS included regardless of
// ratio -- it is cheap (3,480 matrices) and a supercell case can still turn
// out to have a better det=1 match after reduction.
//
// Returns a freshly-assembled MatrixGroups (a small vector of reference_
// wrappers -- NOT a copy of the underlying matrices) -- the ratio differs
// per lattice pair, so the *selection* can't be cached, but nothing it
// selects is ever copied.
// ---------------------------------------------------------------------------
MatrixGroups selectMatrixGroupsForRatio(
   const MultiTransformFinderControls& controls, double ratio);

// Same selection logic, but with the tolerance passed explicitly instead of
// read from controls.getRatioTolerance(). controls is still needed here --
// it supplies the underlying per-determinant matrix caches via
// getDetNMatrices() (e.g. getMatrixOrder() for det=1) -- but its own
// RatioTolerance setting is ignored in favor of the ratioTolerance
// argument. Exists so callers (in particular, tests exercising several
// different tolerance values) can vary just that one value without
// needing to construct or mutate a MultiTransformFinderControls to do it;
// the single-argument overload above is a thin wrapper around this one.
MatrixGroups selectMatrixGroupsForRatio(
   const MultiTransformFinderControls& controls, double ratio, double ratioTolerance);

#endif // SEARCHMATRIXBUILDER_H
