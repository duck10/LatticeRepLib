#include "DeloneFitResults.h"


DeloneFitResults::DeloneFitResults(const double fit, const S6& bestApprox, const S6& perpv, const MatS6& toCanon)
   : m_rawFit(fit)
   , m_zscore(-19191.0)
   , m_bestFit(bestApprox)
   , m_difference(perpv)
   , m_toCanonicalDeloneType(toCanon)
{
}
