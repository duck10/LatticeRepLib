// prj89.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "G6.h"
#include "LRL_Cell.h"
#include "LRL_CoordinateConversionMatrices.h"
#include "LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "Niggli.h"
#include "StoreResults.h"


static const std::string Letters(void) {
	return "V,G,D,S,P,A,B,C,I,F,R,C3,G6,S6,B4,D7,H";
}


std::vector<LRL_ReadLatticeData> GetInputCells(void) {
	const std::string letters = Letters();
	std::string lattice;
	std::vector<LRL_ReadLatticeData> cellDataList;
	LRL_ReadLatticeData rcd;
	while (lattice != "EOF") {
		rcd.read();
		lattice = rcd.GetLattice();
		if ((!lattice.empty()) && (letters.find(static_cast<char>(toupper(lattice[0]))) != std::string::npos))
			cellDataList.push_back(rcd);
	}

	return cellDataList;
}


int main()
{
	StoreResults<double, std::string> store;
	std::cout << "prj89\n";

	const std::vector<LRL_ReadLatticeData> input = GetInputCells();
	G6 vout;
	for (size_t i = 0; i < input.size(); ++i) {
		const G6 inputg6 = input[i].GetCell();
		const bool bniggli = Niggli::ReduceWithoutMatrices(inputg6, vout, 0.0001);
		const double dist = (vout = inputg6).norm();
		std::cout << dist << "  " << input[i].GetStrCell() << "\treduced  " << LRL_Cell_Degrees(vout) << std::endl;
		const double key = pow(double(int(log10(dist))),10.0);
		store.Store(key, input[i].GetStrCell());
	}
	store.ShowResults();

}
