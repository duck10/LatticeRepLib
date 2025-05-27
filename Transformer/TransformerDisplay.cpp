#pragma warning(disable: 4566) // character set issue

#include "TransformerDisplay.h"
#include "TransformerUtilities.h"

#include <iostream>
#include <iomanip>

#include "CS6Dist.h"
#include "CS6Dist.cpp"

TransformerDisplay::TransformerDisplay(const MultiTransformFinderControls& controls)
   : m_controls(controls) {
}

void TransformerDisplay::showInputCells(const LatticeCell& sourceCell, const LatticeCell& referenceCell) const {
   LRL_Cell_Degrees sourceCellDeg(sourceCell.getCell());
   LRL_Cell_Degrees referenceCellDeg(referenceCell.getCell());
   std::cout << "Cell to Transform (Degrees): " << sourceCell.getLatticeType() << " "
      << sourceCellDeg << std::endl;
   std::cout << "Reference Cell (Degrees):    " << referenceCell.getLatticeType() << " "
      << referenceCellDeg << std::endl;
}

void TransformerDisplay::showCompleteTransformations(
   const CompleteTransformationResult& result,
   const LatticeCell& mobileCell,
   const LatticeCell& referenceCell) const {

   if (!result.primitiveResult.isValid || result.primitiveResult.transformations.empty()) {
      std::cout << "No valid transformations found.\n";
      return;
   }
   else
   {
      std::cout << "\nFound " << result.primitiveResult.transformations.size()
         << " valid transformations.\n";
   }

   // Show centering information
   std::cout << "Input centering: " << result.inputCenteringType
      << ", Reference centering: " << result.referenceCenteringType << std::endl;

   // Only show these matrices if details are requested
   if (m_controls.shouldShowDetails()) {
      std::cout << "Input-to-primitive matrix:\n" << result.inputToPrimitiveMatrix << std::endl;
      std::cout << "Reference-to-primitive matrix:\n" << result.referenceToPrimitiveMatrix << std::endl;
   }

   std::cout << "\nBest transformations";
   if (m_controls.shouldSortByP3()) {
      std::cout << " (sorted by P3 distance):\n";
   }
   else {
      std::cout << " (sorted by S6 angle):\n";
   }

   int count = 0;
   for (const auto& transform : result.primitiveResult.transformations) {
      count++;
      std::cout << "=================================\n";
      std::cout << "Transformation " << count << ":\n";


      const LRL_Cell reducedReference = NiggliReduce(referenceCell);
      const LRL_Cell reducedMobile = NiggliReduce(mobileCell);
      const double niggliDistance = CalculateNCDist(reducedMobile, reducedReference);



      // Show applied to/result/reference cells
      std::cout << "Applied to:     " << std::setw(5) << mobileCell.getLatticeType()
         << " " << LRL_Cell_Degrees(mobileCell.getCell()) << std::endl;
      std::cout << "Result:         " << std::setw(5) << referenceCell.getLatticeType()
         << " " << LRL_Cell_Degrees(transform.transformedCompleteCell) << std::endl;
      std::cout << "Reference:      " << std::setw(5) << referenceCell.getLatticeType()
         << " " << LRL_Cell_Degrees(referenceCell.getCell()) << std::endl;

      // Show quality metrics
      std::cout << "Quality:    P3 Distance: " << std::fixed << std::setprecision(6)
         << transform.p3DistanceFinal << " A, ";
      std::cout << "NCDist " << niggliDistance << " A, ";
      std::cout << "S6 Angle: " << std::scientific << std::setprecision(6)
         << transform.s6AngleFinal << " degrees" << std::endl;





      //std::cout << "Result:         " << std::setw(5) << referenceCell.getLatticeType();
      //std::cout << "NCDist " << niggliDistance << " A";
      //std::cout << "S6 Angle: " << std::scientific << std::setprecision(6)
      //   << transform.s6AngleFinal << "degrees" << std::endl;

      // Check determinant of complete matrix
      const double det = transform.completeTransformMatrix.Det();

      // Check for expected values based on centering types
      bool isExpectedDet = false;
      const double tolerance = 0.01;

      // Check against common determinant values for different centerings
      if (std::abs(det - 1.0) < tolerance) isExpectedDet = true;     // P
      if (std::abs(det - 0.5) < tolerance) isExpectedDet = true;     // A, B, C
      if (std::abs(det - 0.25) < tolerance) isExpectedDet = true;    // I, F 
      if (std::abs(det - 1.0 / 9.0) < tolerance) isExpectedDet = true; // R

      // Only show warning for negative or unexpected determinants
      if (det <= 0.0) {
         std::cout << "WARNING: Invalid transformation with negative determinant: " << det << "\n";
      }
      //else if (!isExpectedDet) {
      //   std::cout << "WARNING: Unexpected determinant value: " << det << "\n";
      //   std::cout << "Expected: 1.0 (P), 0.5 (A/B/C), 0.25 (I/F), or 0.111 (R)\n";
      //}

      // Show the complete matrix with determinant info
      std::cout << "\nComplete Matrix (det = " << std::fixed << std::setprecision(3)
         << det << "):\n";
      std::cout << transform.completeTransformMatrix << std::endl;

      // Only show detailed info if requested
      if (m_controls.shouldShowDetails()) {
         //std::cout << "\nPrimitive-to-primitive Matrix:\n";
         //displayMatrix(transform.primitiveToPrimitiveMatrix);

         std::cout << "\nParameter differences:\n";
         displayParameterDifferences(transform.transformedCompleteCell,
            referenceCell.getCell());
      }

      std::cout << "=================================\n\n";
   }
}


// Helper function to display parameter differences
void TransformerDisplay::displayParameterDifferences(
   const LRL_Cell& transformed,
   const LRL_Cell& reference) const {

   const char* labels[] = { "a", "b", "c", "α", "β", "γ" };
   const double rad2deg = 180.0 / M_PI;

   for (int i = 0; i < 6; ++i) {
      double transVal = (i < 3) ? transformed[i] : transformed[i] * rad2deg;
      double refVal = (i < 3) ? reference[i] : reference[i] * rad2deg;
      double diff = std::abs(transVal - refVal);

      std::cout << "  " << labels[i] << ": "
         << std::setw(7) << std::fixed << std::setprecision(3) << transVal
         << " → "
         << std::setw(7) << refVal
         << " (Δ = " << std::setprecision(3) << diff << ")\n";
   }
}
