#pragma once

#ifndef FOLLOWERIO_H
#define FOLLOWERIO_H

#include <ostream>
#include <string>

class FollowerIO {

public:
   static void OutputInfo ( const std::string& s );
   static void OutputInfo ( const std::string& s, std::ostream& output );
   static void OutputError( const std::string& s, std::ostream& output );
   static void OutputSVG  ( const std::string& s, std::ostream& output );
};

#endif
