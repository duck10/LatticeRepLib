

// LinearAxis.cpp: implementation of the LinearAxis class.
//
//////////////////////////////////////////////////////////////////////
// copyright larry andrews
// November, 2004

// Humans want to see some limited number of tickmarks at very simple intervals
// So a table was created of number of tickmarks vs. "nice" interval
// sizes (where the nice intervals were chosen to be 1.0, 2.0, and 5.0,
// scaled into the appropriate range). A selection of reasonable 
// pairs was used to generate the tables in this code. The only crucial
// choice is that the last interval must be large enough to hold any
// data that the code might ever see; that way, it should never fail
// to suggest values for an input data range. The other piece of the
// puzzle was to realize that even for nice step sizes, a choice still
// had to be made about the granularity of where to start the plot.
// Those choices are governed by the array "moduli". The other two
// arrays were chosen to be consistent with the array "moduli". In 
// general, the code does better than people do, because people don't
// have an algorithm. For the code today, it is possible to have
// as minimum use of the plotted axis always > 70%. Adding other values to
// the arrays can raise the minimum used; in particular, adding a
// stepsize of 4.0 would help (but people are ambivalent about that
// value). The other, simpler way to raise the percentages filled is
// to decrease the values in the moduli array.

// Ranges are the allowed sizes of the plot windows (on the same
// relative scale as the dMantissa of the data range).
// The data is arranged such that 12 is large enough that any
// data set could be contained within that interval since the
// corresponding value in moduli is 1.0

// Moduli are the allowed granularity of the ranges and of the
// steps sizes and of the starting and ending points of the
// actual ranges set for plotting (on the relative scale of the dMantissa 
// of the data range)

// Stepsizes are intended to be the spacings between tick marks (on the
// relative scale of the dMantissa of the data range). The input data's end
// points will be within stepsize of the ends of the interval.

// USAGE
// One LinearAxis object should be created. Axis scalings are calculated 
// by calling the function, LinearAxisLimits, with the upper and lower
// data values as arguments. The order of the arguments does not matter.
// LinearAxisLimits returns a AxisLimits object whose public member 
// variables (const) are the upper and lower limits of the axis and the
// number of steps. For convenience, the original upper and lower data
// limits are also included in the AxisLimits object. The member data are:
//
// const double m_lowerLimit;
// const double m_maxDataValue;
// const double m_minDataValue;
// const double m_upperLimit;
// const int    m_numSteps;

// Within the LinearAxis constructor, there are compile-time choices for
// changing the step sizes and the maximum number of tick marks that
// the code will generate. 

//      Here's how to derive from LinearAxis:
//      
//      Write a replacement for the virtual function SetStepSizeList
//
//      THEN EITHER
//      make a constructor for the derived class and call SetStepSizeList
//      and then SetAxisConstants
//      OR
//      put a call to SetAxisConstants at the end of your SetStepSizeList and
//      call SetStepSizeList immediately after constructing the derived object
//
#include "LinearAxis.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iomanip>
#include <iostream>

#ifdef _DEBUG
#undef      THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//-----------------------------------------------------------------------------template Max
template <class U, class V>
    U Max (const U Arg1, const V Arg2)
{
    return (Arg2 < Arg1 ? Arg1 : U(Arg2));
};

//-----------------------------------------------------------------------------template Min
template <class U, class V>
    U Min (const U Arg1, const V Arg2)
{
    return (Arg2 > Arg1 ? Arg1 : U(Arg2));
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------constructor StepSizeList
StepSizeList::StepSizeList( )
{
   m_vStepSizes.clear( );
}

//-----------------------------------------------------------------------------destructor StepSizeList
StepSizeList::~StepSizeList( )
{
   m_vStepSizes.clear( );
}

//-----------------------------------------------------------------------------operator<< StepSizeList
// 
// Insert a new step size into the list of sizes that will be used to create the 
// scaling models. Reject zero, and convert negatives to positives. Scale the 
// input value into 1-10, add it to the list, divide it by 10 and add the 
// result to the list. Sort it immediately so that it is ready for 
// SetAxisConstants. Sorting for each entry is an inefficiency, but there
// are not likely to be many step sizes, and, most likely, the generation of
// the list will only be done once. Duplicate step sizes are rejected.
//
StepSizeList StepSizeList::operator<< ( double d )
{
   if ( d == 0.0 )
      return ( *this );
   d = fabs( d );
   // scale the proposed step size into 1-10
   while ( d < 1.0 )
      d *= 10.0;
   while ( d >= 10.0 )
      d /= 10.0;

   if ( std::find ( m_vStepSizes.begin( ), m_vStepSizes.end( ), d      ) == m_vStepSizes.end( ) )
   {
      m_vStepSizes.push_back( d );
   }
   if ( std::find ( m_vStepSizes.begin( ), m_vStepSizes.end( ), d/10.0 ) == m_vStepSizes.end( ) )
   {
      m_vStepSizes.push_back( d / 10.0 );
   }
   this->sort( );
   return ( *this );
}

//-----------------------------------------------------------------------------sort StepSizeList
size_t StepSizeList::sort( )
{
   std::sort( m_vStepSizes.begin( ), m_vStepSizes.end( ) );
   return m_vStepSizes.size( );
}

//-----------------------------------------------------------------------------size StepSizeList
size_t StepSizeList::size( ) const
{
   const size_t n = m_vStepSizes.size();
   return n;
}

//-----------------------------------------------------------------------------operator[] StepSizeList
double StepSizeList::operator [ ]( const size_t& i ) const
{
   return ( m_vStepSizes[ i ] );
}

//-----------------------------------------------------------------------------clear StepSizeList
void StepSizeList::clear( )
{
   m_vStepSizes.clear( );
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------constructor CAxisLimts
//
// default values are set to DBL_MAX and INT_MAX in the declaration
//
AxisLimits::AxisLimits(
                         const double lowerLimit, 
                         const double maxDataValue, 
                         const double minDataValue, 
                         const double upperLimit,
                         const size_t& numSteps,
                         const size_t& objectScaleFactor )

   : m_lowerLimit   ( lowerLimit   ) 
   , m_maxDataValue ( maxDataValue ) 
   , m_minDataValue ( minDataValue ) 
   , m_upperLimit   ( upperLimit   ) 
   , m_numSteps     ( numSteps     )
   , m_objectScaleFactor( objectScaleFactor)
{ }


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------constructor LinearAxis
// default uiTickLimit is set to 11 in the declaration
//
LinearAxis::LinearAxis( const size_t uiTickLimit )
{
   m_vScale.clear( );
   m_Steps .clear( );
   LinearAxis::SetStepSizeList( );
   const size_t uiNumberOfScalingObjectsCreated = SetAxisConstants( uiTickLimit );
   uiNumberOfScalingObjectsCreated;
}

//-----------------------------------------------------------------------------destructor LinearAxis
LinearAxis::~LinearAxis( )
{
   m_vScale.clear( );
}
AxisLimits LinearAxis::FindActualLimits( const double dDataMin, const double dDataMax )
{
   double dRexponent = floor( log10(dDataMax-dDataMin) ); // get the exponent for the dMantissa in 1-10
   double dExpTerm   = pow(10.0,dRexponent);              // get the exponential term for scientific notation
   double dMantissa  = (dDataMax-dDataMin) / dExpTerm;    // get the dMantissa in 1-10
   double dRangeMin=DBL_MAX;
   double dRangeMax=-DBL_MAX; // set dRangeMax so that the first test will fail
   int i=0;

   // find a range that can possibly contain the input data
   while ((m_vScale[i].GetRange( )<dMantissa) && (i<(static_cast<int>(m_vScale.size( )))))
   {
      i++;
   }

   // now find a range that actually will work
   // subtract one because we'll start by adding one 
   --i;
   i = Max( -1, i );

   // make sure the range limits are both outside the data range
   while ( (dRangeMax<dDataMax) && (i < (static_cast<int>( m_vScale.size( )-1 ))) )
   {
      i ++;
      // move the range minimum value to the next modulus value (scaled into data range)
      const double dMod = m_vScale[i].GetModulus( )*dExpTerm;
      dRangeMin = dDataMin - fmod( dDataMin, dMod );
      if ( dRangeMin > dDataMin )
         dRangeMin -= dMod;
      dRangeMax = dRangeMin + m_vScale[i].GetRange( )*dExpTerm;
   }

   const int numSteps = static_cast<int> ( m_vScale[i].GetRange( ) / m_vScale[i].GetStepSize( ) + 0.5 );
   return ( AxisLimits(   dRangeMin, dDataMax, dDataMin, dRangeMax, numSteps, i ) );
}

//-----------------------------------------------------------------------------LinearAxisLimits
//
// This is the part of the algorithm that searches in the list of scaling
// objects prepared by SetAxisConstants for the first one that fits the 
// needs of the current case. The ranges are sorted in ascending order, so
// a loop searches from the beginning, looking for the first range that could
// even possibly contain both the data minimum and the data maximum.
//
AxisLimits LinearAxis::LinearAxisLimits( const double dDataMin, const double dDataMax )
{
   if ( m_Steps.size( ) == 0 || m_Steps.m_vStepSizes.empty( ) )
   {
      SetStepSizeList( );
   }

   if ( m_vScale.size( ) == 0 )
   {
      SetAxisConstants( );
   }

   if (dDataMin < dDataMax)
   {
      return( FindActualLimits( dDataMin, dDataMax ) );
   }
   // deal with wrong order of limits
   else if ( dDataMin > dDataMax )
   {
      return ( LinearAxisLimits( dDataMax, dDataMin ) );
   }
   // deal with limits equal and zero
   else if   ( (dDataMin == 0.0) && (dDataMax == 0.0) )
   {
      return ( LinearAxisLimits ( -1.0, 1.0 ) );
   }
   // deal with equal, non-zero limits
   else // if (dDataMin == dDataMax)
   {
      return( LinearAxisLimits( 0.9*dDataMax, 1.1*dDataMax ) );
   }
}

//-----------------------------------------------------------------------------constructor Scale
//
// default values are set to DBL_MAX in the declaration
//
LinearAxis::Scale::Scale( 
                    const Modulus&  modulus, 
                    const Range&    range, 
                    const StepSize& stepsize )
   : m_dModulus ( modulus .m_dModulus )
   , m_dRange   ( range   .m_dRange )
   , m_dStepSize( stepsize.m_dStepSize )
{ }

//-----------------------------------------------------------------------------destructor Scale
LinearAxis::Scale::~Scale( )
{
}

//-----------------------------------------------------------------------------operator< Scale
// operator< for sorting the scaling data
// the ranges will be ascending
//
// for equal ranges, the stepsizes will be descending
// for equal ranges and stepsizes, the moduli will be ascending
//
bool LinearAxis::Scale::operator< ( const Scale& cs ) const
{
   if ( m_dRange < cs.m_dRange )
   {
      return ( true );
   }
   else if ( m_dRange > cs.m_dRange )
   {
      return ( false );
   }
   else if ( m_dStepSize > cs.m_dStepSize )
   {
      return ( true );
   }
   else if ( m_dStepSize < cs.m_dStepSize )
   {
      const int iStepsThis = static_cast<int> ( m_dRange / m_dStepSize + 0.1 );
      const int iStepscs   = static_cast<int> ( cs.m_dRange / cs.m_dStepSize + 0.1 );
      return ( iStepsThis < iStepscs );
   }
   else if ( m_dModulus > cs.m_dModulus )
   {
      return ( true );
   }
   else
   {
      return ( false );
   }
}

//-----------------------------------------------------------------------------constructor Modulus
//
// default value is set to DBL_MAX in the declaration
//
LinearAxis::Modulus::Modulus ( const double m )
: m_dModulus( m ) { }

//-----------------------------------------------------------------------------constructor Range
//
// default value is set to DBL_MAX in the declaration
//
LinearAxis::Range::Range( const double r )
: m_dRange( r ) { }

//-----------------------------------------------------------------------------constructor StepSize
//
// default value is set to DBL_MAX in the declaration
//
LinearAxis::StepSize::StepSize( const double s )
: m_dStepSize( s ) { }

//-----------------------------------------------------------------------------GetModulus
double LinearAxis::Scale::GetModulus( ) const
{   return ( m_dModulus ); }

//-----------------------------------------------------------------------------GetStepSize
double LinearAxis::Scale::GetStepSize( ) const
{   return ( m_dStepSize ); }

//-----------------------------------------------------------------------------GetRange
double LinearAxis::Scale::GetRange( ) const
{   return ( m_dRange ) ;}

//-----------------------------------------------------------------------------operator<< (text to stream)
//
// just send the scaling objects to output for checking
//
std::ostream& operator<< ( std::ostream& os, const LinearAxis& la )
{
   const size_t iTemp              = la.GetScale( ).size( );
   const std::streamsize Precision = os.precision( );
   const std::streamsize Width     = os.width( );
   os << "i,  range,  step size, modulus, steps" << std::endl;
   os << std::setw( 3 );
   os << std::setprecision( 3 );
   os << std::showpoint;
   for ( size_t i=0; i<iTemp; ++i )
   {
      os << std::setw( 3 );
      os << i << "  ";
      os << std::setw( 5 );
      os << std::setprecision( 3 );
      os << la.GetScale( )[i].GetRange( ) << "  ";
      os << std::setw( 6 );
      os << std::setprecision( 2 );
      os << la.GetScale( )[i].GetStepSize( ) << "   ";
      os << std::setprecision( 2 );
      os << std::setw( 7 );
      os << la.GetScale( )[i].GetModulus( ) << " ";
      os << std::setw( 6 );
      os << int(la.GetScale( )[i].GetRange( ) / la.GetScale( )[i].GetStepSize( )+0.1);
      os << std::endl;
   }
   os.precision( Precision );
   os.width( Width );
   return ( os );
}

//-----------------------------------------------------------------------------SetAxisConstants
//
// build and sort the scaling objects, based on the list of step sizes
// assigned by SetStepSizeList.
//
size_t LinearAxis::SetAxisConstants( const size_t uiMaxTicks )
{
   // If the user didn't build his own list or didn't call the default list
   // to assign a maximum number of steps, then just use a default one.
   if ( this->m_Steps.size( ) == 0 )
   {
      SetStepSizeList( );
   }
   // If there's already a list, just delete it.
   m_vScale.clear( );
   // generate the scaling data
   size_t ulCount = 0;
   const size_t uiMinTicks = Min( 4u, uiMaxTicks );
   size_t ni = m_Steps.size( );
   ni;
   for( size_t iStep=0; iStep<m_Steps.size( ); ++iStep )
   {
      for( size_t iTicks=uiMinTicks; iTicks<=uiMaxTicks; ++iTicks )
      {
         double dModulus=DBL_MAX;
         const double dRangeTemp = iTicks * m_Steps[iStep];
         if ( dRangeTemp>=1.0 && dRangeTemp<=10+3*m_Steps[iStep] )
         {
            dModulus = m_Steps[iStep];
            ++ ulCount;
            m_vScale.push_back( Scale( 
                                    Modulus ( dModulus ), 
                                    Range   ( dRangeTemp ), 
                                    StepSize( m_Steps[iStep] ) ) );
            if ( m_Steps[iStep] == 0.2 )
            {
               dModulus = 0.1;
               ++ ulCount;
               m_vScale.push_back( Scale( 
                                       Modulus ( dModulus ), 
                                       Range   ( dRangeTemp ), 
                                       StepSize( m_Steps[iStep] ) ) );
            }
            if ( m_Steps[iStep] == 0.4 )
            {
               dModulus = 0.2;
               ++ ulCount;
               m_vScale.push_back( Scale( 
                                       Modulus ( dModulus ), 
                                       Range   ( dRangeTemp ), 
                                       StepSize( m_Steps[iStep] ) ) );
            }
            if ( m_Steps[iStep] == 2.0 )
            {
               dModulus = 1.0;
               ++ ulCount;
               m_vScale.push_back( Scale( 
                                       Modulus ( dModulus ), 
                                       Range   ( dRangeTemp ), 
                                       StepSize( m_Steps[iStep] ) ) );
            }
            if ( m_Steps[iStep] == 4.0 )
            {
               dModulus = 2.0;
               ++ ulCount;
               m_vScale.push_back( Scale( 
                                       Modulus ( dModulus ), 
                                       Range   ( dRangeTemp ), 
                                       StepSize( m_Steps[iStep] ) ) );
            }
         }
      }
   }

   // Emergency backup to catch trouble (never seen with original data).
   if ( m_vScale.empty( ) )
   {
      // Catch pathological cases for too few tick marks specified.
      m_vScale.push_back( Scale( Modulus( 1.0 ), Range( 10.0 ), StepSize( 10.0 ) ) );
      ++ ulCount;
   }

   // Sort the scaling data, most important sort: ascending range values.
   std::sort( m_vScale.begin( ), m_vScale.end( ) );

   return ( ulCount );

}

//-----------------------------------------------------------------------------SetStepSizeList
//
// Compose a vector of the step sizes that will be used (in the range 1.0 to 10).
//
void LinearAxis::SetStepSizeList( )
{
   // a few constants that cause or inhibit certain step sizes
   // Here's where you would add other step sizes you'd like to have.
   const bool  bAllowStepSizeEqualPointTwoFive  = false;
   const bool  bAllowStepSizeEqualPointThree    = false;
   const bool  bAllowStepSizeEqualPointFour     = false;
   const bool  bAllowStepSizeEqualPointSix      = false;

   if ( bAllowStepSizeEqualPointTwoFive )
   {
      m_Steps << 2.5;
   }
   if ( bAllowStepSizeEqualPointThree )
   {
      m_Steps << 3.0;
   }
   if ( bAllowStepSizeEqualPointFour )
   {
      m_Steps << 4.0;
   }
   if ( bAllowStepSizeEqualPointSix )
   {
      m_Steps << 6.0;
   }

   // Now just the "standard" step sizes
   m_Steps <<  2.0;
   m_Steps <<  5.0;  // to catch the case of only 2 steps total !
   m_Steps <<  1.0;
   m_Steps << 10.0;  // to catch the case of only 2 steps total !
}

//-----------------------------------------------------------------------------constructor LinearAxis
LinearAxis::LinearAxis( )
{
   m_vScale.clear( );
   m_Steps .clear( );
}

