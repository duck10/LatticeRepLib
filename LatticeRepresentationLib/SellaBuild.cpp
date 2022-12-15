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
      Expand(vDeloneTypes[i].first, vDeloneTypes[i].second, transformations);
      //Expand(std::set<size_t>(), vDeloneTypes[i].first, MakeSampleType(vDeloneTypes[i].second));
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
      s6 = m * generate.randSellingReduced();
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

void SellaBuild::StoreAllReflections(const std::string& label, const std::set<S6_Ordinals>& s) {
    for (std::set<S6_Ordinals>::const_iterator it = s.begin(); it != s.end(); ++it) {
        StoreAllReflections(label, *it);
    }
}

/*
Produce a list of the zeros in zeros. The list should have the indices of all
those zeros in zeros, EXCEPT for the ones in the exclusions. vZeros has the
list of those indices that are on a boundary and therefore need transformations.
*/

std::vector<size_t> SellaBuild::DetermineToProcess(const std::set<size_t>& exclusions,
   const S6& zeros)
{ // vZeros contains the INDICES of the zero elements of zeros
   std::vector<size_t> vZeros = FindS6Zeros(zeros);
   for (auto it = exclusions.begin(); it != exclusions.end(); ++it)
   {
      auto zeroIT = std::find(vZeros.begin(), vZeros.end(), *it);
      if (zeroIT != vZeros.end()) vZeros.erase(zeroIT);
   }
   return vZeros;
}


void SellaBuild::Expand(const std::set<size_t>& exclusions,
   const std::string& label, const S6& sample) {

   const std::vector<size_t> toProcess = DetermineToProcess(exclusions, sample);
   const size_t nzeros = toProcess.size();

   std::vector<MatS6> vt;

   if (nzeros == 0) {
      S6BoundaryMatricesZero smz;
      vt = smz.GetVector();
   }
   else if ( nzeros == 1) {
      S6BoundaryMatricesOne smo(toProcess[0]);
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

S6 SetPos(const size_t n) {
    S6 zero(0, 0, 0, 0, 0, 0); 
    zero[n] += 0.0000000001; 
    return zero; 
}

S6 SetPos(const size_t n, const S6& s6) {
    S6 out(s6); 
    out[n] += 0.0000000001; 
    return out; 
}

void SellaBuild::Expand(const std::string& label, const MatS6& m, MatS6 transformations) {
   const S6_Ordinals s6 = MakeSampleType(m);
   const std::vector<size_t> vZeros = FindS6Zeros(s6);
   if (vZeros.empty()) {
       StoreAllReflections(label, s6);
   }
   else if (s6.IsValid()) {
      switch (s6.CountZeros()) {
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
   //store.ShowResults();
}

void SellaBuild::OneBound(const std::string& label, const S6_Ordinals& s1,
   const size_t zeroBound)
{
   const S6_Ordinals s6temp = fnRedn[zeroBound](s1);
   StoreAllReflections(label, s6temp);
}

void SellaBuild::ProcessTwoZeros(const std::string& label, const S6& s6) {
    const std::vector<size_t> vZeros = FindS6Zeros(s6);

   // handle input as if no zeros, just reflections
   StoreAllReflections(label, s6);

   // Process each zero by itself
   OneBound(label, s6, vZeros[0]);
   OneBound(label, s6, vZeros[1]);
   
   const S6 red1 = SetPos(vZeros[0],fnRedn[vZeros[0]](s6)); //LCA this is the error point!!!
   // We don't really know where vZeros[1] went in red1
   const S6 red2 = SetPos(vZeros[1],fnRedn[vZeros[1]](s6));

   const std::vector<size_t> vZeros1 = FindS6Zeros(red1);
   const std::vector<size_t> vZeros2 = FindS6Zeros(red2);

   const S6 red12 = fnRedn[vZeros1[0]](red1);
   const S6 red21 = fnRedn[vZeros2[0]](red2);

   StoreAllReflections(label, red1);
   StoreAllReflections(label, red2);
   StoreAllReflections(label, red12);
   StoreAllReflections(label, red21);
}

S6 SetPos(const size_t n, const size_t m) {
    S6 zero(0, 0, 0, 0, 0, 0);
    zero[n] += 0.0000000001;
    zero[m] += 0.0000000001;
    return zero;
}

std::set<S6> SellaBuild::BoundAndRefl(const size_t n, const S6& s) {
    const std::vector< S6(*)(const S6&)> refl = S6::SetRelectionFunctions();
    const S6 s6(s);
    std::set<S6> out;
    S6 rxxxx = fnRedn[n](s);

    for (size_t i = 0; i < refl.size(); ++i) {
        out.insert(refl[i](rxxxx));
    }
    return out;
}

void SellaBuild::ProcessThreeZeros(const std::string& label, const S6& s6) {
    const std::vector<size_t> vZeros = FindS6Zeros(s6);

    // handle input as if no zeros, just reflections
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
      ProcessTwoZeros(label, s6);
   }
   else {
      ProcessThreeZeros(label, s6);
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

std::vector<size_t> FindNearS6Zeros(const S6& s) {
   std::vector<size_t> v;
   for (size_t i = 0; i < 6; ++i) if (abs(s[i]) < 1.0E-5) v.push_back(i);
   return v;
}

void SellaBuild::ShowIndexResults() const {
   indexstore.ShowResults();
}

std::set<S6>SellaBuild::Xpand1(const std::set<S6>& vsin) {
   std::set<S6> out;
   for (auto it = vsin.begin(); it != vsin.end(); ++it) {
      out = Xpand1(*it);
   }

   return out;
}

std::set<S6>SellaBuild::Xpand2(const std::set<S6>& vsin) {
   std::set<S6> vs;
   for (auto it = vsin.begin(); it != vsin.end(); ++it) {
      const std::vector<size_t> vZeros = FindNearS6Zeros(*it);
      std::set<S6> out = Xpand2(*it);
      vs.insert(out.begin(), out.end());
   }
   return vs;
}

std::set<S6>SellaBuild::Xpand3(const std::set<S6>& vsin) {
   std::set<S6> vs;
   for (auto it = vsin.begin(); it != vsin.end(); ++it) {
      const std::vector<size_t> vZeros = FindNearS6Zeros(*it);
      std::set<S6> out = Xpand3(*it);
      vs.insert(out.begin(), out.end());
   }
   return vs;
}

std::set<S6>SellaBuild::Xpand1(const S6& s) {
   std::set<S6> out;
   for (size_t i = 0; i < g_refls.size(); ++i) {
      out.insert(g_refls[i] * s);
   }
   const std::vector<size_t> vZeros = FindNearS6Zeros(s);
   out.insert(fnRedn[vZeros[0]](s));
   return out;
}

std::set<S6>SellaBuild::Xpand2(const S6& s) {
   std::set<S6> out;
   const std::vector<size_t> vZeros = FindS6Zeros(s);
   return out;
}


std::set<S6>SellaBuild::Xpand3(const S6& s) {
   std::set<S6> out;
   const std::vector<size_t> vZeros = FindS6Zeros(s);
   return out;
}
