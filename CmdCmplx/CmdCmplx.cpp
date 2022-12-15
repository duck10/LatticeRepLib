
#include <complex>
#include <iostream>
#include <vector>

#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"


//void ReadComplexLatticeData::CellReader(const std::string& lattice, const std::string& cell) {
//   const int i19191 = 19191;
//}


//ReadComplexLatticeData::ReadComplexLatticeData(const int seed)
//   : LRL_ReadLatticeData(seed)
//{ }

//ReadComplexData ReadComplexData::read() {
//   std::getline(std::cin, m_strCell);
//   if (std::cin && (LRL_StringTools::strToupper(m_strCell.substr(0, 3)) != std::string("END"))) {
//      CellReader(m_strCell);
//   }
//   else {
//      m_lattice = "EOF";
//      m_cell.SetValid(false);
//   }
//   return *this;
//}

//ReadComplexData ReadComplexData::read() {
//
//}

void ComplexCell(const S6& s)
{

   static const double pi = 4.0 * atan(1.0);
   const double asq = -(s[3] + s[2] + s[1]);
   const double bsq = -(s[4] + s[2] + s[0]);
   const double csq = -(s[5] + s[1] + s[0]);
   const std::complex<double> a = std::sqrt(std::complex<double>(asq, 0.0));
   const std::complex<double> b = std::sqrt(std::complex<double>(bsq, 0.0));
   const std::complex<double> c = std::sqrt(std::complex<double>(csq, 0.0));

   const std::complex<double> cosal = s[0] / (b * c);
   const std::complex<double> cosbe = s[1] / (a * c);
   const std::complex<double> cosga = s[2] / (a * b);

   const std::complex<double> ac1 = std::acos(cosal);
   const std::complex<double> ac2 = std::acos(cosbe);
   const std::complex<double> ac3 = std::acos(cosga);



   const std::complex<double> al = std::acos(cosal) * 180.0 / pi;
   const std::complex<double >be = std::acos(cosbe) * 180.0 / pi;
   const std::complex<double> ga = std::acos(cosga) * 180.0 / pi;

   std::cout << std::endl;
   std::cout << "input S6 " << s << std::endl;
   std::cout << "as cell  " << a << " " << b << " " << c << "    " << al << " " << be << " " << ga << std::endl;
   std::cout << "G6    " << G6(s) << std::endl;
}

int main()
{
    std::cout << "; To Complex" << std::endl;
    const std::vector<ReadComplexLatticeData> inputList = ReadComplexLatticeData().ReadLatticeData();

    for (size_t i = 0; i < inputList.size(); ++i) {
        std::cout << inputList[i].GetCell() << std::endl;
        ComplexCell(S6(inputList[i].GetCell()));
    }
   //ReadComplexLatticeData rcd;
   //rcd.read();
   //const int i19191 = 19191;
   //const std::string lattice = LRL_ToString(rcd.GetStrCell()[0]);
   //const std::string strcell = rcd.GetStrCell();
   //const std::string strCellWithoutLattice = strcell.substr(2, strcell.size() - 1);
   //rcd.CellReader(lattice, strCellWithoutLattice);
   //if (!rcd.IsValid())ComplexCell(rcd.GetCell());
   //rcd.CellReader("P", strCellWithoutLattice);
   //if (!rcd.IsValid())ComplexCell(rcd.GetCell());



   //const bool allowInvalid = true;
   //const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   //for (size_t i = 0; i < inputList.size(); ++i) {
   //   const S6 test(inputList[i].GetCell());
   //   ComplexCell(test);
   //}

   //ComplexCell(S6(" 0 0 0 200  0 100"));
   //ComplexCell(S6(" 0 0 0 100 100 100"));
   //ComplexCell(S6(" 0 0 0 0 100 100"));
   //ComplexCell(S6(" 0 0 0 0 0 100"));
   //ComplexCell(S6(" 0 0 0 0 0 0"));
   //ComplexCell(S6("100 100 100 -1 0 0"));
   //ComplexCell(S6("100 100 100 1 0 0"));
   //ComplexCell(S6("100 100 100 0 0 0"));
   //ComplexCell(S6("-100 -100 -100 0 0 0"));
   //ComplexCell(S6("-100 -100 -100 0.01 0 0"));
   //ComplexCell(S6("-100 -100 -100 1 0 0"));

   //ComplexCell(S6("-100 -100 -100 -30 -20 -10"));
   //ComplexCell(S6("100 100 100 30 20 10"));
   //std::cout << LRL_Cell_Degrees(S6("-100 -100 -100 0 0 0")) << std::endl;
}
