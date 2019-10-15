#include "LRL_LatticeMatcher.h"

#include <cfloat>

#include "B4.h"
#include "C3.h"
#include "G6.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_inverse.h"
#include "S6.h"

#include "Selling.h"
#include "StoreResults.h"
#include "TNear.h"


LRL_LatticeMatcher::LRL_LatticeMatcher()
   : m_MVtree()
   , m_matrixTree()
   , m_reference(S6())
   , dcutoff(0.01)
{
   FillReflections( );
   BuildVectorTree( );
}

void LRL_LatticeMatcher::FillReflections( ) {
   static const std::vector<MatS6> refl = MatS6::GetReflections( );
   if (m_matrixTree.empty( ))
      m_matrixTree.insert( refl );
}

void LRL_LatticeMatcher::StoreMatS6IfUnique( const MatS6& m) {
   if (m_matrixTree.NearestNeighbor( dcutoff, m ) == m_matrixTree.end( ))
      m_matrixTree.insert( m );
}

std::vector<MatS6> LRL_LatticeMatcher::DoThreeAxes( ) {
   static std::vector<MatS6> vmp;
   if (vmp.empty( )) {
      static const std::vector<std::pair<MatS6, MatS6> > redc = S6::SetUnreductionMatrices( );

      for (size_t i = 0; i < 6; ++i) {
         const MatS6 mi = redc[i].first;
         StoreMatS6IfUnique( mi );
         for (size_t k = 0; k < 6; ++k) {
            if (i == k) continue;
            const MatS6 mk = redc[k].first * mi;
            StoreMatS6IfUnique( mk );
            for (size_t m = 0; m < 6; ++m) {
               if (m == k) continue;
               StoreMatS6IfUnique( redc[m].first );
            }
         }
      }
   }
   for (size_t i = 0; i < m_matrixTree.size( ); ++i) vmp.push_back( m_matrixTree[i]);
   return vmp;
}

void LRL_LatticeMatcher::BuildVectorTree( void ) {
   FillReflections( );
   static const std::vector<MatS6> vMatS6 = DoThreeAxes( );
   m_matrixTree.insert( vMatS6 );
   ApplyReflections( vMatS6 );
}

void LRL_LatticeMatcher::SetReference( const MV_Pair& mvp ) {
   if (!m_MVtree.empty( ))
      m_MVtree.clear( );
   m_reference = mvp.GetS6();
   m_matReference = mvp.GetMatS6( );
   std::cout << "reference " << m_reference << std::endl << std::endl;
   BuildReferenceTree( m_reference );
}

static MatS6 Inverse( const MatS6& min ) {
   MatS6 m( min );
   std::vector<double> arout( 36 );
   inverse( 6, min.GetVector( ).data( ), arout.data( ) );
   m.SetVector( arout );
   return m;
}


void LRL_LatticeMatcher::BuildReferenceTree( const S6& s) {
   const double normReference = m_reference.norm( );
   const Scaler_MV scale(s);
   for (size_t i = 0; i<m_matrixTree.size(); ++i ) {
      const S6 scaledMV = scale.Scale(m_matrixTree[i] * s);
      const MatN matntest(m_matrixTree[i].GetMatrix( ) );
      const MV_Pair mv( scaledMV, MatS6(MatN(m_matrixTree[i].GetMatrix()).inverse()) );
      StoreMV_IfUnique( scaledMV, Inverse( m_matrixTree[i] ) );
   }
}

void LRL_LatticeMatcher::ApplyReflections( const std::vector<MatS6>& t ) {
   static const std::vector<MatS6> refl = MatS6::GetReflections( );
   for ( size_t i=0; i<t.size(); ++i ) {
      for ( size_t k=0; k<refl.size(); ++k ){
         StoreMatS6IfUnique( refl[k] * t[i] );
      }
   }
}

S6 LRL_LatticeMatcher::MatchReference( const S6& s ) const {
   const static bool debug = false;
   MV_Pair closest;
   const bool b = m_MVtree.NearestNeighbor( DBL_MAX, closest, MV_Pair(s, MatS6()) );
   if (!b) throw;

   const double d = (closest.GetMatS6()*s - m_reference).norm( );
   const int i19191 = 19191;
   const S6 optimized = closest.GetMatS6( ) * s;

   // now check for alternate solutions
   std::vector<MV_Pair> vClosest;
   S6 s6closest;
   double dmin_G6 = DBL_MAX;
   const long n = m_MVtree.FindInSphere( 1.01 * d, vClosest, closest );
   if ( n >= 1 ) {
      for ( size_t i=0; i<n; ++i ) {
         const S6 s6test = vClosest[i].GetMatS6( ) * s;
         const LRL_Cell_Degrees thecell( s6test );
         const double dtest_G6 = (G6(m_reference) - G6( s6test )).norm();
         if (dmin_G6 >= dtest_G6) {
            dmin_G6 = dtest_G6;
            s6closest = s6test;
         }
      }
   }
   if ( debug) std::cout << dmin_G6 << " Optimized FINAL" << s6closest << "  " << C3( s6closest ) << "  " << LRL_Cell_Degrees( s6closest ) << std::endl;

   return s6closest;
}

std::vector<S6> LRL_LatticeMatcher::MatchReference( const std::vector<S6>& vs ) const {
   std::vector<S6> v;
   for (size_t i = 0; i < vs.size( ); ++i) {
      v.push_back( m_matReference * MatchReference( vs[i] ) );
   }
   return v;
}

void LRL_LatticeMatcher::StoreMV_IfUnique( const S6& s, const MatS6& m ) {
   MV_Pair scaled_MV( s, m );
   scaled_MV = MV_Pair::CleanZeros( scaled_MV );
   /*if (m_MVtree.NearestNeighbor( dcutoff, scaled_MV ) == m_MVtree.end( ))*/ {
      m_MVtree.insert( scaled_MV );
   }
}

MV_Pair MV_Pair::CleanZeros( const MV_Pair& mv ) {
   MV_Pair t( mv );
   for (size_t i = 0; i < 6; ++i)
      t.m_s6[i] = (abs( mv.m_s6[i] ) < 1.0E-10) ? 0.0 : t.m_s6[i];
   return t;
}

std::vector<int> MV_Pair::GetOpenIndices( const MV_Pair& mvp ) const {
   std::vector<int> v;
   for (size_t i = (size_t)0; i < (size_t)6; ++i) if (!m_mark[i]) v.push_back( (int)i );
   return v;
}


std::ostream& operator<< ( std::ostream& o, const MV_Pair& v ) {
   o << "MV_Pair" << std::endl;
   o << "input S6 " << v.GetS6( ) << "  size " << v.m_originalSize << std::endl;
   MatS6 mtemp( v.m_ms6 );
   //o << "---------------" << std::endl;
   //o << mtemp << std::endl;
   //o << "---------------" << std::endl;
   o << "matrix times input " << mtemp * v.GetS6( ) << std::endl;
   o << "---------------" << std::endl;

   return o;
}

