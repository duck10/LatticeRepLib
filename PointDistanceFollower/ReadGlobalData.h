#ifndef READGLOBALDATA_H
#define READGLOBALDATA_H

#include "G6.h"
#include "ParseData.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <utility>
#include <vector>

class ReadGlobalData {
public:

   ReadGlobalData( void );
   ~ReadGlobalData( void ) {};
   static G6 GeneratePerturbation(const G6& v, const double perturbationFraction);
   static G6 GeneratePerturbation(const G6& v);
   static std::string GetFollowerMode(void);
   //static std::string strToupper(const std::string& s);

   static bool GetDataFromCIN( const std::vector<ParseData>& );
   static std::vector<ParseData> BuildParseStructure( void );
   static std::string FormatGlobalDataAsString( const std::vector<ParseData>& parseData );
private:

   static std::string m_followerMode;
};

#endif
