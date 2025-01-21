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
   static bool isMode(const std::string& s);
   static bool isAllowedDelone(const std::string& s);
   static bool isAllowedNiggli(const std::string& s);
   static inline const std::string g_AllowedDelone = "CTROMAH";
   static inline const std::string g_DeloneTypes = "hR hP oS oC mS mC mS A1 A2 A3 C1 C3 C5 H4 M1A M1B M2A M2B M3 M4 O1A O1B O2 O3 O4 O5 R1 R3 T1 T2 T5";
   static inline const std::string g_LatticeTypes = "aP cF cI cP hP mC mP oF oI oP oS rP tI tP ";
   static inline const std::string g_Complex = "X x";
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


//static std::string TranslateDeloneToIT(const std::string& s) {
//
//   std::map<std::string, std::string> m;
//   m.insert(std::make_pair("C1", "cI"));
//   m.insert(std::make_pair("C3", "cF"));
//   m.insert(std::make_pair("C5", "cP"));
//   m.insert(std::make_pair("T1", "tI"));
//   m.insert(std::make_pair("T2", "tI"));
//   m.insert(std::make_pair("T5", "tP"));
//   m.insert(std::make_pair("R1", "hR"));
//   m.insert(std::make_pair("R3", "hR"));
//   m.insert(std::make_pair("O1A", "oF"));
//   m.insert(std::make_pair("O2", "oI"));
//   m.insert(std::make_pair("O3", "oI"));
//   m.insert(std::make_pair("O4", "oS"));
//   //m.insert(std::make_pair("O4B", "oS"));
//   m.insert(std::make_pair("O5", "oP"));
//   m.insert(std::make_pair("O1B", "oI"));
//   m.insert(std::make_pair("M1A", "mC"));
//   m.insert(std::make_pair("M1B", "mC"));
//   m.insert(std::make_pair("M2", "mC"));
//   m.insert(std::make_pair("M3", "mC"));
//   m.insert(std::make_pair("M4", "mP"));
//   m.insert(std::make_pair("M2B", "mC"));
//   m.insert(std::make_pair("H4", "hP"));   std::string out{ "" };
//   if (m.find(s) != m.end()) out = m[s];
//   return out;
//}

};


#endif  // CMDGENTYPES_H