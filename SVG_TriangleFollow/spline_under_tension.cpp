/**
* @file spline_under_tension.hpp
* @author  Bill Brouwer <whiskeyjulietb@gmail.com>
* @version 1.0
*
* @section LICENSE
* Copyright 2013 William J. Brouwer
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* @section DESCRIPTION
*
* A port of the spline under tension package of A. K. CLINE AND R. J. RENKA
*
*/

/*
changes by larry andrews Oct 2017
Even the test program did not run correctly. The included binary search function 
did not alway work at the end (ran of the end of a vector). I removed
it and getInterval was changed to use the standard library function
lower_bound. Probably should also check at the bottom using
upper_bound. 

tridiagonalTerms originally returned a float*, so that it could return two
values. Those were allocated in tridiagonalTerms, but never deleted. I 
replaced the two variables with pair<float,float> and changed 
tridiagonalTerms so that it returns a pair.

Some casts between float and double and between int to size_t  and double to float were
added to satisfy some warnings.

Member variable "size" removed because it is not needed and because it
hides library member.

Variables were changed from arrays to vectors so that the size of them
is known.

Fixed the interpolation so that (like at the bottom end) above the top
the return is done by extrapolation.

*/

//#include "stdafx.h"

#include "spline_under_tension.hpp"

#include "OutlierFinder.h"

#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>


class spline_under_tension::coefficient {

private:
   float c1;
   float c2;
   float c3;

public:
   coefficient() {}

   coefficient(const float del1,
      const float del2,
      const float sigma,
      const bool calculateAll) {

      if (!calculateAll) {

         c1 = -1.0f / del1;
         c2 = -c1;
      }
      else {
         if (sigma == 0) {
            //no tension
            const float del = del2 - del1;
            c1 = -(del1 + del2) / (del1*del2);
            c2 = del2 / (del1*del);
            c3 = -del1 / (del2*del);
         }
         else {
            //tension
            const float coshm1 = coshf(sigma*del1) - 1.0f;
            const float coshm2 = coshf(sigma*del2) - 1.0f;

            const float delp = (sigma * (del2 + del1) / 2.0f);
            const float delm = (sigma * (del2 - del1) / 2.0f);

            const float sinhmp = sinhf(delp);
            const float sinhmm = sinhf(delm);

            const float denom = coshm1 * (del2 - del1) - 2.0f * del1 * sinhmp * sinhmm;

            c1 = 2.0f * sinhmp * sinhmm / denom;
            c2 = -coshm2 / denom;
            c3 = coshm1 / denom;
         }
      }
   }

   ~coefficient() {
   }

   float getCoeffOne() const { return c1; }
   float getCoeffTwo() const { return c2; }
   float getCoeffThree() const { return c3; }
}; // end of class spline_under_tension::coefficient


class spline_under_tension::acceleration {

private:
   std::vector<float> yOutput;
   std::pair<float,float> diagTermsPrior;
   float slpp1, slppn;

public:
   acceleration() {}

   acceleration(const float sigmap, // tension
      const std::vector<float>& xValues,
      const std::vector<float>& yValues,
      const float slp1,  // initial slope (if used)
      const float slpn,  // end slope (if used)
      const int islpsw) { // 0,1,2,3 -- 3 is calculate both end slopes from data

      const int n = (int)(xValues.size());

      if (n <= 1)
         sparseDataError();
      if (xValues[n - 1] <= xValues[0])
         monotonicError();

      float delx1, delx2, delxn, delxnm;

      coefficient firstConstants, lastConstants;

      switch (islpsw) {
         //both derivatives given
      case 0:
         slpp1 = slp1;
         slppn = slpn;
         break;

         //first derivative given
      case 1:
         slpp1 = slp1;
         delxn = xValues[n - 1] - xValues[n - 2];
         delxnm = delxn + delxn;

         if (delxn <= 0 || delxnm <= delxn)
            monotonicError();

         lastConstants = spline_under_tension::coefficient(-delxn, -delxnm, sigmap, false);
         slppn = lastConstants.getCoeffOne() * yValues[n - 1] + lastConstants.getCoeffTwo() * yValues[n - 2];
         break;

         //last derivative given
      case 2:
         slppn = slpn;
         delx1 = xValues[1] - xValues[0];
         delx2 = delx1 + delx1;

         if (delx1 <= 0 || delx2 <= delx1)
            monotonicError();

         firstConstants = spline_under_tension::coefficient(delx1, delx2, sigmap, false);
         slpp1 = firstConstants.getCoeffOne() * yValues[0] + firstConstants.getCoeffTwo() * yValues[1];
         break;

         //both derivatives calculated
      case 3:
         delx1 = xValues[1] - xValues[0];
         delx2 = xValues[2] - xValues[0];

         delxn = xValues[n - 1] - xValues[n - 2];
         delxnm = xValues[n - 1] - xValues[n - 3];

         firstConstants = spline_under_tension::coefficient(delx1, delx2, sigmap, true);
         lastConstants = spline_under_tension::coefficient(-delxn, -delxnm, sigmap, true);

         slpp1 = firstConstants.getCoeffOne() * yValues[0] + firstConstants.getCoeffTwo() * yValues[1] +
            firstConstants.getCoeffThree() * yValues[2];

         slppn = lastConstants.getCoeffOne() * yValues[n - 1] + lastConstants.getCoeffTwo() * yValues[n - 2] +
            lastConstants.getCoeffThree() * yValues[n - 3];
         break;
      }
      secondDerivative(xValues, yValues, sigmap);
   }

   void secondDerivative(const std::vector<float>& xValues, const std::vector<float>& yValues, const float sigmap) {

      const int n = (int)(xValues.size());

      for (int i = 0; i < n; i++) yOutput.push_back(-1);

      const float delx1 = xValues[1] - xValues[0];
      std::vector<float> temp(n);

      if (delx1 == 0)
         monotonicError();

      float dx1 = (yValues[1] - yValues[0]) / delx1;

      diagTermsPrior = tridiagonalTerms(sigmap, delx1);
      yOutput[0] = (dx1 - slpp1) / diagTermsPrior.first;
      temp[0] = diagTermsPrior.second / diagTermsPrior.first;

      if (n > 2) {
         for (int i = 1; i < n - 1; i++) {
            const float delx2 = xValues[i + 1] - xValues[i];

            if (delx2 == 0)
               monotonicError();

            const float dx2 = (yValues[i + 1] - yValues[i]) / delx2;
            const std::pair<float, float> diagTermsNext = tridiagonalTerms(sigmap, delx2);
            const float diag = diagTermsPrior.first + diagTermsNext.first - diagTermsPrior.second * temp[i - 1];
            yOutput[i] = (dx2 - dx1 - diagTermsPrior.second * yOutput[i - 1]) / diag;

            temp[i] = diagTermsNext.second / diag;
            dx1 = dx2;

            diagTermsPrior.first = diagTermsNext.first;
            diagTermsPrior.second = diagTermsNext.second;
         }
      }

      const float diag = diagTermsPrior.first - diagTermsPrior.second * temp[n - 2];
      yOutput[n - 1] = (slppn - dx1 - diagTermsPrior.second * yOutput[n - 2]) / diag;

      for (int i = n - 2; i > 0; i--) {
         yOutput[i] = yOutput[i] - temp[i] * yOutput[i + 1];
      }
   }

   std::pair<float,float> tridiagonalTerms(const float sigma, const float del) {
      std::pair<float, float> ret;

      if (sigma == 0) {
         ret.first = del / 3.0f;
         ret.second = del / 6.0f;
      }
      else {
         const double sigdel = double(sigma * del);
         const double denom = (double)(del) / (sinh(sigdel) * sigdel * sigdel);

         ret.first = (float)(denom * (sigdel * (cosh(sigdel) - 1.0F) - sinh(sigdel) + sigdel));
         ret.second = (float)(denom * (sinh(sigdel) - sigdel));
      }
      return ret;
   }

   std::vector<float> getOutput() const {
      return yOutput;
   }

   static void sparseDataError() {
      std::cerr << "Insufficient samples in data \n";
      exit(1);

   }

   static void monotonicError() {
      std::cerr << "Data not strictly increasing \n";
      exit(1);
   }

   ~acceleration() {
   }

};  // end of class spline_under_tension::acceleration

float spline_under_tension::getInterpValue(const float value) {

   int im1 = getInterval(value, xValues);
   if (im1 >= (int)(xValues.size()) - 2) {
      im1 = int(xValues.size()) - 2;
   }

   const float del1 = value - xValues[im1];
   const float del2 = xValues[im1 + 1] - value;
   const float dels = xValues[im1 + 1] - xValues[im1];
   const float sum = (yValues[im1 + 1] * del1 + yValues[im1] * del2) / dels;

   if ((int)sigmap == 0) {
      return sum - del1*del2*(secondDerivative[im1 + 1] * (del1 + dels)
         + secondDerivative[im1] * (del2 + dels)) / (6.0f * dels);
   }
   else {
      const float delp1 = sigmap * (del1 + dels) / 2.0f;
      const float delp2 = sigmap * (del2 + dels) / 2.0f;

      const float sinhm1 = sinhf(sigmap*del1) - sigmap*del1;
      const float sinhm2 = sinhf(sigmap*del2) - sigmap*del2;
      const float sinhms = sinhf(sigmap*dels) - sigmap*dels;

      const float sinhp1 = sinhf(sigmap*del1 / 2.0f) - sigmap*del1 / 2.0f;
      const float sinhp2 = sinhf(sigmap*del2 / 2.0f) - sigmap*del2 / 2.0f;

      const float coshp1 = coshf(delp1) - 1;
      const float coshp2 = coshf(delp2) - 1;

      return sum + (secondDerivative[im1 + 1] * (sinhm1*del2 - del1*(2.0f * (coshp1 + 1.0f) * sinhp2 + sigmap*coshp1*del2))
         + secondDerivative[im1] * (sinhm2*del1 - del2*(2.0f * (coshp2 + 1.0f) * sinhp1 + sigmap*coshp2*del1)))
         / (sigmap * sigmap *dels * (sinhms + sigmap*dels));
   }
}

int spline_under_tension::getInterval(const float value, const std::vector<float>& array) const {
   const std::vector<float>::const_iterator it = std::lower_bound(array.begin(), array.end(), value);
   const int index = (int)(it - array.begin());
   return index;
}

spline_under_tension::spline_under_tension() {};

spline_under_tension::spline_under_tension(const double sigma,
   const std::vector<double>& yVal,
   const int islpsw)
   : sigmap()
   , xValues()
   , yValues()
{
   const int n = (int)(yVal.size());

   for (unsigned int i = 0; i < yVal.size(); ++i) {
      xValues.push_back(float(i));
      yValues.push_back((float(yVal[i])));
   }
   sigmap = (float)(fabs(sigma) * double(n - 1) / (xValues[n - 1] - xValues[0]));

   secondDerivative = acceleration(sigmap, xValues, yValues, -1, -1, islpsw).getOutput();
}

spline_under_tension::spline_under_tension(const float sigma,
   const std::vector<float>& yVal,
   const int islpsw)
   : sigmap()
   , xValues()
   , yValues(yVal)
{
   const int n = (int)(yVal.size());

   for (unsigned int i = 0; i < yVal.size(); ++i)
      xValues.push_back(float(i));
   sigmap = fabs(sigma) * (float)(n - 1) / (xValues[n - 1] - xValues[0]);

   secondDerivative = acceleration(sigmap, xValues, yValues, -1, -1, islpsw).getOutput();
}

spline_under_tension::spline_under_tension(const float sigma,
   const std::vector<float>& xVal,
   const std::vector<float>& yVal,
   const float slp1,
   const float slp2,
   const int islpsw)
   : sigmap()
   , xValues(xVal)
   , yValues(yVal)
{
   const int n = (int)(xVal.size());
   sigmap = fabs(sigma) * (float)(n - 1) / (xVal[n - 1] - xVal[0]);

   secondDerivative = acceleration(sigmap, xValues, yValues, slp1, slp2, islpsw).getOutput();
}


spline_under_tension::~spline_under_tension() {};

