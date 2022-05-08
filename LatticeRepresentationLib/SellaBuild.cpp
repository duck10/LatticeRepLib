#include "SellaBuild.h"

#include "Delone.h"
#include "GenerateRandomLattice.h"
#include "LRL_ToString.h"
#include "LRL_MaximaTools.h"
#include "S6_Ordinals.h"
#include "S6Dist.h"
#include "Selling.h"

std::vector< std::pair<std::string, MatS6> > SellaBuild::vDeloneTypes = Delone::LoadLabeledLatticeTypeProjectors();

void WriteSellMatrixBase() {}

SellaBuild::SellaBuild() {
   store.SetMaxItemStore(350);
}

std::vector<LabeledDeloneTypeMatrices>  SellaBuild::Build() {
	MatS6 transformations;

   if (vDeloneTypes.empty()) SellaBuild::vDeloneTypes = Delone::LoadLabeledLatticeTypeProjectors();


   for (size_t i = 0; i < vDeloneTypes.size(); ++i) {
      transformations = transformations.unit();
      Expand(vDeloneTypes[i].first, vDeloneTypes[i].second, transformations);
   }
   store.ShowTableOfKeysVersusCount();
   ProcessItemStoreToVectorMap();
   std::cout << std::endl << std::endl << "after ProcessItemStoreToVectorMap, the map " << themap.size() << std::endl;
   for (std::map<std::string, std::vector<S6_Ordinals> >::const_iterator ita = themap.begin(); ita != themap.end(); ++ita) std::cout << (*ita).first << "  " << (*ita).second.size() << std::endl;
   LabeledDeloneTypeMatrices lsm2;
   const std::vector<LabeledDeloneTypeMatrices> vtypes = lsm2.ProcessVectorMapToPerpsAndProjectors(themap);

   WriteSellMatrixBase();
   lsm2.WriteSellaMatrices(vtypes);
   return vtypes;
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
   size_t bestIndex;
   S6 bestS6;
   const double s6norm = s6.norm();
   S6 prp;
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

void SellaBuild::TestAllTypes(const S6& s6) { // assumes that s6 is aleady reduced
   for (size_t i = 0; i < perps.size(); ++i) {
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
      for (size_t i = 0; i < perps.size(); ++i) {
         const std::string label = perps[i].GetLabel();
         const double bestForOneType = TestOneType(label, reduced, perps[i].GetMatrices());
         v.push_back(std::make_pair(label, bestForOneType));
      }
   }
   return v;
}

bool IsNotDup(const std::vector<MatS6>& v, const MatS6& m) {
   for (size_t i = 0; i < v.size(); ++i) {
      if ((v[i] - m).norm() < 1.0E-4) return false;
   }
   return true;
}

std::vector<MatS6> RemoveForDuplicates(const std::vector<MatS6>& m) {
   std::vector<MatS6> v(1, m[0]);
   int count = 0;
   for (size_t i = 1; i < m.size(); ++i) {
      if (IsNotDup(v, m[i]))
         v.push_back(m[i]);
      else
         ++count;
   }
   return v;
}

template<typename T>
S6_Ordinals MultiplyUsingFunction( T refl, const S6_Ordinals& so )
{
   S6_Ordinals s1( so );
   s1 = refl( s1 );
   s1.m_ordinals = refl( s1.m_ordinals );
   return s1;
}

void SellaBuild::StoreAllReflections( const std::string& label, const S6_Ordinals& s1in, const MatS6& transformations ) {
   std::vector< S6( *)(const S6&)> refl = S6::SetRelectionFunctions( );
   S6_Ordinals s1( s1in );

   for (size_t i = 0; i < refl.size( ); ++i) {
      store.Store( label, MultiplyUsingFunction( refl[i], s1 ) );
   }
   //store.ShowResults();
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

void SellaBuild::OneBound( const std::string& label, const S6_Ordinals& s1, MatS6 transformations ) {
   static const std::vector< S6( *)(const S6&)> fnRedn = S6Dist::SetVCPFunctions( );

   size_t nzero = 0;
   for (size_t i = 0; i < 6; ++i) if (s1[i] == 0) nzero = i;
   StoreAllReflections( label, s1, transformations );
   const S6_Ordinals s6temp = MultiplyUsingFunction( fnRedn[nzero], s1 );
   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! doesn't accumulate the transformation here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   StoreAllReflections( label, s6temp, transformations );
}

std::vector<S6> MakeAllReflections(const S6& s) {
   std::vector<S6> v;
   static std::vector<MatS6> refls = MatS6::GetReflections();
   for (size_t i = 0; i < refls.size(); ++i) {
      v.push_back(refls[i] * s);
   }
   return v;
}

void SellaBuild::ProcessZeros( const std::string& label, const S6_Ordinals& s6, MatS6 transformations ) {
   static const std::vector< S6(*)(const S6&)> fnRedn = S6Dist::SetVCPFunctions();

   const std::vector<size_t> v = FindS6Zeros( s6 );
   const size_t nzeros = v.size( );

   const size_t totalTransformationCount = store.GetTotalSampleCount();
   const size_t itemTransformationCount = store.GetItemCount(label);

   if (nzeros == 0) {
      return;
   } else if (nzeros == 1) {
      OneBound( label, s6, transformations );
   }
   else if (nzeros == 2) {


      const S6 s1 = MultiplyUsingFunction(fnRedn[v[0]], s6);
      const S6 s2 = MultiplyUsingFunction(fnRedn[v[1]], s6);
      StoreAllReflections(label, s1, transformations);
      StoreAllReflections(label, s2, transformations);

      const std::vector<S6> reflected1 = MakeAllReflections(s1);
      const std::vector<S6> reflected2 = MakeAllReflections(s2);

      for (size_t i = 0; i < reflected1.size(); ++i) {
         StoreAllReflections(label,
            MultiplyUsingFunction(fnRedn[v[1]], reflected1[i]), transformations);
      }

      for (size_t i = 0; i < reflected2.size(); ++i) {
         StoreAllReflections(label,
            MultiplyUsingFunction(fnRedn[v[0]], reflected2[i]), transformations);
      }


   }
   else {
      for (size_t i = 0; i < v.size( ); ++i) {
         S6_Ordinals temp( s6 );
         temp[v[i]] = DBL_MIN;
         ProcessZeros( label, temp, transformations );
      }
   }

}


bool SellaBuild::FindDuplicate(const std::vector<S6_Ordinals>& out, const S6_Ordinals s6) {
   bool fail = false;
   if (!out.empty()) {
      for (size_t o = 0; o < out.size(); ++o) {
         const double d = (out[o] - s6).norm();
         if ((out[o] - s6).norm() < 1.0E-6) fail = true;
      }
   }

   return fail;
}

void SellaBuild::ProcessItemStoreToVectorMap() {
   std::vector<std::string> labels = store.GetKeys();
   for (size_t i = 0; i < labels.size(); ++i) {
      std::vector<S6_Ordinals> out;
      const std::vector<std::pair<std::string, S6_Ordinals> > v = store.GetResult(labels[i]);
      out.push_back(v[0].second);
      for (size_t iv = 1; iv < v.size(); ++iv) {
         const bool fail = FindDuplicate(out, v[iv].second);
         if (!fail) out.push_back(v[iv].second);
      }
      themap.insert(std::make_pair(labels[i], out));
   }
}

std::vector<size_t> SellaBuild::FindS6Zeros(const S6& s) {
   std::vector<size_t> v;
   for (size_t i = 0; i < 6; ++i) if (s[i] == 0.0) v.push_back(i);
   return v;
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

