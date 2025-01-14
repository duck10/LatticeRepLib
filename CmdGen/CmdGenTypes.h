#ifndef CMDGENTYPES_H
#define CMDGENTYPES_H

#include <map>
#include <string>

enum class CmdGenMode {
   ALL,
   DELONE,
   NIGGLI,
   GRUBER,
   RANDOM,
};

// from CmdGen.cpp

class CmdGenTypes {

public:
   bool isMode(const std::string& s);
   bool isAllowedDelone(const std::string& s);
   bool isAllowedNiggli(const std::string& s);
   const std::string g_AllowedDelone = "CTROMAH";
   const std::string g_DeloneTypes = "hR hP oS oC mS mC mS A1 A2 A3 C1 C3 C5 H4 M1A M1B M2A M2B M3 M4 O1A O1B O2 O3 O4 O5 R1 R3 T1 T2 T5";
   const std::string g_LatticeTypes = "aP cF cI cP hP mC mP oF oI oP oS rP tI tP ";
   const std::string g_Complex = "X x";
   //static std::string TranslateDeloneToIT(const std::string& s) {

   std::map<std::string, std::string> m = {
   { "C1", "cI" },
   { "C3", "cF" },
   { "C5", "cP" },
   { "T1", "tI" },
   { "T2", "tI" },
   { "T5", "tP" },
   { "R1", "hR" },
   { "R3", "hR" },
   { "O1A", "oF" },
   { "O2", "oI" },
   { "O3", "oI" },
   { "O4", "oS" },
   { "O5", "oP" },
   { "O1B", "oI" },
   { "M1A", "mC" },
   { "M1B", "mC" },
   { "M2", "mC" },
   { "M3", "mC" },
   { "M4", "mP" },
   { "M2B", "mC" },
   { "H4", "hP" }
   };

   //std::string out{ "" };
   //if (m.find(s) != m.end()) out = m[s];
   //return out;


};


#endif  // CMDGENTYPES_H