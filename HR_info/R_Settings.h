#ifndef R_SETTINGS_H
#define R_SETTINGS_H

#include <iostream>
#include <cmath>
#include <iomanip>
#include <array>
#include <vector>

static const double M_PI = 4.0F * ::atan(1.0F);

enum class RhomboSetting {
   OBVERSE,  // Standard setting, rhombo axes along +c body diagonals
   REVERSE   // Alternative setting, rhombo axes along -c body diagonals
};

struct HexagonalCell {
   double a, c;  // lattice parameters in Angstroms
   double alpha, beta, gamma;  // angles in degrees (90, 90, 120)

   HexagonalCell(double a_val, double c_val)
      : a(a_val), c(c_val), alpha(90.0), beta(90.0), gamma(120.0) {
   }
};

struct RhombohedralCell {
   double a;      // lattice parameter in Angstroms
   double alpha;  // angle in degrees
   RhomboSetting setting;

   RhombohedralCell() : a(0.0), alpha(0.0), setting(RhomboSetting::OBVERSE) {}
   RhombohedralCell(double a_val, double alpha_val, RhomboSetting s = RhomboSetting::OBVERSE)
      : a(a_val), alpha(alpha_val), setting(s) {
   }
};

struct MillerIndices {
   int h, k, l;
   MillerIndices(int h_val, int k_val, int l_val) : h(h_val), k(k_val), l(l_val) {}
};

// Convert from hexagonal to rhombohedral setting
RhombohedralCell hexToRhombo(const HexagonalCell& hex, RhomboSetting setting = RhomboSetting::OBVERSE) {
   RhombohedralCell rhombo;
   rhombo.setting = setting;

   // Calculate rhombohedral lattice parameter (same for both settings)
   // CORRECT formula: aR = √(aH²/3 + cH²/9)
   rhombo.a = sqrt((hex.a * hex.a) / 3.0 + (hex.c * hex.c) / 9.0);

   // Calculate rhombohedral angle (same for both settings)
   // CORRECT formula: α = cos⁻¹((2*cH² - 3*aH²)/(2*(cH² + 3*aH²)))
   const double cos_alpha_r = (2.0 * hex.c * hex.c - 3.0 * hex.a * hex.a) /
      (2.0 * (hex.c * hex.c + 3.0 * hex.a * hex.a));

   rhombo.alpha = acos(cos_alpha_r) * 180.0 / M_PI;

   return rhombo;
}

// Convert from rhombohedral to hexagonal setting (inverse transformation)
HexagonalCell rhomboToHex(const RhombohedralCell& rhombo) {
   const double alpha_rad = rhombo.alpha * M_PI / 180.0;
   const double cos_alpha = cos(alpha_rad);
   const double sin_half_alpha = sin(alpha_rad / 2.0);

   // CORRECT formulas from QuantumATK documentation:
   // aH = 2*aR*sin(αR/2)
   // cH = aR*√3*√(1+2*cos(αR))
   const double aH = 2.0 * rhombo.a * sin_half_alpha;
   const double cH = rhombo.a * sqrt(3.0) * sqrt(1.0 + 2.0 * cos_alpha);

   return HexagonalCell(aH, cH);
}

// Miller indices transformations
MillerIndices hexToRhomboIndices(const MillerIndices& hex_hkl, RhomboSetting setting) {
   if (setting == RhomboSetting::OBVERSE) {
      // Obverse: (h k l)_hex → (h-k, -h, l)_rhombo
      return MillerIndices(hex_hkl.h - hex_hkl.k, -hex_hkl.h, hex_hkl.l);
   }
   else {
      // Reverse: (h k l)_hex → (-h+k, h, l)_rhombo  
      return MillerIndices(-hex_hkl.h + hex_hkl.k, hex_hkl.h, hex_hkl.l);
   }
}

MillerIndices rhomboToHexIndices(const MillerIndices& rhombo_hkl, RhomboSetting setting) {
   if (setting == RhomboSetting::OBVERSE) {
      // Inverse of obverse transformation
      return MillerIndices(-rhombo_hkl.k, rhombo_hkl.h - rhombo_hkl.k, rhombo_hkl.l);
   }
   else {
      // Inverse of reverse transformation
      return MillerIndices(rhombo_hkl.k, rhombo_hkl.h + rhombo_hkl.k, rhombo_hkl.l);
   }
}

// Function to determine setting from systematic absences
RhomboSetting determineSettingFromAbsences(const std::vector<MillerIndices>& absent_reflections) {
   // Check extinction rules for R3 space group
   int obverse_violations = 0;
   int reverse_violations = 0;

   for (const auto& hkl : absent_reflections) {
      // Obverse rule: absent when h-k+l ≠ 3n
      if ((hkl.h - hkl.k + hkl.l) % 3 != 0) {
         // This should be absent in obverse, if it's present, it's a violation
      }
      else {
         obverse_violations++;
      }

      // Reverse rule: absent when -h+k+l ≠ 3n  
      if ((-hkl.h + hkl.k + hkl.l) % 3 != 0) {
         // This should be absent in reverse, if it's present, it's a violation
      }
      else {
         reverse_violations++;
      }
   }

   return (obverse_violations < reverse_violations) ? RhomboSetting::OBVERSE : RhomboSetting::REVERSE;
}

// Function to check if coordinates suggest obverse or reverse
RhomboSetting analyzeCoordinates(const std::vector<std::array<double, 3>>& coordinates) {
   // Look for characteristic positions
   int obverse_matches = 0;
   int reverse_matches = 0;

   const double tolerance = 0.01;

   for (const auto& coord : coordinates) {
      // Check for obverse characteristic positions
      double x = coord[0], y = coord[1], z = coord[2];

      // Obverse positions: (2/3,1/3,1/3), (1/3,2/3,2/3)
      if ((std::abs(x - 2.0 / 3.0) < tolerance && std::abs(y - 1.0 / 3.0) < tolerance && std::abs(z - 1.0 / 3.0) < tolerance) ||
         (std::abs(x - 1.0 / 3.0) < tolerance && std::abs(y - 2.0 / 3.0) < tolerance && std::abs(z - 2.0 / 3.0) < tolerance)) {
         obverse_matches++;
      }

      // Reverse positions: (1/3,2/3,1/3), (2/3,1/3,2/3)  
      if ((std::abs(x - 1.0 / 3.0) < tolerance && std::abs(y - 2.0 / 3.0) < tolerance && std::abs(z - 1.0 / 3.0) < tolerance) ||
         (std::abs(x - 2.0 / 3.0) < tolerance && std::abs(y - 1.0 / 3.0) < tolerance && std::abs(z - 2.0 / 3.0) < tolerance)) {
         reverse_matches++;
      }
   }

   return (obverse_matches > reverse_matches) ? RhomboSetting::OBVERSE : RhomboSetting::REVERSE;
}

// Print cell parameters
void printHexCell(const HexagonalCell& cell) {
   std::cout << std::fixed << std::setprecision(4);
   std::cout << "Hexagonal setting:\n";
   std::cout << "  a = " << cell.a << " Å\n";
   std::cout << "  c = " << cell.c << " Å\n";
   std::cout << "  α = " << cell.alpha << "\n";
   std::cout << "  β = " << cell.beta << "\n";
   std::cout << "  γ = " << cell.gamma << "\n";
}

void printRhomboCell(const RhombohedralCell& cell) {
   std::cout << std::fixed << std::setprecision(4);
   std::cout << "Rhombohedral setting ("
      << (cell.setting == RhomboSetting::OBVERSE ? "Obverse" : "Reverse") << "):\n";
   std::cout << "  a = " << cell.a << " Å\n";
   std::cout << "  alpha = " << cell.alpha << "degrees\n";
}

void printIndices(const MillerIndices& indices, const std::string& label) {
   std::cout << label << ": (" << indices.h << " " << indices.k << " " << indices.l << ")\n";
}

void R_Setting_Info() {
   // Example: Convert hexagonal cell to both rhombohedral settings
   const HexagonalCell hex_cell(5.123, 13.856);

   std::cout << "=== Cell Parameter Conversion ===\n\n";
   printHexCell(hex_cell);
   std::cout << "\n";

   // Convert to both settings
   const RhombohedralCell rhombo_obverse = hexToRhombo(hex_cell, RhomboSetting::OBVERSE);
   const RhombohedralCell rhombo_reverse = hexToRhombo(hex_cell, RhomboSetting::REVERSE);

   printRhomboCell(rhombo_obverse);
   std::cout << "\n";
   printRhomboCell(rhombo_reverse);

   std::cout << "\n=== How to Distinguish Settings ===\n\n";
   std::cout << "Method 1: Systematic Absences (R3 space group)\n";
   std::cout << "  Obverse: h-k+l ≠ 3n are absent\n";
   std::cout << "  Reverse: -h+k+l ≠ 3n are absent\n\n";

   std::cout << "Method 2: Coordinate Analysis\n";
   std::cout << "  When a rhombohedral lattice is described in hexagonal setting,\n";
   std::cout << "  it becomes an R-centered hexagonal cell with 3 lattice points:\n";
   std::cout << "  - One at origin: (0, 0, 0)\n";
   std::cout << "  - Two additional centering points that differ by setting:\n\n";

   std::cout << "  OBVERSE setting - rhombohedral axes point in +c direction:\n";
   std::cout << "    Additional lattice points: (2/3,1/3,1/3), (1/3,2/3,2/3)\n";
   std::cout << "    Atoms often cluster around these positions\n\n";

   std::cout << "  REVERSE setting - rhombohedral axes point in -c direction:\n";
   std::cout << "    Additional lattice points: (1/3,2/3,1/3), (2/3,1/3,2/3)\n";
   std::cout << "    Atoms often cluster around these positions\n\n";

   std::cout << "  HOW TO USE:\n";
   std::cout << "  - Examine atomic coordinates in your structure file (CIF, etc.)\n";
   std::cout << "  - Look for atoms near the characteristic positions above\n";
   std::cout << "  - The pattern tells you which setting is being used\n\n";

   std::cout << "  EXAMPLE:\n";
   std::cout << "  If atoms are found near (0.667, 0.333, 0.333) and (0.333, 0.667, 0.667)\n";
   std::cout << "  → Likely OBVERSE setting\n";
   std::cout << "  If atoms are found near (0.333, 0.667, 0.333) and (0.667, 0.333, 0.667)\n";
   std::cout << "  → Likely REVERSE setting\n\n";

   std::cout << "Method 3: Miller Indices Check\n";
   std::cout << "  The same physical reflection has different indices in each setting.\n";
   std::cout << "  Compare known reflections with expected transformations:\n";

   std::cout << "=== Miller Indices Transformations ===\n\n";

   // Example Miller indices transformations
   std::vector<MillerIndices> hex_indices = {
       MillerIndices(1, 0, 0),
       MillerIndices(1, 1, 0),
       MillerIndices(0, 0, 1),
       MillerIndices(1, 0, 3)
   };

   for (const auto& hex_idx : hex_indices) {
      std::cout << "Hexagonal ";
      printIndices(hex_idx, "(h k l)");

      const MillerIndices rhombo_obv = hexToRhomboIndices(hex_idx, RhomboSetting::OBVERSE);
      std::cout << "  → Obverse ";
      printIndices(rhombo_obv, "(h k l)");

      const MillerIndices rhombo_rev = hexToRhomboIndices(hex_idx, RhomboSetting::REVERSE);
      std::cout << "  → Reverse ";
      printIndices(rhombo_rev, "(h k l)");

      std::cout << "\n";
   }

   std::cout << "=== Key Points for IUCr Publications ===\n";
   std::cout << "• Always specify which setting (obverse/reverse) you're using\n";
   std::cout << "• Cell parameters (a, alpha) are identical for both settings\n";
   std::cout << "• Miller indices and atomic coordinates differ between settings\n";
   std::cout << "• Check your software's default convention\n";
   std::cout << "• Use systematic absences to determine setting from diffraction data\n";

}

#endif // R_SETTINGS_H
