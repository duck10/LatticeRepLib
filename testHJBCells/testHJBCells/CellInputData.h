#ifndef CELLINPUTDATA_H
#define CELLINPUTDATA_H
#ifdef _MSC_VER
#pragma once
#endif

#include "Cell.h"

#include <algorithm>
#include <string>
#include <utility>

class CellInputData {
public:
   CellInputData(void);

   bool operator== (const std::string& s) const;
   std::string GetInputDataType(void) const { return m_inputDataType; }
   std::string GetStrCell(void) const { return m_strCell; }
   std::string GetLattice(void) const { return m_lattice; }
   Cell GetCell(void)           const { return m_cell; }
   std::pair<int, int> GetVarietyRange(void)           const { return m_varietyRange; }
   void SetInputDataType(const std::string& inputDataType) { m_inputDataType = inputDataType; }
   void SetLattice(const std::string& lattice) { m_lattice = lattice; }
   void SetStrCell(const std::string& s) { m_strCell = s; }
   void SetCell(const Cell& c) { m_cell = c; }
   void SetVarietyRange(const std::pair<int, int>& p) { m_varietyRange = p; }

protected:
   std::string m_lattice;
   std::string m_inputDataType;
   std::string m_strCell;
   Cell        m_cell;
   std::pair<int, int> m_varietyRange;
};

#endif
