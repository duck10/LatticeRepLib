#ifndef READGLOBALDATA_H
#define READGLOBALDATA_H

#include "G6.h"

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

   class ParseData {
   public:
      std::string m_label;
      std::string m_dataTypeToRead;
      void*       m_dataLocation;
      ParseData( const std::string& label, const std::string& dataType, void* dataLocation )
         : m_label( label )
         , m_dataTypeToRead(dataType)
         , m_dataLocation( dataLocation )
      {
      };
   };

private:
   static bool GetDataFromCIN( const std::vector<ReadGlobalData::ParseData>& );
   static std::vector<ReadGlobalData::ParseData> BuildParseStructure( void );
   static std::string FormatGlobalDataAsString( const std::vector<ParseData>& parseData );

   static std::string m_followerMode;
};

#endif
