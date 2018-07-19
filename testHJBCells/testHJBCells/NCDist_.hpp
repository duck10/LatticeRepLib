#ifndef NCDIST_HPP
#define NCDIST_HPP

#include "G6.h"
/* #include "NCDist.h" */
extern "C" {
    double NCDist(double *, double *);
}

static void MakeArray(const G6& g, double * a)
{
   for (size_t i = 0; i < 6; ++i) a[i] = g[i];
}

template<typename T1, typename T2>
double NCDist_(const T1& t1, const T2& t2)
{
    const G6 g1 = G6(t1);
    const G6 g2 = G6(t2);
    double * array1 = new double[6];
    double * array2 = new double[6];
    double dist;
    MakeArray(g1, array1);
    MakeArray(g2, array2);
    dist = NCDist(array1, array2);
    delete[] array1;
    delete[] array2;
    return dist;
    
}


#endif  // NCDIST_HPP