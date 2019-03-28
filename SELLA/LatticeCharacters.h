#ifndef LATTICECHARACTERS_H
#define LATTICECHARACTERS_H

#include <string>
#include <vector>

class LatticeCharacters {
public:

	   "K1”, "cI”, "rrr rrr"
		"K3”, “cF”,		"rr0 rr0"      
		"K5”, “cP”,		"000 rrr"      
		"Q1”, “tI”,		"rrs rrs"      
		"Q2”, “tI”,		"rr0 rrs"      
		"Q5”, “tP”,		"000 rrs"      
		"R1”, "rP”, "rrr sss"
		"R3”, “rP”,		"r0r sr0"      
		"O1A", "oF,”		"rrs rrt"      
		"O1B", "oI,”		"rrs rrs"      
		"O2”, “oI”,		"rs0 sr0"      
		"O3”, “oI”,		"rs0 rs0"      
		"O4”, “oP”,		"00r sst"      
		"O5”, “oP”,		"000 rst"      
		"M1A", "mC,”		"rrs ttu"      
		"M1B", “mC, ”		"rst rsu"
		"M2A", “mC, ”		"rs0 rst"
		"M2B", “mC, ”		"rs0 stu"
		"M3”, “mC”,		"rs0 ts0"      
		"M4”, "mP”, "00r stu"
		"T1”, “aP”,		"rst uvw"      
		"T2”, “aP”,		"rs0 tuv"      
		"T3”, “aP”,		"rs0 tu0"      
		"H4”, "hP”, "00r rrs"




private:
	std::string m_deloneName;
	std::string m_bravaisType;
	std::string m_character;
	std::string m_toCentered_E3;
	MatS6       m_toCentered_S6;
	LabeledSellaMatricesForDeloneType m_matrices;



};


#endif  // LATTICECHARACTERS_H