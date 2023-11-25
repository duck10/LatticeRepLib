// HSV.h: interface for the CHSV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(HSV_H_DEFINED)
#define HSV_H_DEFINED
#define COLORREF unsigned long
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RGB.h"

class CRGB;

class CHSV  
{
public:
	static CHSV RandomColor( void );
	CHSV(const unsigned long c);
	CHSV( void );
	virtual ~CHSV( void );
	CHSV( const int& h, const int& s, const int& v );
	CHSV( const CRGB& rgb );
   //operator COLORREF( void ) const;

	int m_nHue;
	int m_nValue;
	int m_nSaturation;
	bool m_bHueIsDefined;
	const bool m_bIsValid;

   CHSV& operator=(const CHSV& hsv);
public:
   static CHSV MixHSV(const CHSV& hsv1, const CHSV& hsv2, const double& d);
};

#endif // !defined(HSV_H_DEFINED)
