#include "SellaBuild.h"


#include "GenerateRandomLattice.h"
#include "LRL_ToString.h"
#include "LRL_MaximaTools.h"
#include "S6Dist.h"
#include "Selling.h"
#include "StoreResults.h"

std::vector< std::pair<std::string, MatS6> > SellaBuild::vDeloneTypes = Delone::LoadLabeledLatticeTypeProjectors();


SellaBuild::SellaBuild() {
   store.SetMaxItemStore(350);

   for (unsigned long i = 17; i < 18/*vDeloneTypes.size()*/; ++i) {
      //for (unsigned long i = 2; i < 3; ++i) {  // to show only C5
      Expand(vDeloneTypes[i].first, vDeloneTypes[i].second);
   }
   store.ShowTableOfKeysVersusCount();
   ProcessItemStoreToVectorMap();
   std::cout << std::endl << std::endl << "after ProcessItemStoreToVectorMap, the map " << themap.size() << std::endl;
   for (auto ita = themap.begin(); ita != themap.end(); ++ita) std::cout << (*ita).first << "  " << (*ita).second.size() << std::endl;
   ProcessVectorMapToPerpsAndProjectors();
   std::cout << std::endl << std::endl << "after ProcessVectorMapToPerpsAndProjectors, the vperps " << perps.size() << std::endl;
   for (auto ita = perps.begin(); ita != perps.end(); ++ita) std::cout << (*ita).GetLabel() << "  " << (*ita).size() << std::endl;
   //store.ShowResults();
}

static int seed = 19191;

S6 SellaBuild::MakeSampleType(const MatS6& m) {
   GenerateRandomLattice<S6> generate(seed);
   bool valid = false;
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
   S6 out;
   std::vector<std::pair<std::string, double> > v;
   const double s6norm = s6.norm();
   if (!s6.IsValid() && s6norm > 1.0E-4) return v;

   const bool b = Selling::Reduce(s6, out);
   if (b) {
      const double n1 = s6.norm();
      const double n2 = out.norm();
      for (unsigned long i = 0; i < perps.size(); ++i) {
         const std::string label = perps[i].GetLabel();
         const double best = TestOneType(label, out, perps[i].GetMatrices());
         v.push_back(std::make_pair(label, best));
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

void SellaBuild::ProcessVectorMapToPerpsAndProjectors() {
   const MatS6 unit = MatS6().unit();
   for ( auto it = themap.begin(); it != themap.end(); ++it) {
      const std::pair<std::string, std::vector<S6> > p = *it;
      std::vector<MatS6> vmprj;
      std::vector<MatS6> vmprp;
      const std::string label(p.first);
      for (unsigned long i = 0; i < p.second.size(); ++i) {
         vmprj.push_back(ProjectorFromVector(label, p.second[i]));
         vmprp.push_back(unit - ProjectorFromVector(label, p.second[i]));
      }

      const std::vector<MatS6> tempprj = RemoveForDuplicates(vmprj);
      const LabeledSellaMatrices lsmprj(label, tempprj);
      projectors.push_back(lsmprj);

      const std::vector<MatS6> tempprp = RemoveForDuplicates(vmprp);
      const LabeledSellaMatrices lsm(label, tempprp);
      perps.push_back(lsm);
   }
}

void SellaBuild::Expand(const std::string& label, const MatS6& m) {
   const S6 s6 = MakeSampleType(m);

   if (s6.IsValid()) {
      switch (s6.CountZeros()) {
      case 0:
         StoreAllReflections(label, s6);
         break;
      case 1:
         OneBound(label, s6);
         break;
      case 2:
      case 3:
         ProcessZeros(label, s6);
         break;
      default:
         throw "this should never happen";
         break;
      }
   }
   //store.ShowResults();
}

bool SellaBuild::FindDuplicate(const std::vector<S6>& out, const S6 s6) {
   bool fail = false;
   if (!out.empty()) {
      for (unsigned long o = 0; o < out.size(); ++o) {
         if ((out[o] - s6).norm() < 1.0E-6) fail = true;
      }
   }

   return fail;
}

void SellaBuild::ProcessItemStoreToVectorMap() {
   std::vector<std::string> labels = store.GetKeys();
   for (unsigned long i = 0; i < labels.size(); ++i) {
      std::vector<S6> out;
      const std::vector<std::pair<std::string, S6> > v = store.GetResult(labels[i]);
      out.push_back(v[0].second);
      for (unsigned long iv = 0; iv < v.size(); ++iv) {
         const bool fail = FindDuplicate(out, v[iv].second);
         if (!fail) out.push_back(v[iv].second);
      }
      themap.insert(std::make_pair(labels[i], out));
   }
}



unsigned long SellaBuild::Index6(const unsigned long i, const unsigned long j) {
   return 6 * i + j % 6;
}

unsigned long CountEqualButNonZero(const double d, const S6& s) {
   unsigned long count = 0;
   for (unsigned long i = 0; i < 6; ++i) {
      if (abs(s[i] - d) < 1.0E-6 && abs(d) > 1.0E-6) ++count;
   }
   return count;
}

double SellaBuild::Fraction(const double d, const S6& s) {
   const double count = CountEqualButNonZero(d, s);
   return (count == 0.0) ? 0.0 : 1.0 / (double)(count);
}

S6 ReZeroScalars(const S6& s) {
   S6 s6(s);
   for (unsigned long i = 0; i < 6; ++i)
      if (abs(s6[i]) < 1.0E-6) s6[i] = 0.0;
   return s6;
}

MatS6 SellaBuild::ProjectorFromVector(const std::string& label, const S6& s6in) {
   MatS6 m;
   const S6 s = ReZeroScalars(s6in);

   for (unsigned long j = 0; j < 6; ++j) {
      for (unsigned long k = j+1; k < 6; ++k) {
         const double thisFrac = Fraction(s[k], s);
         m[Index6(j, k)] = (abs(s[j] - s[k]) < 1.0E-5 && abs(s[k]) > 1.0E-5) ? thisFrac : 0.0;
         m[Index6(k, j)] = m[Index6(j, k)];
      }
   }

   for (unsigned long k = 0; k < 6; ++k) {
      const double thisFrac = Fraction(s[k], s);
      m[Index6(k, k)] = (thisFrac == 0.0 || abs(s[k]) < 1.0E-6) ? 0.0 : thisFrac;
   }

   return m;
}

std::vector<unsigned long> SellaBuild::FindS6Zeros(const S6& s) {
   std::vector<unsigned long> v;
   for (unsigned long i = 0; i < 6; ++i) if (s[i] == 0.0) v.push_back(i);
   return v;
}

void SellaBuild::StoreAllReflections(const std::string& label, const S6& s1) {
   std::vector< S6(*)(const S6&)> refl = S6::SetRelectionFunctions();

   for (unsigned long i = 0; i < refl.size(); ++i) {
      store.Store(label, refl[i](s1));
   }
   //store.ShowResults();
}

void SellaBuild::OneBound(const std::string& label, const S6& s1) {
   static const std::vector< S6(*)(const S6&)> fnRedn = S6Dist::SetVCPFunctions();

   unsigned long nzero = 0;
   for (unsigned long i = 0; i < 6; ++i) if (s1[i] == 0) nzero = i;
   StoreAllReflections(label, s1);
   const S6 s6temp = fnRedn[nzero](s1);
   StoreAllReflections(label, s6temp);
}

void SellaBuild::ProcessZeros(const std::string& label, const S6& s6) {
   const std::vector<unsigned long> v = FindS6Zeros(s6);
   const unsigned long nzeros = v.size();

   if (nzeros == 0) {
      return;
   }
   else if (nzeros == 1) {
      OneBound(label, s6);
   }
   else {
      S6 s6a(s6);
      S6 s6b(s6);
      S6 s6c(s6);
      for (unsigned long i = 0; i < v.size(); ++i) {
         S6 temp(s6);
         temp[v[i]] = DBL_MIN;
         ProcessZeros(label, temp);
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

void SellaBuild::WriteSellaMatrices(const std::string& functionName, const std::vector<LabeledSellaMatrices>& mat) const {

   std::cout << "std::vector<LabeledSellaMatrices> " << functionName << "() {" << std::endl;

   std::cout << "static double prp[268][36]= { " << std::endl;

   for (unsigned long j = 0; j < mat.size(); ++j) {
      const LabeledSellaMatrices& v = mat[j];
      //WriteOnePerp( "firstfunc", v.GetLabel(), v.GetMatrices());



      std::cout << "/*  " << v.GetLabel() << "  */" << std::endl;
      std::vector<MatS6> vm = v.GetMatrices();

      for (unsigned long k = 0; k < vm.size(); ++k) {
         std::cout << "   vm.push_back(MatS6(";

         for (unsigned long ll = 0; ll < vm[k].size(); ++ll) {
            std::cout << vm[k][ll];
            const unsigned long ssss = vm[k].size() - 1;
            std::cout << ((ll < ssss) ? "," : "));");
         }
         std::cout << ((j < mat.size() - 1 || k < vm.size() - 1) ? "," : "") << std::endl;
      }
      std::cout << std::endl << std::endl;
   }

   std::cout << "   };" << std::endl;
   std::cout << "}   // end of " << functionName << std::endl << std::endl;
}

LabeledSellaMatrices SellaBuild::CreatePerps_H4() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.666667, 0, -0.333333, 0, -0.333333, 0, 0, 1, 0, 0, 0, 0, -0.333333, 0, 0.666667, 0, -0.333333, 0, 0, 0, 0, 0, 0, 0, -0.333333, 0, -0.333333, 0, 0.666667));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.666667, 0, -0.333333, 0, -0.333333, 0, 0, 1, 0, 0, 0, 0, -0.333333, 0, 0.666667, 0, -0.333333, 0, 0, 0, 0, 1, 0, 0, -0.333333, 0, -0.333333, 0, 0.666667));
   vm.push_back(MatS6(0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.666667, 0, 0, 0, -0.333333, -0.333333, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.333333, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667));
   vm.push_back(MatS6(0.666667, 0, 0, 0, -0.333333, -0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.333333, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667));
   vm.push_back(MatS6(0.666667, 0, 0, 0, -0.333333, -0.333333, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.333333, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667));
   vm.push_back(MatS6(0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.666667, 0, -0.333333, 0, -0.333333, 0, 0, 0, 0, 0, 0, 0, -0.333333, 0, 0.666667, 0, -0.333333, 0, 0, 0, 0, 1, 0, 0, -0.333333, 0, -0.333333, 0, 0.666667));
   return LabeledSellaMatrices("H4", vm);
}

LabeledSellaMatrices SellaBuild::CreatePerps_K1() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.833333, -0.166667, -0.166667, -0.166667, -0.166667, -0.166667, -0.166667, 0.833333, -0.166667, -0.166667, -0.166667, -0.166667, -0.166667, -0.166667, 0.833333, -0.166667, -0.166667, -0.166667, -0.166667, -0.166667, -0.166667, 0.833333, -0.166667, -0.166667, -0.166667, -0.166667, -0.166667, -0.166667, 0.833333, -0.166667, -0.166667, -0.166667, -0.166667, -0.166667, -0.166667, 0.833333));
   return LabeledSellaMatrices("C1", vm);
}


LabeledSellaMatrices SellaBuild::CreatePerps_K3() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, -0.25, 0, 0, 0, 1, 0, 0, 0, -0.25, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.75, 0, -0.25, -0.25, 0, -0.25, 0, 1, 0, 0, 0, 0, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, 0, 0, 0, 0, 1, 0, -0.25, 0, -0.25, -0.25, 0, 0.75));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, -0.25, 0, 0, 0, 1, 0, 0, 0, -0.25, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75));
   return LabeledSellaMatrices("C3", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_K5() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667));
   vm.push_back(MatS6(0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, -0.333333, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, -0.333333, -0.333333, 0, 0, 0, 0.666667));
   vm.push_back(MatS6(0.666667, 0, -0.333333, 0, -0.333333, 0, 0, 1, 0, 0, 0, 0, -0.333333, 0, 0.666667, 0, -0.333333, 0, 0, 0, 0, 1, 0, 0, -0.333333, 0, -0.333333, 0, 0.666667, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.666667, 0, 0, -0.333333, -0.333333, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.333333, 0, 0, 0.666667, -0.333333, 0, -0.333333, 0, 0, -0.333333, 0.666667));
   vm.push_back(MatS6(0.666667, 0, 0, -0.333333, 0, -0.333333, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.333333, 0, 0, 0.666667, 0, -0.333333, 0, 0, 0, 0, 1, 0, -0.333333, 0, 0, -0.333333, 0, 0.666667));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.666667, 0, -0.333333, -0.333333, 0, 0, 0, 1, 0, 0, 0, 0, -0.333333, 0, 0.666667, -0.333333, 0, 0, -0.333333, 0, -0.333333, 0.666667));
   vm.push_back(MatS6(0.666667, 0, -0.333333, 0, 0, -0.333333, 0, 1, 0, 0, 0, 0, -0.333333, 0, 0.666667, 0, 0, -0.333333, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, -0.333333, 0, -0.333333, 0, 0, 0.666667));
   vm.push_back(MatS6(0.666667, 0, 0, -0.333333, -0.333333, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.333333, 0, 0, 0.666667, -0.333333, 0, -0.333333, 0, 0, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.666667, -0.333333, 0, 0, -0.333333, 0, -0.333333, 0.666667, 0, 0, -0.333333, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.333333, -0.333333, 0, 0, 0.666667, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, 0, -0.333333, 0, 0, -0.333333, 0.666667, 0, -0.333333, 0, 0, 0, 0, 1, 0, 0, 0, -0.333333, -0.333333, 0, 0.666667));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, 0, 0, -0.333333, 0, -0.333333, 0.666667, 0, 0, -0.333333, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.333333, -0.333333, 0, 0, 0.666667));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.666667, 0, -0.333333, -0.333333, 0, 0, 0, 1, 0, 0, 0, 0, -0.333333, 0, 0.666667, -0.333333, 0, 0, -0.333333, 0, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.666667, -0.333333, 0, -0.333333, 0, 0, -0.333333, 0.666667, 0, -0.333333, 0, 0, 0, 0, 1, 0, 0, 0, -0.333333, -0.333333, 0, 0.666667, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, 0, -0.333333, 0, 0, -0.333333, 0.666667, 0, -0.333333, 0, 0, 0, 0, 1, 0, 0, 0, -0.333333, -0.333333, 0, 0.666667, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.666667, 0, -0.333333, -0.333333, 0, 0, 0, 1, 0, 0, 0, 0, -0.333333, 0, 0.666667, -0.333333, 0, 0, -0.333333, 0, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   return LabeledSellaMatrices("C5", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_M1A() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, -0.5, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   return LabeledSellaMatrices("M1A", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_M1B() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0.5));
   return LabeledSellaMatrices("M1B", vm);
}


LabeledSellaMatrices SellaBuild::CreatePerps_M2B() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, -0.5, 0, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, -0.5, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   return LabeledSellaMatrices("M2B", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_M2A() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, 0, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0, -0.5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5));
   vm.push_back(MatS6(0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1));
   return LabeledSellaMatrices("M2A", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_M3() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, 0, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0, -0.5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, -0.5, 0, 0, 0, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5));
   vm.push_back(MatS6(0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1));
   return LabeledSellaMatrices("M3", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_M4() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   return LabeledSellaMatrices("M4", vm);
}


LabeledSellaMatrices SellaBuild::CreatePerps_O1A() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, -0.25, 0, 0, 0, 0, 0, 0, 0, -0.25, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.75, 0, -0.25, -0.25, 0, -0.25, 0, 0, 0, 0, 0, 0, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, 0, 0, 0, 0, 0, 0, -0.25, 0, -0.25, -0.25, 0, 0.75));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, -0.25, 0, 0, 0, 0, 0, 0, 0, -0.25, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75));
   return LabeledSellaMatrices("O1A", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_O1B() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, -0.25, 0, 0, 0, 0.5, 0, 0, -0.5, -0.25, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, 0, 0, -0.5, 0, 0, 0.5));
   vm.push_back(MatS6(0.75, 0, -0.25, -0.25, 0, -0.25, 0, 0.5, 0, 0, -0.5, 0, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, 0, -0.5, 0, 0, 0.5, 0, -0.25, 0, -0.25, -0.25, 0, 0.75));
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, -0.25, -0.5, 0, 0, 0.5, 0, 0, 0, -0.25, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75));
   return LabeledSellaMatrices("O1B", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_O2() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.5, 0, 0, 0, -0.5, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0, -0.5, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5));
   vm.push_back(MatS6(0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1));
   return LabeledSellaMatrices("O2", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_O3() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, -0.5, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0, -0.5, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, -0.5, 0, 0, 0, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5));
   vm.push_back(MatS6(0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1));
   return LabeledSellaMatrices("O3", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_O4() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.5, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, -0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5));
   vm.push_back(MatS6(0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0, -0.5, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, -0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, 0, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0, -0.5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.5, 0, 0, 0, 0.5));
   return LabeledSellaMatrices("O4", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_O5() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   return LabeledSellaMatrices("O5", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_T1() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, -0.25, 0, 0, 0, 0.5, 0, 0, -0.5, -0.25, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, 0, 0, -0.5, 0, 0, 0.5));
   vm.push_back(MatS6(0.75, 0, -0.25, -0.25, 0, -0.25, 0, 0.5, 0, 0, -0.5, 0, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, 0, -0.5, 0, 0, 0.5, 0, -0.25, 0, -0.25, -0.25, 0, 0.75));
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, -0.25, -0.5, 0, 0, 0.5, 0, 0, 0, -0.25, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75));
   return LabeledSellaMatrices("T1", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_T2() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, -0.25, 0, 0, 0, 1, 0, 0, 0, -0.25, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.75, 0, -0.25, -0.25, 0, -0.25, 0, 1, 0, 0, 0, 0, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, 0, 0, 0, 0, 0, 0, -0.25, 0, -0.25, -0.25, 0, 0.75));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, -0.25, 0, 0, 0, 1, 0, 0, 0, -0.25, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, -0.25, 0, 0, 0, 0, 0, 0, 0, -0.25, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75));
   vm.push_back(MatS6(0.75, 0, -0.25, -0.25, 0, -0.25, 0, 0, 0, 0, 0, 0, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, 0, 0, 0, 0, 1, 0, -0.25, 0, -0.25, -0.25, 0, 0.75));
   vm.push_back(MatS6(0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, -0.25, -0.25, 0, 0, 0, 0, 0, 0, 0, -0.25, -0.25, 0, 0.75, -0.25, 0, -0.25, -0.25, 0, -0.25, 0.75, 0, 0, 0, 0, 0, 0, 1));
   return LabeledSellaMatrices("T2", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_T5() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.5, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, -0.5, 0, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, -0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, 0, 0, -0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5));
   vm.push_back(MatS6(0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0, -0.5, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, -0.5, 0, 0, 0, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, -0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, 0, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0, -0.5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, -0.5, 0, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, -0.5, 0, 0, 0, 0, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, -0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, -0.5, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.5, 0, 0, 0, 0.5));
   return LabeledSellaMatrices("T5", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_R1() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667));
   vm.push_back(MatS6(0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, -0.333333, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, -0.333333, -0.333333, 0, 0, 0, 0.666667));
   vm.push_back(MatS6(0.666667, 0, -0.333333, 0, -0.333333, 0, 0, 0.666667, 0, -0.333333, 0, -0.333333, -0.333333, 0, 0.666667, 0, -0.333333, 0, 0, -0.333333, 0, 0.666667, 0, -0.333333, -0.333333, 0, -0.333333, 0, 0.666667, 0, 0, -0.333333, 0, -0.333333, 0, 0.666667));
   vm.push_back(MatS6(0.666667, 0, 0, 0, -0.333333, -0.333333, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, -0.333333, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667));
   return LabeledSellaMatrices("R1", vm);
}



LabeledSellaMatrices SellaBuild::CreatePerps_R3() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.666667, 0, -0.333333, 0, -0.333333, 0, 0, 1, 0, 0, 0, 0, -0.333333, 0, 0.666667, 0, -0.333333, 0, 0, 0, 0, 0, 0, 0, -0.333333, 0, -0.333333, 0, 0.666667, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.666667, 0, -0.333333, 0, -0.333333, 0, 0, 0, 0, 0, 0, 0, -0.333333, 0, 0.666667, 0, -0.333333, 0, 0, 0, 0, 1, 0, 0, -0.333333, 0, -0.333333, 0, 0.666667, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, -0.333333, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, -0.333333, -0.333333, 0, 0, 0, 0.666667));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667));
   vm.push_back(MatS6(0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, -0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, -0.333333, -0.333333, 0, 0, 0, 0.666667));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667, 0, 0, 0, -0.333333, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, -0.333333, -0.333333, 0, 0, 0, 0.666667));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, -0.333333, 0, 0, 0, -0.333333, 0.666667, -0.333333, 0, 0, 0, -0.333333, -0.333333, 0.666667));
   vm.push_back(MatS6(0.666667, 0, -0.333333, 0, -0.333333, 0, 0, 1, 0, 0, 0, 0, -0.333333, 0, 0.666667, 0, -0.333333, 0, 0, 0, 0, 1, 0, 0, -0.333333, 0, -0.333333, 0, 0.666667, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.666667, 0, -0.333333, -0.333333, 0, 0, 0, 1, 0, 0, 0, 0, -0.333333, 0, 0.666667, -0.333333, 0, 0, -0.333333, 0, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, 0, -0.333333, 0, 0, -0.333333, 0.666667, 0, -0.333333, 0, 0, 0, 0, 1, 0, 0, 0, -0.333333, -0.333333, 0, 0.666667, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.666667, -0.333333, 0, -0.333333, 0, 0, -0.333333, 0.666667, 0, -0.333333, 0, 0, 0, 0, 1, 0, 0, 0, -0.333333, -0.333333, 0, 0.666667, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.666667, 0, -0.333333, -0.333333, 0, 0, 0, 1, 0, 0, 0, 0, -0.333333, 0, 0.666667, -0.333333, 0, 0, -0.333333, 0, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, 0, 0, -0.333333, 0, -0.333333, 0.666667, 0, 0, -0.333333, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.333333, -0.333333, 0, 0, 0.666667));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, 0, -0.333333, 0, 0, -0.333333, 0.666667, 0, -0.333333, 0, 0, 0, 0, 0, 0, 0, 0, -0.333333, -0.333333, 0, 0.666667));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, 0, -0.333333, 0, 0, -0.333333, 0.666667, 0, -0.333333, 0, 0, 0, 0, 0, 0, 0, 0, -0.333333, -0.333333, 0, 0.666667, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.666667, 0, -0.333333, -0.333333, 0, 0, 0, 0, 0, 0, 0, 0, -0.333333, 0, 0.666667, -0.333333, 0, 0, -0.333333, 0, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, 0, 0, -0.333333, 0, -0.333333, 0.666667, 0, 0, -0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.333333, -0.333333, 0, 0, 0.666667));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.666667, -0.333333, 0, -0.333333, 0, 0, -0.333333, 0.666667, 0, -0.333333, 0, 0, 0, 0, 1, 0, 0, 0, -0.333333, -0.333333, 0, 0.666667));
   vm.push_back(MatS6(0.666667, -0.333333, 0, -0.333333, 0, 0, -0.333333, 0.666667, 0, -0.333333, 0, 0, 0, 0, 0, 0, 0, 0, -0.333333, -0.333333, 0, 0.666667, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.666667, 0, -0.333333, -0.333333, 0, 0, 0, 0, 0, 0, 0, 0, -0.333333, 0, 0.666667, -0.333333, 0, 0, -0.333333, 0, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.666667, -0.333333, 0, 0, -0.333333, 0, -0.333333, 0.666667, 0, 0, -0.333333, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.333333, -0.333333, 0, 0, 0.666667, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.666667, 0, 0, -0.333333, -0.333333, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.333333, 0, 0, 0.666667, -0.333333, 0, -0.333333, 0, 0, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.666667, 0, -0.333333, 0, 0, -0.333333, 0, 1, 0, 0, 0, 0, -0.333333, 0, 0.666667, 0, 0, -0.333333, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, -0.333333, 0, -0.333333, 0, 0, 0.666667));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.666667, 0, -0.333333, -0.333333, 0, 0, 0, 1, 0, 0, 0, 0, -0.333333, 0, 0.666667, -0.333333, 0, 0, -0.333333, 0, -0.333333, 0.666667));
   vm.push_back(MatS6(0.666667, 0, 0, -0.333333, 0, -0.333333, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.333333, 0, 0, 0.666667, 0, -0.333333, 0, 0, 0, 0, 0, 0, -0.333333, 0, 0, -0.333333, 0, 0.666667));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.666667, 0, 0, -0.333333, -0.333333, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.333333, 0, 0, 0.666667, -0.333333, 0, -0.333333, 0, 0, -0.333333, 0.666667));
   vm.push_back(MatS6(0.666667, 0, -0.333333, 0, 0, -0.333333, 0, 0, 0, 0, 0, 0, -0.333333, 0, 0.666667, 0, 0, -0.333333, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, -0.333333, 0, -0.333333, 0, 0, 0.666667));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.666667, 0, -0.333333, -0.333333, 0, 0, 0, 0, 0, 0, 0, 0, -0.333333, 0, 0.666667, -0.333333, 0, 0, -0.333333, 0, -0.333333, 0.666667));
   vm.push_back(MatS6(0.666667, -0.333333, 0, 0, -0.333333, 0, -0.333333, 0.666667, 0, 0, -0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -0.333333, -0.333333, 0, 0, 0.666667, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.666667, 0, 0, -0.333333, -0.333333, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.333333, 0, 0, 0.666667, -0.333333, 0, -0.333333, 0, 0, -0.333333, 0.666667, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.666667, 0, 0, -0.333333, -0.333333, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.333333, 0, 0, 0.666667, -0.333333, 0, -0.333333, 0, 0, -0.333333, 0.666667));
   vm.push_back(MatS6(0.666667, 0, 0, -0.333333, 0, -0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -0.333333, 0, 0, 0.666667, 0, -0.333333, 0, 0, 0, 0, 1, 0, -0.333333, 0, 0, -0.333333, 0, 0.666667));
   return LabeledSellaMatrices("R3", vm);
}

std::vector<LabeledSellaMatrices> SellaBuild::CreateAllPerps(){
   std::vector< LabeledSellaMatrices> vlsm;
   vlsm.push_back(SellaBuild::CreatePerps_H4());
   vlsm.push_back(SellaBuild::CreatePerps_K1());
   vlsm.push_back(SellaBuild::CreatePerps_K3());
   vlsm.push_back(SellaBuild::CreatePerps_K5());
   vlsm.push_back(SellaBuild::CreatePerps_M1A());
   vlsm.push_back(SellaBuild::CreatePerps_M1B());
   vlsm.push_back(SellaBuild::CreatePerps_M2A());
   vlsm.push_back(SellaBuild::CreatePerps_M2B());
   vlsm.push_back(SellaBuild::CreatePerps_M3());
   vlsm.push_back(SellaBuild::CreatePerps_M4());
   vlsm.push_back(SellaBuild::CreatePerps_O1A());
   vlsm.push_back(SellaBuild::CreatePerps_O1B());
   vlsm.push_back(SellaBuild::CreatePerps_O2());
   vlsm.push_back(SellaBuild::CreatePerps_O3());
   vlsm.push_back(SellaBuild::CreatePerps_O4());
   vlsm.push_back(SellaBuild::CreatePerps_O5());
   vlsm.push_back(SellaBuild::CreatePerps_T1());
   vlsm.push_back(SellaBuild::CreatePerps_T2());
   vlsm.push_back(SellaBuild::CreatePerps_T5());
   vlsm.push_back(SellaBuild::CreatePerps_R1());
   vlsm.push_back(SellaBuild::CreatePerps_R3());

   return vlsm;
}

LabeledSellaMatrices SellaBuild::CreatePrjs_H4() {
   std::vector<MatS6> vm;
   /*  H4   */
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 1, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333));
   vm.push_back(MatS6(0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.333333, 0, 0, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333));
   vm.push_back(MatS6(0.333333, 0, 0, 0, 0.333333, 0.333333, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333));
   vm.push_back(MatS6(0.333333, 0, 0, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333));
   vm.push_back(MatS6(0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 1, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333));
   return LabeledSellaMatrices("H4", vm);
}

LabeledSellaMatrices SellaBuild::CreatePrjs_C1() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667));
   return LabeledSellaMatrices("C1", vm);
}

LabeledSellaMatrices SellaBuild::CreatePrjs_C3() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 0, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.25, 0, 0.25, 0.25, 0, 0.25, 0, 0, 0, 0, 0, 0, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0, 0, 0, 0, 0, 0, 0.25, 0, 0.25, 0.25, 0, 0.25));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 0, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25));
   return LabeledSellaMatrices("C3", vm);
}


LabeledSellaMatrices SellaBuild::CreatePrjs_C5() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333));
   vm.push_back(MatS6(0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0, 0, 0.333333));
   vm.push_back(MatS6(0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0.333333));
   vm.push_back(MatS6(0.333333, 0, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0, 0.333333, 0, 0.333333));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0.333333));
   vm.push_back(MatS6(0.333333, 0, 0.333333, 0, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0, 0.333333));
   vm.push_back(MatS6(0.333333, 0, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.333333, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0, 0.333333));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.333333, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   return LabeledSellaMatrices("C5", vm);
}


LabeledSellaMatrices SellaBuild::CreatePrjs_M1A() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0, 0.5, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0.5, 0, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   return LabeledSellaMatrices("M1A", vm);
}


LabeledSellaMatrices SellaBuild::CreatePrjs_M1B() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 1, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.5, 0, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 1, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5));
   return LabeledSellaMatrices("M1B", vm);
}

LabeledSellaMatrices SellaBuild::CreatePrjs_M2B() {
   std::vector<MatS6> vm;
   vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.5, 0, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 1, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 1, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   vm.push_back(MatS6(0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1));
   vm.push_back(MatS6(0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5));
   vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0, 0.5, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0.5));
   vm.push_back(MatS6(0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
   return LabeledSellaMatrices("M2B", vm);
}


   /*  M2A  */
   LabeledSellaMatrices SellaBuild::CreatePrjs_M2A() {
      std::vector<MatS6> vm;
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0.5, 0, 0, 0, 0, 0.5));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0.5, 0, 0, 0, 0.5));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5));
      vm.push_back(MatS6(0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0));
      return LabeledSellaMatrices("M2A", vm);
   }


   /*  M3   */
   LabeledSellaMatrices SellaBuild::CreatePrjs_M3() {
      std::vector<MatS6> vm;
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.5, 0, 0, 0.5));
      vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0.5));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0.5, 0, 0, 0, 0.5));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5));
      vm.push_back(MatS6(0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0));
      return LabeledSellaMatrices("M3", vm);
   }


   /*  M4   */
   LabeledSellaMatrices SellaBuild::CreatePrjs_M4() {
      std::vector<MatS6> vm;
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      return LabeledSellaMatrices("M4", vm);
   }

   /*  O1A  */
   LabeledSellaMatrices SellaBuild::CreatePrjs_O1A() {
      std::vector<MatS6> vm;
      vm.push_back(MatS6(0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 1, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.25, 0, 0.25, 0.25, 0, 0.25, 0, 1, 0, 0, 0, 0, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0, 0, 0, 0, 1, 0, 0.25, 0, 0.25, 0.25, 0, 0.25));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 1, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25));
      return LabeledSellaMatrices("O1A", vm);
   }


   /*  O1B  */
   LabeledSellaMatrices SellaBuild::CreatePrjs_O1B() {
      std::vector<MatS6> vm;
      vm.push_back(MatS6(0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 0.5, 0, 0, 0.5, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 0.5, 0, 0, 0.5));
      vm.push_back(MatS6(0.25, 0, 0.25, 0.25, 0, 0.25, 0, 0.5, 0, 0, 0.5, 0, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0, 0.5, 0, 0, 0.5, 0, 0.25, 0, 0.25, 0.25, 0, 0.25));
      vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0.5, 0, 0, 0.5, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25));
      return LabeledSellaMatrices("O1B", vm);
   }


   /*  O2   */
   LabeledSellaMatrices SellaBuild::CreatePrjs_O2() {
      std::vector<MatS6> vm;
      vm.push_back(MatS6(0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0.5, 0, 0, 0, 0, 0.5));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5));
      vm.push_back(MatS6(0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5));
      vm.push_back(MatS6(0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0));
      return LabeledSellaMatrices("O2", vm);
   }


   /*  O3   */
   LabeledSellaMatrices SellaBuild::CreatePrjs_O3() {
      std::vector<MatS6> vm;
      vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0.5));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5));
      vm.push_back(MatS6(0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5));
      vm.push_back(MatS6(0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0));
      return LabeledSellaMatrices("O3", vm);
   }


   /*  O4   */
   LabeledSellaMatrices SellaBuild::CreatePrjs_O4() {
      std::vector<MatS6> vm;
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5));
      vm.push_back(MatS6(0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0, 0.5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0.5, 0, 0, 0, 0, 0.5));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0.5, 0, 0, 0, 0.5));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5));
      vm.push_back(MatS6(0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0.5, 0, 0, 0, 0, 0.5));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0.5, 0, 0, 0, 0, 0.5));
      vm.push_back(MatS6(0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5));
      return LabeledSellaMatrices("O4", vm);
   }


   /*  O5   */
   LabeledSellaMatrices SellaBuild::CreatePrjs_O5() {
      std::vector<MatS6> vm;
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      return LabeledSellaMatrices("O5", vm);
   }


   /*  Q1   */
   LabeledSellaMatrices SellaBuild::CreatePrjs_T1() {
      std::vector<MatS6> vm;
      vm.push_back(MatS6(0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 0.5, 0, 0, 0.5, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 0.5, 0, 0, 0.5));
      vm.push_back(MatS6(0.25, 0, 0.25, 0.25, 0, 0.25, 0, 0.5, 0, 0, 0.5, 0, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0, 0.5, 0, 0, 0.5, 0, 0.25, 0, 0.25, 0.25, 0, 0.25));
      vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0.5, 0, 0, 0.5, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25));
      return LabeledSellaMatrices("T1", vm);
   }


   /*  Q2   */
   LabeledSellaMatrices SellaBuild::CreatePrjs_T2() {
      std::vector<MatS6> vm;
      vm.push_back(MatS6(0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 0, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.25, 0, 0.25, 0.25, 0, 0.25, 0, 0, 0, 0, 0, 0, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0, 0, 0, 0, 1, 0, 0.25, 0, 0.25, 0.25, 0, 0.25));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 0, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 1, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25));
      vm.push_back(MatS6(0.25, 0, 0.25, 0.25, 0, 0.25, 0, 1, 0, 0, 0, 0, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0, 0, 0, 0, 0, 0, 0.25, 0, 0.25, 0.25, 0, 0.25));
      vm.push_back(MatS6(0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 1, 0, 0, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0.25, 0.25, 0, 0, 0, 0, 0, 0, 0));
      return LabeledSellaMatrices("T2", vm);
   }


   /*  Q5   */
   LabeledSellaMatrices SellaBuild::CreatePrjs_T5() {
      std::vector<MatS6> vm;
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5));
      vm.push_back(MatS6(0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0, 0.5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0.5));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.5, 0, 0, 0.5));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5));
      vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0.5, 0, 0, 0, 0.5));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5));
      vm.push_back(MatS6(0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0.5));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5));
      vm.push_back(MatS6(0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0.5));
      vm.push_back(MatS6(0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0.5));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5));
      return LabeledSellaMatrices("T5", vm);
   }


   /*  R1   */
   LabeledSellaMatrices SellaBuild::CreatePrjs_R1() {
      std::vector<MatS6> vm;
      vm.push_back(MatS6(0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333));
      vm.push_back(MatS6(0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 0.333333));
      vm.push_back(MatS6(0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333));
      vm.push_back(MatS6(0.333333, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333));
      return LabeledSellaMatrices("R1", vm);
   }


   /*  R3   */
   LabeledSellaMatrices SellaBuild::CreatePrjs_R3() {
      std::vector<MatS6> vm;
      vm.push_back(MatS6(0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 1, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 1, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0, 0, 0.333333));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333));
      vm.push_back(MatS6(0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0, 0, 0.333333));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0.333333, 0.333333, 0, 0, 0, 0.333333));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333, 0, 0, 0, 0.333333, 0.333333, 0.333333));
      vm.push_back(MatS6(0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.333333, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0, 0.333333));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0, 0, 1, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0, 0, 1, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 1, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0, 0.333333));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333));
      vm.push_back(MatS6(0.333333, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0, 0, 1, 0, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 1, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.333333, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.333333, 0, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 1));
      vm.push_back(MatS6(0.333333, 0, 0.333333, 0, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0.333333, 0, 0.333333, 0, 0, 0.333333));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0.333333));
      vm.push_back(MatS6(0.333333, 0, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 1, 0, 0.333333, 0, 0, 0.333333, 0, 0.333333));
      vm.push_back(MatS6(1, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0.333333));
      vm.push_back(MatS6(0.333333, 0, 0.333333, 0, 0, 0.333333, 0, 1, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0, 0, 0.333333));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0, 1, 0, 0, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0.333333));
      vm.push_back(MatS6(0.333333, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0.333333, 0, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0.333333, 0, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0));
      vm.push_back(MatS6(0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.333333, 0, 0, 0.333333, 0.333333, 0, 0.333333, 0, 0, 0.333333, 0.333333));
      vm.push_back(MatS6(0.333333, 0, 0, 0.333333, 0, 0.333333, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0, 0.333333, 0, 0.333333, 0, 0, 0, 0, 0, 0, 0.333333, 0, 0, 0.333333, 0, 0.333333));
      return LabeledSellaMatrices("R1", vm);
   }

   std::vector<LabeledSellaMatrices> SellaBuild::CreateAllPrjs() {
      std::vector< LabeledSellaMatrices> vlsm;
      vlsm.push_back(SellaBuild::CreatePrjs_H4());
      vlsm.push_back(SellaBuild::CreatePrjs_C1());
      vlsm.push_back(SellaBuild::CreatePrjs_C3());
      vlsm.push_back(SellaBuild::CreatePrjs_C5());
      vlsm.push_back(SellaBuild::CreatePrjs_M1A());
      vlsm.push_back(SellaBuild::CreatePrjs_M1B());
      vlsm.push_back(SellaBuild::CreatePrjs_M2A());
      vlsm.push_back(SellaBuild::CreatePrjs_M2B());
      vlsm.push_back(SellaBuild::CreatePrjs_M3());
      vlsm.push_back(SellaBuild::CreatePrjs_M4());
      vlsm.push_back(SellaBuild::CreatePrjs_O1A());
      vlsm.push_back(SellaBuild::CreatePrjs_O1B());
      vlsm.push_back(SellaBuild::CreatePrjs_O2());
      vlsm.push_back(SellaBuild::CreatePrjs_O3());
      vlsm.push_back(SellaBuild::CreatePrjs_O4());
      vlsm.push_back(SellaBuild::CreatePrjs_O5());
      vlsm.push_back(SellaBuild::CreatePrjs_T1());
      vlsm.push_back(SellaBuild::CreatePrjs_T2());
      vlsm.push_back(SellaBuild::CreatePrjs_T5());
      vlsm.push_back(SellaBuild::CreatePrjs_R1());
      vlsm.push_back(SellaBuild::CreatePrjs_R3());

      return vlsm;
   }

