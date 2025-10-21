#include "NiggliProjectors.h"
#include <cmath>
#include <iostream>
#include <iomanip>

#include "LRL_Cell.h"
#include "Niggli.h"

NiggliProjectors::NiggliProjectors() {
   initializeAcuteBoundaries();
   initializeObtuseBoundaries();
}

bool NiggliProjectors::isAcuteBranch(const G6& g) {
   // Acute branch: g4, g5, g6 all >= 0
   // Obtuse branch: g4, g5, g6 all <= 0
   int positiveCount = 0;
   int negativeCount = 0;

   if (g[3] > 0) positiveCount++; else if (g[3] < 0) negativeCount++;
   if (g[4] > 0) positiveCount++; else if (g[4] < 0) negativeCount++;
   if (g[5] > 0) positiveCount++; else if (g[5] < 0) negativeCount++;

   // If all positive or all zero, it's acute
   return (negativeCount == 0);
}

void NiggliProjectors::initializeAcuteBoundaries() {
   // Boundaries that appear in BOTH branches

   // Boundary 1: g1 = g2
   {
      std::vector<double> trans = { 0,1,0,0,0,0, 1,0,0,0,0,0, 0,0,1,0,0,0, 0,0,0,0,1,0, 0,0,0,1,0,0, 0,0,0,0,0,1 };
      std::vector<double> proj = { 0.5,0.5,0,0,0,0, 0.5,0.5,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,1,0, 0,0,0,0,0,1 };
      acuteBoundaries.push_back(BoundaryPolytope("1", "g1 = g2", "Equal cell edges", MatG6(trans), MatG6(proj)));
   }

   // Boundary 1S: Special condition when g1 = g2: |g4| = |g5|
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,0,1,0, 0,0,0,1,0,0, 0,0,0,0,0,1 };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,0.5,0.5,0, 0,0,0,0.5,0.5,0, 0,0,0,0,0,1 };
      acuteBoundaries.push_back(BoundaryPolytope("1S", "|g4| = |g5| (when g1=g2)", "Special condition", MatG6(trans), MatG6(proj)));
   }

   // Boundary 2: g2 = g3
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,0,1,0,0,0, 0,1,0,0,0,0, 0,0,0,1,0,0, 0,0,0,0,0,1, 0,0,0,0,1,0 };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,0.5,0.5,0,0,0, 0,0.5,0.5,0,0,0, 0,0,0,1,0,0, 0,0,0,0,1,0, 0,0,0,0,0,1 };
      acuteBoundaries.push_back(BoundaryPolytope("2", "g2 = g3", "Equal cell edges", MatG6(trans), MatG6(proj)));
   }

   // Boundary 2S: Special condition when g2 = g3: |g5| = |g6|
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,0,1, 0,0,0,0,1,0 };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,0.5,0.5, 0,0,0,0,0.5,0.5 };
      acuteBoundaries.push_back(BoundaryPolytope("2S", "|g5| = |g6| (when g2=g3)", "Special condition", MatG6(trans), MatG6(proj)));
   }

   // Boundary 3: g4 = 0 (ACUTE: uses R5_Plus equivalent)
   {
      std::vector<double> trans = {
         1,0,0,0,0,0,  // g1' = g1
         0,1,0,0,0,0,  // g2' = g2  
         0,1,1,-1,0,0, // g3' = g2 + g3 - g4
         0,-2,0,1,0,0, // g4' = -2g2 + g4
         0,0,0,0,1,-1, // g5' = g5 - g6
         0,0,0,0,0,1   // g6' = g6
      };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,0,0,0, 0,0,0,0,1,0, 0,0,0,0,0,1 };
      acuteBoundaries.push_back(BoundaryPolytope("3", "g4 = 0", "90°", MatG6(trans), MatG6(proj)));
   }

   // Boundary 4: g5 = 0 (ACUTE: uses R6_Plus equivalent)
   {
      std::vector<double> trans = {
         1,0,0,0,0,0,  // g1' = g1
         0,1,0,0,0,0,  // g2' = g2
         1,0,1,0,-1,0, // g3' = g1 + g3 - g5
         0,0,0,1,0,-1, // g4' = g4 - g6
         -2,0,0,0,1,0, // g5' = -2g1 + g5
         0,0,0,0,0,1   // g6' = g6
      };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,0,0, 0,0,0,0,0,1 };
      acuteBoundaries.push_back(BoundaryPolytope("4", "g5 = 0", "90°", MatG6(trans), MatG6(proj)));
   }

   // Boundary 5: g6 = 0 (ACUTE: uses R7_Plus equivalent)
   {
      std::vector<double> trans = {
         1,0,0,0,0,0,  // g1' = g1
         1,1,0,0,0,-1, // g2' = g1 + g2 - g6
         0,0,1,0,0,0,  // g3' = g3
         0,0,0,1,-1,0, // g4' = g4 - g5
         0,0,0,0,1,0,  // g5' = g5
         -2,0,0,0,0,1  // g6' = -2g1 + g6
      };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,1,0, 0,0,0,0,0,0 };
      acuteBoundaries.push_back(BoundaryPolytope("5", "g6 = 0", "90°", MatG6(trans), MatG6(proj)));
   }

   // Boundary 6/7: g2 = g4 (acute: g4 > 0, use R5_Plus)
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,1,1,-1,0,0, 0,-2,0,1,0,0, 0,0,0,0,1,-1, 0,0,0,0,0,1 };
      std::vector<double> proj = {
         1,   0,   0,   0,   0,   0,
         0,   0.5, 0,   0.5, 0,   0,
         0,   0,   1,   0,   0,   0,
         0,   0.5, 0,   0.5, 0,   0,
         0,   0,   0,   0,   1,   0,
         0,   0,   0,   0,   0,   1
      };
      acuteBoundaries.push_back(BoundaryPolytope("6/7", "g2 = g4", "Face diagonal", MatG6(trans), MatG6(proj)));
   }

   // Boundary 9/A: g1 = g5 (acute: g5 > 0, use R6_Plus)
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 1,0,1,0,-1,0, 0,0,0,1,0,-1, -2,0,0,0,1,0, 0,0,0,0,0,1 };
      std::vector<double> proj = {
         0.5, 0,   0,   0,   0.5, 0,
         0,   1,   0,   0,   0,   0,
         0,   0,   1,   0,   0,   0,
         0,   0,   0,   1,   0,   0,
         0.5, 0,   0,   0,   0.5, 0,
         0,   0,   0,   0,   0,   1
      };
      acuteBoundaries.push_back(BoundaryPolytope("9/A", "g1 = g5", "Face diagonal", MatG6(trans), MatG6(proj)));
   }

   // Boundary C/D: g1 = g6 (acute: g6 > 0, use R7_Plus)
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 1,1,0,0,0,-1, 0,0,1,0,0,0, 0,0,0,1,-1,0, 0,0,0,0,1,0, -2,0,0,0,0,1 };
      std::vector<double> proj = {
         0.5, 0,   0,   0,   0,   0.5,
         0,   1,   0,   0,   0,   0,
         0,   0,   1,   0,   0,   0,
         0,   0,   0,   1,   0,   0,
         0,   0,   0,   0,   1,   0,
         0.5, 0,   0,   0,   0,   0.5
      };
      acuteBoundaries.push_back(BoundaryPolytope("C/D", "g1 = g6", "Face diagonal", MatG6(trans), MatG6(proj)));
   }

   // Sign-only transformations (simple sign flips for Phase 1)
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,-1,0, 0,0,0,0,0,-1 };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,1,0, 0,0,0,0,0,1 };
      acuteBoundaries.push_back(BoundaryPolytope("3sign", "sign correction", "sp34c", MatG6(trans), MatG6(proj)));
   }

   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,-1,0,0, 0,0,0,0,1,0, 0,0,0,0,0,-1 };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,1,0, 0,0,0,0,0,1 };
      acuteBoundaries.push_back(BoundaryPolytope("4sign", "sign correction", "sp34b", MatG6(trans), MatG6(proj)));
   }

   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,-1,0,0, 0,0,0,0,-1,0, 0,0,0,0,0,1 };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,1,0, 0,0,0,0,0,1 };
      acuteBoundaries.push_back(BoundaryPolytope("5sign", "sign correction", "sp34a", MatG6(trans), MatG6(proj)));
   }

   // Special 4D boundaries
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,0,0, 0,0,0,0,0,1 };
      std::vector<double> proj = { 0.5,0,0,0,0,0.5, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,0,0, 0.5,0,0,0,0,0.5 };
      acuteBoundaries.push_back(BoundaryPolytope("D'", "g1=g6, g5=0", "Special 4D", MatG6(trans), MatG6(proj)));
   }

   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,1,0,0,0,0, 0,0,0,0,1,0, 0,0,0,0,0,0 };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,0.5,0,0.5,0,0, 0,0,1,0,0,0, 0,0.5,0,0.5,0,0, 0,0,0,0,1,0, 0,0,0,0,0,0 };
      acuteBoundaries.push_back(BoundaryPolytope("7'", "g2=g4, g6=0", "Special 4D", MatG6(trans), MatG6(proj)));
   }

   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 1,0,0,0,0,0, 0,0,0,0,0,0 };
      std::vector<double> proj = { 0.5,0,0,0,0.5,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0.5,0,0,0,0.5,0, 0,0,0,0,0,0 };
      acuteBoundaries.push_back(BoundaryPolytope("A'", "g1=g5, g6=0", "Special 4D", MatG6(trans), MatG6(proj)));
   }
}




void NiggliProjectors::initializeObtuseBoundaries() {
   // Boundaries that appear in BOTH branches (same as acute)

   // Boundary 1: g1 = g2
   {
      std::vector<double> trans = { 0,1,0,0,0,0, 1,0,0,0,0,0, 0,0,1,0,0,0, 0,0,0,0,1,0, 0,0,0,1,0,0, 0,0,0,0,0,1 };
      std::vector<double> proj = { 0.5,0.5,0,0,0,0, 0.5,0.5,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,1,0, 0,0,0,0,0,1 };
      obtuseBoundaries.push_back(BoundaryPolytope("1", "g1 = g2", "Equal cell edges", MatG6(trans), MatG6(proj)));
   }

   // Boundary 1S: Special condition when g1 = g2: |g4| = |g5|
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,0,1,0, 0,0,0,1,0,0, 0,0,0,0,0,1 };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,0.5,0.5,0, 0,0,0,0.5,0.5,0, 0,0,0,0,0,1 };
      obtuseBoundaries.push_back(BoundaryPolytope("1S", "|g4| = |g5| (when g1=g2)", "Special condition", MatG6(trans), MatG6(proj)));
   }

   // Boundary 2: g2 = g3
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,0,1,0,0,0, 0,1,0,0,0,0, 0,0,0,1,0,0, 0,0,0,0,0,1, 0,0,0,0,1,0 };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,0.5,0.5,0,0,0, 0,0.5,0.5,0,0,0, 0,0,0,1,0,0, 0,0,0,0,1,0, 0,0,0,0,0,1 };
      obtuseBoundaries.push_back(BoundaryPolytope("2", "g2 = g3", "Equal cell edges", MatG6(trans), MatG6(proj)));
   }

   // Boundary 2S: Special condition when g2 = g3: |g5| = |g6|
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,0,1, 0,0,0,0,1,0 };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,0.5,0.5, 0,0,0,0,0.5,0.5 };
      obtuseBoundaries.push_back(BoundaryPolytope("2S", "|g5| = |g6| (when g2=g3)", "Special condition", MatG6(trans), MatG6(proj)));
   }

   // Boundary 3: g4 = 0 (OBTUSE: uses R5_Minus equivalent)
   {
      std::vector<double> trans = {
         1,0,0,0,0,0,  // g1' = g1
         0,1,0,0,0,0,  // g2' = g2
         0,1,1,1,0,0,  // g3' = g2 + g3 + g4
         0,2,0,1,0,0,  // g4' = 2g2 + g4
         0,0,0,0,1,1,  // g5' = g5 + g6
         0,0,0,0,0,1   // g6' = g6
      };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,0,0,0, 0,0,0,0,1,0, 0,0,0,0,0,1 };
      obtuseBoundaries.push_back(BoundaryPolytope("3", "g4 = 0", "90°", MatG6(trans), MatG6(proj)));
   }

   // Boundary 4: g5 = 0 (OBTUSE: uses R6_Minus equivalent)
   {
      std::vector<double> trans = {
         1,0,0,0,0,0,  // g1' = g1
         0,1,0,0,0,0,  // g2' = g2
         1,0,1,0,1,0,  // g3' = g1 + g3 + g5
         0,0,0,1,0,1,  // g4' = g4 + g6
         2,0,0,0,1,0,  // g5' = 2g1 + g5
         0,0,0,0,0,1   // g6' = g6
      };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,0,0, 0,0,0,0,0,1 };
      obtuseBoundaries.push_back(BoundaryPolytope("4", "g5 = 0", "90°", MatG6(trans), MatG6(proj)));
   }

   // Boundary 5: g6 = 0 (OBTUSE: uses R7_Minus equivalent)
   {
      std::vector<double> trans = {
         1,0,0,0,0,0,  // g1' = g1
         1,1,0,0,0,1,  // g2' = g1 + g2 + g6
         0,0,1,0,0,0,  // g3' = g3
         0,0,0,1,1,0,  // g4' = g4 + g5
         0,0,0,0,1,0,  // g5' = g5
         2,0,0,0,0,1   // g6' = 2g1 + g6
      };

      // Boundary R8: Body diagonal (obtuse, different from R12)
      {
         std::vector<double> trans = {
            1, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1,  // Different signs from R12
            0, 2, 0, 1, 0, 1,  // Different signs from R12
            2, 0, 0, 0, 1, 1,  // Different signs from R12
            0, 0, 0, 0, 0, 1
         };
         std::vector<double> proj = {
            0.8,-0.2,0,-0.2,-0.2,-0.2,
            -0.2,0.8,0,-0.2,-0.2,-0.2,
            0,0,1,0,0,0,
            -0.2,-0.2,0,0.8,-0.2,-0.2,
            -0.2,-0.2,0,-0.2,0.8,-0.2,
            -0.2,-0.2,0,-0.2,-0.2,0.8
         };
         obtuseBoundaries.push_back(BoundaryPolytope("R8", "Body diagonal (obtuse)", "R8", MatG6(trans), MatG6(proj)));
      }


      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,1,0, 0,0,0,0,0,0 };
      obtuseBoundaries.push_back(BoundaryPolytope("5", "g6 = 0", "90°", MatG6(trans), MatG6(proj)));
   }

   // Sign-only transformations (simple sign flips for Phase 1)
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,-1,0, 0,0,0,0,0,-1 };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,1,0, 0,0,0,0,0,1 };
      obtuseBoundaries.push_back(BoundaryPolytope("3sign", "sign correction", "sp34c", MatG6(trans), MatG6(proj)));
   }

   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,-1,0,0, 0,0,0,0,1,0, 0,0,0,0,0,-1 };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,1,0, 0,0,0,0,0,1 };
      obtuseBoundaries.push_back(BoundaryPolytope("4sign", "sign correction", "sp34b", MatG6(trans), MatG6(proj)));
   }

   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,-1,0,0, 0,0,0,0,-1,0, 0,0,0,0,0,1 };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,1,0, 0,0,0,0,0,1 };
      obtuseBoundaries.push_back(BoundaryPolytope("5sign", "sign correction", "sp34a", MatG6(trans), MatG6(proj)));
   }

   // OBTUSE-ONLY boundaries (use R_Minus variants)

   // Boundary 8: g2 + g4 = 0 (obtuse: g4 < 0, use R5_Minus)
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,1,1,1,0,0, 0,2,0,1,0,0, 0,0,0,0,1,1, 0,0,0,0,0,1 };
      std::vector<double> proj = {
         1,    0,    0,    0,    0,   0,    // Row 0: unchanged
         0,    0.5,  0,   -0.5,  0,   0,    // Row 1: g2' = 0.5*g2 - 0.5*g4
         0,    0,    1,    0,    0,   0,    // Row 2: unchanged
         0,   -0.5,  0,    0.5,  0,   0,    // Row 3: g4' = -0.5*g2 + 0.5*g4
         0,    0,    0,    0,    1,   0,    // Row 4: unchanged
         0,    0,    0,    0,    0,   1     // Row 5: unchanged
      };
      obtuseBoundaries.push_back(BoundaryPolytope("8", "g2 + g4 = 0", "Face diagonal", MatG6(trans), MatG6(proj)));
   }

   // Boundary B: g1 + g5 = 0 (obtuse: g5 < 0, use R6_Minus)
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 1,0,1,0,1,0, 0,0,0,1,0,1, 2,0,0,0,1,0, 0,0,0,0,0,1 };
      std::vector<double> proj = {
         0.5,  0,   0,   0,   -0.5, 0,    // Row 0: g1' = 0.5*g1 - 0.5*g5
         0,    1,   0,   0,    0,   0,    // Row 1: unchanged
         0,    0,   1,   0,    0,   0,    // Row 2: unchanged
         0,    0,   0,   1,    0,   0,    // Row 3: unchanged
         -0.5, 0,   0,   0,    0.5, 0,    // Row 4: g5' = -0.5*g1 + 0.5*g5
         0,    0,   0,   0,    0,   1     // Row 5: unchanged
      };
      obtuseBoundaries.push_back(BoundaryPolytope("B", "g1 + g5 = 0", "Face diagonal", MatG6(trans), MatG6(proj)));
   }

   // Boundary E: g1 + g6 = 0 (obtuse: g6 < 0, use R7_Minus)
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 1,1,0,0,0,1, 0,0,1,0,0,0, 0,0,0,1,1,0, 0,0,0,0,1,0, 2,0,0,0,0,1 };
      std::vector<double> proj = {
         0.5,  0,   0,   0,   0,   -0.5,  // Row 0: g1' = 0.5*g1 - 0.5*g6
         0,    1,   0,   0,   0,    0,    // Row 1: unchanged
         0,    0,   1,   0,   0,    0,    // Row 2: unchanged
         0,    0,   0,   1,   0,    0,    // Row 3: unchanged
         0,    0,   0,   0,   1,    0,    // Row 4: unchanged
         -0.5, 0,   0,   0,   0,    0.5   // Row 5: g6' = -0.5*g1 + 0.5*g6
      };
      obtuseBoundaries.push_back(BoundaryPolytope("E", "g1 + g6 = 0", "Face diagonal", MatG6(trans), MatG6(proj)));
   }

   // Boundary F: Use the CORRECTED R12 matrix
   {
      std::vector<double> trans = {
         1,0,0,0,0,0,
         0,1,0,0,0,0,
         1,1,1,-1,-1,1,
         0,-2,0,1,0,-1,
         -2,0,0,0,1,-1,
         0,0,0,0,0,1
      };
      std::vector<double> proj = {
         0.8,-0.2,0,-0.2,-0.2,-0.2,
         -0.2,0.8,0,-0.2,-0.2,-0.2,
         0,0,1,0,0,0,
         -0.2,-0.2,0,0.8,-0.2,-0.2,
         -0.2,-0.2,0,-0.2,0.8,-0.2,
         -0.2,-0.2,0,-0.2,-0.2,0.8
      };
      obtuseBoundaries.push_back(BoundaryPolytope("F", "g1 + g2 + g4 + g5 + g6 = 0", "Body diagonal", MatG6(trans), MatG6(proj)));
   }

   // Special 4D boundaries
   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,1,0,0,0,0, 0,0,0,0,1,0, 0,0,0,0,0,0 };
      std::vector<double> proj = { 1,0,0,0,0,0, 0,0.5,0,-0.5,0,0, 0,0,1,0,0,0, 0,-0.5,0,0.5,0,0, 0,0,0,0,1,0, 0,0,0,0,0,0 };
      obtuseBoundaries.push_back(BoundaryPolytope("8'", "g2+g4=0, g6=0", "Special 4D", MatG6(trans), MatG6(proj)));
   }

   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 1,0,0,0,0,0, 0,0,0,0,0,0 };
      std::vector<double> proj = { 0.5,0,0,0,-0.5,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, -0.5,0,0,0,0.5,0, 0,0,0,0,0,0 };
      obtuseBoundaries.push_back(BoundaryPolytope("B'", "g1+g5=0, g6=0", "Special 4D", MatG6(trans), MatG6(proj)));
   }

   {
      std::vector<double> trans = { 1,0,0,0,0,0, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,0,0, 1,0,0,0,0,0 };
      std::vector<double> proj = { 0.5,0,0,0,0,-0.5, 0,1,0,0,0,0, 0,0,1,0,0,0, 0,0,0,1,0,0, 0,0,0,0,0,0, -0.5,0,0,0,0,0.5 };
      obtuseBoundaries.push_back(BoundaryPolytope("E'", "g1+g6=0, g5=0", "Special 4D", MatG6(trans), MatG6(proj)));
   }
}




const BoundaryPolytope* NiggliProjectors::getBoundary(const std::string& name, bool isAcute) const {
   const auto& boundaries = isAcute ? acuteBoundaries : obtuseBoundaries;
   for (const auto& boundary : boundaries) {
      if (boundary.name == name) {
         return &boundary;
      }
   }
   return nullptr;
}

G6 NiggliProjectors::projectToBoundary(const G6& g, const std::string& boundaryName, bool isAcute) const {
   const BoundaryPolytope* boundary = getBoundary(boundaryName, isAcute);
   if (!boundary) {
      return g;
   }
   return boundary->projector * g;
}

double NiggliProjectors::signedDistanceToBoundary(const G6& g, const std::string& boundaryName, bool isAcute) const {
   const BoundaryPolytope* boundary = getBoundary(boundaryName, isAcute);
   //if (!boundary) {
   //   if (boundaryName == "F") {
   //      std::cout << "ERROR: Boundary F not found! isAcute=" << isAcute << "\n";
   //   }
   //   return 0.0;
   //}

   if (!boundary) {
      return 0.0;
   }

   // For boundaries 1-5, use geometric projection
   if (boundaryName == "1") {
      return g[1] - g[0];  // positive when g1 <= g2 (inside)
   } else if (boundaryName == "1S") {
      return std::abs(g[4]) - std::abs(g[3]);  // positive when |g4| <= |g5|
   } else if (boundaryName == "2") {
      return g[2] - g[1];  // positive when g2 <= g3
   } else if (boundaryName == "2S") {
      return std::abs(g[5]) - std::abs(g[4]);  // positive when |g5| <= |g6|
   } else if (boundaryName == "3") {
      if (isAcute) {
         return (g[3] >= 0) ? (g[1] - g[3]) : 1.0;
      } else {
         // Obtuse: g4 < 0, so -g4 = |g4|
         // We want: g2 - |g4| = g2 + g4 (since g4 is negative)
         return g[1] + g[3];  // simpler!
      }
   } else if (boundaryName == "4") {
      return isAcute ? g[4] : (g[0] + g[4]);  // g1 + g5 (g5 is negative)
   } else if (boundaryName == "5") {
      return isAcute ? g[5] : (g[0] + g[5]);  // g1 + g6 (g6 is negative)
   }

   // For face/body diagonal boundaries, check the actual inequality
   else if (boundaryName == "6/7") {
      // Acute: g4 <= g2 (inside Niggli cone)
      // Distance is positive when inside, negative when outside
      double primaryDist = g[1] - g[3];  // g2 - g4

      // If clearly away from boundary, return primary distance
      if (std::abs(primaryDist) > 1.0e-6) {
         return primaryDist;
      }

      // On boundary, check secondary condition: 2g5 <= g6
      return g[5] - 2.0 * g[4];

   } else if (boundaryName == "8") {
      // Obtuse: valid when g2 + g4 >= 0 (or |g4| <= g2)
      const double primaryDist = g[1] + g[3];  // g2 + g4 (positive when valid)

      if (std::abs(primaryDist) > 1.0e-6) {
         return primaryDist;
      }

      // On boundary, check secondary: g6 <= 0
      return -g[5];

   } else if (boundaryName == "9/A") {
      // Acute: g5 <= g1
      double primaryDist = g[0] - g[4];  // g1 - g5

      if (std::abs(primaryDist) > 1.0e-6) {
         return primaryDist;
      }

      // On boundary, check secondary: 2g4 <= g6
      return g[5] - 2.0 * g[3];

   } else if (boundaryName == "B") {
      // Obtuse: g5 <= -g1, or g1 + g5 <= 0
      double primaryDist = -(g[0] + g[4]);

      if (std::abs(primaryDist) > 1.0e-6) {
         return primaryDist;
      }

      // On boundary, check secondary: g6 <= 0
      return -g[5];

   } else if (boundaryName == "C/D") {
      // Acute: g6 <= g1
      double primaryDist = g[0] - g[5];  // g1 - g6

      if (std::abs(primaryDist) > 1.0e-6) {
         return primaryDist;
      }

      // On boundary, check secondary: 2g4 <= g5
      return g[4] - 2.0 * g[3];

   } else if (boundaryName == "E") {
      // Obtuse: g6 <= -g1, or g1 + g6 <= 0
      double primaryDist = -(g[0] + g[5]);

      if (std::abs(primaryDist) > 1.0e-6) {
         return primaryDist;
      }

      // On boundary, check secondary: g5 <= 0
      return -g[4];

   } else if (boundaryName == "F") {
      // Body diagonal: valid when g1+g2+g3+g4+g5+g6 >= g3
      // Or equivalently: g1+g2+g4+g5+g6 >= 0
      double sum = g[0] + g[1] + g[2] + g[3] + g[4] + g[5];
      return sum - g[2];  // Positive when valid (sum >= g3), negative when violated (sum < g3)
   }

   else if (boundaryName == "R8") {
      double sum = g[0] + g[1] + g[2] + g[3] + g[4] + g[5];
      return sum - g[2];
   }

   // For special 4D boundaries (7', 8', A', B', D', E'), use geometric projection
   else {
      G6 proj = boundary->projector * g;
      G6 perp = g - proj;
      return perp.norm();  // These are less critical, simple distance is okay
   }

   return 0.0;
}


bool NiggliProjectors::isInsideRelativeToBoundary(const G6& g, const std::string& boundaryName, bool isAcute) const {
   return signedDistanceToBoundary(g, boundaryName, isAcute) >= 0.0;
}

G6 NiggliProjectors::applyTransformation(const G6& g, const std::string& boundaryName, bool isAcute) const {
   if (boundaryName.empty()) {
      return g;  // No transformation to apply
   }

   const std::string actualBoundary = selectSubBoundary(g, boundaryName, 1.0e-12);
   const BoundaryPolytope* boundary = getBoundary(actualBoundary, isAcute);

   if (!boundary) {
      std::cerr << "ERROR: Boundary '" << actualBoundary << "' not found (isAcute=" << isAcute << ")\n";
      return g;  // Return unchanged rather than crash
   }

   return boundary->transformation * g;
}
std::ostream& operator<<(std::ostream& os, const BoundaryPolytope& bp) {
   os << "Boundary " << bp.name << ": " << bp.condition << "\n";
   os << "  Classification: " << bp.classification << "\n";
   os << "  Transformation:\n";
   std::vector<double> trans = bp.transformation.GetVector();
   for (int i = 0; i < 6; ++i) {
      os << "    [";
      for (int j = 0; j < 6; ++j) {
         if (j > 0) os << ", ";
         os << std::setw(4) << trans[i * 6 + j];
      }
      os << "]\n";
   }
   os << "  Projector:\n";
   std::vector<double> proj = bp.projector.GetVector();
   for (int i = 0; i < 6; ++i) {
      os << "    [";
      for (int j = 0; j < 6; ++j) {
         if (j > 0) os << ", ";
         os << std::setw(6) << proj[i * 6 + j];
      }
      os << "]\n";
   }
   return os;
}

std::string NiggliProjectors::getSignCorrectionBoundary(const G6& g, double epsilon) {
   int s4 = (g[3] > epsilon) ? 1 : -1;
   int s5 = (g[4] > epsilon) ? 1 : -1;
   int s6 = (g[5] > epsilon) ? 1 : -1;

   // Pattern match for all 8 cases
   if (s4 == 1 && s5 == 1 && s6 == 1) {
      return ""; // (+++) already consistent
   } else if (s4 == -1 && s5 == -1 && s6 == -1) {
      return ""; // (---) already consistent
   } else if (s4 == -1 && s5 == -1 && s6 == 1) {
      return "5sign"; // (--+) flip g4,g5 ? (+++)
   } else if (s4 == -1 && s5 == 1 && s6 == -1) {
      return "4sign"; // (-+-) flip g4,g6 ? (+++)
   } else if (s4 == -1 && s5 == 1 && s6 == 1) {
      return "3sign"; // (-++) flip g5,g6 ? (---)
   } else if (s4 == 1 && s5 == -1 && s6 == -1) {
      return "3sign"; // (+--) flip g5,g6 ? (+++)
   } else if (s4 == 1 && s5 == -1 && s6 == 1) {
      return "4sign"; // (+-+) flip g4,g6 ? (---)
   } else if (s4 == 1 && s5 == 1 && s6 == -1) {
      return "5sign"; // (++-) flip g4,g5 ? (---)
   }

   return "";
}



std::ostream& operator<<(std::ostream& os, const NiggliProjectors& np) {
   os << "Niggli Reduction Boundary Polytopes\n";
   os << "====================================\n\n";

   os << "ACUTE BRANCH (+++) - " << np.acuteBoundaries.size() << " boundaries:\n";
   os << std::string(50, '-') << "\n";
   for (const auto& boundary : np.acuteBoundaries) {
      os << boundary << "\n";
   }

   os << "\nOBTUSE BRANCH (---) - " << np.obtuseBoundaries.size() << " boundaries:\n";
   os << std::string(50, '-') << "\n";
   for (const auto& boundary : np.obtuseBoundaries) {
      os << boundary << "\n";
   }

   return os;
}

double NiggliProjectors::maxViolation(const G6& g, double epsilon) const {
   // Determine branch
   bool g4nearZero = (std::abs(g[3]) < epsilon);
   bool g5nearZero = (std::abs(g[4]) < epsilon);
   bool g6nearZero = (std::abs(g[5]) < epsilon);

   bool checkAcute = g4nearZero || g5nearZero || g6nearZero ||
      (g[3] > 0 && g[4] > 0 && g[5] > 0);
   bool checkObtuse = g4nearZero || g5nearZero || g6nearZero ||
      (g[3] < 0 && g[4] < 0 && g[5] < 0);

   double worstDistance = 0.0;

   if (checkAcute) {
      for (const auto& boundary : acuteBoundaries) {
         double dist = signedDistanceToBoundary(g, boundary.name, true);
         worstDistance = std::min(worstDistance, dist);
      }
   }

   if (checkObtuse) {
      for (const auto& boundary : obtuseBoundaries) {
         double dist = signedDistanceToBoundary(g, boundary.name, false);
         worstDistance = std::min(worstDistance, dist);
      }
   }

   return -worstDistance;  // Return positive value for magnitude of worst violation
}

std::string NiggliProjectors::selectSubBoundary(const G6& g, const std::string& boundaryName, double epsilon) const {
   if (boundaryName == "6/7") {
      if (std::abs(g[1] - g[3]) <= epsilon) {
         std::string subBoundary = (2.0 * g[4] < g[5] + epsilon) ? "6" : "7";
         if (getBoundary(subBoundary, true) != nullptr) {
            return subBoundary;
         }
      }
      return "6/7";
   } else if (boundaryName == "8") {
      return "8";
   } else if (boundaryName == "9/A") {
      if (std::abs(g[0] - g[4]) <= epsilon) {
         std::string subBoundary = (2.0 * g[3] < g[5] + epsilon) ? "9" : "A";
         if (getBoundary(subBoundary, true) != nullptr) {
            return subBoundary;
         }
      }
      return "9/A";
   } else if (boundaryName == "B") {
      return "B";
   } else if (boundaryName == "C/D") {
      if (std::abs(g[0] - g[5]) <= epsilon) {
         std::string subBoundary = (2.0 * g[3] < g[4] + epsilon) ? "C" : "D";
         if (getBoundary(subBoundary, true) != nullptr) {
            return subBoundary;
         }
      }
      return "C/D";
   } else if (boundaryName == "E") {
      return "E";
   }
   return boundaryName;
}

void NiggliProjectors::ApplyPhase1(G6& current, const NiggliProjectors& np, double epsilon, bool debug) {
   bool changed = true;
   int phase1Iterations = 0;
   const int maxPhase1Iterations = 10;

   while (changed && phase1Iterations < maxPhase1Iterations) {
      changed = false;
      phase1Iterations++;

      // Step 1: Sort g1 <= g2 <= g3
      int pattern = 0;
      if (current[0] > current[1] + epsilon) pattern |= 4;
      if (current[1] > current[2] + epsilon) pattern |= 2;
      if (current[0] > current[2] + epsilon) pattern |= 1;

      switch (pattern) {
      case 0:
         break;
      case 1:
         break;
      case 2:
         if (debug) std::cout << "    Phase1: Pattern 010, applying M2\n";
         current = np.applyTransformation(current, "2", true);
         changed = true;
         continue;
      case 3:
         if (debug) std::cout << "    Phase1: Pattern 011, applying M1, M2\n";
         current = np.applyTransformation(current, "1", true);
         current = np.applyTransformation(current, "2", true);
         changed = true;
         continue;
      case 4:
         if (debug) std::cout << "    Phase1: Pattern 100, applying M1\n";
         current = np.applyTransformation(current, "1", true);
         changed = true;
         continue;
      case 5:
         if (debug) std::cout << "    Phase1: Pattern 101, applying M1, M2\n";
         current = np.applyTransformation(current, "1", true);
         current = np.applyTransformation(current, "2", true);
         changed = true;
         continue;
      case 6:
         if (debug) std::cout << "    Phase1: Pattern 110, applying M2, M1\n";
         current = np.applyTransformation(current, "2", true);
         current = np.applyTransformation(current, "1", true);
         changed = true;
         continue;
      case 7:
         if (debug) std::cout << "    Phase1: Pattern 111, applying M2, M1\n";
         current = np.applyTransformation(current, "2", true);
         current = np.applyTransformation(current, "1", true);
         changed = true;
         continue;
      }

      // Step 2: Special conditions
      bool g1nearG2 = (std::abs(current[0] - current[1]) < epsilon);
      bool g2nearG3 = (std::abs(current[1] - current[2]) < epsilon);

      //if (g1nearG2 && std::abs(current[3]) > std::abs(current[4]) + epsilon) {
      //   if (debug) std::cout << "    Phase1: Applying 1S\n";
      //   current = np.applyTransformation(current, "1S", true);
      //   changed = true;
      //   continue;
      //}

      //if (g2nearG3 && std::abs(current[4]) > std::abs(current[5]) + epsilon) {
      //   if (debug) std::cout << "    Phase1: Applying 2S\n";
      //   current = np.applyTransformation(current, "2S", true);
      //   changed = true;
      //   continue;
      //}

      // Step 3: Sign consistency
      std::string signBoundary = getSignCorrectionBoundary(current, epsilon);
      if (!signBoundary.empty()) {
         if (debug) std::cout << "    Phase1: Applying sign correction " << signBoundary << "\n";
         current = np.applyTransformation(current, signBoundary, true);
         changed = true;
         continue;
      }
   }

   if (debug && phase1Iterations > 1) {
      std::cout << "    Phase1 complete after " << phase1Iterations << " iterations\n";
   }
}


G6 NiggliProjectors::phase1(const G6& g, bool debug) {
   G6 current = g;

   if (debug) {
      std::cout << "  Initial Phase 1:\n";
   }

   int maxPhase1Iterations = 10;
   for (int i = 0; i < maxPhase1Iterations; ++i) {
      G6 beforePhase1 = current;

      // Use your existing ApplyPhase1 static method
      ApplyPhase1(current, *this, 1.0e-12, debug);

      // Check if Phase 1 made any changes
      if ((current - beforePhase1).norm() < 1e-12) {
         if (debug) {
            std::cout << "    Phase1 complete after " << (i + 1) << " iterations\n";
         }
         break;
      }
   }

   return current;
}

void NiggliProjectors::findWorstViolation(const G6& g, std::string& worstBoundary, bool& worstIsAcute,
   double& worstDistance, int& worstPriority, bool debug) {
   worstDistance = 0.0;
   worstPriority = 0;
   worstBoundary = "";
   worstIsAcute = true;

   double epsilon = 1.0e-12;

   // Determine which branch we're in
   bool isInAcuteBranch = (g[3] >= 0 && g[4] >= 0 && g[5] >= 0);
   bool isInObtuseBranch = (g[3] <= 0 && g[4] <= 0 && g[5] <= 0);
   bool isMixedSigns = (!isInAcuteBranch && !isInObtuseBranch);

   if (debug) {
      std::cout << "    Checking all boundaries: " << std::endl;
      std::cout << "    IsNiggli " << ((Niggli::IsNiggli(g)) ? "true" : "false") << std::endl;
      std::cout << "    g " << g << std::endl;
   }

   // Error check: Mixed signs should never occur after Phase 1
   if (isMixedSigns) {
      if (debug) {
         std::cout << "ERROR: Mixed signs detected - Phase 1 failed to establish pure branch\n";
         std::cout << "g4=" << g[3] << " g5=" << g[4] << " g6=" << g[5] << "\n";
      }
      return; // No valid boundary can be applied
   }

   // Priority 1: Check boundaries 3, 4, 5
   std::vector<std::string> priority1Boundaries = { "3", "4", "5" };

   // Acute branch
   if (isInAcuteBranch) {
      for (const auto& boundaryName : priority1Boundaries) {
         double distance = signedDistanceToBoundary(g, boundaryName, true);
         if (debug) {
            std::cout << boundaryName << " (" << distance << ") ";
         }
         if (distance < worstDistance) {
            worstDistance = distance;
            worstBoundary = boundaryName;
            worstIsAcute = true;
            worstPriority = 1;
         }
      }
   }

   // Obtuse branch - with precondition checks
   if (isInObtuseBranch) {
      for (const auto& boundaryName : priority1Boundaries) {
         bool preconditionMet = false;

         if (boundaryName == "3") {
            // R5: |g4| > |g2| AND g2 + g3 + g4 > 0 (keeps g3 positive)
            if (std::abs(g[3]) > std::abs(g[1]) + epsilon) {
               if (g[1] + g[2] + g[3] > epsilon) {
                  preconditionMet = true;
               }
            }
         } else if (boundaryName == "4") {
            // R6: |g5| > |g1| AND g1 + g3 + g5 > 0 (keeps g3 positive)
            if (std::abs(g[4]) > std::abs(g[0]) + epsilon) {
               if (g[0] + g[2] + g[4] > epsilon) {
                  preconditionMet = true;
               }
            }
         } else if (boundaryName == "5") {
            // R7: |g6| > |g1| AND g1 + g2 + g6 > 0 (keeps g2 positive)
            if (std::abs(g[5]) > std::abs(g[0]) + epsilon) {
               if (g[0] + g[1] + g[5] > epsilon) {
                  preconditionMet = true;
               }
            }
         }

         if (preconditionMet) {
            double distance = signedDistanceToBoundary(g, boundaryName, false);
            if (debug) {
               std::cout << boundaryName << " (" << distance << ") ";
            }
            if (distance < worstDistance) {
               worstDistance = distance;
               worstBoundary = boundaryName;
               worstIsAcute = false;
               worstPriority = 1;
            }
         }
      }
   }

   if (worstDistance < -epsilon) {
      if (debug) {
         std::cout << "\n    " << (worstIsAcute ? "Acute" : "Obtuse")
            << " boundary " << worstBoundary << " distance: " << worstDistance << " [VIOLATION]\n";
      }
      return;
   }

   if (debug) {
      std::cout << "\n    Checking boundaries: ";
   }

   // Priority 2: Face diagonal boundaries (acute only)
   if (isInAcuteBranch) {
      std::vector<std::string> priority2Boundaries = { "6/7", "9/A", "C/D" };

      for (const auto& boundaryName : priority2Boundaries) {
         double distance = signedDistanceToBoundary(g, boundaryName, true);
         if (debug) {
            std::cout << boundaryName << " (" << distance << ") ";
         }
         if (distance < worstDistance) {
            worstDistance = distance;
            worstBoundary = boundaryName;
            worstIsAcute = true;
            worstPriority = 2;
         }
      }
   }

   if (worstDistance < -epsilon) {
      if (debug) {
         std::cout << "\n    Acute boundary " << worstBoundary << " distance: " << worstDistance << " [VIOLATION]\n";
      }
      return;
   }

   if (debug) {
      std::cout << "\n    Checking boundaries: ";
   }

   // Priority 3: Obtuse-only boundaries
   if (isInObtuseBranch) {
      std::vector<std::string> priority3Boundaries;
      priority3Boundaries.push_back("8");

      // Boundary B (R10) - check preconditions
      if ((std::abs(g[4] - g[0]) <= epsilon && 2.0 * g[3] < g[5] + epsilon) ||
         (std::abs(g[4] + g[0]) <= epsilon && g[5] < 0.0)) {
         priority3Boundaries.push_back("B");
      }

      // Boundary E (R11) - check preconditions
      if ((std::abs(g[5] - g[0]) <= epsilon && 2.0 * g[3] < g[4] + epsilon) ||
         (std::abs(g[5] + g[0]) <= epsilon && g[4] < 0.0)) {
         priority3Boundaries.push_back("E");
      }

      // Boundary R8: Body diagonal for obtuse branch
      double bodyDiagSum = g[3] + g[4] + g[5] + std::abs(g[0]) + std::abs(g[1]);
      if (bodyDiagSum + epsilon < 0.0) {
         if (debug) {
            std::cout << "R8(body diag) ";
         }
         priority3Boundaries.push_back("R8");
      }

      // Boundary F (R12)
      if ((std::abs(bodyDiagSum) <= epsilon) &&
         (2.0 * (std::abs(g[0]) + g[4]) + g[5] > epsilon)) {
         if (debug) {
            std::cout << "F(R12 check) ";
         }
         priority3Boundaries.push_back("F");
      }

      // Check all priority 3 boundaries
      for (const auto& boundaryName : priority3Boundaries) {
         double distance = signedDistanceToBoundary(g, boundaryName, false);
         if (debug) {
            std::cout << boundaryName << " (" << distance << ") ";
         }
         if (distance < worstDistance) {
            worstDistance = distance;
            worstBoundary = boundaryName;
            worstIsAcute = false;
            worstPriority = 3;
         }
      }
   }

   if (worstDistance < -epsilon) {
      if (debug) {
         std::cout << "\n    Obtuse boundary " << worstBoundary << " distance: " << worstDistance << " [VIOLATION]\n";
      }
      return;
   }

   if (debug) {
      std::cout << "\n    Checking boundaries: ";
   }

   // Priority 4: Special 4D boundaries
   std::vector<std::string> priority4Boundaries = { "D'", "7'", "A'", "8'", "B'", "E'" };

   for (const auto& boundaryName : priority4Boundaries) {
      if (isInAcuteBranch) {
         double acuteDistance = signedDistanceToBoundary(g, boundaryName, true);
         if (acuteDistance < worstDistance) {
            worstDistance = acuteDistance;
            worstBoundary = boundaryName;
            worstIsAcute = true;
            worstPriority = 4;
         }
      }

      if (isInObtuseBranch) {
         double obtuseDistance = signedDistanceToBoundary(g, boundaryName, false);
         if (obtuseDistance < worstDistance) {
            worstDistance = obtuseDistance;
            worstBoundary = boundaryName;
            worstIsAcute = false;
            worstPriority = 4;
         }
      }

      if (debug) {
         double minDist = std::min(
            isInAcuteBranch ? signedDistanceToBoundary(g, boundaryName, true) : 1e10,
            isInObtuseBranch ? signedDistanceToBoundary(g, boundaryName, false) : 1e10
         );
         std::cout << boundaryName << " (" << minDist << ") ";
      }
   }

   if (debug && worstDistance < -epsilon) {
      std::cout << "\n    " << (worstIsAcute ? "Acute" : "Obtuse")
         << " boundary " << worstBoundary << " distance: " << worstDistance << " [VIOLATION]\n";
   }
}

void NiggliProjectors::testTransformationMatrices() {
   std::cout << "\n======================================================================\n";
   std::cout << "Testing transformation matrix volume preservation\n";
   std::cout << "======================================================================\n";

   // Generate a random general G6 vector
   G6 testG6(10.5, 15.3, 20.7, 5.2, -3.8, 7.1);
   LRL_Cell originalCell(testG6);
   double originalVolume = originalCell.Volume();

   std::cout << "Original G6: " << testG6 << std::endl;
   std::cout << "Original Volume: " << originalVolume << std::endl << std::endl;

   // Test all acute boundaries
   std::cout << "ACUTE BOUNDARIES:\n";
   for (const auto& boundary : acuteBoundaries) {
      G6 transformed = boundary.transformation * testG6;
      LRL_Cell transformedCell(transformed);
      double transformedVolume = transformedCell.Volume();
      double volumeDiff = std::abs(transformedVolume - originalVolume);

      std::cout << "Boundary " << boundary.name << ": ";
      std::cout << "Volume = " << transformedVolume;
      std::cout << ", Diff = " << volumeDiff;

      if (volumeDiff > 1.0e-6) {
         std::cout << " **VOLUME NOT PRESERVED**";
      }
      std::cout << std::endl;
   }

   // Test all obtuse boundaries
   std::cout << "\nOBTUSE BOUNDARIES:\n";
   for (const auto& boundary : obtuseBoundaries) {
      G6 transformed = boundary.transformation * testG6;
      LRL_Cell transformedCell(transformed);
      double transformedVolume = transformedCell.Volume();
      double volumeDiff = std::abs(transformedVolume - originalVolume);

      std::cout << "Boundary " << boundary.name << ": ";
      std::cout << "Volume = " << transformedVolume;
      std::cout << ", Diff = " << volumeDiff;

      if (volumeDiff > 1.0e-6) {
         std::cout << " **VOLUME NOT PRESERVED**";
      }
      std::cout << std::endl;
   }
}

