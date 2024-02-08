#include "GetDate.h"

#include <ctime>
#include <iomanip>
#include <string>
#include <sstream>

#pragma warning(disable: 4996) // stupid visual studio

std::string GetDate() {
   std::time_t now = std::time(nullptr);
   std::tm const* localTime = std::localtime(&now);

   std::ostringstream os;
   os << std::put_time(localTime, "%Y-%m-%d");
   return os.str();
}
