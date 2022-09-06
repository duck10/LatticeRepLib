#include "SellaBuild.h"

#include "Delone.h"
#include "GenerateRandomLattice.h"
#include "LRL_ToString.h"
#include "LRL_MaximaTools.h"
#include "S6BoundaryTransforms.h"
#include "S6_Ordinals.h"
#include "S6Dist.h"
#include "Selling.h"

std::vector< std::pair<std::string, MatS6> > SellaBuild::vDeloneTypes = Delone::LoadLabeledLatticeTypeProjectors();
static const std::vector<MatS6> g_refls = MatS6::GetReflections();
static const std::vector< S6(*)(const S6&)> fnRedn = S6Dist::SetVCPFunctions();


SellaBuild::SellaBuild() {
   store.SetMaxItemStore(20000);
}

std::vector<LabeledDeloneTypeMatrices>  SellaBuild::Build() {
	MatS6 transformations;

   if (vDeloneTypes.empty()) SellaBuild::vDeloneTypes = Delone::LoadLabeledLatticeTypeProjectors();


   for (size_t i = 0; i < vDeloneTypes.size(); ++i) {
      transformations = transformations.unit();
      //Expand(vDeloneTypes[i].first, vDeloneTypes[i].second, transformations);
      Expand(vDeloneTypes[i].first, MakeSampleType(vDeloneTypes[i].second));
   }
   store.ShowTableOfKeysVersusCount();
   ProcessItemStoreToVectorMap();
   std::cout << std::endl << std::endl << "after ProcessItemStoreToVectorMap, the map " << themap.size() << std::endl;
   for (std::map<std::string, std::vector<S6_Ordinals> >::const_iterator ita = themap.begin(); ita != themap.end(); ++ita) std::cout << (*ita).first << "  " << (*ita).second.size() << std::endl;
   LabeledDeloneTypeMatrices lsm2;
   const std::vector<LabeledDeloneTypeMatrices> vtypes = lsm2.ProcessVectorMapToPerpsAndProjectors(themap);

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

void SellaBuild::StoreAllReflections( const std::string& label, const S6_Ordinals& s1in ) {
   const std::vector< S6( *)(const S6&)> refl = S6::SetRelectionFunctions( );
   S6_Ordinals s1( s1in );

   for (size_t i = 0; i < refl.size( ); ++i) {
      store.Store( label, refl[i](s1 ) );
   }
   //store.ShowResults();
}

void SellaBuild::Expand(const std::string& label, const S6& sample) {
   const size_t nzeros = sample.CountZeros();
   const std::vector<size_t> vZeros = FindS6Zeros(sample);

   std::vector<MatS6> vt;

   if (nzeros == 0) {
      S6BoundaryMatricesZero smz;
      vt = smz.GetVector();
   }
   else if ( nzeros == 1) {
      S6BoundaryMatricesOne smo(vZeros[0]);
         vt = smo.GetVector();
   }
   else if (nzeros == 2) {
      S6BoundaryMatricesThree smthr(2, 3, 5);
      vt = smthr.GetVector();
   }      
   else if (nzeros == 3) {
      S6BoundaryMatricesThree smthr(2, 3, 5);
      vt = smthr.GetVector();
   }

   for (size_t i = 0; i < vt.size(); ++i) {
      store.Store(label, vt[i] * sample);
   }
   
   const int i19191 = 19191;


      
   
   //switch (nzeros) {
   //case 0:
   //   S6BoundaryMatricesZero smz;
   //   vt = smz.GetVector();
   //   break;
   //case 1:
   //   S6BoundaryMatricesOne smo(0);
   //   vt = smo.GetVector();
   //   break;
   //case 2:
   //   S6BoundaryMatricesTwo smtwo(2, 3);
   //   vt = smtwo.GetVector();
   //   break;
   //case 3:
   //   S6BoundaryMatricesThree smthr(2, 3, 5);
   //   vt = smthr.GetVector();
   //   break;
   //default:
   //   throw "this should never happen";
   //   break;
   //}
   //store.ShowResults();

}

void SellaBuild::Expand(const std::string& label, const MatS6& m, MatS6 transformations) {
   const S6_Ordinals s6 = MakeSampleType(m);
   const std::vector<size_t> vZeros = FindS6Zeros(s6);

   if (s6.IsValid()) {
      switch (s6.CountZeros()) {
      case 0:
         StoreAllReflections(label, s6 );
         break;
      case 1:
         OneBound(label, s6, vZeros[0]);
         break;
      case 2:
         ProcessTwoZeros(label, s6, vZeros);
         break;
      case 3:
         ProcessThreeZeros(label, s6, vZeros);
         break;
      default:
         throw "this should never happen";
         break;
      }
   }
   //store.ShowResults();
}

void SellaBuild::OneBound(const std::string& label, const S6_Ordinals& s1,
   const size_t zeroBound)
{
   const S6_Ordinals s6temp = fnRedn[zeroBound](s1);
   StoreAllReflections(label, s6temp);
}

void SellaBuild::ProcessTwoZeros(const std::string& label, const S6& s6, const std::vector<size_t>& vZeros) {

   // handle input as if no zeros, just reflections
   StoreAllReflections(label, s6);

   // Process each zero by itself
   OneBound(label, s6, vZeros[0]);
   OneBound(label, s6, vZeros[1]);
   
   fnRedn[0](s6);
   const S6 red1 =  fnRedn[vZeros[0]](s6);
   const S6 red2 =  fnRedn[vZeros[1]](s6);
   const S6 red12 = fnRedn[vZeros[0]](red1);
   const S6 red21 = fnRedn[vZeros[1]](red2);
   StoreAllReflections(label, red12);
   StoreAllReflections(label, red21);

}

void SellaBuild::ProcessThreeZeros(const std::string& label, const S6& s6, const std::vector<size_t>& vZeros) {
   // base vector
   StoreAllReflections(label, s6);

   // treat each separate bound
   const S6 red_0 = fnRedn[vZeros[0]](s6);
   const S6 red_1 = fnRedn[vZeros[1]](s6);
   const S6 red_2 = fnRedn[vZeros[2]](s6);
   
   StoreAllReflections(label, red_0);
   StoreAllReflections(label, red_1);
   StoreAllReflections(label, red_2);


   // treat each pair of zeros
   const S6 red_1_0 = fnRedn[vZeros[1]](red_0);
   const S6 red_2_0 = fnRedn[vZeros[2]](red_0);
   const S6 red_0_1 = fnRedn[vZeros[0]](red_1);
   const S6 red_2_1 = fnRedn[vZeros[2]](red_1);
   const S6 red_0_2 = fnRedn[vZeros[0]](red_2);
   const S6 red_1_2 = fnRedn[vZeros[1]](red_2);

   StoreAllReflections(label, red_1_0);
   StoreAllReflections(label, red_2_0);
   StoreAllReflections(label, red_0_1);
   StoreAllReflections(label, red_2_1);
   StoreAllReflections(label, red_0_2);
   StoreAllReflections(label, red_1_2);

   // now process all three zeros
   const S6 red_0_1_2 = fnRedn[vZeros[0]](red_1_2);
   const S6 red_0_2_1 = fnRedn[vZeros[0]](red_2_1);
   const S6 red_1_0_2 = fnRedn[vZeros[1]](red_0_2);
   const S6 red_1_2_0 = fnRedn[vZeros[1]](red_2_0);
   const S6 red_2_0_1 = fnRedn[vZeros[2]](red_0_1);
   const S6 red_2_1_0 = fnRedn[vZeros[2]](red_1_0);

   StoreAllReflections(label, red_0_1_2);
   StoreAllReflections(label, red_0_2_1);
   StoreAllReflections(label, red_1_0_2);
   StoreAllReflections(label, red_1_2_0);
   StoreAllReflections(label, red_2_0_1);
   StoreAllReflections(label, red_2_1_0);

}

void SellaBuild::ProcessZeros( const std::string& label, const S6_Ordinals& s6 ) {
   std::vector< S6(*)(const S6&)> refl = S6::SetRelectionFunctions();

   const std::vector<size_t> vZeros = FindS6Zeros( s6 );
   const size_t nzeros = vZeros.size( );

   if (nzeros == 0) {
      return;
   } else if (nzeros == 1) {
      OneBound( label, s6, vZeros[0]);
   }
   else if (nzeros == 2) {
      ProcessTwoZeros(label, s6, vZeros);
   }
   else {
      ProcessThreeZeros(label, s6, vZeros);
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

void SellaBuild::ShowIndexResults() const {
   indexstore.ShowResults();
}

