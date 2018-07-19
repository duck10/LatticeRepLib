#include "Delone.h"
#include "D6.h"
#include "LatticeConverter.h"
#include "MaximaTools.h"
#include "ReadCellData.h"
#include "Reducer.h"
#include "ToString.h"


void LatticeConverter::TextOutput(const std::string& label, const std::string& lattice, const Cell& cell) const {
    std::cout << label << std::endl;
    std::cout << "lattice " << lattice << std::endl;
    std::cout << "Cell  " << ToString(cell) << std::endl;
    std::cout << "G6 " << ToString(cell.Cell2V6()) << std::endl;
    std::cout << "D7  " << ToString(D7(cell.Cell2V6())) << std::endl;
    std::cout << "Scalars " << ToString(D6(cell)) << std::endl;
}

void LatticeConverter::MaximaOutput(const std::string& label, const std::string& lattice, const Cell& cell) const {
    std::cout << label << std::endl;
    std::cout << "lattice " << lattice << std::endl;
    std::cout << "Cell  " << MaximaTools::MaximaFromString(ToString(cell)) << std::endl;
    std::cout << "G6 " << MaximaTools::MaximaFromString(ToString(cell.Cell2V6())) << std::endl;
    std::cout << "D7  " << MaximaTools::MaximaFromString(ToString(D7(cell.Cell2V6()))) << std::endl;
    std::cout << "Scalars " << MaximaTools::MaximaFromString(ToString(D6(cell))) << std::endl;
}

LatticeConverter::LatticeConverter(const eOutputType type)
        : m_OutputType(type)
    {

    }

    void LatticeConverter::SetOutputMaxima(void) { m_OutputType = emaxima; }
    void LatticeConverter::SetOutputText(void) { m_OutputType = etext; }

    void LatticeConverter::Output(const std::string& label, const std::string& lattice, const Cell& cell) const {
        (m_OutputType == etext) ? TextOutput(label, lattice, cell) : MaximaOutput(label, lattice, cell);
        //std::cout << label << std::endl;
        //std::cout << "lattice " << lattice << std::endl;
        //std::cout << "Cell  " << ToString(cell) << std::endl;
        //std::cout << "G6 " << ToString(cell.Cell2V6()) << std::endl;
        //std::cout << "D7  " << ToString(D7(cell.Cell2V6())) << std::endl;
        //std::cout << "Scalars " << ToString(D6(cell)) << std::endl;
    }

    Cell LatticeConverter::NiggliReduceCell(const std::string& lattice, const Cell& cell) {
        const G6 g6 = cell.Cell2V6();
        const Mat66 mLattice = Cell::LatSymMat66(lattice, cell);
        Mat66 m66;
        G6 redVec;
        const bool b = Reducer::Reduce(mLattice*g6, m66, redVec, 0.00000001);
        if (b) {
            return Cell(redVec);;
        }
        else {
            return Cell();
        }
    }

    void LatticeConverter::NiggliReducedOutput(const std::string& label, const std::string& lattice, const Cell& cell) {
        const Cell reducedCell = NiggliReduceCell(lattice, cell);
        Output(label, "P", reducedCell);
    }

    Cell LatticeConverter::DeloneReduceCell(const std::string& lattice, const Cell& cell) {
        const G6 g6 = cell.Cell2V6();
        const Mat66 mLattice = Cell::LatSymMat66(lattice, cell);
        Mat66 m66;
        G6 redVec;
        const bool b = Delone::Reduce(mLattice*g6, m66, redVec, 0.00000001);
        if (b) {
            return Cell(redVec);;
        }
        else {
            return Cell();
        }
    }

    void LatticeConverter::DeloneReducedOutput(const std::string& label, const std::string& lattice, const Cell& cell) {
        const Cell reducedCell = DeloneReduceCell(lattice, cell);
        Output(label, "P", reducedCell);
    }
