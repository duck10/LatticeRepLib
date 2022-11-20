


#include "LRL_Cell.h"
#include "G6.h"
#include "S6.h"
#include "B4.h"
#include "D7.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_RandTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "LRL_StringTools.h"


#include <algorithm>
#include <complex>
#include <cmath>
#include <sstream>
#include <string>
#include <utility>

void LRL_ReadLatticeData::CellReader(const std::string& lattice, const std::string& cell) {
   CellReader(lattice + " " + cell);
}

static int folseed = 19192;

//static const std::vector<size_t> vAvoid(avoidList, avoidList + sizeof(avoidList) / sizeof(avoidList[0]));
std::string G6_names[] = { "G ", "V ", "G6 " };
std::vector<std::string> vG6_names(G6_names, G6_names + sizeof(G6_names) / sizeof(G6_names[0]));

std::string D7_names[] = { "D ", "D7 " };
std::vector<std::string> vD7_names(D7_names, D7_names + sizeof(D7_names) / sizeof(D7_names[0]));

std::string S6_names[] = { "S ", "S6 " };
std::vector<std::string> vS6_names(S6_names, S6_names + sizeof(S6_names) / sizeof(S6_names[0]));

std::string C3_names[] = { "C3 " };
std::vector<std::string> vC3_names(C3_names, C3_names + sizeof(C3_names) / sizeof(C3_names[0]));

std::string lattice_names[] = { "A ", "B ", "C ", "P ", "R ", "F ", "I ", "H " };
std::vector<std::string> vlattice_names(lattice_names, lattice_names + sizeof(lattice_names) / sizeof(lattice_names[0]));

bool LRL_ReadLatticeData::IsLatticeName(const std::string inputName, const std::vector<std::string>& nameList) {
   return std::find(nameList.begin(), nameList.end(), inputName) != nameList.end();
}

bool LRL_ReadLatticeData::IsLatticeName(const std::vector<std::string>& nameList, const std::string inputName) {
   return std::find(nameList.begin(), nameList.end(), inputName) != nameList.end();
}

bool HasLatticeParams6(const std::string& s) {
   std::istringstream iss( s );
   std::vector<std::string> fields;
   std::string onefield;
   while (iss) {
      iss >> onefield;
      if (iss) fields.push_back( onefield );
   }
   return fields.size( ) == 6;
}

bool HasLatticeParams7( const std::string& s ) {
   std::istringstream iss( s );
   std::vector<std::string> fields;
   std::string onefield;
   while (iss) {
      iss >> onefield;
      if (iss) fields.push_back( onefield );
   }
   return fields.size( ) == 7;
}

std::pair<std::vector<double>, std::vector<double> > LRL_ReadLatticeData::SplitFields( const int n, const std::vector<double>& fields) {
   // if there are more than 6 fields, try to assume that the rest are standard deviations
   std::vector<double> v1, v2;
   if (n <= fields.size( ))
      v1 = std::vector<double>( fields.begin( ), fields.begin( ) + n );
   if ( 2*n <= fields.size( ))
      v2 = std::vector<double>( fields.begin( ) + n, fields.begin( ) + 2*n );
   return std::make_pair( v1, v2 );
}

bool LRL_ReadLatticeData::SetRandomCell( const std::string& inputDataType) {
   if (inputDataType != "RANDOM ") return false;

   m_inputDataType = "P";
   m_lattice = "P";
   const G6 g6Cell = generator.SetLengthLimits( 10.0, 100.0 ).GenerateExtreme( );
   m_cell = LRL_Cell( g6Cell );
   m_cell.SetSigmas(LRL_Cell( 0.1 * m_cell[0], 0.1 * m_cell[1], 0.1 * m_cell[2], 0.1 * m_cell[3], 0.1 * m_cell[4], 0.1 * m_cell[5] ));
   m_strCell = std::string("RANDOM (P ") + LRL_ToString( m_cell ) + std::string(")");
   return true;
}

bool LRL_ReadLatticeData::SetD7Data( const std::string& inputDataType, const std::vector<double>& fields ) {
   const bool test = IsLatticeName( m_inputDataType, vD7_names );
   if (test) {
      m_cell = LRL_Cell( D7( fields ) );
      m_lattice = "P";
   }
   return test;
}

bool LRL_ReadLatticeData::SetG6Data( const std::string& inputDataType, const std::vector<double>& fields ) {
   const bool test = IsLatticeName( m_inputDataType, vG6_names );
   if (test && fields.size() >= 6) {
      m_cell = LRL_Cell( G6( fields ) );
      m_lattice = "P";
   }
   return test;
}

bool LRL_ReadLatticeData::SetC3Data( const std::string& inputDataType, const std::vector<double>& fields ) {
   const bool test = IsLatticeName( m_inputDataType, vC3_names );
   if (test) {
      m_cell = LRL_Cell( C3( fields ) );
      m_lattice = "P";
   }
   return test;
}

bool LRL_ReadLatticeData::SetS6Data( const std::string& inputDataType, const std::vector<double>& fields ) {
   const bool test = IsLatticeName( m_inputDataType, vS6_names );
   if (test) {
      m_cell = LRL_Cell( S6( fields ) );
      m_lattice = "P";
   }
   return test;
}

bool LRL_ReadLatticeData::SetUnitCellTypeData( const std::string& inputDataType, const std::pair<std::vector<double>, std::vector<double> >& params ) {
   const bool test = IsLatticeName( m_inputDataType, vlattice_names );
   if (test && params.first.size()>=6) {
      m_cell = LRL_Cell( params.first[0], params.first[1], params.first[2], params.first[3], params.first[4], params.first[5] );
      m_cell.SetValid( G6( m_cell ).IsValid( ) );
      if (params.second.size( ) >= 6)
         m_cell.SetSigmas( params.second );
      else {
         std::vector<double> s( 6 );
         for (size_t i = 0; i < 6; ++i) s[i] = 0.01 * m_cell[i];
         m_cell.SetSigmas( s );
      }
   }
   return test;
}

std::vector<double> LRL_ReadLatticeData::GetFieldsForCellFromString(const std::string& s) {
   std::vector<double> toReturn;
   std::istringstream iss(s);
   m_inputDataType.clear();
   if ((LRL_StringTools::strToupper(s.substr(0, 3)) == std::string("END"))) {
      m_lattice = "EOF";
   }
   else {
      iss >> m_inputDataType;
      m_inputDataType = LRL_StringTools::strToupper(m_inputDataType);
      m_lattice = m_inputDataType[0];
      double onefield;
      while (iss) {
         iss >> onefield;
         if (iss) toReturn.push_back(onefield);
      }
      if (toReturn.size() < 6 && m_inputDataType != "RANDOM") {
         toReturn.clear();
         if ( m_lattice != ";" && (!s.empty()) && s != " ")
            std::cout << "input line rejected, invalid cell A: " << s << std::endl;
         m_lattice = "";
         m_cell.SetValid(false);
         return std::vector<double>();
      }
   }
   return toReturn;
}

bool LRL_ReadLatticeData::StringToCell(const std::vector<double>& fields) {
   bool valid = false;
   {
      const std::pair<std::vector<double>, std::vector<double> > params = SplitFields(6, fields);
      m_inputDataType = LRL_StringTools::strToupper(m_inputDataType + " ");
      valid =
         SetRandomCell(m_inputDataType) ||
         SetG6Data(m_inputDataType, fields) ||
         SetD7Data(m_inputDataType, fields) ||
         SetS6Data(m_inputDataType, fields) ||
         SetC3Data(m_inputDataType, fields) ||
         SetUnitCellTypeData(m_inputDataType, params);

      if (valid && params.second.size() >= 6) {// ASSUMING ALL SIGMAS FOR ALL TYPES ARE CELL SIGMAS, NOT OTHER TYPES
         m_cell.SetSigmas(params.second);
         //return false;
      }
   }
   return valid;
}

void LRL_ReadLatticeData::CellReader(const std::string& s) {

   const std::string strupper = LRL_StringTools::strToupper(s.substr(0, 5));
   std::vector<double> fields = GetFieldsForCellFromString(s);
   if (s[0] == ';') m_incomingSemicolons += "\n" +m_strCell;
   if (s.length() == 0 || s[0] == ';') return;
   if (fields.size() < 6 && strupper !="RANDO") {
      std::cout << "input line rejected, insufficient data  " << s << std::endl;
      return;
   }

   bool valid = StringToCell(fields);

   if (s[0] == ';') m_incomingSemicolons += "\n"+s;
   if (!valid || !m_cell.GetValid()) {
      if (s[0] != ';' && (!s.empty()) && s != " ")
         std::cout << "input line rejected, invalid cell B: " << s << std::endl;
      m_lattice = "";
      m_cell.SetValid(false);
   }
}

std::vector<CellInputData> LRL_ReadLatticeData::ReadAllLatticeData(const int seed) {
   std::vector<CellInputData> celldata;
   std::string lattice("");
   LRL_ReadLatticeData rcd(seed);
   while (lattice != "EOF") {
      rcd.read();
      lattice = "EOF";
      lattice = rcd.GetLattice();
      if (lattice != "EOF" && !lattice.empty()) {
         rcd.SetVarietyRange(std::make_pair(0, 23));
         celldata.push_back(rcd);
      }
   }
   return celldata;
}

std::vector<CellInputData> LRL_ReadLatticeData::ReadAllLatticeDataAndMakePrimitive(const int seed){
   std::vector<CellInputData> cellData = ReadAllLatticeData(seed);
   for ( size_t i=0; i<cellData.size(); ++i ) {
      if ( cellData[i].GetLattice() != "P") {
         LRL_Cell cell = LatticeConverter::MakePrimitiveCell(cellData[i].GetLattice(), cellData[i].GetCell());
         cellData[i].SetCell(cell);
         cellData[i].SetLattice("P");
      }
   }
   return cellData;
}

LRL_ReadLatticeData::LRL_ReadLatticeData(const int seed )
: generator(seed) {
   S6().SetSeed(seed);
}

LRL_ReadLatticeData LRL_ReadLatticeData::CreateLatticeData(const std::string& s) {
   if (std::cin && (LRL_StringTools::strToupper(m_strCell.substr(0, 3)) != std::string("END")))
      CellReader(m_strCell);
   else {
      m_lattice = "EOF";
      m_cell.SetValid(false);
   }
   return *this;
}

LRL_ReadLatticeData LRL_ReadLatticeData::read(void) {
   m_strCell.clear();
   m_cell.SetValid(false);
   std::getline(std::cin, m_strCell);
   if (m_strCell[0] == ';') m_incomingSemicolons  += "\n"+m_strCell;
   if (m_strCell.length() > 0 && m_strCell[0] == ';') return *this;
   if (std::cin && (LRL_StringTools::strToupper(m_strCell.substr(0, 3)) != std::string("END"))) {
      CellReader(m_strCell);
   }
   else {
      m_lattice = "EOF";
      m_cell.SetValid(false);
   }
   return *this;
}

std::vector<LRL_ReadLatticeData> LRL_ReadLatticeData::ReadLatticeData() {
   std::vector<LRL_ReadLatticeData> inputList;
   LRL_ReadLatticeData readData;
   LRL_ReadLatticeData  input;
   while (input.GetLattice() != "EOF") {
      input = read();
      if (input.IsValid()) inputList.push_back(input);
//      std::cout << "number of cells read " << inputList.size() << std::endl;
      const size_t n = inputList.size();
   }
   return inputList;
}