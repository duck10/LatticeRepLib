// RGB.cpp: implementation of the CRGB class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"

#include "RGB.h"
#include <cmath>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const double PI = 4.0F * ::atan( 1.0F );
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CRGB::CRGB( )  :
	m_nRed    (INT_MAX),
	m_nGreen  (INT_MAX),
	m_nBlue   (INT_MAX),
	m_bIsValid(false)
//-----------------------------------------------------------------------------
{
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CRGB::~CRGB( )
//-----------------------------------------------------------------------------
{
	m_nRed     = INT_MAX;
	m_nGreen   = INT_MAX;
	m_nBlue    = INT_MAX;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CRGB::CRGB( const CHSV& hsv ) :
   m_bIsValid( true )
//-----------------------------------------------------------------------------
{
	if ( hsv.m_nSaturation == 0 ) 
	{
		m_nRed   = hsv.m_nValue;
		m_nGreen = hsv.m_nValue;
		m_nBlue  = hsv.m_nValue;
	} 
	else 
	{
      const double fHue = static_cast<double>(hsv.m_nHue)/60.0F;
		const double fValue      = static_cast<double>(hsv.m_nValue) / 255;
		const double fSaturation = static_cast<double>(hsv.m_nSaturation) / 255;
/*
      const double f           = ::fmod(fHue, 2.0);
		const double p           = static_cast<double>(fValue * (1.0F -   fSaturation ));
		const double q           = static_cast<double>(fValue * (1.0F - ( fSaturation * f ) ));
		const double t           = static_cast<double>(fValue * (1.0F - ( fSaturation * ( 1.0F - f ) ) ));

		switch ( static_cast<long>(fHue )%6 )
		{
			case 0:
				red   = fValue;
				green = t;
				blue  = p;
				break;
			case 1:
				red   = q;
				green = fValue;
				blue  = p;
				break;
			case 2:
				red   = p;
				green = fValue;
				blue  = t;
				break;
			case 3:
				red   = p;
				green = q;
				blue  = fValue;
				break;
			case 4:
				red   = t;
				green = p;
				blue  = fValue;
				break;
			case 5:
				red   = fValue;
				green = p;
				blue  = q;
				break;
		}
*/

      //double newF = ::fmod(fHue, 1.0 );
      //if ( static_cast<long>(fHue ) %2 == 0 ) // reverse if even
      //{
      //   newF = 1.0-newF;
      //}
                                   // New calculation by lca  09/06
                                   // No one has used trig before.
                                   // This takes the previous linear,
                                   // sawtooth tranformation and makes
                                   // it smooth. It removes the if test
                                   // that makes the discontinuous 
                                   // slope change.
      const double newF = 0.5F + 0.5F * cos(fHue*PI);  // Just 0 to 1
      const double m    = 1.0F - fSaturation;
      const double n    = 1.0F - fSaturation * newF;

      double red, green, blue;
		switch ( static_cast<long>(fHue ) )
		{
         default:
			case 0:
				red   = 1.0;
				green = n;
				blue  = m;
				break;
			case 1:
				red   = n;
				green = 1.0;
				blue  = m;
				break;
			case 2:
				red   = m;
				green = 1.0;
				blue  = n;
				break;
			case 3:
				red   = m;
				green = n;
				blue  = 1.0;
				break;
			case 4:
				red   = n;
				green = m;
				blue  = 1.0;
				break;
			case 5:
				red   = 1.0;
				green = m;
				blue  = n;
				break;
		}

      m_nRed   = static_cast<int>( fValue * red   * 255.0L);
      m_nGreen = static_cast<int>( fValue * green * 255.0L);
      m_nBlue  = static_cast<int>( fValue * blue  * 255.0L);
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CRGB::CRGB( const int& r, const int& g, const int& b ) :
   m_bIsValid( true ),
	m_nRed   ( r ),
	m_nGreen ( g ),
	m_nBlue  ( b )
//-----------------------------------------------------------------------------
{

	while ( m_nRed   <   0 ) m_nRed   += 255;
	while ( m_nGreen <   0 ) m_nGreen += 255;
	while ( m_nBlue  <   0 ) m_nBlue  += 255;

	while ( m_nRed   > 255 ) m_nRed   -= 255;
	while ( m_nGreen > 255 ) m_nGreen -= 255;
	while ( m_nBlue  > 255 ) m_nBlue  -= 255;
}

////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//CRGB::CRGB(const COLORREF& c) :
//   m_bIsValid( true ),
//   m_nBlue   ( (              c ) >> 16),
//   m_nGreen  ( ( 0x0000FF00 & c ) >> 8),
//   m_nRed    ( ( 0x000000FF & c ))
////---------------------------------------------------------------------
//{
//}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CRGB CRGB::MixRGB(const CRGB& rgb1, const CRGB& rgb2, const double& d)
//---------------------------------------------------------------------
{
   CRGB rgbTemp(rgb1);
   rgbTemp.m_nRed   = static_cast<int>( (1.0-d)*(double)rgb1.m_nRed   + d*(double)rgb2.m_nRed);
   rgbTemp.m_nBlue  = static_cast<int>( (1.0-d)*(double)rgb1.m_nBlue  + d*(double)rgb2.m_nBlue);
   rgbTemp.m_nGreen = static_cast<int>( (1.0-d)*(double)rgb1.m_nGreen + d*(double)rgb2.m_nGreen);
   return( rgbTemp );
}
