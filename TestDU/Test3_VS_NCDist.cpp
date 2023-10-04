#include "Test3_VS_NCDIst.h"
#include "DC7u.h"
#include "LRL_Cell.h"
#include "NCDist.h"
#include "Niggli.h"

#include <cmath>
#include <ostream>

;
std::vector<Test3Results> TestDistance::Test3_VS_NCDist() {

   const size_t ntest = 30;
   std::vector<Test3Results> results;
   Test3Results result;

   //TestDistance testD;
   for (size_t i = 0; i < ntest; ++i) {
      const G6 g1 = TestDistance::G6_rand_reduced();
      const G6 g2 = 10 * TestDistance::G6_rand_reduced();
      const std::vector<MatDC7u> vm = TestDistance::SelectMatrices(g1);

      const double ncdist = NCDist(g1.data(), g2.data());
      const double dist = TestDistance::BestDistance(g1, g2, vm);

      std::cout << "dist " << dist << "  ncdist " << ncdist
         << " ratio " << dist/ncdist << std::endl;
      result.SetDist(dist);
      result.SetNCDist(ncdist);
      result.SetG1(g1);;
      result.SetG2(g2);;
      result.SetMatrices(vm);;

      results.emplace_back(result);
   }
   return results;
}


std::vector<MatDC7u>  TestDistance::SelectMatrices(const G6& g) {
   static const std::vector<MatDC7u> vm = MatDC7u::MakeBoundaryMatrices();
   std::vector<MatDC7u> out;

   for (size_t i = 0; i < vm.size(); ++i) {
      const bool b = vm[i].ShouldApply(g);
      if (b) out.emplace_back(vm[i]);
   }
   return out;
}

G6 TestDistance::G6_rand_reduced() {
   const G6 g6rand = G6::rand();

   int count = 0;
   do {
      ++count;
      G6 temp(G6::rand());
      G6 red;
      const bool b = Niggli::Reduce(temp, red);
      if (b && LRL_Cell(g6rand).IsValid()) return red;
   } while (count < 100);

   return G6();
}

double TestDistance::BestDistance(const G6& g1, const G6& g2, const std::vector<MatDC7u>& vm) {

   double dist = DBL_MAX;
   for (size_t i = 0; i < vm.size(); ++i) {
      const double test = DistanceBetween(DC7u(g1), vm[i]*DC7u(g2));
      //std::cout << i << " " << test << std::endl;
      dist = std::min(dist, test);
   }
   return dist;
}

std::ostream& operator<< (std::ostream& o, const Test3Results& t) {
   o << "DC7u dist. " << t.m_DC7uDistance << "\n";
   o << "ncdist " << t.m_NCDist << "\n";
   o << "g1 " << t.m_g1 << "\n";
   o << "g2 " << t.m_g2 << "\n";
   o << "dc7u1 " << DC7u(t.m_g1) << "\n";
   o << "dc7u2 " << DC7u(t.m_g2) << "\n";
   o << "no. of matrices " << t.m_vm.size() << "\n";
   //o << t.m_vm << "\n";
   return o;
}


;
