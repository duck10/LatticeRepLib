#ifndef PROGRAMHEADER_H
#define PROGRAMHEADER_H

// ---------------------------------------------------------------------------
// ShowProgramHeader -- prints a short banner giving the program name, the
// date/time the program is being RUN, and the date/time the executable was
// COMPILED (from the compiler-supplied __DATE__/__TIME__ macros). Used by
// both ShowHelp() and the TEST-mode output so both share one consistent,
// clearly-set-off banner. ASCII only.
// ---------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <cstdlib>

inline void ShowProgramHeader(const std::string& programName) {
   // Run date/time (current, UTC, from the system clock). std::chrono's
   // system_clock + std::format need no deprecated CRT function (no
   // std::localtime, no pragma, no #if _MSC_VER) -- fully standard C++20,
   // identical on every compiler. Shown in UTC rather than local time so
   // the stamp is unambiguous regardless of machine time zone/DST.
   const auto runTimePoint =
      std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
   const std::string runDateStr = std::format("{:%Y-%m-%d %H:%M:%S}", runTimePoint);

   // Compile date/time, reformatted from __DATE__ ("Mmm dd yyyy") and
   // __TIME__ ("hh:mm:ss") into the same yyyy-mm-dd style as the run date,
   // so the two lines are easy to compare at a glance. __DATE__ has a fixed,
   // guaranteed layout (3-char month, space-padded 2-digit day, 4-digit
   // year), so it is parsed directly by position rather than with sscanf.
   static const std::string MONTHS =
      "JanFebMarAprMayJunJulAugSepOctNovDec";
   const std::string compileDate(__DATE__);
   const std::string monthStr = compileDate.substr(0, 3);
   const std::string dayStr = compileDate.substr(4, 2);
   const std::string yearStr = compileDate.substr(7, 4);

   const int compileMonth = static_cast<int>(MONTHS.find(monthStr)) / 3 + 1;
   const int compileDay = std::atoi(dayStr.c_str());  // atoi tolerates the leading space for days < 10

   const std::string compileDateStr = std::format("{}-{:02d}-{:02d} {}",
      yearStr, compileMonth, compileDay, __TIME__);
   const std::string sourceStr("Andrews and Bernstein, 2026");

   const std::string border(70, '=');
   std::cout << border << std::endl;
   std::cout << "  " << programName << std::endl;
   std::cout << std::format("  {:<23}: {}", "Run date (UTC)", runDateStr) << std::endl;
   std::cout << std::format("  {:<23}: {}", "Compiled (local time)", compileDateStr) << std::endl;
   std::cout << std::format("  {:<23}: {}", "Compiled (local time)", sourceStr) << std::endl;
   std::cout << border << std::endl;
   std::cout << std::endl;
}

#endif // PROGRAMHEADER_H