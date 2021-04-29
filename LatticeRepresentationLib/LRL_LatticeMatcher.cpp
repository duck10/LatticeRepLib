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


std::vector<std::pair<MatS6, MatS6> > CreateReductionMatrices() {
   std::vector<std::pair<MatS6, MatS6> > m(1, std::make_pair(MatS6().unit(), MatS6().unit()));
   static const std::vector<std::pair<MatS6, MatS6> > redc = S6::SetUnreductionMatrices();
   m.insert(m.end(), redc.begin(), redc.end());
   return m;
}

void PrintMatrices(const std::vector<MatS6>& mv) {
   for (size_t i = 0; i < mv.size(); ++i)
      std::cout << std::endl << std::endl << mv[i] << std::endl;
}

std::vector<MatS6> LRL_LatticeMatcher::BuildMatrixBase() {
   static const std::vector<std::pair<MatS6, MatS6> > redc(CreateReductionMatrices());
   static const std::vector<MatS6> refl_one = MatS6::GetReflections();

   m_matrixTree.clear();

   // Create the seed sequence
   m_matrixTree.insert(redc[0].first);

   // Expand the seed sequence 
   std::vector<MatS6> base;
   for (size_t i = 0; i < redc.size(); ++i) {
      const size_t msize = m_matrixTree.size();
      for (size_t m = 0; m < msize; ++m) {
         base.push_back(redc[i].first * m_matrixTree[m]);
      }
   }

   m_matrixTree.clear();
   m_matrixTree.insert(base);
   m_matrixTree.insert(refl_one);
   std::cout << "LRL_LatticeMatcher before loops " << "  matrix tree size = " << m_matrixTree.size() << std::endl;
   std::cout << "LRL_LatticeMatcher before loops " << "  base size = " << base.size() << std::endl;

   for (size_t k = 0; k < m_matrixGenerationRecursionDepth; ++k) {
      for (size_t i = 0; i < base.size(); ++i) {
         const size_t msize = m_matrixTree.size();
         for (size_t m = 0; m < msize; ++m) {
            const MatS6 temp = base[i] * m_matrixTree[m];
            const bool returnvalue = StoreMatS6IfUnique(temp);
         }
      }
      std::cout << "LRL_LatticeMatcher k = " << k << "  matrix tree size = " << m_matrixTree.size() << std::endl;
   }
   //exit(0);
   return base;
}

LRL_LatticeMatcher::LRL_LatticeMatcher()
   : m_MVtree()
   , m_matrixTree()
   , m_reducedReference(S6())
   , dcutoff(0.01)
   , m_matrixGenerationRecursionDepth(2)
   , m_sphereSearchEnlargementfactor(2.0)
   , m_useOLD_Algorithm(false)
{

   BuildMatrixBase();

   //BuildMatrixTree();
}

LRL_LatticeMatcher::LRL_LatticeMatcher(const int matrixGenerationRecursionDepth,
   const double sphereSearchEnlargementfactor,
   const bool m_useOLD_Algorithm
)
   : m_MVtree()
   , m_matrixTree()
   , m_reducedReference(S6())
   , dcutoff(0.01)
   , m_matrixGenerationRecursionDepth(matrixGenerationRecursionDepth)
   , m_sphereSearchEnlargementfactor(sphereSearchEnlargementfactor)
   , m_useOLD_Algorithm(m_useOLD_Algorithm)
{
   BuildMatrixBase();

   //BuildMatrixTree();
}


void LRL_LatticeMatcher::FillReflections( ) {
   static const std::vector<MatS6> refl = MatS6::GetReflections( );
   if (m_matrixTree.empty( ))
      m_matrixTree.insert( refl );
}

bool LRL_LatticeMatcher::StoreMatS6IfUnique( const MatS6& m) {
   if (m_matrixTree.NearestNeighbor(dcutoff, m) == m_matrixTree.end()) {
      m_matrixTree.insert(m);
      if ((m_matrixTree.size() % 1000) == 0) std::cout << " tree size " << m_matrixTree.size() << std::endl;
      return true;
   }
   return false;
}

static const std::vector<std::pair<MatS6, MatS6> > redc( CreateReductionMatrices( ) );
static const std::vector<MatS6> refl_one = MatS6::GetReflections( );


void LRL_LatticeMatcher::ExpandReflections(const MatS6& m) {
   StoreMatS6IfUnique(m);
      const size_t currentMatrixTreeSize = m_matrixTree.size();
   for (size_t i = 0; i < refl_one.size(); ++i) {
      for (size_t k = 0; k < currentMatrixTreeSize; ++k) {
         const MatS6 mi = refl_one[i] * m_matrixTree[k];
         StoreMatS6IfUnique(mi);
      }
   }
}

void LRL_LatticeMatcher::ExpandBoundaries(const MatS6& m) {
   StoreMatS6IfUnique(m);
      const size_t currentMatrixTreeSize = m_matrixTree.size();
   for (size_t i = 0; i < redc.size(); ++i) {
      for (size_t k = 0; k < currentMatrixTreeSize; ++k) {
         const MatS6 mi = redc[i].first * m_matrixTree[k];
         StoreMatS6IfUnique(mi);
      }
   }
}

void LRL_LatticeMatcher::MultiplyAndExpand(const int n, const MatS6& transform, const MatS6& m) {
   const MatS6 mi = transform * m;
   StoreMatS6IfUnique(mi);
   if (n > 1) ExpandMatrices_OLD(n - 1, mi);
}

void LRL_LatticeMatcher::MultiplyAndExpand_OLD(const int n, const MatS6& transform, const MatS6& m) {
   const MatS6 mi = transform * m;
   StoreMatS6IfUnique(mi);
   if (n > 1) ExpandMatrices_OLD(n - 1, mi);
}

void LRL_LatticeMatcher::ExpandMatrices( const int n, const MatS6& m ) {
   m_matrixTree.insert(m);

   for (size_t i = 0; i < n; ++i) {
      ExpandBoundaries( m);
      ExpandReflections( m);
   }
}

void LRL_LatticeMatcher::ExpandMatrices_OLD(const int n, const MatS6& m) {
   for (size_t i = 0; i < refl_one.size(); ++i) {
      MultiplyAndExpand(n, refl_one[i], m);
   }

   for (size_t i = 0; i < redc.size(); ++i) {
      MultiplyAndExpand(n, redc[i].first, m);
   }
}

std::vector<MatS6> LRL_LatticeMatcher::DoThreeAxes( ) {
   std::cout << " in DoThreeAxesA, matrix tree size = " << m_matrixTree.size() << "  recursionDepth = " << m_matrixGenerationRecursionDepth << std::endl;
 
   if (m_useOLD_Algorithm)
      ExpandMatrices_OLD(m_matrixGenerationRecursionDepth, MatS6().unit());
   else
      ExpandMatrices(m_matrixGenerationRecursionDepth, MatS6().unit());

   return m_matrixTree.GetObjectStore();
}

void LRL_LatticeMatcher::BuildMatrixTree( void ) {
   //m_matrixTree.clear();
   if (m_matrixTree.empty( )) {
      FillReflections( );
      DoThreeAxes( );
      std::cout << " finishing BuildMatrixTree, matrix tree size = " << m_matrixTree.size() << "  recursionDepth = " << m_matrixGenerationRecursionDepth << std::endl;

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

void LRL_LatticeMatcher::BuildReferenceTree( const S6& reference) {
   const double normReference = m_reducedReference.norm( );
   const Scaler_MV scale(reference);
   for (size_t i = 0; i<m_matrixTree.size(); ++i ) {
      const S6 scaledMV = scale.Scale(m_matrixTree[i] * reference);
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

std::pair<double, MV_Pair> LRL_LatticeMatcher::FindClosest(const S6& sample) const {
   MV_Pair closest;
   const Scaler_MV scale(m_reducedReference);
   const S6 scaled_s = scale.Scale(sample);
   //if (!scaled_s.IsValid()) throw;
   const bool b = m_MVtree.NearestNeighbor(DBL_MAX, closest, MV_Pair(scaled_s, MatS6()));
   const double dist = (scaled_s - closest.GetS6()).norm();
   if (!b) throw;
   return std::make_pair(dist, closest);
}

std::vector<MV_Pair> LRL_LatticeMatcher::FindNearToClosest(const double d, const MV_Pair& sample) const {
   std::vector<MV_Pair> vClosest;
   const long n = m_MVtree.FindInSphere(m_sphereSearchEnlargementfactor * d + 0.02*sample.GetS6().norm(), vClosest, sample);
   return vClosest;
}

S6 LRL_LatticeMatcher::FindBestAmongMany(const std::vector<MV_Pair>& vClosest, const S6& sample) const {
   S6 s6closest;
   double dmin_G6 = DBL_MAX;
   for (size_t i = 0; i < vClosest.size(); ++i) {
      const S6 s6test = vClosest[i].GetMatS6() * sample;
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

std::vector<S6> ExpandVectorByReflections(const S6& sample) {  // should check for duplicates
   std::vector<S6> expandedSample;
   for (size_t i = 0; i < refl_one.size(); ++i)
      expandedSample.push_back(refl_one[i] * sample);
   return expandedSample;
}

std::vector<S6> ExpandVectorByBoundaries(const std::vector<S6>& sampleList) {  // should check for duplicates
   std::vector<S6> expandedSample;
   for (size_t i = 0; i < redc.size(); ++i)
      for (size_t k = 0; k < sampleList.size(); ++k)
         expandedSample.push_back(redc[i].first * sampleList[k]);
   return expandedSample;
}

S6 LRL_LatticeMatcher::MatchReference(const S6& sample) const {
   const std::vector<S6> expandedSample = ExpandVectorByBoundaries(ExpandVectorByReflections(sample));
   const std::vector<S6> matches = MatchReference(expandedSample);
   double best = DBL_MAX;
   S6 bestS6;
   for (size_t i = 0; i < matches.size(); ++i) {
      const double dist = ((*this).GetReference() - matches[i]).norm();
      if (dist < best) {
         best = dist;
         bestS6 = matches[i];
      }
   }
   return bestS6;
}

S6 LRL_LatticeMatcher::InternalMatchReference(const S6& sample) const {
   const static bool debug = false;
   std::pair<double, MV_Pair> closest = FindClosest(sample);

   const double d = closest.first;
   const Scaler_MV scale(m_reducedReference);
   const S6 scaled_s = scale.Scale(sample);
   const std::vector<MV_Pair> vClosest = FindNearToClosest(d, MV_Pair(scaled_s, MatS6()));
   //if (vClosest.empty()) throw;

   for (size_t i = 0; i < vClosest.size(); ++i) if (!vClosest[i].GetS6().IsValid()) throw;
   return FindBestAmongMany(vClosest, sample);
}

std::vector<S6> LRL_LatticeMatcher::MatchReference( const std::vector<S6>& vSamples ) const {
   std::vector<S6> v;
   for (size_t i = 0; i < vSamples.size(); ++i) {
      if (!vSamples[i].IsValid()) throw;
      const S6 result = m_matReference * InternalMatchReference(vSamples[i]);
      v.push_back(result);
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

LMDist::LMDist(const int recursionDepth, const double sphereEnlargement, const bool m_useOLD_Algorithm) 
: LRL_LatticeMatcher(recursionDepth, sphereEnlargement, m_useOLD_Algorithm)
{
   m_MVtree.clear();
   BuildMatrixTree();
   BuildReferenceTree(m_reducedReference);
   SetRecursionDepth(recursionDepth);
   SetSphereEnlargementFactor(sphereEnlargement);
}

LMDist::LMDist() {}

//LMDist::LMDist(const S6& s) {
//   SetReferenceLattice(MV_Pair(s, MatS6().unit()));
//   m_MVtree.clear();
//   BuildMatrixTree();
//   BuildReferenceTree(m_reducedReference);
//   SetRecursionDepth(3);
//   SetSphereEnlargementFactor(4.0);
//
//}
//
double LMDist::DistanceBetween( const S6& s2 ) {
   const S6 matched = MatchReference( s2 );
   return (m_reducedReference - matched).norm( );
}

double LMDist::DistanceBetween( const S6& s1, const S6& s2 ) {
   SetReferenceLattice( s1 );
   m_bestFit = MatchReference( s2 );
   const double dist = (m_reducedReference - m_bestFit).norm( );
   //std::cout << "in LMDist::DistanceBetween " << dist << std::endl 
   //   << " " << C3(s1) << std::endl
   //   << " " << C3(s2) << std::endl
   //   << " " << C3(m_reducedReference) << std::endl
   //   << " " << C3(m_bestFit) << std::endl << std::endl;
   return dist;
}
