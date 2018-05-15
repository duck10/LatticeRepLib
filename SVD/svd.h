#ifndef SVD_H
#define SVD_H

/*
this is svd from the internet
in the following:
T2 is std::vector<double>
T1 is std::vector<std::vector<double> >

*/

#include <vector>


#include <cmath>
#include <algorithm>
#include <vector>
#include <utility>

template<typename T>
double sqr(const T a) { return a * a; }

template<typename T1, typename T2>
double pythag(const T1 a, const T2 b)
{
   return sqrt(sqr(a) + sqr(b));
}

template<typename T>
double sign(const T a, const T b) {
   return (b<0.0) ? -a : a;
}

template<typename T1, typename T2>
T2 SetNewVector(const T1& t1) {
   T2 t2(t1.size());
   for (unsigned int i = 0; i<t1.size(); ++i)
      t2[i] = t1[i];
   return t2;
}

template<typename T1, typename T2>
T2 SetNewVectorOfVectors(const T1& t1) {
   T2 t2(t1.size());
   for (unsigned int i = 0; i<t1.size(); ++i)
      t2[i].resize(t1[0].size());
   for (unsigned int i = 0; i<t1.size(); ++i)
      for (unsigned int j = 0; i<t1[0].size(); ++j)
         t2[i][j] = t1[i][j];
   return t2;
}


template<typename T2>
T2 AddZerothElementToVector(const T2& vin) {
   T2 v(1, 0.0);
   v.insert(v.end(), vin.begin(), vin.end());
   return v;
}

template<typename T2>
T2 RemoveZerothElementFromVector(const T2& vin) {
   return(T2(++vin.begin(), vin.end()));
}

template<typename T1, typename T2>
T1 AddZerothElementToVectorOfVectors(const T1& vin) {
   const unsigned int vinSize = (unsigned int)(vin.size());
   const unsigned int NewElementSize = (unsigned int)(vin[0].size()) + 1UL;

   T1 v(1, T2(NewElementSize, 0.0));
   for (unsigned int i = 0; i<vinSize; ++i)
      v.push_back(AddZerothElementToVector(vin[i]));
   return v;
}

template<typename T1>
T1 RemoveZerothElementFromVectorOfVectors(const T1& vin) {
   const unsigned int vinSize = (unsigned int)(vin.size());

   T1 v;
   for (unsigned int i = 1; i<vinSize; ++i)
      v.push_back(RemoveZerothElementFromVector(vin[i]));

   return v;
}

template<typename T1, typename T2>
void One_Based_svdcmp(T1& a, int m, int n, T2& w, T1& v)
{
   double g, scale, anorm;
   double *rv1 = new double[n] - 1;
   g = scale = anorm = 0.0;
   int l;
   for (int i = 1; i <= n; ++i) {
      l = i + 1;
      rv1[i] = scale * g;
      g = scale = 0.0;
      if (i <= m) {
         for (int k = i; k <= m; ++k) scale += fabs(a[k][i]);
         if (scale) {
            double s = 0.0;
            for (int k = i; k <= m; ++k) {
               a[k][i] /= scale;
               s += a[k][i] * a[k][i];
            }
            double f = a[i][i];
            g = -sign(sqrt(s), f);
            double h = f * g - s;
            a[i][i] = f - g;
            for (int j = l; j <= n; ++j) {
               double sum = 0.0;
               for (int k = i; k <= m; ++k) sum += a[k][i] * a[k][j];
               double fct = sum / h;
               for (int k = i; k <= m; ++k) a[k][j] += fct * a[k][i];
            }
            for (int k = i; k <= m; ++k) a[k][i] *= scale;
         }
      }
      w[i] = scale * g;
      g = scale = 0.0;
      if ((i <= m) && (i != n)) {
         for (int k = l; k <= n; ++k) scale += fabs(a[i][k]);
         if (scale) {
            double s = 0.0;
            for (int k = l; k <= n; ++k) {
               a[i][k] /= scale;
               s += a[i][k] * a[i][k];
            }
            double f = a[i][l];
            g = -sign(sqrt(s), f);
            double h = f * g - s;
            a[i][l] = f - g;
            for (int k = l; k <= n; ++k) rv1[k] = a[i][k] / h;
            for (int j = l; j <= m; ++j) {
               double sum = 0.0;
               for (int k = l; k <= n; ++k) sum += a[j][k] * a[i][k];
               for (int k = l; k <= n; ++k) a[j][k] += sum * rv1[k];
            }
            for (int k = l; k <= n; ++k) a[i][k] *= scale;
         }
      }
      anorm = std::max(anorm, (fabs(w[i]) + fabs(rv1[i])));
   }
   {
      double f;
      for (int i = n, l; i >= 1; --i) {
         if (i<n) {       // this makes f and l not dependent
            if (f) {
               for (int j = l; j <= n; ++j) v[j][i] = (a[i][j] / a[i][l]) / f;
               for (int j = l; j <= n; ++j) {
                  double sum = 0.0;
                  for (int k = l; k <= n; ++k) sum += a[i][k] * v[k][j];
                  for (int k = l; k <= n; ++k) v[k][j] += sum * v[k][i];
               }
            }
            for (int j = l; j <= n; ++j) v[i][j] = v[j][i] = 0.0;
         }
         v[i][i] = 1.0;
         f = rv1[i];
         l = i;
      }
   }
   for (int i = std::min(m, n); i >= 1; --i) {
      int l = i + 1;
      g = w[i];
      for (int j = l; j <= n; ++j) a[i][j] = 0.0;
      if (g) {
         g = 1.0 / g;
         for (int j = l; j <= n; ++j) {
            double sum = 0.0;
            for (int k = l; k <= m; ++k) sum += a[k][i] * a[k][j];
            double f = (sum / a[i][i])*g;
            for (int k = i; k <= m; ++k) a[k][j] += f * a[k][i];
         }
         for (int j = i; j <= m; ++j) a[j][i] *= g;
      }
      else for (int j = i; j <= m; ++j) a[j][i] = 0.0;
      ++a[i][i];
   }
   for (int k = n; k >= 1; --k) {
      for (int its = 1; its <= 30; ++its) {
         int flag = 1, nm, l;
         for (l = k; l >= 1; --l) {
            nm = l - 1;
            if ((double)(fabs(rv1[l]) + anorm) == anorm) {
               flag = 0;
               break;
            }
            if ((double)(fabs(w[nm]) + anorm) == anorm) break;
         }
         if (flag) {
            double c = 0.0, s = 1.0;
            for (int i = l; i <= k; ++i) {
               double f = s * rv1[i];
               rv1[i] = c * rv1[i];
               if ((double)(fabs(f) + anorm) == anorm) break;
               g = w[i];
               double h = pythag(f, g);
               w[i] = h;
               h = 1.0 / h;
               c = g * h;
               s = -f * h;
               for (int j = 1; j <= m; ++j) {
                  double y = a[j][nm];
                  double z = a[j][i];
                  a[j][nm] = y * c + z * s;
                  a[j][i] = z * c - y * s;
               }
            }
         }
         double z = w[k];
         if (l == k) {
            if (z<0.0) {
               w[k] = -z;
               for (int j = 1; j <= n; ++j) v[j][k] = -v[j][k];
            }
            break;
         }
         if (its == 60) exit(fprintf(stderr, "no convergence in 30 svdcmp iterations"));
         double x = w[l];
         double y = w[nm = k - 1];
         g = rv1[nm];
         double h = rv1[k];
         double f = ((y - z)*(y + z) + (g - h)*(g + h)) / (2.0*h*y);
         g = pythag(f, 1.0);
         f = ((x - z)*(x + z) + h * ((y / (f + sign(g, f))) - h)) / x;
         double c = 1.0, s = 1.0;
         for (int j = l; j <= nm; ++j) {
            int i = j + 1;
            g = rv1[i];
            y = w[i];
            h = s * g;
            g *= c;
            z = pythag(f, h);
            rv1[j] = z;
            c = f / z;
            s = h / z;
            f = x * c + g * s;
            g = g * c - x * s;
            h = y * s;
            y *= c;
            for (int jj = 1; jj <= n; ++jj) {
               x = v[jj][j];
               z = v[jj][i];
               v[jj][j] = x * c + z * s;
               v[jj][i] = z * c - x * s;
            }
            z = pythag(f, h);
            w[j] = z;
            if (z) {
               z = 1.0 / z;
               c = f * z;
               s = h * z;
            }
            f = c * g + s * y;
            x = c * y - s * g;
            for (int jj = 1; jj <= m; ++jj) {
               y = a[jj][j];
               z = a[jj][i];
               a[jj][j] = y * c + z * s;
               a[jj][i] = z * c - y * s;
            }
         }
         rv1[l] = 0.0;
         rv1[k] = f;
         w[k] = x;
      }
   }
   delete (rv1 + 1);
}




template<typename T1, typename T2>
void svdcmp( T1& a, T2& w, T1& v) {
   int m = (int)(a.size());
   int n = (int)(a[0].size());
   T1 aO(AddZerothElementToVectorOfVectors<T1, T2>(a));
   T1 vO(n + 1, T2(n + 1));
   T2 wO(AddZerothElementToVector(w));
   One_Based_svdcmp(aO, m, n, wO, vO);
   a = RemoveZerothElementFromVectorOfVectors(aO);
   v = RemoveZerothElementFromVectorOfVectors(vO);
   w = RemoveZerothElementFromVector(wO);
}

#endif  //  SVD_H