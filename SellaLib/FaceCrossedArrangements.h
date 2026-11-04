#ifndef FACECROSSEDARRANGEMENTS_H
#define FACECROSSEDARRANGEMENTS_H

// FaceCrossedArrangements.h  (Nov 2026)
//
// The missing face arrangements of SELLA white paper v4, Appendix B: reduced
// forms of special members of a Delone type (lattices on a cone face) that
// no arrangement in the type's own projector list covers. 143 arrangements
// across 11 types; the other 13 types are complete.
//
// Each arrangement is a 6-character pattern over slots s1..s6: '0' is an
// exact zero, equal letters are equal values. The projector follows from the
// pattern (white paper section 3): P[i][j] = 1/|G| when slots i and j carry
// the same letter G, 0 otherwise; Perp = I - P. So no matrices are stored
// here -- only the patterns, which can be checked by eye against the paper.
//
// Every pattern was checked against LabeledSellaMatrices.cpp (Nov 2026):
// each one equals a projector already stored for the donor type named in
// the comments (the Appendix B table), and none equals a projector of its
// own type. VerifyFaceCrossedArrangements repeats that check at run time.
//
// Header-only (inline), so no project or makefile changes are needed.

#include "LabeledSellaMatrices.h"
#include "MatS6.h"
#include "S6.h"

#include <cstddef>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

// Projector for one arrangement pattern such as "0ab0ab".
inline MatS6 ProjectorFromArrangement(const std::string& pattern) {
   if (pattern.size() != 6) {
      throw std::runtime_error("ProjectorFromArrangement: pattern must have 6 characters: " + pattern);
   }
   double p[36] = { 0.0 };
   for (size_t i = 0; i < 6; ++i) {
      if (pattern[i] == '0') continue;
      size_t groupSize = 0;
      for (size_t k = 0; k < 6; ++k) {
         if (pattern[k] == pattern[i]) ++groupSize;
      }
      for (size_t j = 0; j < 6; ++j) {
         if (pattern[j] == pattern[i]) {
            p[6 * i + j] = 1.0 / static_cast<double>(groupSize);
         }
      }
   }
   return MatS6(p[0], p[1], p[2], p[3], p[4], p[5],
      p[6], p[7], p[8], p[9], p[10], p[11],
      p[12], p[13], p[14], p[15], p[16], p[17],
      p[18], p[19], p[20], p[21], p[22], p[23],
      p[24], p[25], p[26], p[27], p[28], p[29],
      p[30], p[31], p[32], p[33], p[34], p[35]);
}

inline MatS6 PerpFromArrangement(const std::string& pattern) {
   return MatS6().unit() - ProjectorFromArrangement(pattern);
}

// The Appendix B patterns, by Delone type name.
inline std::map<std::string, std::vector<std::string>> BuildFaceCrossedTable() {
   std::map<std::string, std::vector<std::string>> table;
   // R1: 12 missing, already in the library as C5 (12)
   table["R1"] = {
      "00a0aa", "00aa0a", "0a00aa", "0a0aa0", "0aa00a", "0aa0a0", "a00a0a", "a00aa0",
      "a0a00a", "a0aa00", "aa00a0", "aa0a00"
   };
   // R3: 4 missing, already in the library as C5 (4)
   table["R3"] = {
      "000aaa", "0aaa00", "a0a0a0", "aa000a"
   };
   // O1B: 6 missing, already in the library as O3 (6)
   table["O1B"] = {
      "0aa0bb", "0ab0ba", "a0ab0b", "a0bb0a", "aa0bb0", "ab0ba0"
   };
   // O2: 3 missing, already in the library as O3 (3)
   table["O2"] = {
      "0ab0ab", "a0ba0b", "ab0ab0"
   };
   // O4: 12 missing, already in the library as T5 (12)
   table["O4"] = {
      "00a0ba", "00ab0a", "0a00ab", "0a0ba0", "0ab00b", "0ab0a0", "a00a0b", "a00ab0",
      "a0b00b", "a0ba00", "ab00b0", "ab0a00"
   };
   // M1A: 30 missing, already in the library as O4 (24), M2B (6)
   table["M1A"] = {
      "00aabc", "00abac", "0a0abc", "0a0bca", "0abc0a", "0abc0c", "0abcab", "0abcb0",
      "0abcc0", "a00bac", "a00bca", "a0b0ca", "a0b0cc", "a0bacb", "a0bbc0", "a0bcc0",
      "aab0c0", "aabc00", "ab00ac", "ab00cc", "ab0abc", "ab0b0c", "ab0c0c", "aba00c",
      "abac00", "abb00c", "abb0c0", "abc0bc", "abca0c", "abcab0"
   };
   // M1B: 18 missing, already in the library as M3 (12), M2B (6)
   table["M1B"] = {
      "0aa0bc", "0aabcc", "0ab0bc", "0ab0ca", "0ab0cc", "a0ab0c", "a0abcb", "a0bb0c",
      "a0bc0a", "a0bc0c", "aa0bbc", "aa0bc0", "ab0bc0", "ab0ca0", "ab0cc0", "abc0cb",
      "abcba0", "abcc0a"
   };
   // M2A: 18 missing, already in the library as O4 (12), M3 (6)
   table["M2A"] = {
      "00abbc", "0a0bcb", "0ab0ac", "0ab0cb", "0aba0c", "0abbc0", "a00bcc", "a0b0ac",
      "a0ba0c", "a0bc0b", "a0bcb0", "aab00c", "ab00ca", "ab0ac0", "ab0c0b", "ab0cb0",
      "aba0c0", "abbc00"
   };
   // M2B: 24 missing, already in the library as T5 (24)
   table["M2B"] = {
      "00a0ab", "00a0bb", "00aa0b", "00ab0b", "0a00ba", "0a00bb", "0a0ab0", "0a0bb0",
      "0aa00b", "0aa0b0", "0ab00a", "0ab0b0", "a00b0a", "a00b0b", "a00ba0", "a00bb0",
      "a0a00b", "a0ab00", "a0b00a", "a0bb00", "aa00b0", "aa0b00", "ab00a0", "ab0b00"
   };
   // M3: 12 missing, already in the library as T5 (12)
   table["M3"] = {
      "000aab", "000aba", "000abb", "0aab00", "0aba00", "0abb00", "a0a0b0", "a0b0a0",
      "a0b0b0", "aa000b", "ab000a", "ab000b"
   };
   // A3: 4 missing, already in the library as O5 (4)
   table["A3"] = {
      "000abc", "0abc00", "a0b0c0", "ab000c"
   };
   return table;
}

inline const std::map<std::string, std::vector<std::string>>& FaceCrossedTable() {
   static const std::map<std::string, std::vector<std::string>> table = BuildFaceCrossedTable();
   return table;
}

// The patterns for one type; empty for the 13 types that need none.
inline const std::vector<std::string>& FaceCrossedArrangements(const std::string& type) {
   static const std::vector<std::string> none;
   const auto& table = FaceCrossedTable();
   const auto it = table.find(type);
   return (it == table.end()) ? none : it->second;
}

struct FaceCrossedMatrices {
   std::vector<MatS6> prjs;
   std::vector<MatS6> perps;
};

inline std::map<std::string, FaceCrossedMatrices> BuildFaceCrossedMatrices() {
   std::map<std::string, FaceCrossedMatrices> out;
   for (const auto& entry : FaceCrossedTable()) {
      FaceCrossedMatrices& m = out[entry.first];
      for (const auto& pattern : entry.second) {
         m.prjs.push_back(ProjectorFromArrangement(pattern));
         m.perps.push_back(PerpFromArrangement(pattern));
      }
   }
   return out;
}

// Projectors and perps for one type, built once and cached.
inline const FaceCrossedMatrices& GetFaceCrossedMatrices(const std::string& type) {
   static const std::map<std::string, FaceCrossedMatrices> all = BuildFaceCrossedMatrices();
   static const FaceCrossedMatrices none;
   const auto it = all.find(type);
   return (it == all.end()) ? none : it->second;
}

inline size_t CountFaceCrossedArrangements() {
   size_t n = 0;
   for (const auto& entry : FaceCrossedTable()) n += entry.second.size();
   return n;
}

// True when two matrices act identically on the six unit vectors.
inline bool SameMatS6(const MatS6& a, const MatS6& b) {
   static const std::vector<S6> basis = {
      S6("1 0 0 0 0 0"), S6("0 1 0 0 0 0"), S6("0 0 1 0 0 0"),
      S6("0 0 0 1 0 0"), S6("0 0 0 0 1 0"), S6("0 0 0 0 0 1") };
   for (const auto& e : basis) {
      if ((a * e - b * e).norm() > 1.0E-9) return false;
   }
   return true;
}

// Run-time check of the claims in the header comment: every pattern's
// projector is already stored for some OTHER type, and none is stored for
// its own type. Prints one line per problem; returns the number of problems.
inline size_t VerifyFaceCrossedArrangements(std::ostream& os) {
   size_t problems = 0;
   const std::vector<LabeledSellaMatrices>& library = LabeledSellaMatrices::projectors;
   for (const auto& entry : FaceCrossedTable()) {
      const std::string& type = entry.first;
      for (const auto& pattern : entry.second) {
         const MatS6 p = ProjectorFromArrangement(pattern);
         bool inOwn = false;
         bool inOther = false;
         for (const auto& labeled : library) {
            for (const auto& stored : labeled.GetMatrices()) {
               if (!SameMatS6(p, stored)) continue;
               if (labeled.GetLabel() == type) inOwn = true;
               else inOther = true;
            }
         }
         if (inOwn || !inOther) {
            ++problems;
            os << ";Warning: FaceCrossed " << type << " " << pattern
               << (inOwn ? " is already in its own list" : " matches no stored projector")
               << std::endl;
         }
      }
   }
   return problems;
}

#endif // FACECROSSEDARRANGEMENTS_H
