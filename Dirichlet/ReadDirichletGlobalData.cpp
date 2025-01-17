
#include "ReadDirichletGlobalData.h"

#include "G6.h"
#include "DirichletProgramConstants.h"
#include "LRL_RandTools.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "ParseData.h"
#include "Theta.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <utility>

std::string ReadCellData::m_constantData = "";
//std::string       FollowerConstants::globalDistanceDisable;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool not_space( const char c ) {
   return( c != ' ' );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool space( const char c ) {
   return( c == ' ' );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::vector<std::string> SplitBetweenBlanks( const std::string& s ) {
   std::vector<std::string> str;
   std::string::const_iterator i = s.begin( );

   while ( i != s.end( ) )
   {
      //look for the next non-blank
      i = std::find_if( i, s.end(), not_space );
      const std::string::const_iterator i2 = std::find_if( i, s.end( ), space );
      str.push_back( std::string( i, i2 ) );
      i = i2;
   }

   return( str );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::string GetDataString( const std::string& s, const size_t n ) {
   std::vector<std::string> vstr = SplitBetweenBlanks(s);
   if ( vstr.size( ) < n+1 ) vstr.resize( n+1, "0" );
   return( vstr[n] );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const int GetIntData( const std::string& s ) {
   return( atoi(s.c_str( ) ) );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const double GetDoubleData( const std::string& s ) {
   const double result = atof(s.c_str( ) );
   return( atof(s.c_str( ) ) );
}

const bool GetBoolData( const std::string& s ) {
   const std::string sData = GetDataString(LRL_StringTools::strToupper(s), 0);
   return( sData == "TRUE" || sData == "YES" );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::pair<int,int> Get2IntData( const std::vector<std::string>& s ) {
   const std::string sData1 = GetDataString( s[1], 0 );
   const std::string sData2 = GetDataString( s[2], 0 );
   return( std::make_pair( atoi(sData1.c_str()), atoi(sData2.c_str()) ) );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void SetGlobalValue( const std::string& dataType, 
                     const std::vector<std::string>& values, 
                     const std::string& wholeLine,
                     void* pData ) {
   if (values.size() < 2) return;
   if (dataType == "bool") {
      *(bool*)pData = GetBoolData(values[1]);
   }
   else if (dataType == "int") {
      *(int*)pData = GetIntData(values[1]);
   }
   else if (dataType == "double") {
      *(double*)pData = GetDoubleData(values[1]);
   }
   else if (dataType == "string") {
      *(std::string*)pData = values[1];
   }
   else if (dataType == "vectorStr") {
      (*(std::vector<std::string>*)pData).push_back( wholeLine + "\n");
   }
   else // data type unknown -- probably input error
      //(*(std::vector<std::string>*)pData).push_back(wholeLine);
      throw "unknown data type";
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// add some small random perturbation
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
G6 ReadCellData::GeneratePerturbation(const G6& v, const double perturbationFraction) {
   const G6 random = G6::rand();
   const G6 perturbation = perturbationFraction * random;
   const G6 perturbedVector = v + perturbation;
   return(perturbedVector);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// add some small random perturbation
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
G6 ReadCellData::GeneratePerturbation(const G6& v) {
   const G6 random = G6::rand();
   const G6 perturbation = /*FollowerConstants::globalFractionalAmountToPerturb **/ random;
   const G6 perturbedVector = v + perturbation;
   return(perturbedVector);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::pair<std::string, void*> FindBestTextMatch( const std::string& stringToMatch,
                                                       const std::vector<ParseData>& parseData,
                                                       const ThetaMatch<std::string>& tMatch ) {
   int bestMatchIndex = -1;
   double bestMatch = DBL_MAX;
   const std::string commandToMatch = SplitBetweenBlanks(stringToMatch)[0];

   for( size_t i=0; i!=parseData.size( ); ++i ) {
      const double match = tMatch.theta(LRL_StringTools::strToupper(commandToMatch), parseData[i].GetLabel());
      if ( match < bestMatch ) {
         bestMatch = match;
         bestMatchIndex = int(i);
      }
   }

   if (bestMatch > 0.6) {
      bestMatchIndex = 13;
      return(std::make_pair("vectorStr", parseData[bestMatchIndex].GetLocation()));
   }
   else {
      return(std::make_pair(parseData[bestMatchIndex].GetDataType(), parseData[bestMatchIndex].GetLocation()));
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool ReadCellData::GetDataFromCIN( const std::vector<ParseData>& parseData  ) {
   ThetaMatch<std::string> tMatch;
   char buffer[200];

   std::string s;
   std::vector<std::string> vStrTest;
   do {
      if ( std::cin.getline( buffer, sizeof(buffer)/sizeof(buffer[0]) ).eof( ) ) break;
      s = LRL_StringTools::strToupper( buffer );
      vStrTest = SplitBetweenBlanks( s );
   } while ( (! std::cin) || s.empty( ) || vStrTest.empty( ) || vStrTest[0].empty( ) );

   bool returnValue = false;
   if ( ! std::cin || s.substr(0,3)==std::string( "END" ) ) {
      returnValue = false;
   } else {
      const std::pair<std::string, void*> bestMatch = FindBestTextMatch( std::string(buffer), parseData, tMatch );
      const std::vector<std::string> vStr = SplitBetweenBlanks( s );
      if ( vStr.empty( ) || vStr[0].empty( ) || bestMatch.first.empty( ) )
         returnValue = true;
      SetGlobalValue( bestMatch.first, vStr, s, bestMatch.second );
      returnValue = true;
   }

   //OutputConstants(parseData);

   return returnValue;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
ReadCellData::ReadCellData( ) {
   std::cout << "Input Cell Data, end with \"end\"" << std::endl;
   const std::vector<ParseData> inputLabels = DirichletProgramConstants::BuildParseStructure( );

   while( std::cin && GetDataFromCIN( inputLabels ) ) { }

   m_constantData = DirichletProgramConstants::OutputConstants();
   //GLOBAL_Report::globalDataReport = FormatGlobalDataAsString( inputLabels );
}
