#ifndef CELL_FACTORY_H
#define CELL_FACTORY_H

#include "LRL_Cell.h"
#include "ControlVariables.h"
#include "S6.h"
#include <map>
#include <memory>
#include <string>

class CellFactory {
public:
   static std::map<std::string, std::unique_ptr<LRL_Cell>> createEnabledCells(const ControlVariables& controlVars, const S6& s6Point);
};

#endif // CELL_FACTORY_H