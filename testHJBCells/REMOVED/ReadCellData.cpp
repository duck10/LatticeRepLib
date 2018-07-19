#ifdef _MSC_VER
#pragma once
#endif

#include "Cell.h"
#include "D6.h"
#include "ReadCellData.h"
#include "ReadGlobalData.h"
#include "D7.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>
#include <utility>

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//static std::string xxstrToupper( const std::string& s ) {
//   std::string ss;
//   std::transform(s.begin(), s.end(), std::back_inserter( ss ), toupper );
//   return( ss );
//}

void ReadCellData::CellReader(const std::string& lattice, const std::string& cell) {
   CellReader(lattice + " " + cell);
}

void ReadCellData::CellReader(const std::string& s) {
   if ( (xxstrToupper(s.substr(0, 3)) != std::string("END"))) {
      std::istringstream iss(s);
      iss >> m_inputDataType;
      if (toupper(m_inputDataType[0]) == 'V') {
         G6 v6;
         for( size_t i = 0; i < 6; ++i)
            iss >> v6[i];
         m_cell = v6;
      }
      else if (toupper(m_inputDataType[0]) == 'D') {
         D7 v7;
         double d;
         for( size_t i = 0; i < 7; ++i) {
         iss >> d;
         v7[i] = d;
      }
         G6 v6(v7);
         m_cell = v6;
      }
      else if (toupper(m_inputDataType[0]) == 'S') {
         D6 e;
         for( size_t i = 0; i < 6; ++i)
            iss >> e[i];
         m_cell = G6(e);
      }
      else {
         for( size_t i = 0; i < 6; ++i)
            iss >> m_cell[i];
         for( size_t i = 3; i < 6; ++i)
            m_cell[i] *= 4.0 * atan(1.0) / 180.0;
      }
      iss >> m_lattice;
      m_lattice = m_inputDataType;
      if (xxstrToupper(m_lattice) == "END" || xxstrToupper(m_lattice) == "EOF") {
         m_lattice = "EOF";
         return;
      }
   }
   else {
      m_lattice = "EOF";
   }
}

ReadCellData::ReadCellData(void) {

   std::getline(std::cin, m_strCell);
   if (std::cin && (xxstrToupper(m_strCell.substr(0, 3)) != std::string("END"))) {
      CellReader(m_strCell);
   }
   else {
      m_lattice = "EOF";
   }
}

//   ReadCellData::ReadCellData(void) {
//
//   std::getline( std::cin, m_strCell );
//   if (std::cin && (xxstrToupper(m_strCell.substr(0, 3)) != std::string("END"))) {
//      std::istringstream iss( m_strCell );
//      iss >> m_inputDataType;
//      if ( toupper( m_inputDataType[0] ) == 'V' ) {
//         G6 v6;
//         for( size_t i = 0; i < 6; ++i )
//            iss >> v6[i];
//         m_cell = v6;
//      }
//      else if ( toupper( m_inputDataType[0] ) == 'D' ) {
//         D7 v7;
//         for( size_t i = 0; i < 7; ++i )
//            iss >> v7[i];
//         G6 v6(v7);
//         m_cell = v6;
//      }
//      else if ( toupper( m_inputDataType[0] ) == 'S' ) {
//         DeloneScalars e;
//         for( size_t i = 0; i < 6; ++i )
//            iss >> e[i];
//         m_cell = G6( e );
//      }
//      else {
//         for( size_t i = 0; i < 6; ++i )
//            iss >> m_cell[i];
//         for( size_t i = 3; i < 6; ++i )
//            m_cell[i] *= 4.0 * atan( 1.0 ) / 180.0;
//      }
//      iss >> m_lattice;
//      m_lattice = m_inputDataType;
//	  if (xxstrToupper(m_lattice) == "END" || xxstrToupper(m_lattice) == "EOF") {
//		  m_lattice = "EOF";
//		  return;
//	  }
//   }
//   else {
//      m_lattice = "EOF";
//   }
//}
