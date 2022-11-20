
#include <iostream>
#include <list>
#include <string>

#include "C3.h"
#include "LRL_ReadLatticeData.h"

std::vector<S6> FindNearestReflection(const std::vector<S6>& var);

S6 FindNearestReflection(const S6& ref, const S6& var) {
   static const std::vector<MatS6> refl_one = MatS6::GetReflections();
   double distMin = DBL_MAX;
   S6 s6min;

   for (size_t i = 0; i < refl_one.size(); ++i) {
      double d = (ref - refl_one[i] * var).norm();
      if (d < distMin) {
         distMin = std::min(distMin, d);
         s6min = refl_one[i] * var;
      }
   }
   return s6min;
}

std::vector<S6> FindNearestReflection(const std::vector<S6>& var) {
   if (var.empty()) return std::vector<S6>();
   std::vector<S6> out(1, var[0]);
   for (size_t i = 1; i < var.size(); ++i) {
      out.push_back(FindNearestReflection(out[i - 1], var[i]));
   }
   return out;
}


std::list<std::string> allowed{ "seq","C3","RI" };

std::string sorter;

int main(int argc, char* argv[])
{
   if (argc != 2 || std::find(allowed.begin(), allowed.end(), argv[1]) == allowed.end()) {
      std::cout << " input param must be one of 'seq', 'C3', 'RI'\n\n";
      exit(0);
   }
   else {
      sorter = argv[1];
   }

   LRL_ReadLatticeData reader;
   std::cout << "; Various sortings" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = reader.ReadLatticeData();





   if (sorter == "seq") {
      std::vector<S6> v;
      for (size_t i = 0; i < inputList.size(); ++i) {
         v.push_back(inputList[i].GetCell());
      }
      const std::vector<S6> vv = FindNearestReflection(v);
      for (size_t i = 0; i < inputList.size(); ++i) {
         std::cout << "S6 " << vv[i] << std::endl;;
      }

   }
   else if (sorter == "C3" ) {
      // sort, convert to fundamental unit, for C3; return S6
      for (size_t i = 0; i < inputList.size(); ++i) {
         std::cout << "S6 " << S6(C3::ConvertToFundamentalUnit(inputList[i].GetCell())) << std::endl;;
      }
   }
   else if (sorter == "RI") {
      std::cout << "not implemented yet" << std::endl;
   }
   else {
      std::cout << "something went wrong in cmdSort" << std::endl;
      exit(1);
   }

   std::cout << "; " + reader.GetIncomingSemicolons() << std::endl;


}
