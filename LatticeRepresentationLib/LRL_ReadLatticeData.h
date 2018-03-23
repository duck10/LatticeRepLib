#ifndef LRL_READLATTICEDATA_H
#define LRL_READLATTICEDATA_H

#include "LRL_Cell.h"
#include "CellInputData.h"
#include "GenerateRandomLattice.h"

class LRL_ReadLatticeData : public CellInputData {
public:
   LRL_ReadLatticeData( const int seed /*= 0*/);
   LRL_ReadLatticeData read();
   void CellReader(const std::string& s);
   void CellReader(const std::string& lattice, const std::string& cell);
   GenerateRandomLattice<S6> generator;
};

#endif // LRL_READLATTICEDATA_H
