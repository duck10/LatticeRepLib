#ifndef CMDBGAOL_CONTROLS_H
#define CMDBGAOL_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"

#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

class BgaolControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const BgaolControls& ctrl) {
      os << "; CmdBGAOL Controls" << std::endl;
      os << ";   from the web " << ((ctrl.hasWebInput) ? "true" : "false") << std::endl;
      os << "; showDetails " << ((ctrl.showDetails) ? "true" : "false") << std::endl;
      os << "; blocksize = " << ctrl.blocksize << std::endl;
      os << "; blockstart = " << ctrl.blockstart << std::endl;
      os << "; sigmas = ";
      if (ctrl.sigmas.empty()) {
         os << "(none)";
      }
      else {
         for (size_t i = 0; i < ctrl.sigmas.size(); ++i) {
            os << ctrl.sigmas[i] << " ";
         }
      }
      os << std::endl;

      return os;
   }

   BgaolControls() {
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables& controls, const std::string& value) {
            showControls = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("DETAILS", .35,
         [this](BaseControlVariables&, const std::string& value) {
            showDetails = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("SIGMAS", .5,
         [this](BaseControlVariables&, const std::string& value) {
            setSigmas(value);
         }
      );
   }

   bool shouldShowDetails() const { return showDetails; }
   bool getHasWebInput() const { return hasWebInput; }
   void setWebRun(bool webInput) { hasWebInput = webInput; }

   // Decode the first 6 floating point values (separated by blanks and/or commas).
   // Text after the sixth value is ignored.
   // On any failure among the first 6 tokens -- a bad token, fewer than 6 values,
   // overflow, a non-finite value, or a negative value -- sigmas is left empty.
   void setSigmas(const std::string& value) {
      sigmas.clear();

      std::string text(value);
      for (size_t i = 0; i < text.size(); ++i) {
         if (text[i] == ',') text[i] = ' ';
      }

      std::istringstream iss(text);
      std::vector<double> parsed;
      std::string token;
      while (parsed.size() < 6 && (iss >> token)) {
         double d = 0.0;
         if (!parseDouble(token, d)) return;
         parsed.push_back(d);
      }
      // anything after the sixth value is ignored

      if (parsed.size() != 6) return;
      sigmas = parsed;
   }

   const std::vector<double>& getSigmas() const { return sigmas; }
   bool hasSigmas() const { return !sigmas.empty(); }

private:
   // The whole token must be a valid, finite number; "1.2x" is rejected.
   static bool parseDouble(const std::string& token, double& result) {
      const char* begin = token.c_str();
      char* end = nullptr;
      errno = 0;
      const double d = std::strtod(begin, &end);
      if (end == begin) return false;
      if (*end != '\0') return false;
      if (errno == ERANGE) return false;
      if (!std::isfinite(d)) return false;
      if (d < 0.0) return false;
      result = d;
      return true;
   }

   size_t blocksize = 0;
   size_t blockstart = 0;
   bool showDetails = false;
   bool hasWebInput = false;
   std::vector<double> sigmas;
};

#endif // CMDBGAOL_CONTROLS_H
