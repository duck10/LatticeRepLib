#ifndef DIAGNOSE_O2_GAP_H
#define DIAGNOSE_O2_GAP_H

// DiagnoseO2Gap.h
//
// Diagnostic for the O2 library gap (SELLA_handoff_2026-08-18.md, Section 1,
// planned-next-step items 1-2). NOT YET RUN -- draft for review before
// compiling, per usual practice.
//
// APPROACH CHANGE this session: ExpandAcrossBoundaryZeros turned out to be
// a dead end for this purpose. O2's zero slot (S6 position 2) is a fixed
// point of its own boundary-crossing transform across the WHOLE O2
// subspace, not just for one pathological seed -- confirmed independently
// two ways: (1) BuildCharacterExample("rs0 srt") expanded to just itself,
// 0 rejections; (2) real cmdgen O2 output (10 examples) all share the
// identical slot pattern (v0=v4, v1=v3, v2=0, v5 free) regardless of the
// numeric values, meaning the zero-crossing genuinely goes nowhere for
// any generic O2 point. That machinery answers a different question
// (reaching other TYPES across a shared boundary) than the one Section 1
// actually needs (enumerating O2's OWN 24 orientations).
//
// Replaced with MatS6::GetReflections() (already in this codebase, used
// live in CmdS6Refl's main()): the 24 matrices permuting the 4 underlying
// Selling vectors (a,b,c,d). This is the actual symmetry group that
// generates O2's different orientations, not a boundary-crossing walk.
//
// Each of the 24 reflected results is generally NOT already Delone-
// reduced (some component may come out positive), so each is re-reduced
// via Selling::Reduce before being checked against the library -- this
// mirrors the Niggli::Reduce(dummyS6, g6red) pattern already used in
// GenerateNiggliBase::Generate, applied to Selling reduction instead.
//
// Uses the ALREADY-ESTABLISHED test, not a reinvented one:
// DoesProjectorAgreeWithCharacter(character, prj) applies prj to a fixed
// generic "goat" probe vector and checks that the resulting components
// group into the equalities the character string demands (r's all equal
// each other, s's all equal each other, etc). CheckOneProjector is the
// complementary check that validates the '0' slot count (via a different
// probe, "ones", and a norm identity) -- DoesProjectorAgreeWithCharacter
// alone does not validate zero slots, so both are combined below. Both
// confirmed public in class CheckDeloneProjectors
// (GenerateLatticeTypeExamples.h/.cpp).
//
// ASSUMES: Selling::Reduce has an output-parameter signature analogous to
// Niggli::Reduce(const S6&, S6&) -> bool, i.e.
// Selling::Reduce(const S6&, S6&) -> bool. Referenced by name in this
// session's handoff (Section 0, "Selling::Reduce returning differently-
// oriented representatives") but its exact signature wasn't confirmed
// directly this session -- fix the call below if it differs.

#include "GenerateLatticeTypeExamples.h"
#include "LabeledSellaMatrices.h"
#include "MatS6.h"
#include "S6.h"
#include "Selling.h"

#include <cmath>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// Derives a character string (e.g. "rs0 srt") from a concrete, already-
// reduced S6 representative by clustering equal-within-tolerance
// components. Inverse operation of BuildCharacterExample. Space inserted
// after the 3rd component to match the existing character-string
// convention (e.g. "rs0 srt").
inline std::string DeriveCharacterFromRepresentative(const S6& v, const double tol = 1.0E-6) {
   static const std::string letters = "rstuvw";
   std::string sixChars;
   std::vector<double> assignedValues;

   for (size_t i = 0; i < 6; ++i) {
      const double d = v[i];
      if (std::abs(d) < tol) {
         sixChars += '0';
         continue;
      }
      bool matched = false;
      for (size_t j = 0; j < assignedValues.size() && !matched; ++j) {
         if (std::abs(d - assignedValues[j]) < tol) {
            sixChars += letters[j];
            matched = true;
         }
      }
      if (!matched) {
         if (assignedValues.size() >= letters.size()) {
            sixChars += '?';  // more than 6 distinct nonzero values -- should not happen for S6
         }
         else {
            assignedValues.push_back(d);
            sixChars += letters[assignedValues.size() - 1];
         }
      }
   }

   return sixChars.substr(0, 3) + " " + sixChars.substr(3, 3);
}

// Produces one generic, non-pathologically-symmetric O2-type S6 example.
inline S6 GenerateGenericO2Seed() {
   const S6 randomReduced = S6().randDeloneReduced();
   return Delone_O2().GetPrj() * randomReduced;
}

struct O2GapDiagnosisEntry {
   size_t reflectionIndex = 0;
   bool wasAlreadyReduced = false;
   bool reduceSucceeded = false;
   S6 representative;  // post-reduction form actually checked against the library
   std::string derivedCharacter;
   std::vector<size_t> matchingProjectorIndices;  // empty == uncovered
};

struct O2GapDiagnosisReport {
   S6 seed;
   std::vector<O2GapDiagnosisEntry> entries;
};

inline bool IsS6Reduced(const S6& v) {
   for (size_t i = 0; i < 6; ++i) if (v[i] > 0.0) return false;
   return true;
}

inline O2GapDiagnosisReport DiagnoseO2Gap(const double tol = 1.0E-6) {
   const std::vector<MatS6> knownPrjs = LabeledSellaMatrices::CreatePrjs_O2().GetMatrices();
   const std::vector<MatS6> reflections = MatS6::GetReflections();  // 24 matrices

   O2GapDiagnosisReport report;
   report.seed = GenerateGenericO2Seed();

   for (size_t i = 0; i < reflections.size(); ++i) {
      const S6 raw = reflections[i] * report.seed;

      O2GapDiagnosisEntry entry;
      entry.reflectionIndex = i;
      entry.wasAlreadyReduced = IsS6Reduced(raw);

      S6 reduced;
      entry.reduceSucceeded = entry.wasAlreadyReduced ? true : Selling::Reduce(raw, reduced);
      entry.representative = entry.wasAlreadyReduced ? raw : reduced;

      if (!entry.reduceSucceeded) {
         report.entries.push_back(entry);  // no character/coverage check possible
         continue;
      }

      entry.derivedCharacter = DeriveCharacterFromRepresentative(entry.representative, tol);
      for (size_t p = 0; p < knownPrjs.size(); ++p) {
         const bool agreesOnGrouping =
            CheckDeloneProjectors::DoesProjectorAgreeWithCharacter(entry.derivedCharacter, knownPrjs[p]);
         const bool agreesOnZeroCount =
            CheckDeloneProjectors::CheckOneProjector(entry.derivedCharacter, knownPrjs[p]);
         if (agreesOnGrouping && agreesOnZeroCount) {
            entry.matchingProjectorIndices.push_back(p);
         }
      }
      report.entries.push_back(entry);
   }

   return report;
}

inline void PrintO2GapDiagnosis() {
   const O2GapDiagnosisReport report = DiagnoseO2Gap();

   std::cout << "O2 diagnosis via MatS6::GetReflections() (24 orientations)" << std::endl;
   std::cout << "seed: " << report.seed << std::endl;
   std::cout << LabeledSellaMatrices::CreatePrjs_O2().size() << " stored projector(s)" << std::endl;

   size_t uncoveredCount = 0;
   size_t reduceFailedCount = 0;
   std::vector<std::string> uncoveredCharacters;

   for (const O2GapDiagnosisEntry& entry : report.entries) {
      std::cout << "  [" << entry.reflectionIndex << "] "
         << (entry.wasAlreadyReduced ? "(already reduced) " : "(re-reduced) ");

      if (!entry.reduceSucceeded) {
         std::cout << "Selling::Reduce FAILED -- no character/coverage check possible" << std::endl;
         ++reduceFailedCount;
         continue;
      }

      std::cout << entry.representative << " char=\"" << entry.derivedCharacter << "\" -- ";
      if (entry.matchingProjectorIndices.empty()) {
         std::cout << "NO stored projector agrees (candidate missing orientation)";
         ++uncoveredCount;
         bool alreadyListed = false;
         for (const std::string& c : uncoveredCharacters) {
            if (c == entry.derivedCharacter) { alreadyListed = true; break; }
         }
         if (!alreadyListed) uncoveredCharacters.push_back(entry.derivedCharacter);
      }
      else {
         std::cout << "agrees with projector index/indices:";
         for (const size_t idx : entry.matchingProjectorIndices) std::cout << " " << idx;
      }
      std::cout << std::endl;
   }

   std::cout << uncoveredCount << " of " << report.entries.size()
      << " reflection(s) uncovered, " << reduceFailedCount << " Selling::Reduce failure(s), "
      << "spanning " << uncoveredCharacters.size() << " distinct uncovered character pattern(s)." << std::endl;
   for (const std::string& c : uncoveredCharacters) {
      std::cout << "  distinct uncovered character: \"" << c << "\"" << std::endl;
   }
}

#endif  // DIAGNOSE_O2_GAP_H
