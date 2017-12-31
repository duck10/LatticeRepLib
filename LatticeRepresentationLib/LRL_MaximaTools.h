#ifndef LRL_MAXIMATOOLS_H
#define LRL_MAXIMATOOLS_H

#include <string>

#include "LRL_ToString.h"

namespace LRL_MaximaTools {
bool not_space( const char c ) ;

bool space( const char c );

bool newline( const char c );

std::string MaximaFromString( const std::string& s );

const std::string Retrieve_One_Matrix_Row( const unsigned long rowLength, const std::string& s );

std::string MaximaFromString( const unsigned long rowLength, const std::string& s );

template<typename TMAT>
std::string MaximaFromMat( const TMAT& m ) {
   return MaximaFromString( m.GetRowDim(), LRL_ToString( m ) );
}

};


#endif  // LRL_MAXIMATOOLS_H