// WilkinsonScale.h
#ifndef WILKINSON_SCALE_H
#define WILKINSON_SCALE_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

class WilkinsonScale {
public:
   struct Scale {
      double min;
      double max;
      double step;
      int numTicks;
      double score;  // Overall score for this scale
   };

   struct ScoringWeights {
      double simplicity = 1.0;  // Preference for "nice" numbers
      double coverage = 1.0;    // How well the scale covers the data range
      double density = 1.0;     // How close to desired tick count
      double legibility = 1.0;  // Preference for certain "readable" numbers
   };

   explicit WilkinsonScale(int desiredTicks = 5, const ScoringWeights& weights = ScoringWeights())
      : m_desiredTicks(desiredTicks), m_weights(weights) {
      initializeCandidates();
   }

   Scale calculateScale(double dataMin, double dataMax);

private:
   struct ScoreComponents {
      double simplicity;
      double coverage;
      double density;
      double legibility;
   };

   void initializeCandidates() {
      // Q is a list of nice numbers
      m_Q = { 1, 5, 2, 2.5, 4, 3 };
      // m_qBias affects preference among candidate step sizes
      m_qBias = { 0.0, 0.4, 0.3, 0.5, 0.4, 0.3 };

      // Extended step sizes if needed
      m_extendedQ = { 1, 5, 2, 2.5, 4, 3, 1.5, 7, 6, 8, 9 };
      m_extendedQBias = { 0.0, 0.4, 0.3, 0.5, 0.4, 0.3, 0.5, 0.3, 0.3, 0.3, 0.3 };
   }

   ScoreComponents calculateScoreComponents(const Scale& scale,
      double dataMin, double dataMax,
      double stepSize, int qIndex);

   double calculateScore(const ScoreComponents& scores);

   int m_desiredTicks;
   ScoringWeights m_weights;
   std::vector<double> m_Q;        // Nice numbers
   std::vector<double> m_qBias;    // Bias for each nice number
   std::vector<double> m_extendedQ;
   std::vector<double> m_extendedQBias;
};

// WilkinsonScale.cpp
WilkinsonScale::Scale WilkinsonScale::calculateScale(double dataMin, double dataMax) {
   if (dataMin == dataMax) {
      dataMin -= 0.5;
      dataMax += 0.5;
   }

   // Ensure min < max
   if (dataMin > dataMax) {
      std::swap(dataMin, dataMax);
   }

   const double range = dataMax - dataMin;
   const double magnitude = pow(10, floor(log10(range)));

   Scale bestScale;
   bestScale.score = -std::numeric_limits<double>::infinity();

   // Try different step sizes and magnitudes
   for (int magScale = -2; magScale <= 2; ++magScale) {
      const double mag = magnitude * pow(10, magScale);

      for (size_t i = 0; i < m_Q.size(); ++i) {
         const double stepSize = m_Q[i] * mag;

         // Try different numbers of ticks around desired amount
         for (int j = -1; j <= 1; ++j) {
            const int numTicks = m_desiredTicks + j;
            if (numTicks < 2) continue;

            // Calculate potential min/max values
            const double span = stepSize * (numTicks - 1);
            const double min = floor(dataMin / stepSize) * stepSize;
            const double max = min + span;

            if (min <= dataMin && max >= dataMax) {
               Scale scale{ min, max, stepSize, numTicks };
               auto scores = calculateScoreComponents(scale, dataMin, dataMax,
                  stepSize, i);
               scale.score = calculateScore(scores);

               if (scale.score > bestScale.score) {
                  bestScale = scale;
               }
            }
         }
      }
   }

   return bestScale;
}

WilkinsonScale::ScoreComponents WilkinsonScale::calculateScoreComponents(
   const Scale& scale, double dataMin, double dataMax,
   double stepSize, int qIndex) {

   ScoreComponents scores;

   // Simplicity score
   scores.simplicity = 1 - (qIndex / double(m_Q.size())) - m_qBias[qIndex];

   // Coverage score
   const double range = dataMax - dataMin;
   const double coverageMin = std::min(dataMin, scale.min);
   const double coverageMax = std::max(dataMax, scale.max);
   scores.coverage = 1 - ((coverageMax - coverageMin) - range) / range;

   // Density score
   const double densityTarget = m_desiredTicks;
   scores.density = 1 - std::abs(scale.numTicks - densityTarget) / densityTarget;

   // Legibility score
   // Prefer numbers that end in 0, 2, 5
   const bool hasNiceEnd = (int(scale.min / stepSize) % 10) == 0 ||
      (int(scale.min / stepSize) % 10) == 2 ||
      (int(scale.min / stepSize) % 10) == 5;
   scores.legibility = hasNiceEnd ? 1.0 : 0.8;

   return scores;
}

double WilkinsonScale::calculateScore(const ScoreComponents& scores) {
   return (scores.simplicity * m_weights.simplicity +
      scores.coverage * m_weights.coverage +
      scores.density * m_weights.density +
      scores.legibility * m_weights.legibility) /
      (m_weights.simplicity + m_weights.coverage +
         m_weights.density + m_weights.legibility);
}

#endif // WILKINSON_SCALE_H

