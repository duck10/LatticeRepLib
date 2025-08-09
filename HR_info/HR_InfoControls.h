// Add to HR_InfoControls.h

#ifndef HR_INFO_CONTROLS_H
#define HR_INFO_CONTROLS_H

#include "BaseControlVariables.h"
#include "BlockUtils.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"
#include "Processing.h"
#include "R_Settings.h"
#include "RhombohedralTestSuite.h"  // Add this include

#include <sstream>

class HR_InforControls : public BaseControlVariables {

public:

   friend std::ostream& operator<< (std::ostream& os, const HR_InforControls& csc) {
      std::cout << "Commands:\n"
         "  sg <number>\n"
         "  bravais hP|hR\n"
         "  system Trigonal|Hexagonal\n"
         "  family Hexagonal\n"
         "  hex <a> <c>\n"
         "  rhomb <a> <alpha>\n"
         "  cell <a> <b> <c> <alpha> <beta> <gamma>\n"
         "  r_settings\n"
         "  test [number]    (run test suite)\n"
         "  help\n"
         //"  q (quit)\n\n"
         "Lattice inputs (lattice type must be first):\n"
         "  P <a> <b> <c> <alpha> <beta> <gamma>     (primitive)\n"
         "  A <a> <b> <c> <alpha> <beta> <gamma>     (A-centered)\n"
         "  B <a> <b> <c> <alpha> <beta> <gamma>     (B-centered)\n"
         "  C <a> <b> <c> <alpha> <beta> <gamma>     (C-centered)\n"
         "  F <a> <b> <c> <alpha> <beta> <gamma>     (F-centered)\n"
         "  I <a> <b> <c> <alpha> <beta> <gamma>     (I-centered)\n"
         "  H <a> <b> <c> <alpha> <beta> <gamma>     (H-centered)\n"
         "  R <a> <b> <c> <alpha> <beta> <gamma>     (R-centered)\n"
         "  G6 <g1> <g2> <g3> <g4> <g5> <g6>  (G6 representation)\n"
         "  S6 <s1> <s2> <s3> <s4> <s5> <s6>  (S6 representation)\n"
         "  P3 <x1> <y1> <x2> <y2> <x3> <y3>  (P3 representation)\n"
         "  RANDOM                            (random lattice)\n\n";
      return os;
   }

   HR_InforControls() {
      // Register existing command handlers
      InputHandler::registerHandler("HELP", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            showHelp();
         });

      //InputHandler::registerHandler("SHOW", 0.0,
      //   [this](const BaseControlVariables&, const std::string& value) {
      //      std::cout << (*this) << std::endl;
      //   });

      InputHandler::registerHandler("SG", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            process_sg(std::stoi(value));
         });

      InputHandler::registerHandler("FAMILY", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            process_family();
         });

      InputHandler::registerHandler("BRAVAIS", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            process_bravais(value);
         });

      InputHandler::registerHandler("SYSTEM", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            process_system(value);
         });

      InputHandler::registerHandler("HEX", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            std::istringstream iss(value);
            double a, c;
            iss >> a >> c;
            if (!iss.fail()) {
               process_hex(a, c);
            }
            else {
               std::cout << "Need 2 numbers: a c\n";
            }
         });

      InputHandler::registerHandler("RHOMB", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            std::istringstream iss(value);
            double a, alpha;
            iss >> a >> alpha;
            if (!iss.fail()) {
               process_rhomb(a, alpha);
            }
            else {
               std::cout << "Need 2 numbers: a alpha(deg)\n";
            }
         });

      InputHandler::registerHandler("CELL", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            std::istringstream iss(value);
            double a, b, c, alpha, beta, gamma;
            iss >> a >> b >> c >> alpha >> beta >> gamma;
            if (!iss.fail()) {
               Cell cell{ a, b, c, alpha, beta, gamma };
               process_cell(cell);
            }
            else {
               std::cout << "Usage: cell a b c alpha beta gamma\n";
            }
         });

      InputHandler::registerHandler("R_SETTINGS", 0.60,
         [this](const BaseControlVariables&, const std::string& value) {
            R_Setting_Info();
         });

      // Add the TEST handler
      InputHandler::registerHandler("TEST", 0.43,
         [this](const BaseControlVariables&, const std::string& value) {
            try {
               if (value.empty() || LRL_StringTools::strToupper(value) == "TEST") {
                  // Just "test" with no number - run all tests
                  RhombohedralTestSuite::runAllTests();
               }
               else {
                  // "test" followed by a number - parse and set
                  int testNum = std::stoi(value);
                  if (testNum < 0) {
                     // Negative numbers show test list
                     RhombohedralTestSuite::showTestList();
                  }
                  else if (testNum == 0) {
                     // 0 means run all tests
                     RhombohedralTestSuite::runAllTests();
                  }
                  else {
                     // Positive number - run specific test
                     RhombohedralTestSuite::runSpecificTest(testNum);
                  }
               }
            }
            catch (const std::exception& e) {
               std::cout << "Warning: Invalid test number: " << value << ". Use 'test -1' to see available tests." << std::endl;
               RhombohedralTestSuite::showTestList();
            }
         });

      // Register lattice type handlers
      InputHandler::registerHandler("P", 1.0,
         [this](const BaseControlVariables&, const std::string& value) {
            processLatticeInput("P", value);
         });

      InputHandler::registerHandler("A", 1.0,
         [this](const BaseControlVariables&, const std::string& value) {
            processLatticeInput("A", value);
         });

      InputHandler::registerHandler("B", 1.0,
         [this](const BaseControlVariables&, const std::string& value) {
            processLatticeInput("B", value);
         });

      InputHandler::registerHandler("C", 1.0,
         [this](const BaseControlVariables&, const std::string& value) {
            processLatticeInput("C", value);
         });

      InputHandler::registerHandler("F", 1.0,
         [this](const BaseControlVariables&, const std::string& value) {
            processLatticeInput("F", value);
         });

      InputHandler::registerHandler("I", 1.0,
         [this](const BaseControlVariables&, const std::string& value) {
            processLatticeInput("I", value);
         });

      InputHandler::registerHandler("H", 1.0,
         [this](const BaseControlVariables&, const std::string& value) {
            processLatticeInput("H", value);
         });

      InputHandler::registerHandler("R", 1.0,
         [this](const BaseControlVariables&, const std::string& value) {
            processLatticeInput("R", value);
         });

      InputHandler::registerHandler("G6", 1.0,
         [this](const BaseControlVariables&, const std::string& value) {
            processLatticeInput("G6", value);
         });

      InputHandler::registerHandler("G", 1.0,
         [this](const BaseControlVariables&, const std::string& value) {
            processLatticeInput("G", value);
         });

      InputHandler::registerHandler("S6", 1.0,
         [this](const BaseControlVariables&, const std::string& value) {
            processLatticeInput("S6", value);
         });

      InputHandler::registerHandler("S", 1.0,
         [this](const BaseControlVariables&, const std::string& value) {
            processLatticeInput("S", value);
         });

      InputHandler::registerHandler("P3", 1.0,
         [this](const BaseControlVariables&, const std::string& value) {
            processLatticeInput("P3", value);
         });

      InputHandler::registerHandler("RANDOM", 1.0,
         [this](const BaseControlVariables&, const std::string& value) {
            processLatticeInput("RANDOM", value);
         });

      InputHandler::registerHandler("QUIT", 0.50,
         [this](const BaseControlVariables&, const std::string& value) {
            return 0;
         });

      InputHandler::registerHandler("Q", 0.0,
         [this](const BaseControlVariables&, const std::string& value) {
            return 0;
         });
   }

private:
   void processLatticeInput(const std::string& latticeType, const std::string& parameters) {
      try {
         // Reconstruct the full input line with lattice type first
         const std::string fullLine = latticeType + " " + parameters;

         // Use InputHandler to parse this lattice input
         const LatticeCell latticeCell = InputHandler::processSingleLatticeInput(fullLine);

         // Process the parsed lattice cell
         process_lattice_cell_input(latticeCell);

      }
      catch (const std::exception& e) {
         std::cerr << "Error processing " << latticeType << " input: " << e.what() << std::endl;
      }
   }
};

#endif // HR_INFO_CONTROLS_H

