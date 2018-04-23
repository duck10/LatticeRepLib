#ifndef LRL_READLATTICEDATA_H
#define LRL_READLATTICEDATA_H

#include "LRL_Cell.h"
#include "CellInputData.h"
#include "GenerateRandomLattice.h"

class LRL_ReadLatticeData : public CellInputData {
public:
   LRL_ReadLatticeData( const int seed = 0);
   LRL_ReadLatticeData read();
   void CellReader(const std::string& s);
   void CellReader(const std::string& lattice, const std::string& cell);
   GenerateRandomLattice<S6> generator;
   bool IsLatticeName(const std::string inputName, const std::vector<std::string>& nameList);
   bool IsLatticeName(const std::vector<std::string>& nameList, const std::string inputName);
};

#endif // LRL_READLATTICEDATA_H
