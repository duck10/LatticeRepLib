#include <iostream>

#include "TemplateControls.h"
#include "ProgramSetup.h"

#include "SellaBuild.h"
#include "Selling.h"
#include "Niggli.h"
#include "S6.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "MatrixCellOperator.h"

int main() {
   std::cout << "; LRL_TEMPLATE" << std::endl;
   int countErrors = 0;

   try {
      TemplateControls controls;
      const BasicProgramInput<TemplateControls> dc_setup("TEMPLATE", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {

         S6 out;
         S6 out3D;
         Selling::Reduce(input.getCell(), out);
         MatS6 ms6;
         Matrix_3x3 m33;
         Selling::ReduceWithtransforms(input.getCell(), ms6, out3D, m33);

         S6 niggliS6;
         S6 niggliS63D;
         Niggli::Reduce(out, niggliS6);
         Niggli::Reduce(out3D, niggliS63D);


         const double fit = (P3(niggliS6) - P3(niggliS63D)).norm();
         //std::cout << (niggliS6) << std::endl;
         //std::cout << (niggliS63D) << std::endl << std::endl;
         if (fit > 0.001 ) {

            ++countErrors;
            std::cout << ";Test Selling 3D input " << input;
            std::cout << " fit " << fit << std::endl;
            std::cout << out << std::endl;
            std::cout << out3D << std::endl << std::endl;
         }
      }
      if (countErrors == 0) {
         std::cout << "no disagreements were found" << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cout << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}
