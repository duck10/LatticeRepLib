#ifndef NIGGLI_PROJECTORS_H
#define NIGGLI_PROJECTORS_H

#include "G6.h"
#include "MatG6.h"

#include <string>
#include <vector>
#include <iostream>

struct BoundaryPolytope {
   std::string name;           // e.g., "1", "2", ..., "F"
   std::string condition;      // e.g., "g1 = g2"
   std::string classification; // "Equal cell edges", "90°", "Face diagonal", "Body diagonal"
   MatG6 transformation;       // The boundary transformation matrix (6×6)
   MatG6 projector;           // Projector into the boundary hyperplane (6×6)

   BoundaryPolytope(const std::string& n, const std::string& cond,
      const std::string& cls,
      const MatG6& trans, const MatG6& proj)
      : name(n), condition(cond), classification(cls),
      transformation(trans), projector(proj) {
   }

   friend std::ostream& operator<<(std::ostream& os, const BoundaryPolytope& bp);
};

class NiggliProjectors {
private:
   std::vector<BoundaryPolytope> acuteBoundaries;   // For +++ branch (g4,g5,g6 > 0)
   std::vector<BoundaryPolytope> obtuseBoundaries;  // For --- branch (g4,g5,g6 < 0)

   void initializeAcuteBoundaries();
   void initializeObtuseBoundaries();
   // Add to NiggliProjectors class (private section in .h file):
   std::string selectSubBoundary(const G6& g, const std::string& boundaryName, double epsilon) const;

public:
   NiggliProjectors();

   // Get boundaries for a specific branch
   const std::vector<BoundaryPolytope>& getAcuteBoundaries() const { return acuteBoundaries; }
   const std::vector<BoundaryPolytope>& getObtuseBoundaries() const { return obtuseBoundaries; }

   // Get a specific boundary by name from the appropriate branch
   const BoundaryPolytope* getBoundary(const std::string& name, bool isAcute) const;

   // Project a G6 vector onto a boundary polytope
   G6 projectToBoundary(const G6& g, const std::string& boundaryName, bool isAcute) const;

   // Calculate signed distance from a G6 point to a boundary
   // Positive distance means inside Niggli cone, negative means outside
   double signedDistanceToBoundary(const G6& g, const std::string& boundaryName, bool isAcute) const;

   // Check if a point is inside the Niggli cone relative to a boundary
   bool isInsideRelativeToBoundary(const G6& g, const std::string& boundaryName, bool isAcute) const;

   // Apply boundary transformation
   G6 applyTransformation(const G6& g, const std::string& boundaryName, bool isAcute) const;

   // Determine which branch a G6 vector belongs to
   static bool isAcuteBranch(const G6& g);

   static std::string getSignCorrectionBoundary(const G6& g, double epsilon = 1.0e-8);

   double maxViolation(const G6& g, double epsilon) const;

   static void ApplyPhase1(G6& current, const NiggliProjectors& np, double epsilon, bool debug);

   G6 phase1(const G6& g, bool debug);

   void findWorstViolation(const G6& g, std::string& worstBoundary, bool& worstIsAcute,
      double& worstDistance, int& worstPriority, bool debug);

   void testTransformationMatrices();


   // Output operator
   friend std::ostream& operator<<(std::ostream& os, const NiggliProjectors& np);
};

#endif // NIGGLI_PROJECTORS_H

