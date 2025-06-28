#ifndef QUALITY_ASSESSMENT_H
#define QUALITY_ASSESSMENT_H

#include "LatticeCell.h"
#include "MobileComparisonResult.h"
#include "MultiTransformFinderControls.h"
#include "P3.h"

#include <iomanip>

// Unified Quality Assessment System
// Single source of truth for all quality-related decisions

#include <vector>
#include <string>
#include <algorithm>

enum class QualityLevel {
   EXCELLENT = 0,
   GOOD = 1,
   POOR = 2,
   TERRIBLE = 3
};

class QualityAssessment {
public:
   struct QualityThresholds {
      double excellent;
      double good;
      double poor;
      // terrible is anything > poor
      std::string method;
      double referenceScale; // For context/debugging
   };

   // FIXED: operator<< now only shows a summary, not full details
   friend std::ostream& operator<<(std::ostream& os, const QualityAssessment& qa) {
      os << "=== QUALITY ASSESSMENT ===" << std::endl;
      os << "Method: " << qa.m_thresholds.method << std::endl;
      os << "Reference P3 norm: " << std::fixed << std::setprecision(3)
         << qa.m_thresholds.referenceScale << " Å" << std::endl;
      return os;
   }

   struct QualityStats {
      int excellentCount;
      int goodCount;
      int poorCount;
      int terribleCount;
      int totalCount;
      double bestDistance;
      double worstDistance;
      QualityThresholds thresholds;
   };

private:
   QualityThresholds m_thresholds;
   std::vector<double> m_distances;
   std::vector<QualityLevel> m_qualities;
   QualityStats m_stats;

public:
   // Constructor: Establish thresholds once, use everywhere
   QualityAssessment(const LatticeCell& reference,
      const std::vector<double>& distances)
      : m_distances(distances) {

      // Calculate P3-relative thresholds (single source of truth)
      double p3Norm = P3(reference.getCell()).norm();

      // Determine adaptive thresholds based on data characteristics
      double minDistance = *std::min_element(distances.begin(), distances.end());
      double relativeMin = minDistance / p3Norm;

      if (relativeMin < 0.001) {
         // Precision regime
         m_thresholds = { p3Norm * 0.002, p3Norm * 0.01, p3Norm * 0.05,
                        "Precision P3", p3Norm };
      }
      else if (relativeMin < 0.01) {
         // Standard regime  
         m_thresholds = { p3Norm * 0.01, p3Norm * 0.05, p3Norm * 0.20,
                        "Standard P3", p3Norm };
      }
      else {
         // Relaxed regime
         m_thresholds = { p3Norm * 0.05, p3Norm * 0.20, p3Norm * 0.50,
                        "Relaxed P3", p3Norm };
      }

      // Classify all distances once
      classifyDistances();

      // Calculate statistics once
      calculateStats();
   }

   // Core quality assessment - used everywhere
   QualityLevel assessQuality(double distance) const {
      if (distance <= m_thresholds.excellent) return QualityLevel::EXCELLENT;
      if (distance <= m_thresholds.good) return QualityLevel::GOOD;
      if (distance <= m_thresholds.poor) return QualityLevel::POOR;
      return QualityLevel::TERRIBLE;
   }

   // String representation - FIXED: This should ONLY return the quality string
   std::string qualityToString(QualityLevel quality) const {
      switch (quality) {
      case QualityLevel::EXCELLENT: return "EXCELLENT";
      case QualityLevel::GOOD: return "GOOD";
      case QualityLevel::POOR: return "POOR";
      case QualityLevel::TERRIBLE: return "TERRIBLE";
      default: return "UNKNOWN";
      }
   }

   // Convenience method - FIXED: This should ONLY return the quality string
   std::string assessQualityString(double distance) const {
      return qualityToString(assessQuality(distance));
   }

   // Accessors for display logic
   const QualityThresholds& getThresholds() const { return m_thresholds; }
   const QualityStats& getStats() const { return m_stats; }

   // Get quality for specific mobile (by index)
   QualityLevel getQuality(size_t index) const {
      if (index < m_qualities.size()) return m_qualities[index];
      return QualityLevel::TERRIBLE;
   }

   // Display methods (centralized formatting)
   void displayThresholdInfo() const {
      std::cout << "=== QUALITY ASSESSMENT ===" << std::endl;
      std::cout << "Method: " << m_thresholds.method << std::endl;
      std::cout << "Reference P3 norm: " << std::fixed << std::setprecision(3)
         << m_thresholds.referenceScale << " Å" << std::endl;

      std::cout << "Thresholds:" << std::endl;
      std::cout << "  EXCELLENT <= " << std::scientific << std::setprecision(2)
         << m_thresholds.excellent << " ("
         << std::fixed << std::setprecision(1)
         << (m_thresholds.excellent / m_thresholds.referenceScale * 100) << "% of P3)" << std::endl;
      std::cout << "  GOOD      <= " << std::scientific << std::setprecision(2)
         << m_thresholds.good << " ("
         << std::fixed << std::setprecision(1)
         << (m_thresholds.good / m_thresholds.referenceScale * 100) << "% of P3)" << std::endl;
      std::cout << "  POOR      <= " << std::scientific << std::setprecision(2)
         << m_thresholds.poor << " ("
         << std::fixed << std::setprecision(1)
         << (m_thresholds.poor / m_thresholds.referenceScale * 100) << "% of P3)" << std::endl;
      std::cout << std::endl;
   }

   void displayQualitySummary() const {
      std::cout << "Quality Summary:" << std::endl;
      if (m_stats.excellentCount > 0) {
         std::cout << "  Excellent (<= " << std::scientific << std::setprecision(1)
            << m_thresholds.excellent << "): " << m_stats.excellentCount << " mobiles" << std::endl;
      }
      if (m_stats.goodCount > 0) {
         std::cout << "  Good     (<= " << std::scientific << std::setprecision(1)
            << m_thresholds.good << "): " << m_stats.goodCount << " mobiles" << std::endl;
      }
      if (m_stats.poorCount > 0) {
         std::cout << "  Poor     (<= " << std::scientific << std::setprecision(1)
            << m_thresholds.poor << "): " << m_stats.poorCount << " mobiles" << std::endl;
      }
      if (m_stats.terribleCount > 0) {
         std::cout << "  Terrible (>  " << std::scientific << std::setprecision(1)
            << m_thresholds.poor << "): " << m_stats.terribleCount << " mobiles" << std::endl;
      }
   }

private:
   void classifyDistances() {
      m_qualities.reserve(m_distances.size());
      for (double distance : m_distances) {
         m_qualities.push_back(assessQuality(distance));
      }
   }

   void calculateStats() {
      m_stats.excellentCount = 0;
      m_stats.goodCount = 0;
      m_stats.poorCount = 0;
      m_stats.terribleCount = 0;

      for (QualityLevel quality : m_qualities) {
         switch (quality) {
         case QualityLevel::EXCELLENT: m_stats.excellentCount++; break;
         case QualityLevel::GOOD: m_stats.goodCount++; break;
         case QualityLevel::POOR: m_stats.poorCount++; break;
         case QualityLevel::TERRIBLE: m_stats.terribleCount++; break;
         }
      }

      m_stats.totalCount = int(m_distances.size());
      m_stats.bestDistance = m_distances.empty() ? 0.0 :
         *std::min_element(m_distances.begin(), m_distances.end());
      m_stats.worstDistance = m_distances.empty() ? 0.0 :
         *std::max_element(m_distances.begin(), m_distances.end());
      m_stats.thresholds = m_thresholds;
   }
};

#endif // !QUALITY_ASSESSMENT_H

