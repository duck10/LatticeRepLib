
#include "ReadGlobalData.h"

#include "G6.h"
#include "DirichletConstants.h"
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
                     const std::vector<std::string>& value, 
                     const std::string& wholeLine,
                     void* pData ) {
   //if (value.size() > 2) {}
   if (false) {}
   else if ( dataType == "bool" ) {
      *(bool*)pData = GetBoolData( value[1] );
   } else if ( dataType == "int" ) {
      *(int*)pData = GetIntData( value[1] );
   } else if ( dataType == "double" ) {
      *(double*)pData = GetDoubleData( value[1] );
   }
   else if (dataType == "string") {
      *(std::string*)pData = value[1];
   }
   else if (dataType == "vector") {
      (*(std::vector<std::string>*)pData).push_back(wholeLine);


      //   if (LRL_StringTools::strToupper(value[1]) == "LINE") {
   //      FollowerConstants::globalFollowerMode = FollowerConstants::globalLine;
   //   }
   //   else if (LRL_StringTools::strToupper(value[1]) == "LINE3") {
   //      FollowerConstants::globalFollowerMode = FollowerConstants::globalLine3;
   //   }
   //   else if (LRL_StringTools::strToupper(value[1]) == "TRIANGLE") {
   //      FollowerConstants::globalFollowerMode = FollowerConstants::globaltriangle;
   //   }
   //   else {
   //      FollowerConstants::globalFollowerMode = FollowerConstants::globalSinglePoint;
   //   }
   //}
   //else if (dataType == "DistanceEnableString") {
   //   FollowerConstants::globalDistanceEnableList.push_back( value[1]);
   //}
   //else if (dataType == "DistanceDisableString") {
   //   std::vector<std::string>& thelist(FollowerConstants::globalDistanceEnableList);
   //   const std::vector<std::string>::iterator it = std::find(thelist.begin(), thelist.end(),  value[1]) ;
   //   if (thelist.end() != it) thelist.erase(it);
   //}
   //else {
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// add some small random perturbation
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
G6 ReadGlobalData::GeneratePerturbation(const G6& v, const double perturbationFraction) {
   const G6 random = G6::rand();
   const G6 perturbation = perturbationFraction * random;
   const G6 perturbedVector = v + perturbation;
   return(perturbedVector);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// add some small random perturbation
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
G6 ReadGlobalData::GeneratePerturbation(const G6& v) {
   const G6 random = G6::rand();
   const G6 perturbation = /*FollowerConstants::globalFractionalAmountToPerturb **/ random;
   const G6 perturbedVector = v + perturbation;
   return(perturbedVector);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::string TranslateGlobalValue( const std::string& dataType, void* pData ) {
   std::string s;

   if ( dataType == "bool" )
      s = *(bool*)pData ? "true" : "false";
   else if ( dataType == "int" )
      s = LRL_ToString(*(int*)pData);
   else if ( dataType == "double" )
      s = LRL_ToString(*(double*)pData);
   else if ( dataType == "2doubles" )
      s = LRL_ToString( (*(std::pair<int,int>*)pData).first ) + " " + LRL_ToString( (*(std::pair<int,int>*)pData).second );
   else if ( dataType == "string" )
      s = *(std::string*)pData;
   //else if ( dataType == "fixRandomSeed" ) {
   //   s = *(bool*)pData ? "true" : "false";
	  //SetSeed(std::abs(GLOBAL_RunInputVector::globalConstantRandomSeed ? GLOBAL_RunInputVector::globalInputRandomSeed : (int)(time(0)%32767)) );
   //}
   //else if ( dataType == "FollowerMode" ) { //globalFollow, globalMovie, globalWeb
   //   if ( (*(FollowerConstants::enumFollowerMode*)pData) == FollowerConstants::globalSinglePoint)
   //      s = "point";
   //   else
   //      s = "line";
   //}
 else {
   }

   return( s );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::pair<std::string, void*> FindBestTextMatch( const std::string& stringToMatch,
                                                       const std::vector<ParseData>& parseData,
                                                       const ThetaMatch<std::string>& tMatch ) {
   size_t bestMatchIndex = 0;
   double bestMatch = DBL_MAX;
   const std::string commandToMatch = SplitBetweenBlanks(stringToMatch)[0];

   for( size_t i=0; i!=parseData.size( ); ++i ) {
      const double match = tMatch.theta(LRL_StringTools::strToupper(commandToMatch), parseData[i].m_label);
      if ( match < bestMatch ) {
         bestMatch = match;
         bestMatchIndex = i;
      }
   }

   if ( bestMatch > 0.9 )
      return( std::make_pair( "", (void*)0 ) );
   else
      return(std::make_pair(parseData[bestMatchIndex].m_dataTypeToRead, parseData[bestMatchIndex].m_dataLocation));
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool ReadGlobalData::GetDataFromCIN( const std::vector<ParseData>& parseData  ) {
   ThetaMatch<std::string> tMatch;
   char buffer[200];

   std::string s;
   std::vector<std::string> vStrTest;
   do {
      if ( std::cin.getline( buffer, sizeof(buffer)/sizeof(buffer[0]) ).eof( ) ) break;
      s = LRL_StringTools::strToupper( buffer );
      vStrTest = SplitBetweenBlanks( s );
   } while ( (! std::cin) || s.empty( ) || vStrTest.empty( ) || vStrTest[0].empty( ) );

   if ( ! std::cin || s.substr(0,3)==std::string( "END" ) ) {
      return( false );
   } else {
      const std::pair<std::string, void*> bestMatch = FindBestTextMatch( std::string(buffer), parseData, tMatch );
      const std::vector<std::string> vStr = SplitBetweenBlanks( s );
      if ( vStr.empty( ) || vStr[0].empty( ) || bestMatch.first.empty( ) )
         return( true );
      SetGlobalValue( bestMatch.first, vStr, s, bestMatch.second );
      return( true );
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::string ReadGlobalData::FormatGlobalDataAsString( const std::vector<ParseData>& parseData ) {   
   std::string s;
   for( size_t i=0; i<parseData.size(); ++i )
   {
      const ParseData& pd = parseData[i];
      s += pd.m_label + " " + pd.m_dataTypeToRead + " " + TranslateGlobalValue(pd.m_dataTypeToRead, pd.m_dataLocation) + "\n";
   }

   s += "\n";

   return( s );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
ReadGlobalData::ReadGlobalData( ) {
   std::cout << "Input Global Data, end with \"end\"" << std::endl;
   const std::vector<ParseData> inputLabels = DirichletConstants::BuildParseStructure( );

   while( std::cin && GetDataFromCIN( inputLabels ) ) { }

   //GLOBAL_Report::globalDataReport = FormatGlobalDataAsString( inputLabels );
}
