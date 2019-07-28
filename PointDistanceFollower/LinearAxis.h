// LinearAxis.h: interface for the LinearAxis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(LINEARAXIS_H)
#define LINEARAXIS_H

#include <cfloat>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>

#include <cstdlib>
#include <cstddef>

//-----------------------------------------------------------------------------
class AxisLimits  
{
   friend class LinearAxis;
public:
   AxisLimits(
                   const double lowerLimit    = DBL_MAX, 
                   const double maxDataValue  = DBL_MAX, 
                   const double minDataValue  = DBL_MAX, 
                   const double upperLimit    = DBL_MAX,
                   const size_t& numSteps      = ULONG_MAX,
                   const size_t& objectScaleFactor = ULONG_MAX);
   AxisLimits& operator=( const AxisLimits& ) const; // no assignment operator allowed

public:
	const double m_lowerLimit;
	const double m_maxDataValue;
	const double m_minDataValue;
	const double m_upperLimit;
	const size_t m_numSteps;
	const size_t m_objectScaleFactor;

};

//-----------------------------------------------------------------------------
class StepSizeList  
{
public:
	StepSizeList( );
	virtual ~StepSizeList( );

	void clear ( );
	size_t size( ) const;
   StepSizeList operator<< ( const double d );
   double operator[ ] ( const size_t& i ) const;
   std::vector<double> m_vStepSizes;
private:
	size_t sort( );

};


//-----------------------------------------------------------------------------
class LinearAxis : private StepSizeList
{
public:
   class Scale;  // forward declaration
   // the operator<< is a friend so that it can use Scale's private member functions
   friend std::ostream& operator<< ( std::ostream& os, const LinearAxis& la );

   //#############################################################################
   //-----------------------------------------------------------------------------
   //           BEGIN NESTED CLASSES
   //-----------------------------------------------------------------------------
   //#############################################################################
      class StepSize
      {
         friend class Scale;
         friend class LinearAxis;
      private:
         explicit StepSize( const double s = DBL_MAX );
         const double m_dStepSize;
         StepSize& operator=( const StepSize& ) const; // no assignment operator allowed
      public:
	     // virtual void SetStepSizeList( ) = 0;
      };
      //-----------------------------------------------------------------------------
      class Modulus
      {
         friend class Scale;
         friend class LinearAxis;
      private:
         explicit Modulus( const double m = DBL_MAX );
         Modulus& operator=( const Modulus& ) const; // no assignment operator allowed
         const double m_dModulus;
      };
      //-----------------------------------------------------------------------------
      class Range
      {
         friend class Scale;
         friend class LinearAxis;
      private:
         explicit Range( const double r = DBL_MAX );
         Range& operator=( const Range& ) const; // no assignment operator allowed
         const double m_dRange;
      };
      //-----------------------------------------------------------------------------
      class Scale
      {
         friend class LinearAxis;
         friend std::ostream& operator<< ( std::ostream& os, const LinearAxis& la );

      public:
	      ~Scale( );
         bool operator< ( const Scale& cs ) const;
	      double GetModulus ( ) const;
	      double GetStepSize( ) const;
	      double GetRange   ( ) const;

      private:
         explicit Scale( 
                       const Modulus&  dModulus   = LinearAxis::Modulus (DBL_MAX), 
                       const Range&    dRange     = LinearAxis::Range   (DBL_MAX), 
                       const StepSize& dStepSize  = LinearAxis::StepSize(DBL_MAX) );
	      double m_dModulus;
	      double m_dRange;
	      double m_dStepSize;
   //#############################################################################
   //-----------------------------------------------------------------------------
   //           END NESTED CLASSES
   //-----------------------------------------------------------------------------
   //#############################################################################

   };

public:
	LinearAxis( );
	StepSizeList m_Steps;
	LinearAxis ( const size_t uiTickMax );
	virtual ~LinearAxis( );
	AxisLimits LinearAxisLimits( const double dMin, const double dMax );
   size_t SetAxisConstants( const size_t uiMaxTicks = 11 );
	virtual void SetStepSizeList( );
   const std::vector<Scale>& GetScale( void ) const { return( m_vScale ); }

protected:

private:
   AxisLimits FindActualLimits( const double dDataMin, const double dDataMax );

   std::vector<Scale> m_vScale;
};


#endif

