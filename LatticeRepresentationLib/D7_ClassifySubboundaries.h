#ifndef CLASSIFYSUBBOUNDARIES_H
#define CLASSIFYSUBBOUNDARIES_H

#include "D7_boundary.h"
#include "D7_BoundaryList.h"

#include <string>
#include <vector>

class D7_Boundary;
class D7;

class D7_ClassifySubboundaries {
public:
   static std::vector<std::pair<std::string, std::string> > ClassifyVector( const D7& v, const double delta );
   static std::string ClassifyCase( const std::string& name, const D7& v, const double delta );
   static std::string ClassifyCase4( const D7& v, const double delta );
   static std::string ClassifyCase5( const D7& v, const double delta );
   static std::string ClassifyCase6( const D7& v, const double delta );
   static std::string ClassifyCase8( const D7& v, const double delta );
   static std::string ClassifyCase9( const D7& v, const double delta );
   static std::string ClassifyCase7( const D7& v, const double delta );
   static void        TestClassify( const int ntest, const D7_Boundary& d, std::string( *fn )(const D7& v, const double delta), const double delta);
   static void        TestClassifyFunctions( void );
};


#endif