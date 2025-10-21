#ifndef TESTDELTA_H
#define TESTDELTA_H

#include <string>
#include <vector>
#include "G6.h"

class TestDelta {
public:
   struct Result {
      std::string trigger;
      bool fired;
      std::string reason;
   };

   static std::vector<Result> RunAllTriggers(const G6& vin, double delta);
   static void CompareDeltas(const G6& vin);
   static void RunTriggerDiagnostics();

   // Static trigger functions
   static std::pair<bool, std::string> TriggerSP1(const G6& vin, double delta);
   static std::pair<bool, std::string> TriggerSP2(const G6& vin, double delta);
   static std::pair<bool, std::string> TriggerR5(const G6& vin, double delta);
   static std::pair<bool, std::string> TriggerR6(const G6& vin, double delta);
   static std::pair<bool, std::string> TriggerR7(const G6& vin, double delta);
   static std::pair<bool, std::string> TriggerR8(const G6& vin, double delta);
   static std::pair<bool, std::string> TriggerR9(const G6& vin, double delta);
   static std::pair<bool, std::string> TriggerR10(const G6& vin, double delta);
   static std::pair<bool, std::string> TriggerR11(const G6& vin, double delta);
   static std::pair<bool, std::string> TriggerR12(const G6& vin, double delta);
};

#endif
