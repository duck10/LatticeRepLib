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
      TestSelling3DControls controls;
      const BasicProgramInput<TestSelling3DControls> dc_setup("Test Selling 3D", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      int count = 0;

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
         if (fit > 0.001  || count == 0) {

            if ( fit > 0.001)
            {
               ++countErrors;
            }
            std::cout << ";Test Selling 3D input " << input;
            std::cout << " P3 fit " << fit << std::endl;
            std::cout << out << std::endl;
            std::cout << out3D << std::endl << std::endl;
         }
         ++count;
      }
      if (countErrors == 0) {
         std::cout << "no disagreements were found" << std::endl;
      }

      Selling::Write_S6_E3_MatrixPairs();


      return 0;
   }
   catch (const std::exception& e) {
      std::cout << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}
