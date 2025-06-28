#ifndef REDUCER_H
#define REDUCER_H

#include "LRL_Vector3.h"
#include "MatG6.h"
#include "G6.h"

/*  class Niggli
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

class Niggli {
public:
   ~Niggli(void);
   static bool Reduce(const G6& vi, G6& vout, const bool sellingFirst);
   static bool Reduce(const G6& vi, G6& vout);
   static bool Reduce(const G6& vi, MatG6& m, G6& vout, const double delta);
   static bool NearRed(const G6& gvec, const double delta = 0.0);
   static void MKnorm(const G6& vi, MatG6& m, G6& vout, const double delta);
   static bool IsNiggli(const S6& s);
   static bool IsNiggli(const G6& s, const double delta = 1.0e-6);
   static bool IsNiggli(const D7& s);
   static void MKnormWithoutMatrices(const G6& vi, G6& vout, const double delta);
   static bool ReduceWithoutMatrices(const G6& vi, G6& vout, const double delta);
   static void ShowStoreResults();

   static bool ReduceWithTransforms(const G6& vi, MatG6& mG6, Matrix_3x3& m3d, G6& vout, const double delta=0.0);

   template<typename TVEC>
   static bool Reduce(const TVEC& vi, TVEC& vout) {
      G6 g6out;
      MatG6 mtemp;
      const bool b = Reduce(G6(vi), mtemp, g6out, 0.0);
      vout = g6out;
      return b;
   }

   template<typename TVEC, typename TMAT>
   static bool Reduce(const TVEC& vi, TMAT& m, TVEC& vout, const double delta = 0.0) {
      G6 vtemp;
      MatG6 mtemp;
      mtemp = MatG6::Eye();
      const bool b = Reduce(G6(vi), mtemp, vtemp, delta);
      vout = TVEC(vtemp);
      m = TMAT(mtemp);
      return b;
   }

   static std::string GetName() { return "Niggli"; }
   static size_t GetCycles() { return m_ReductionCycleCount; }

private:
   static void Reporter(const std::string& text, const G6& vin, const G6& vout, const MatG6& m);
   static size_t m_ReductionCycleCount;

   // at least for now, all functions are static, and there is no member data
   // forbid constructor
   Niggli(void);

public:
   static void CheckAllNiggliMatrixDeterminants();

};

#endif // REDUCER_H

