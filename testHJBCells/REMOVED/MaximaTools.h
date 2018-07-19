#ifndef MAXIMATOOLS_H
#define MAXIMATOOLS_H
#ifdef _MSC_VER
#pragma once
#endif

#include <string>

namespace MaximaTools {
bool not_space( const char c ) ;

bool space( const char c );

bool newline( const char c );

std::string MaximaFromString( const std::string& s );

const std::string Retrieve_One_Matrix_Row( const size_t rowLength, const std::string& s );

std::string MaximaFromString( const size_t rowLength, const std::string& s );

template<typename TMAT>
std::string MaximaFromMat( const TMAT& m ) {
   return MaximaFromString( m.GetRowDim(), ToString( m ) );
}

std::string makeMat7( std::string name, std::string x, std::string s );

};


#endif