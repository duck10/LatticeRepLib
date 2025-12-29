// CycleDetection.h - Structures and functions for analyzing Niggli reduction cycles
// Add this to your KrivyGruber_LCA header or as a separate include

#ifndef CYCLE_DETECTION_H
#define CYCLE_DETECTION_H

#include "G6.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>

namespace CycleDetection {

   // Structure to capture state at each iteration
   struct IterationState {
      int iteration;
      G6 g6;                    // G6 values
      double trace;             // g1 + g2 + g3
      int stepFired;            // Which step fired (0=none, 1-8)
      bool signNormFired;       // Did sign normalization fire?
      std::string signPattern;  // e.g., "(-,+,-)" or "(+,+,+)"

      IterationState()
         : iteration(0), trace(0.0), stepFired(0), signNormFired(false) {
      }

      IterationState(int iter, const G6& g, int step, bool signNorm)
         : iteration(iter), g6(g), stepFired(step), signNormFired(signNorm) {
         trace = g[0] + g[1] + g[2];
         signPattern = computeSignPattern(g);
      }

      // Compute sign pattern string
      static std::string computeSignPattern(const G6& g) {
         std::string s = "(";
         s += (g[3] >= 0.0) ? "+" : "-";
         s += ",";
         s += (g[4] >= 0.0) ? "+" : "-";
         s += ",";
         s += (g[5] >= 0.0) ? "+" : "-";
         s += ")";
         return s;
      }

      // Output operator
      friend std::ostream& operator<<(std::ostream& os, const IterationState& state) {
         os << "Iteration " << state.iteration << ":\n"
            << "  G6: " << state.g6 << "\n"
            << "  trace: " << std::fixed << std::setprecision(15) << state.trace << "\n"
            << "  signs: " << state.signPattern << "\n"
            << "  step: " << state.stepFired
            << " (sign norm: " << (state.signNormFired ? "yes" : "no") << ")";
         return os;
      }
   };

   // Analysis results structure
   struct CycleAnalysis {
      bool isCycling;
      int cycleLength;
      std::vector<int> cyclePattern;  // Sequence of steps that cycle
      double traceVariation;          // Max variation in trace
      std::string diagnosis;          // Human-readable explanation

      CycleAnalysis() : isCycling(false), cycleLength(0), traceVariation(0.0) {}

      friend std::ostream& operator<<(std::ostream& os, const CycleAnalysis& analysis) {
         os << "\n=== CYCLE ANALYSIS ===\n";
         if (analysis.isCycling) {
            os << "CYCLING DETECTED: Yes\n";
            os << "Cycle length: " << analysis.cycleLength << " iterations\n";
            os << "Step pattern: ";
            for (size_t i = 0; i < analysis.cyclePattern.size(); ++i) {
               if (i > 0) os << " -> ";
               os << analysis.cyclePattern[i];
            }
            os << " (repeats)\n";
         } else {
            os << "CYCLING DETECTED: No\n";
         }
         os << std::fixed << std::setprecision(17);
         os << "Trace variation: " << analysis.traceVariation;
         if (analysis.traceVariation == 0.0) {
            os << " (exactly zero - all traces identical)";
         } else if (analysis.traceVariation < 1e-13) {
            os << " (~1 ULP - one unit in last place)";
         } else if (analysis.traceVariation < 1e-10) {
            os << " (< 1e-10 - essentially constant)";
         }
         os << "\n";
         os << "\nDiagnosis:\n" << analysis.diagnosis << "\n";
         return os;
      }
   };

   // Analyze iteration history to detect cycles
   inline CycleAnalysis analyzeCycle(const std::vector<IterationState>& history) {
      CycleAnalysis result;

      if (history.size() < 10) {
         result.diagnosis = "Insufficient data for cycle analysis (need at least 10 iterations)";
         return result;
      }

      // Calculate trace variation
      double minTrace = history[0].trace;
      double maxTrace = history[0].trace;
      for (const auto& state : history) {
         minTrace = std::min(minTrace, state.trace);
         maxTrace = std::max(maxTrace, state.trace);
      }
      result.traceVariation = maxTrace - minTrace;

      // Check if trace is essentially constant
      bool traceConstant = (result.traceVariation < 1e-12);

      // Extract step sequence (last 21 iterations)
      std::vector<int> stepSequence;
      size_t startIdx = (history.size() > 21) ? history.size() - 21 : 0;
      for (size_t i = startIdx; i < history.size(); ++i) {
         stepSequence.push_back(history[i].stepFired);
      }

      // Look for repeating pattern
      int detectedCycleLength = 0;
      for (int len = 1; len <= 10; ++len) {
         bool isPattern = true;
         for (size_t i = 0; i + len < stepSequence.size(); ++i) {
            if (stepSequence[i] != stepSequence[i + len]) {
               isPattern = false;
               break;
            }
         }
         if (isPattern && stepSequence.size() >= 2 * len) {
            detectedCycleLength = len;
            break;
         }
      }

      if (detectedCycleLength > 0) {
         result.isCycling = true;
         result.cycleLength = detectedCycleLength;
         for (int i = 0; i < detectedCycleLength; ++i) {
            result.cyclePattern.push_back(stepSequence[i]);
         }
      }

      // Build diagnosis
      std::ostringstream diag;

      if (traceConstant && result.isCycling) {
         diag << "TRUE CYCLING: Steps actively firing in repeating pattern while trace remains constant.\n\n";

         // Analyze the pattern
         diag << "Step sequence for last " << stepSequence.size() << " iterations:\n";
         diag << "Steps: ";
         for (size_t i = 0; i < stepSequence.size(); ++i) {
            if (i > 0) diag << " -> ";
            diag << stepSequence[i];
         }
         diag << "\n\n";

         // Analyze sign patterns
         std::vector<std::string> signPatterns;
         for (size_t i = startIdx; i < history.size(); ++i) {
            signPatterns.push_back(history[i].signPattern);
         }

         diag << "Sign patterns cycling: ";
         bool allSame = true;
         for (size_t i = 1; i < signPatterns.size(); ++i) {
            if (signPatterns[i] != signPatterns[0]) {
               allSame = false;
               break;
            }
         }

         if (allSame) {
            diag << signPatterns[0] << " (constant)\n";
         } else {
            diag << "Multiple patterns detected\n";
            // Show unique patterns
            std::vector<std::string> uniquePatterns;
            for (const auto& pat : signPatterns) {
               if (std::find(uniquePatterns.begin(), uniquePatterns.end(), pat) == uniquePatterns.end()) {
                  uniquePatterns.push_back(pat);
               }
            }
            for (const auto& pat : uniquePatterns) {
               diag << "  " << pat << "\n";
            }
         }

         diag << "\nRoot cause: ";

         // Check for near-cubic symmetry
         const G6& lastG6 = history.back().g6;
         double g1 = lastG6[0];
         double g2 = lastG6[1];
         double g4 = lastG6[3];
         double g5 = lastG6[4];

         bool nearCubic = (std::abs(std::abs(g4) - g2) < 1e-10) ||
            (std::abs(std::abs(g5) - g1) < 1e-10);

         if (nearCubic) {
            diag << "Near-cubic symmetry detected.\n";
            diag << "  |g4| ~ g2: " << std::abs(std::abs(g4) - g2) << "\n";
            diag << "  |g5| ~ g1: " << std::abs(std::abs(g5) - g1) << "\n";
            diag << "\nWhen |g4| ~ g2 and |g5| ~ g1:\n";
            diag << "- Both (+,+,+) and (-,-,-) normalized states trigger reduction steps\n";
            diag << "- Reduction step formulas (e.g., g4 = -2*g2 + g4) create sign changes\n";
            diag << "- Algorithm alternates between +++ and --- normalizations\n";
            diag << "- Each normalization triggers different reduction step\n";
            diag << "- Endless cycle with no convergence\n";
         } else {
            diag << "Step conditions repeatedly satisfied despite stable trace.\n";
            diag << "Parameters create conditions where reduction steps fire but make no net progress.\n";
         }

      } else if (traceConstant && !result.isCycling) {
         diag << "STUCK WITHOUT EXIT: Algorithm reached reduced form but cannot detect completion.\n\n";
         diag << "All steps show: 0 (no steps firing)\n";
         diag << "Trace is constant: " << std::fixed << std::setprecision(15) << history.back().trace << "\n";
         diag << "\nThe cell is already in Niggli reduced form, but without convergence detection,\n";
         diag << "the algorithm cannot recognize this and continues running indefinitely.\n";

      } else if (!traceConstant) {
         diag << "ACTIVE REDUCTION: Trace is still changing, algorithm is making progress.\n";
         diag << "Trace variation: " << std::scientific << result.traceVariation << "\n";
         diag << "This is normal behavior, not a convergence problem.\n";

      } else {
         diag << "UNKNOWN PATTERN: Neither clear cycling nor clear stuck state detected.\n";
         diag << "More analysis needed.\n";
      }

      result.diagnosis = diag.str();
      return result;
   }

   // Function to print detailed comparison table
   inline void printDetailedCycleTable(const std::vector<IterationState>& history, std::ostream& os) {
      if (history.empty()) {
         os << "No iteration history available.\n";
         return;
      }

      os << "\n=== DETAILED ITERATION HISTORY ===\n\n";

      // Table header
      os << std::setw(6) << "Iter"
         << std::setw(22) << "g1"
         << std::setw(22) << "g2"
         << std::setw(22) << "g3"
         << std::setw(22) << "g4"
         << std::setw(22) << "g5"
         << std::setw(22) << "g6"
         << std::setw(22) << "trace"
         << std::setw(10) << "Signs"
         << std::setw(6) << "Step"
         << "\n";

      os << std::string(170, '-') << "\n";

      // Table rows
      for (const auto& state : history) {
         os << std::setw(6) << state.iteration;
         for (int i = 0; i < 6; ++i) {
            os << std::setw(22) << std::fixed << std::setprecision(15) << state.g6[i];
         }
         os << std::setw(22) << std::fixed << std::setprecision(15) << state.trace;
         os << std::setw(10) << state.signPattern;
         os << std::setw(6) << state.stepFired;
         os << "\n";
      }

      os << "\n";
   }

   // Function to print parameter changes between iterations
   inline void printParameterChanges(const std::vector<IterationState>& history, std::ostream& os) {
      if (history.size() < 2) {
         os << "Need at least 2 iterations to show changes.\n";
         return;
      }

      os << "\n=== PARAMETER CHANGES BETWEEN ITERATIONS ===\n\n";

      // Header
      os << std::setw(12) << "From->To"
         << std::setw(15) << "Delta_g1"
         << std::setw(15) << "Delta_g2"
         << std::setw(15) << "Delta_g3"
         << std::setw(15) << "Delta_g4"
         << std::setw(15) << "Delta_g5"
         << std::setw(15) << "Delta_g6"
         << std::setw(15) << "Delta_trace"
         << std::setw(10) << "Step"
         << "\n";

      os << std::string(125, '-') << "\n";

      // Show changes
      for (size_t i = 1; i < history.size(); ++i) {
         const auto& prev = history[i - 1];
         const auto& curr = history[i];

         os << std::setw(5) << prev.iteration << "->" << std::setw(5) << curr.iteration;

         for (int j = 0; j < 6; ++j) {
            double delta = curr.g6[j] - prev.g6[j];
            if (std::abs(delta) < 1e-20) {
               os << std::setw(15) << "0";
            } else {
               os << std::setw(15) << std::scientific << std::setprecision(3) << delta;
            }
         }

         double deltaTrace = curr.trace - prev.trace;
         if (std::abs(deltaTrace) < 1e-20) {
            os << std::setw(15) << "0";
         } else {
            os << std::setw(15) << std::scientific << std::setprecision(3) << deltaTrace;
         }

         os << std::setw(10) << curr.stepFired;
         os << "\n";
      }

      os << "\n";
   }

} // namespace CycleDetection

#endif // CYCLE_DETECTION_H
