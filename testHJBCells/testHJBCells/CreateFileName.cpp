#include "CreateFileName.h"
#include <ctime>
#include <cstdio>


#pragma warning( disable: 4996 )
///////////////////////////////////////////////////////////////////////
// Create a unique file name from the date and time
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*static*/ std::string CreateFileName::Create( const std::string& prefix, const std::string& extension )
//---------------------------------------------------------------------
{
   const time_t       now        = time(NULL);
         char         buft[256];
   const int          len        = sizeof(buft);

   for ( int i=0; i<len; ++i ) buft[i] = '\0';

   strftime( buft, sizeof(buft), "%Y-%m-%d.%X", localtime(&now) );

   for ( int i=0; i<len; ++i )
   {
      // blanks are irritating and colons are illegal
      if ( buft[i] == ' ' || buft[i] == ':' ) buft[i] = '_';
   }

   return( prefix + "_" + std::string( buft ) + "." + extension );
}
