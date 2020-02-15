#include "LRL_LatticeMatcher.h"

#include <cfloat>

#include "B4.h"
#include "C3.h"
#include "G6.h"
#include "LRL_Cell_Degrees.h"
#include "S6.h"

#include "Selling.h"
#include "StoreResults.h"
#include "TNear.h"


LRL_LatticeMatcher::LRL_LatticeMatcher()
   : m_MVtree()
   , m_matrixTree()
   , m_reducedReference(S6())
   , dcutoff(0.01)
{
   BuildMatrixTree( );
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
}

void LRL_LatticeMatcher::BuildMatrixTree( void ) {
   if (m_matrixTree.empty( )) {
      FillReflections( );
      static const std::vector<MatS6> vMatS6 = DoThreeAxes( );
      m_matrixTree.insert( vMatS6 );
      ApplyReflections( vMatS6 );

      // TO GO ONE LEVEL DEEPER
      //static const std::vector<MatS6> vMatS6A = DoThreeAxes( );
      //m_matrixTree.insert( vMatS6A );
      //ApplyReflections( vMatS6A );

      // TO GO EVEN ONE LEVEL MORE DEEPER
      //static const std::vector<MatS6> vMatS6B = DoThreeAxes( );
      //m_matrixTree.insert( vMatS6B );
      //ApplyReflections( vMatS6B );
   }
}

void LRL_LatticeMatcher::SetReferenceLattice( const MV_Pair& mpReducedReference ) {
   if ((m_reducedReference - mpReducedReference.GetS6()).norm( ) < 1.0E-4) return;
   m_MVtree.clear( );
   m_reducedReference = mpReducedReference.GetS6();
   m_matReference = mpReducedReference.GetMatS6( );
   BuildReferenceTree( m_reducedReference );
}

void LRL_LatticeMatcher::SetReferenceLattice( const S6& newReference) {
   if ((newReference - m_reducedReference).norm( ) < 1.0E-4) return;
   SetReferenceLattice( MV_Pair( newReference, MatS6( ).unit( ) ) );
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

double LRL_LatticeMatcher::DistanceBetween( const S6& s1, const S6& s2 ) {
   return DBL_MAX;
}

MV_Pair LRL_LatticeMatcher::FindClosest(const S6& reference) const {
   MV_Pair closest;
   const Scaler_MV scale(m_reducedReference);
   const S6 scaled_s = scale.Scale(reference);
   if (!scaled_s.IsValid()) throw;
   const bool b = m_MVtree.NearestNeighbor(DBL_MAX, closest, MV_Pair(scaled_s, MatS6()));
   if (!b) throw;
   return closest;
}

std::vector<MV_Pair> LRL_LatticeMatcher::FindNearToClosest(const double d, const MV_Pair& sample) const {
   std::vector<MV_Pair> vClosest;
   const long n = m_MVtree.FindInSphere(1.01 * d + 0.1, vClosest, sample);
   return vClosest;
}

S6 LRL_LatticeMatcher::FindBestAmongMany(const std::vector<MV_Pair>& vClosest, const S6& reference) const {
   S6 s6closest;
   double dmin_G6 = DBL_MAX;
   for (size_t i = 0; i < vClosest.size(); ++i) {
      const S6 s6test = vClosest[i].GetMatS6() * reference;
      if (!s6test.IsValid()) throw;
      const double d2 = (s6test - m_reducedReference).norm();
      double dtest_G6 = (G6(m_reducedReference) - G6(s6test)).norm();
      dtest_G6 = d2;  // NOTE: IGNORING g6

      if (dmin_G6 >= dtest_G6) {
         dmin_G6 = dtest_G6;
         s6closest = s6test;
      }
   }
   //if (!s6closest.IsValid()) throw;
   return s6closest;
}

S6 LRL_LatticeMatcher::MatchReference( const S6& sample ) const {
   const static bool debug = false;
   MV_Pair closest = FindClosest(sample);

   const double d = (closest.GetMatS6()*sample - m_reducedReference).norm( );  //LCA IS THE MATRIX RIGHT ??????????????????????

   const std::vector<MV_Pair> vClosest = FindNearToClosest(d, MV_Pair(sample,MatS6()));
   //if (vClosest.empty()) throw;

   for (size_t i = 0; i < vClosest.size(); ++i) if (!vClosest[i].GetS6().IsValid()) throw;
   return FindBestAmongMany(vClosest, sample);
}

std::vector<S6> LRL_LatticeMatcher::MatchReference( const std::vector<S6>& vSamples ) const {
   std::vector<S6> v;
   for (size_t i = 0; i < vSamples.size(); ++i) {
      if (!vSamples[i].IsValid()) throw;
      const S6 intermediate = MatchReference(vSamples[i]);
      //if (!intermediate.IsValid()) throw;
      const S6 result = m_matReference * MatchReference(vSamples[i]);
      v.push_back(result);
      //if (!result.IsValid()) throw;
   }
   return v;
}

void LRL_LatticeMatcher::StoreMV_IfUnique( const S6& s, const MatS6& m ) {
   MV_Pair scaled_MV( s, m );
   scaled_MV = MV_Pair::CleanZeros( scaled_MV );
   m_MVtree.insert( scaled_MV );
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
   m_MVtree.clear( );
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
