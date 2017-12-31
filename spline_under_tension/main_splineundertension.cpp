#include "OutlierFinder.h"
#include "spline_under_tension.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>

#include <vector>
using namespace std;


int main() {
   {

      vector<double> xvals, yvals;
      yvals.push_back(0.0);
      yvals.push_back(1.11494F);
      yvals.push_back(2.22988F);
      yvals.push_back(3.34482F);
      yvals.push_back(4.45976F);
      yvals.push_back(5.5747F);
      yvals.push_back(6.68964F);
      yvals.push_back(7.80458F);
      yvals.push_back(8.91952F);
      yvals.push_back(10.0345F);
      yvals.push_back(11.1494F);
      yvals.push_back(12.2643F);
      yvals.push_back(13.3793F);
      yvals.push_back(14.4942F);
      yvals.push_back(15.6092F);
      yvals.push_back(16.7241F);
      yvals.push_back(17.839F);
      yvals.push_back(18.954F);
      yvals.push_back(20.0689F);
      yvals.push_back(21.1839F);
      yvals.push_back(22.2988F);
      yvals.push_back(23.4137F);
      yvals.push_back(24.5287F);
      yvals.push_back(25.6436F);
      yvals.push_back(26.7586F);
      yvals.push_back(27.8735F);
      yvals.push_back(28.9884F);
      yvals.push_back(30.1034F);
      yvals.push_back(31.2183F);
      yvals.push_back(32.3333F);
      yvals.push_back(33.4482F);
      yvals.push_back(34.5631F);
      yvals.push_back(35.6781F);
      yvals.push_back(36.1702F);
      yvals.push_back(35.4794F);
      yvals.push_back(34.8106F);
      yvals.push_back(37.165F);
      yvals.push_back(33.5442F);
      yvals.push_back(31.4762F);
      yvals.push_back(29.5466F);
      yvals.push_back(27.8458F);
      yvals.push_back(26.3113F);
      yvals.push_back(25.095F);
      yvals.push_back(24.2447F);
      yvals.push_back(23.7998F);
      yvals.push_back(23.7829F);
      yvals.push_back(24.195F);
      yvals.push_back(25.0148F);
      yvals.push_back(26.2043F);

      const int n = (int)(yvals.size());

      for (int i = 0; i<(int)(yvals.size()); i++) {
         xvals.push_back(float(i));
      }

      for (int i = 0; i<n; i++) {
         if (i > 19) yvals[i] += 4.0F*std::max(0.0, float(24 - i) / 3.0);
      }

      OutlierFinder of(yvals);
      const double d = of.GetValue(5.0);
      const std::vector<std::pair<double, double> > glitches = of.FindDiscontinuities(2.0);
      const std::vector<std::pair<double, double> > glitches2 = OutlierFinder(yvals).FindDiscontinuities(2.0);

      spline_under_tension foo(0.1, yvals, 3);

      //for (int i = 0; i < n+6; ++i) {
      //   std::cout << i << "  " << foo.getInterpValue(float(i-3)) << std::endl;
      //}

      const int shift = 3;
      for (int i = 0; i < n + 2*shift; ++i) {
         int k = i - shift;
         std::cout << k+0.5 << "  " << foo.getInterpValue(float(k)+0.5F);
         if (k >= 0 && k < n-1) std::cout << "  " << k << "  " << xvals[k] << "   " << yvals[k];
         std::cout << std::endl;
      }
      exit(0);
   }
      
      vector<float> xvals, yvals;
   
      for (int i = 0; i<2048; i++) {
      xvals.push_back(i*0.05f);
      yvals.push_back(sin(xvals[i]));
      
   }
   
      
      //spline_under_tension foo(0.1F, xvals, yvals, -1, -1, 3);
      spline_under_tension foo(0.1F, yvals, 3);

      	//vector<float> out=foo.getSecondDer();
      
      	//for (vector<float>::iterator it = out.begin(); it != out.end(); it++)
      	//	cout << *it << endl;
      
      	//exit(0);
      
      cout << "#yvals; interp" << endl;
   cout << foo.getInterpValue(xvals[0] + 0.03333F);
   for (int i = 1; i<1024; i++) {
      cout << "," << foo.getInterpValue(xvals[i] + 0.3333F);
      
         
   }
   
      cout << endl;
   cout << "#actual yvals" << endl;
   cout << sin(xvals[0] + 0.03333);
   for (int i = 1; i<1024; i++) {
      cout << "," << sin(xvals[i] + 0.3333);
      
   }
   cout << endl;
   cout << "#xvals" << endl;
   cout << xvals[0] + 0.03333;
   for (int i = 1; i<1024; i++) {
      cout << "," << xvals[i] + 0.3333;
      
         
   }
   cout << endl;
   
      return 0;
   
};