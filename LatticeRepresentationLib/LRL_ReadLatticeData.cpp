
//#include "stdafx.h"

#include "LRL_Cell.h"
#include "G6.h"
#include "S6.h"
#include "B4.h"
#include "D7.h"
#include "LRL_RandTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "LRL_StringTools.h"


#include <algorithm>
#include <complex>
#include <cmath>
#include <sstream>
#include <string>
#include <utility>

void LRL_ReadLatticeData::CellReader(const std::string& lattice, const std::string& cell) {
   CellReader(lattice + " " + cell);
}

static int folseed = 19192;

//static const std::vector<unsigned long> vAvoid(avoidList, avoidList + sizeof(avoidList) / sizeof(avoidList[0]));
std::string G6_names[] = { "G ", "V ", "G6 " };
std::vector<std::string> vG6_names(G6_names, G6_names + sizeof(G6_names) / sizeof(G6_names[0]));

std::string D7_names[] = { "D ", "D7 " };
std::vector<std::string> vD7_names(D7_names, D7_names + sizeof(D7_names) / sizeof(D7_names[0]));

std::string S6_names[] = { "S ", "S6 " };
std::vector<std::string> vS6_names(S6_names, S6_names + sizeof(S6_names) / sizeof(S6_names[0]));

std::string C3_names[] = { "C3 " };
std::vector<std::string> vC3_names(C3_names, C3_names + sizeof(C3_names) / sizeof(C3_names[0]));

std::string lattice_names[] = { "A ", "B ", "C ", "P ", "R ", "F ", "I " };
std::vector<std::string> vlattice_names(lattice_names, lattice_names + sizeof(lattice_names) / sizeof(lattice_names[0]));

void LRL_ReadLatticeData::CellReader(const std::string& s) {
   bool valid = false;
   if ((LRL_StringTools::strToupper(s.substr(0, 3)) != std::string("END"))) {
      std::istringstream iss(s);
      iss >> m_inputDataType;
      m_inputDataType = LRL_StringTools::strToupper(m_inputDataType + " ");
      if (m_inputDataType == "RANDOM ") {
         m_inputDataType = "P";
         m_lattice = "P";
         //const G6 g6Cell = LRL_Cell::randDeloneUnreduced();
         const G6 g6Cell = generator.SetLengthLimits(10.0, 100.0).GenerateExtreme();
         m_cell = LRL_Cell(g6Cell);
         m_strCell = LRL_ToString(LRL_Cell(m_cell));
         valid = true;
      }
      else if ( std::find(vG6_names.begin(),vG6_names.end(), m_inputDataType) != vG6_names.end()) {
         G6 v6;
         for (unsigned long i = 0; i < 6; ++i)
            iss >> v6[i];
         m_cell = v6;
         valid = m_cell.GetValid();
      }
      else if (std::find(vD7_names.begin(), vD7_names.end(), m_inputDataType) != vD7_names.end()) {
         D7 v7;
         double d;
         for (unsigned long i = 0; i < 7; ++i) {
            iss >> d;
            v7[i] = d;
         }
         v7.SetValid(true);
         G6 v6(v7);
         m_cell = v6;
         valid = true;
      }
      else if (std::find(vS6_names.begin(), vS6_names.end(), m_inputDataType) != vS6_names.end()) {
         S6 e;
         for (unsigned long i = 0; i < 6; ++i)
            iss >> e[i];
         valid = true;
         m_cell = e;
      }
      else if (std::find(vC3_names.begin(), vC3_names.end(), m_inputDataType) != vC3_names.end()) {
         C3 e;
         std::vector<double> vd(20);
         double d;
         for (unsigned long i = 0; i < 6; ++i) {
            iss >> d;
            vd[i] = d;
         }
         e[0] = std::complex<double>(vd[0], vd[1]);
         e[1] = std::complex<double>(vd[2], vd[3]);
         e[2] = std::complex<double>(vd[4], vd[5]);
         e.SetValid(true);
         m_lattice = "P";
         m_inputDataType = "P";
         m_cell = e;
         valid = true;
      }
      else if (std::find(vlattice_names.begin(), vlattice_names.end(), m_inputDataType) != vlattice_names.end()) {
         for (unsigned long i = 0; i < 6; ++i)
            iss >> m_cell[i];
         for (unsigned long i = 3; i < 6; ++i)
            m_cell[i] *= 4.0 * atan(1.0) / 180.0;
         valid = true;
         m_cell.SetValid(true);
      }
      else {
      }

      if (valid && m_cell.GetValid()) {
         iss >> m_lattice;
         m_lattice = m_inputDataType[0];
         m_cell.SetValid(true);
         if (LRL_StringTools::strToupper(m_lattice) == "END" || LRL_StringTools::strToupper(m_lattice) == "EOF") {
            m_lattice = "EOF";
            return;
         }
      }
      else {
         std::cout << "input line rejected, invalid" << std::endl;
         m_lattice = "";
      }
   }
   else {
      m_lattice = "EOF";
   }
}

LRL_ReadLatticeData::LRL_ReadLatticeData(const int seed )
: generator(seed) {
}

LRL_ReadLatticeData LRL_ReadLatticeData::read(void) {
   std::getline(std::cin, m_strCell);
   if (std::cin && (LRL_StringTools::strToupper(m_strCell.substr(0, 3)) != std::string("END"))) {
      CellReader(m_strCell);
      return *this;
   }
   else {
      m_lattice = "EOF";
      return *this;
   }
}
