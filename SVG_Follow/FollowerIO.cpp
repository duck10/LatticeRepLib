
#include "FollowerIO.h"

#include "FollowerConstants.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>

void FollowerIO::OutputInfo ( const std::string& s ) {
   if (FollowerConstants::globalFollowerMode == FollowerConstants::globalWeb ) {
   } else {
      std::cout << s;
   }
}

void FollowerIO::OutputInfo ( const std::string& s, std::ostream& output ) {
   OutputSVG( s, output );
}

void FollowerIO::OutputError( const std::string& s, std::ostream& output ) {
   OutputSVG( s, output );
}

void FollowerIO::OutputSVG  ( const std::string& s, std::ostream& output ) {
   if (FollowerConstants::globalFollowerMode == FollowerConstants::globalWeb ) {
      std::cout << s << std::endl;
   } else {
      output << s;
   }
}

