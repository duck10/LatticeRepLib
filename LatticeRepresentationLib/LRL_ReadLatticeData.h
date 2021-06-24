#ifndef LRL_READLATTICEDATA_H
#define LRL_READLATTICEDATA_H

#include "LRL_Cell.h"
#include "CellInputData.h"
#include "GenerateRandomLattice.h"

class LRL_ReadLatticeData : public CellInputData {
public:
   LRL_ReadLatticeData( const int seed = 0);
   LRL_ReadLatticeData read();
   LRL_ReadLatticeData CreateLatticeData(const std::string& s);
   std::vector<LRL_ReadLatticeData> ReadLatticeData();
   void CellReader( const std::string& s );
   void CellReader(const std::string& lattice, const std::string& cell);
   std::vector<double> GetFieldsForCellFromString(const std::string& s);

   static std::vector<CellInputData> ReadAllLatticeData(const int seed);
   static std::vector<CellInputData> ReadAllLatticeDataAndMakePrimitive(const int seed);

protected:
   GenerateRandomLattice<S6> generator;
   std::pair<std::vector<double>, std::vector<double> > SplitFields(const int n, const std::vector<double>& fields);

   bool SetRandomCell( const std::string& inputDataType );
   bool SetG6Data( const std::string& inputDataType, std::vector<double>& fields );
   bool SetD7Data( const std::string& inputDataType, std::vector<double>& fields );
   bool SetS6Data( const std::string& inputDataType, std::vector<double>& fields );
   bool SetC3Data( const std::string& inputDataType, std::vector<double>& fields );
   bool SetUnitCellTypeData( const std::string& inputDataType, const std::pair<std::vector<double>, std::vector<double> >& params );
   bool IsLatticeName(const std::string inputName, const std::vector<std::string>& nameList);
   bool IsLatticeName(const std::vector<std::string>& nameList, const std::string inputName);

};

#endif // LRL_READLATTICEDATA_H
