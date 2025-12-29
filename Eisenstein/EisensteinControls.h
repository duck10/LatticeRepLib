#ifndef EISENSTEIN_CONTROLS_H
#define EISENSTEIN_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

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

class EisenControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const EisenControls& ctrl) {
      os << "; Eisenstein\n";
      os << "; Prefix: " << ctrl.getPrefix() << std::endl;
      os << "; Max traces to plot (MAXTRACES): " << ctrl.m_maxTraces << std::endl;
      if (!ctrl.m_title.empty()) {
         os << "; Title: (TITLE) " << ctrl.m_title << std::endl;
      }
      return os;
   }

   EisenControls() {
      InputHandler::registerHandler("TITLE", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            m_title = convertFromAllCaps(value);
         });

      InputHandler::registerHandler("MAXTRACES", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            m_maxTraces = validateMaxTraces(value);
         });
   }

   // Utility function to convert ALL CAPS to Title Case
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

   bool shouldShowControls() const { return showControls; }
   std::string getPrefix() const { return prefix; }
   std::string getTitle() const { return m_title; }
   int getMaxTraces() const { return m_maxTraces; }
   void setMaxTraces(int maxTraces) {
      if (maxTraces >= MIN_MAXTRACES && maxTraces <= MAX_MAXTRACES) {
         m_maxTraces = maxTraces;
      }
   }

private:
   const std::string prefix = "PRR";  // Prefix for Eisenstein reduction files
   std::string m_title;
   int m_maxTraces = DEFAULT_MAXTRACES;  // Default 30
};

#endif // EISENSTEIN_CONTROLS_H