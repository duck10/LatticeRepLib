#ifndef NIGGLI_TYPES_H
#define NIGGLI_TYPES_H

#include "G6.h"
#include <cfloat>
#include <string>

// ============================================================
// Options controlling algorithm behavior
// ============================================================

struct NiggliOptions {

   enum class Termination {
      NoStepFired,    // classical: stop when no step fires in a full pass
      TracePlateau    // stop when no step fires OR Tr(G) fractional decrease < traceTol
                      // traceTol guards against floating-point oscillation where
                      // steps fire repeatedly but Tr is stuck at the noise floor
   };

   Termination   termination = Termination::NoStepFired;
   double        epsilon = 0.0;              // 0 = exact comparisons
   double        traceTol = 100.0 * DBL_EPSILON; // for TracePlateau only
   int           maxIterations = 100;   // enough for normal cases; 2-cycles hit this fast

   std::string Label() const {
      std::string s;
      s += (termination == Termination::NoStepFired) ? "NoStepFired" : "TracePlateau";
      if (epsilon != 0.0) s += "_GKeps";
      return s;
   }
};

// Named presets -- all use EachCycle normalization, which is required
// for completeness. EntryOnly normalization fails on ~34% of random
// cells and is not a viable algorithm variant.

inline NiggliOptions PresetKrivyGruber1976() {
   NiggliOptions o;
   o.termination = NiggliOptions::Termination::NoStepFired;
   o.epsilon = 0.0;
   return o;
}

inline NiggliOptions PresetTracePlateau() {
   NiggliOptions o;
   o.termination = NiggliOptions::Termination::TracePlateau;
   o.epsilon = 0.0;
   return o;
}

inline NiggliOptions PresetGrosseKunstleve() {
   // GK epsilon: scale-relative fudge applied to all comparisons.
   // Introduced by Grosse-Kunstleve et al. (2004) to prevent infinite
   // loops in floating-point implementations. Mathematically changes
   // the problem; cells near Niggli boundaries receive a perturbed answer.
   NiggliOptions o;
   o.termination = NiggliOptions::Termination::NoStepFired;
   o.epsilon = 1.0e-5;
   return o;
}

// ============================================================
// Result from one reduction run
// ============================================================

struct NiggliResult {
   G6     cell;
   int    iterations = 0;
   int    steps = 0;
   double traceInitial = 0.0;
   double traceFinal = 0.0;
   bool   converged = false;
   bool   isReduced = false;   // post-hoc IsNiggliReduced check
};

#endif // NIGGLI_TYPES_H
