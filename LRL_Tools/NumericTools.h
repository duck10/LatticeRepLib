#include <limits>
#include <string>
#include <sstream>

template<typename T>
static bool isNumber(const std::string& str) {
   std::stringstream ss(str);
   T num;
   if (ss >> num) {
      return ss.eof();
   }
   return false;
}

template<typename T>
static T getNumber(const std::string& str) {
   std::stringstream ss(str);
   T num = 19191;
   if (ss >> num) {
      return num;
   }
   return std::numeric_limits<T>::max();
}
