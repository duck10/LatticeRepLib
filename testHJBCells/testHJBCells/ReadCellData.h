#ifndef READCELLDATA_H
#define READCELLDATA_H
#ifdef _MSC_VER
#pragma once
#endif

#include "Cell.h"
#include "CellInputData.h"

class ReadCellData : public CellInputData {
public:
   ReadCellData(void);
   void CellReader(const std::string& s);
   void CellReader(const std::string& lattice, const std::string& cell);
};

#endif
