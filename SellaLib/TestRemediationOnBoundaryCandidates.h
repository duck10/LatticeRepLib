#ifndef TEST_REMEDIATION_ON_BOUNDARY_CANDIDATES_H
#define TEST_REMEDIATION_ON_BOUNDARY_CANDIDATES_H

// TestRemediationOnBoundaryCandidates.h
//
// Tests Section 3's prediction from SELLA_handoff_2026-08-19.md directly:
// does one application of GrimmerChains::Remediation (which already tries
// the original orientation plus all 6 single boundary-crossings, per
// GrimmerTree.cpp:676, read this session) close the fit(O1B) gap between
// candidates A (5.19875, poor) and B (0.06462, good) -- two physically
// close lattices (cmddists-confirmed) that landed in different local
// orientations near the O1B/O2/O3 triple point?
//
// ASSUMPTION FLAGGED, not silently guessed: Remediation's first parameter
// is named `bravaisName` in the source, but GenerateDeloneBase::Select is
// the same selector used throughout this codebase for DELONE type names
// (Delone_O1B, Delone_O2, etc.), and the whole session has referred to
// this type as "O1B" (matching CmdSella's own TYPES O2 O3 O1B control and
// COMPARE output). Passing "O1B" below on that basis -- but the parameter
// NAME suggests it may actually expect a Bravais symbol like "oI" instead.
// If GetGeneralType()/GetBravaisType() on the result don't look sane
// (e.g. empty, or clearly not O1B-related), that mismatch is the first
// thing to check, not a sign Remediation itself doesn't work.
//
// Second parameter (deltaFromZeroAllowed / initialFit in the .cpp) is
// UNUSED inside Remediation's body -- confirmed by reading the source
// this session. Value passed below is a placeholder; it should not affect
// the result.

#include "GrimmerTree.h"
#include "S6.h"

#include <iomanip>
#include <iostream>
#include <string>

inline void PrintRemediationResult(const S6& v, const std::string& label, const std::string& typeName) {
   GrimmerChains gc(v);
   const DeloneFitResults result = gc.Remediation(typeName, 1.0E-6);  // second param unused, see note above

   std::cout << std::fixed << std::setprecision(5);
   std::cout << label << " (input): " << v << std::endl;
   std::cout << "  Remediation(\"" << typeName << "\") result:"
      << "  GeneralType=" << result.GetGeneralType()
      << "  BravaisType=" << result.GetBravaisType()
      << "  RawFit=" << result.GetRawFit() << std::endl;
   std::cout.unsetf(std::ios::floatfield);
}

inline void TestRemediationOnBoundaryCandidates() {
   const S6 candidateA("-43.34318 -38.15350 -0.00055 -38.13490 -43.34264 -0.02616");  // raw COMPARE fit(O1B): 5.19875
   const S6 candidateB("-38.11642 -43.36206 -0.01851 -38.17840 -43.42403 -0.04436");  // raw COMPARE fit(O1B): 0.06462

   std::cout << "Testing whether one Remediation pass closes the O1B fit gap "
      << "(Section 3, SELLA_handoff_2026-08-19.md)" << std::endl;
   std::cout << "Prior to Remediation: A=5.19875, B=0.06462 (raw COMPARE fit, no boundary-crossing search)"
      << std::endl;

   PrintRemediationResult(candidateA, "Candidate A", "O1B");
   PrintRemediationResult(candidateB, "Candidate B", "O1B");
}

#endif  // TEST_REMEDIATION_ON_BOUNDARY_CANDIDATES_H