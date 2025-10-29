#include <iostream>

#include "TestControls.h"
#include "ProgramSetup.h"

#include "EdgeTransformationMatrices.h"
#include "EisensteinReduction.h"
#include "G6.h"
#include "GetTrace.h"
#include "LRL_Cell.h"
#include "KrivyGruber.h"
#include "MatrixCellOperator.h"
#include "Niggli.h"
#include "ScoredReservoir.h"
#include "TestToConvergence.h"
#include "TransformationMatrices.h"

#include <sstream>


int main() {
   std::cout << "; Test Reservoir" << std::endl;

   TestControls controls;
   const BasicProgramInput<TestControls> dc_setup("Test Reservoir", controls);

   if (dc_setup.getInputList().empty()) {
      throw std::runtime_error("; No input vectors provided");
   }

   if (controls.shouldShowControls()) {
      std::cout << controls << std::endl;
   }

   for (const auto& input : dc_setup.getInputList()) {
      ScoredReservoir<G6> res(5);
      res.setCheckDuplicateKeys(true, 0.001);
      std::ostringstream title;
      LRL_Cell cell = input.getCell();

      //static const std::vector<Matrix_3x3> matrices = TransformationMatrices::generateUnimodularMatrices();
      static const std::vector<Matrix_3x3> matrices = GetAllTransforms();
      title << "test for " << matrices.size() << " matrices, one pass only";
      res.setTitle(title.str());
      double previousTrace = DBL_MAX;
      G6 best;
      for (const auto& m : matrices) {
         G6 g6 = m * cell;
         if (GetTrace(g6) < previousTrace) {
            previousTrace = GetTrace(g6);
            best = g6;
            cell = g6;
         }
         res.add(GetTrace(g6), g6);
      }
      G6 out;
      Niggli::ReduceWithoutMatrices(cell, out, 1.0E-8);
      std::cout << "Niggli,          trace=" << std::fixed << std::setprecision(3)<< GetTrace(out) << std::endl;
      const KrivyGruber::ReductionResult result = KrivyGruber::Reduce(cell);
      std::cout << "KrivyGruber,     trace=" << std::fixed << std::setprecision(3) << GetTrace(result.reducedG6) << std::endl;
      const EisensteinReduction::ReductionResult eresult = EisensteinReduction::Reduce(cell, 1.0E-8);
      std::cout << "Eisenstein3480,  trace=" << std::fixed << std::setprecision(3) << GetTrace(eresult.reducedCell) << std::endl;
      std::tuple<double,G6,int> einsteinResult =TestToConvergence(cell, 100, GetAllTransforms(), 1.0E-8);
      std::cout << "Eisenstein24,    trace=" << std::fixed << std::setprecision(3) << GetTrace(eresult.reducedCell) << std::endl;


      std::cout << std::endl;
      std::cout << "Niggli,          reduced= " << out << std::endl;
      std::cout << "KrivyGruber,     reduced= " << result.reducedG6 << std::endl;
      std::cout << "Eisenstein3480,  reduced= " << G6(eresult.reducedCell) << std::endl;
      std::cout << "Eisenstein24,    reduced= " << std::get<1>(einsteinResult) << std::endl;
      std::cout << std::endl;




      std::cout << res << std::endl;
   }

   return 0;
}
