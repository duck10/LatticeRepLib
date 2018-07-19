#ifndef D7DIST_HPP
#define D7DIST_HPP

#include <algorithm>
#include<utility>
#include "D7.h"
/* #include "D7Dist.h" */
extern "C" {
    double D7Dist(double *, double *);
}

static void MakeArray(const D7& g, double *a)
{
   for (size_t i = 0; i < 7; ++i) a[i] = g[i];
}

template<typename T1, typename T2>
static double D7Dist_(const T1& t1, const T2& t2) {
    
    
    // for LCA's boundary code
    //const D7 v7var = D7_DistanceSearch<D7>::SearchOutBestMatch(t1, t2);
    //const double d1a = (t1 - v7var).norm();
    //const double d2a = (t2 - v7var).norm();
    //const double d12a = (t1 - t2).norm();
    
    //return std::min(d1a, d12a);
    
    const D7 d1 = D7(t1);
    const D7 d2 = D7(t2);
    double * array1 = new double[7];
    double * array2 = new double[7];
    double dist;
    MakeArray(d1, array1);
    MakeArray(d2, array2);
    dist = D7Dist(array1, array2);
    delete[] array1;
    delete[] array2;
    return dist;
}


#endif  // D7DIST_HPP