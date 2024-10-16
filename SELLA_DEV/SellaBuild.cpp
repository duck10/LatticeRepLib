#include "Delone.h"
#include "EnhancedS6.h"
#include "GenerateRandomLattice.h"
#include "LRL_ToString.h"
#include "MatS6.h"
#include "S6_Ordinals.h"
#include "S6Dist.h"
#include "SellaBuild.h"
#include "Selling.h"
#include "StoreResults.h"

std::vector< std::pair<std::string, MatS6> > SellaBuild::vDeloneTypes = Delone::LoadLabeledLatticeTypeProjectors();
static const std::vector<MatS6> g_refls = MatS6::GetReflections();
//static const std::vector< EnhancedS6(*)(const EnhancedS6&)> fnMakeVirtualCartPoint = S6Dist::SetVCPFunctions();
//static const std::vector<EnhancedS6(*)(const EnhancedS6&)> fnRefl = EnhancedS6::SetReflectionFunctions();

std::set<EnhancedS6> BoundaryAndAllReflections(const size_t n, const EnhancedS6& s) {
   static const VCPVector vcpVector;  // Create a static instance of VCPVector
   const EnhancedS6 vcpResult = vcpVector.applyVCP(n, s);
   const std::set<EnhancedS6> bAndR = SellaBuild::GenerateAllReflections(vcpResult);
   return bAndR;
}

std::set<EnhancedS6> BoundaryAndAllReflections(const size_t n, const std::set<EnhancedS6>& s) {
   std::set<EnhancedS6> out;
   for (const auto& it : s) {
      const std::set<EnhancedS6> temp = BoundaryAndAllReflections(n, it);
      out.insert(temp.begin(), temp.end());
   }
   return out;
}
SellaBuild::SellaBuild() : vcpVector() {
   store.SetMaxItemStore(20000);
}

std::vector<LabeledDeloneTypeMatrices> SellaBuild::Build() {
   MatS6 transformations;

   if (vDeloneTypes.empty())
      SellaBuild::vDeloneTypes = Delone::LoadLabeledLatticeTypeProjectors();

   for (size_t i = 0; i < vDeloneTypes.size(); ++i) {
      transformations = transformations.unit();
      Expand2(vDeloneTypes[i].first, vDeloneTypes[i].second);
      Expand(vDeloneTypes[i].first, vDeloneTypes[i].second/*, transformations*/);
   }

   store.ShowTableOfKeysVersusCount();
   ProcessItemStoreToVectorMap();

   std::cout << std::endl << std::endl << "after ProcessItemStoreToVectorMap, the map " << xfmap.size() << std::endl;
   for (const auto& ita : xfmap) {
      std::cout << ita.first << "  " << ita.second.size() << std::endl;
      // If you want to print S6 part of each EnhancedS6, you can add:
      // for (const auto& enhancedS6 : ita.second) {
      //     std::cout << "S6: " << enhancedS6.GetS6() << std::endl;
      // }
   }

   LabeledDeloneTypeMatrices lsm2;
   const std::vector<LabeledDeloneTypeMatrices> vtypes = lsm2.ProcessVectorMapToPerpsAndProjectors(xfmap);
   lsm2.WriteSellaMatrices(vtypes);
   return vtypes;
}
   static int seed = 19191;

   EnhancedS6 SellaBuild::MakeSampleType(const MatS6 & m) {
      GenerateRandomLattice<S6> generate(seed);
      EnhancedS6 s6;
      s6.SetValid(false);
      int count = 0;
      bool again = true;

      while (again && count < 10) {
         s6 = m * generate.randSellingReduced();
         again = !s6.GetValid();
         ++count;
      }
      return s6;
   }

   double SellaBuild::TestOneType(const std::string & label, const EnhancedS6 & s6, const std::vector<MatS6>&vm) {
      double best = DBL_MAX;
      size_t bestIndex;
      EnhancedS6 bestS6;
      const double s6norm = s6.norm();
      EnhancedS6 prp;
      for (size_t i = 0; i < vm.size(); ++i) {
         prp = vm[i] * s6;
         if (best > prp.norm()) {
            best = prp.norm();
            bestIndex = i;
            bestS6 = vm[i] * s6;
         }
      }

      indexstore.Store(LRL_ToString(label, " ", bestIndex), LRL_ToString(best, "     ", bestS6));
      return best;
   }

   void SellaBuild::TestAllTypes(const EnhancedS6 & s6) {
      for (const auto& p : perps) {
         const std::string label = p.GetLabel();
         const double best = TestOneType(label, s6, p.GetMatrices());
      }
   }

   //bool IsNotDup(const std::vector<MatS6>&v, const MatS6 & m) {
   //   for (const auto& mat : v) {
   //      if ((mat - m).norm() < 1.0E-8) return false;
   //   }
   //   return true;
   //}

   //std::vector<MatS6> RemoveForDuplicates(const std::vector<MatS6>&m) {
   //   std::vector<MatS6> v(1, m[0]);
   //   int count = 0;
   //   for (size_t i = 1; i < m.size(); ++i) {
   //      if (IsNotDup(v, m[i]))
   //         v.push_back(m[i]);
   //      else
   //         ++count;
   //   }
   //   return v;
   //}

   std::vector<EnhancedS6(*)(const EnhancedS6&)> SellaBuild::reflectionFunctions = {
      // Reflection 1
      [](const EnhancedS6& s) {
          const MatS6 reflMat("1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1");
          S6 reflected = reflMat * static_cast<const S6&>(s);
          return EnhancedS6(reflected, reflMat * s.getTransformMatrix());
      },
      // Reflection 2
      [](const EnhancedS6& s) {
          const MatS6 reflMat("1 0 0 0 0 0  0 0 1 0 0 0  0 1 0 0 0 0  0 0 0 1 0 0  0 0 0 0 0 1  0 0 0 0 1 0");
          S6 reflected = reflMat * static_cast<const S6&>(s);
          return EnhancedS6(reflected, reflMat * s.getTransformMatrix());
      },
      // Reflection 3
      [](const EnhancedS6& s) {
          const MatS6 reflMat("1 0 0 0 0 0  0 0 0 0 0 1  0 0 0 0 1 0  0 0 0 1 0 0  0 0 1 0 0 0  0 1 0 0 0 0");
          S6 reflected = reflMat * static_cast<const S6&>(s);
          return EnhancedS6(reflected, reflMat * s.getTransformMatrix());
      },
      // Reflection 4
      [](const EnhancedS6& s) {
          const MatS6 reflMat("1 0 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 1  0 0 0 1 0 0  0 1 0 0 0 0  0 0 1 0 0 0");
          S6 reflected = reflMat * static_cast<const S6&>(s);
          return EnhancedS6(reflected, reflMat * s.getTransformMatrix());
      },
      // Reflection 5
      [](const EnhancedS6& s) {
          const MatS6 reflMat("0 1 0 0 0 0  1 0 0 0 0 0  0 0 1 0 0 0  0 0 0 0 1 0  0 0 0 1 0 0  0 0 0 0 0 1");
          S6 reflected = reflMat * static_cast<const S6&>(s);
          return EnhancedS6(reflected, reflMat * s.getTransformMatrix());
      },
      // Reflection 6
      [](const EnhancedS6& s) {
          const MatS6 reflMat("0 0 0 0 0 1  1 0 0 0 0 0  0 0 0 0 1 0  0 0 1 0 0 0  0 0 0 1 0 0  0 1 0 0 0 0");
          S6 reflected = reflMat * static_cast<const S6&>(s);
          return EnhancedS6(reflected, reflMat * s.getTransformMatrix());
      },
      // ... Continue with the remaining 18 reflection functions ...
      // Reflection 24
      [](const EnhancedS6& s) {
          const MatS6 reflMat("0 1 0 0 0 0  0 0 0 0 0 1  0 0 0 1 0 0  0 0 0 0 1 0  0 0 1 0 0 0  1 0 0 0 0 0");
          S6 reflected = reflMat * static_cast<const S6&>(s);
          return EnhancedS6(reflected, reflMat * s.getTransformMatrix());
      }
   };

   void SellaBuild::StoreAllReflections(const std::string & label, const EnhancedS6 & s) {
      for (const auto& reflFunc : reflectionFunctions) {
         store.Store(label, reflFunc(s));
      }
   }

   void SellaBuild::StoreAllReflections(const std::string & label, const std::set<EnhancedS6>&s) {
      for (const auto& it : s) {
         StoreAllReflections(label, it);
      }
   }

   std::set<EnhancedS6> SellaBuild::GenerateAllReflections(const EnhancedS6 & s1in) {
      std::set<EnhancedS6> setS6;
      for (const auto& reflFunc : reflectionFunctions) {
         setS6.insert(reflFunc(s1in));
      }
      return setS6;
   }

   std::set<EnhancedS6> SellaBuild::GenerateAllReflections(const std::set<EnhancedS6>&s) {
      std::set<EnhancedS6> setS6;
      for (const auto& it : s) {
         std::set<EnhancedS6> reflections = GenerateAllReflections(it);
         setS6.insert(reflections.begin(), reflections.end());
      }
      return setS6;
   }
   std::vector<size_t> SellaBuild::DetermineToProcess(const std::set<size_t>&exclusions,
      const EnhancedS6 & zeros)
   {
      std::vector<size_t> vZeros = FindS6Zeros(zeros);
      for (const auto& it : exclusions)
      {
         const auto zeroIT = std::find(vZeros.begin(), vZeros.end(), it);
         if (zeroIT != vZeros.end()) vZeros.erase(zeroIT);
      }
      return vZeros;
   }

   EnhancedS6 SetPos(const size_t n) {
      EnhancedS6 zero(S6(0, 0, 0, 0, 0, 0));
      zero[n] += 0.0000000001;
      return zero;
   }

   EnhancedS6 SellaBuild::SetPos(const size_t n, const EnhancedS6 & s6) {
      EnhancedS6 out(s6);
      out[n] += 0.0000000001;
      return out;
   }

   std::set<EnhancedS6> SellaBuild::SetPos(const size_t n, const std::set<EnhancedS6>&s6) {
      std::set<EnhancedS6> out;
      int i19191 = 0;
      for (const auto& it : s6)
      {
         ++i19191;
         const EnhancedS6 temp = SetPos(n, it);
         out.insert(temp);
      }
      return out;
   }

   void SellaBuild::Expand(const std::string & label, const MatS6 & fromCanon) {
      const EnhancedS6 s6 = MakeSampleType(fromCanon);
      MatN mn(36);
      mn.SetVector(fromCanon.GetVector());
      const std::vector<size_t> vZeros = FindS6Zeros(s6);
      if (vZeros.empty()) {
         StoreAllReflections(label, s6);
      }
      else if (s6.IsValid()) {
         switch (s6.CountZeros()) {
         case 0:
            StoreAllReflections(label, s6);
            break;
         case 1:
            OneBound(label, s6, vZeros[0]);
            break;
         case 2:
            ProcessTwoZeros(label, s6);
            break;
         case 3:
            ProcessThreeZeros(label, s6);
            break;
         default:
            throw "this should never happen";
            break;
         }
      }
   }

   void SellaBuild::Expand2(const std::string & deloneType, const MatS6 & projectorForCanonDeloneType) {
      const EnhancedS6 s6 = MakeSampleType(projectorForCanonDeloneType);
      MatN projector(36);
      projector.SetVector(projectorForCanonDeloneType.GetVector());
      const std::set<EnhancedS6> out2 = GenerateAllReflections(s6);
      StoreAllReflections(deloneType, out2); // did not make any more get stored
      const std::set<EnhancedS6> out = Xpand_A(deloneType, out2, projectorForCanonDeloneType);
      StoreAllReflections(deloneType, out);
   }

   void SellaBuild::OneBound(const std::string & label, const EnhancedS6 & s1,
      const size_t zeroBound)
   {
      const EnhancedS6 s6temp = vcpVector.applyVCP(zeroBound, s1);
      StoreAllReflections(label, s6temp);
   }

   void SellaBuild::ProcessTwoZeros(const std::string & label, const EnhancedS6 & s6) {
      const std::vector<size_t> vZeros = FindS6Zeros(s6);

      // handle input as if no zeros, just reflections
      StoreAllReflections(label, s6);

      // Process each zero by itself
      OneBound(label, s6, vZeros[0]);
      OneBound(label, s6, vZeros[1]);

      const EnhancedS6 red1 = SetPos(vZeros[0], vcpVector.applyVCP(vZeros[0], s6));
      const EnhancedS6 red2 = SetPos(vZeros[1], vcpVector.applyVCP(vZeros[1], s6));

      const std::vector<size_t> vZeros1 = FindS6Zeros(red1);
      const std::vector<size_t> vZeros2 = FindS6Zeros(red2);

      const EnhancedS6 red12 = vcpVector.applyVCP(vZeros1[0], red1);
      const EnhancedS6 red21 = vcpVector.applyVCP(vZeros2[0], red2);

      StoreAllReflections(label, red1);
      StoreAllReflections(label, red2);
      StoreAllReflections(label, red12);
      StoreAllReflections(label, red21);
   }

   EnhancedS6 SetPos(const size_t n, const size_t m) {
      EnhancedS6 zero(S6(0, 0, 0, 0, 0, 0));
      zero[n] += 0.0000000001;
      zero[m] += 0.0000000001;
      return zero;
   }

   std::set<EnhancedS6> SellaBuild::BoundAndRefl(const size_t n, const EnhancedS6 & s) {
      const EnhancedS6 s6(s);
      std::set<EnhancedS6> out;
      EnhancedS6 rxxxx = vcpVector.applyVCP(n, s);

      for (const auto& reflFunc : reflectionFunctions) {
         out.insert(reflFunc(rxxxx));
      }

      return out;
   }
   void SellaBuild::ProcessThreeZeros(const std::string & label, const EnhancedS6 & s6) {
      const std::vector<size_t> vZeros = FindS6Zeros(s6);

      // handle input as if no zeros, just reflections
      StoreAllReflections(label, s6);

      // treat each separate bound
      const EnhancedS6 red_0 = vcpVector.applyVCP(vZeros[0], s6);
      const EnhancedS6 red_1 = vcpVector.applyVCP(vZeros[1], s6);
      const EnhancedS6 red_2 = vcpVector.applyVCP(vZeros[2], s6);

      StoreAllReflections(label, red_0);
      StoreAllReflections(label, red_1);
      StoreAllReflections(label, red_2);

      // treat each pair of zeros
      const EnhancedS6 red_1_0 = vcpVector.applyVCP(vZeros[1], red_0);
      const EnhancedS6 red_2_0 = vcpVector.applyVCP(vZeros[2], red_0);
      const EnhancedS6 red_0_1 = vcpVector.applyVCP(vZeros[0], red_1);
      const EnhancedS6 red_2_1 = vcpVector.applyVCP(vZeros[2], red_1);
      const EnhancedS6 red_0_2 = vcpVector.applyVCP(vZeros[0], red_2);
      const EnhancedS6 red_1_2 = vcpVector.applyVCP(vZeros[1], red_2);

      StoreAllReflections(label, red_1_0);
      StoreAllReflections(label, red_2_0);
      StoreAllReflections(label, red_0_1);

      StoreAllReflections(label, red_1_0);
      StoreAllReflections(label, red_2_0);
      StoreAllReflections(label, red_0_1);
      StoreAllReflections(label, red_2_1);
      StoreAllReflections(label, red_0_2);
      StoreAllReflections(label, red_1_2);

      // now process all three zeros
      const EnhancedS6 red_0_1_2 = vcpVector.applyVCP(vZeros[0], red_1_2);
      const EnhancedS6 red_0_2_1 = vcpVector.applyVCP(vZeros[0], red_2_1);
      const EnhancedS6 red_1_0_2 = vcpVector.applyVCP(vZeros[1], red_0_2);
      const EnhancedS6 red_1_2_0 = vcpVector.applyVCP(vZeros[1], red_2_0);
      const EnhancedS6 red_2_0_1 = vcpVector.applyVCP(vZeros[2], red_0_1);
      const EnhancedS6 red_2_1_0 = vcpVector.applyVCP(vZeros[2], red_1_0);

      StoreAllReflections(label, red_0_1_2);
      StoreAllReflections(label, red_0_2_1);
      StoreAllReflections(label, red_1_0_2);
      StoreAllReflections(label, red_1_2_0);
      StoreAllReflections(label, red_2_0_1);
      StoreAllReflections(label, red_2_1_0);
   }

   void SellaBuild::ProcessZeros(const std::string & label, const EnhancedS6 & s6) {
      const std::vector<size_t> vZeros = FindS6Zeros(s6);
      const size_t nzeros = vZeros.size();

      if (nzeros == 0) {
         return;
      }
      else if (nzeros == 1) {
         OneBound(label, s6, vZeros[0]);
      }
      else if (nzeros == 2) {
         ProcessTwoZeros(label, s6);
      }
      else {
         ProcessThreeZeros(label, s6);
      }
   }

   static std::vector<Transformations> ConvertEnhancedS6ToTransformed(const std::vector<EnhancedS6>&v) {
      std::vector<Transformations> out;
      for (const auto& i : v) out.emplace_back(Transformations(i));
      return out;
   }

   template<typename T>
   static void PrintMap(const std::map<std::string, std::vector<T> >&t) {
      for (const auto& it : t) {
         std::cout << it.first << "  " << it.second[0] << std::endl;
      }
   }

   void SellaBuild::ProcessItemStoreToVectorMap() {
      std::vector<std::string> labels = store.GetKeys();
      for (const auto& label : labels) {
         std::vector<EnhancedS6> out;
         const std::vector<std::pair<std::string, EnhancedS6>>& v = store.GetResult(label);

         for (const auto& pair : v) {
            const EnhancedS6& s6 = pair.second;
            /*if (out.empty() || !FindDuplicate(out, s6))*/ {  // made not difference to comment out
               // but more points after ProcesseItemStoreToVectorMap 
               out.push_back(s6);
            }
         }

         xfmap.insert(std::make_pair(label, out));
      }
   }

   bool SellaBuild::FindDuplicate(const std::vector<EnhancedS6>& out, const EnhancedS6& s6) {
      for (const auto& e : out) {
         if ((e.GetS6() - s6.GetS6()).norm() < 1.0E-6) return true;
      }
      return false;
   }

   std::vector<size_t> SellaBuild::FindS6Zeros(const EnhancedS6 & s) {
      std::vector<size_t> v;
      for (size_t i = 0; i < 6; ++i) if (s[i] == 0.0) v.push_back(i);
      return v;
   }

   std::vector<size_t> SellaBuild::FindNearS6Zeros(const EnhancedS6 & s) {
      std::vector<size_t> v;
      for (size_t i = 0; i < 6; ++i) if (abs(s[i]) < 1.0E-5) v.push_back(i);
      return v;
   }

   void SellaBuild::ShowIndexResults() const {
      indexstore.ShowResults();
   }

   static EnhancedS6 ResetZeros(const EnhancedS6 & sin) {
      EnhancedS6 sout(sin);
      for (size_t i = 0; i < 6; ++i) if (abs(sin[i]) < 1.0E-3) sout[i] = 0.0;
      return sout;
   }

   static EnhancedS6 Rounder(const EnhancedS6 & s6) {
      EnhancedS6 out(s6);
      for (size_t i = 0; i < 6; ++i) out[i] = round(out[i]);
      return out;
   }

   static std::set<EnhancedS6> Rounder(const std::set<EnhancedS6>&ss) {
      std::set<EnhancedS6> out;
      for (const auto& it : ss) {
         out.insert(Rounder(it));
      }
      return out;
   }

   std::set<EnhancedS6> SellaBuild::Xpand1(const std::string & label, const EnhancedS6 & s, const MatS6 & projectorForCanonDeloneType) {
      std::set<EnhancedS6> out = GenerateAllReflections(s);
      const std::vector<size_t> vZeros = FindNearS6Zeros(s);
      if (vZeros.empty()) return out;

      const std::set<EnhancedS6> out1 = GenerateAllReflections(Rounder(BoundaryAndAllReflections(vZeros[0], s)));
      out.insert(out1.begin(), out1.end());

      return out;
   }

   std::set<EnhancedS6> SellaBuild::Xpand1(const std::string & label, const std::set<EnhancedS6>&vsin, const MatS6 & toCanon) {
      std::set<EnhancedS6> out;
      for (const auto& it : vsin) {
         out = Xpand1(label, ResetZeros(it), toCanon);
      }
      return out;
   }

   std::set<EnhancedS6> SellaBuild::Xpand2(const std::string & label, const EnhancedS6 & s, const MatS6 & projectorForCanonDeloneType) {
      const std::vector<size_t> vZeros = FindS6Zeros(s);
      if (vZeros.size() < 2) {
         return Xpand1(label, s, projectorForCanonDeloneType);
      }
      const EnhancedS6 s6temp0 = Rounder(vcpVector.applyVCP(vZeros[0], s));
      const EnhancedS6 s6temp1 = Rounder(vcpVector.applyVCP(vZeros[1], s));

      const std::set<EnhancedS6> pluss6temp0 = Xpand1(label, SetPos(0, s6temp0), projectorForCanonDeloneType);
      const std::set<EnhancedS6> pluss6temp1 = Xpand1(label, SetPos(1, s6temp1), projectorForCanonDeloneType);

      const std::set<EnhancedS6> setRefl1 = GenerateAllReflections(s6temp0);
      const std::set<EnhancedS6> setRefl2 = GenerateAllReflections(s6temp1);
      const std::set<EnhancedS6> setRefl3 = GenerateAllReflections(pluss6temp0);
      const std::set<EnhancedS6> setRefl4 = GenerateAllReflections(pluss6temp1);

      std::set<EnhancedS6> out;
      out.insert(s6temp0);
      out.insert(s6temp1);
      out.insert(pluss6temp0.begin(), pluss6temp0.end());
      out.insert(pluss6temp1.begin(), pluss6temp1.end());
      out.insert(setRefl1.begin(), setRefl1.end());
      out.insert(setRefl2.begin(), setRefl2.end());
      out.insert(setRefl3.begin(), setRefl3.end());
      out.insert(setRefl4.begin(), setRefl4.end());

      return out;
   }

   std::set<EnhancedS6> SellaBuild::Xpand2(const std::string & label, const std::set<EnhancedS6>&vsin, const MatS6 & toCanon) {
      std::set<EnhancedS6> vs;
      for (const auto& it : vsin) {
         std::set<EnhancedS6> out = Xpand2(label, ResetZeros(it), toCanon);
         vs.insert(out.begin(), out.end());
      }
      return vs;
   }

   std::set<EnhancedS6> SellaBuild::Xpand_A(const std::string & label, const EnhancedS6 & s, const MatS6 & projectorForCanonDeloneType) {
      std::set<EnhancedS6> out;
      const std::vector<size_t> vZeros = FindS6Zeros(s);
      if (vZeros.size() < 3) {
         return Xpand2(label, s, projectorForCanonDeloneType);
      }

      const EnhancedS6 s6temp0 = Rounder(vcpVector.applyVCP(vZeros[0], s));
      const EnhancedS6 s6temp1 = Rounder(vcpVector.applyVCP(vZeros[1], s));
      const EnhancedS6 s6temp2 = Rounder(vcpVector.applyVCP(vZeros[2], s));

      const std::set<EnhancedS6> pluss6temp0 = Rounder(Xpand2(label, GenerateAllReflections(SetPos(0, s6temp0)), projectorForCanonDeloneType));
      const std::set<EnhancedS6> pluss6temp1 = Rounder(Xpand2(label, GenerateAllReflections(SetPos(1, s6temp1)), projectorForCanonDeloneType));
      const std::set<EnhancedS6> pluss6temp2 = Rounder(Xpand2(label, GenerateAllReflections(SetPos(2, s6temp2)), projectorForCanonDeloneType));

      const std::set<EnhancedS6> setRefl1 = Rounder(GenerateAllReflections(SetPos(0, s6temp0)));
      const std::set<EnhancedS6> setRefl2 = Rounder(GenerateAllReflections(SetPos(1, s6temp1)));
      const std::set<EnhancedS6> setRefl3 = Rounder(GenerateAllReflections(SetPos(2, s6temp2)));
      const std::set<EnhancedS6> setRefl4 = Rounder(GenerateAllReflections(SetPos(0, pluss6temp0)));
      const std::set<EnhancedS6> setRefl5 = Rounder(GenerateAllReflections(SetPos(1, pluss6temp1)));
      const std::set<EnhancedS6> setRefl6 = Rounder(GenerateAllReflections(SetPos(2, pluss6temp2)));

      out.insert(s6temp0);
      out.insert(s6temp1);
      out.insert(s6temp2);

      out.insert(pluss6temp0.begin(), pluss6temp0.end());
      out.insert(pluss6temp1.begin(), pluss6temp1.end());
      out.insert(pluss6temp2.begin(), pluss6temp2.end());

      out.insert(setRefl1.begin(), setRefl1.end());
      out.insert(setRefl2.begin(), setRefl2.end());
      out.insert(setRefl3.begin(), setRefl3.end());
      out.insert(setRefl4.begin(), setRefl4.end());
      out.insert(setRefl5.begin(), setRefl5.end());
      out.insert(setRefl6.begin(), setRefl6.end());

      return out;
   }

   std::set<EnhancedS6> SellaBuild::Xpand_A(const std::string & label, const std::set<EnhancedS6>&vsin, const MatS6 & toCanon) {
      std::set<EnhancedS6> out;
      for (const auto& it : vsin) {
         const std::set<EnhancedS6> vs = Rounder(Xpand_A(label, ResetZeros(it), toCanon));
         out.insert(vs.begin(), vs.end());
      }
      return out;
   }
