#ifndef REDUCER_H
#define REDUCER_H

#if _MSC_VER
#pragma once
#endif // _MSC_VER

#include "MatG6.h"
#include "G6.h"

/*  class Reducer
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
A class to implement Niggli reduction in the G6 space. Reduce returns a reduced cell
and the matrix that converts the input cell to the reduced cell.

Reduced(void)
== constructor -- nothing to do here
static void Reduce( const G6& vi, MatG6& m, G6& vout, const double delta )
== returns vout as the reduced cell of vi and m, the conversion matrix
static bool NearRed( const G6& gvec, const double delta );
== determines whether gvec is reduced within delta
static bool Near2Red( const G6& gvec, const double delta, G6& vout, double& dist )
== determines whether gvec is reduced within delta, and returns the reduced cell
and how far from reduced
static void Reporter( const std::string& text, const G6& vin, const G6& vout, const MatG6& m )
== prints information about each step in reduction (including standard presentation)

private:
static void MKnorm( const G6& vi, MatG6& m, G6& vout, const double delta  )
== internal function to convert vi to standard presentation - the matrix
to implement that change is returned
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

class Reducer {
public:
   static bool Reduce( const G6& vi, MatG6& m, G6& vout, const double delta );
   static G6 Reduce( const G6& vi );
   static bool NearRed( const G6& gvec, const double delta );
   static bool Near2Red( const G6& gvec, const double delta, G6& vout, double& dist );
   static void MKnorm( const G6& vi, MatG6& m, G6& vout, const double delta );
   static G6 Niggli2Delone( const G6& v );
   std::vector<double> Get4D_Vector( const G6& v );

   template<typename TVEC, typename TMAT>
   static bool Reduce( const TVEC& vi, TMAT& m, TVEC& vout, const double delta ) {
      G6 vtemp;
      MatG6 mtemp;
      mtemp = mtemp.Eye();
      const bool b = Reduce( G6(vi), mtemp, vtemp, delta );
      vout = TVEC(vtemp);
      m = TMAT( mtemp );
      return b;
   }


private:
   static void Reporter( const std::string& text, const G6& vin, const G6& vout, const MatG6& m );

   // at least for now, all functions are static, and there is no member data
   // forbid constructor and destructor
   Reducer( void );
   ~Reducer( void );

public:

};

#endif // REDUCER_H

