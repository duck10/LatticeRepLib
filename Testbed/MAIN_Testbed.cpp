// Testbed.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

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
   MV_Pair( ) {}
   MV_Pair( const S6& s, const MatS6& m ) : m_s6( s ), m_ms6( m ), m_originalSize( s.norm( ) ) {}
   MV_Pair( const MatS6& m, const S6& s ) : m_s6( s ), m_ms6( m ), m_originalSize( s.norm( ) ) {}
   MV_Pair operator- ( const MV_Pair& mv ) const { MV_Pair mvp; mvp.m_s6 = m_s6 - mv.m_s6; return mvp; }
   MV_Pair operator- ( const S6& s ) const { MV_Pair mvp; mvp.m_s6 = m_s6 - s; return mvp; }
   double norm( const MV_Pair& mv ) const { return mv.m_s6.norm( ); }
   double norm( void ) const { return m_s6.norm( ); }
   S6 GetS6( void ) const { return m_s6; }
   MatS6 GetMatS6( void ) const { return m_ms6; }
   double GetSize( void ) const {return m_originalSize;}

protected:
   S6 m_s6;
   MatS6 m_ms6;
   double m_originalSize;
};


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

void ExpandReflections_MV( const S6& s ) {
   static const std::vector<MatS6> refl = MatS6::GetReflections( );
   const Scaler_MV scale(s);
   for (size_t i = 0; i < refl.size( ); ++i) {
      //std::cout << "p  " << LRL_Cell_Degrees(refl[i] * s) << std::endl;
      const S6 scaled = scale.Scale( refl[i] * s);
      const MV_Pair scaled_MV( scaled, Inverse(refl[i]) );
      if (mvtree.NearestNeighbor( dcutoff, scaled_MV ) == mvtree.end( )) {
         mvtree.insert( scaled_MV );
      }
   }
}

void ExpandBoundaries_MV( ) {
   static const std::vector<std::pair<MatS6, MatS6> > redc = S6::SetUnreductionMatrices( );

   std::vector<S6> v1;
   for (size_t i = 0; i < mvtree.size( ); ++i) v1.push_back( mvtree[i].GetS6( ) );

   const Scaler_MV scale( v1[0] );

   std::string success;
   for (size_t k = 0; k < v1.size( ); ++k) {
      for (size_t i = 0; i < redc.size( ); ++i) {
         //std::cout << "p  " << LRL_Cell_Degrees(redc[i].first * v1[k]) << std::endl;
         const S6 scaled = scale.Scale( redc[i].first * v1[k] );
         const MV_Pair scaled_MV( scaled, Inverse(redc[i].first) );
         if (mvtree.NearestNeighbor( dcutoff, scaled_MV ) == mvtree.end( )) {
            mvtree.insert( scaled_MV );
            success = "succeed ";
         } else {
            success = "failed ";
         }
         std::cout << std::endl << success + "k,i " << k << "  " << i << "  " << std::endl;
         std::cout << "redc[i].first * s " << scaled_MV.GetS6( ) << std::endl;
      }
   }
}

void Expand( const S6& s ) {
   ExpandReflections_MV( s );
   ExpandBoundaries_MV( );
   mvtree;
}

std::vector<S6> MakeListAllSameLength( const std::vector<S6>& vin) {
   const double d = vin[0].norm( );
   std::vector<S6> v;

   for (size_t i = 0; i < vin.size( ); ++i)
      v.push_back( d / vin[i].norm( ) * vin[i] );
   return vin;
   return v;
}

std::vector<S6>  GetReducedInput() {
   const std::vector<LRL_ReadLatticeData> input = GetInputCells( );
   if (input.size( ) < 2) {
      std::cout << "At least two input cells are required" << std::endl;
      exit( 0 );
   }
   const std::vector<S6> vLat = GetInputSellingReducedVectors( input );
   return vLat;
}

int main( int argc, char* argv[] )
{
   const std::vector<S6> vLat =  GetReducedInput( );

   Expand( vLat[0] );
   Scaler_MV scaler( vLat[0] );

   for ( size_t i=0; i<mvtree.size(); ++i ) 
      std::cout << mvtree[i].GetS6() << "      " << LRL_Cell_Degrees( mvtree[i].GetS6( )) << std::endl;

   std::cout << "the closest match" << std::endl;
   static const MatS6 constMatS6 = MatS6();
   for (size_t i = 0; i < vLat.size( ); ++i){
      CNearTree<MV_Pair>::iterator it_MV = mvtree.NearestNeighbor(100000., MV_Pair(vLat[i], constMatS6));
      const S6 test = (*it_MV).GetS6( );
      //std::cout << i << "  " << (vLat[i] - (*it_MV).GetS6( )).norm( ) << std::endl;
      std::cout << i << "  " << (scaler.Scale( vLat[i] ) - (*it_MV).GetS6( )).norm( ) << 
         "  " << LRL_Cell_Degrees((*it_MV).GetMatS6() *  vLat[i] ) << std::endl;
   }

   return 0;
}

/*
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