#ifndef RHOMBOHEDRAL_TEST_SUITE_H
#define RHOMBOHEDRAL_TEST_SUITE_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "InputHandler.h"
#include "R_Settings.h"
#include "Processing.h"

class RhombohedralTestSuite {
public:
   struct TestCase {
      std::string name;
      std::string input;
      bool shouldSucceed;
      std::string expectedNote;
      double expectedVolumeRatio;

      TestCase(const std::string& n, const std::string& i, bool succeed = true,
         const std::string& note = "", double volRatio = 3.0)
         : name(n), input(i), shouldSucceed(succeed), expectedNote(note), expectedVolumeRatio(volRatio) {
      }
   };

private:
   static std::vector<TestCase> basicRhomboTests;
   static std::vector<TestCase> hexRhomboTests;
   static std::vector<TestCase> nonRhomboTests;
   static std::vector<TestCase> edgeCaseTests;
   static std::vector<TestCase> invalidTests;

   static bool runSingleTest(const TestCase& test, int testNum) {
      std::cout << "\n--- Test " << testNum << ": " << test.name << " ---\n";
      std::cout << "Input: " << test.input << "\n";

      try {
         const LatticeCell latticeCell = InputHandler::processSingleLatticeInput(test.input);

         if (!test.shouldSucceed) {
            std::cout << "❌ FAIL: Expected failure but test succeeded\n";
            return false;
         }

         // Run the processing
         process_lattice_cell_input(latticeCell);

         std::cout << "✅ PASS: Test completed successfully\n";
         return true;

      }
      catch (const std::exception& e) {
         if (test.shouldSucceed) {
            std::cout << "❌ FAIL: Expected success but got error: " << e.what() << "\n";
            return false;
         }
         else {
            std::cout << "✅ PASS: Correctly rejected invalid input: " << e.what() << "\n";
            return true;
         }
      }
   }

public:
   static void initializeTests() {
      // Test Group 1: Basic Rhombohedral Cells
      basicRhomboTests = {
          TestCase("Cubic Close Packing (60°)", "R 10 10 10 60 60 60", true, "cubic close packing"),
          TestCase("Simple Cubic (90°)", "R 10 10 10 90 90 90", true, "simple cubic"),
          TestCase("Very Acute (10°)", "R 10 10 10 10 10 10", true, "Very acute"),
          TestCase("Obtuse (119°)", "R 10 10 10 119 119 119", true, ""),
          TestCase("Near Limit (119.9°)", "R 10 10 10 119.9 119.9 119.9", true, ""),
          TestCase("45° Rhombohedral", "R 10 10 10 45 45 45", true, "Very acute")
      };

      // Test Group 2: Hexagonal Rhombohedral 
      hexRhomboTests = {
          TestCase("Standard Hex-Rhombo", "R 10 10 15 90 90 120", true, ""),
          TestCase("P vs R Same Cell", "P 10 10 15 90 90 120", true, ""),
          TestCase("Another Hex-Rhombo", "R 8 8 12 90 90 120", true, "")
      };

      // Test Group 3: Non-Rhombohedral Cells
      nonRhomboTests = {
          TestCase("Simple Cubic (P)", "P 10 10 10 90 90 90", true, "not have rhombohedral"),
          TestCase("Tetragonal", "P 10 10 15 90 90 90", true, "not have rhombohedral"),
          TestCase("Orthorhombic", "P 10 12 15 90 90 90", true, "not have rhombohedral")
      };

      // Test Group 4: Edge Cases
      edgeCaseTests = {
          TestCase("Very Small Cell", "R 0.1 0.1 0.1 45 45 45", true, "Very acute"),
          TestCase("Close to 90°", "R 10 10 10 89.9 89.9 89.9", true, ""),
          TestCase("G6 Cubic Input", "G6 100 100 100 0 0 0", true, "simple cubic"),
          TestCase("Round-trip Test", "R 8.5 8.5 8.5 73.2 73.2 73.2", true, "")
      };

      // Test Group 5: Invalid Inputs (should fail)
      invalidTests = {
          TestCase("Invalid 150° Angle", "R 10 10 10 150 150 150", false),
          TestCase("Invalid 120.1° Angle", "R 10 10 10 120.1 120.1 120.1", false),
          TestCase("Degenerate 120° Angle", "R 10 10 10 120 120 120", false),
          TestCase("Too Few Parameters", "R 10 10", false),
          TestCase("Invalid Characters", "R abc def ghi", false),
          TestCase("Missing Parameter", "R 10 10 10 10 10", false)
      };
   }

   static void runTestGroup(const std::vector<TestCase>& tests, const std::string& groupName, int& totalTests, int& passedTests) {
      std::cout << "\n" << std::string(60, '=') << "\n";
      std::cout << "  " << groupName << "\n";
      std::cout << std::string(60, '=') << "\n";

      int groupPassed = 0;
      for (size_t i = 0; i < tests.size(); ++i) {
         if (runSingleTest(tests[i], totalTests + 1)) {
            passedTests++;
            groupPassed++;
         }
         totalTests++;
      }

      std::cout << "\nGroup Summary: " << groupPassed << "/" << tests.size() << " tests passed\n";
   }

   static void runAllTests() {
      initializeTests();

      std::cout << "\n" << std::string(70, '=') << "\n";
      std::cout << "  RHOMBOHEDRAL ANALYSIS SYSTEM - COMPREHENSIVE TEST SUITE\n";
      std::cout << std::string(70, '=') << "\n";

      int totalTests = 0;
      int passedTests = 0;

      runTestGroup(basicRhomboTests, "GROUP 1: BASIC RHOMBOHEDRAL CELLS", totalTests, passedTests);
      runTestGroup(hexRhomboTests, "GROUP 2: HEXAGONAL RHOMBOHEDRAL CELLS", totalTests, passedTests);
      runTestGroup(nonRhomboTests, "GROUP 3: NON-RHOMBOHEDRAL CELLS", totalTests, passedTests);
      runTestGroup(edgeCaseTests, "GROUP 4: EDGE CASES", totalTests, passedTests);
      runTestGroup(invalidTests, "GROUP 5: INVALID INPUTS (SHOULD FAIL)", totalTests, passedTests);

      // Final Summary
      std::cout << "\n" << std::string(70, '=') << "\n";
      std::cout << "  FINAL TEST RESULTS\n";
      std::cout << std::string(70, '=') << "\n";
      std::cout << "Total Tests: " << totalTests << "\n";
      std::cout << "Passed: " << passedTests << "\n";
      std::cout << "Failed: " << (totalTests - passedTests) << "\n";
      std::cout << "Success Rate: " << std::fixed << std::setprecision(1)
         << (100.0 * passedTests / totalTests) << "%\n";

      if (passedTests == totalTests) {
         std::cout << "\n🎉 ALL TESTS PASSED! The rhombohedral analysis system is working perfectly.\n";
      }
      else {
         std::cout << "\n⚠️  Some tests failed. Please review the results above.\n";
      }
      std::cout << std::string(70, '=') << "\n";
   }

   static void runSpecificTest(int testNumber) {
      initializeTests();

      // Combine all tests into one vector for numbering
      std::vector<TestCase> allTests;
      std::vector<std::string> groupNames;

      // Add all test groups
      for (const auto& test : basicRhomboTests) {
         allTests.push_back(test);
         groupNames.push_back("Basic Rhombohedral");
      }
      for (const auto& test : hexRhomboTests) {
         allTests.push_back(test);
         groupNames.push_back("Hexagonal Rhombohedral");
      }
      for (const auto& test : nonRhomboTests) {
         allTests.push_back(test);
         groupNames.push_back("Non-Rhombohedral");
      }
      for (const auto& test : edgeCaseTests) {
         allTests.push_back(test);
         groupNames.push_back("Edge Cases");
      }
      for (const auto& test : invalidTests) {
         allTests.push_back(test);
         groupNames.push_back("Invalid Inputs");
      }

      if (testNumber < 1 || testNumber > static_cast<int>(allTests.size())) {
         std::cout << "Invalid test number. Available tests:\n\n";
         for (size_t i = 0; i < allTests.size(); ++i) {
            std::cout << std::setw(3) << (i + 1) << ". "
               << groupNames[i] << ": " << allTests[i].name << "\n";
         }
         std::cout << "\nUse 'test 0' to run all tests.\n";
         return;
      }

      std::cout << "\nRunning specific test #" << testNumber << ":\n";
      runSingleTest(allTests[testNumber - 1], testNumber);
   }

   static void showTestList() {
      initializeTests();

      std::cout << "\n" << std::string(60, '=') << "\n";
      std::cout << "  AVAILABLE TESTS\n";
      std::cout << std::string(60, '=') << "\n";

      int testNum = 1;

      std::cout << "\nBASIC RHOMBOHEDRAL CELLS:\n";
      for (const auto& test : basicRhomboTests) {
         std::cout << std::setw(3) << testNum++ << ". " << test.name << "\n";
      }

      std::cout << "\nHEXAGONAL RHOMBOHEDRAL CELLS:\n";
      for (const auto& test : hexRhomboTests) {
         std::cout << std::setw(3) << testNum++ << ". " << test.name << "\n";
      }

      std::cout << "\nNON-RHOMBOHEDRAL CELLS:\n";
      for (const auto& test : nonRhomboTests) {
         std::cout << std::setw(3) << testNum++ << ". " << test.name << "\n";
      }

      std::cout << "\nEDGE CASES:\n";
      for (const auto& test : edgeCaseTests) {
         std::cout << std::setw(3) << testNum++ << ". " << test.name << "\n";
      }

      std::cout << "\nINVALID INPUTS (SHOULD FAIL):\n";
      for (const auto& test : invalidTests) {
         std::cout << std::setw(3) << testNum++ << ". " << test.name << "\n";
      }

      std::cout << "\nUSAGE:\n";
      std::cout << "  test 0       - Run all tests\n";
      std::cout << "  test <num>   - Run specific test\n";
      std::cout << "  test -1      - Show this list\n";
      std::cout << std::string(60, '=') << "\n";
   }
};

// Initialize static members
std::vector<RhombohedralTestSuite::TestCase> RhombohedralTestSuite::basicRhomboTests;
std::vector<RhombohedralTestSuite::TestCase> RhombohedralTestSuite::hexRhomboTests;
std::vector<RhombohedralTestSuite::TestCase> RhombohedralTestSuite::nonRhomboTests;
std::vector<RhombohedralTestSuite::TestCase> RhombohedralTestSuite::edgeCaseTests;
std::vector<RhombohedralTestSuite::TestCase> RhombohedralTestSuite::invalidTests;

#endif // RHOMBOHEDRAL_TEST_SUITE_H
