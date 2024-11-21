#include "CellFactory.h"
#include "LRL_Cell.h"
#include "S6.h"
#include "G6.h"
#include "R9.h"

std::map<std::string, std::unique_ptr<LRL_Cell>> CellFactory::createEnabledCells(const ControlVariables& controlVars, const S6& s6Point) {
   std::map<std::string, std::unique_ptr<LRL_Cell>> cells;
   for (const auto& type : controlVars.enabledTypes) {
      if (type == "S6") {
         cells["S6"] = std::make_unique<S6>(s6Point);
      }
      else if (type == "G6") {
         cells["G6"] = std::make_unique<G6>(G6(s6Point));
      }
      else if (type == "R9") {
         cells["R9"] = std::make_unique<R9>(s6Point);
      }
      // Add other types as needed
   }
   return cells;
}