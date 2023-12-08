#include "LRL_Cell.h"
#include "G6.h"
#include "S6.h"
#include "B4.h"
#include "D7.h"
#include "DC7u.h"
#include "D13.h"
#include "V7.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_RandTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "LRL_StringTools.h"
#include "LRL_Vector3.h"


#include <algorithm>
#include <complex>
#include <cmath>
#include <regex>
#include <sstream>
#include <string>
#include <utility>

void LRL_ReadLatticeData::CellReader(const std::string& lattice, const std::string& cell) {
   CellReader(lattice + " " + cell);
}

static const int folseed = 19192;

const std::string G6_names[] = { "G ", "G6 " };
const std::vector<std::string> vG6_names(G6_names, G6_names + sizeof(G6_names) / sizeof(G6_names[0]));

//const std::string D7_names[] = { "D ", "D7 " }; // removed 2023-11-22
const std::string D7_names[] = { " " };
const std::vector<std::string> vD7_names(D7_names, D7_names + sizeof(D7_names) / sizeof(D7_names[0]));

//const std::string D7_names[] = { "D ", "D7 " }; // removed 2023-11-22
const std::string V7_names[] = { " V V7 " };
const std::vector<std::string> v7_names(V7_names, V7_names + sizeof(V7_names) / sizeof(V7_names[0]));

const std::string S6_names[] = { "S ", "S6 " };
const std::vector<std::string> vS6_names(S6_names, S6_names + sizeof(S6_names) / sizeof(S6_names[0]));

const std::string C3_names[] = { "C3 " };
const std::vector<std::string> vC3_names(C3_names, C3_names + sizeof(C3_names) / sizeof(C3_names[0]));

const std::string B4_names[] = { "B ", "B4 " };
const std::vector<std::string> vB4_names(B4_names, B4_names + sizeof(B4_names) / sizeof(B4_names[0]));

const std::string U_names[] = { "U ", "DC7U ", "DU " };
const std::vector<std::string> vU_names(U_names, U_names + sizeof(U_names) / sizeof(U_names[0]));

const std::string SL_names[] = { "SL ", "RI " };
const std::vector<std::string> vSL_names(SL_names, SL_names + sizeof(SL_names) / sizeof(SL_names[0]));



static const std::string symbols6(" A B C F G H I P R S G6 S6 "); // ALL BLANKS ARE NOT OPTIONAL
static const std::string symbols7(" V V7  ");
static const std::string symbols13(" /*DC13*/ ");
static const std::string symbolsSL(" RI SL ");
static const std::string allowedLatticeSymbols = symbols6 + symbols7 + symbols13 + symbolsSL;


size_t SizeForLatticeType(const std::string& lattice) {
   if (symbols6.find(" " + lattice + " ") != std::string::npos) return 6;
   if (symbols7.find(" " + lattice + " ") != std::string::npos) return 7;
   if (symbols13.find(" " + lattice + " ") != std::string::npos) return 13;
   if (symbolsSL.find(" " + lattice + " ") != std::string::npos) return 6;
   throw " in SizeForLatticeType, got unrecognized lattice " + lattice;
}


const std::string lattice_names[] = { "A ", "B ", "C ", "P ", "R ", "F ", "I ", "H ",
   "a ", "b ", "c ", "p ", "r ", "f ", "h ", "h " };
const std::vector<std::string> vlattice_names(lattice_names, lattice_names + sizeof(lattice_names) / sizeof(lattice_names[0]));

bool LRL_ReadLatticeData::IsLatticeName(const std::string& inputName, const std::vector<std::string>& nameList) {
   std::string upperName(LRL_StringTools::strToupper(inputName));
   if (upperName.back() != ' ') upperName += " ";
   const bool b =  std::find(nameList.begin(), nameList.end(),
     upperName) != nameList.end();
   return b;
}

bool LRL_ReadLatticeData::IsLatticeName(const std::vector<std::string>& nameList, const std::string& inputName) {
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
   // if there are more than n fields, try to assume that the rest are standard deviations
   std::vector<double> v1;
   std::vector<double> v2;
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
   m_strCell = std::string("RANDOM (P ") + LRL_ToString( LRL_Cell_Degrees(m_cell )) + std::string(")");
   return true;
}

bool LRL_ReadLatticeData::SetD7Data( const std::vector<double>& fields ) {
   const bool test = IsLatticeName( m_inputDataType, vD7_names );
   if (test) {
      m_cell = LRL_Cell( D7( fields ) );
      m_lattice = "P";
   }
   return test;
}

bool LRL_ReadLatticeData::SetG6Data( const std::vector<double>& fields ) {
   const bool test = IsLatticeName( m_inputDataType, vG6_names );
   //const size_t nParams = SizeForLatticeType(m_inputDataType);
   if (test && fields.size() >= 6) {
      m_cell = LRL_Cell( G6( fields ) );
      m_lattice = "P";
   }
   return test;
}

bool LRL_ReadLatticeData::SetC3Data(const std::vector<double>& fields) {
   const bool test = IsLatticeName(m_inputDataType, vC3_names);
   if (test) {
      m_cell = LRL_Cell(C3(fields));
      m_lattice = "P";
   }
   return test;
}

bool LRL_ReadLatticeData::SetSLData(const std::vector<double>& fields) {
   const bool test = IsLatticeName(m_inputDataType, vSL_names);
   if (test) {
      const S6 s6(-fields[0] * fields[0], -fields[1] * fields[1], -fields[2] * fields[2], -fields[3] * fields[3],
         -fields[4] * fields[4], -fields[5] * fields[5]);
      m_cell = s6;
      m_lattice = "P";
   }
   return test;
}

bool LRL_ReadLatticeData::SetB4Data(const std::vector<double>& fields) {
   if (fields.size() < 12) return false;
   const bool test = IsLatticeName(m_inputDataType, vB4_names);
   const Vector_3 v1(Vector_3(fields[0], fields[1], fields[2]));
   const Vector_3 v2(Vector_3(fields[3], fields[4], fields[5]));
   const Vector_3 v3(Vector_3(fields[6], fields[7], fields[8]));
   const Vector_3 v4(Vector_3(fields[9], fields[10], fields[11]));
   const double s1 = v2.Dot(v3);
   const double s2 = v1.Dot(v3);
   const double s3 = v1.Dot(v2);
   const double s4 = v4.Dot(v1);
   const double s5 = v4.Dot(v2);
   const double s6 = v4.Dot(v3);
   if (test) {
      m_cell = LRL_Cell(S6(s1,s2,s3,s4,s5,s6));
      m_lattice = "P";
   }
   return test;
}

bool LRL_ReadLatticeData::SetUData(const std::vector<double>& fields) {
   bool test = IsLatticeName(m_inputDataType, vU_names);
   if (fields.size() < 7) test = false;
   if (test) {
      DC7u dc7u = DC7u(fields);
      m_cell = LRL_Cell(G6(dc7u));
      m_lattice = "P";
   }
   return test;
}

bool LRL_ReadLatticeData::SetV7Data(const std::vector<double>& fields) {
   std::vector<std::string> names{ "V7", " V7 ", " V7", "V7 ", "V", " V ", " V", "V "};
   bool test = IsLatticeName(m_inputDataType, names);
   if (!test) return false;
   if (fields.size() < 7) return false;

   const V7 v7(fields);
   m_cell = V7::ConvertV7ToG6(v7);;
   test = m_cell.GetValid();

   return test;
}

bool LRL_ReadLatticeData::SetS6Data(const std::vector<double>& fields) {
   const bool test = IsLatticeName(m_inputDataType, vS6_names);
   if (test) {
      m_cell = LRL_Cell(S6(fields));
      m_lattice = "P";
   }
   return test;
}

bool LRL_ReadLatticeData::SetUnitCellTypeData( const std::pair<std::vector<double>, std::vector<double> >& params ) {
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
      return toReturn;
   }
   else {
      iss >> m_inputDataType;
      m_inputDataType = LRL_StringTools::strToupper(m_inputDataType);
      m_lattice = m_inputDataType;
      double onefield;
      if (m_inputDataType == "RANDOM") {
         toReturn.emplace_back(m_inputDataType != "RANDOM");
         return toReturn;
      }
      while (iss) {
         iss >> onefield;
         if (iss) toReturn.push_back(onefield);
      }
      if (toReturn.size() < 6 && m_inputDataType != "RANDOM") {  //LCA fix for type DC7u
         toReturn.clear();
         //if ( m_lattice != ";" && (!s.empty()) && s != " ")
         //   std::cout << ";input line rejected, invalid cell (A): " << s << std::endl;
         m_lattice = "";
         m_cell.SetValid(false);
         std::cout << " in GetFieldsForCellFromString " << LRL_ToString(toReturn) << std::endl;
         return std::vector<double>();
      }
   }
   return toReturn;
}

bool LRL_ReadLatticeData::StringToCell(const std::vector<double>& fields) {
   bool valid = false;
   const std::pair<std::vector<double>, std::vector<double> > params = SplitFields(6, fields);
   m_inputDataType = LRL_StringTools::strToupper(m_inputDataType + " ");
   valid =
      SetRandomCell(m_inputDataType) ||
      SetV7Data(fields) ||
      SetG6Data(fields) ||
      //SetD7Data(fields) ||
      SetS6Data(fields) ||
      SetC3Data(fields) ||
      SetB4Data(fields) ||
      SetUData(fields) ||
      SetSLData(fields) ||
      SetUnitCellTypeData(params);

   if (valid && params.second.size() >= 6) {// ASSUMING ALL SIGMAS FOR ALL TYPES ARE CELL SIGMAS, NOT OTHER TYPES
         m_cell.SetSigmas(params.second);
         //return false;
      }
   return valid;
}

void LRL_ReadLatticeData::CellReader(const std::string& s) {

   //const std::string strupper = LRL_StringTools::strToupper(s.substr(0, 5));
   std::vector<double> fields = GetFieldsForCellFromString(s);
   if (s[0] == ';') m_incomingSemicolons += "\n" +m_strCell;
   if (fields.empty() && s.length() != 0 && s[0] != ';') {
      std::cout << "; input line rejected(F)" << std::endl;
      return;
   }

   bool valid = StringToCell(fields);
   if (!valid || !m_cell.GetValid()) {
      if (s[0] != ';' && (!s.empty()) && s != " ")
         std::cout << ";input line rejected (D), invalid cell (B): " << s << std::endl;
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

std::vector<CellInputData> LRL_ReadLatticeData::ReadAllLatticeDataAndMakePrimitive(const int seed) {
   std::vector<CellInputData> cellData = ReadAllLatticeData(seed);
   for (size_t i = 0; i < cellData.size(); ++i) {
      if (cellData[i].GetLattice() != "P") {
         LRL_Cell cell = LatticeConverter::MakePrimitiveCell(cellData[i].GetLattice(), cellData[i].GetCell());
         cellData[i].SetCell(cell);
         cellData[i].SetLattice("P");
      }
   }
   return cellData;
}

LRL_ReadLatticeData::LRL_ReadLatticeData(const int seed)
   : generator(seed) {
   S6::SetSeed(seed);
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

std::string replaceTabsAndCommas(std::string str) {
   const  std::regex reg("[^a-zA-Z0-9.; +-]");
   return std::regex_replace(str, reg, " ");
}

void Process(const std::string& lattice, const std::vector<std::string>& params) {
   std::cout << "in Process" << std::endl;
   std::cout << "lattice symbols " << lattice << std::endl;
   std::cout << " parameters " << std::endl;
   std::cout << LRL_ToString(params) << std::endl << std::endl;
}

bool isNumber(const std::string& s) {
   const std::regex reg("^-?(?:[0-9]+|[0-9]*\\.[0-9]+)([eE][-+]?[0-9]+)?$");
   return  std::regex_match(s, reg);
}

bool is_valid_float(const std::string str) {
   const std::regex regex("^([-+]?((([0-9]+)|[0-9]*\\.([0-9]+)?)([e?E][-+]?[0-9]+)?)|([0-9]*\\.[0-9]+))");
   return std::regex_match(str, regex);
}


bool CheckParameters( const std::string& lattice,  const std::vector<std::string>& fields)
{
   const size_t n = SizeForLatticeType(lattice);
   for (size_t i = 1; i < n; ++i ) { 
      if (!is_valid_float(fields[i])) {
         return false;
      }
   }
   return true;
}

std::vector<std::string> RearrangeFields(const std::vector<std::string>& fields, const size_t n) {
   std::vector<std::string> out;
   out.emplace_back(fields[n]);
   for (size_t i = 0; i < fields.size(); ++i) {
      if (i != n) {
         out.emplace_back(fields[i]);
      }
   }
   return out;
}

std::vector<std::string> IsLattice(const std::vector<std::string>& fields) {

   std::vector<std::string> out;

   if (allowedLatticeSymbols.find(" "+fields[0] + " ") != std::string::npos) {
      return fields;
   }
   else if (fields.size() > 6 && symbols6.find(" " + fields[6] + " ") != std::string::npos) {
      return RearrangeFields(fields, 6);
   }
   else if (fields.size() > 7 && symbols7.find(" " + fields[7] + " ") != std::string::npos) {
      return RearrangeFields(fields, 7);
   }
   else if (fields.size() > 13 && symbols13.find(" " + fields[13] + " ") != std::string::npos) {
      return RearrangeFields(fields, 13);
   }
   else if (fields.size() > 6 && symbolsSL.find(" " + fields[6] + " ") != std::string::npos) {
      return RearrangeFields(fields, 6);
   }

   return out;
}

std::vector<std::string> CleanInput(std::string& strcell) {
   const std::string cleaned = LRL_StringTools::strToupper(replaceTabsAndCommas(strcell));
   return LRL_StringTools::SplitBetweenBlanks(cleaned);
}

bool FindLatticeSymbol(const std::string& lattice, const std::vector<std::string>& fields) {
   const size_t maxIndex = SizeForLatticeType(lattice);;

   for (size_t i = 0; i < std::min(fields.size(),maxIndex); ++i) {
      if (allowedLatticeSymbols.find(" " + fields[i] + " ") != std::string::npos) {
         return true;
      }
   }
   return false;
}

std::string LRL_ReadLatticeData::CellReaderB(std::string& strcell) const {
   //fromBard();
   if (strcell.empty() || strcell[0] == ';') return ""; // ignore lines starting with semicolon or are empty
   if (strcell.find_first_not_of(" ") == std::string::npos) return ""; // ignore lines that are only blanks

   std::vector<std::string> fields = CleanInput(strcell);
   if (fields[0] == "RANDOM") return fields[0];
   if (fields[0] == "V7")
   {
      std::cout << "; V7 input is not implemented (B)" << std::endl;
      return "";;
   }

   if (fields.size() == 1 && fields[0].empty()) {
      return "";
   }

   if (fields.size() < 7) {
      std::cout << "; not enough data (B)" << std::endl;
      return "";
   }

   std::vector<std::string> newFields = IsLattice(fields);
   const std::string newLattice = (newFields.empty()) ? " " : newFields[0];

   if (newFields.empty()) {
      std::cout << "; lattice type not recognized" << std::endl;
   }
   else if (LRL_StringTools::strToupper(newFields[0]).find("V7") != std::string::npos
      && newFields.size() < 8) {
      std::cout << "; not enough data (A)" << std::endl;
      return "";
   }
   else if ( newFields.size() < 7) {
      std::cout << "; not enough data (A)" << std::endl;
      return "";
   }

   if (allowedLatticeSymbols.find(newFields[0]) == std::string::npos) {  //LCA fix for type DC7u
      std::cout << "; invalid input data" << std::endl;
      return "";
   }

   std::string lattice = newFields[0];
   std::vector<std::string>::iterator it;
   std::vector<std::string> parameters;
   if (allowedLatticeSymbols.find(lattice) != std::string::npos) {
      const bool b = CheckParameters(lattice, newFields);
      if (!b) {
         std::cout << "; invalid cell parameter (A)" << std::endl;
         return "";
      }
      for (size_t i = 1; i < newFields.size(); ++i) {  //LCA fix for type DC7u
         parameters.emplace_back(newFields[i]);
      }
      return  lattice + " " + LRL_ToString(parameters);

   }
   else if (parameters.empty() && allowedLatticeSymbols.find(newFields[6]) != std::string::npos) {  //LCA fix for type DC7u
      lattice = newFields[6];  //LCA fix for type DC7u
      it = newFields.begin();
      const bool b = CheckParameters(lattice, newFields);
      if (!b) {
         std::cout << "; invalid cell parameter (B)" << std::endl;
         return "";
      }
      for (size_t i = 0; i < 6; ++i) {  //LCA fix for type DC7u
         parameters.emplace_back(newFields[i]);
      }
   }
   else {
      std::cout << "; no valid lattice type" << std::endl;
      return "";
   }

   // Process(lattice, parameters);

   return lattice + " " + LRL_ToString(parameters);

}

LRL_ReadLatticeData LRL_ReadLatticeData::read(void) {
   m_strCell.clear();
   m_cell.SetValid(false);
   std::getline(std::cin, m_strCell);
   
   const std::string ucase = LRL_StringTools::strToupper(m_strCell.substr(0, 10));

   if (!std::cin || ucase.find("END") != std::string::npos) {
      m_lattice = "EOF";
   }
   else if (ucase.find("RANDOM") != std::string::npos) {
      bool b = LRL_ReadLatticeData::SetRandomCell("RANDOM ");
      const int i19191 = 19191;
   }
   else {
      m_strCell = CellReaderB(m_strCell);
      if (m_strCell.length() > 3)
         CellReader(m_strCell);
   }

   return *this;
}

std::vector<LRL_ReadLatticeData> LRL_ReadLatticeData::ReadLatticeData() {
   std::vector<LRL_ReadLatticeData> inputList;
   LRL_ReadLatticeData readData;
   LRL_ReadLatticeData  input;
   size_t ordinal = 1;
   while (input.GetLattice() != "EOF") {
      input = read();
      if (input.IsValid()) {
         input.SetOrdinal(ordinal);
         inputList.push_back(input);
         ++ordinal;
      }

   }
   return inputList;
}
