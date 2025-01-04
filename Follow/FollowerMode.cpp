// FollowerMode.cpp
#include "FollowerMode.h"

std::string toString(FollowerMode mode) {
   switch (mode) {
   case FollowerMode::POINT: return "POINT";
   case FollowerMode::LINE: return "LINE";
   case FollowerMode::CHORD: return "CHORD";
   case FollowerMode::CHORD3: return "CHORD3";
   case FollowerMode::TRIANGLE: return "TRIANGLE";
   }
   throw std::runtime_error("Unknown FollowerMode value");
}

FollowerMode toFollowerMode(const std::string& s) {
   if (s == "POINT") return FollowerMode::POINT;
   if (s == "LINE") return FollowerMode::LINE;
   if (s == "CHORD") return FollowerMode::CHORD;
   if (s == "CHORD3") return FollowerMode::CHORD3;
   if (s == "TRIANGLE") return FollowerMode::TRIANGLE;
   throw std::runtime_error("Invalid FollowerMode string: " + s);
}

std::ostream& operator<<(std::ostream& os, const FollowerMode& mode) {
   os << toString(mode);
   return os;
}
