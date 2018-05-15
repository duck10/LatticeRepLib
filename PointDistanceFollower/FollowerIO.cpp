
#include "FollowerIO.h"

#include "FollowerConstants.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>

void FollowerIO::OutputInfo ( const std::string& s ) {
      std::cout << s;
}

void FollowerIO::OutputInfo ( const std::string& s, std::ostream& output ) {
   OutputSVG( s, output );
}

void FollowerIO::OutputError( const std::string& s, std::ostream& output ) {
   OutputSVG( s, output );
}

void FollowerIO::OutputSVG  ( const std::string& s, std::ostream& output ) {
      output << s;
}

