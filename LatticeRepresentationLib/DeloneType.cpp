#include "DeloneType.h"

DeloneType::DeloneType(const std::string& deloneType, 
	const std::string& bravaisType, const std::string& character, const std::pair<std::string, MatS6 >& fromCanon)
	: m_deloneName(deloneType)
	, m_bravaisType(bravaisType)
	, m_character(character)
	, m_toCentered_E3()
	, m_toCentered_S6(fromCanon.second)
{
}


DeloneType::DeloneType(
   const std::string& deloneType, 
   const std::string& bravaisType, 
   const std::string& character, 
   const std::string& toCentered_E3,
   const MatS6& toCentered,
   const MatricesForOneDeloneType& vm) 
   : m_deloneName(deloneType)
   , m_bravaisType(bravaisType)
   , m_character(character)
   , m_toCentered_E3(toCentered_E3)
   , m_toCentered_S6(toCentered)
   , m_matrices(vm)
{
}

std::tuple<double, S6, MatS6> DeloneType::GetFit(const S6& s6) {
   unsigned long n = 0;
   double best = DBL_MAX;
   for ( unsigned long i=0; i< m_matrices.size(); ++i ) {
      const S6 testv = m_matrices.GetPerps(i) * s6;
      const S6 bestv = m_matrices.GetPrj(i) * s6;
      const double test = testv.norm();
      if ( best > test){
         n = i;
         best = test;


         //std::cout << " representation   " << i << std::endl;
         //std::cout << "original " << s6 << std::endl;
         //std::cout << "projected  " << bestv << std::endl;
         //std::cout << "perp v " << testv << std::endl;
         //std::cout << "projected s6 " << bestv << std::endl;
         //std::cout << "distance " << test << std::endl;
      }
   }
   if (best < 1.0E-8) best = 0.0;
   return std::make_tuple(best, m_matrices.GetPrj(n)*s6, m_matrices.GetToCanon(n));
}


std::ostream& operator<< (std::ostream& o, const DeloneType& m) {
   o << "Delone Type  " << m.m_deloneName << std::endl;
   o << "BravaisLattice Type  " << m.m_bravaisType << std::endl;
   o << "Character Type  " << m.m_character << std::endl;
   o << "Canonical to Centered (E3)  " << m.m_toCentered_E3 << std::endl;
   o << "Canonical to Centered (S6)  " << std::endl << m.m_toCentered_S6 << std::endl;
   o << "Count of all representations  " << m.m_matrices.size() << std::endl;
   return o;
}

