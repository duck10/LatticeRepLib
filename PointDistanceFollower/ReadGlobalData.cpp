
#include "ReadGlobalData.h"

#include "G6.h"
#include "FollowerConstants.h"
//#include "FollowerIO.h"
#include "LRL_RandTools.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
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
const std::string GetDataString( const std::string& s, const unsigned long n ) {
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

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//std::string ReadGlobalData::strToupper(const std::string& s) {
//   std::string ss;
//   std::transform(s.begin(), s.end(), std::back_inserter( ss ), toupper );
//   return( ss );
//}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
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
std::string GetG6Data( const std::vector<std::string>& v ) {
   std::string s = """";
   for( unsigned long i=1; i<7; ++i )
      s += v[i] + " ";
   return( s + """" );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void SetGlobalValue( const std::string& dataType, 
                     const std::vector<std::string>& value, 
                     void* pData ) {
   if ( dataType == "g6" &&  value.size() < 7) {
      std::cout << "REJECTED " << value[0] << std::endl << std::flush;
      return;
   } else if ( dataType != "g6" && value.size() < 2 ) {
      std::cout << "REJECTED " << value[0] << std::endl << std::flush;
      return;
   }

   if ( dataType == "bool" ) {
      *(bool*)pData = GetBoolData( value[1] );
   } else if ( dataType == "int" ) {
      *(int*)pData = GetIntData( value[1] );
   } else if ( dataType == "double" ) {
      *(double*)pData = GetDoubleData( value[1] );
   } else if ( dataType == "2doubles" ) {
      const std::pair<int,int> plotSpec = Get2IntData( value );
      *(std::pair<int,int>*)pData = plotSpec;
   } else if ( dataType == "string" ) {
      *(std::string*)pData = value[1];
   } else if ( dataType == "g6" ) {
      const std::string v6 = GetG6Data( value );
      if ( v6[0] != DBL_MAX )
         GLOBAL_RunInputVector::globalData.push_back( v6 );
   } else if ( dataType == "fixRandomSeed" ) {
      *(bool*)pData = GetBoolData( value[1] );
   } else if ( dataType == "FollowerMode" ) {
      if (LRL_StringTools::strToupper(value[1]) == "LINE") {
         FollowerConstants::globalFollowerMode = FollowerConstants::globalLine;
      }
      else if(LRL_StringTools::strToupper(value[1]) == "LINE3") {
         FollowerConstants::globalFollowerMode = FollowerConstants::globalLine3;
      }
   else {
      FollowerConstants::globalFollowerMode = FollowerConstants::globalSinglePoint;
   }
   }
   else {
   }
}

//    enum enumFollowerMode{ globalSinglePoint, globalLine, globalLine3 };

std::string ReadGlobalData::GetFollowerMode(void) {
   if (FollowerConstants::globalFollowerMode == FollowerConstants::globalSinglePoint) return "Single Point";
   if (FollowerConstants::globalFollowerMode == FollowerConstants::globalLine) return "Line";
   if (FollowerConstants::globalFollowerMode == FollowerConstants::globalLine3) return "LINE3";
   return "UNKNOWN FOLLOWER MODE";
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
   const G6 perturbation = FollowerConstants::globalFractionalAmountToPerturb * random;
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
   else if ( dataType == "fixRandomSeed" ) {
      s = *(bool*)pData ? "true" : "false";
	  SetSeed(std::abs(GLOBAL_RunInputVector::globalConstantRandomSeed ? GLOBAL_RunInputVector::globalInputRandomSeed : (int)(time(0)%32767)) );
   }
   else if ( dataType == "FollowerMode" ) { //globalFollow, globalMovie, globalWeb
      if ( (*(FollowerConstants::enumFollowerMode*)pData) == FollowerConstants::globalSinglePoint)
         s = "point";
      else
         s = "line";
   } else {
   }

   return( s );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::pair<std::string, void*> FindBestTextMatch( const std::string& stringToMatch,
                                                       const std::vector<ReadGlobalData::ParseData>& parseData,
                                                       const ThetaMatch<std::string>& tMatch ) {
   unsigned long bestMatchIndex = 0;
   double bestMatch = DBL_MAX;
   const std::string commandToMatch = SplitBetweenBlanks(stringToMatch)[0];

   for( unsigned long i=0; i!=parseData.size( ); ++i ) {
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
bool ReadGlobalData::GetDataFromCIN( const std::vector<ReadGlobalData::ParseData>& parseData  ) {
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
      SetGlobalValue( bestMatch.first, vStr, bestMatch.second );
      return( true );
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::vector<ReadGlobalData::ParseData> ReadGlobalData::BuildParseStructure( void ) {
   std::vector<ParseData> v;

   v.push_back( ParseData( LRL_StringTools::strToupper( "PrintDistanceData" ), "bool", (void*)&FollowerConstants::globalPrintAllDistanceData ) );
   v.push_back( ParseData( LRL_StringTools::strToupper( "GlitchesOnly" ),      "bool", (void*)&FollowerConstants::globalOutputGlitchesOnly ) );

   v.push_back( ParseData( LRL_StringTools::strToupper( "Plot" ),             "2doubles", (void*)&FollowerConstants::globalWhichComponentsToPlot ) );

   v.push_back( ParseData( LRL_StringTools::strToupper( "PerturbBy" ),      "double", (void*)&FollowerConstants::globalFractionalAmountToPerturb ) );
   v.push_back( ParseData( LRL_StringTools::strToupper( "GlitchLevel" ),    "double", (void*)&FollowerConstants::globalPercentChangeToDetect ) );
   v.push_back( ParseData( LRL_StringTools::strToupper( "BoundaryWindow" ), "double", (void*)&FollowerConstants::globalFractionToDetermineCloseToBoundary ) );

   v.push_back( ParseData( LRL_StringTools::strToupper( "FramesPerPath" ), "int",  (void*)&FollowerConstants::globalFramesPerSegment ) );
   v.push_back( ParseData( LRL_StringTools::strToupper( "StepsPerFrame" ),"int",  (void*)&FollowerConstants::globalStepsPerFrame ) );
   v.push_back( ParseData( LRL_StringTools::strToupper( "Trials" ),       "int",  (void*)&FollowerConstants::globalNumberOfTrialsToAttempt ) );
   v.push_back( ParseData( LRL_StringTools::strToupper( "AllBlack" ),     "bool", (void*)&FollowerConstants::globalPlotAllSegmentsAsBlack ) );

   v.push_back( ParseData( LRL_StringTools::strToupper( "FilePrefix" ),   "string", (void*)&FollowerConstants::globalFileNamePrefix ) );

   v.push_back( ParseData( LRL_StringTools::strToupper( "GraphSize" ),    "int", (void*)&SVG_WriterConstants::globalGraphSpace ) );
   v.push_back( ParseData( LRL_StringTools::strToupper( "Border" ),       "int", (void*)&SVG_WriterConstants::globalGraphBorder ) );
   v.push_back( ParseData( LRL_StringTools::strToupper( "PlotSize" ),     "int", (void*)&SVG_WriterConstants::globalPlotSpace ) );

   v.push_back(ParseData(LRL_StringTools::strToupper("G6LineWidth"    ), "int", (void*)&SVG_DistancePlotConstants::globalG6DataLineStrokeWidth));
   v.push_back(ParseData(LRL_StringTools::strToupper("DeloneLineWidth"), "int", (void*)&SVG_DistancePlotConstants::globalDeloneDataLineStrokeWidth));
   v.push_back( ParseData(LRL_StringTools::strToupper( "AxisWidth" ),             "int", (void*)&SVG_DistancePlotConstants::globalDataAxisWidth       ) );
   v.push_back( ParseData(LRL_StringTools::strToupper( "TicLength" ),             "int", (void*)&SVG_DistancePlotConstants::globalY_AxisTicMarkLength ) );

   v.push_back( ParseData(LRL_StringTools::strToupper( "Vector" ),                "g6", (void*)0 ) );

   v.push_back( ParseData( LRL_StringTools::strToupper( "FollowerMode" ),      "FollowerMode", (void*)&FollowerConstants::globalFollowerMode ) );

   v.push_back( ParseData(LRL_StringTools::strToupper( "FixRandomSeed" ),      "fixRandomSeed", (void*)&GLOBAL_RunInputVector::globalConstantRandomSeed ) );

   v.push_back(ParseData(LRL_StringTools::strToupper("RandomSeed"), "int", (void*)&GLOBAL_RunInputVector::globalInputRandomSeed));

   //bool GLOBAL_FileNames::globalShouldTimeStamp = true; // NOT IMPLEMENTED AS INPUT VARIABLE !!!!!!!!!!!!

   v.push_back(ParseData(LRL_StringTools::strToupper("TimeStamp"), "bool", (void*)&GLOBAL_Files::globalShouldTimeStamp));

   v.push_back( ParseData(LRL_StringTools::strToupper( "END" ),             "end", (void*)0) );

   return( v );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::string ReadGlobalData::FormatGlobalDataAsString( const std::vector<ParseData>& parseData ) {   
   std::string s;
   for( unsigned long i=0; i<parseData.size(); ++i )
   {
      const ParseData& pd = parseData[i];
      s += pd.m_label + " " + pd.m_dataTypeToRead + " " + TranslateGlobalValue(pd.m_dataTypeToRead, pd.m_dataLocation) + "\n";
   }

   s += "\n";

   s += ((GLOBAL_RunInputVector::globalData.empty( ) ) ? "\n" : "User Input Vectors ****************************\n");

   std::vector<G6>::const_iterator it = GLOBAL_RunInputVector::globalData.begin();
   for ( ;it !=GLOBAL_RunInputVector::globalData.end(); ++ it )
      s += LRL_ToString( *it ) + "\n";

   s += ((GLOBAL_RunInputVector::globalData.empty( ) ) ? "\n" : "END User Input Vectors ****************************\n\n");

   return( s );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
ReadGlobalData::ReadGlobalData( ) {
   std::cout << "Input Global Data, end with \"end\"" << std::endl;
   const std::vector<ParseData> inputLabels = BuildParseStructure( );

   while( std::cin && GetDataFromCIN( inputLabels ) ) { }

   GLOBAL_Report::globalDataReport = FormatGlobalDataAsString( inputLabels );
   //FollowerIO::OutputInfo( GLOBAL_Report::globalDataReport );
}
