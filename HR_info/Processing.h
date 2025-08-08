#ifndef PROCESSING_H
#define PROCESSING_H

#include <cmath>
#include <string>
#include <vector>
#include <optional>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <iomanip>

#include "ThetaInsensitive.h"
#include "LRL_Cell.h"
#include "LatticeCell.h"

static const double PI = 4.0F * ::atan(1.0F);
static const double DEG = 180.0 / PI;
static const double RAD = 1.0 / DEG;

// ------------------------------------------------------------------
// 1.  Enumerations
// ------------------------------------------------------------------
enum class Family { Hexagonal };
enum class System { Trigonal, Hexagonal };
enum class Bravais { hP, hR };

const char* to_string(Family   f) { return "Hexagonal"; }
const char* to_string(System   s)
{
   switch (s)
   {
   case System::Trigonal:  return "Trigonal";
   case System::Hexagonal: return "Hexagonal";
   }
   return "";
}
const char* to_string(Bravais  b)
{
   switch (b)
   {
   case Bravais::hP: return "hP";
   case Bravais::hR: return "hR";
   }
   return "";
}

// ------------------------------------------------------------------
// 2.  Data structures
// ------------------------------------------------------------------
struct PointGroup
{
   std::string international;
   std::string schoenflies;
};

struct SpaceGroup
{
   int         number;
   std::string symbol;
   PointGroup  pointGroup;
   System      system;
   Bravais     bravais;
};

// ------------------------------------------------------------------
// 3.  Complete database for the Hexagonal family
//     (space-groups 143–194, Trigonal + Hexagonal systems)
// ------------------------------------------------------------------
const std::vector<SpaceGroup> DB = {
   // ------------- Trigonal system -------------
   {143,"P3",   {"3","C3"},   System::Trigonal,  Bravais::hP},
   {144,"P31",  {"3","C3"},   System::Trigonal,  Bravais::hP},
   {145,"P32",  {"3","C3"},   System::Trigonal,  Bravais::hP},
   {146,"R3",   {"3","C3"},   System::Trigonal,  Bravais::hR},

   {147,"P-3",  {"-3","C3i"}, System::Trigonal,  Bravais::hP},
   {148,"R-3",  {"-3","C3i"}, System::Trigonal,  Bravais::hR},

   {149,"P312", {"32","D3"},  System::Trigonal,  Bravais::hP},
   {150,"P321", {"32","D3"},  System::Trigonal,  Bravais::hP},
   {151,"P3112",{"32","D3"},  System::Trigonal,  Bravais::hP},
   {152,"P3121",{"32","D3"},  System::Trigonal,  Bravais::hP},
   {153,"P3212",{"32","D3"},  System::Trigonal,  Bravais::hP},
   {154,"P3221",{"32","D3"},  System::Trigonal,  Bravais::hP},
   {155,"R32",  {"32","D3"},  System::Trigonal,  Bravais::hR},

   {156,"P3m1", {"3m","C3v"}, System::Trigonal,  Bravais::hP},
   {157,"P31m", {"3m","C3v"}, System::Trigonal,  Bravais::hP},
   {158,"P3c1", {"3m","C3v"}, System::Trigonal,  Bravais::hP},
   {159,"P31c", {"3m","C3v"}, System::Trigonal,  Bravais::hP},
   {160,"R3m",  {"3m","C3v"}, System::Trigonal,  Bravais::hR},
   {161,"R3c",  {"3m","C3v"}, System::Trigonal,  Bravais::hR},

   {162,"P-31m",{"-3m","D3d"},System::Trigonal,  Bravais::hP},
   {163,"P-31c",{"-3m","D3d"},System::Trigonal,  Bravais::hP},
   {164,"P-3m1",{"-3m","D3d"},System::Trigonal,  Bravais::hP},
   {165,"P-3c1",{"-3m","D3d"},System::Trigonal,  Bravais::hP},
   {166,"R-3m", {"-3m","D3d"},System::Trigonal,  Bravais::hR},
   {167,"R-3c", {"-3m","D3d"},System::Trigonal,  Bravais::hR},

   // ------------- Hexagonal system -------------
   {168,"P6",    {"6","C6"},    System::Hexagonal, Bravais::hP},
   {169,"P61",   {"6","C6"},    System::Hexagonal, Bravais::hP},
   {170,"P65",   {"6","C6"},    System::Hexagonal, Bravais::hP},
   {171,"P62",   {"6","C6"},    System::Hexagonal, Bravais::hP},
   {172,"P64",   {"6","C6"},    System::Hexagonal, Bravais::hP},
   {173,"P63",   {"6","C6"},    System::Hexagonal, Bravais::hP},

   {174,"P-6",   {"-6","C3h"},  System::Hexagonal, Bravais::hP},

   {175,"P6/m",  {"6/m","C6h"}, System::Hexagonal, Bravais::hP},
   {176,"P63/m", {"6/m","C6h"}, System::Hexagonal, Bravais::hP},

   {177,"P622",  {"622","D6"},  System::Hexagonal, Bravais::hP},
   {178,"P6122", {"622","D6"},  System::Hexagonal, Bravais::hP},
   {179,"P6522", {"622","D6"},  System::Hexagonal, Bravais::hP},
   {180,"P6222", {"622","D6"},  System::Hexagonal, Bravais::hP},
   {181,"P6422", {"622","D6"},  System::Hexagonal, Bravais::hP},
   {182,"P6322", {"622","D6"},  System::Hexagonal, Bravais::hP},

   {183,"P6mm",  {"6mm","C6v"}, System::Hexagonal, Bravais::hP},
   {184,"P6cc",  {"6mm","C6v"}, System::Hexagonal, Bravais::hP},
   {185,"P63cm", {"6mm","C6v"}, System::Hexagonal, Bravais::hP},
   {186,"P63mc", {"6mm","C6v"}, System::Hexagonal, Bravais::hP},

   {187,"P-6m2", {"6m2","D3h"}, System::Hexagonal, Bravais::hP},
   {188,"P-6c2", {"6m2","D3h"}, System::Hexagonal, Bravais::hP},
   {189,"P-62m", {"6m2","D3h"}, System::Hexagonal, Bravais::hP},
   {190,"P-62c", {"6m2","D3h"}, System::Hexagonal, Bravais::hP},

   {191,"P6/mmm", {"6/mmm","D6h"}, System::Hexagonal, Bravais::hP},
   {192,"P6/mcc", {"6/mmm","D6h"}, System::Hexagonal, Bravais::hP},
   {193,"P63/mcm",{"6/mmm","D6h"}, System::Hexagonal, Bravais::hP},
   {194,"P63/mmc",{"6/mmm","D6h"}, System::Hexagonal, Bravais::hP}
};

// 3b.  We now explicitly know the mapping
static constexpr Family SINGLE_FAMILY = Family::Hexagonal;

// ------------------------------------------------------------------
// 4.  Predicate functors (no lambdas)
// ------------------------------------------------------------------
struct NumberMatch
{
   int n;
   explicit NumberMatch(int num) : n(num) {}
   bool operator()(const SpaceGroup& sg) const { return sg.number == n; }
};

struct BravaisMatch
{
   Bravais b;
   explicit BravaisMatch(Bravais br) : b(br) {}
   bool operator()(const SpaceGroup& sg) const { return sg.bravais == b; }
};

struct SystemMatch
{
   System s;
   explicit SystemMatch(System sy) : s(sy) {}
   bool operator()(const SpaceGroup& sg) const { return sg.system == s; }
};

// ------------------------------------------------------------------
// 5.  Query functions
// ------------------------------------------------------------------
std::optional<SpaceGroup> bySpaceGroup(int n)
{
   auto it = std::find_if(DB.begin(), DB.end(), NumberMatch(n));
   return (it != DB.end()) ? std::optional<SpaceGroup>(*it) : std::nullopt;
}

std::vector<SpaceGroup> byBravais(Bravais b)
{
   std::vector<SpaceGroup> out;
   std::copy_if(DB.begin(), DB.end(), std::back_inserter(out), BravaisMatch(b));
   return out;
}

std::vector<SpaceGroup> bySystem(System s)
{
   std::vector<SpaceGroup> out;
   std::copy_if(DB.begin(), DB.end(), std::back_inserter(out), SystemMatch(s));
   return out;
}

std::vector<SpaceGroup> byFamily(Family f)
{
   // In this database every entry belongs to Hexagonal family
   return DB;
}

void showHelp()
{
   std::cout <<
      "Available commands (spelling approximation is active – you do not need to type the exact words):\n\n"
      "  sg <number>      Show the hexagonal family space-group whose number is <number>.\n"
      "                   Examples: sg 186, spacegroup 152, spg 194\n\n"
      "  bravais hP       List every space-group whose Bravais lattice is hP.\n"
      "  bravais hR       List every space-group whose Bravais lattice is hR.\n"
      "  bravais          List every space-group whose Bravais lattice is hR or hP.\n"
      "                   Examples: bravais hp, bravais hr\n\n"
      "  system Trigonal  List every space-group belonging to the Trigonal system.\n"
      "  system Hexagonal List every space-group belonging to the Hexagonal system.\n"
      "                   Examples: sys Trigonal, system hex, crystal hexagonal\n\n"
      "  family Hexagonal List every space-group that belongs to the Hexagonal family.\n"
      "                   Examples: fam Hexagonal, family hex, hexagonal family\n\n"
      "  r_settings       some information about rhombohedral lattices including\n"
      "                   obverse/reverse and indexing.\n\n"
      "  P/R/I/F/etc.     unit cell information\n\n"
      "  q                Quit the program.\n"
      "                   Any close spelling of \"quit\" also exits.\n\n";
}

// ------------------------------------------------------------------
// 6.  Pretty printer
// ------------------------------------------------------------------
void print(const SpaceGroup& sg)
{
   std::cout
      << "Space-group #" << sg.number
      << "  Symbol: " << sg.symbol
      << "  Point-group: " << sg.pointGroup.international
      << " (" << sg.pointGroup.schoenflies << ")\n"
      << "  Crystal-system: " << to_string(sg.system)
      << "  Bravais: " << to_string(sg.bravais)
      << "  Family: " << to_string(Family::Hexagonal) << '\n';
}

struct Cell {
   double a, b, c, alpha, beta, gamma;
};

struct Setting {
   Cell cell;
   std::string centre;   // "R"  or "P"
   std::string setting;  // "obverse", "reverse", or empty
};

// CORRECTED conversion functions
inline Cell hex_to_rhom(double aH, double cH)
{
   // CORRECT formulas from AFLOW documentation:
   // aR = √(aH²/3 + cH²/9)
   // α = cos⁻¹((2*cH² - 3*aH²)/(2*(cH² + 3*aH²)))
   const double aR = sqrt((aH * aH) / 3.0 + (cH * cH) / 9.0);

   const double cos_alpha_r = (2.0 * cH * cH - 3.0 * aH * aH) /
      (2.0 * (cH * cH + 3.0 * aH * aH));
   const double alpha = acos(cos_alpha_r) * 180.0 / PI;

   return { aR, aR, aR, alpha, alpha, alpha };
}

inline Cell rhom_to_hex(double aR, double alphaDeg)
{
   const double alpha_rad = alphaDeg * PI / 180.0;
   const double cos_alpha = cos(alpha_rad);
   const double sin_half_alpha = sin(alpha_rad / 2.0);

   // CORRECT formulas from QuantumATK documentation:
   // aH = 2*aR*sin(αR/2)
   // cH = aR*√3*√(1+2*cos(αR))
   const double aH = 2.0 * aR * sin_half_alpha;
   const double cH = aR * sqrt(3.0) * sqrt(1.0 + 2.0 * cos_alpha);

   return { aH, aH, cH, 90.0, 90.0, 120.0 };
}

// -----------------------------------------------------------------
//  Produce *all* correct settings for the rhombohedral lattice
// -----------------------------------------------------------------
std::vector<Setting> centredForms(const Cell& c)
{
   const double eps = 1e-3;
   std::vector<Setting> out;

   // 1. primitive rhombohedral lattice
   if (std::abs(c.a - c.b) < eps && std::abs(c.b - c.c) < eps &&
      std::abs(c.alpha - c.beta) < eps && std::abs(c.beta - c.gamma) < eps &&
      std::abs(c.alpha - 90.0) > eps)
   {
      Cell hex = rhom_to_hex(c.a, c.alpha);
      out.push_back({ hex, "R", "obverse" });
      out.push_back({ hex, "R", "reverse" });
      out.push_back({ c,   "P", "primitive" });
   }
   // 2. hexagonal / rhombohedral R-centred lattice
   else if (std::abs(c.a - c.b) < eps &&
      std::abs(c.alpha - 90.0) < eps &&
      std::abs(c.beta - 90.0) < eps &&
      std::abs(c.gamma - 120.0) < eps)
   {
      Cell rhom = hex_to_rhom(c.a, c.c);
      out.push_back({ c,   "R", "obverse" });
      out.push_back({ c,   "R", "reverse" });
      out.push_back({ rhom,"P", "primitive" });
   }
   else
   {
      std::cout << "Not a rhombohedral lattice.\n";
   }
   return out;
}

// Forward declarations for lattice cell processing
class LatticeCell;
void process_lattice_cell_input(const LatticeCell& latticeCell);

// ------------------------------------------------------------------
//  Processing helpers (no reading; they only act on already-parsed data)
// ------------------------------------------------------------------
void process_sg(const int n)
{
   if (auto sg = bySpaceGroup(n))
      print(*sg);
   else
      std::cout << "Unknown space-group #" << n << '\n';
}

void process_bravais(const std::string& token)
{
   if (token.empty()) {
      const Bravais hp = Bravais::hP;
      std::cout << "Bravais types hP\n";
      for (const auto& sg : byBravais(hp))
         print(sg);
      const Bravais hR = Bravais::hR;
      std::cout << "\nBravais types hR\n";
      for (const auto& sg : byBravais(hR))
         print(sg);
   }
   else
   {
      const Bravais b = (isClose(token, "hP")) ? Bravais::hP : Bravais::hR;
      for (const auto& sg : byBravais(b))
         print(sg);
   }
}

void process_system(const std::string& token)
{
   const System s = (isClose(token, "Trigonal")) ? System::Trigonal : System::Hexagonal;
   for (const auto& sg : bySystem(s))
      print(sg);
}

void process_family()
{
   for (const auto& sg : byFamily(Family::Hexagonal))
      print(sg);
}

void process_help() { showHelp(); }

void process_hex(const double a, const double c)
{
   const Cell in{ a, a, c, 90.0, 90.0, 120.0 };
   const auto list = centredForms(in);
   for (const auto& s : list)
   {
      std::cout << s.centre << "  "
         << std::fixed << std::setprecision(4)
         << s.cell.a << ' ' << s.cell.b << ' ' << s.cell.c
         << "  " << s.cell.alpha << ' ' << s.cell.beta << ' ' << s.cell.gamma;
      if (!s.setting.empty()) std::cout << "  (" << s.setting << ')';
      std::cout << '\n';
   }
}

void process_rhomb(const double a, const double alpha)
{
   const Cell in{ a, a, a, alpha, alpha, alpha };
   const auto list = centredForms(in);
   for (const auto& s : list)
   {
      std::cout << s.centre << "  "
         << std::fixed << std::setprecision(4)
         << s.cell.a << ' ' << s.cell.b << ' ' << s.cell.c
         << "  " << s.cell.alpha << ' ' << s.cell.beta << ' ' << s.cell.gamma;
      if (!s.setting.empty()) std::cout << "  (" << s.setting << ')';
      std::cout << '\n';
   }
}

void process_cell(const Cell& c)
{
   const auto list = centredForms(c);
   for (const auto& s : list)
   {
      std::cout << s.centre << "  "
         << std::fixed << std::setprecision(4)
         << s.cell.a << ' ' << s.cell.b << ' ' << s.cell.c
         << "  " << s.cell.alpha << ' ' << s.cell.beta << ' ' << s.cell.gamma;
      if (!s.setting.empty()) std::cout << "  (" << s.setting << ')';
      std::cout << '\n';
   }
}


// Implementation of process_lattice_cell_input function
// Add this before the #endif in Processing.h

// Forward declarations needed
class LatticeCell;
class LRL_Cell;

inline void process_lattice_cell_input(const LatticeCell& latticeCell) {
   std::cout << "\n=== Processing Lattice Cell Input ===\n";

   // Extract cell information using correct method names
   const LRL_Cell& lrlCell = latticeCell.getCell();
   const std::string& latticeType = latticeCell.getLatticeType();
   const std::string& inputLine = latticeCell.getInputLine();

   std::cout << "Input: " << inputLine << "\n";
   std::cout << "Lattice Type: " << latticeType << "\n";

   const LRL_Cell_Degrees d(lrlCell);
   std::cout << std::fixed << std::setprecision(4);
   std::cout << "Cell parameters: "
      << LRL_Cell_Degrees(lrlCell) << "\n";

   const double eps = 1e-3;

   // Check for primitive rhombohedral: a=b=c, α=β=γ

   const bool isPrimitiveRhombo =
      (std::abs(d[0]-d[1]  )< eps) &&
      (std::abs(d[1]-d[2]  )< eps) &&
      (std::abs(d[3] - d[4])< eps) &&
      (std::abs(d[4] - d[5])< eps);

   // Check for hexagonal rhombohedral: a=b≠c, α=β=90°, γ=120°
   const bool isHexRhombo =
      (std::abs(d[0] - d[1]) < eps) &&
      (std::abs(d[3] - 90.0) < eps) &&
      (std::abs(d[4] - 90.0) < eps) &&
      (std::abs(d[5] -120.0) < eps);

   // Extract cell parameters directly from LRL_Cell array indexing
// LRL_Cell stores: [a, b, c, alpha, beta, gamma] with angles in radians
   Cell c;
   c.a = lrlCell[0];      // a
   c.b = lrlCell[1];      // b
   c.c = lrlCell[2];      // c
   c.alpha = lrlCell[3] * 180.0 / PI;  // alpha in degrees
   c.beta = lrlCell[4] * 180.0 / PI;   // beta in degrees
   c.gamma = lrlCell[5] * 180.0 / PI;  // gamma in degrees

   if (isPrimitiveRhombo) {
      std::cout << "\n=== Rhombohedral Information ===\n";
      std::cout << "Primitive rhombohedral cell\n";
      std::cout << "alpha = beta = gamma = " << c.alpha << " degrees\n";

      // Show all equivalent settings using existing function
      const std::vector<Setting> settings = centredForms(c);
      if (!settings.empty()) {
         std::cout << "\nEquivalent settings:\n";
         for (const Setting& s : settings) {
            std::cout << s.centre << "  "
               << std::fixed << std::setprecision(4)
               << s.cell.a << ' ' << s.cell.b << ' ' << s.cell.c
               << "  " << s.cell.alpha << ' ' << s.cell.beta << ' ' << s.cell.gamma;
            if (!s.setting.empty()) {
               std::cout << "  (" << s.setting << ')';
            }
            std::cout << '\n';
         }
      }

      // Show hexagonal equivalent - need to include R_Settings.h for these functions
      // You'll need: #include "R_Settings.h" at the top of Processing.h
      const RhombohedralCell rhomboCell(c.a, c.alpha);
      const HexagonalCell hexCell = rhomboToHex(rhomboCell);
      std::cout << "\nHexagonal equivalent: a=" << hexCell.a
         << " c=" << hexCell.c << "\n";

      // Verify volume conservation
      const double alpha_rad = c.alpha * PI / 180.0;
      const double cos_alpha = cos(alpha_rad);
      const double rhombo_volume = c.a * c.a * c.a * sqrt(1.0 + 2.0 * cos_alpha * cos_alpha * cos_alpha
         - 3.0 * cos_alpha * cos_alpha);
      const double hex_volume = hexCell.a * hexCell.a * hexCell.c * sqrt(3.0) / 2.0;
      std::cout << "Rhombohedral volume: " << rhombo_volume << "\n";
      std::cout << "Hexagonal volume: " << hex_volume << " (should be 3x rhombohedral)\n";
      std::cout << "Volume ratio: " << hex_volume / rhombo_volume << " (should be 3.0)\n";

      // Special case information
      if (std::abs(c.alpha - 60.0) < eps) {
         std::cout << "Note: alpha=60 degrees corresponds to cubic close packing\n";
      }
      else if (std::abs(c.alpha - 90.0) < eps) {
         std::cout << "Note: alpha=90 degrees corresponds to simple cubic\n";
      }
      else if (c.alpha < 45.0) {
         std::cout << "Note: Very acute rhombohedral angle (" << c.alpha << " degrees)\n";
      }
      else if (c.alpha > 135.0) {
         std::cout << "Note: Very obtuse rhombohedral angle (" << c.alpha << " degrees)\n";
      }
   }
   else if (isHexRhombo) {
      std::cout << "\n=== Rhombohedral Information ===\n";
      std::cout << "Hexagonal representation of rhombohedral lattice\n";

      // Show all equivalent settings
      const std::vector<Setting> settings = centredForms(c);
      if (!settings.empty()) {
         std::cout << "\nEquivalent settings:\n";
         for (const Setting& s : settings) {
            std::cout << s.centre << "  "
               << std::fixed << std::setprecision(4)
               << s.cell.a << ' ' << s.cell.b << ' ' << s.cell.c
               << "  " << s.cell.alpha << ' ' << s.cell.beta << ' ' << s.cell.gamma;
            if (!s.setting.empty()) {
               std::cout << "  (" << s.setting << ')';
            }
            std::cout << '\n';
         }
      }

      const HexagonalCell hexCell(c.a, c.c);
      const RhombohedralCell rhomboObverse = hexToRhombo(hexCell, RhomboSetting::OBVERSE);

      std::cout << "\nRhombohedral parameters: a=" << rhomboObverse.a
         << " alpha=" << rhomboObverse.alpha << " degrees\n";
      std::cout << "Obverse/reverse settings available (see 'r_settings' for details)\n";
   }
   else {
      std::cout << "\n=== Cell Information ===\n";
      std::cout << "This cell does not have rhombohedral symmetry\n";

      // Still show equivalent settings if available from centredForms
      const std::vector<Setting> settings = centredForms(c);
      if (!settings.empty()) {
         std::cout << "\nEquivalent settings:\n";
         for (const Setting& s : settings) {
            std::cout << s.centre << "  "
               << std::fixed << std::setprecision(4)
               << s.cell.a << ' ' << s.cell.b << ' ' << s.cell.c
               << "  " << s.cell.alpha << ' ' << s.cell.beta << ' ' << s.cell.gamma;
            if (!s.setting.empty()) {
               std::cout << "  (" << s.setting << ')';
            }
            std::cout << '\n';
         }
      }
   }

   std::cout << "\n";
}


#endif // !PROCESSING_H
