#include "DeloneFitResults.h"
#include "DeloneType.h"
#include "LRL_Cell_Degrees.h"
#include "MatG6.h"
#include "MatS6.h"

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


std::ostream& operator<< (std::ostream& o, const DeloneType& m) {
   o << "REPORT FOR ONE DELONE TYPE " << std::endl;
   o << "\t" << "Delone Type  " << m.m_deloneName << std::endl;
   o << "\t" << "BravaisLattice Type  " << m.m_bravaisType << std::endl;
   o << "\t" << "Character Type  " << m.m_character << std::endl;
   o << "\t" << "Canonical to Centered (E3)  " << m.m_toCentered_E3 << std::endl;
   //o << "\t" << "Canonical to Centered (S6)  " << std::endl << m.m_toCentered_S6 << std::endl;
   o << "\t" << "Count of all representations  " << m.m_matrices.size() << std::endl;
   return o;
}

std::ostream& operator<< ( std::ostream& o, const DeloneFitResults& dfr) {
   o << "\tm_latticeType           " << dfr.m_latticeType <<
      ",  " << dfr.GetType() << std::endl;
   o << "\tm_rawFit                " << dfr.m_rawFit << std::endl;
   o << "\tm_DeloneType            " << dfr.m_DeloneType << std::endl;
   o << "\tm_zscore                " << dfr.m_zscore << std::endl;
   o << "\tm_bestFit               " << dfr.m_bestFit << std::endl;
   o << "\tm_difference            " << dfr.m_difference << std::endl;

   o << "\tBest Cell in original centering\n";
   o << "\t" << LRL_Cell_Degrees(MatS6::Inverse(dfr.m_reductionMatrix) * dfr.GetToCanon() * dfr.m_bestFit) << std::endl;

   //const MatS6 makePrim = LRL_Cell::G6MakePrimitiveMatrix(dfr.m_latticeType);
   //const MatS6 prim2Center( LRL_Cell::G6MakePrimitiveMatrix(dfr.m_latticeType));

   // this block is not working correctly 2022-04-19
   //const S6 canon = dfr.GetToCanon() * dfr.m_bestFit;
   //o << "\tcanon   "  << (dfr.GetToCanon() * dfr.m_bestFit) << std::endl;
   //const S6 unreduced = MatS6::Inverse(dfr.m_reductionMatrix)* canon;
   //const S6 centered = MatS6::Inverse(prim2Center) * canon;
   //const LRL_Cell_Degrees projectedCenteredCell(centered);
   //o << "\tprojectedCenteredCell" << projectedCenteredCell << std::endl;


   //o << "m_toCanonicalDeloneType " << std::endl << dfr.m_toCanonicalDeloneType << std::endl;
   return o;
}


//void TestReflections(const std::string& name, const S6& best, const MatS6&  toCanonicalDeloneType) {
//   static std::vector<MatS6> refls = MatS6::GetReflections();
//   std::cout << name << "************************************************************" << std::endl;
//   for (size_t i = 0; i < refls.size(); ++i) {
//      std::cout << i << "  " << refls[i] * toCanonicalDeloneType * best << std::endl;
//   }
//   std::cout << "************************************************************" << std::endl;
//}




//DeloneFitResults DeloneType::GetFit(const std::string& name, const S6& s6, const MatS6& reductionMatrix) const {
//   size_t nBest = 0;
//   S6 smallestPerp;
//   double bestFit = DBL_MAX;
//   for ( size_t i=0; i< m_matrices.size(); ++i ) {
//      const S6 perpv = m_matrices.GetPerp(i) * s6;
//      const double rawFit = perpv.norm();
//      if ( bestFit > rawFit){
//         nBest = i;
//         bestFit = rawFit;
//         smallestPerp = perpv;
//
//         //std::cout << " representation   " << i << std::endl;
//         //std::cout << "original " << s6 << std::endl;
//         //std::cout << "projected  " << bestv << std::endl;
//         //std::cout << "perp v " << perpv << std::endl;
//         //std::cout << "projected s6 " << bestv << std::endl;
//         //std::cout << "distance " << rawFit << std::endl;
//      }
//   }
//   if (bestFit < 1.0E-8) bestFit = 0.0;
//   const S6 bestv = m_matrices.GetPrj(nBest) * s6;
//   const MatS6 toCanonicalDeloneType = m_matrices.GetToCanon(nBest);
//
//   //TestReflections(name, bestv, toCanonicalDeloneType);
//   return DeloneFitResults( bestFit, bestv, smallestPerp, MatS6::Inverse(reductionMatrix) * toCanonicalDeloneType );
//   //return DeloneFitResults(bestFit, bestv, smallestPerp, MatS6::Inverse(reductionMatrix));
//}
