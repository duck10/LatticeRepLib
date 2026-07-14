#ifndef EISENSTEIN_CONTROLS_H
#define EISENSTEIN_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"
#include <cctype>
#include <string>
#include <iostream>

// =============================================================
// MATRIX SET SELECTION
// =============================================================

enum class MatrixSetChoice {
   Shears12,
   Eisenstein145,
   Full3480
};

inline MatrixSetChoice parseMatrixSet(const std::string& value) {
   std::string v;
   v.reserve(value.size());
   for (char c : value) v += std::toupper(c);

   if (v == "12" || v == "SHEARS" || v == "SHEARS12")
      return MatrixSetChoice::Shears12;

   if (v == "145" || v == "EISENSTEIN" || v == "EISENSTEIN145")
      return MatrixSetChoice::Eisenstein145;

   if (v == "3480" || v == "FULL" || v == "FULL3480")
      return MatrixSetChoice::Full3480;

   std::cerr << ";Warning: Invalid MATRIXSET '" << value
      << "', using default (145)\n";
   return MatrixSetChoice::Eisenstein145;
}

static inline const char* matrixSetToString(MatrixSetChoice choice) {
   switch (choice) {
   case MatrixSetChoice::Shears12:      return "12 (Shears)";
   case MatrixSetChoice::Eisenstein145: return "145 (Eisenstein neighborhood)";
   case MatrixSetChoice::Full3480:      return "3480 (Full unimodular)";
   }
   return "145 (Eisenstein neighborhood)";
}

// =============================================================
// MAXTRACES VALIDATION
// =============================================================

static const int DEFAULT_MAXTRACES = 30;
static const int MIN_MAXTRACES = 10;
static const int MAX_MAXTRACES = 200;

static int validateMaxTraces(const std::string& value) {
   try {
      int maxTraces = std::stoi(value);
      if (maxTraces < MIN_MAXTRACES || maxTraces > MAX_MAXTRACES) {
         std::cerr << ";Warning: MAXTRACES " << maxTraces << " out of range ["
            << MIN_MAXTRACES << "," << MAX_MAXTRACES << "], using "
            << DEFAULT_MAXTRACES << std::endl;
         return DEFAULT_MAXTRACES;
      }
      return maxTraces;
   }
   catch (...) {
      std::cerr << ";Warning: Invalid MAXTRACES value '" << value
         << "', using " << DEFAULT_MAXTRACES << std::endl;
      return DEFAULT_MAXTRACES;
   }
}

// =============================================================
// EISENSTEIN CONTROLS CLASS
// =============================================================

class EisenControls : public BaseControlVariables {
public:

   friend std::ostream& operator<<(std::ostream& os, const EisenControls& ctrl) {
      os << "; Eisenstein\n";
      os << "; Prefix: " << ctrl.getPrefix() << std::endl;
      os << "; Max traces to plot (MAXTRACES): " << ctrl.m_maxTraces << std::endl;
      os << "; Matrix set (MATRIXSET): " << matrixSetToString(ctrl.m_matrixSet) << std::endl;

      if (!ctrl.m_title.empty()) {
         os << "; Title: (TITLE) " << ctrl.m_title << std::endl;
      }
      return os;
   }

   EisenControls() {

      // TITLE
      InputHandler::registerHandler("TITLE", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            m_title = convertFromAllCaps(value);
         });

      // MAXTRACES
      InputHandler::registerHandler("MAXTRACES", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            m_maxTraces = validateMaxTraces(value);
         });

      // MATRIXSET
      InputHandler::registerHandler("MATRIXSET", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            m_matrixSet = parseMatrixSet(value);
         });
   }

   // =============================================================
   // Utility: Convert ALL CAPS to Title Case
   // =============================================================
   std::string convertFromAllCaps(const std::string& input) const {
      if (input.empty()) return input;

      std::string result;
      result.reserve(input.length());

      bool capitalize = true;
      for (char c : input) {
         if (std::isspace(c)) {
            result += c;
            capitalize = true;
         } else if (capitalize) {
            result += std::toupper(c);
            capitalize = false;
         } else {
            result += std::tolower(c);
         }
      }
      return result;
   }

   // =============================================================
   // Accessors
   // =============================================================
   bool shouldShowControls() const { return showControls; }
   std::string getPrefix() const { return prefix; }
   std::string getTitle() const { return m_title; }
   int getMaxTraces() const { return m_maxTraces; }

   MatrixSetChoice getMatrixSet() const { return m_matrixSet; }

   void setMaxTraces(int maxTraces) {
      if (maxTraces >= MIN_MAXTRACES && maxTraces <= MAX_MAXTRACES)
         m_maxTraces = maxTraces;
   }

   void setMatrixSet(MatrixSetChoice choice) {
      m_matrixSet = choice;
   }

private:
   const std::string prefix = "PRR";  // Prefix for Eisenstein reduction files
   std::string m_title;
   int m_maxTraces = DEFAULT_MAXTRACES;
   MatrixSetChoice m_matrixSet = MatrixSetChoice::Eisenstein145;  // Default
};

#endif // EISENSTEIN_CONTROLS_H
