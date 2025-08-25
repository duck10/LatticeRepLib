// Fixed Enhanced Boundary Detector
// Drop-in replacement for your classifyBoundaryType function

#ifndef ENHANCED_BOUNDARY_DETECTOR_H
#define ENHANCED_BOUNDARY_DETECTOR_H

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

class EnhancedBoundaryDetector {
private:
   double small_thresh = 0.5;
   double orth_thresh = 1.0;
   double angle_thresh = 2.0;
   double coord_thresh = 0.5;
   double mag_thresh_percent = 0.03;

   double magnitude(const std::pair<double, double>& v) const {
      return std::sqrt(v.first * v.first + v.second * v.second);
   }

   double dotProduct(const std::pair<double, double>& v1, const std::pair<double, double>& v2) const {
      return v1.first * v2.first + v1.second * v2.second;
   }

   double angle(const std::pair<double, double>& v1, const std::pair<double, double>& v2) const {
      double dot = dotProduct(v1, v2);
      double mag1 = magnitude(v1);
      double mag2 = magnitude(v2);
      if (mag1 < 1e-10 || mag2 < 1e-10) return 0.0;
      double cosTheta = std::max(-1.0, std::min(1.0, dot / (mag1 * mag2)));
      return std::acos(cosTheta) * 180.0 / 3.14159265359;  // Using explicit pi value
   }

public:
   std::string classifyBoundaryType(const double percentDiff, const P3& p3Before, const P3& p3After) {
      if (percentDiff < 0.1) return "too_small_difference";

      std::vector<std::string> conditions;

      const auto& p1b = p3Before[0], p2b = p3Before[1], p3b = p3Before[2];
      const auto& p1a = p3After[0], p2a = p3After[1], p3a = p3After[2];

      double mag1b = magnitude(p1b), mag2b = magnitude(p2b), mag3b = magnitude(p3b);
      double mag1a = magnitude(p1a), mag2a = magnitude(p2a), mag3a = magnitude(p3a);

      // Component boundaries
      if (std::min(std::abs(p1b.first), std::abs(p1a.first)) < small_thresh) conditions.push_back("p1_x_small");
      if (std::min(std::abs(p2b.first), std::abs(p2a.first)) < small_thresh) conditions.push_back("p2_x_small");
      if (std::min(std::abs(p3b.first), std::abs(p3a.first)) < small_thresh) conditions.push_back("p3_x_small");

      // Sign crossings
      if (p1b.first * p1a.first <= 0) conditions.push_back("p1_x_sign_flip");
      if (p2b.first * p2a.first <= 0) conditions.push_back("p2_x_sign_flip");
      if (p3b.first * p3a.first <= 0) conditions.push_back("p3_x_sign_flip");

      // Magnitude equalities (percentage-based)
      if (mag1b > 1e-10 && mag2b > 1e-10 && mag1a > 1e-10 && mag2a > 1e-10) {
         double diffB = std::abs(mag1b - mag2b) / std::max(mag1b, mag2b);
         double diffA = std::abs(mag1a - mag2a) / std::max(mag1a, mag2a);
         if (std::min(diffB, diffA) < mag_thresh_percent) conditions.push_back("mag_p1_eq_p2");
      }

      if (mag1b > 1e-10 && mag3b > 1e-10 && mag1a > 1e-10 && mag3a > 1e-10) {
         double diffB = std::abs(mag1b - mag3b) / std::max(mag1b, mag3b);
         double diffA = std::abs(mag1a - mag3a) / std::max(mag1a, mag3a);
         if (std::min(diffB, diffA) < mag_thresh_percent) conditions.push_back("mag_p1_eq_p3");
      }

      if (mag2b > 1e-10 && mag3b > 1e-10 && mag2a > 1e-10 && mag3a > 1e-10) {
         double diffB = std::abs(mag2b - mag3b) / std::max(mag2b, mag3b);
         double diffA = std::abs(mag2a - mag3a) / std::max(mag2a, mag3a);
         if (std::min(diffB, diffA) < mag_thresh_percent) conditions.push_back("mag_p2_eq_p3");
      }

      // Orthogonality
      if (std::min(std::abs(dotProduct(p1b, p2b)), std::abs(dotProduct(p1a, p2a))) < orth_thresh) conditions.push_back("p1_perp_p2");
      if (std::min(std::abs(dotProduct(p1b, p3b)), std::abs(dotProduct(p1a, p3a))) < orth_thresh) conditions.push_back("p1_perp_p3");
      if (std::min(std::abs(dotProduct(p2b, p3b)), std::abs(dotProduct(p2a, p3a))) < orth_thresh) conditions.push_back("p2_perp_p3");

      // 90-degree angles only (most important)
      if (std::min(std::abs(angle(p1b, p2b) - 90.0), std::abs(angle(p1a, p2a) - 90.0)) < angle_thresh) conditions.push_back("angle_p1_p2_90deg");
      if (std::min(std::abs(angle(p1b, p3b) - 90.0), std::abs(angle(p1a, p3a) - 90.0)) < angle_thresh) conditions.push_back("angle_p1_p3_90deg");
      if (std::min(std::abs(angle(p2b, p3b) - 90.0), std::abs(angle(p2a, p3a) - 90.0)) < angle_thresh) conditions.push_back("angle_p2_p3_90deg");

      // Coordinate relationships
      if (std::min(std::abs(p1b.first + p1b.second), std::abs(p1a.first + p1a.second)) < coord_thresh) conditions.push_back("p1_x_eq_neg_y");
      if (std::min(std::abs(p2b.first + p2b.second), std::abs(p2a.first + p2a.second)) < coord_thresh) conditions.push_back("p2_x_eq_neg_y");
      if (std::min(std::abs(p3b.first + p3b.second), std::abs(p3a.first + p3a.second)) < coord_thresh) conditions.push_back("p3_x_eq_neg_y");

      // Classification
      if (conditions.empty()) {
         return "truly_unclassified";
      } else if (conditions.size() == 1) {
         return "single_" + conditions[0];
      } else if (conditions.size() == 2) {
         return "multi_2_conditions";
      } else if (conditions.size() == 3) {
         return "multi_3_conditions";
      } else {
         return "complex_" + std::to_string(conditions.size()) + "_conditions";
      }
   }
};

#endif // ENHANCED_BOUNDARY_DETECTOR_H
