#include "SellaBuild.h"

#include "EnhancedS6.h"
#include "GenerateRandomLattice.h"
#include "LabeledDeloneTypeMatrices.h"
#include "LRL_ToString.h"
#include "LRL_MaximaTools.h"
//#include "S6_Ordinals.h"
#include "S6Dist.h"
#include "Selling.h"
#include "StoreResults.h"

std::vector< std::pair<std::string, MatS6> > SellaBuild::vDeloneTypes = Delone::LoadLabeledLatticeTypeProjectors();

template<typename T> 
T ResetNearZero(const T& t) {
   T out;
   for (size_t i = 0; i < t.size(); ++i) {
      out[i] = (abs(t[i]) < 1.0E-10) ? 0.0 : t[i];
   }
   return out;
}

SellaBuild::SellaBuild() {
   //store.SetMaxItemStore(350);
   storeEN.SetMaxItemStore(350);
}

void SellaBuild::Build() {
   MatS6 transformations;
   for (unsigned long i = 0; i < vDeloneTypes.size(); ++i) {
      //transformations = transformations.unit();
      //for (unsigned long i = 2; i < 3; ++i) {  // to show only C5
      Expand(vDeloneTypes[i].first, vDeloneTypes[i].second);
   }
   //store.ShowTableOfKeysVersusCount();
   storeEN.ShowTableOfKeysVersusCount();
   ProcessItemStoreToVectorMap();
   //std::cout << std::endl << std::endl << "after ProcessItemStoreToVectorMap, the map " << themap.size() << std::endl;
   //std::cout << "from 'themap'" << std::endl;
   //for (auto ita = themap.begin(); ita != themap.end(); ++ita) std::cout << (*ita).first << "  " << (*ita).second.size() << std::endl;
   //std::cout << "from 'xfmap'" << std::endl;
   //for (auto ita = xfmap.begin(); ita != xfmap.end(); ++ita) std::cout << (*ita).first << "  " << (*ita).second.size() << std::endl;




   //LabeledDeloneTypeMatrices lsm2;
   LabeledDeloneTypeMatrices lsm2EN;
   //const std::vector<LabeledDeloneTypeMatrices> vtypes = lsm2.ProcessVectorMapToPerpsAndProjectors(themap);
   const std::vector<LabeledDeloneTypeMatrices> vtypesEN = lsm2EN.ProcessVectorMapToPerpsAndProjectors(xfmap);
   //std::cout << "from lsm2, not lsm2EN" << std::endl;
   //lsm2.WriteSellaMatrices(vtypes);
   //std::cout << "from lsm2EN, not lsm2" << std::endl;
   lsm2EN.WriteSellaMatrices(vtypesEN);
   //std::cout << std::endl << std::endl << "after ProcessVectorMapToPerpsAndProjectors, the vperps " << perps.size() << std::endl;
   //for (auto ita :perps)
   //{
   //   std::cout << ita.GetLabel() << "  " << ita.size() << std::endl;

   //}
   //store.ShowResults();
   //storeEN.ShowResults();
}

static int seed = 19191;

EnhancedS6 SellaBuild::MakeSampleType(const MatS6& m) {
   GenerateRandomLattice<S6> generate(seed);
   EnhancedS6 s6;
   s6.SetValid(false);
   int count = 0;
   bool again = true;

   while (again && count < 10) {
      const EnhancedS6 ran = generate.randSellingReduced();
      s6 = m * ran;
      again = !s6.GetValid();
      ++count;
   }
   return s6;
}


//double SellaBuild::TestOneType(const std::string& label, const S6 &s6, const std::vector<MatS6>& vm) {
//   double best = DBL_MAX;
//   unsigned long bestIndex;
//   S6 bestS6;
//   const double s6norm = s6.norm();
//   S6 prp;
//   for (unsigned long i = 0; i < vm.size(); ++i) {
//      prp = vm[i] * s6;
//      if (best > prp.norm()) {
//         best = prp.norm();
//         bestIndex = i;
//         bestS6 = vm[i] * s6;
//      }
//   }
//
//   //indexstore.Store(LRL_ToString(label, " ", bestIndex), LRL_ToString(best, "     ", bestS6));
//   return best;
//}

//void SellaBuild::TestAllTypes(const S6& s6) { // assumes that s6 is aleady reduced
//   for (unsigned long i = 0; i < perps.size(); ++i) {
//      const std::string label = perps[i].GetLabel();
//      const double best = TestOneType(label, s6, perps[i].GetMatrices());
//   }
//}

//std::vector<std::pair<std::string, double> > SellaBuild::GetVectorOfFits(const S6& s6) {
//   S6 reduced;
//   std::vector<std::pair<std::string, double> > v;
//   const double s6norm = s6.norm();
//   if (!s6.IsValid() && s6norm > 1.0E-4) return v;
//
//   const bool b = Selling::Reduce(s6, reduced);
//   if (b) {
//      for (unsigned long i = 0; i < perps.size(); ++i) {
//         const std::string label = perps[i].GetLabel();
//         const double bestForOneType = TestOneType(label, reduced, perps[i].GetMatrices());
//         v.push_back(std::make_pair(label, bestForOneType));
//      }
//   }
//   return v;
//}

bool IsNotDup(const std::vector<MatS6>& v, const MatS6& m) {
   for (unsigned long i = 0; i < v.size(); ++i) {
      if ((v[i] - m).norm() < 1.0E-4) return false;
   }
   return true;
}

std::vector<MatS6> RemoveForDuplicates(const std::vector<MatS6>& m) {
   std::vector<MatS6> v(1, m[0]);
   int count = 0;
   for (unsigned long i = 1; i < m.size(); ++i) {
      if (IsNotDup(v, m[i]))
         v.push_back(m[i]);
      else
         ++count;
   }
   return v;
}

void SellaBuild::Expand(const std::string& label, const MatS6& projectorForDeloneType) {
   const S6_Ordinals s6 = MakeSampleType(projectorForDeloneType);

   if (s6.IsValid()) {
      switch (s6.CountZeros()) {
      case 0:
         //StoreAllReflections(label, s6, transformations);
         StoreAllReflections(label, EnhancedS6(s6));
         break;
      case 1:
         //OneBound(label, s6, transformations);
         OneBound(label, EnhancedS6(s6));
         break;
      case 2:
      case 3:
         //ProcessZeros(label, s6, transformations);
         ProcessZeros(label, EnhancedS6(s6));
         break;
      default:
         throw "this should never happen";
         break;
      }
   }
}

//bool SellaBuild::FindDuplicate(const std::vector<S6_Ordinals>& out, const S6_Ordinals s6) {
//   bool fail = false;
//   if (!out.empty()) {
//      for (unsigned long o = 0; o < out.size(); ++o) {
//         const double d = (out[o] - s6).norm();
//         if ((out[o] - s6).norm() < 1.0E-6) fail = true;
//      }
//   }
//
//   return fail;
//}

bool SellaBuild::FindDuplicate(const std::vector<EnhancedS6>& out, const EnhancedS6 s6) {
   bool fail = false;
   if (!out.empty()) {
      for (unsigned long o = 0; o < out.size(); ++o) {
         const double d = (out[o] - s6).norm();
         if ((out[o] - s6).norm() < 1.0E-6) fail = true;
      }
   }

   return fail;
}

void SellaBuild::ProcessItemStoreToVectorMap() {
   std::vector<std::string> labels = storeEN.GetKeys();
   for (unsigned long i = 0; i < labels.size(); ++i) {
      std::vector<S6_Ordinals> out;
      std::vector<EnhancedS6> outEN;
      //const std::vector<std::pair<std::string, S6_Ordinals> > v = store.GetResult(labels[i]);
      const std::vector<std::pair<std::string, EnhancedS6> > vEN = storeEN.GetResult(labels[i]);
      //out.push_back(v[0].second);
      outEN.push_back(vEN[0].second);
      for (unsigned long iv = 1; iv < vEN.size(); ++iv) {
         const bool fail = FindDuplicate(outEN, vEN[iv].second);
            if (!fail)
         {
               //out.push_back(v[iv].second);
               outEN.push_back(vEN[iv].second);
            }
      }

      //std::vector<EnhancedS6> enh_out(out.size());
      //for (size_t kk = 0; kk < outEN.size(); ++kk) {
      //   enh_out[kk] = outEN[kk];
      //}
      //themap.insert(std::make_pair(labels[i], out));
      xfmap.insert(std::make_pair(labels[i], outEN));
   }
}

//std::vector<unsigned long> SellaBuild::FindS6Zeros(const S6& s) {
//   std::vector<unsigned long> v;
//   for (unsigned long i = 0; i < 6; ++i) if (s[i] == 0.0) v.push_back(i);
//   return v;
//}

std::vector<unsigned long> SellaBuild::FindS6Zeros(const EnhancedS6& s) {
   std::vector<unsigned long> v;
   for (unsigned long i = 0; i < 6; ++i) if (s[i] == 0.0) v.push_back(i);
   return v;
}

//template<typename T>
//S6_Ordinals MultiplyUsingFunction(T refl, const S6_Ordinals& so)
//{
//   S6_Ordinals s1(so);
//   s1 = refl(s1);
//   s1.m_ordinals = refl(s1.m_ordinals);
//   return s1;
//}

//template<typename T>
//EnhancedS6 MultiplyUsingFunction(T refl, const EnhancedS6& so)
//{
//   EnhancedS6 s1(so);
//   s1 = refl(s1);
//   return refl(s1);
//}

//void SellaBuild::StoreAllReflections(const std::string& label, const S6_Ordinals& s1in, const MatS6& transformations) {
//   std::vector< S6(*)(const S6&)> refl = S6::SetReflectionFunctions();
//   S6_Ordinals s1(s1in);
//
//   for (unsigned long i = 0; i < refl.size(); ++i) {
//      store.Store(label, MultiplyUsingFunction(refl[i], s1));
//   }
//}

void SellaBuild::StoreAllReflections(const std::string& label, const EnhancedS6& s1in) {
   //std::vector<EnhancedS6(*)(const EnhancedS6&)> refl = EnhancedS6::SetReflectionFunctions();
   static const std::vector<MatS6> vrefl_mats = MatS6::GetReflections();
   for (const auto& m : vrefl_mats) {
      storeEN.Store(label, m * s1in);
   }
}

static MatS6 GetS6ReductionMatrix(const size_t n) {
   const static    std::vector<std::pair<MatS6, MatS6> > fullRedMats = S6::SetUnreductionMatrices();
   const static std::vector<MatS6> redmats = {
      fullRedMats[0].first,
      fullRedMats[1].first,
      fullRedMats[2].first,
      fullRedMats[3].first,
      fullRedMats[4].first,
      fullRedMats[5].first
   };
   return redmats[n];
}

//void SellaBuild::OneBound(const std::string& label, const S6_Ordinals& s1, MatS6 transformations) {
//   static const std::vector< S6(*)(const S6&)> fnRedn = S6Dist::SetVCPFunctions();
//
//   unsigned long nzero = 0;
//   for (unsigned long i = 0; i < 6; ++i) if (s1[i] == 0) nzero = i;
//   //StoreAllReflections(label, s1, transformations);
//   StoreAllReflections(label, EnhancedS6(s1), transformations);
//   //const S6_Ordinals s6temp = MultiplyUsingFunction(fnRedn[nzero], s1);
//   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! doesn't accumulate the transformation here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//   //StoreAllReflections(label, S6_Ordinals(s6temp), transformations);
//   StoreAllReflections(label, GetS6ReductionMatrice(nzero) * EnhancedS6(s1), transformations);
//}

std::vector<EnhancedS6> SellaBuild::CreateAllReflections(const std::string& label, const EnhancedS6& s1) {
   static const std::vector<MatS6> vrefl_mats = MatS6::GetReflections();
   std::vector<EnhancedS6> out;
   for (const auto& m : vrefl_mats) {
      const EnhancedS6 ms1 = m * s1;
      out.emplace_back(m * s1);
   }
   return out;
}

void SellaBuild::OneBound(const std::string& label, const EnhancedS6& s1) {
   //static const std::vector< S6(*)(const S6&)> fnRedn = S6Dist::SetVCPFunctions();
   const size_t n = FindS6Zeros(s1)[0];
   unsigned long nzero = 0;
   for (unsigned long i = 0; i < 6; ++i) if (s1[i] == 0) nzero = i;
   //StoreAllReflections(label, S6_Ordinals(s1), transformations);
   StoreAllReflections(label, s1);

   const MatS6 mat = GetS6ReductionMatrix(n);
   const EnhancedS6 s6temp = mat * s1;

   //THIS IS WRONG !!!!!!!!!!!!!!!!!!!!!!!!!!!
   //const S6_Ordinals s6temp = MultiplyUsingFunction(fnRedn[nzero], s1);
   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! doesn't accumulate the transformation here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   //StoreAllReflections(label, S6_Ordinals(s6temp), transformations);
   StoreAllReflections(label, s6temp);
}

//void SellaBuild::ProcessZeros(const std::string& label, const S6_Ordinals& s6, MatS6 transformations) {
//   const std::vector<unsigned long> v = FindS6Zeros(s6);
//   const unsigned long nzeros = (unsigned long)(v.size());
//
//   if (nzeros == 0) {
//      return;
//   }
//   else if (nzeros == 1) {
//      OneBound(label, s6, transformations);
//   }
//   else {
//      for (unsigned long i = 0; i < v.size(); ++i) {
//         S6_Ordinals temp(s6);
//         temp[v[i]] = DBL_MIN;
//         ProcessZeros(label, temp, transformations);
//      }
//   }
//
//}

void SellaBuild::ProcessMultipleZeros(const std::string& label, const EnhancedS6& s6) {
   StoreAllReflections(label, s6);
   const std::vector<unsigned long> v = FindS6Zeros(s6);
   for (unsigned long i = 0; i < v.size(); ++i) {
      EnhancedS6 temp(s6);
      temp[v[i]] = DBL_MIN;  // makes it not zero
      const MatS6 mat = GetS6ReductionMatrix(v[i]);
      StoreAllReflections(label, mat * temp);
      ProcessMultipleZeros(label, mat*temp);
   }
}

void SellaBuild::ProcessTwoZeros(const std::string& label, const EnhancedS6& s6) {
   StoreAllReflections(label, s6);
   const std::vector<unsigned long> v = SellaBuild::FindS6Zeros(s6);
   const MatS6 mat1 = GetS6ReductionMatrix(v[0]);
   const MatS6 mat2 = GetS6ReductionMatrix(v[1]);
   S6 s1 = mat1 * s6;
   s1[v[0]] = DBL_MIN;
   S6 s2 = mat2 * s6;
   s2[v[1]] = DBL_MIN;
   OneBound(label, s1);
   OneBound(label, s2);
   //StoreAllReflections(label, mat1 * mat2 * s6);
   //StoreAllReflections(label, mat2 * mat1 * s6);
   //StoreAllReflections(label, mat2 * mat1 * mat2 * s6);
   //StoreAllReflections(label, mat1 * mat2 * mat1 * s6);
}

void SellaBuild::ProcessTwoZeros(const std::string& label, const std::vector<EnhancedS6>& vs) {
   for (size_t i = 0; i < vs.size(); ++i) {
      const EnhancedS6 si = vs[i];
      const std::vector<EnhancedS6> vrefl = CreateAllReflections(label, si);
      for (const auto& sr : vrefl) {
         ProcessTwoZeros(label, sr);
      }
   }
}

void SellaBuild::ProcessThreeZeros(const std::string& label, const EnhancedS6& s6) {
   StoreAllReflections(label, s6);
   const std::vector<unsigned long> v = SellaBuild::FindS6Zeros(s6);
   const MatS6 mat1 = GetS6ReductionMatrix(v[0]);
   const MatS6 mat2 = GetS6ReductionMatrix(v[1]);
   const MatS6 mat3 = GetS6ReductionMatrix(v[2]);
   EnhancedS6 s1 = mat1 * s6;
   EnhancedS6 s2 = mat2 * s6;
   EnhancedS6 s3 = mat3 * s6;
   s1[v[0]] = DBL_MIN;
   s2[v[1]] = DBL_MIN;
   s3[v[1]] = DBL_MIN;
   ProcessTwoZeros(label, CreateAllReflections(label, s1));
   ProcessTwoZeros(label, CreateAllReflections(label, s2));
   ProcessTwoZeros(label, CreateAllReflections(label, s3));
   //StoreAllReflections(label, mat1 * mat2 * s6);
   //StoreAllReflections(label, mat1 * mat3 * s6);
   //StoreAllReflections(label, mat1 * mat2 * mat1 * s6);
   //StoreAllReflections(label, mat1 * mat3 * mat1 * s6);

   //StoreAllReflections(label, mat2 * mat1 * s6);
   //StoreAllReflections(label, mat2 * mat3 * s6);
   //StoreAllReflections(label, mat2 * mat1 * mat2 * s6);
   //StoreAllReflections(label, mat2 * mat3 * mat2 * s6);

   //StoreAllReflections(label, mat3 * mat1 * s6);
   //StoreAllReflections(label, mat3 * mat2 * s6);
   //StoreAllReflections(label, mat3 * mat1 * mat3 * s6);
   //StoreAllReflections(label, mat3 * mat2 * mat3 * s6);


   //StoreAllReflections(label, mat1 * mat2 * mat3 * s6);
   //StoreAllReflections(label, mat1 * mat3 * mat2 * s6);
   //StoreAllReflections(label, mat2 * mat1 * mat3 * s6);
   //StoreAllReflections(label, mat2 * mat3 * mat1 * s6);
   //StoreAllReflections(label, mat3 * mat2 * mat1 * s6);
   //StoreAllReflections(label, mat3 * mat1 * mat2 * s6);

   //StoreAllReflections(label, mat2 * mat3 * mat1 * mat2 * s6);
   //StoreAllReflections(label, mat3 * mat1 * mat2 * mat3 * s6);
   //StoreAllReflections(label, mat2 * mat1 * mat3 * mat2 * s6);
   //StoreAllReflections(label, mat3 * mat2 * mat1 * mat3 * s6);
   //StoreAllReflections(label, mat1 * mat2 * mat3 * mat1 * s6);
   //StoreAllReflections(label, mat1 * mat3 * mat2 * mat1 * s6);
}


void SellaBuild::ProcessZeros(const std::string& label, const EnhancedS6& s6) {
   const std::vector<unsigned long> v = FindS6Zeros(s6);
   const unsigned long nzeros = (unsigned long)(v.size());

   if (nzeros == 0) {
      return;
   }
   else if (nzeros == 1) {
      OneBound(label, s6);
   }
   else if (nzeros == 2) {
      ProcessTwoZeros(label, s6);
   }
   else if (nzeros == 3) {
      ProcessThreeZeros(label, s6);
   }
   else {
      //for (unsigned long i = 0; i < v.size(); ++i) {
      //   EnhancedS6 temp(s6);
      //   temp[v[i]] = DBL_MIN;  // makes it not zero
      //   std::cout << temp << std::endl;
         //ProcessZeros(label, temp);
         ProcessMultipleZeros(label, s6);
         // LCA this would not work for three zeros !!!!!!!!!!!!!!!!!!!!!
         // I'm not even sure it's correct for two
         // I think the issue is that after that call to ProcessZeros,
         // temp needs to be modified by the boundary transform,
         // and then ProcessZeros called again.

         // Then somewhere before condensing the results, the near-zeros,
         // need to be reset!!!!!!!!!!!!!
         // Or else the comparison function needs to allow a small difference.
      //}
   }

}

//void SellaBuild::TestPerps() {
//   const G6 g6(" 100 100 100 100 100 100");
//   const LRL_Cell cell(" 10 10 10 89 89 89");
//   const S6 s6(cell);
//   TestAllTypes(s6);
//}

//void SellaBuild::ShowIndexResults() const {
//   indexstore.ShowResults();
//}

