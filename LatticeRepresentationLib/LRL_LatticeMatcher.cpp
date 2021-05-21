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


void LRL_LatticeMatcher::PrintMVtree() const {
   for (size_t i = 0; i < m_MVtree.size(); ++i) {
      std::cout << " " << LRL_ToString(m_MVtree[i].GetS6()) << std::endl;
   }
}

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
      for (auto k = m_matrixTree.begin(); k != m_matrixTree.end(); ++k) {
         base.push_back(redc[i].first * (*k));
      }
   }

   m_matrixTree.clear();
   for ( size_t i=0; i<base.size(); ++i)
      m_matrixTree.insert(base[i]);
   for (size_t i = 0; i < refl_one.size(); ++i)
      m_matrixTree.insert(refl_one[i]);

   for (size_t k = 0; k < m_matrixGenerationRecursionDepth; ++k) {
      for (size_t i = 0; i < base.size(); ++i) {
         const size_t msize = m_matrixTree.size();
         for (auto m= m_matrixTree.begin(); m!= m_matrixTree.end(); ++m) {
            const MatS6 temp = base[i] * (*m);
            const bool returnvalue = StoreMatS6IfUnique(temp);
         }
      }
      std::cout << "LRL_LatticeMatcher k = " << k << "  matrix tree size = " << m_matrixTree.size() << std::endl;
   }
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


void LRL_LatticeMatcher::FillReflections() {
   static const std::vector<MatS6> refl = MatS6::GetReflections();
   for ( size_t i=0; i<refl.size(); ++i)
      m_matrixTree.insert(refl[i]);
}

bool LRL_LatticeMatcher::StoreMatS6IfUnique(const MatS6& m) {
   m_matrixTree.insert(m);
   return true;
}

static const std::vector<std::pair<MatS6, MatS6> > redc(CreateReductionMatrices());
static const std::vector<MatS6> refl_one = MatS6::GetReflections();


void LRL_LatticeMatcher::ExpandReflections(const MatS6& m) {
   StoreMatS6IfUnique(m);
   const size_t currentMatrixTreeSize = m_matrixTree.size();
   for (size_t i = 0; i < refl_one.size(); ++i) {
      for (auto k = m_matrixTree.begin(); k != m_matrixTree.end(); ++k) {
         const MatS6 mi = refl_one[i] * (*k);
         StoreMatS6IfUnique(mi);
      }
   }
}

void LRL_LatticeMatcher::ExpandBoundaries(const MatS6& m) {
   StoreMatS6IfUnique(m);
   const size_t currentMatrixTreeSize = m_matrixTree.size();
   for (size_t i = 0; i < redc.size(); ++i) {
      for (auto k = m_matrixTree.begin(); k != m_matrixTree.end(); ++k) {
         const MatS6 mi = redc[i].first * (*k);
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

void LRL_LatticeMatcher::ExpandMatrices(const int n, const MatS6& m) {
   m_matrixTree.insert(m);

   for (size_t i = 0; i < n; ++i) {
      ExpandBoundaries(m);
      ExpandReflections(m);
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

std::set<MatS6> LRL_LatticeMatcher::DoThreeAxes() {
   if (m_useOLD_Algorithm)
      ExpandMatrices_OLD(m_matrixGenerationRecursionDepth, MatS6().unit());
   else
      ExpandMatrices(m_matrixGenerationRecursionDepth, MatS6().unit());

   return m_matrixTree;
}

void LRL_LatticeMatcher::BuildMatrixTree(void) {
   //m_matrixTree.clear();
   if (m_matrixTree.empty()) {
      FillReflections();
      DoThreeAxes();

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

void LRL_LatticeMatcher::SetReferenceLattice(const MV_Pair& mpReducedReference) {
   if ((m_reducedReference - mpReducedReference.GetS6()).norm() < 1.0E-4) return;
   m_MVtree.clear();
   m_reducedReference = mpReducedReference.GetS6();
   //std::cout << "in SetRefLat S6 " << m_reducedReference << "  G6 " << G6(m_reducedReference) << std::endl;
   //std::cout << "in SetRefLat S6 " << m_originalReducedReference << "  G6 " << G6(m_originalReducedReference) << std::endl;
   m_matReference = mpReducedReference.GetMatS6();
   BuildReferenceTree(m_reducedReference);
}

void LRL_LatticeMatcher::SetReferenceLattice(const S6& newReference) {
   m_originalReducedReference = newReference;;
   m_originalReducedReferenceNorm = newReference.Norm();

   if ((newReference - m_reducedReference).norm() < 1.0E-4) return;
   SetReferenceLattice(MV_Pair(newReference, MatS6().unit()));
}

void LRL_LatticeMatcher::BuildReferenceTree(const S6& reference) {
   const double normReference = m_reducedReference.norm();
   const Scaler_MV scale(reference);
   for (auto i = m_matrixTree.begin(); i != m_matrixTree.end(); ++i ) {
      const S6 scaledMV = scale.Scale(*i * reference);
      StoreMV_IfUnique(scaledMV, MatS6::Inverse(*i));
   }
}

void LRL_LatticeMatcher::ApplyReflections(const std::vector<MatS6>& t) {
   static const std::vector<MatS6> refl = MatS6::GetReflections();
   for (size_t i = 0; i < t.size(); ++i) {
      for (size_t k = 0; k < refl.size(); ++k) {
         StoreMatS6IfUnique(refl[k] * t[i]);
      }
   }
}

double LRL_LatticeMatcher::DistanceBetween(const S6& s1, const S6& s2) {
   return DBL_MAX;
}

std::pair<double, MV_Pair> LRL_LatticeMatcher::FindClosest(const S6& sample) const {
   MV_Pair closest;
   const Scaler_MV scale(m_reducedReference);
   const S6 scaled_s = scale.Scale(sample);
   //if (!scaled_s.IsValid()) throw;
   const bool b = m_MVtree.NearestNeighbor(DBL_MAX, closest, MV_Pair(scaled_s, MatS6()));
   const double dist = (scaled_s - closest.GetS6()).norm();
   if (!b) throw("Nothing was within range, so probably m_MVtree is empty");
   return std::make_pair(dist, closest);
}

std::vector<MV_Pair> LRL_LatticeMatcher::FindNearToClosest(const double d, const MV_Pair& sample) const {
   std::vector<MV_Pair> vClosest;
   const long n = m_MVtree.FindInSphere(m_sphereSearchEnlargementfactor * d + 0.02 * sample.GetS6().norm(), vClosest, sample);
   return vClosest;
}

S6 LRL_LatticeMatcher::FindBestAmongMany(const std::vector<MV_Pair>& vClosest, const S6& sample) const {
   S6 s6closest;
   double dmin_G6 = DBL_MAX;
   for (size_t i = 0; i < vClosest.size(); ++i) {
      const S6 s6test = vClosest[i].GetMatS6() * sample;
      if (!s6test.IsValid()) throw("I cannot think how this could happen. The only way is if sample is invalid ");
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
   for (size_t i = 0; i < refl_one.size(); ++i) {
      expandedSample.push_back(refl_one[i] * sample);
      //std::cout << refl_one[i] * sample << refl_one[i] * sample << std::endl;
   }
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

   //const std::vector<S6> expandedSample = ExpandVectorByBoundaries(ExpandVectorByReflections(sample));
   //const std::vector<S6> expandedSample(ExpandVectorByReflections(sample));
   const std::vector<S6> expandedSample(std::vector<S6>(1,sample));

   const std::vector<S6> matches = MatchReference(expandedSample);
   //std::cout << matches.size() << std::endl;

   double best = DBL_MAX;
   S6 bestS6;
   for (size_t i = 0; i < matches.size(); ++i) {
      const double dist = (G6(m_originalReducedReferenceNorm * (*this).GetReference()) - G6(matches[i])).norm();
      //std::cout << "   " << dist << " G6 match, G6 reference  " << G6(matches[i]) << "     " << G6(GetReference()) << std::endl;
      if (dist < best) {
         best = dist;
         bestS6 = matches[i];
      }
   }

   //std::cout << "  result in MatchReference " << best << "     " << bestS6 << std::endl;

   return bestS6;
}

S6 LRL_LatticeMatcher::InternalMatchReference(const S6& sample) const {
   const static bool debug = false;
   std::pair<double, MV_Pair> closest = FindClosest(sample);
   const double d = closest.first;
   const Scaler_MV scale(m_reducedReference);
   const S6 scaled_s = scale.Scale(sample);
   const std::vector<MV_Pair> vClosest = FindNearToClosest(d, MV_Pair(scaled_s, MatS6()));
   if (vClosest.empty()) throw("Is MV_Pair empty?");

   for (size_t i = 0; i < vClosest.size(); ++i) {
      const LRL_Cell_Degrees cell(vClosest[i].GetS6());
      if (!vClosest[i].GetS6().IsValid()) {
         std::cout << " invalid in InternalMatchReference " << i << " " << vClosest[i].GetS6() << std::endl;
         throw("this should not happen, and if it's invalid here, I have no idea how to proceed. It might mean that there is bad data in MV_Pair");
      }
   }

   return FindBestAmongMany(vClosest, sample);
}

std::vector<S6> LRL_LatticeMatcher::MatchReference(const std::vector<S6>& vSamples) const {
   std::vector<S6> v;
   for (size_t i = 0; i < vSamples.size(); ++i) {
      if (!vSamples[i].IsValid()) std::cout << "invalid in MatchReference " << LRL_ToString(vSamples[i].GetVector()) << std::endl;
      if (!vSamples[i].IsValid()) throw;
      const S6 result = m_matReference * InternalMatchReference(vSamples[i]);
      v.push_back(result);
   }
   return v;
}

void LRL_LatticeMatcher::StoreMV_IfUnique(const S6& s, const MatS6& m) {
   MV_Pair scaled_MV(s, m);
   if (!LRL_Cell(s).IsValid()) throw;
   scaled_MV = MV_Pair::CleanZeros(scaled_MV);
   m_MVtree.insert(scaled_MV);
}

MV_Pair MV_Pair::CleanZeros(const MV_Pair& mv) {
   MV_Pair t(mv);
   for (size_t i = 0; i < 6; ++i)
      t.m_s6[i] = (std::abs(mv.m_s6[i]) < 1.0E-10) ? 0.0 : t.m_s6[i];
   return t;
}

std::vector<int> MV_Pair::GetOpenIndices(const MV_Pair& mvp) const {
   std::vector<int> v;
   for (size_t i = (size_t)0; i < (size_t)6; ++i) if (!m_mark[i]) v.push_back((int)i);
   return v;
}

double LRL_LatticeMatcher::GetMaxRadius() const {
   const S6 s("0 0 0 0 0 0");
   MV_Pair mvout;
   const MV_Pair mv(s, MatS6());
   const bool b = m_MVtree.FarthestNeighbor(mvout, mv);
   return mvout.GetS6().Norm();
}

std::ostream& operator<< (std::ostream& o, const MV_Pair& v) {
   o << "MV_Pair" << std::endl;
   o << "input S6 " << v.GetS6() << "  size " << v.m_originalSize << std::endl;
   MatS6 mtemp(v.m_ms6);
   //o << "---------------" << std::endl;
   //o << mtemp << std::endl;
   //o << "---------------" << std::endl;
   o << "matrix times input " << mtemp * v.GetS6() << std::endl;
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
double LMDist::DistanceBetween(const S6& s2) {
   const S6 matched = MatchReference(s2);
   return (m_reducedReference - matched).norm();
}

double LMDist::DistanceBetween(const S6& s1, const S6& s2) {
   SetReferenceLattice(s1);
   m_bestFit = MatchReference(s2);
   const double dist = (m_reducedReference - m_bestFit).norm();
   return dist;
}
