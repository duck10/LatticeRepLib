#ifndef LRL_LATTICEMATCHER_H
#define LRL_LATTICEMATCHER_H

#include "MatS6.h"
#include "S6.h"
#include "TNear.h"

#include <set>

class Scaler_MV {
public:
   Scaler_MV( const S6& s ) :base( s ), dbase( base.norm( ) ) {}
   S6 Scale( const S6& s ) const { return dbase / s.norm( ) * s; }
   S6 Scale( void ) const { return Scale( base ); }
protected:
   S6 base;
   double dbase;
};

class MV_Pair {
public:
   friend std::ostream& operator<< ( std::ostream& o, const C3& v );

   MV_Pair( ) {}
   MV_Pair( const S6& s, const MatS6& m ) : m_s6( s ), m_ms6( m ), m_originalSize( s.norm( ) ), m_mark( 6, false ) { m_ms6 = ResetZeros( m_ms6 ); }
   MV_Pair( const MatS6& m, const S6& s ) : m_s6( s ), m_ms6( m ), m_originalSize( s.norm( ) ), m_mark( 6, false ) { m_ms6 = ResetZeros( m_ms6 ); }
   ~MV_Pair( ) { m_mark.clear( ); }
   MV_Pair operator- ( const MV_Pair& mv ) const { MV_Pair mvp; mvp.m_s6 = m_s6 - mv.m_s6; return mvp; }
   MV_Pair operator- ( const S6& s ) const { MV_Pair mvp; mvp.m_s6 = m_s6 - s; return mvp; }
   double norm( const MV_Pair& mv ) const { return mv.m_s6.norm( ); }
   double norm( void ) const { return m_s6.norm( ); }
   S6 GetS6( void ) const { return m_s6; }
   MatS6 GetMatS6( void ) const { return m_ms6; }
   double GetSize( void ) const { return m_originalSize; }
   static MatS6 ResetZeros( const MatS6& m ) { MatS6 mt( m ); for (size_t i = 0; i < 6; ++i) if (m[i] == 0.0) mt[i] = 0.0; return mt; }
   static MV_Pair CleanZeros( const MV_Pair& mv );
   void SetMark( const int i ) { m_mark[i] = true; }
   bool GetMark( const int i ) const { return m_mark[i]; }
   void ClearMarks( void ) { for (size_t i = 0; i < 6; ++i) m_mark[i] = false; }
   std::vector<int> GetOpenIndices( const MV_Pair& mvp ) const;

public:
   S6 m_s6;
   MatS6 m_ms6;
   double m_originalSize;
   std::vector<bool> m_mark;
};

class LRL_LatticeMatcher {
public:
   LRL_LatticeMatcher( );
   LRL_LatticeMatcher(const int matrixGenerationRecursionDepth,
      const double sphereSearchEnlargementfactor,
      const bool m_useOLD_Algorithm=false);
   std::vector<MatS6> BuildMatrixBase();

   void SetReferenceLattice( const MV_Pair& mvp );
   void SetReferenceLattice( const S6& s ); // for distance calculation
   S6 GetReference( ) const { return m_reducedReference; };
   MatS6 GetReferenceMatrix() const { return m_matReference; }
   std::vector<MV_Pair> FindNearToClosest(const double d, const MV_Pair& closest) const;
   S6 FindBestAmongMany(const std::vector<MV_Pair>& vClosest, const S6& closest) const;
   S6 MatchReference(const S6& s);
   std::vector<S6> MatchReference(const std::vector<S6>& samples, const MatS6& mat_reference);
   std::vector<S6> MatchReferenceForExpandedSample( const std::vector<S6>& vs );
   S6 InternalMatchReference(const S6& s);
   std::pair<double, MV_Pair> FindClosest(const S6& s) const;
   void ApplyReflections( const std::vector<MatS6>& t );
   size_t size( ) const { return m_MVtree.size( ); }
   double DistanceBetween( const S6& s1, const S6& s2 );
   void BuildMatrixTree( );
   S6 GetBestMatch(void) const { return m_bestFit; }
   void SetRecursionDepth(const int i) { m_matrixGenerationRecursionDepth = i; }
   void SetSphereEnlargementFactor(const double d) { m_sphereSearchEnlargementfactor = d; }
   void SetUseOldAlgorithm(const bool b) { m_useOLD_Algorithm = b; }
   CNearTree<MV_Pair> GetMatrixTree() const { return m_MVtree; };
   int GetRecursionDepth() const { return m_matrixGenerationRecursionDepth; }
   void PrintMVtree() const;
   double GetMaxRadius() const;
   double Angle6(const S6& s1, const S6& s2) const;
   void SetNewAngleAgreement( double angle);
   std::vector<double> GetAngleAgreements(void) const { return m_angleAgreement; }

protected: // functions
   void BuildReferenceTree( const S6& s );
   void MultiplyAndExpand(const int n, const MatS6& transform, const MatS6& m);
   void MultiplyAndExpand_OLD(const int n, const MatS6& transform, const MatS6& m);
   void ExpandMatrices(const int n, const MatS6& m);
   void ExpandMatrices_OLD(const int n, const MatS6& m);
   std::set<MatS6> DoThreeAxes( );
   void StoreMV_IfUnique( const S6& s, const MatS6& m );
   void FillReflections( );
   bool StoreMatS6IfUnique( const MatS6& m );
   void ExpandReflections(const MatS6& m);
   void ExpandBoundaries(const MatS6& m);

protected: // member data
   CNearTree<MV_Pair> m_MVtree;
   std::set<MatS6> m_matrixTree;
   S6 m_reducedReference;
   S6 m_originalReducedReference;
   double m_originalReducedReferenceNorm;
   MatS6 m_matReference;
   double dcutoff;
   S6 m_bestFit;

   int m_matrixGenerationRecursionDepth;
   double m_sphereSearchEnlargementfactor;
   bool m_useOLD_Algorithm;
   std::vector<double> m_angleAgreement;
};


class LMDist : public LRL_LatticeMatcher {
public:
   LMDist();
   LMDist(const int recursionDepth, const double sphereEnlargement, const bool b_UseOLD_Algorthm = false);
   double DistanceBetween( const S6& s2 );
   double DistanceBetween( const S6& s1, const S6& s2 );
};

#endif  //  LRL_LATTICEMATCHER_H