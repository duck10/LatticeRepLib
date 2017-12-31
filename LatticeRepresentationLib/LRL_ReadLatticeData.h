#ifndef LRL_READLATTICEDATA_H
#define LRL_READLATTICEDATA_H

#include "LRL_Cell.h"
#include "CellInputData.h"

class LRL_ReadLatticeData : public CellInputData {
public:
   LRL_ReadLatticeData(void);
   void CellReader(const std::string& s);
   void CellReader(const std::string& lattice, const std::string& cell);
};

#endif // LRL_READLATTICEDATA_H
