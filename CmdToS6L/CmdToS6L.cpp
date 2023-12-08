
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "LRL_DataToSVG.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "S6.h"
#include "Selling.h"

static const std::vector<MatS6> refl_one = MatS6::GetReflections();

void OutputVector(const std::vector<S6>& vs) {
   for (size_t i = 0; i < vs.size(); ++i) {
      std::cout << vs[i] << std::endl;
   }
}

std::vector<S6> GenerateReflections(const S6& s6) {
   std::vector<S6> out;
   for (size_t i = -0; i < refl_one.size(); ++i) {
      out.emplace_back(refl_one[i] * s6);
   }
   return out;
}

std::vector<S6> EliminateDuplicates(const std::vector<S6>& vs) {
   std::vector<S6> out(1, vs[0]);
   for (size_t i = 1; i < vs.size(); ++i) {
      std::vector<S6> toAdd(1, vs[i]);
      for (size_t kkk = 0; kkk < out.size(); ++kkk)
      {
         const double d = (vs[i] - out[kkk]).norm();
         if (d < 1.0E-6) {
            toAdd.clear();
            break;
         }
      }
      out.insert(out.end(), toAdd.begin(), toAdd.end());
   }
   return out;
}

std::vector<S6> ResetZeros(const std::vector<S6>& vs) {
   std::vector<S6> out;
   for (size_t kkk = 0; kkk < vs.size(); ++kkk)
   {
      S6 s6;
      for (size_t i = 0; i < 6; ++i) {
         s6[i] = (abs(vs[kkk][i]) < 1.0E-6) ? 0 : vs[kkk][i];
      }
      out.emplace_back(s6);
   }
   return out;
}

static S6 ResetZeros(const S6& vs) {
   S6 s6;
   for (size_t i = 0; i < 6; ++i) {
      s6[i] = (abs(vs[i]) < 1.0E-6) ? 0 : vs[i];
   }
   return s6;
}

std::vector<S6> DoSqrt(const std::vector<S6>& vs) {
   std::vector<S6> out;
   for (size_t i = 0; i < vs.size(); ++i) {
      const S6& s(vs[i]);
      S6 temp(sqrt(s[0]), sqrt(s[1]), sqrt(s[2]), sqrt(s[3]), sqrt(s[4]), sqrt(s[5]));
      out.emplace_back(temp);
   }
   return out;
}

std::vector<S6> KeepOnlyFirstMin(const std::vector<S6>& vs) {
   std::vector<S6> out;
   // since all the vectors have the same set of values, we only the minimum value in one
   const std::vector<double> vd = vs[0].GetVector();
   const double dmin = minNC(vd[0], vd[1], vd[2], vd[3], vd[4], vd[5]);
   for (size_t i = 0; i < vs.size(); ++i)
   {
      const S6& s = vs[i];
      if (abs(s[0] - dmin) < 1.0E-6) out.emplace_back(s);
   }
   return out;
}

std::vector<S6> KeepOnlySecondMin(const std::vector<S6>& vs) {
   std::vector<S6> out;
   // since all the vectors have the same set of values, we only the minimum value in one
   const std::vector<double> vd = vs[0].GetVector();
   const double dmin = minNC(vd[1], vd[2], vd[3], vd[4], vd[5]);
   for (size_t i = 0; i < vs.size(); ++i)
   {
      const S6& s = vs[i];
      if (abs(s[1] - dmin) < 1.0E-6) out.emplace_back(s);
   }
   return out;
}

std::vector<S6> ResortFirstElement(const std::vector<S6>& vs) {
   std::vector<S6> out;
   for (size_t i = 0; i < vs.size(); ++i)
   {
      S6 s = vs[i];
      const double d = s[0] - s[1];
      if (d < 1.0E-6) {
         const double smin = std::min(s[3], s[4]);
         const double smax = std::max(s[3], s[4]);
         s[3] = smin;
         s[4] = smax;
      }
      out.emplace_back(s);
   }
   return out;
}

std::vector<S6> KeepDminFirst(const std::vector<S6>& vs) {
   std::vector<S6> out;
   const std::vector<double> vd = vs[0].GetVector();
   const double dmin = minNC(vd[0], vd[1], vd[2], vd[3], vd[4], vd[5]);

   for (size_t i = 0; i < vs.size(); ++i)
   {
      const S6 s = vs[i];
      const double d = s[0] - dmin;
      if (d < 1.0E-6) {
         out.emplace_back(s);
      }
   }
   return out;
}

std::vector<S6> KeepSecondMin(const std::vector<S6>& vs) {
   std::vector<S6> out;
   const std::vector<double> vd = vs[0].GetVector();
   const double dmin = minNC(vd[1], vd[2], vd[4], vd[5]);

   for (size_t i = 0; i < vs.size(); ++i)
   {
      const S6 s = vs[i];
      const double d = s[1] - dmin;
      if (d < 1.0E-6) {
         out.emplace_back(s);
      }
   }
   return out;
}

std::vector<S6> ResortSecondElement(const std::vector<S6>& vs) {
   std::vector<S6> out;
   for (size_t i = 0; i < vs.size(); ++i)
   {
      S6 s = vs[i];
      const double d = s[2] - s[3];
      if (d < 1.0E-6) {
         const double smin = std::min(s[4], s[5]);
         const double smax = std::max(s[4], s[5]);
         s[4] = smin;
         s[5] = smax;
      }
      out.emplace_back(s);
   }
   return out;
}

std::vector<S6> ResortElements_1_3(const std::vector<S6>& vs) {
   const std::vector<S6> sort1 = ResortFirstElement(vs);
   const std::vector<S6> sort2 = ResortSecondElement(sort1);
   const std::vector<S6> sort3 = ResortFirstElement(sort2);
   const std::vector<S6> sort4 = ResortFirstElement(sort3);
   return sort4;
}

S6 AllPositive(const S6& s) {
   S6 out(s);
   for (size_t i=0; i < 6; ++i) {
      out[i] = -out[i];
   }
   return out;
}

int CountZeros(const S6& s6) {
   int count = 0;
   for (size_t i = 0; i < 6; ++i)
      if (s6[i] == 0.0) ++count;
   return count;
}

std::string Format_V1(const S6& s) {
   //std::cout << "; Root Invariant for V2 " << LRL_ToString("{", s6[1], "(", s6[2], s6[3], s6[4], ")", , s6[5], "}") << std::endl;
   const double& s1 = s[0];
   const double& s2 = s[1];
   const double& s3 = s[2];
   const double& s4 = s[3];
   const double& s5 = s[4];
   const double& s6 = s[5];
   //; Root Invariant for V2 { 1.62541 ( 2.86617 2.24793 5.37153 ) 4.4525 }
   const std::string descr = " Root Invariant for V1 ";
   const std::string ss1 = LRL_DataToSVG("{", s1, ", ");
   const std::string ss2 = LRL_DataToSVG(s2, ", ");
   const std::string ss3 = LRL_DataToSVG(s3, ", ");
   const std::string ss4 = LRL_DataToSVG(s4, ", ");
   const std::string ss5 = LRL_DataToSVG(s5, ", ");
   const std::string ss6 = LRL_DataToSVG(s6, "}");
   const std::string out(descr + ss1 + ss2 + ss3 + ss4 + ss5 + ss6);
   return out;
}

   std::string Format_V2(const S6 & s) {
      //std::cout << "; Root Invariant for V2 " << LRL_ToString("{", s6[1], "(", s6[2], s6[3], s6[4], ")", , s6[5], "}") << std::endl;
      //const double& s1 = s[0];
      const double& s2 = s[1];
      const double& s3 = s[2];
      const double& s4 = s[3];
      const double& s5 = s[4];
      const double& s6 = s[5];
      //; Root Invariant for V2 { 1.62541 ( 2.86617 2.24793 5.37153 ) 4.4525 }
      const std::string descr = " Root Invariant for V2 ";
      const std::string ss2 = LRL_DataToSVG("(", s2, ", ");
      const std::string ss3 = LRL_DataToSVG(s3, ", ");
      const std::string ss4 = LRL_DataToSVG("{", s4, ", ");
      const std::string ss5 = LRL_DataToSVG(s5, "), ");
      const std::string ss6 = LRL_DataToSVG(s6, "}");
      const std::string out(descr + ss4 + ss2 + ss3 + ss5 + ss6);
      return out;
   }

std::string Format_V4(const S6& s) {
   //std::cout << "; Root Invariant for V2 " << LRL_ToString("{", s6[1], "(", s6[2], s6[3], s6[4], ")", , s6[5], "}") << std::endl;
   const double& s1 = s[0];
   const double& s2 = s[1];
   const double& s3 = s[2];
   const double& s4 = s[3];
   const double& s5 = s[4];
   const double& s6 = s[5];
   std::vector<double> vd{ s3, s4, s5 };
   std::sort(vd.begin(), vd.end());
   S6 out;
   out[0] = s1;
   out[1] = s2;
   out[2] = vd[0];
   out[3] = vd[1];
   out[4] = vd[2];
   out[5] = s6;
   //; Root Invariant for V4
   const std::string descr = " Root Invariant for V4 ";
   const std::string ss3 = LRL_DataToSVG("{(", out[2], ", ");
   const std::string ss4 = LRL_DataToSVG(out[3], ", ");
   const std::string ss5 = LRL_DataToSVG(out[4], "), ");
   const std::string ss6 = LRL_DataToSVG(s6, "}");
   const std::string strout(descr + ss3 + ss4 + ss5 + ss6);
   return strout;
}

S6 Resort_V1(const S6& s6) {

   S6 out(s6);
   if (abs(out[0] - out[3]) < 1.0E-5 && out[3] > out[4])
   {
      std::swap(out[3], out[4]);

   }
   else if (abs(out[1] - out[4]) < 1.0E-5 && out[4] > out[5])
   {
      std::swap(out[4], out[5]);
   }
   return out;
}

S6 Resort_V2(const S6& s6) {

   S6 out;
   //std::cout << " s6 entering Resort_V2  " << s6 << std::endl;
   std::vector<double> vd{ s6[1], s6[2], s6[4] };
   std::sort(vd.begin(), vd.end());
   //std::cout << "vd in Resort_V2 " << LRL_ToString(vd ) << std::endl;
   out[0] = s6[0];
   out[1] = vd[0]; //this is wrong and incomplete !!!!!!!!!!!!!!!!!!!!!!
   out[2] = vd[1];
   out[3] = s6[3];
   out[4] = vd[2];
   out[5] = s6[5];
   //std::cout << " final out in Resort_V2 " << out << std::endl << std::endl;
   return out;
}

S6 Resort_V3(const S6& s6) {

   S6 out(s6);
   std::vector<double> vd{ out[1], out[2], out[4], out[5] };
   std::sort(vd.begin(), vd.end());
   out[1] = vd[0];
   out[2] = vd[1];
   out[4] = vd[2];
   out[5] = vd[3];
   return out;
}

std::string Format_V3(const S6& s) {
   //const double& s1 = s[0];
   const double& s2 = s[1];
   const double& s3 = s[2];
   const double& s5 = s[4];
   const double& s6 = s[5];
   //; Root Invariant for V3
   const std::string descr = " Root Invariant for V3 ";
   const std::string ss3 = LRL_DataToSVG("{", s2, ", ");
   const std::string ss4 = LRL_DataToSVG(s3, ", ");
   const std::string ss5 = LRL_DataToSVG(s5, ", ");
   const std::string ss6 = LRL_DataToSVG(s6, "}");
   const std::string out(descr + ss3 + ss4 + ss5 + ss6);
   return out;
}

std::string Format_V5(const S6& s) {
   //const double& s1 = s[0];
   //const double& s2 = s[1];
   const double& s4 = s[3];
   const double& s5 = s[4];
   const double& s6 = s[5];
   //; Root Invariant for V2 { 1.62541 ( 2.86617 2.24793 5.37153 ) 4.4525 }
   const std::string descr = " Root Invariant for V5 ";
   const std::string ss4 = LRL_DataToSVG("{", s4, ", ");
   const std::string ss5 = LRL_DataToSVG(s5, ", ");
   const std::string ss6 = LRL_DataToSVG(s6, "}");
   const std::string out(descr + ss4 + ss5 + ss6);
   return out;
}

std::string  OutputRootInvariants(const S6& s6) {
   const int zeroCount = CountZeros(s6);
   std::string out;
   if (zeroCount == 0) {
      out = Format_V1(s6);
      }
   else if (zeroCount == 1) {
      out = Format_V2(Resort_V2(s6));
      }
   else if (zeroCount == 2 && s6[1] == 0.0) {
      out = Format_V4(s6);
   }
   else if (zeroCount == 2) {
      out = Format_V3(s6);
   }
   else if (zeroCount == 3) {
      out = Format_V5(s6);
   }
   else {
      throw; "not supposed to happen";
   }
   //std::cout << out << std::endl;
   return out;
}


std::pair<S6, std::string>   MakeRI(const LRL_ReadLatticeData& input, const S6& positiveRed) {
   //// this is the calculation (at least my attempt) to compute Bright and Kurlin's "Root Invariant"

   // generate 24 reflections
   const std::vector<S6> allRefls = GenerateReflections(positiveRed);
   const std::vector<S6> sqrted = DoSqrt(allRefls);
   const std::vector<S6> resetZeros1 = ResetZeros(sqrted);
   // remove unsort examples
   const std::vector<S6> firstElementDmin = KeepDminFirst(resetZeros1);
   const std::vector<S6> secondElement = KeepSecondMin(firstElementDmin);
   // set near zeros to zero (probably not needed)
   const std::vector<S6> resetZeros2 = ResetZeros(secondElement);
   // remove actual near duplicates
   const std::vector<S6> allSorted = ResortElements_1_3(resetZeros2);
   // remove actual near duplicates, because sorting values can generate them
   const std::vector<S6> noDups2 = EliminateDuplicates(allSorted);
   std::vector<S6> likeRI(noDups2);

   if (likeRI.size() > 1 && likeRI[0][0] == 0.0 && likeRI[0][3] == 0) {
      const S6 s6 = Resort_V3(likeRI[0]);
      likeRI.clear();
      likeRI.emplace_back(s6);
   }
   else if (likeRI.size() > 1 && CountZeros(likeRI[0]) == 0) {
      const S6 s6 = Resort_V1(likeRI[0]);
      likeRI.clear();
      likeRI.emplace_back(s6);
   }
   else if (likeRI.size() > 1 && likeRI[0][0] == 0.0 && CountZeros(likeRI[0]) == 1) {
      const S6 s6 = Resort_V2(likeRI[0]);
      likeRI.clear();
      likeRI.emplace_back(s6);
   }

   if (likeRI.size() > 1 || likeRI.empty()) {
      std::cout << input.GetStrCell() << std::endl;
      OutputVector(likeRI);
      throw; "bad likeRI";
   }
   std::vector<S6> summary;
   //summary.emplace_back(likeRI[0]);

   for (size_t i = 0; i < likeRI.size(); ++i) {
      const std::string RI = OutputRootInvariants(likeRI[i]);
      //const std::string line = "; SL " + LRL_ToString(likeRI[i], RI);
   }
   const std::pair<S6, std::string> pss = std::make_pair(likeRI[0], OutputRootInvariants(likeRI[0]));

   return pss;
}

S6  MakeS6L(const S6& s6) {
   const S6 s(sqrt(s6[0]), sqrt(s6[1]), sqrt(s6[2]), sqrt(s6[3]), sqrt(s6[4]), sqrt(s6[5]));
   return s;
}

int main(int argc, char* argv[]) {
   static std::string name = ""; // blank or unrecognized gives all types

   std::cout << "; To Linearized S6 and root invariants" << std::endl;

   static const std::string nameRootFragment("root");
   static const std::string nameRootSLFragment("linear");
   if (argc > 1) {
      const std::string arg(argv[1]);
      if (arg.find(nameRootFragment) != std::string::npos) name = "R";
      if (arg.find(nameRootSLFragment) != std::string::npos) name = "S";
      if (arg.find("R") != std::string::npos) name = "R";
      if (arg.find("S") != std::string::npos) name = "S";
   }


   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   std::vector<S6> summary;

   std::vector<std::pair<S6,std::string> >RIs;
   std::vector<S6> SLs;
   for (size_t i = 0; i < inputList.size(); ++i) {
      const LRL_Cell cell = LatticeConverter::MakePrimitiveCell(inputList[i].GetLattice(), inputList[i].GetCell());
      S6 red;
      const bool b = Selling::Reduce(cell, red);
      const S6 s6(AllPositive(red));

      // this is the calculation (at least my attempt) to compute Bright and Kurlin's "Root Invariant"
      RIs.emplace_back(MakeRI(inputList[i], s6));
      SLs.emplace_back(MakeS6L(s6));
   }

   if ( name.empty() || name[0]=='R')
   {
      for (size_t i = 0; i < RIs.size(); ++i) {
         std::cout << "RI " << ResetZeros(RIs[i].first) << RIs[i].second << std::endl;
      }
   }

   std::cout << std::endl;

   if ( name.empty() || name[0] =='S')
   {
      for (size_t i = 0; i < SLs.size(); ++i) {
         std::cout << "SL " << ResetZeros(SLs[i]) << std::endl;
      }
   }
}
