// TestCommandSystem.cpp
#include <iostream>
#include "RegistryTest.h"

int main() {
   CommandSystem commands;

   // Register test commands
   commands.registerCommand("SHOW_CONTROLS", [](const std::string& value) {
      std::cout << "SHOW_CONTROLS executed with value: " << value << std::endl;
      });

   commands.registerCommand("ECHO", [](const std::string& value) {
      std::cout << "ECHO executed with value: " << value << std::endl;
      });

   commands.registerCommand("WEB_INPUT", [](const std::string& value) {
      std::cout << "WEB_INPUT executed with value: " << value << std::endl;
      });

   commands.registerCommand("BLOCK_START", [](const std::string& value) {
      std::cout << "BLOCK_START executed with value: " << value << std::endl;
      });

   commands.registerCommand("BLOCK_SIZE", [](const std::string& value) {
      std::cout << "BLOCK_SIZE executed with value: " << value << std::endl;
      });

   std::cout << "testing various match thresholds for BLOCKT\n";
   for (const double& d : { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 }) {
      std::cout << d << " ";
      const bool result = commands.executeCommand("BLOCKSt", d, "TRUE");
      if (!result) {
         std::cout << "failed\n";
      }
   }

   std::cout << "testing various match thresholds for SHOWONTROL\n";
   for (const double& d : { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 }) {
      std::cout << d << " ";
      const bool result = commands.executeCommand("SHOWONTROL", d, "TRUE");
      if (!result) {
         std::cout << "failed\n";
      }
   }


   std::cout << "\nTesting exact matches:\n";
   std::cout << "=====================\n";
   commands.executeCommand("SHOW_CONTROLS", "TRUE");
   commands.executeCommand("ECHO", "17");
   commands.executeCommand("WEB_INPUT", "blank");

   std::cout << "\nTesting fuzzy matches:\n";
   std::cout << "=====================\n";
   std::cout << "Testing 'show':\n";
   commands.executeCommand("show", "TRUE");
   std::cout << "\nTesting 'ShowControls':\n";
   commands.executeCommand("ShowControls", "TRUE");
   std::cout << "\nTesting 'SHCTRL':\n";
   commands.executeCommand("SHCTRL", .5, "TRUE");
   std::cout << "\nTesting 'eko':\n";
   commands.executeCommand("eko", .7,  "true");
   std::cout << "\nTesting 'web':\n";
   commands.executeCommand("web", "");

   std::cout << "\nTesting non-matches:\n";
   std::cout << "===================\n";
   bool result = commands.executeCommand("invalid_command", "");
   std::cout << "Invalid command returned: " << (result ? "true" : "false") << std::endl;

   return 0;
}

