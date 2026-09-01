// BaseControlVariables.h
#ifndef BASE_CONTROL_VARIABLES_H
#define BASE_CONTROL_VARIABLES_H

#include "ControlFeature.h"
#include "InputHandler.h"
#include "S6.h"

#include <vector>
#include <memory>
#include <ostream>
#include <sstream>

class BaseControlVariables {
protected:
   std::vector<std::unique_ptr<ControlFeature>> features;
   bool webRun = false;
   bool showControls = false;
   bool echo = false;
   // Mirrors S6.cpp's own hardcoded s6RandomSeedS6 default so this
   // displays honestly when SEED/RANDOMSEED is never set.
   int randomSeed = 19195;

   //BaseControlVariables(const BaseControlVariables&) = delete;
   //BaseControlVariables& operator=(const BaseControlVariables&) = delete;

public:
   virtual ~BaseControlVariables() = default;

   BaseControlVariables() {
      InputHandler::registerHandler("ECHO", .5,
         [this](BaseControlVariables& controls, const std::string& value) {
            echo = (value == "1" || value == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables& controls, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("SHOWCONTROLS", .5,
         [this](BaseControlVariables& controls, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("WEB_INPUT", .5,
         [this](BaseControlVariables& controls, const std::string& value) {
            webRun = (value == "1" || value == "TRUE" || value.empty());
         }
      );

      // SEED / RANDOMSEED -- overrides S6::rand()'s random seed (default
      // hardcoded in S6.cpp as s6RandomSeedS6=19195, years old and never
      // touched since -- Larry, 2026-08-29). Registered here, not in any
      // one program's own Controls class, so every program built on this
      // framework gets it automatically -- Larry's point: the only way
      // this can be genuinely universal (not just "applied once instead
      // of at several call sites in one program") is for it to take
      // effect eagerly, inside the base class, as the line is parsed --
      // not deferred to some later single call site in a specific
      // main(), which only ever reaches that one program and only
      // whatever runs after that call.
      //
      // Two names for the same handler, both kept (not one renamed to
      // the other) -- SEED was briefly renamed to RANDOMSEED out of a
      // (subsequently disproven, via CmdSellaControls's own SHOW output)
      // concern that it collided with SEEDBOUNDARIES; both names are
      // now registered so nothing that already used SEED breaks.
      const auto seedHandler = [this](BaseControlVariables&, const std::string& value) {
         randomSeed = std::stoi(value);
         S6::SetSeed(randomSeed);
         };
      InputHandler::registerHandler("SEED", .5, seedHandler);
      InputHandler::registerHandler("RANDOMSEED", .5, seedHandler);
   }

   template<typename T>
   T* getFeature() {
      for (auto& feature : features) {
         if (auto* p = dynamic_cast<T*>(feature.get())) {
            return p;
         }
      }
      return nullptr;
   }

   virtual bool handleInput(const std::string& command, const std::string& value) {
      for (auto& feature : features) {
         if (feature->handleInput(command, value)) {
            return true;
         }
      }
      return false;
   }

   virtual std::string getState() const {
      std::ostringstream oss;
      for (const auto& feature : features) {
         oss << feature->getFeatureState();
      }
      return oss.str();
   }

   bool getWebRun() const { return webRun; }
   void setWebRun(const bool webrun) { webRun = webrun; }
   void setHasWebInput(const bool b) { webRun = b; }
   bool getHasWebInput() const { return webRun; }
   bool getShowControls() const { return showControls; }
   bool getShouldControls() const { return showControls; }
   void setShowContols(const bool b) { showControls = b; }
   bool getEcho() const { return echo; }
   void setEcho(const bool b) { echo = b; }
   bool shouldShowControls() const { return showControls; }
   int getRandomSeed() const { return randomSeed; }

   friend std::ostream& operator<<(std::ostream& os, const BaseControlVariables& cv);
};

inline std::ostream& operator<<(std::ostream& os, const BaseControlVariables& cv) {
   os << ";Control Variables:\n";
   for (const auto& feature : cv.features) {
      feature->writeToStream(os);
   }
   return os;
}

#endif // BASE_CONTROL_VARIABLES_H
