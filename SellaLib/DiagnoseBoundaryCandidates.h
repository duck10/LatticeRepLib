#ifndef DIAGNOSE_BOUNDARY_CANDIDATES_H
#define DIAGNOSE_BOUNDARY_CANDIDATES_H

// DiagnoseBoundaryCandidates.h
//
// Follow-up to DiagnoseO2Gap.h, prompted by real CmdSella COMPARE output at
// the O1B/O2/O3 boundary (2026-08-18 session, later half). The character-
// clustering approach in DiagnoseO2Gap.h (fixed tolerance 1.0E-6) is NOT
// trustworthy this close to a boundary: the two representatives below have
// near-zero components (-0.00055/-0.02616, -0.01851/-0.04436) that are
// real but not within 1.0E-6 of zero, so clustering would silently call
// them "no zeros" (O1B-shaped) rather than recognizing they are sitting
// very close to O2/O3 zero slots. Deciding "is this effectively zero" via
// an arbitrary tolerance IS the open question near a boundary, not a
// preprocessing step -- so this diagnostic avoids it entirely.
//
// Instead: for each candidate representative, computes the fixed-point
// residual norm(P*v - v) against EVERY stored projector in O1B's, O2's,
// and O3's libraries (LabeledSellaMatrices::CreatePrjs_O1B/O2/O3()). A
// small residual means v sits close to that orientation's subspace; a
// large one means it doesn't. This is continuous, needs no tolerance
// decision up front, and should be directly comparable to SellaFit's own
// reported fit values (5.19875 for representative A against O1B, 0.06462
// for representative B against O1B, per the COMPARE output) -- if the
// residuals computed here roughly track those numbers, that's a good
// sign this residual IS essentially what SellaFit itself computes, which
// then makes it meaningful to also compute the SAME residual against O2
// and O3's libraries (something the COMPARE output as pasted didn't show
// for these two points).

#include "LabeledSellaMatrices.h"
#include "MatS6.h"
#include "S6.h"

#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

struct LibraryResidual {
   size_t bestIndex = 0;
   double bestResidual = 0.0;
   std::vector<double> allResiduals;  // one per stored projector, in library order
};

inline LibraryResidual ComputeLibraryResidual(const S6& v, const std::vector<MatS6>& library) {
   LibraryResidual result;
   result.bestResidual = -1.0;
   for (size_t i = 0; i < library.size(); ++i) {
      const S6 projected = library[i] * v;
      const double residual = (projected - v).norm();
      result.allResiduals.push_back(residual);
      if (result.bestResidual < 0.0 || residual < result.bestResidual) {
         result.bestResidual = residual;
         result.bestIndex = i;
      }
   }
   return result;
}

inline void PrintBoundaryCandidateDiagnosis(const S6& v, const std::string& label) {
   const std::vector<MatS6> o1b = LabeledSellaMatrices::CreatePrjs_O1B().GetMatrices();
   const std::vector<MatS6> o2 = LabeledSellaMatrices::CreatePrjs_O2().GetMatrices();
   const std::vector<MatS6> o3 = LabeledSellaMatrices::CreatePrjs_O3().GetMatrices();

   const LibraryResidual rO1B = ComputeLibraryResidual(v, o1b);
   const LibraryResidual rO2 = ComputeLibraryResidual(v, o2);
   const LibraryResidual rO3 = ComputeLibraryResidual(v, o3);

   std::cout << std::fixed << std::setprecision(5);
   std::cout << label << ": " << v << std::endl;
   std::cout << "  best O1B residual: " << rO1B.bestResidual
      << "  (projector index " << rO1B.bestIndex << ")" << std::endl;
   std::cout << "  best O2  residual: " << rO2.bestResidual
      << "  (projector index " << rO2.bestIndex << ")" << std::endl;
   std::cout << "  best O3  residual: " << rO3.bestResidual
      << "  (projector index " << rO3.bestIndex << ")" << std::endl;
   std::cout.unsetf(std::ios::floatfield);
}

inline void PrintBoundaryComparison() {
   // The two representatives from CmdSella COMPARE (2026-08-18 session),
   // both arising from the same G6 81.538 81.538 81.538 -76.269 -86.807
   // 0.000 (Delone O3) input cell:
   const S6 candidateA("-43.34318 -38.15350 -0.00055 -38.13490 -43.34264 -0.02616");  // COMPARE-reported O1B fit: 5.19875
   const S6 candidateB("-38.11642 -43.36206 -0.01851 -38.17840 -43.42403 -0.04436");  // COMPARE-reported O1B fit: 0.06462

   std::cout << "Boundary candidate diagnosis -- O1B/O2/O3 residuals" << std::endl;
   std::cout << "(compare best-O1B-residual below to CmdSella's own reported fit values: "
      << "A=5.19875, B=0.06462 -- if these roughly match, this residual IS "
      << "what SellaFit computes, and the O2/O3 numbers alongside it are new information)"
      << std::endl;
   PrintBoundaryCandidateDiagnosis(candidateA, "Candidate A");
   PrintBoundaryCandidateDiagnosis(candidateB, "Candidate B");
}

#endif  // DIAGNOSE_BOUNDARY_CANDIDATES_H
