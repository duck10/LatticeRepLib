// this program grew from KIMI.ai's translation of the Wikipedia page for
// Hexagonal crystal family
// https://en.wikipedia.org/wiki/Hexagonal_crystal_family
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>

#include "R_Settings.h"
#include "ThetaInsensitive.h"

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
      "  sg <number>      Show the space-group whose number is <number>.\n"
      "                   Examples: sg 186, spacegroup 152, spg 194\n\n"
      "  bravais hP       List every space-group whose Bravais lattice is hP.\n"
      "  bravais hR       List every space-group whose Bravais lattice is hR.\n"
      "                   Examples: bravais hp, bravais hr, lattice hp\n\n"
      "  system Trigonal  List every space-group belonging to the Trigonal system.\n"
      "  system Hexagonal List every space-group belonging to the Hexagonal system.\n"
      "                   Examples: sys Trigonal, system hex, crystal hexagonal\n\n"
      "  family Hexagonal List every space-group that belongs to the Hexagonal family.\n"
      "                   Examples: fam Hexagonal, family hex, hexagonal family\n\n"
      "  r_sttings        some information about rhombohedral lattices including\n"
      "                   obverse/reverse and indexing.\n\n"
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

inline Cell hex_to_rhom(double aH, double cH)
{
   const double aR = std::sqrt((aH * aH) / 3.0 + (cH * cH) / 9.0);
   // here is the WRONG formual that KIMI kept insisting was correct
   //const double cosA = (2.0 * aH * aH - cH * cH) / (2.0 * (aH * aH + cH * cH));
   //const double alpha = std::acos(cosA) * 180.0 / PI;

   double cos_alpha_r = (2.0 * cH *cH - 3.0 * aH *aH) /
      (2.0 * cH * cH + 6.0 * aH *aH);
   const double alpha = acos(cos_alpha_r) * 180.0 / PI;
   return { aR, aR, aR, alpha, alpha, alpha };
}

inline Cell rhom_to_hex(double aR, double alphaDeg)
{
   const double alpha = alphaDeg * PI / 180.0;
   const double aH = aR * std::sqrt(2.0 * (1.0 - std::cos(alpha)));
   const double cH = aR * std::sqrt(3.0 * (1.0 + 2.0 * std::cos(alpha)));
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
      std::cerr << "DEBUG >>> hex_to_rhom  aR=" << rhom.a
         << "  alpha=" << rhom.alpha << '\n';   // <- HERE
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
   const Bravais b = (isClose(token, "hP")) ? Bravais::hP : Bravais::hR;
   for (const auto& sg : byBravais(b))
      print(sg);
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

// ------------------------------------------------------------------
// 7.  Interactive shell
// ------------------------------------------------------------------
// ------------------------------------------------------------------
//  Global processing functions (unchanged except signature)
// ------------------------------------------------------------------
void process_sg(const int n);
void process_bravais(const std::string& token);
void process_system(const std::string& token);
void process_family();
void process_help();
void process_hex(const double a, const  double c);
void process_rhomb(const double a, const double alpha);
void process_cell(const Cell& c);

// ------------------------------------------------------------------
//  Centralised input loop
// ------------------------------------------------------------------
int main()
{
   std::cout << "Navigating the Hexagonal crystal family\n\n";
   std::cout << "This program was partly written by the AI Kimi.\n"
      "Kimi used the Wikipedia page for Hexagonal crystal family\n"
      "for the source data.\n\n";

   std::cout << "Commands:\n"
      "  sg <number>\n"
      "  bravais hP|hR\n"
      "  system Trigonal|Hexagonal\n"
      "  family Hexagonal\n"
      "  hex <a> <c>\n"
      "  rhomb <a> <alpha>\n"
      "  cell <a> <b> <c> <α> <β> <γ>\n"
      "  help\n"
      "  q (quit)\n\n";

   std::string line;
   while (std::cout << "> ", std::getline(std::cin, line))
   {
      std::istringstream iss(line);
      std::string cmd;
      iss >> cmd;
      if (cmd.empty()) continue;
      // -------------- dispatch ----------------
      else if (isClose(cmd, "q", .5) || isClose(cmd, "quit", .5)) break;
      else if (isClose(cmd, "help", .4)) {
         process_help();
      }
      else if (isClose(cmd, "sg", .3)) {
         int n;  iss >> n;
         if (!iss.fail()) process_sg(n);
         else std::cout << "Need an integer after sg\n";
      }
      else if (isClose(cmd, "bravais", .3)) {
         std::string token;  iss >> token;
         if (!token.empty()) process_bravais(token);
         else std::cout << "Need hP or hR after bravais\n";
      }
      else if (isClose(cmd, "system", .4)) {
         std::string token;  iss >> token;
         if (!token.empty()) process_system(token);
         else std::cout << "Need Trigonal or Hexagonal after system\n";
      }
      else if (isClose(cmd, "family", .5)) {
         std::string token;  iss >> token;
         if (isClose(token, "Hexagonal", .7))
            process_family();
         else
            std::cout << "Unknown family\n";
      }
      else if (isClose(cmd, "hex")) {
         double a, c;  iss >> a >> c;
         if (!iss.fail()) process_hex(a, c);
         else std::cout << "Need 2 numbers: a c\n";
      }
      else if (isClose(cmd, "rhomb")) {
         double a, alpha;  iss >> a >> alpha;
         if (!iss.fail()) process_rhomb(a, alpha);
         else std::cout << "Need 2 numbers: a alpha(deg)\n";
      }
      else if (isClose(cmd, "cell")) {
         double a, b, c, alpha, beta, gamma;
         iss >> a >> b >> c >> alpha >> beta >> gamma;
         if (!iss.fail()) {
            Cell cell{ a, b, c, alpha, beta, gamma };
            process_cell(cell);
         }
         else {
            std::cout << "Usage: cell a b c alpha beta gamma\n";
         }
      }
      else if (isClose(cmd, "r_settings", .6)) {
         R_Setting_Info();
      }
      else
      {
         std::cout << "Unknown command\n";
      }
   }
   return 0;
}