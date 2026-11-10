#ifndef CONVENTIONALCELL_H
#define CONVENTIONALCELL_H

// ConventionalCell.h  (2026-11-08)
//
// Turns a SELLA fit into two conventional cells for its Bravais type:
//   ideal -- the projected target (the nearest lattice of the type), in the
//            conventional setting, with the type's exact symmetry;
//   input -- the input lattice itself in the same setting (unreduced,
//            possibly centered, slightly off the ideal).
//
// Steps:
//   1. ToCanonicalArrangement: from the projected point y (on the winning
//      arrangement, library or face), search the 24 reflections and the
//      boundary transforms R_i for a matrix M taking y to the type's
//      canonical arrangement (its character, e.g. O4 = "00r sst"). R_i is
//      tried only where it keeps the vector reduced, which on y means only
//      where slot i is exactly 0; there R_i acts as the face permutation
//      F_i. The full R_i matrix is kept, so M is a change of superbase that
//      applies to the input lattice as well as to y.
//   2. The conventional centering table (E3 rows: A, B, C in terms of the
//      canonical superbase a, b, c) gives the conventional basis.
//   3. Cell parameters from the Gram matrix of A, B, C.
//
// The centering table was checked on 1000 random members of each Delone
// type (centering letter, right angles, equal edges, beta > 90 for the
// monoclinic types; beta = 90 occurs only for special members). Settings: mC is C-centered with b unique, mP has b
// unique, hR is given in the rhombohedral (primitive) setting. Nine rows
// differ from DeloneTypeList::CreateE3CenteringMatrixList (R1, R3, all mC,
// M4): R3 and M3 were wrong there; the rest used other settings.
//
// The toCanon matrices in LabeledSellaMatrices.cpp are all the identity, so
// they are not used.
//
// Monoclinic types (2026-11-08): the table gives a valid mC or mP cell, but
// not a unique one; beta can come out far from 90 (155 degrees was seen).
// ReduceMonoclinicPlane then reduces a and c in the plane normal to b:
// mP -- Gauss reduction, a <= c; mC -- a may change only by even multiples
// of c, which keeps the C centering, and c by any multiple of a. The result
// is the shortest a allowed by the centering, then the shortest c, with
// beta >= 90 (checked against brute force on 400 random meshes each).
// The matrix is found on the ideal cell and applied to both cells.
//
// I-centered alternative (2026-11-08): when the reduced C-centered beta exceeds
// MONOCLINIC_I_BETA_THRESHOLD, the I-centered cell is also given (MonoclinicCToI):
// a and c are the shortest vectors of the two mesh classes mod 2 that are
// not the centering class, a <= c, beta >= 90 (checked against brute force
// on 1000 random meshes).
//
// Basis matrix from the typed cell (2026-11-09): AddTypedBasis gives the 3x3
// whose rows are the conventional A, B, C in terms of the cell as typed
// (a, b, c of the input line, centered or not). The S6 chain is
//   typed S6 --mat--> reduced S6 --candidate--> winning candidate --toCanon-->
// canonical arrangement, where mat comes from LatticeConverter::
// SellingReduceCell (m66 * MatS6(mLattice)). The 3x3 of that MatS6 product is
// recovered exactly by BasisMatrixFromMatS6 (no search), then
// T * E3 is applied (and TI for the I2/m cell). Entries are rational for
// centered input (1/2, 1/3, ...). Two checks are returned: the e3Tos6 round
// trip of the recovered matrix, and the typed Gram matrix carried by the
// final 3x3 against the printed input cell.

#include "DeloneFitResults.h"
#include "DeloneParentageTest.h"
#include "FaceCrossedArrangements.h"
#include "GenerateLatticeTypeExamples.h"
#include "LRL_Cell.h"
#include "MatS6.h"
#include "S6.h"
#include "Selling.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

struct Matrix3 {
   double m[3][3];
};

struct ConventionalCenteringEntry {
   std::string e3;    // rows: A, B, C in terms of a, b, c
   char centering;    // P, I, F, C
};

inline std::map<std::string, ConventionalCenteringEntry> BuildConventionalCenteringTable() {
   std::map<std::string, ConventionalCenteringEntry> t;
   t["C1"] = { "0 1 1  1 0 1  1 1 0", 'I' };
   t["C3"] = { "1 1 0  -1 1 0  1 1 2", 'F' };
   t["C5"] = { "1 0 0  0 1 0  0 0 1", 'P' };
   t["R1"] = { "1 0 0  0 1 0  0 0 1", 'P' };   // rhombohedral setting
   t["R3"] = { "1 0 0  1 1 1  1 0 1", 'P' };   // rhombohedral setting
   t["T1"] = { "0 1 1  1 0 1  1 1 0", 'I' };
   t["T2"] = { "1 0 0  0 1 0  1 1 2", 'I' };
   t["T5"] = { "1 0 0  0 1 0  0 0 1", 'P' };
   t["O1A"] = { "1 1 0  -1 1 0  1 1 2", 'F' };
   t["O1B"] = { "0 1 1  1 0 1  1 1 0", 'I' };
   t["O2"] = { "1 0 0  0 1 0  1 1 2", 'I' };
   t["O3"] = { "0 1 1  1 0 1  1 1 0", 'I' };
   t["O4"] = { "1 -1 0  1 1 0  0 0 1", 'C' };
   t["O5"] = { "1 0 0  0 1 0  0 0 1", 'P' };
   t["M1A"] = { "1 1 0  -1 1 0  0 0 1", 'C' };
   t["M1B"] = { "1 -1 0  1 1 0  0 1 1", 'C' };
   t["M2A"] = { "1 0 2  1 0 0  0 1 0", 'C' };
   t["M2B"] = { "1 -1 0  1 1 0  0 1 1", 'C' };
   t["M3"] = { "0 1 -1  0 1 1  1 0 1", 'C' };
   t["M4"] = { "0 1 0  0 0 1  1 0 0", 'P' };
   t["A1"] = { "1 0 0  0 1 0  0 0 1", 'P' };
   t["A2"] = { "1 0 0  0 1 0  0 0 1", 'P' };
   t["A3"] = { "1 0 0  0 1 0  0 0 1", 'P' };
   t["H4"] = { "1 0 0  0 1 0  0 0 1", 'P' };
   return t;
}

inline const std::map<std::string, ConventionalCenteringEntry>& ConventionalCenteringTable() {
   static const std::map<std::string, ConventionalCenteringEntry> table = BuildConventionalCenteringTable();
   return table;
}

inline Matrix3 ParseMatrix3(const std::string& s) {
   Matrix3 out = {};
   std::istringstream in(s);
   for (size_t i = 0; i < 3; ++i) {
      for (size_t j = 0; j < 3; ++j) {
         in >> out.m[i][j];
      }
   }
   return out;
}

// --- step 1: to the canonical arrangement --------------------------------

// True when v has the pattern of the character: 0 where it has '0', one
// value across each letter. Spaces in the character are ignored.
inline bool LiesOnCharacter(const S6& v, const std::string& character, const double tol) {
   std::string pattern;
   for (const char c : character) {
      if (c != ' ') pattern.push_back(c);
   }
   if (pattern.size() != 6) return false;
   for (size_t i = 0; i < 6; ++i) {
      if (pattern[i] == '0' && std::fabs(v[i]) > tol) return false;
   }
   for (size_t i = 0; i < 6; ++i) {
      if (pattern[i] == '0') continue;
      for (size_t j = i + 1; j < 6; ++j) {
         if (pattern[j] == pattern[i] && std::fabs(v[i] - v[j]) > tol) return false;
      }
   }
   return true;
}

inline double LargestScalar(const S6& v) {
   double m = v[0];
   for (size_t i = 1; i < 6; ++i) {
      if (v[i] > m) m = v[i];
   }
   return m;
}

struct CanonicalSearchNode {
   S6 v;
   MatS6 m;   // v = m * (starting vector)
};

inline bool AlreadySeen(const std::vector<S6>& seen, const S6& v, const double tol) {
   for (const auto& s : seen) {
      if ((s - v).norm() <= tol) return true;
   }
   return false;
}

// Applies each matrix to the node; keeps results that are still reduced and
// not seen before.
inline void ExpandCanonicalNode(const CanonicalSearchNode& node, const std::vector<MatS6>& mats,
   const double tol, std::vector<S6>& seen, std::vector<CanonicalSearchNode>& next) {
   for (const auto& T : mats) {
      const S6 w = T * node.v;
      if (LargestScalar(w) > tol) continue;
      if (AlreadySeen(seen, w, tol)) continue;
      seen.push_back(w);
      next.push_back(CanonicalSearchNode{ w, T * node.m });
   }
}

// Finds M with M * y on the canonical arrangement of the character.
// Breadth first over reflections and boundary transforms; a prototype
// reached the canonical arrangement from every library and face
// arrangement of all 24 types within two steps.
inline bool ToCanonicalArrangement(const S6& y, const std::string& character, MatS6& toCanon,
   const size_t maxDepth = 4) {
   static const std::vector<MatS6> reflections = MatS6::GetReflections();
   static const std::vector<MatS6> boundaryTransforms = Selling::GetBoundaryTransforms();
   const double tol = 1.0E-10 * std::max(1.0, y.norm());

   std::vector<CanonicalSearchNode> frontier;
   frontier.push_back(CanonicalSearchNode{ y, MatS6().unit() });
   std::vector<S6> seen;
   seen.push_back(y);

   for (size_t depth = 0; depth <= maxDepth; ++depth) {
      for (const auto& node : frontier) {
         if (LiesOnCharacter(node.v, character, tol)) {
            toCanon = node.m;
            return true;
         }
      }
      std::vector<CanonicalSearchNode> next;
      for (const auto& node : frontier) {
         ExpandCanonicalNode(node, reflections, tol, seen, next);
         ExpandCanonicalNode(node, boundaryTransforms, tol, seen, next);
      }
      if (next.empty()) break;
      frontier = next;
   }
   return false;
}

// --- steps 2 and 3: conventional basis and cell parameters ----------------

inline Matrix3 IdentityMatrix3() {
   Matrix3 t = {};
   t.m[0][0] = 1.0;
   t.m[1][1] = 1.0;
   t.m[2][2] = 1.0;
   return t;
}

// A * B
inline Matrix3 MultiplyMatrix3(const Matrix3& A, const Matrix3& B) {
   Matrix3 out = {};
   for (size_t i = 0; i < 3; ++i)
      for (size_t j = 0; j < 3; ++j)
         for (size_t k = 0; k < 3; ++k)
            out.m[i][j] += A.m[i][k] * B.m[k][j];
   return out;
}

// row[to] -= n * row[from]
inline void SubtractRowMultiple(Matrix3& T, const size_t to, const size_t from, const double n) {
   for (size_t j = 0; j < 3; ++j) T.m[to][j] -= n * T.m[from][j];
}

// Gram matrix of a, b, c from the S6 scalars
// (s1 = b.c, s2 = a.c, s3 = a.b, s4 = a.d, s5 = b.d, s6 = c.d).
inline Matrix3 GramFromS6(const S6& s) {
   Matrix3 g = {};
   g.m[0][0] = -(s[1] + s[2] + s[3]);
   g.m[1][1] = -(s[2] + s[0] + s[4]);
   g.m[2][2] = -(s[1] + s[0] + s[5]);
   g.m[0][1] = g.m[1][0] = s[2];
   g.m[0][2] = g.m[2][0] = s[1];
   g.m[1][2] = g.m[2][1] = s[0];
   return g;
}

// M * G * M^T
inline Matrix3 TransformGram(const Matrix3& M, const Matrix3& G) {
   Matrix3 mg = {};
   for (size_t i = 0; i < 3; ++i)
      for (size_t j = 0; j < 3; ++j)
         for (size_t k = 0; k < 3; ++k)
            mg.m[i][j] += M.m[i][k] * G.m[k][j];
   Matrix3 out = {};
   for (size_t i = 0; i < 3; ++i)
      for (size_t j = 0; j < 3; ++j)
         for (size_t k = 0; k < 3; ++k)
            out.m[i][j] += mg.m[i][k] * M.m[j][k];
   return out;
}

// Reduction of a and c in the plane normal to the unique axis b, for a
// monoclinic conventional Gram matrix G (rows a, b, c). step = 1 for mP,
// step = 2 for mC (a changes only by even multiples of c, keeping C).
// Returns T with rows the new a, b, c in terms of the old ones; det T = +1.
inline Matrix3 ReduceMonoclinicPlane(const Matrix3& G, const int step) {
   Matrix3 T = IdentityMatrix3();
   const double slack = 1.0 + 1.0E-12;
   const double s = static_cast<double>(step);
   for (int pass = 0; pass < 200; ++pass) {
      bool changed = false;
      Matrix3 g = TransformGram(T, G);
      if (std::fabs(g.m[0][2]) > 0.5 * g.m[0][0] * slack) {
         SubtractRowMultiple(T, 2, 0, std::round(g.m[0][2] / g.m[0][0]));
         changed = true;
         g = TransformGram(T, G);
      }
      if (std::fabs(g.m[0][2]) > 0.5 * s * g.m[2][2] * slack) {
         SubtractRowMultiple(T, 0, 2, s * std::round(g.m[0][2] / (s * g.m[2][2])));
         changed = true;
      }
      if (!changed) break;
   }
   Matrix3 g = TransformGram(T, G);
   if (step == 1 && g.m[0][0] > g.m[2][2]) {
      // (a, b, c) -> (c, -b, a)
      const Matrix3 swapAC = { { { 0.0, 0.0, 1.0 }, { 0.0, -1.0, 0.0 }, { 1.0, 0.0, 0.0 } } };
      T = MultiplyMatrix3(swapAC, T);
      g = TransformGram(T, G);
   }
   if (g.m[0][2] > 0.0) {
      // (a, b, c) -> (-a, -b, c): beta obtuse
      const Matrix3 flip = { { { -1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, { 0.0, 0.0, 1.0 } } };
      T = MultiplyMatrix3(flip, T);
   }
   return T;
}

// The final change of conventional basis for a type, found on the ideal
// (exactly symmetric) conventional Gram matrix: the monoclinic plane
// reduction for M1A ... M4, the identity for every other type. Used by both
// ConventionalCellsForFit and the library check, so they cannot drift apart.
inline Matrix3 ConventionalSettingMatrix(const std::string& deloneName, const char centering,
   const Matrix3& idealGram) {
   if (deloneName.empty() || deloneName[0] != 'M') return IdentityMatrix3();
   const int step = (centering == 'C') ? 2 : 1;
   return ReduceMonoclinicPlane(idealGram, step);
}

// --- I-centered alternative for mC ---------------------------------------------

// Above this C-centered beta (degrees) the I-centered cell is also reported.
const double MONOCLINIC_I_BETA_THRESHOLD = 120.0;

// Row i of T as a vector in the current basis.
inline void MatrixRow(const Matrix3& T, const size_t i, double v[3]) {
   for (size_t j = 0; j < 3; ++j) v[j] = T.m[i][j];
}

// v . G . w
inline double GramDot(const Matrix3& G, const double v[3], const double w[3]) {
   double sum = 0.0;
   for (size_t i = 0; i < 3; ++i)
      for (size_t j = 0; j < 3; ++j)
         sum += v[i] * G.m[i][j] * w[j];
   return sum;
}

inline double Determinant3(const Matrix3& T) {
   return T.m[0][0] * (T.m[1][1] * T.m[2][2] - T.m[1][2] * T.m[2][1])
      - T.m[0][1] * (T.m[1][0] * T.m[2][2] - T.m[1][2] * T.m[2][0])
      + T.m[0][2] * (T.m[1][0] * T.m[2][1] - T.m[1][1] * T.m[2][0]);
}

// True if v (components on a, b, c of a C cell, b component 0) is in the
// centering class of the a-c mesh: a odd, c even.
inline bool InCenteringClass(const double v[3]) {
   const long long x = static_cast<long long>(std::llround(v[0]));
   const long long z = static_cast<long long>(std::llround(v[2]));
   return (x % 2 != 0) && (z % 2 == 0);
}

// From a C-centered Gram matrix Gc (rows a, b, c; b unique) to a reduced I-centered
// cell. Gauss-reduce the mesh to u, v; the three mod-2 classes are
// represented at their shortest by u, v and the shorter of u + v, u - v.
// The one in the centering class (a odd, c even) is dropped; the other two
// are the I cell's a and c (any two of them form a basis of the mesh).
// Then a <= c, beta >= 90, det = +1. Returns T, rows the I-cell a, b, c in
// terms of the C-cell a, b, c.
inline Matrix3 MonoclinicCToI(const Matrix3& Gc) {
   const Matrix3 T1 = ReduceMonoclinicPlane(Gc, 1);
   double u[3];
   double v[3];
   MatrixRow(T1, 0, u);
   MatrixRow(T1, 2, v);
   double sum[3];
   double diff[3];
   for (size_t j = 0; j < 3; ++j) {
      sum[j] = u[j] + v[j];
      diff[j] = u[j] - v[j];
   }
   double w[3];
   const bool sumShorter = GramDot(Gc, sum, sum) <= GramDot(Gc, diff, diff);
   for (size_t j = 0; j < 3; ++j) w[j] = sumShorter ? sum[j] : diff[j];

   const double* reps[3] = { u, v, w };
   const double* kept[2] = { nullptr, nullptr };
   size_t nKept = 0;
   for (size_t k = 0; k < 3; ++k) {
      if (!InCenteringClass(reps[k]) && nKept < 2) kept[nKept++] = reps[k];
   }
   if (nKept != 2) return IdentityMatrix3();   // not a C cell; leave unchanged

   const bool firstShorter = GramDot(Gc, kept[0], kept[0]) <= GramDot(Gc, kept[1], kept[1]);
   const double* aI = firstShorter ? kept[0] : kept[1];
   const double* cI = firstShorter ? kept[1] : kept[0];
   Matrix3 T = {};
   for (size_t j = 0; j < 3; ++j) {
      T.m[0][j] = aI[j];
      T.m[2][j] = cI[j];
   }
   T.m[1][1] = 1.0;
   double a[3];
   double c[3];
   MatrixRow(T, 0, a);
   MatrixRow(T, 2, c);
   if (GramDot(Gc, a, c) > 0.0) {
      for (size_t j = 0; j < 3; ++j) T.m[2][j] = -T.m[2][j];   // beta obtuse
   }
   if (Determinant3(T) < 0.0) T.m[1][1] = -1.0;
   return T;
}

struct ConventionalCellParameters {
   char centering = 'P';
   double a = 0.0;
   double b = 0.0;
   double c = 0.0;
   double alpha = 0.0;   // degrees
   double beta = 0.0;
   double gamma = 0.0;
};

inline double AngleDegrees(const double dot, const double len1, const double len2) {
   static const double degreesPerRadian = 45.0 / std::atan(1.0);
   double cosine = dot / (len1 * len2);
   if (cosine > 1.0) cosine = 1.0;
   if (cosine < -1.0) cosine = -1.0;
   return degreesPerRadian * std::acos(cosine);
}

inline ConventionalCellParameters CellFromGram(const Matrix3& G, const char centering) {
   ConventionalCellParameters p;
   p.centering = centering;
   p.a = std::sqrt(G.m[0][0]);
   p.b = std::sqrt(G.m[1][1]);
   p.c = std::sqrt(G.m[2][2]);
   p.alpha = AngleDegrees(G.m[1][2], p.b, p.c);
   p.beta = AngleDegrees(G.m[0][2], p.a, p.c);
   p.gamma = AngleDegrees(G.m[0][1], p.a, p.b);
   return p;
}

inline std::string FormatConventionalCell(const ConventionalCellParameters& p, const int precision = 4) {
   std::ostringstream os;
   os << p.centering << std::fixed << std::setprecision(precision)
      << " " << p.a << " " << p.b << " " << p.c
      << " " << p.alpha << " " << p.beta << " " << p.gamma;
   return os.str();
}

struct ConventionalCellResult {
   bool ok = false;
   ConventionalCellParameters ideal;   // the projected target
   ConventionalCellParameters input;   // the input lattice in the same setting
   MatS6 toCanon;
   bool hasAlternateI = false;          // mC with C-centered beta above the threshold
   ConventionalCellParameters idealI;   // I-centered versions of ideal and input
   ConventionalCellParameters inputI;

   // kept for AddTypedBasis
   Matrix3 settingFromCanonical = {};   // T * E3
   Matrix3 settingIFromSetting = {};    // TI (when hasAlternateI)
   Matrix3 inputSettingGram = {};       // Gram matrix of the printed input cell
   Matrix3 inputSettingGramI = {};

   // filled by AddTypedBasis
   bool hasTypedBasis = false;
   Matrix3 typedToConventional = {};    // rows A, B, C in terms of typed a, b, c
   Matrix3 typedToConventionalI = {};   // the same for the I2/m cell
   double typedBasisRoundTripError = 0.0;   // max |e3Tos6(M) - MatS6|
   double typedBasisGramError = 0.0;        // relative, typed Gram vs input cell
};

// Both conventional cells for one fit. Uses the fit's projected target
// (GetBestFit) and winning candidate (GetWinningCandidate), which are in
// the same basis. character is the Delone type's character, e.g. "00r sst".
inline ConventionalCellResult ConventionalCellsForFit(const DeloneFitResults& fit,
   const std::string& deloneName, const std::string& character) {
   ConventionalCellResult r;
   const auto& table = ConventionalCenteringTable();
   const auto it = table.find(deloneName);
   if (it == table.end()) return r;

   MatS6 toCanon;
   if (!ToCanonicalArrangement(fit.GetBestFit(), character, toCanon)) return r;

   const Matrix3 E3 = ParseMatrix3(it->second.e3);
   const S6 idealCanonical = toCanon * fit.GetBestFit();
   const S6 inputCanonical = toCanon * fit.GetWinningCandidate();
   const Matrix3 idealGram = TransformGram(E3, GramFromS6(idealCanonical));
   const Matrix3 inputGram = TransformGram(E3, GramFromS6(inputCanonical));
   const Matrix3 T = ConventionalSettingMatrix(deloneName, it->second.centering, idealGram);
   const Matrix3 idealSetting = TransformGram(T, idealGram);
   const Matrix3 inputSetting = TransformGram(T, inputGram);
   r.ideal = CellFromGram(idealSetting, it->second.centering);
   r.input = CellFromGram(inputSetting, it->second.centering);
   r.toCanon = toCanon;
   r.settingFromCanonical = MultiplyMatrix3(T, E3);
   r.inputSettingGram = inputSetting;
   if (!deloneName.empty() && deloneName[0] == 'M' && it->second.centering == 'C'
      && r.ideal.beta > MONOCLINIC_I_BETA_THRESHOLD) {
      const Matrix3 TI = MonoclinicCToI(idealSetting);
      r.idealI = CellFromGram(TransformGram(TI, idealSetting), 'I');
      r.inputSettingGramI = TransformGram(TI, inputSetting);
      r.inputI = CellFromGram(r.inputSettingGramI, 'I');
      r.settingIFromSetting = TI;
      r.hasAlternateI = true;
   }
   r.ok = true;
   return r;
}

// The character of a Delone type by name ("O4" -> "00r sst"); empty if the
// name is unknown.
inline std::string DeloneCharacterForName(const std::string& name) {
   static const std::vector<std::shared_ptr<GenerateDeloneBase>> types = GenerateDeloneBase().Select("");
   for (const auto& t : types) {
      if (t->GetName() == name) return t->GetCharacter();
   }
   return std::string();
}

// Both conventional cells for one fit, by the fit's own Delone type name.
inline ConventionalCellResult ConventionalCellsForFit(const DeloneFitResults& fit) {
   const std::string name = fit.GetGeneralType();
   return ConventionalCellsForFit(fit, name, DeloneCharacterForName(name));
}

// --- basis matrix from the typed cell -------------------------------------

// out = mat * u (S6 vectors as plain arrays; MatS6 is row-major).
inline void ApplyMatS6(const MatS6& mat, const double u[6], double out[6]) {
   for (size_t r = 0; r < 6; ++r) {
      out[r] = 0.0;
      for (size_t k = 0; k < 6; ++k) out[r] += mat[6 * r + k] * u[k];
   }
}

// Gram matrix of a, b, c from six S6 scalars (same formulas as GramFromS6).
inline Matrix3 GramFromSixScalars(const double s[6]) {
   Matrix3 g = {};
   g.m[0][0] = -(s[1] + s[2] + s[3]);
   g.m[1][1] = -(s[2] + s[0] + s[4]);
   g.m[2][2] = -(s[1] + s[0] + s[5]);
   g.m[0][1] = g.m[1][0] = s[2];
   g.m[0][2] = g.m[2][0] = s[1];
   g.m[1][2] = g.m[2][1] = s[0];
   return g;
}

// Gram image under mat of the Gram matrix whose S6 is u.
inline Matrix3 GramImage(const MatS6& mat, const double u[6]) {
   double s[6];
   ApplyMatS6(mat, u, s);
   return GramFromSixScalars(s);
}

// Column of M, up to sign, from P = m m^T.
inline void ColumnFromOuterProduct(const Matrix3& p, double col[3]) {
   size_t iMax = 0;
   for (size_t i = 1; i < 3; ++i) {
      if (p.m[i][i] > p.m[iMax][iMax]) iMax = i;
   }
   const double root = std::sqrt(std::max(0.0, p.m[iMax][iMax]));
   for (size_t i = 0; i < 3; ++i) col[i] = (root == 0.0) ? 0.0 : p.m[i][iMax] / root;
}

// Frobenius inner product of Q with u v^T + v u^T.
inline double SymmetricOverlap(const Matrix3& q, const double u[3], const double v[3]) {
   double sum = 0.0;
   for (size_t i = 0; i < 3; ++i)
      for (size_t j = 0; j < 3; ++j)
         sum += q.m[i][j] * (u[i] * v[j] + v[i] * u[j]);
   return sum;
}

// The 3x3 M (rows: new basis in terms of old, as in MatS6::e3Tos6) whose
// induced S6 map is mat. The image of E_jj is m_j m_j^T (m_j = column j),
// giving each column up to sign; the images of E_01 + E_10 and E_02 + E_20
// fix the signs of columns 1 and 2 relative to column 0; det M > 0 removes
// the overall sign (M and -M induce the same map). roundTripError receives
// max |e3Tos6(M) - mat|.
inline Matrix3 BasisMatrixFromMatS6(const MatS6& mat, double& roundTripError) {
   // S6 of E_00, E_11, E_22, E_01 + E_10, E_02 + E_20
   static const double unitDiagonal[3][6] = {
      { 0.0, 0.0, 0.0, -1.0, 0.0, 0.0 },
      { 0.0, 0.0, 0.0, 0.0, -1.0, 0.0 },
      { 0.0, 0.0, 0.0, 0.0, 0.0, -1.0 } };
   static const double unitAB[6] = { 0.0, 0.0, 1.0, -1.0, -1.0, 0.0 };
   static const double unitAC[6] = { 0.0, 1.0, 0.0, -1.0, 0.0, -1.0 };

   double cols[3][3];
   for (size_t j = 0; j < 3; ++j) {
      ColumnFromOuterProduct(GramImage(mat, unitDiagonal[j]), cols[j]);
   }
   if (SymmetricOverlap(GramImage(mat, unitAB), cols[0], cols[1]) < 0.0) {
      for (size_t i = 0; i < 3; ++i) cols[1][i] = -cols[1][i];
   }
   if (SymmetricOverlap(GramImage(mat, unitAC), cols[0], cols[2]) < 0.0) {
      for (size_t i = 0; i < 3; ++i) cols[2][i] = -cols[2][i];
   }
   Matrix3 M = {};
   for (size_t i = 0; i < 3; ++i)
      for (size_t j = 0; j < 3; ++j)
         M.m[i][j] = cols[j][i];
   if (Determinant3(M) < 0.0) {
      for (size_t i = 0; i < 3; ++i)
         for (size_t j = 0; j < 3; ++j)
            M.m[i][j] = -M.m[i][j];
   }

   std::vector<double> rowMajor(9);
   for (size_t i = 0; i < 3; ++i)
      for (size_t j = 0; j < 3; ++j)
         rowMajor[3 * i + j] = M.m[i][j];
   const MatS6 back = MatS6::e3Tos6(rowMajor);
   roundTripError = 0.0;
   for (size_t k = 0; k < 36; ++k) {
      roundTripError = std::max(roundTripError, std::fabs(back[k] - mat[k]));
   }
   return M;
}

// max |G1 - G2| / max(1, largest diagonal of G2)
inline double RelativeGramDifference(const Matrix3& g1, const Matrix3& g2) {
   double scale = 1.0;
   for (size_t i = 0; i < 3; ++i) scale = std::max(scale, std::fabs(g2.m[i][i]));
   double diff = 0.0;
   for (size_t i = 0; i < 3; ++i)
      for (size_t j = 0; j < 3; ++j)
         diff = std::max(diff, std::fabs(g1.m[i][j] - g2.m[i][j]));
   return diff / scale;
}

// Fills the typed-cell basis matrices of r. typedToReduced is the mat
// returned by LatticeConverter::SellingReduceCell for the typed cell;
// typedS6 is S6(G6(typed cell)), the typed cell itself (not made primitive).
inline void AddTypedBasis(ConventionalCellResult& r, const DeloneFitResults& fit,
   const MatS6& typedToReduced, const S6& typedS6) {
   if (!r.ok) return;
   const MatS6 typedToCanonical = r.toCanon * fit.GetCandidateMatrix() * typedToReduced;
   const Matrix3 toCanonical = BasisMatrixFromMatS6(typedToCanonical, r.typedBasisRoundTripError);
   r.typedToConventional = MultiplyMatrix3(r.settingFromCanonical, toCanonical);

   const Matrix3 typedGram = GramFromS6(typedS6);
   r.typedBasisGramError = RelativeGramDifference(
      TransformGram(r.typedToConventional, typedGram), r.inputSettingGram);
   if (r.hasAlternateI) {
      r.typedToConventionalI = MultiplyMatrix3(r.settingIFromSetting, r.typedToConventional);
      r.typedBasisGramError = std::max(r.typedBasisGramError, RelativeGramDifference(
         TransformGram(r.typedToConventionalI, typedGram), r.inputSettingGramI));
   }
   r.hasTypedBasis = true;
}

// One entry as an integer or a fraction with a small denominator, else
// a decimal.
inline std::string FormatBasisEntry(const double x) {
   static const int denominators[] = { 1, 2, 3, 4, 6, 8, 12 };
   for (const int d : denominators) {
      const double scaled = x * static_cast<double>(d);
      const double n = std::round(scaled);
      if (std::fabs(scaled - n) < 1.0E-6) {
         const long long ni = static_cast<long long>(n);
         std::ostringstream os;
         os << ni;
         if (ni != 0 && d != 1) os << "/" << d;
         return os.str();
      }
   }
   std::ostringstream os;
   os << std::fixed << std::setprecision(6) << x;
   return os.str();
}

// Rows A, B, C, in the layout of the centering table ("1 0 0  0 1 0  0 0 1").
inline std::string FormatBasisMatrix(const Matrix3& M) {
   std::ostringstream os;
   for (size_t i = 0; i < 3; ++i) {
      if (i > 0) os << "  ";
      for (size_t j = 0; j < 3; ++j) {
         if (j > 0) os << " ";
         os << FormatBasisEntry(M.m[i][j]);
      }
   }
   return os.str();
}

// --- self-check -----------------------------------------------------------

inline char ExpectedConventionalCentering(const std::string& bravais) {
   if (bravais == "hR") return 'P';   // rhombohedral setting
   if (bravais.size() == 2 && (bravais[1] == 'I' || bravais[1] == 'F' || bravais[1] == 'C')) return bravais[1];
   return 'P';
}

inline bool NearlyEqual(const double x, const double y, const double tol) {
   return std::fabs(x - y) <= tol;
}

// The symmetry the conventional cell of an exact member must have.
inline bool ConventionalSymmetryHolds(const std::string& bravais, const ConventionalCellParameters& p) {
   const double angTol = 1.0E-6;
   const double lenTol = 1.0E-9 * std::max(1.0, std::max(p.a, std::max(p.b, p.c)));
   const bool right3 = NearlyEqual(p.alpha, 90.0, angTol) && NearlyEqual(p.beta, 90.0, angTol)
      && NearlyEqual(p.gamma, 90.0, angTol);
   const char family = bravais.empty() ? 'a' : bravais[0];
   if (family == 'c') return right3 && NearlyEqual(p.a, p.b, lenTol) && NearlyEqual(p.b, p.c, lenTol);
   if (family == 't') return right3 && NearlyEqual(p.a, p.b, lenTol);
   if (family == 'o') return right3;
   if (bravais == "hP") {
      return NearlyEqual(p.a, p.b, lenTol) && NearlyEqual(p.alpha, 90.0, angTol)
         && NearlyEqual(p.beta, 90.0, angTol) && NearlyEqual(p.gamma, 120.0, angTol);
   }
   if (bravais == "hR") {
      return NearlyEqual(p.a, p.b, lenTol) && NearlyEqual(p.b, p.c, lenTol)
         && NearlyEqual(p.alpha, p.beta, angTol) && NearlyEqual(p.beta, p.gamma, angTol);
   }
   if (family == 'm') {
      // also reduced in the a-c plane: |a.c| <= a.a / 2 and <= step * c.c / 2
      // (C: step 2, P: step 1, a <= c); I-centered: a <= c and |a.c| <= a.a
      const double step = (p.centering == 'C') ? 2.0 : 1.0;
      const double ac = std::fabs(p.a * p.c * std::cos(std::atan(1.0) * p.beta / 45.0));
      const double slack = 1.0 + 1.0E-9;
      const bool reduced = (p.centering == 'I')
         ? ac <= p.a * p.a * slack
         : ac <= 0.5 * p.a * p.a * slack && ac <= 0.5 * step * p.c * p.c * slack;
      const bool aFirst = (p.centering == 'C') || p.a <= p.c + lenTol;
      return NearlyEqual(p.alpha, 90.0, angTol) && NearlyEqual(p.gamma, 90.0, angTol)
         && p.beta > 90.0 - angTol && reduced && aFirst;
   }
   return true;   // aP
}

struct ConventionalCellCheckCounts {
   size_t arrangements = 0;
   size_t samples = 0;
   size_t skippedInvalid = 0;
   size_t failures = 0;
};

// For every arrangement (library and face) of every Delone type, draws
// samplesPerArrangement generic members on that arrangement and checks that
// each reaches the canonical arrangement and that its conventional cell has
// the type's symmetry and centering. Prints up to maxPrinted failures.
inline ConventionalCellCheckCounts CheckConventionalCellsAllArrangements(std::ostream& os,
   const size_t samplesPerArrangement = 3, const size_t maxPrinted = 10) {
   ConventionalCellCheckCounts counts;
   const std::vector<std::shared_ptr<GenerateDeloneBase>> types = GenerateDeloneBase().Select("");
   const auto& table = ConventionalCenteringTable();

   for (const auto& type : types) {
      const std::string name = type->GetName();
      const std::string bravais = type->GetBravaisType();
      const auto it = table.find(name);
      if (it == table.end()) {
         ++counts.failures;
         os << ";   " << name << ": no conventional centering entry" << std::endl;
         continue;
      }
      const Matrix3 E3 = ParseMatrix3(it->second.e3);
      const char expectedCentering = ExpectedConventionalCentering(bravais);

      std::vector<MatS6> prjs = type->GetSellaProjectors();
      const FaceCrossedMatrices& face = GetFaceCrossedMatrices(name);
      prjs.insert(prjs.end(), face.prjs.begin(), face.prjs.end());

      for (size_t p = 0; p < prjs.size(); ++p) {
         ++counts.arrangements;
         for (size_t k = 0; k < samplesPerArrangement; ++k) {
            const S6 y = GetValidTypeSample(prjs[p]);
            if (!LRL_Cell(y).IsValid()) {
               ++counts.skippedInvalid;
               continue;
            }
            ++counts.samples;
            MatS6 toCanon;
            std::string problem;
            if (!ToCanonicalArrangement(y, type->GetCharacter(), toCanon)) {
               problem = "did not reach the canonical arrangement";
            }
            else {
               const Matrix3 gram = TransformGram(E3, GramFromS6(toCanon * y));
               const Matrix3 T = ConventionalSettingMatrix(name, it->second.centering, gram);
               const ConventionalCellParameters cell =
                  CellFromGram(TransformGram(T, gram), it->second.centering);
               if (it->second.centering != expectedCentering) {
                  problem = "centering letter differs from the Bravais type";
               }
               else if (!ConventionalSymmetryHolds(bravais, cell)) {
                  problem = "conventional cell lacks the symmetry: " + FormatConventionalCell(cell);
               }
               else if (bravais == "mC") {
                  // the I-centered alternative, checked for every mC sample
                  const Matrix3 setting = TransformGram(T, gram);
                  const Matrix3 TI = MonoclinicCToI(setting);
                  const ConventionalCellParameters cellI =
                     CellFromGram(TransformGram(TI, setting), 'I');
                  if (!ConventionalSymmetryHolds(bravais, cellI)) {
                     problem = "I-centered cell lacks the symmetry: " + FormatConventionalCell(cellI);
                  }
               }
            }
            if (!problem.empty()) {
               ++counts.failures;
               if (counts.failures <= maxPrinted) {
                  os << ";   " << name << " arrangement " << p
                     << ((p < type->GetSellaProjectors().size()) ? " (library)" : " (face)")
                     << " S " << y << ": " << problem << std::endl;
               }
            }
         }
      }
   }
   return counts;
}

#endif // CONVENTIONALCELL_H