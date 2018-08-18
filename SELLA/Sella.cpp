#include "Sella.h"


#include "GenerateRandomLattice.h"
#include "Selling.h"
#include "StoreResults.h"

std::vector< std::pair<std::string, MatS6> > Sella::vDeloneTypes = Delone::LoadLabeledLatticeTypeProjectors();


Sella::Sella() {
   store.SetMaxItemStore(250);

   for (unsigned long i = 0; i < vDeloneTypes.size(); ++i) {
      Expand(vDeloneTypes[i].first, vDeloneTypes[i].second);
   }
   ProcessItemStoreToVectorMap();
   ProcessVectorMapToPerpsAndProjectors();
   //store.ShowResults();
}

static int seed = 19191;

S6 Sella::MakeSampleType(const MatS6& m) {
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

double Sella::TestOneType(const std::string& label, const S6 &s6, const std::vector<MatS6>& vm) {
   double best = DBL_MAX;
   for (unsigned long i = 0; i < vm.size(); ++i) {
      const S6 prp = vm[i] * s6;
      best = std::min(best, prp.norm());
      //std::cout << i << "  " << label<< "  " << prp.norm() << std::endl;
   }
   return best;
}

void Sella::TestAllTypes(const S6& s6) {
   S6 s6out;
   Selling::Reduce(s6, s6out);
   for (unsigned long i = 0; i < prps.size(); ++i) {
      const std::string label = prps[i].first;
      const double best = TestOneType(label, s6out, prps[i].second);
      std::cout << i << "   " << label << "  " << best << std::endl;
   }

}

void Sella::ProcessVectorMapToPerpsAndProjectors() {
   const MatS6 unit = MatS6().unit();
   std::map<std::string, std::vector<S6> >::iterator it;
   for (it = themap.begin(); it != themap.end(); ++it) {
      const std::pair<std::string, std::vector<S6> > p = *it;
      std::vector<MatS6> vmprj;
      std::vector<MatS6> vmprp;
      const std::string label(p.first);
      for (unsigned long i = 0; i < p.second.size(); ++i) {
         const MatS6 mprj = ProjectorFromVector(p.first, p.second[i]);
         vmprj.push_back(ProjectorFromVector(p.first, p.second[i]));
         vmprp.push_back(unit - ProjectorFromVector(p.first, p.second[i]));
      }
      prjs.push_back(std::make_pair(label, vmprj));
      prps.push_back(std::make_pair(label, vmprp));
   }
}

void Sella::Expand(const std::string& label, const MatS6& m) {
   if (label[0] == 'T') return;
   const S6 s6 = MakeSampleType(m);

   bool b = s6.IsValid();

   if (s6.IsValid()) {
      switch (s6.CountZeros()) {
      case 0:
         StoreAllReflections(label, s6);
         break;
      case 1:
         OneBound(label, s6);
         break;
      case 2:
         ProcessZeros(label, s6);
         break;
      case 3:
         ProcessZeros(label, s6);
         break;
      default:
         throw "this should never happen";
         break;
      }
   }
}

bool Sella::FindDuplicate(const std::vector<S6>& out, const S6 s6) {
   bool fail = false;
   if (!out.empty()) {
      for (unsigned long o = 0; o < out.size(); ++o) {
         if ((out[o] - s6).norm() < 1.0E-6) fail = true;
      }
   }

   return fail;
}

void Sella::ProcessItemStoreToVectorMap() {
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



unsigned long Sella::Index6(const unsigned long i, const unsigned long j) {
   return 6 * i + j % 6;
}

double Sella::Fraction(const double d, const S6& s) {
   double dout = 0.0;
   for (unsigned long i = 0; i < 6; ++i) {
      if (abs(s[i] - d) < 1.0E-6 && abs(d) > 1.0E-6) dout += 1.0;
   }
   return (dout == 0.0) ? 0.0 : 1.0 / dout;
}

MatS6 Sella::ProjectorFromVector(const std::string& label, const S6& s) {
   MatS6 m;
   std::pair<std::string, std::vector<MatS6> > oneperp;
   for (unsigned long j = 0; j < 6; ++j) {
      for (unsigned long k = 0; k < 6; ++k) {
         const unsigned long index1 = Index6(j, k);
         const unsigned long index2 = Index6(k, j);
         const double thisFrac = Fraction(s[k], s);
         m[index1] = (abs(s[j] - s[k]) < 1.0E-5 && abs(s[k]) > 1.0E-5) ? thisFrac : 0.0;
         m[Index6(k, k)] = (thisFrac == 0.0 || abs(s[k]) < 1.0E-6) ? 0.0 : thisFrac;
         m[Index6(j, j)] = (thisFrac == 0.0 || abs(s[k]) < 1.0E-6) ? 0.0 : thisFrac;
      }
   }

   const MatS6 unit = MatS6().unit();
   //projectors.push_back(std::make_pair(label, m));
   //perps.push_back(std::make_pair(label, unit-m));

   return m;
}

std::vector<unsigned long> Sella::FindS6Zeros(const S6& s) {
   std::vector<unsigned long> v;
   for (unsigned long i = 0; i < 6; ++i) if (s[i] == 0.0) v.push_back(i);
   return v;
}

void Sella::StoreAllReflections(const std::string& label, const S6& s1) {
   std::vector< S6(*)(const S6&)> refl = S6::SetRelectionFunctions();

   for (unsigned long i = 0; i < refl.size(); ++i) {
      store.Store(label, refl[i](s1));
   }
}

void Sella::OneBound(const std::string& label, const S6& s1) {
   static const std::vector< S6(*)(const S6&)> fnRedn = S6::SetReduceFunctions();

   unsigned long nzero = 0;
   for (unsigned long i = 0; i < 6; ++i) if (s1[i] == 0) nzero = i;
   S6 s6temp = fnRedn[nzero](s1);
   StoreAllReflections(label, s6temp);
}

void Sella::ProcessZeros(const std::string& label, const S6& s6) {
   const std::vector<unsigned long> v = FindS6Zeros(s6);
   const unsigned long n = v.size();

   if (n == 0) {
      return;
   }
   else if (n == 1) {
      OneBound(label, s6);
   }
   else {
      for (unsigned long i = 0; i < v.size(); ++i) {
         S6 temp(s6);
         temp[v[i]] = DBL_MIN;
         ProcessZeros(label, temp);
      }
   }

}

void Sella::TestPerps() {
   const G6 g6(" 100 100 100 100 100 100");
   const LRL_Cell cell(" 10 10 10 89 89 89");
   const S6 s6(cell);
   TestAllTypes(s6);
}
