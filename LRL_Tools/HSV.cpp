// HSV.cpp: implementation of the CHSV class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"

#include <cmath>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include "HSV.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CHSV::CHSV( void )  :
	m_nHue          ( INT_MAX ),
	m_nValue        ( INT_MAX ),
	m_nSaturation   ( INT_MAX ),
	m_bHueIsDefined ( false ),
	m_bIsValid      ( false )
//-----------------------------------------------------------------------------
{
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CHSV::~CHSV( void )
//-----------------------------------------------------------------------------
{
	m_nHue          = INT_MAX;
	m_nValue        = INT_MAX;
	m_nSaturation   = INT_MAX;
	m_bHueIsDefined = false;
	//m_bIsValid      = false;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CHSV::CHSV( const CRGB& rgb ) :
	m_bIsValid( true )
//-----------------------------------------------------------------------------
{
	m_bHueIsDefined = true;

	short iMax, iMin;
	// Calculate the value component
   // note that microsoft has coopted the min and max functions
	iMax = static_cast<short>(( rgb.m_nRed  > rgb.m_nGreen ) ? rgb.m_nRed  : rgb.m_nGreen);
	iMax = static_cast<short>(( rgb.m_nBlue >         iMax ) ? rgb.m_nBlue : iMax);
	iMin = static_cast<short>(( rgb.m_nRed  < rgb.m_nGreen ) ? rgb.m_nRed  : rgb.m_nGreen);
	iMin = static_cast<short>(( rgb.m_nBlue <         iMin ) ? rgb.m_nBlue : iMin);

	m_nValue = iMax;
	if (iMax != 0) // Calculate the saturation component
	{
		m_nSaturation = int( 255 * static_cast<long>(iMax - iMin) / static_cast<float>(iMax) + 0.5F );  // added float to make more stable
		while ( m_nSaturation > 255)
      {
         m_nSaturation -= 255;
      }
	} 
	else 
	{
		m_nSaturation = 0;
	}
	
	float fHue;
	const float fDelta = static_cast<float>( iMax - iMin );
	if ( m_nSaturation == 0) // Calculate the hue
	{
		m_bHueIsDefined = false;
		fHue            = 0;
	} 
	else if (rgb.m_nRed == iMax)        // between magenta and yellow
	{
		fHue = static_cast<float>( rgb.m_nGreen - rgb.m_nBlue  ) / fDelta;
	} 
	else if (rgb.m_nGreen == iMax) // between cyan and yellow -- 2.0F*60 = 120 degrees
	{
		fHue = 2.0F + ( rgb.m_nBlue  - rgb.m_nRed   ) / fDelta;
	}
	else                           // between  magenta and cyan -- 4.0F*60 = 240 degrees
	{
		fHue = 4.0F + ( rgb.m_nRed   - rgb.m_nGreen ) / fDelta;
	}
	fHue *= 60.0F;
	while ( fHue < 0.0F )
		fHue += 360.0F;
	m_nHue = static_cast<int>( fHue + 0.5F ) % 360;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CHSV::CHSV( const int &h, const int &s, const int &v ) :
	m_bIsValid( true )
//-----------------------------------------------------------------------------
{
	m_nHue          = h;
	m_nValue        = v;
	m_nSaturation   = s;
	m_bHueIsDefined = ( m_nSaturation != 0 );

	while ( m_nHue        <   0 ) m_nHue        += 360;
	while ( m_nValue      <   0 ) m_nValue      += 255;
	while ( m_nSaturation <   0 ) m_nSaturation += 255;
	while ( m_nHue        > 360 ) m_nHue        -= 360;
	while ( m_nValue      > 255 ) m_nValue      -= 255;
	while ( m_nSaturation > 255 ) m_nSaturation -= 255;
}

////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//CHSV CHSV::RandomColor( void )
////-----------------------------------------------------------------------------
//{
//	// keep Value greater than ~0.2 (relative) (the eye doesn't distinguish blacks well)
//	// need to be absolutely sure that black ( RGB(0,0,0) ) never occurs since that is 
//	// the bound color used to isolate regions, and on low color-resolution
//	// systems you don't want nearest color to be black
//
//	const long MINIMUMVALUESETTING = static_cast<long>(0.2F * 255.0F);
//	const double dvd = static_cast<double>(255L - MINIMUMVALUESETTING);
//
//   const long lHue        = static_cast<long>( 359.0F * static_cast<double>(::rand( )) ) / RAND_MAX;                   ::rand( );
//	// sqrt is correct here
//   const long lSaturation = static_cast<long>( 255.0F * ::sqrt( static_cast<double>( ::rand( ) ) / RAND_MAX ) ); ::rand( );
//	// just for effect, bias value towards full saturation using sqrt, so it's more colorful
//   // removed 09/06
//   const long lValue      = MINIMUMVALUESETTING + static_cast<long>( dvd   * ::sqrt( static_cast<double>( ::rand( ) ) / RAND_MAX ) ); 
//
//	return( CHSV( lHue, lSaturation, lValue ) );
//}

CHSV::CHSV(const unsigned long c) :
m_bIsValid(true)
//-----------------------------------------------------------------------------
{
	//const long blue = (c) >> 16;
	//const long green = (0x0000FF00 & c) >> 8;
	//const long red = (0x000000FF & c);
	const long blue = c & 0xFF;
	const long green = (c >> 8) & 0xFF;
	const long red = c >> 16;
	const CRGB rgb = CRGB(red, green, blue);
	const CHSV hsv = rgb;
	m_nHue = hsv.m_nHue;
	m_nSaturation = hsv.m_nSaturation;
	m_nValue = hsv.m_nValue;
	std::cout << "in CHSV() c= " << c << std::endl;
	//std::cout << std::hex << std::setfill('0') << std::setw(6) << mincol << std::endl;
	std::cout << " rgb " << red << " " << green << " " << blue << std::endl;
	std::cout << " hsv " << m_nHue << " " << m_nSaturation << " " << m_nValue << std::endl;

}

////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//CHSV::operator COLORREF( void ) const
////-----------------------------------------------------------------------------
//{
//   CRGB rgb( *this );
//   return( CRGB( rgb.m_nRed, rgb.m_nGreen, rgb.m_nBlue ) );
//}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CHSV& CHSV::operator= ( const CHSV& hsv )
//-----------------------------------------------------------------------------
{
   hsv;
   // if ( this != &hsv )
      return( *this );
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CHSV CHSV::MixHSV(const CHSV& hsv1, const CHSV& hsv2, const double& d)
//-----------------------------------------------------------------------------
{
   CHSV hsvTemp(hsv1);
   const double sine1 =   ::sin(hsv1.m_nHue/360.0*2.0*3.14159);
   const double sine2 =   ::sin(hsv2.m_nHue/360.0*2.0*3.14159);
   const double cosine1 = ::cos(hsv1.m_nHue/360.0*2.0*3.14159);
   const double cosine2 = ::cos(hsv2.m_nHue/360.0*2.0*3.14159);
   //const double dTemp = ::atan2( ( (1.0-d)*sine1+d*sine2 ) , ( (1.0-d)*cosine1+d*cosine2 ) );
   hsvTemp.m_nHue = static_cast<int>( 360.0/2/3.14159*::atan2( ( (1.0-d)*sine1+d*sine2 ) , ( (1.0-d)*cosine1+d*cosine2 ) ) );
   if ( hsvTemp.m_nHue < 0 ) hsvTemp.m_nHue += 360;
   hsvTemp.m_nSaturation = static_cast<int>((1.0-d)*hsv1.m_nSaturation + d*hsv2.m_nSaturation );
   hsvTemp.m_nValue = static_cast<int>((1.0-d)*hsv1.m_nValue + d*hsv2.m_nValue );
   return( hsvTemp );
}
