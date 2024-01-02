

#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "DC7u.h"
#include "RI.h"
#include "S6.h"
//#include "S6L.h"
#include "V7.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include "utility"
#include <vector>

static double isValidScale(const std::string& str) {
   std::istringstream iss(str);
   double d;
   iss >> d >> std::ws;  // Extract value and ignore trailing whitespace
   return d; 
}

template<typename T>
static T Scale(const double scale, const T& t) {
   return scale * t / t.norm();
}

template<typename T>
static void Output(const double scaleFactor, const T& t, const std::string& label) {
   std::cout << T::GetName() + " " << Scale<T>(scaleFactor, t) << " " + label << std::endl;
}

static std::string GetArgs(const int argc, char* argv[]) {
   const std::vector<std::string> allowedTypes{ "S6","V7","U","DC7U","RI"/*,"S6L"*/ };

   std::string type = "S6";
   if (argc == 1) {
      // values of parameters already set
   }
   else {
      const std::string arg(argv[1]);
     if (std::find(allowedTypes.begin(), allowedTypes.end(), arg) != allowedTypes.end()) type = arg;
   }

   return type;
}

int main(int argc, char* argv[])
{
   const std::string type = GetArgs(argc, argv);

   std::cout << "; Scale Vectors" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   const double scaleFactor = (inputList.empty()) ? 0.0 : (S6(inputList[0].GetCell())).norm();

   for (size_t i = 0; i < inputList.size(); ++i) {
      const S6 s6(inputList[i].GetCell());
      const std::string label = (i == 0) ? "REFERENCE" : "scaled";
      if (type == "S6")
         Output<S6>(scaleFactor, s6, label);
      else if (type == "RI")
         Output<RI>(scaleFactor, s6, label);
      else if (type == "V7")
         Output<V7>(scaleFactor, s6, label);
      else if (type == "U" || type == "DC7U")
         Output<DC7u>(scaleFactor, s6, label);
      //else if (type == "S6L")S6L is not implemented as a class yet (2024-01-01)
      //            Output<S6L>(scaleFactor, s6, label);
      else 
         Output<S6>(scaleFactor, s6, label);
   }
}
