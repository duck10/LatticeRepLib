#include "DeloneType.h"

#include <cfloat>
//DeloneType::DeloneType(const std::string& deloneType, 
//	const std::string& bravaisType, const std::string& character, const std::pair<std::string, MatS6 >& fromCanon)
//	: m_deloneName(deloneType)
//	, m_bravaisType(bravaisType)
//	, m_character(character)
//	, m_toCentered_E3()
//	, m_toCentered_S6(fromCanon.second)
//{
//}

DeloneType::DeloneType(
   const std::string& deloneType, 
   const std::string& bravaisType, 
   const std::string& character, 
   const std::string& toCentered_E3,
   const MatS6& toCentered,
   const int freePsrams,
   const MatricesForOneDeloneType& vm)
   : m_deloneName(deloneType)
   , m_bravaisType(bravaisType)
   , m_character(character)
   , m_toCentered_E3(toCentered_E3)
   , m_toCentered_S6(toCentered)
   , m_freePsrams( freePsrams )
   , m_matrices(vm)
{
}
//
//DeloneFitResults DeloneType::GetFit( const S6& s6 ) const
//{
//   return DeloneFitResults( );
//}

/*
protected:
   double m_rawFit;
   double m_zscore;
   S6 m_bestFit;
   S6 m_difference;
   MatS6 m_toCanonicalDeloneType;
*/

DeloneFitResults::DeloneFitResults( const double fit, const S6& bestApprox, const S6& perpv, const MatS6& toCanon )
: m_rawFit(fit)
, m_zscore(-19191.0)
, m_bestFit( bestApprox )
, m_difference(perpv)
, m_toCanonicalDeloneType(toCanon)
{
}


std::ostream& operator<< (std::ostream& o, const DeloneType& m) {
   o << "Delone Type  " << m.m_deloneName << std::endl;
   o << "BravaisLattice Type  " << m.m_bravaisType << std::endl;
   o << "Character Type  " << m.m_character << std::endl;
   o << "Canonical to Centered (E3)  " << m.m_toCentered_E3 << std::endl;
   //o << "Canonical to Centered (S6)  " << std::endl << m.m_toCentered_S6 << std::endl;
   o << "Count of all representations  " << m.m_matrices.size() << std::endl;
   return o;
}

std::ostream& operator<< ( std::ostream& o, const DeloneFitResults& dfr) {
   o << "m_latticeType           " << dfr.m_latticeType << std::endl;
   o << "m_rawFit                " << dfr.m_rawFit << std::endl;
   o << "m_zscore                " << dfr.m_zscore << std::endl;
   o << "m_bestFit               " << dfr.m_bestFit << std::endl;
   o << "m_difference            " << dfr.m_difference << std::endl;
   //o << "m_toCanonicalDeloneType " << std::endl << dfr.m_toCanonicalDeloneType << std::endl;
   return o;
}

DeloneFitResults DeloneType::GetFit(const S6& s6) const {
   size_t n = 0;
   S6 smallestPerp;
   double bestFit = DBL_MAX;
   for ( size_t i=0; i< m_matrices.size(); ++i ) {
      const S6 perpv = m_matrices.GetPerp(i) * s6;
      const double rawFit = perpv.norm();
      if ( bestFit > rawFit){
         n = i;
         bestFit = rawFit;
         smallestPerp = perpv;

         //std::cout << " representation   " << i << std::endl;
         //std::cout << "original " << s6 << std::endl;
         //std::cout << "projected  " << bestv << std::endl;
         //std::cout << "perp v " << perpv << std::endl;
         //std::cout << "projected s6 " << bestv << std::endl;
         //std::cout << "distance " << rawFit << std::endl;
      }
   }
   if (bestFit < 1.0E-8) bestFit = 0.0;
   const S6 bestv = m_matrices.GetPrj(n) * s6;
   MatS6 toCanonicalDeloneType = m_matrices.GetToCanon(n);
   return DeloneFitResults( bestFit, bestv, smallestPerp, toCanonicalDeloneType );
}
