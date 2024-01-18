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
static const std::vector< S6(*)(const S6&)> fnMakeVirtualCartPoint = S6Dist::SetVCPFunctions();
static const std::vector<S6(*)(const S6&)> fnRefl = S6::SetRelectionFunctions();

std::set<S6> BoundaryAndAllReflections(const size_t n, const S6& s) {
      const std::set<S6> bAndR = SellaBuild::GenerateAllReflections(fnMakeVirtualCartPoint[n](s));
      return bAndR;
}

std::set<S6> BoundaryAndAllReflections(const size_t n, const std::set<S6>& s) {
   std::set<S6> out;
   for (auto it = s.begin(); it != s.end(); ++it) {
      const std::set<S6> temp = BoundaryAndAllReflections(n, *it);
      out.insert(temp.begin(), temp.end());
   }
   return out;
}

SellaBuild::SellaBuild() {
   store.SetMaxItemStore(20000);
}

std::vector<LabeledDeloneTypeMatrices>  SellaBuild::Build() {
	MatS6 transformations;

   if (vDeloneTypes.empty()) SellaBuild::vDeloneTypes = Delone::LoadLabeledLatticeTypeProjectors();


   for (size_t i = 0; i < vDeloneTypes.size(); ++i) {
      transformations = transformations.unit();
//      Expand(vDeloneTypes[i].first, vDeloneTypes[i].second/*, transformations*/);
      Expand2(vDeloneTypes[i].first, vDeloneTypes[i].second/*, transformations*/);
   }
   store.ShowTableOfKeysVersusCount();
   ProcessItemStoreToVectorMap();
   std::cout << std::endl << std::endl << "after ProcessItemStoreToVectorMap, the map " << xfmap.size() << std::endl;
   for (auto ita = xfmap.begin(); ita != xfmap.end(); ++ita) std::cout << (*ita).first << "  " << (*ita).second.size() << std::endl;
   LabeledDeloneTypeMatrices lsm2;
   const std::vector<LabeledDeloneTypeMatrices> vtypes = lsm2.ProcessVectorMapToPerpsAndProjectors(themap, xfmap);

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


double SellaBuild::TestOneType(const std::string& label, const S6& s6, const std::vector<MatS6>& vm) {
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

void SellaBuild::Store(const std::string& label, const S6_Ordinals& s) {
   store.Store(label, s);
}

void SellaBuild::Store(const std::string& label, const std::set<S6_Ordinals>& s) {
   for (auto i = s.begin(); i != s.end(); ++i) {
      Store(label, *i);
   }
}

void SellaBuild::StoreAllReflections(const std::string& label, const S6_Ordinals& s1in) {
   S6_Ordinals s1(s1in);

   for (size_t i = 0; i < fnRefl.size(); ++i) {
      store.Store(label, fnRefl[i](s1));
   }
}

void SellaBuild::StoreAllReflections(const std::string& label, const std::set<S6_Ordinals>& s) {
   for (std::set<S6_Ordinals>::const_iterator it = s.begin(); it != s.end(); ++it) {
      StoreAllReflections(label, *it);
   }
}

void SellaBuild::StoreAllReflections(const std::string& label, const S6& s) {
   S6_Ordinals s1(s);
   for (size_t i = 0; i < fnRefl.size(); ++i) {
      store.Store(label, fnRefl[i](s1));
   }
}

void SellaBuild::StoreAllReflections(const std::string& label, const std::set<S6>& s) {
   for (auto it = s.begin(); it != s.end(); ++it) {
      StoreAllReflections(label, *it);
   }
}


std::set<S6> SellaBuild::GenerateAllReflections(const S6& s1in) {
   std::set<S6> setS6;
   for (size_t i = 0; i < fnRefl.size(); ++i) {
      setS6.insert(fnRefl[i](s1in));
   }
   return setS6;
}

std::set<S6> SellaBuild::GenerateAllReflections(const std::set<S6>& s) {
   std::set<S6> setS6;
   std::set<S6> xxxx;
   for (std::set<S6>::const_iterator it = s.begin(); it != s.end(); ++it) {
      xxxx = GenerateAllReflections(*it);
   }
   setS6.insert(xxxx.begin(), xxxx.end());
   return setS6;
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
      const auto zeroIT = std::find(vZeros.begin(), vZeros.end(), *it);
      if (zeroIT != vZeros.end()) vZeros.erase(zeroIT);
   }
   return vZeros;
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

std::set<S6> SetPos(const size_t n, const std::set<S6>& s6) {
   std::set<S6> out;
   int i19191 = 0;
   for (auto it = s6.begin(); it != s6.end(); ++it)
   {
      ++i19191;
      const S6 temp = SetPos(n, *it);
      //std::cout << temp << std::endl;
      out.insert(temp);
   }
   return out;
}

S6 SetPos(const size_t n1, const size_t n2, const S6& s6) {
   S6 out(s6);
   out[n1] += 0.0000000001;
   out[n2] += 0.0000000001;
   return out;
}

void SellaBuild::Expand(const std::string& label, const MatS6& fromCanon/*, MatS6 transformations*/) {
   const S6_Ordinals s6 = MakeSampleType(fromCanon);
   MatN mn(36);
   mn.SetVector(fromCanon.GetVector());
   //const MatS6 toCanon = mn.inverse();
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

void SellaBuild:: Expand2(const std::string& label, const MatS6& fromCanon) {
   const S6_Ordinals s6 = MakeSampleType(fromCanon);
   MatN mn(36);
   mn.SetVector(fromCanon.GetVector());
   //const MatS6 toCanon = mn.inverse();
   const std::set<S6> out =Xpand3(label, s6, fromCanon);
   StoreAllReflections(label, out);
}

void SellaBuild::OneBound(const std::string& label, const S6_Ordinals& s1,
   const size_t zeroBound)
{
   const S6_Ordinals s6temp = fnMakeVirtualCartPoint[zeroBound](s1);
   StoreAllReflections(label, s6temp);
}

void SellaBuild::ProcessTwoZeros(const std::string& label, const S6& s6) {
    const std::vector<size_t> vZeros = FindS6Zeros(s6);

   // handle input as if no zeros, just reflections
   StoreAllReflections(label, s6);

   // Process each zero by itself
   OneBound(label, s6, vZeros[0]);
   OneBound(label, s6, vZeros[1]);
   
   const S6 red1 = SetPos(vZeros[0],fnMakeVirtualCartPoint[vZeros[0]](s6)); //LCA this is the error point!!!
   // We don't really know where vZeros[1] went in red1
   const S6 red2 = SetPos(vZeros[1],fnMakeVirtualCartPoint[vZeros[1]](s6));

   const std::vector<size_t> vZeros1 = FindS6Zeros(red1);
   const std::vector<size_t> vZeros2 = FindS6Zeros(red2);

   const S6 red12 = fnMakeVirtualCartPoint[vZeros1[0]](red1);
   const S6 red21 = fnMakeVirtualCartPoint[vZeros2[0]](red2);

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
    const S6 s6(s);
    std::set<S6> out;
    S6 rxxxx = fnMakeVirtualCartPoint[n](s);

    for (size_t i = 0; i < fnRefl.size(); ++i) {
        out.insert(fnRefl[i](rxxxx));
    }
    return out;
}

void SellaBuild::ProcessThreeZeros(const std::string& label, const S6& s6) {
   const std::vector<size_t> vZeros = FindS6Zeros(s6);

   // handle input as if no zeros, just reflections
   StoreAllReflections(label, s6);

   // treat each separate bound
   const S6 red_0 = fnMakeVirtualCartPoint[vZeros[0]](s6);
   const S6 red_1 = fnMakeVirtualCartPoint[vZeros[1]](s6);
   const S6 red_2 = fnMakeVirtualCartPoint[vZeros[2]](s6);

   StoreAllReflections(label, red_0);
   StoreAllReflections(label, red_1);
   StoreAllReflections(label, red_2);


   // treat each pair of zeros
   const S6 red_1_0 = fnMakeVirtualCartPoint[vZeros[1]](red_0);
   const S6 red_2_0 = fnMakeVirtualCartPoint[vZeros[2]](red_0);
   const S6 red_0_1 = fnMakeVirtualCartPoint[vZeros[0]](red_1);
   const S6 red_2_1 = fnMakeVirtualCartPoint[vZeros[2]](red_1);
   const S6 red_0_2 = fnMakeVirtualCartPoint[vZeros[0]](red_2);
   const S6 red_1_2 = fnMakeVirtualCartPoint[vZeros[1]](red_2);

   StoreAllReflections(label, red_1_0);
   StoreAllReflections(label, red_2_0);
   StoreAllReflections(label, red_0_1);
   StoreAllReflections(label, red_2_1);
   StoreAllReflections(label, red_0_2);
   StoreAllReflections(label, red_1_2);

   // now process all three zeros
   const S6 red_0_1_2 = fnMakeVirtualCartPoint[vZeros[0]](red_1_2);
   const S6 red_0_2_1 = fnMakeVirtualCartPoint[vZeros[0]](red_2_1);
   const S6 red_1_0_2 = fnMakeVirtualCartPoint[vZeros[1]](red_0_2);
   const S6 red_1_2_0 = fnMakeVirtualCartPoint[vZeros[1]](red_2_0);
   const S6 red_2_0_1 = fnMakeVirtualCartPoint[vZeros[2]](red_0_1);
   const S6 red_2_1_0 = fnMakeVirtualCartPoint[vZeros[2]](red_1_0);

   StoreAllReflections(label, red_0_1_2);
   StoreAllReflections(label, red_0_2_1);
   StoreAllReflections(label, red_1_0_2);
   StoreAllReflections(label, red_1_2_0);
   StoreAllReflections(label, red_2_0_1);
   StoreAllReflections(label, red_2_1_0);

}

void SellaBuild::ProcessZeros( const std::string& label, const S6_Ordinals& s6 ) {
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


bool SellaBuild::FindDuplicate(const std::vector<S6>& out, const S6& s6) {
   bool fail = false;
   if (!out.empty()) {
      for (size_t o = 0; o < out.size(); ++o) {
         const double d = (out[o] - s6).norm();
         if ((out[o] - s6).norm() < 1.0E-6) fail = true;
      }
   }

   return fail;
}

std::vector<Transformations> ConvertS6ToTrans(const std::vector<S6>& v) {
   std::vector<Transformations> out;
   for (size_t i = 0; i < v.size(); ++i) out.push_back(Transformations(v[i]));
   return out;
}

template<typename T>
void PrintMap(const std::map<std::string, std::vector<T> >& t) {
   for (auto it = t.begin(); it != t.end(); ++it) {
      std::cout << (*it).first << "  " << (*it).second[0] << std::endl;
   }
}

void SellaBuild::ProcessItemStoreToVectorMap() {
   std::vector<std::string> labels = store.GetKeys();
   for (size_t i = 0; i < labels.size(); ++i) {
      std::vector<S6> out;
      const std::vector<std::pair<std::string, S6> > v = store.GetResult(labels[i]);
      out.push_back(v[0].second);
      for (size_t iv = 1; iv < v.size(); ++iv) {
         const bool fail = FindDuplicate(out, v[iv].second);
         if (!fail) out.push_back(v[iv].second);
      }
      //themap.insert(std::make_pair(labels[i], out));
      xfmap.insert(std::make_pair(labels[i], ConvertS6ToTrans(out)));
   }
}

std::vector<size_t> SellaBuild::FindS6Zeros(const S6& s) {
      std::vector<size_t> v;
   for (size_t i = 0; i < 6; ++i) if (s[i] == 0.0) v.push_back(i);
   return v;
}

std::vector<size_t> SellaBuild::FindNearS6Zeros(const S6& s) {
   std::vector<size_t> v;
   for (size_t i = 0; i < 6; ++i) if (abs(s[i]) < 1.0E-5) v.push_back(i);
   return v;
}

void SellaBuild::ShowIndexResults() const {
   indexstore.ShowResults();
}

S6 ResetZeros(const S6& sin) {
   S6 sout(sin);
   for (size_t i = 0; i < 6; ++i) if (abs(sin[i]) < 1.0E-3) sout[i] = 0.0;
   return sout;
}

S6 Rounder(const S6& s6) {
   S6 out(s6);
   for (size_t i = 0; i < 6; ++i) out[i] = round(out[i]);
   return out;
}

std::set<S6> Rounder(const std::set<S6>& ss) {
   std::set<S6> out;
   for (auto it = ss.begin(); it != ss.end(); ++it) {
      out.insert(Rounder(*it));
   }
   return out;
}

std::set<S6> SellaBuild::Xpand1(const std::string& label, const S6& s, const MatS6& toCanon) {
   std::set<S6> out = GenerateAllReflections(s);
   const std::vector<size_t> vZeros = FindNearS6Zeros(s);
   if (vZeros.empty()) return out;

   const std::set<S6> out1 = GenerateAllReflections(Rounder(BoundaryAndAllReflections(vZeros[0], s)));
   out.insert(out1.begin(), out1.end());

   //for (auto it = out.begin(); it != out.end(); ++it) std::cout << *it << std::endl;
   return out;
}

std::set<S6> SellaBuild::Xpand1(const std::string& label, const std::set<S6>& vsin, const MatS6& toCanon) {
   std::set<S6> out;
   for (auto it = vsin.begin(); it != vsin.end(); ++it) {
      out = Xpand1(label, ResetZeros(*it), toCanon);
   }
   return out;
}

std::set<S6> SellaBuild::Xpand2(const std::string& label, const S6& s, const MatS6& toCanon) {
   const std::vector<size_t> vZeros = FindS6Zeros(s);
   if (vZeros.size() < 2) {
      return Xpand1(label, s, toCanon);
   }
   const S6 s6temp0 = Rounder(fnMakeVirtualCartPoint[vZeros[0]](s));
   const S6 s6temp1 = Rounder(fnMakeVirtualCartPoint[vZeros[1]](s));

   const std::set<S6>  pluss6temp0 = Xpand1(label, SetPos(0,s6temp0), toCanon);
   const std::set<S6>  pluss6temp1 = Xpand1(label, SetPos(1,s6temp1), toCanon);

   const std::set<S6> setRefl1  = GenerateAllReflections(s6temp0);
   const std::set<S6> setRefl2  = GenerateAllReflections(s6temp1);
   const std::set<S6> setRefl3  = GenerateAllReflections(pluss6temp0);
   const std::set<S6> setRefl4  = GenerateAllReflections(pluss6temp1);

   std::set<S6> out;
   out.insert(s6temp0);
   out.insert(s6temp1);
   out.insert(pluss6temp0.begin(), pluss6temp0.end());
   out.insert(pluss6temp1.begin(), pluss6temp1.end());
   out.insert(setRefl1.begin(), setRefl1.end());
   out.insert(setRefl2.begin(), setRefl2.end());
   out.insert(setRefl3.begin(), setRefl3.end());
   out.insert(setRefl4.begin(), setRefl4.end());

   //std::cout << "at the end of Xpand2" << std::endl;
   //const int i19191 = 19191;
   //for (auto it = out.begin(); it != out.end(); ++it) std::cout << *it << std::endl;

   return out;
}

std::set<S6> SellaBuild::Xpand2(const std::string& label, const std::set<S6>& vsin, const MatS6& toCanon) {
   std::set<S6> vs;
   for (auto it = vsin.begin(); it != vsin.end(); ++it) {
      std::set<S6> out = Xpand2(label, ResetZeros(*it), toCanon);
      vs.insert(out.begin(), out.end());
   }
   return vs;
}

std::set<S6> SellaBuild::Xpand3(const std::string& label, const S6& s, const MatS6& toCanon) {
   std::set<S6> out;
   const std::vector<size_t> vZeros = FindS6Zeros(s);
   if (vZeros.size() < 3) {
      return Xpand2(label, s, toCanon);
   }

   const S6 s6temp0 = Rounder(fnMakeVirtualCartPoint[vZeros[0]](s));
   const S6 s6temp1 = Rounder(fnMakeVirtualCartPoint[vZeros[1]](s));
   const S6 s6temp2 = Rounder(fnMakeVirtualCartPoint[vZeros[2]](s));

   const std::set<S6>  pluss6temp0 = Rounder(Xpand2(label, SetPos(0, s6temp0), toCanon));
   const std::set<S6>  pluss6temp1 = Rounder(Xpand2(label, SetPos(1, s6temp1), toCanon));
   const std::set<S6>  pluss6temp2 = Rounder(Xpand2(label, SetPos(2, s6temp2), toCanon));

   const std::set<S6> setRefl1 =Rounder(GenerateAllReflections(SetPos(0, s6temp0)));
   const std::set<S6> setRefl2 =Rounder(GenerateAllReflections(SetPos(1, s6temp1)));
   const std::set<S6> setRefl3 =Rounder(GenerateAllReflections(SetPos(2, s6temp2)));
   const std::set<S6> setRefl4 =Rounder(GenerateAllReflections(SetPos(0, pluss6temp0)));
   const std::set<S6> setRefl5 =Rounder(GenerateAllReflections(SetPos(1, pluss6temp1)));
   const std::set<S6> setRefl6 =Rounder(GenerateAllReflections(SetPos(2, pluss6temp2)));

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

std::set<S6> SellaBuild::Xpand3(const std::string& label, const std::set<S6>& vsin, const MatS6& toCanon) {
   std::set<S6> out;
   for (auto it = vsin.begin(); it != vsin.end(); ++it) {
      const std::set<S6> vs = Rounder(Xpand3(label, ResetZeros(*it), toCanon));
      out.insert(vs.begin(), vs.end());
   }
   return out;
}

