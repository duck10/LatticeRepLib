#ifndef METHOD_LIBRARY_H
#define METHOD_LIBRARY_H

// MethodLibrary.h
//
// Registry of candidate-generation methods for the method-library
// research pipeline (see chat, Sept 2026): a scored-candidate currency
// shared by every method, five methods (6/24/144/168/3480 candidates),
// and two command-string interpreters (SEQUENCE, BESTOF) driven by
// CmdSellaControls' SEQUENCE/BESTOF/NEARTIEEPS keywords.
//
// Method contract: every method takes a vector of ScoredCandidates and a
// target Delone type, and returns a vector of ScoredCandidates that is
// NEVER worse than the input -- input candidates are always folded into
// the comparison pool, so a method with nothing better than what it
// received returns the input back unchanged (chat, Sept 2026: "no
// candidate is ever worse than the input"). The returned vector holds
// every candidate within nearTieEpsilon of the best fit found, not just
// the single best one -- near-ties are kept deliberately (Larry's
// design: "outputting the several best results if they are relatively
// close in value").
//
// 168's transforms are Andrews/Bernstein/Sauter's published VCP
// candidate set (BuildVCPCandidates, DeloneParentageTest.h) -- NOT the
// same as RemediatedFit.h's Remediation168ForOneSubtype, which combines
// VCP candidates with an additional 36-candidate 2-hop boundary-transform
// search per round (168 + 36, despite the name). That combined search is
// a separate, existing thing and is deliberately not reused here, so
// each registry entry below is one clean, isolated candidate set
// matching its published/counted size (chat, Sept 2026).
//
// 3480's transforms are TransformationMatrices::generateUnimodularMatrices(1),
// flattened via MatS6::e3Tos6 -- moved here from the CmdSella.cpp main()
// diagnostic scratch block (SellaFitCanonical investigation, since
// deleted) that originally built this same vector. Confirmed by Larry to
// produce exactly 3480 candidates.

#include "DeloneParentageTest.h"
#include "GenerateLatticeTypeExamples.h"
#include "MatS6.h"
#include "S6.h"
#include "Sella.h"
#include "Selling.h"
#include "TransformationMatrices.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

struct ScoredCandidate {
   S6 cell;
   double fit;
   std::string producedBy;   // method name, for provenance when auditing chains

   // Identity is the cell alone -- two candidates from different methods
   // (or the same method's different transforms) that land on the exact
   // same cell are the same candidate, regardless of fit/producedBy
   // (fit is deterministic given cell and targetType, so it can't
   // actually differ; producedBy is just provenance, not identity).
   bool operator== (const ScoredCandidate& other) const {
      return cell == other.cell;
   }

   friend std::ostream& operator<< (std::ostream& os, const ScoredCandidate& c) {
      os << c.producedBy << "  fit=" << c.fit << "  cell=" << c.cell;
      return os;
   }
};

inline double ComputeFit(const S6& cell, const std::shared_ptr<GenerateDeloneBase>& targetType) {
   return Sella::SellaFitXXXXXX(targetType, cell).GetRawFit();
}

// DedupKey / IsNewCell -- O(1) amortized duplicate check, replacing an
// earlier O(n)-per-insert linear scan (ContainsCell) that caused a real
// multi-hour hang (chat, Sept 2026): a SEQUENCE stage can pass dozens to
// hundreds of near-tie survivors into the next stage (near-degenerate
// points have many valid, genuinely distinct S6 labelings), and 3480's
// transforms applied to, say, 96 survivors generates ~334,000 raw
// candidates in ONE type on ONE cell -- an O(n) scan per insert there is
// O(n^2) overall, roughly 10^11 comparisons. Rounding each S6 component
// to dedupTolerance's decimal scale and hashing the result turns "is this
// a duplicate" into a single hash-set lookup regardless of pool size.
inline std::string DedupKey(const S6& cell, const double dedupTolerance) {
   const double scale = 1.0 / dedupTolerance;
   std::ostringstream os;
   for (size_t i = 0; i < 6; ++i) {
      const long long rounded = static_cast<long long>(std::round(cell[i] * scale));
      os << rounded << ",";
   }
   return os.str();
}

inline bool IsNewCell(
   std::unordered_set<std::string>& seenKeys,
   const S6& cell,
   const double dedupTolerance = 1.0E-6) {
   const std::string key = DedupKey(cell, dedupTolerance);
   if (seenKeys.count(key) > 0) return false;
   seenKeys.insert(key);
   return true;
}

// FilterNearTies -- shared filter body: keeps every candidate within
// nearTieEpsilon of the best fit found in allCandidates. allCandidates
// must include the method's own input (see class comment above), so this
// is also what guarantees the "never worse than input" contract.
inline std::vector<ScoredCandidate> FilterNearTies(
   const std::vector<ScoredCandidate>& allCandidates,
   const double nearTieEpsilon) {

   double bestFit = allCandidates[0].fit;
   for (const auto& c : allCandidates) {
      bestFit = std::min(bestFit, c.fit);
   }

   std::vector<ScoredCandidate> out;
   for (const auto& c : allCandidates) {
      if (c.fit <= bestFit + nearTieEpsilon) {
         out.push_back(c);
      }
   }
   return out;
}

inline std::vector<MatS6> BuildUnimodularTransforms() {
   const std::vector<Matrix_3x3> m3 = TransformationMatrices::generateUnimodularMatrices(1);
   std::vector<MatS6> transforms;
   transforms.reserve(m3.size());
   for (const auto& m : m3) {
      const std::vector<double> flat = {
         m[0], m[1], m[2],
         m[3], m[4], m[5],
         m[6], m[7], m[8]
      };
      transforms.push_back(MatS6::e3Tos6(flat));
   }
   return transforms;
}

// ApplyTransformsAndFilter -- shared body for the three methods that are
// a single flat list of MatS6 transforms applied to every input cell
// (6, 24, 3480). 144 and 168 are NOT single flat transform lists (144 is
// a composition of two sets, 168 is BuildVCPCandidates' own cell-valued
// output), so they have their own bodies below instead of using this one.
inline std::vector<ScoredCandidate> ApplyTransformsAndFilter(
   const std::vector<ScoredCandidate>& input,
   const std::vector<MatS6>& transforms,
   const std::shared_ptr<GenerateDeloneBase>& targetType,
   const double nearTieEpsilon,
   const std::string& methodLabel) {

   std::vector<ScoredCandidate> allCandidates;
   std::unordered_set<std::string> seenKeys;
   for (const auto& in : input) {
      if (IsNewCell(seenKeys, in.cell)) allCandidates.push_back(in);
   }
   for (const auto& in : input) {
      for (const auto& M : transforms) {
         const S6 candidateCell = M * in.cell;
         if (!IsNewCell(seenKeys, candidateCell)) continue;
         allCandidates.push_back(ScoredCandidate{
            candidateCell, ComputeFit(candidateCell, targetType), methodLabel });
      }
   }
   return FilterNearTies(allCandidates, nearTieEpsilon);
}

inline std::vector<ScoredCandidate> ApplyMethod6(
   const std::vector<ScoredCandidate>& input,
   const std::shared_ptr<GenerateDeloneBase>& targetType,
   const double nearTieEpsilon) {
   static const std::vector<MatS6> boundaryTransforms = Selling::GetBoundaryTransforms();
   return ApplyTransformsAndFilter(input, boundaryTransforms, targetType, nearTieEpsilon, "M6");
}

inline std::vector<ScoredCandidate> ApplyMethod24(
   const std::vector<ScoredCandidate>& input,
   const std::shared_ptr<GenerateDeloneBase>& targetType,
   const double nearTieEpsilon) {
   static const std::vector<MatS6> reflections = MatS6::GetReflections();
   return ApplyTransformsAndFilter(input, reflections, targetType, nearTieEpsilon, "M24");
}

inline std::vector<ScoredCandidate> ApplyMethod3480(
   const std::vector<ScoredCandidate>& input,
   const std::shared_ptr<GenerateDeloneBase>& targetType,
   const double nearTieEpsilon) {
   static const std::vector<MatS6> unimodularTransforms = BuildUnimodularTransforms();
   return ApplyTransformsAndFilter(input, unimodularTransforms, targetType, nearTieEpsilon, "M3480");
}

// Method144 composes 6 boundary transforms x 24 reflections
// (reflection * (boundary * p), matching Remediation144's own
// composition order in RemediatedFit.h) rather than a single flat
// transform list, since 144 is a genuine composition of two separate
// transform sets.
inline std::vector<ScoredCandidate> ApplyMethod144(
   const std::vector<ScoredCandidate>& input,
   const std::shared_ptr<GenerateDeloneBase>& targetType,
   const double nearTieEpsilon) {

   static const std::vector<MatS6> boundaryTransforms = Selling::GetBoundaryTransforms();
   static const std::vector<MatS6> reflections = MatS6::GetReflections();

   std::vector<ScoredCandidate> allCandidates;
   std::unordered_set<std::string> seenKeys;
   for (const auto& in : input) {
      if (IsNewCell(seenKeys, in.cell)) allCandidates.push_back(in);
   }
   for (const auto& in : input) {
      for (const auto& boundary : boundaryTransforms) {
         const S6 afterBoundary = boundary * in.cell;
         for (const auto& reflection : reflections) {
            const S6 candidateCell = reflection * afterBoundary;
            if (!IsNewCell(seenKeys, candidateCell)) continue;
            allCandidates.push_back(ScoredCandidate{
               candidateCell, ComputeFit(candidateCell, targetType), "M144" });
         }
      }
   }
   return FilterNearTies(allCandidates, nearTieEpsilon);
}

// Method168 uses BuildVCPCandidates directly -- it already returns
// candidate CELLS built from one seed point (not transform matrices), so
// it doesn't fit ApplyTransformsAndFilter's "transform x cell" shape.
inline std::vector<ScoredCandidate> ApplyMethod168(
   const std::vector<ScoredCandidate>& input,
   const std::shared_ptr<GenerateDeloneBase>& targetType,
   const double nearTieEpsilon) {

   std::vector<ScoredCandidate> allCandidates;
   std::unordered_set<std::string> seenKeys;
   for (const auto& in : input) {
      if (IsNewCell(seenKeys, in.cell)) allCandidates.push_back(in);
   }
   for (const auto& in : input) {
      const std::vector<S6> candidateCells = BuildVCPCandidates(in.cell);
      for (const auto& candidateCell : candidateCells) {
         if (!IsNewCell(seenKeys, candidateCell)) continue;
         allCandidates.push_back(ScoredCandidate{
            candidateCell, ComputeFit(candidateCell, targetType), "M168" });
      }
   }
   return FilterNearTies(allCandidates, nearTieEpsilon);
}

using CandidateMethodFn = std::vector<ScoredCandidate>(*)(
   const std::vector<ScoredCandidate>&,
   const std::shared_ptr<GenerateDeloneBase>&,
   const double);

struct MethodEntry {
   int number;
   std::string name;
   CandidateMethodFn fn;
};

inline const std::vector<MethodEntry>& GetMethodRegistry() {
   static const std::vector<MethodEntry> registry = {
      { 6, "M6", ApplyMethod6 },
      { 24, "M24", ApplyMethod24 },
      { 144, "M144", ApplyMethod144 },
      { 168, "M168", ApplyMethod168 },
      { 3480, "M3480", ApplyMethod3480 }
   };
   return registry;
}

inline bool IsAllDigits(const std::string& token) {
   if (token.empty()) return false;
   for (const char c : token) {
      if (!std::isdigit(static_cast<unsigned char>(c))) return false;
   }
   return true;
}

// FindMethod -- resolves a token (bare number "168" or bare name "M168")
// against the registry. Returns nullptr on no match; callers warn and
// skip rather than throw (Larry's call, chat Sept 2026: "unrecognized
// method name/number: warn and ignore").
inline const MethodEntry* FindMethod(const std::string& token) {
   const auto& registry = GetMethodRegistry();
   if (IsAllDigits(token)) {
      const int n = std::stoi(token);
      for (const auto& entry : registry) {
         if (entry.number == n) return &entry;
      }
      return nullptr;
   }
   for (const auto& entry : registry) {
      if (entry.name == token) return &entry;
   }
   return nullptr;
}

struct MethodStep {
   const MethodEntry* entry;
   int repeatCount;
};

// ParseMethodSpec -- tokenizes a SEQUENCE/BESTOF control string. Grammar:
// whitespace-separated tokens, each either a bare method name/number or
// name(n)/number(n) meaning "apply this method n times, each round's
// output feeding the next round's input" (chat, Sept 2026: "168(2) would
// mean run 168 the second time using the output of the first run as
// input"). Malformed tokens and unrecognized methods are warned about
// and skipped, not treated as fatal.
inline std::vector<MethodStep> ParseMethodSpec(const std::string& spec) {
   std::vector<MethodStep> steps;
   std::istringstream istr(spec);
   std::string token;
   while (istr >> token) {
      std::string namePart = token;
      int repeatCount = 1;

      const size_t openParen = token.find('(');
      if (openParen != std::string::npos) {
         const size_t closeParen = token.find(')', openParen);
         if (closeParen == std::string::npos) {
            std::cerr << ";Warning: malformed repeat count in method token '"
               << token << "', ignoring token" << std::endl;
            continue;
         }
         namePart = token.substr(0, openParen);
         const std::string countPart = token.substr(openParen + 1, closeParen - openParen - 1);
         if (!IsAllDigits(countPart)) {
            std::cerr << ";Warning: malformed repeat count in method token '"
               << token << "', ignoring token" << std::endl;
            continue;
         }
         repeatCount = std::stoi(countPart);
         if (repeatCount <= 0) {
            std::cerr << ";Warning: repeat count must be positive in method token '"
               << token << "', ignoring token" << std::endl;
            continue;
         }
      }

      const MethodEntry* entry = FindMethod(namePart);
      if (entry == nullptr) {
         std::cerr << ";Warning: unrecognized method '" << namePart
            << "', ignoring token" << std::endl;
         continue;
      }
      steps.push_back(MethodStep{ entry, repeatCount });
   }
   return steps;
}

inline std::vector<ScoredCandidate> RunMethodStep(
   const MethodStep& step,
   const std::vector<ScoredCandidate>& input,
   const std::shared_ptr<GenerateDeloneBase>& targetType,
   const double nearTieEpsilon) {

   std::vector<ScoredCandidate> current = input;
   for (int round = 0; round < step.repeatCount; ++round) {
      current = step.entry->fn(current, targetType, nearTieEpsilon);
   }
   return current;
}

// RunSequenceInterpreter -- applies each step left-to-right, each step's
// output feeding the next step's input.
inline std::vector<ScoredCandidate> RunSequenceInterpreter(
   const std::string& spec,
   const std::vector<ScoredCandidate>& seed,
   const std::shared_ptr<GenerateDeloneBase>& targetType,
   const double nearTieEpsilon) {

   const std::vector<MethodStep> steps = ParseMethodSpec(spec);
   std::vector<ScoredCandidate> current = seed;
   for (const auto& step : steps) {
      current = RunMethodStep(step, current, targetType, nearTieEpsilon);
   }
   return current;
}

// RunBestOfInterpreter -- applies each step INDEPENDENTLY to the same
// starting seed (a step's own repeat count, if any, runs as its own
// self-contained arm before joining the comparison), then keeps whichever
// arm(s) landed within nearTieEpsilon of the best fit found across all
// arms.
inline std::vector<ScoredCandidate> RunBestOfInterpreter(
   const std::string& spec,
   const std::vector<ScoredCandidate>& seed,
   const std::shared_ptr<GenerateDeloneBase>& targetType,
   const double nearTieEpsilon) {

   const std::vector<MethodStep> steps = ParseMethodSpec(spec);
   std::vector<ScoredCandidate> allResults;
   std::unordered_set<std::string> seenKeys;
   for (const auto& step : steps) {
      std::vector<ScoredCandidate> armResult = RunMethodStep(step, seed, targetType, nearTieEpsilon);

      // Relabel with the arm's repeat count -- a method's own producedBy
      // string (set inside ApplyMethodN) doesn't know how many rounds it
      // ran, so "168" and "168(2)" would otherwise print identically
      // (chat, Sept 2026) even though they're different arms.
      const std::string armLabel = step.entry->name +
         (step.repeatCount > 1 ? ("(" + std::to_string(step.repeatCount) + ")") : "");

      for (auto& c : armResult) {
         c.producedBy = armLabel;
         // Dedupe across arms, not just within one arm's own candidate
         // pool -- two different arms landing on the exact same cell
         // should count once.
         if (IsNewCell(seenKeys, c.cell)) {
            allResults.push_back(c);
         }
      }
   }
   if (allResults.empty()) {
      return seed;   // every token was unrecognized/malformed
   }
   return FilterNearTies(allResults, nearTieEpsilon);
}

using InterpreterFn = std::vector<ScoredCandidate>(*)(
   const std::string&,
   const std::vector<ScoredCandidate>&,
   const std::shared_ptr<GenerateDeloneBase>&,
   const double);

inline void PrintMethodResults(
   const std::string& interpreterLabel,
   const std::vector<ScoredCandidate>& results) {
   std::cout << "; " << interpreterLabel << " results (" << results.size() << "):" << std::endl;
   for (const auto& c : results) {
      std::cout << ";   " << c << std::endl;
   }
}

// PrintMethodResultsCompact -- one line per type: name, best fit, and
// survivor count (not the full candidate list). Used when TARGETTYPE is
// not set and all 24 types are being looped -- see chat, Sept 2026,
// output-volume concern, same shape as DirectClassify.h's PrintCompact.
// SelectTargetTypes -- TARGETTYPE support for one OR SEVERAL type names
// (e.g. "M4" or "M4 O4"). GenerateDeloneBase::Select (GenerateLatticeType
// Examples.cpp) takes exactly one string and has no multi-name splitting
// of its own, so a multi-type TARGETTYPE is handled entirely here:
// tokenize on whitespace, call Select() once per token, concatenate.
// Unrecognized tokens (Select returns empty) are warned about and
// skipped, matching ParseMethodSpec's "warn and ignore" convention.
inline std::vector<std::shared_ptr<GenerateDeloneBase>> SelectTargetTypes(const std::string& spec) {
   std::vector<std::shared_ptr<GenerateDeloneBase>> out;
   std::istringstream istr(spec);
   std::string token;
   while (istr >> token) {
      const std::vector<std::shared_ptr<GenerateDeloneBase>> matches =
         GenerateDeloneBase().Select(token);
      if (matches.empty()) {
         std::cerr << ";Warning: unrecognized target type '" << token
            << "', ignoring" << std::endl;
         continue;
      }
      for (const auto& m : matches) {
         out.push_back(m);
      }
   }
   return out;
}

inline void PrintMethodResultsCompact(
   const std::string& typeName,
   const std::vector<ScoredCandidate>& results) {
   double bestFit = results.empty() ? 0.0 : results[0].fit;
   for (const auto& c : results) {
      bestFit = std::min(bestFit, c.fit);
   }
   std::cout << ";     " << typeName << "  " << bestFit
      << "  (" << results.size() << " near-tie survivor(s))" << std::endl;
}

// RunAndPrintOneInterpreter -- shared per-cell driver for either
// interpreter: builds one seed per Delone type (sptypes is either all 24
// or the TARGETTYPE-restricted subset -- one or several names, see
// SelectTargetTypes -- chat, Sept 2026, output volume), runs
// interpreterFn once per type, and prints either full per-candidate
// detail or a compact one-line-per-type summary, controlled by
// fullDetail (FULLDETAIL control) independently of how many types were
// selected -- a batch run over several TARGETTYPE-restricted types wants
// compact output too, not just the "all 24" case (chat, Sept 2026).
inline void RunAndPrintOneInterpreter(
   const std::string& interpreterLabel,
   const InterpreterFn interpreterFn,
   const std::string& spec,
   const std::vector<std::shared_ptr<GenerateDeloneBase>>& sptypes,
   const S6& oneLattice,
   const double nearTieEpsilon,
   const bool fullDetail) {

   std::cout << "; " << interpreterLabel << std::endl;
   for (const auto& sptype : sptypes) {
      const std::vector<ScoredCandidate> seed = {
         ScoredCandidate{ oneLattice, ComputeFit(oneLattice, sptype), "input" }
      };
      const std::vector<ScoredCandidate> result =
         interpreterFn(spec, seed, sptype, nearTieEpsilon);

      if (fullDetail) {
         PrintMethodResults(sptype->GetName(), result);
      }
      else {
         PrintMethodResultsCompact(sptype->GetName(), result);
      }
   }
}

#endif  // METHOD_LIBRARY_H