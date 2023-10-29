#ifndef LRL_READLATTICEDATA_H
#define LRL_READLATTICEDATA_H

#include "LRL_Cell.h"
#include "CellInputData.h"
#include "GenerateRandomLattice.h"

class LRL_ReadLatticeData : public CellInputData {
public:
   LRL_ReadLatticeData(const int seed = 0);
   void clear();
   LRL_ReadLatticeData read();
   LRL_ReadLatticeData CreateLatticeData(const std::string& s);
   std::vector<LRL_ReadLatticeData> ReadLatticeData();
   void CellReaderA(const std::string& s);
   std::string CellReaderB(std::string& strcell) const;
   void CellReader(const std::string& s);
   void CellReader(const std::string& lattice, const std::string& cell);
   std::vector<double> GetFieldsForCellFromString(const std::string& s);
   bool StringToCell(const std::vector<double>& fields);

   static std::vector<CellInputData> ReadAllLatticeData(const int seed);
   static std::vector<CellInputData> ReadAllLatticeDataAndMakePrimitive(const int seed);
  std::string
      GetIncomingSemicolons (void) const { return m_incomingSemicolons; }

  void SetOrdinal(const size_t n) { m_ordinal = n; }
  size_t GetOrdinal() const { return m_ordinal; }
protected:
   GenerateRandomLattice<S6> generator;
   static std::pair<std::vector<double>, std::vector<double> > SplitFields(const int n, const std::vector<double>& fields);

   bool SetRandomCell( const std::string& inputDataType );
   bool SetG6Data( const std::vector<double>& fields );
   bool SetD7Data( const std::vector<double>& fields );
   bool SetS6Data(  const std::vector<double>& fields );
   bool SetC3Data( const std::vector<double>& fields);
   bool SetB4Data( const std::vector<double>& fields);
   bool SetUData( const std::vector<double>& fields);
   bool SetUnitCellTypeData( const std::pair<std::vector<double>, std::vector<double> >& params );
   static bool IsLatticeName(const std::string inputName, const std::vector<std::string>& nameList);
   static bool IsLatticeName(const std::vector<std::string>& nameList, const std::string inputName);

   std::string m_incomingSemicolons;
   size_t m_ordinal;
};

#endif // LRL_READLATTICEDATA_H
