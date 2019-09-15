// Testbed.cpp : Defines the entry point for the console application.
//

#include <iomanip>
#include <sstream>

#include "ColorTables.h"

#include "CellInputData.h"
#include "CS6Dist.h"
#include "G6.h"
#include "Delone.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_CreateFileName.h"
#include "LRL_DataToSVG.h"
#include "LRL_inverse.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "LRL_MaximaTools.h"
#include "MatS6.h"
#include "LRL_MaximaTools.h"
#include "S6.h"

#include "Selling.h"
#include "StoreResults.h"
#include "TNear.h"


class Scaler_MV {
public:
   Scaler_MV( const S6& s ) :base(s), dbase(base.norm()) {}
   S6 Scale( const S6& s ) const { return dbase / s.norm( ) * s; }
   S6 Scale( void ) const { return Scale(base); }
protected:
   S6 base;
   double dbase;
};

class MV_Pair {
public:
   friend std::ostream& operator<< ( std::ostream& o, const C3& v );

   MV_Pair() {}
   MV_Pair( const S6& s, const MatS6& m ) : m_s6( s ), m_ms6( m ), m_originalSize( s.norm( ) ), m_mark(6,false) {m_ms6 = ResetZeros(m_ms6);}
   MV_Pair( const MatS6& m, const S6& s ) : m_s6( s ), m_ms6( m ), m_originalSize( s.norm( ) ), m_mark(6,false) {m_ms6 = ResetZeros(m_ms6);}
   MV_Pair operator- ( const MV_Pair& mv ) const { MV_Pair mvp; mvp.m_s6 = m_s6 - mv.m_s6; return mvp; }
   MV_Pair operator- ( const S6& s ) const { MV_Pair mvp; mvp.m_s6 = m_s6 - s; return mvp; }
   double norm( const MV_Pair& mv ) const { return mv.m_s6.norm( ); }
   double norm( void ) const { return m_s6.norm( ); }
   S6 GetS6( void ) const { return m_s6; }
   MatS6 GetMatS6( void ) const { return m_ms6; }
   double GetSize( void ) const {return m_originalSize;}
   static MatS6 ResetZeros( const MatS6& m ) { MatS6 mt( m ); for (size_t i = 0; i < 6; ++i) if (m[i] == 0.0) mt[i] = 0.0; return mt; }
   static MV_Pair CleanZeros( const MV_Pair& mv ) { 
      MV_Pair t( mv ); 
      for (size_t i=0; i<6; ++i ) 
         t.m_s6[i] = (abs( mv.m_s6[i] ) < 1.0E-10) ?  0.0 : t.m_s6[i];
      return t;
   }
   void SetMark( const int i ) { m_mark[i] = true; }
   bool GetMark( const int i ) const { return m_mark[i]; }
   bool ClearMarks( void ) { for (size_t i = 0; i < 6; ++i) m_mark[i] = false; }
   std::vector<int> GetOpenIndeces( const MV_Pair& mvp ) const { 
      std::vector<int> v;
      for (size_t i = 0; i < 6; ++i) if (!m_mark[i]) v.push_back( i ); 
      return v;
   }
public:
   S6 m_s6;
   MatS6 m_ms6;
   double m_originalSize;
   std::vector<bool> m_mark;
};

std::ostream& operator<< ( std::ostream& o, const MV_Pair& v ) {
   o << "MV_Pair" << std::endl;
   o << "input S6 " << v.GetS6( ) << "  size " << v.m_originalSize << std::endl;
   MatS6 mtemp( v.m_ms6 );
   //o << "---------------" << std::endl;
   //o << mtemp << std::endl;
   //o << "---------------" << std::endl;
   o << "matrix times input " <<  mtemp * v.GetS6( ) << std::endl;
   o << "---------------" << std::endl;

   return o;
}


CNearTree<MV_Pair> mvtree;

static const double dcutoff = 0.01;



static MatS6 Inverse( const MatS6& min ) {
   MatS6 m( min );
   std::vector<double> arout( 36 );
   inverse( 6, min.GetVector( ).data( ), arout.data( ) );
   m.SetVector( arout );
   return m;
}


std::string Letters( void ) {
   return "V,G,D,S,P,A,B,C,I,F,R,C3,G6,S6,B4,D7,H";
}

std::vector<LRL_ReadLatticeData> GetInputCells( void ) {
   const std::string letters = Letters( );
   std::string lattice;
   std::vector<LRL_ReadLatticeData> cellDataList;
   LRL_ReadLatticeData rcd;
   while (lattice != "EOF") {
      rcd.read( );
      lattice = rcd.GetLattice( );
      if ((!lattice.empty( )) && (letters.find( static_cast<char>(toupper( lattice[0] )) ) != std::string::npos))
         cellDataList.push_back( rcd );
   }

   return cellDataList;
}

std::vector<S6> GetInputSellingReducedVectors( const std::vector<LRL_ReadLatticeData>& input ) {
   std::vector<S6> v;
   LatticeConverter converter;


   for (size_t i = 0; i < input.size( ); ++i) {
      const S6 s6 = converter.SellingReduceCell( input[i].GetLattice( ), input[i].GetCell( ) );
      v.push_back( s6 );
   }
   return v;
}

std::vector<S6>  GetReducedInput( ) {
   const std::vector<LRL_ReadLatticeData> input = GetInputCells( );
   if (input.size( ) < 2) {
      std::cout << "At least two input cells are required" << std::endl;
      exit( 0 );
   }
   const std::vector<S6> vLat = GetInputSellingReducedVectors( input );
   return vLat;
}

void StoreMV_IfUnique( const S6& s, const MatS6& m ) {
   MV_Pair scaled_MV( s, m );
   scaled_MV = MV_Pair::CleanZeros( scaled_MV );
   if (mvtree.NearestNeighbor( dcutoff, scaled_MV ) == mvtree.end( )) {
      mvtree.insert( scaled_MV );
   }
}

void ExpandReflections_MV( const MV_Pair& mv ) {
   static const std::vector<MatS6> refl = MatS6::GetReflections( );
   const Scaler_MV scale( mv.GetS6() );
   for (size_t i = 0; i < refl.size( ); ++i) {
      //std::cout << "p  " << LRL_Cell_Degrees(refl[i] * s) << std::endl;
      const S6 scaledS6 = scale.Scale( refl[i] * mv.GetS6() );
      StoreMV_IfUnique( scaledS6, mv.GetMatS6() * Inverse( refl[i] ) );
   }
}

void ExpandReflections_MV( const S6& s ) {
   static const std::vector<MatS6> refl = MatS6::GetReflections( );
   const Scaler_MV scale(s);
   for (size_t i = 0; i < refl.size( ); ++i) {
      const S6 scaledS6 = scale.Scale( refl[i] * s);
      StoreMV_IfUnique( scaledS6, Inverse( refl[i] ) );
   }
}

void ExpandBoundaries_MV( ) {
   static const std::vector<std::pair<MatS6, MatS6> > redc = S6::SetUnreductionMatrices( );

   std::vector<S6> v1;
   std::vector<MatS6> m1;
   for (size_t i = 0; i < mvtree.size( ); ++i) {
      v1.push_back( mvtree[i].GetS6( ) );
      m1.push_back( ( mvtree[i].GetMatS6( ) ) );
   }

   const Scaler_MV scale( v1[0] );

   for (size_t k = 0; k < v1.size( ); ++k) {
      for (size_t i = 0; i < redc.size( ); ++i) {
         const S6 scaledS6 = scale.Scale( redc[i].first * v1[k] );
<<<<<<< HEAD
         const  MatS6 totalTransform = redc[i].first * m1[k];

         StoreMV_IfUnique( scaledS6, Inverse( totalTransform ) );
         //const MV_Pair scaled_MV( scaledS6, Inverse( totalTransform ) );
         //if (mvtree.NearestNeighbor( dcutoff, scaled_MV ) == mvtree.end( )) {
         //   mvtree.insert( scaled_MV );
         //}
=======
         const  MatS6 totalTransform = m1[k] * Inverse( redc[i].first );
         const MV_Pair scaled_MV( scaledS6, totalTransform );
         if (mvtree.NearestNeighbor( dcutoff, scaled_MV ) == mvtree.end( )) {
            mvtree.insert( scaled_MV );
         }
>>>>>>> eb7b2280e095be86dfd2e9d60b33f75597cffa37
      }
   }
}

void Expand( const S6& s ) {
   ExpandReflections_MV( s );

   ExpandBoundaries_MV( );
}

void Expand_MV_2( const std::string& label, const MV_Pair& mvp ) {
   const S6 s6( mvp.GetS6( ) );
   if (s6.IsValid( )) {
      switch (s6.CountZeros( )) {
      case 0:
         ExpandReflections_MV( mvp );
         break;
      case 1:
         OneBound( label, mvp );
         break;
      case 2:
      case 3:
         ProcessZeros( label, mvp );
         break;
      default:
         throw "this should never happen";
         break;
      }
   }
   //store.ShowResults();
}

void ReportMultiMatch( const Scaler_MV& scaler, const double closestDistance, const S6& toFind, const std::vector<MV_Pair>& sphere ) {
   static const MatS6 constMatS6 = MatS6( );
   std::cout << "closestDistance " << closestDistance << std::endl;
   for ( size_t i=0; i<sphere.size(); ++i ) {
      std::cout << i << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
      std::cout << sphere[i] << std::endl;
      std::cout << "  d=" << closestDistance <<
         "     " << sphere[i].GetMatS6( ) * toFind << "      " << LRL_Cell_Degrees( sphere[i].GetMatS6( ) * toFind ) << std::endl;
   }
}

void ReportOneMatch( const Scaler_MV& scaler, const S6& toFind ) {
   static const MatS6 constMatS6 = MatS6( );
   CNearTree<MV_Pair>::iterator it_MV = mvtree.NearestNeighbor( 100000., MV_Pair( toFind, constMatS6 ) );
   const double closestDistance = (scaler.Scale( toFind ) - (*it_MV).GetS6( )).norm( );
   std::vector<MV_Pair> sphere;
   const long n = mvtree.FindInSphere( 1.01 * closestDistance, sphere, MV_Pair( toFind, constMatS6 ) );
   if (n == 1) {
      std::cout << "tree index " << it_MV.get_position( ) << "  " << closestDistance <<
         "     " << (*it_MV).GetMatS6( ) * toFind << "      " << LRL_Cell_Degrees( (*it_MV).GetMatS6( ) * toFind ) << std::endl;
   }
   else {
      std::cout << "WARNING ######################################################################## found more than one match  n=" << n << std::endl;
      ReportMultiMatch( scaler, closestDistance, toFind, sphere );
   }
}

void DoAllMatches( const std::vector<S6>& vLat ) {
   const Scaler_MV scaler( vLat[0] );

   for (size_t i = 0; i < vLat.size( ); ++i) {
      std::cout << std::endl << "search item index " << i << std::endl;
      ReportOneMatch( scaler, vLat[i] );
      std::cout << "===================================================================================================" << std::endl;
   }
}

int main( int argc, char* argv[] )
{
   const std::vector<S6> vLat =  GetReducedInput( );

   Expand( vLat[0] );

   DoAllMatches( vLat );

   return 0;
}

/*

f 10 10.001 10.002  89 89 89
f 10 10 10  90 90 90
f 11 10 10  90 90 90
f 10 11 10  90 90 90  this is the problem one
f 10 10 11  90 90 90
end



f 10 10.001 10.002  90.01 90.02 90.03
f 10 10 10  90 90 90
f 11 10 10  90 90 90
f 10 11 10  90 90 90  this is the problem one
f 10 10 11  90 90 90
end


f 10 10.001 10.002  90 90 90
f 10 10 10  90 90 90
f 11 10 10  90 90 90
f 10 11 10  90 90 90  this is the problem one
f 10 10 11  90 90 90
end


f 10 10 10  90 90 90
f 10 10 10  90 90 90
f 11 10 10  90 90 90
f 10 11 10  90 90 90  this is the problem one
f 10 10 11  90 90 90
end



f 10 10 10  90 90 90
i 10 10 10  90 90 90
p  10 10 10  90 90 90
f 10 10 10  90 90 90
f 10 10 11  90 90 90
end

f 10 10 10  90 90 90
f 10 10 10  90 90 90
f 10 10 11  90 90 90
end

c 10 20 30 90 118 90
p   27.14100  11.18034  26.80144     93.90767 158.64514  95.59396
p   27.14100  11.18034  26.80144     93.90767 158.64514  95.59396
p   27.14100  11.18034  11.18034    126.86990  95.59396  95.59396
p   27.14100  26.80144  11.18034     93.90767  95.59396 158.64514
p   27.14100  26.80144  11.18034     93.90767  95.59396 158.64514
p   27.14100  11.18034  11.18034    126.86990  95.59396  95.59396
p   27.14100  11.18034  26.80144     93.90767 158.64514  95.59396
p   11.18034  27.14100  26.80144    158.64514  93.90767  95.59396
p   11.18034  27.14100  11.18034     95.59396 126.86990  95.59396
p   11.18034  26.80144  27.14100    158.64514  95.59396  93.90767
p   11.18034  26.80144  11.18034     93.90767 126.86990  93.90767
p   11.18034  11.18034  27.14100     95.59396  95.59396 126.86990
p   11.18034  11.18034  26.80144     93.90767  93.90767 126.86990
p   26.80144  27.14100  11.18034     95.59396  93.90767 158.64514
p   26.80144  27.14100  11.18034     95.59396  93.90767 158.64514
p   26.80144  11.18034  27.14100     95.59396 158.64514  93.90767
p   26.80144  11.18034  11.18034    126.86990  93.90767  93.90767
p   26.80144  11.18034  27.14100     95.59396 158.64514  93.90767
p   26.80144  11.18034  11.18034    126.86990  93.90767  93.90767
p   11.18034  27.14100  11.18034     95.59396 126.86990  95.59396
p   11.18034  27.14100  26.80144    158.64514  93.90767  95.59396
p   11.18034  11.18034  27.14100     95.59396  95.59396 126.86990
p   11.18034  11.18034  26.80144     93.90767  93.90767 126.86990
p   11.18034  26.80144  27.14100    158.64514  95.59396  93.90767
p   11.18034  26.80144  11.18034     93.90767 126.86990  93.90767
p   28.32800  11.18034  26.80144     86.09233 156.81153 107.53456
p   27.14100  28.32800  26.80144    156.81153  21.35486 156.87148
p   27.14100  11.18034  10.00000    116.56505 102.58967  84.40604
p   27.14100  28.32800  10.00000     88.14729 102.58967 156.87148
p   28.32800  11.18034  28.32800    109.28080 138.65723 107.53456
p   10.00000  28.32800  26.80144    156.81153  98.76519  91.85271
p   28.32800  11.18034  11.18034     53.13010 109.28080 107.53456
p   27.14100  28.32800  11.18034    109.28080  84.40604 156.87148
p   27.14100  11.18034  28.32800    109.28080 156.87148  84.40604
p   27.14100  28.32800  28.32800     41.34277 156.87148 156.87148
p   28.32800  11.18034  10.00000    116.56505  91.85271 107.53456
p   28.32800  10.00000  11.18034    116.56505 107.53456  91.85271
p   10.00000  26.80144  11.18034     86.09233 116.56505  98.76519
p   27.14100  10.00000  11.18034    116.56505  84.40604 102.58967
p   27.14100  26.80144  28.32800    156.81153 156.87148  21.35486
p   27.14100  10.00000  28.32800     88.14729 156.87148 102.58967
p   10.00000  26.80144  28.32800    156.81153  91.85271  98.76519
p   28.32800  28.32800  11.18034    109.28080 107.53456 138.65723
p   28.32800  27.14100  26.80144     21.35486 156.81153 156.87148
p   11.18034  28.32800  26.80144    156.81153  86.09233 107.53456
p   11.18034  27.14100  28.32800    156.87148 109.28080  84.40604
p   11.18034  28.32800  28.32800    138.65723 109.28080 107.53456
p   28.32800  27.14100  10.00000    102.58967  88.14729 156.87148
p   28.32800  10.00000  26.80144     98.76519 156.81153  91.85271
p   28.32800  27.14100  11.18034     84.40604 109.28080 156.87148
p   11.18034  28.32800  11.18034    109.28080  53.13010 107.53456
p   11.18034  27.14100  10.00000    102.58967 116.56505  84.40604
p   11.18034  28.32800  10.00000     91.85271 116.56505 107.53456
p   28.32800  27.14100  28.32800    156.87148  41.34277 156.87148
p   10.00000  28.32800  11.18034    107.53456 116.56505  91.85271
p   28.32800  26.80144  27.14100     21.35486 156.87148 156.81153
p   11.18034  28.32800  27.14100    156.87148  84.40604 109.28080
p   11.18034  26.80144  28.32800    156.81153 107.53456  86.09233
p   11.18034  28.32800  28.32800    138.65723 107.53456 109.28080
p   28.32800  26.80144  10.00000     98.76519  91.85271 156.81153
p   28.32800  10.00000  27.14100    102.58967 156.87148  88.14729
p   28.32800  26.80144  11.18034     86.09233 107.53456 156.81153
p   11.18034  28.32800  11.18034    107.53456  53.13010 109.28080
p   11.18034  26.80144  10.00000     98.76519 116.56505  86.09233
p   11.18034  28.32800  10.00000     88.14729 116.56505 109.28080
p   28.32800  26.80144  28.32800    156.81153  41.34277 156.81153
p   10.00000  28.32800  11.18034    109.28080 116.56505  88.14729
p   10.00000  11.18034  27.14100     84.40604 102.58967 116.56505
p   11.18034  10.00000  27.14100    102.58967  84.40604 116.56505
p   11.18034  11.18034  28.32800    109.28080 107.53456  53.13010
p   11.18034  10.00000  28.32800     91.85271 107.53456 116.56505
p   10.00000  11.18034  28.32800    107.53456  91.85271 116.56505
p   28.32800  28.32800  27.14100    156.87148 156.87148  41.34277
p   10.00000  11.18034  26.80144     86.09233  98.76519 116.56505
p   11.18034  10.00000  26.80144     98.76519  86.09233 116.56505
p   11.18034  11.18034  28.32800    107.53456 109.28080  53.13010
p   11.18034  10.00000  28.32800     88.14729 109.28080 116.56505
p   10.00000  11.18034  28.32800    109.28080  88.14729 116.56505
p   28.32800  28.32800  26.80144    156.81153 156.81153  41.34277
p   10.00000  27.14100  11.18034     84.40604 116.56505 102.58967
p   26.80144  10.00000  11.18034    116.56505  86.09233  98.76519
p   26.80144  27.14100  28.32800    156.87148 156.81153  21.35486
p   26.80144  10.00000  28.32800     91.85271 156.81153  98.76519
p   10.00000  27.14100  28.32800    156.87148  88.14729 102.58967
p   28.32800  28.32800  11.18034    107.53456 109.28080 138.65723
p   28.32800  11.18034  27.14100     84.40604 156.87148 109.28080
p   26.80144  28.32800  27.14100    156.87148  21.35486 156.81153
p   26.80144  11.18034  10.00000    116.56505  98.76519  86.09233
p   26.80144  28.32800  10.00000     91.85271  98.76519 156.81153
p   28.32800  11.18034  28.32800    107.53456 138.65723 109.28080
p   10.00000  28.32800  27.14100    156.87148 102.58967  88.14729
p   28.32800  11.18034  11.18034     53.13010 107.53456 109.28080
p   26.80144  28.32800  11.18034    107.53456  86.09233 156.81153
p   26.80144  11.18034  28.32800    107.53456 156.81153  86.09233
p   26.80144  28.32800  28.32800     41.34277 156.81153 156.81153
p   28.32800  11.18034  10.00000    116.56505  88.14729 109.28080
p   28.32800  10.00000  11.18034    116.56505 109.28080  88.14729
end

*/