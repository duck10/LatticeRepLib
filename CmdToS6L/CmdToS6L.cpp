
#include <algorithm>
#include <cmath>
#include <iostream>

#include "LRL_DataToSVG.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "S6.h"
#include "Selling.h"
#include <vector>

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
   for (size_t kkk=0;kkk<vs.size();++kkk)
   {
      S6 s6;
      for (size_t i = 0; i < 6; ++i) {
         s6[i] = (abs(vs[kkk][i]) < 1.0E-6) ? 0 : vs[kkk][i];
      }
      out.emplace_back(s6);
   }
   return out;
}

std::vector<S6> DoSqrt(const std::vector<S6>& vs) {
   std::vector<S6> out;
   for (size_t i = 0; i < vs.size(); ++i) {
      S6 s(vs[i]);
      S6 temp(sqrt(s[0]), sqrt(s[1]), sqrt(s[2]), sqrt(s[3]), sqrt(s[4]), sqrt(s[5]));
      out.emplace_back(temp);
   }
   return out;
}

std::vector<S6> KeepOnlyFirstMin(const std::vector<S6>& vs) {
   std::vector<S6> out;
   // since all the vectors have the same value, we only the minimum value in one
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
   // since all the vectors have the same value, we only the minimum value in one
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
   //std::cout << "ResortElements_1_3   vs" << std::endl;
   //OutputVector(vs);
   const std::vector<S6> sort1 = ResortFirstElement(vs);
   //std::cout << "ResortElements_1_3  sort1" << std::endl;
   //OutputVector(sort1);
   const std::vector<S6> sort2 = ResortSecondElement(sort1);
   //std::cout << " ResortElements_1_3 sort2" << std::endl;
   //OutputVector(sort2);
   const std::vector<S6> sort3 = ResortFirstElement(sort2);
   //std::cout << " ResortElements_1_3 sort3" << std::endl;
   //OutputVector(sort3);
   const std::vector<S6> sort4 = ResortFirstElement(sort3);
   //std::cout << "ResortElements_1_3 sort4" << std::endl;
   //OutputVector(sort4);
   return sort4;
}

S6 AllPositive(const S6& s) {
   S6 out(s);
   for (size_t i=0; i < 6; ++i) {
      out[i] = -out[i];
   }
   return out;
}

//std::vector<S6> MoveMinToFirst(const std::vector<S6>& vs) {
//   std::vector<S6> out;
//   const double dmin = minNC(vs[0][0], vs[0][1], vs[0][2], vs[0][3], vs[0][4], vs[0][5]);
//   for (size_t i = 0; i < vs.size(); ++i) {
//      const S6& s6 = vs[i];
//      const std::vector<S6> reflected = GenerateReflections(s6);
//      if ()
//   }
//   return out;
//}
//
//std::vector<S6> MoveSecontMinToSecond(const std::vector<S6>& vs) {
//   std::vector<S6> out;
//   const double dmin2 = minNC(vs[0][1], vs[0][2], vs[0][3], vs[0][4], vs[0][5]);
//   return out;
//}

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
   const std::string ss1 = LRL_DataToSVG("{", s1, ", ");
   const std::string ss2 = LRL_DataToSVG(s2, ", ");
   const std::string ss3 = LRL_DataToSVG(s3, ", ");
   const std::string ss4 = LRL_DataToSVG(s4, ", ");
   const std::string ss5 = LRL_DataToSVG(s5, ", ");
   const std::string ss6 = LRL_DataToSVG(s6, "} Root Invariant for V1");
   const std::string out(ss1 + ss2 + ss3 + ss4 + ss5 + ss6);
   std::cout << out << std::endl;
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
      const std::string ss2 = LRL_DataToSVG("{", s2, ", ");
      const std::string ss3 = LRL_DataToSVG("(", s3, ", ");
      const std::string ss4 = LRL_DataToSVG(s4, ", ");
      const std::string ss5 = LRL_DataToSVG(s5, "), ");
      const std::string ss6 = LRL_DataToSVG(s6, "} Root Invariant for V2");
      const std::string out(ss2 + ss3 + ss4 + ss5 + ss6);
      std::cout << out << std::endl;
      return out;
   }

std::string Format_V4(const S6& s) {
   //std::cout << "; Root Invariant for V2 " << LRL_ToString("{", s6[1], "(", s6[2], s6[3], s6[4], ")", , s6[5], "}") << std::endl;
   //const double& s1 = s[0];
   //const double& s2 = s[1];
   const double& s3 = s[2];
   const double& s4 = s[3];
   const double& s5 = s[4];
   const double& s6 = s[5];
   //; Root Invariant for V2 { 1.62541 ( 2.86617 2.24793 5.37153 ) 4.4525 }
   const std::string ss3 = LRL_DataToSVG("{(", s3, ", ");
   const std::string ss4 = LRL_DataToSVG(s4, ", ");
   const std::string ss5 = LRL_DataToSVG(s5, "), ");
   const std::string ss6 = LRL_DataToSVG(s6, "} Root Invariant for V4");
   const std::string out(ss3 + ss4 + ss5 + ss6);
   std::cout << out << std::endl;
   return out;
}

std::string Format_V3(const S6& s) {
   //std::cout << "; Root Invariant for V2 " << LRL_ToString("{", s6[1], "(", s6[2], s6[3], s6[4], ")", , s6[5], "}") << std::endl;
   //const double& s1 = s[0];
   const double& s2 = s[1];
   const double& s3 = s[2];
   const double& s5 = s[4];
   const double& s6 = s[5];
   //; Root Invariant for V2 { 1.62541 ( 2.86617 2.24793 5.37153 ) 4.4525 }
   const std::string ss3 = LRL_DataToSVG("{", s2, ", ");
   const std::string ss4 = LRL_DataToSVG(s3, ", ");
   const std::string ss5 = LRL_DataToSVG(s5, ", ");
   const std::string ss6 = LRL_DataToSVG(s6, "} Root Invariant for V3");
   const std::string out(ss3 + ss4 + ss5 + ss6);
   std::cout << out << std::endl;
   return out;
}

std::string Format_V5(const S6& s) {
   //std::cout << "; Root Invariant for V2 " << LRL_ToString("{", s6[1], "(", s6[2], s6[3], s6[4], ")", , s6[5], "}") << std::endl;
   //const double& s1 = s[0];
   //const double& s2 = s[1];
   const double& s4 = s[3];
   const double& s5 = s[4];
   const double& s6 = s[5];
   //; Root Invariant for V2 { 1.62541 ( 2.86617 2.24793 5.37153 ) 4.4525 }
   const std::string ss4 = LRL_DataToSVG("{", s4, ", ");
   const std::string ss5 = LRL_DataToSVG(s5, ", ");
   const std::string ss6 = LRL_DataToSVG(s6, "} Root Invariant for V5");
   const std::string out(ss4 + ss5 + ss6);
   std::cout << out << std::endl;
   return out;
}

void OutputRootInvariants(const S6& s6) {
   const int zeroCount = CountZeros(s6);
   if (zeroCount == 0) {
      // Root invaiant for V1
      //std::cout << "; Root Invariant for V1 " << LRL_ToString(s6) << std::endl;
      Format_V1(s6);
      }
   else if (zeroCount == 1) {
      // Root Invariant for V2
      //std::cout << "; Root Invariant for V2 " << LRL_ToString("{", s6[1], "(", s6[2], s6[3], s6[4], ")",, s6[5], "}") << std::endl;
      Format_V2(s6);
      }
   else if (zeroCount == 2 && s6[1] == 0.0) {
      //std::cout << "; Root Invariant for V4 " << LRL_ToString(s6[2],s6[3],s6[4],s6[5]) << std::endl;
      Format_V4(s6);
   }
   else if (zeroCount == 2) {
      //std::cout << "; Root Invariant for V3 " << LRL_ToString(s6[1], s6[2], s6[4], s6[5]) << std::endl;
      Format_V3(s6);
   }
   else if (zeroCount == 3) {
      //std::cout << "; Root Invariant for V5 " << LRL_ToString(s6[3], s6[4], s6[5]) << std::endl;
      Format_V5(s6);
   }
   else {
      throw; "not supposed to happen";
   }
}

int main()
{
   //std::vector<S6> sv(3);
   //sv[0] = S6::rand();
   //sv[1] = sv[0];
   //sv[2] = sv[0];
   //const std::vector<S6> out = EliminateDuplicates(sv);
   //OutputVector(out);
   //exit(0);


   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::cout << "; To S6Linear" << std::endl;

   for (size_t i = 0; i < inputList.size(); ++i) {
      const LRL_Cell cell = LatticeConverter::MakePrimitiveCell(inputList[i].GetLattice(), inputList[i].GetCell());
      S6 red;
      const bool b = Selling::Reduce(cell, red);
      const S6 s6(AllPositive(red));

      //// this is the calculation (at least my attempt) to compute Bright and Kurlin's "Root Invariant"

      // generate 24 reflections
      const std::vector<S6> allRefls = GenerateReflections(s6);       
      const std::vector<S6> sqrted = DoSqrt(allRefls);
      const std::vector<S6> resetZeros1 = ResetZeros(sqrted);
      // remove unsort examples
      //OutputVector(allRefls);
      const std::vector<S6> firstElementDmin = KeepDminFirst(resetZeros1);
      //std::cout << std::endl;
      //OutputVector(firstElementDmin);
      //std::cout << std::endl;
      const std::vector<S6> secondElement = KeepSecondMin(firstElementDmin);
      //std::cout << " cleaned " << std::endl;
      //OutputVector(cleaned);
      // set near zeros to zero
      const std::vector<S6> resetZeros2 = ResetZeros(secondElement);
      // remove actual near duplicates
      //const std::vector<S6> noDups = EliminateDuplicates(resetZeros);
      // remove from list those with 1st element not min
      const std::vector<S6> allSorted = ResortElements_1_3(resetZeros2);
      // remove from list those with 2nd element not 2nd min
      // if s1==s2, choose so s4,s5 are sorted
      // if s2==s3, choose so s5,s6 are sorted
      //std::cout << " all sorted " << std::endl;
      // OutputVector(allSorted);
      // remove actual near duplicates
      const std::vector<S6> noDups2 = EliminateDuplicates(allSorted);
      //std::cout << " noDups after sorting " << std::endl;
      //OutputVector(noDups2);
      //const std::vector<S6> sortedSecondElement = ResortSecondElement(noDups2);
      //const std::vector<S6> noDups3 = EliminateDuplicates(sortedSecondElement);
      // sqrt at some point !!!!!!
      const std::vector<S6> likeRI = DoSqrt(noDups2);
      //std::cout << " after DoSqrt" << std::endl;
      // OutputVector(likeRI);

      //for (size_t i = 0; i < likeRI.size(); ++i) {
      //   std::cout << "; SL " << likeRI[i] << std::endl;
      //}

      for (size_t i = 0; i < likeRI.size(); ++i) {
         OutputRootInvariants(likeRI[i]);
      }
   }
}
