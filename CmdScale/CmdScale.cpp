

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
static void Output(const double scaleFactor, const T& t) {
   std::cout << T::GetName() + " " << Scale<T>(scaleFactor, t) << std::endl;
}

static std::pair<double, std::string> GetArgs(const int argc, char* argv[]) {
   const std::vector<std::string> allowedTypes{ "S6","V7","U","DC7U","RI","S6L" };

   double scaleFactor = 1000;
   std::string type = "S6";
   if (argc == 1) {
      // values of parameters already set
   }
   else if (argc == 2) {
      const std::string arg(argv[1]);
      if (isValidScale(arg) > 0.0) scaleFactor = atof(argv[1]);
      else type = LRL_StringTools::strToupper(argv[1]);
   }
   else if (argc == 3) {
      const std::string arg1(argv[1]);
      const std::string arg2(argv[2]);

      if (isValidScale(arg1) > 0.0) {
         scaleFactor = atof(argv[1]);
         const std::string testType = LRL_StringTools::strToupper(arg2);
         if (std::find(allowedTypes.begin(), allowedTypes.end(), testType) != allowedTypes.end()) type = testType;
      }
      else if (isValidScale(arg2) > 0.0) {
         scaleFactor = atof(argv[2]);
         const std::string testType = LRL_StringTools::strToupper(arg1);
         if (std::find(allowedTypes.begin(), allowedTypes.end(), testType) != allowedTypes.end()) type = testType;
      }
      else {
         // accept defaults
      }
   }
   return std::make_pair(scaleFactor, type);
}

int main(int argc, char* argv[])
{
   const std::pair<double, std::string> args = GetArgs(argc, argv);
   const std::string& type = args.second;
   const double& scaleFactor = args.first;

   std::cout << "; Scale Vectors" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   for (size_t i = 0; i < inputList.size(); ++i) {
      const S6 s6(inputList[i].GetCell());

      if (type == "S6")
         Output<S6>(scaleFactor, s6);
      else if (type == "RI")
         Output<RI>(scaleFactor, s6);
      else if (type == "V7")
         Output<V7>(scaleFactor, s6);
      else if (type == "U" || type == "DC7U")
         Output<DC7u>(scaleFactor, s6);
      //else if (type == "S6L")S6L is not implemented as a class yet (2024-01-01)
      //            Output<S6L>(scaleFactor, s6);
      else 
         Output<S6>(scaleFactor, s6);
   }
}
