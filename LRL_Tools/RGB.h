// RGB.h: interface for the CRGB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(RGB_H_DEFINED)
#define RGB_H_DEFINED

#define COLORREF unsigned long
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSV.h"

class CHSV;

class CRGB  
{
public:
	//CRGB( const COLORREF& c );
	CRGB( const int& r, const int& g, const int& b );
	CRGB( const CHSV& hsv );
	CRGB( void );
	virtual ~CRGB( void );

	const bool m_bIsValid;
	int m_nGreen;
	int m_nBlue;
	int m_nRed;

public:
   static CRGB MixRGB(const CRGB& rgb1, const CRGB& rgb2, const double& d);
};

#endif // !defined(RGB_H_DEFINED)
