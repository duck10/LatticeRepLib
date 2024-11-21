//#include "GlitchDetector.h"
//#include <cmath>
//#include <iostream>
//
//std::vector<GlitchDetector::Glitch> GlitchDetector::detect(
//   const std::vector<double>& values,
//   double valueThreshold,
//   double derivativeThreshold,
//   double tension) {
//
//   std::vector<Glitch> glitches;
//   if (values.size() < 3) return glitches;
//
//   // Calculate the average and standard deviation of the values
//   double sum = std::accumulate(values.begin(), values.end(), 0.0);
//   double mean = sum / values.size();
//   double sq_sum = std::inner_product(values.begin(), values.end(), values.begin(), 0.0,
//      std::plus<>(), [mean](double x, double y) { return (x - mean) * (y - mean); });
//   double stddev = std::sqrt(sq_sum / (values.size() - 1));
//
//   // Use the standard deviation to set a dynamic threshold if not provided
//   double dynamicValueThreshold = (valueThreshold <= 0.0) ? 2.0 * stddev : valueThreshold;
//
//   std::vector<double> firstDerivatives(values.size() - 1);
//   std::vector<double> secondDerivatives(values.size() - 2);
//
//   // Calculate first derivatives
//   for (size_t i = 1; i < values.size(); ++i) {
//      firstDerivatives[i - 1] = values[i] - values[i - 1];
//   }
//
//   // Calculate second derivatives
//   for (size_t i = 1; i < values.size() - 1; ++i) {
//      secondDerivatives[i - 1] = firstDerivatives[i] - firstDerivatives[i - 1];
//   }
//
//   // Calculate the standard deviation of second derivatives
//   double deriv2_sum = std::accumulate(secondDerivatives.begin(), secondDerivatives.end(), 0.0);
//   double deriv2_mean = deriv2_sum / secondDerivatives.size();
//   double deriv2_sq_sum = std::inner_product(secondDerivatives.begin(), secondDerivatives.end(), secondDerivatives.begin(), 0.0,
//      std::plus<>(), [deriv2_mean](double x, double y) { return (x - deriv2_mean) * (y - deriv2_mean); });
//   double deriv2_stddev = std::sqrt(deriv2_sq_sum / (secondDerivatives.size() - 1));
//
//   // Use the second derivative standard deviation to set a dynamic threshold if not provided
//   double dynamicDerivativeThreshold = (derivativeThreshold <= 0.0) ? 2.5 * deriv2_stddev : derivativeThreshold;
//
//   for (size_t i = 1; i < values.size() - 1; ++i) {
//      double currentValue = values[i];
//      double previousValue = values[i - 1];
//      double nextValue = values[i + 1];
//      double changePercent = std::abs((currentValue - previousValue) / previousValue) * 100.0;
//      double secondDerivative = secondDerivatives[i - 1];
//
//      // Check for sudden changes in the second derivative (change in slope)
//      bool isSlopeChange = std::abs(secondDerivative) > dynamicDerivativeThreshold;
//
//      // Check for significant value change
//      bool isValueChange = std::abs(currentValue - mean) > dynamicValueThreshold;
//
//      // Detect glitch if there's a significant change in slope or value
//      if (isSlopeChange || isValueChange) {
//         // Additional check to avoid detecting continuous slopes
//         bool isContinuousSlope = (currentValue > previousValue && currentValue < nextValue) ||
//            (currentValue < previousValue && currentValue > nextValue);
//
//         if (!isContinuousSlope) {
//            glitches.emplace_back(i, currentValue, changePercent, secondDerivative);
//            std::cout << "Glitch detected at " << i << " (value: " << currentValue
//               << ", change: " << changePercent << "%, second derivative: " << secondDerivative << ")" << std::endl;
//         }
//      }
//   }
//
//   return glitches;
//}
//
//std::vector<double> GlitchDetector::applySplineUnderTension(const std::vector<double>& data) {
//   spline_under_tension spline(tension, data, splineSwitch);
//   std::vector<double> out;
//   out.reserve(data.size());
//
//   const double offset = 0.5; // Evaluate at midpoints between original data points
//
//   // First point remains the same
//   out.push_back(data.front());
//
//   // Evaluate spline at midpoints for interior points
//   for (size_t n = 1; n < data.size() - 1; ++n) {
//      out.push_back(spline.getInterpValue(static_cast<float>(n) + offset));
//   }
//
//   // Last point remains the same
//   out.push_back(data.back());
//
//   // Print debug information
//   std::cout << "Spline smoothing details:" << std::endl;
//   for (size_t i = 0; i < std::min(size_t(10), data.size()); ++i) {
//      std::cout << "Index " << i << ": Original = " << data[i]
//         << ", Smoothed = " << out[i]
//         << ", Difference = " << (out[i] - data[i]) << std::endl;
//   }
//
//   return out;
//}