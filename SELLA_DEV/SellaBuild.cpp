#include "SellaBuild.h"


#include "GenerateRandomLattice.h"
#include "LabeledSellaMatricesForDeloneType.h"
#include "LRL_ToString.h"
#include "LRL_MaximaTools.h"
#include "S6_Ordinals.h"
#include "S6Dist.h"
#include "Selling.h"
#include "StoreResults.h"

std::vector< std::pair<std::string, MatS6> > SellaBuild::vDeloneTypes = Delone::LoadLabeledLatticeTypeProjectors();

SellaBuild::SellaBuild() {
   store.SetMaxItemStore(350);
}

void SellaBuild::Build() {
	MatS6 transformations;
   for (unsigned long i = 0; i < vDeloneTypes.size(); ++i) {
		transformations.unit();
      //for (unsigned long i = 2; i < 3; ++i) {  // to show only C5
      Expand(vDeloneTypes[i].first, vDeloneTypes[i].second, transformations);
   }
   store.ShowTableOfKeysVersusCount();
   ProcessItemStoreToVectorMap();
   std::cout << std::endl << std::endl << "after ProcessItemStoreToVectorMap, the map " << themap.size() << std::endl;
   for (auto ita = themap.begin(); ita != themap.end(); ++ita) std::cout << (*ita).first << "  " << (*ita).second.size() << std::endl;
   LabeledSellaMatricesForDeloneType lsm2;
   const std::vector<LabeledSellaMatricesForDeloneType> vtypes = lsm2.ProcessVectorMapToPerpsAndProjectors(themap);
   lsm2.WriteSellaMatrices(vtypes);
   //std::cout << std::endl << std::endl << "after ProcessVectorMapToPerpsAndProjectors, the vperps " << perps.size() << std::endl;
   //for (auto ita = perps.begin(); ita != perps.end(); ++ita) std::cout << (*ita).GetLabel() << "  " << (*ita).size() << std::endl;
   //store.ShowResults();
}

static int seed = 19191;

S6 SellaBuild::MakeSampleType(const MatS6& m) {
   GenerateRandomLattice<S6> generate(seed);
   S6 s6;
   s6.SetValid(false);
   int count = 0;
   bool again = true;

   while (again && count < 10) {
      const S6 ran = generate.randSellingReduced();
      s6 = m * ran;
      again = !s6.GetValid();
      ++count;
   }
   return s6;
}


double SellaBuild::TestOneType(const std::string& label, const S6 &s6, const std::vector<MatS6>& vm) {
   double best = DBL_MAX;
   unsigned long bestIndex;
   S6 bestS6;
   const double s6norm = s6.norm();
   S6 prp;
   for (unsigned long i = 0; i < vm.size(); ++i) {
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

void SellaBuild::TestAllTypes(const S6& s6) { // assumes that s6 is aleady reduced
   for (unsigned long i = 0; i < perps.size(); ++i) {
      const std::string label = perps[i].GetLabel();
      const double best = TestOneType(label, s6, perps[i].GetMatrices());
   }
}

std::vector<std::pair<std::string, double> > SellaBuild::GetVectorOfFits(const S6& s6) {
   S6 reduced;
   std::vector<std::pair<std::string, double> > v;
   const double s6norm = s6.norm();
   if (!s6.IsValid() && s6norm > 1.0E-4) return v;

   const bool b = Selling::Reduce(s6, reduced);
   if (b) {
      for (unsigned long i = 0; i < perps.size(); ++i) {
         const std::string label = perps[i].GetLabel();
         const double bestForOneType = TestOneType(label, reduced, perps[i].GetMatrices());
         v.push_back(std::make_pair(label, bestForOneType));
      }
   }
   return v;
}

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

void SellaBuild::Expand(const std::string& label, const MatS6& m, MatS6 transformations) {
   const S6_Ordinals s6 = MakeSampleType(m);

   if (s6.IsValid()) {
      switch (s6.CountZeros()) {
      case 0:
         StoreAllReflections(label, s6, transformations);
         break;
      case 1:
         OneBound(label, s6, transformations);
         break;
      case 2:
      case 3:
         ProcessZeros(label, s6, transformations);
         break;
      default:
         throw "this should never happen";
         break;
      }
   }
   //store.ShowResults();
}

bool SellaBuild::FindDuplicate(const std::vector<S6_Ordinals>& out, const S6_Ordinals s6) {
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
   std::vector<std::string> labels = store.GetKeys();
   for (unsigned long i = 0; i < labels.size(); ++i) {
      std::vector<S6_Ordinals> out;
      const std::vector<std::pair<std::string, S6_Ordinals> > v = store.GetResult(labels[i]);
      out.push_back(v[0].second);
      for (unsigned long iv = 1; iv < v.size(); ++iv) {
         const bool fail = FindDuplicate(out, v[iv].second);
         if (!fail) out.push_back(v[iv].second);
      }
      themap.insert(std::make_pair(labels[i], out));
   }
}

std::vector<unsigned long> SellaBuild::FindS6Zeros(const S6& s) {
   std::vector<unsigned long> v;
   for (unsigned long i = 0; i < 6; ++i) if (s[i] == 0.0) v.push_back(i);
   return v;
}

template<typename T>
S6_Ordinals MultiplyUsingFunction(T refl, const S6_Ordinals& so )
{
   S6_Ordinals s1(so);
   s1 = refl(s1);
   s1.m_ordinals = refl(s1.m_ordinals);
   return s1;
}

void SellaBuild::StoreAllReflections(const std::string& label, const S6_Ordinals& s1in, const MatS6& transformations) {
   std::vector< S6(*)(const S6&)> refl = S6::SetRelectionFunctions();
   S6_Ordinals s1(s1in);

   for (unsigned long i = 0; i < refl.size(); ++i) {
      store.Store(label, MultiplyUsingFunction(refl[i], s1));
   }
   //store.ShowResults();
}

void SellaBuild::OneBound(const std::string& label, const S6_Ordinals& s1, MatS6 transformations) {
   static const std::vector< S6(*)(const S6&)> fnRedn = S6Dist::SetVCPFunctions();

   unsigned long nzero = 0;
   for (unsigned long i = 0; i < 6; ++i) if (s1[i] == 0) nzero = i;
   StoreAllReflections(label, s1, transformations);
   const S6_Ordinals s6temp = MultiplyUsingFunction(fnRedn[nzero], s1);
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! doesn't accumulate the transformation here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   StoreAllReflections(label, s6temp, transformations);
}

void SellaBuild::ProcessZeros(const std::string& label, const S6_Ordinals& s6, MatS6 transformations) {
   const std::vector<unsigned long> v = FindS6Zeros(s6);
   const unsigned long nzeros = (unsigned long)(v.size());

   if (nzeros == 0) {
      return;
   }
   else if (nzeros == 1) {
      OneBound(label, s6, transformations);
   }
   else {
      for (unsigned long i = 0; i < v.size(); ++i) {
         S6_Ordinals temp(s6);
         temp[v[i]] = DBL_MIN;
         ProcessZeros(label, temp, transformations);
      }
   }

}

void SellaBuild::TestPerps() {
   const G6 g6(" 100 100 100 100 100 100");
   const LRL_Cell cell(" 10 10 10 89 89 89");
   const S6 s6(cell);
   TestAllTypes(s6);
}

void SellaBuild::ShowIndexResults() const {
   indexstore.ShowResults();
}

