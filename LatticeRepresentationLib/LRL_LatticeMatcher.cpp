#include "LRL_LatticeMatcher.h"

#include <cfloat>

#include "B4.h"
#include "C3.h"
#include "G6.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ToString.h"
#include "S6.h"

#include "Selling.h"
#include "StoreResults.h"
#include "TNear.h"


LRL_LatticeMatcher::LRL_LatticeMatcher()
   : m_MVtree_Angular()
   , m_matrixTree()
   , m_reducedReference(S6())
   , dcutoff(0.01)
{
   BuildMatrixTree( );
}

StoreResults<int, std::string> LRL_LatticeMatcher::record;

void LRL_LatticeMatcher::FillReflections( ) {
   static const std::vector<MatS6> refl = MatS6::GetReflections( );
   if (m_matrixTree.empty( ))
      m_matrixTree.insert( refl );
}

void LRL_LatticeMatcher::StoreMatS6IfUnique( const MatS6& m) {
   if (m_matrixTree.NearestNeighbor( dcutoff, m ) == m_matrixTree.end( ))
      m_matrixTree.insert( m );
}

std::vector<std::pair<MatS6, MatS6> > CreateReductionMatrices() {
   std::vector<std::pair<MatS6, MatS6> > m( 1, std::make_pair( MatS6( ).unit( ), MatS6( ).unit( ) ) );
   static const std::vector<std::pair<MatS6, MatS6> > redc = S6::SetUnreductionMatrices( );
   m.insert( m.end( ), redc.begin( ), redc.end( ) );
   return m;
}

static const std::vector<std::pair<MatS6, MatS6> > redc( CreateReductionMatrices( ) );
static const std::vector<MatS6> refl_one = MatS6::GetReflections( );

void LRL_LatticeMatcher::ExpandMatrices( const int n, const MatS6& m ) {

   for (size_t i = 0; i < refl_one.size( ); ++i) {
      const MatS6 mi = refl_one[i] * m;
      StoreMatS6IfUnique( mi );
      if (n > 1) ExpandMatrices( n - 1, mi );
   }

   for (size_t i = 0; i < redc.size( ); ++i) {
      const MatS6 mi = redc[i].first * m;
      StoreMatS6IfUnique( mi );
      if (n > 1) ExpandMatrices( n - 1, mi );
   }
}

std::vector<MatS6> LRL_LatticeMatcher::DoThreeAxes( ) {

   ExpandMatrices( 2, MatS6( ).unit( ) );
   return m_matrixTree.GetObjectStore( );



   static std::vector<MatS6> vmp;
   if (vmp.empty( )) {
      for (size_t i = 0; i < 6; ++i) {
         const MatS6 mi = redc[i].first;
         StoreMatS6IfUnique( mi );
         for (size_t k = 0; k < 6; ++k) {
            if (i == k) continue;
            const MatS6 mk = redc[k].first * mi;
            StoreMatS6IfUnique( mk );
            for (size_t m = 0; m < 6; ++m) {
               if (m == k) continue;
               const MatS6 mm = redc[m].first * mk;
               StoreMatS6IfUnique( mm );
            }
         }
      }
      for (size_t i = 0; i < m_matrixTree.size( ); ++i) vmp.push_back( m_matrixTree[i] );
   }
   return vmp;
}

void LRL_LatticeMatcher::BuildMatrixTree( void ) {
   if (m_matrixTree.empty( )) {
      FillReflections( );
      static const std::vector<MatS6> vMatS6 = DoThreeAxes( );
      m_matrixTree.insert( vMatS6 );
      ApplyReflections( vMatS6 );

      // !!!!!!!!!!!!!!!! code to generate a deeper matrix tree !!!!!!!!!!!!!!!!!!!!!!
      //static const std::vector<MatS6> vMatS6A = DoThreeAxes( );
      //m_matrixTree.insert( vMatS6A );
      //ApplyReflections( vMatS6A );
      //static const std::vector<MatS6> vMatS6B = DoThreeAxes( );
      //m_matrixTree.insert( vMatS6B );
      //ApplyReflections( vMatS6B );
   }
}

void LRL_LatticeMatcher::SetReferenceLattice( const MV_Pair& mpReducedReference ) {
   if ((m_reducedReference - mpReducedReference.GetS6()).norm( ) < 1.0E-4) return;
   m_MVtree_Angular.clear( );
   m_reducedReference = mpReducedReference.GetS6();
   m_matReference = mpReducedReference.GetMatS6( );
   BuildReferenceTree( m_reducedReference );
}

void LRL_LatticeMatcher::SetReferenceLattice( const S6& s) {
   if ((s - m_reducedReference).norm( ) < 1.0E-4) return;
   SetReferenceLattice( MV_Pair( s, MatS6( ).unit( ) ) );
}

void LRL_LatticeMatcher::BuildReferenceTree( const S6& s) {
   const double normReference = m_reducedReference.norm( );
   const Scaler_MV scale(s);
   for (size_t i = 0; i<m_matrixTree.size(); ++i ) {
      const S6 scaledMV = scale.Scale(m_matrixTree[i] * s);
      StoreMV_IfUnique( scaledMV, MatS6::Inverse( m_matrixTree[i] ) );
   }
}

void LRL_LatticeMatcher::ApplyReflections( const std::vector<MatS6>& t ) {
   static const std::vector<MatS6> refl = MatS6::GetReflections( );
   for (size_t i = 0; i < t.size( ); ++i) {
      for (size_t k = 0; k < refl.size( ); ++k) {
         StoreMatS6IfUnique( refl[k] * t[i] );
      }
   }
}

std::pair<double, AngularS6> LRL_LatticeMatcher::MatchOneFromTheSphere( const AngularS6& s, const std::vector<AngularS6>& vClosest ) const
{
   double dmin_S6 = DBL_MAX;
   AngularS6 s6closest;
   for (size_t i = 0; i < vClosest.size(); ++i) {
      const S6 s6test = vClosest[i].GetMatS6( ) * s.GetOriginalS6( );
      const double d2 = (s6test - s.GetOriginalS6( )).norm( );
      const double d_G6 = (G6( s6test ) - G6( s.GetOriginalS6( ) )).norm( );
      const double dtest_S6 = d2;

      if (dmin_S6 >= dtest_S6) {
         dmin_S6 = dtest_S6;
         s6closest = s6test;
      }
   }
   return std::make_pair( dmin_S6, s6closest );
}

AngularS6 LRL_LatticeMatcher::SelectAlternateImageOfReference( const AngularS6& s, const AngularS6& closest ) const {
   // now check for alternate solutions
   const double angle1 = closest.AngleBetween( closest, AngularS6( s.GetS6( ), MatS6( ) ) );
   record.Store(int( angle1*180./(4.0*atan(1.0))), "");

   std::vector<AngularS6> vClosest;
   const long n = m_MVtree_Angular.FindInSphere( 1.01 * angle1 + 0.05, vClosest, closest );

   std::pair<double, AngularS6> best( std::make_pair( DBL_MAX, S6( ) ) );
   return MatchOneFromTheSphere( s, vClosest ).second;
}

AngularS6 LRL_LatticeMatcher::FindClosest( const S6& s ) const {
   AngularS6 closest;
   const bool b = m_MVtree_Angular.NearestNeighbor( DBL_MAX, closest, AngularS6( s, MatS6( ) ) );
   if (!b) throw;
   return closest;
}

S6 LRL_LatticeMatcher::MatchReference( const S6& s ) const {
   const static bool debug = false;
   const AngularS6 revisedClosest = SelectAlternateImageOfReference( s, FindClosest( s ) );
   return revisedClosest.GetMatS6( ) * s;
}

std::vector<S6> LRL_LatticeMatcher::MatchReference( const std::vector<S6>& vs ) const {
   std::vector<S6> v;
   for (size_t i = 0; i < vs.size( ); ++i)
      v.push_back( m_matReference * MatchReference( vs[i] ) );
   return v;
}

void LRL_LatticeMatcher::StoreMV_IfUnique( const S6& s, const MatS6& m ) {
   AngularS6 scaled_MV( s, m );
   //scaled_MV = MV_Pair::CleanZeros( scaled_MV );
   /*if (m_MVtree_Angular.NearestNeighbor( dcutoff, scaled_MV ) == m_MVtree_Angular.end( ))*/ {
      m_MVtree_Angular.insert( scaled_MV );
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

LMDist::LMDist( const S6& s ) {
   SetReferenceLattice( MV_Pair( s, MatS6( ).unit( ) ) );
   m_MVtree_Angular.clear( );
   BuildMatrixTree( );
   BuildReferenceTree( m_reducedReference );
}

double LMDist::DistanceBetween( const S6& s2 ) {
   const S6 matched = MatchReference( s2 );
   return (m_reducedReference - matched).norm( );
}

double LMDist::DistanceBetween( const S6& s1, const S6& s2 ) {
   SetReferenceLattice( s1 );
   const S6 matched = MatchReference( s2 );
   const double dist = (m_reducedReference - matched).norm( );
   //std::cout << "in LMDist::DistanceBetween " << dist << std::endl 
   //   << " " << C3(s1) << std::endl
   //   << " " << C3(s2) << std::endl
   //   << " " << C3(m_reducedReference) << std::endl
   //   << " " << C3(matched) << std::endl << std::endl;
   return dist;
}
