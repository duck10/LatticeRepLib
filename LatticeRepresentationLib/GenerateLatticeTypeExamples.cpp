#include "GenerateLatticeTypeExamples.h"

GenerateLatticeTypeExamples::GenerateLatticeTypeExamples() {

}

IT_3::IT_3() : GenerateNiggliBase() {
   m_name = "44A";
   m_bravaisType = "cP";
   m_ITnumber = " 3";
   m_freeParams = 1;
   m_character = "[r, r, r, 0, 0, 0]";
   //m_DC7_character = "[r,r,r,2r,2r,2r,2r]";
   m_prjPrefix = 1.0 / 3.0;
   m_strProjector = "1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
   m_projector = m_prjPrefix * MatG6(m_strProjector);
   /*
      m_projector = 0.333333 0.333333 0.333333 0 0 0
   0.333333 0.333333 0.333333 0 0 0
   0.333333 0.333333 0.333333 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0
   0 0 0 0 0 0 ;
   */
} // end of class IT_Lat_Char_3 constructor


std::ostream& operator<< (std::ostream& o, const GenerateNiggliBase& dfr) {
   o << "IT number " << dfr.GetITNumber() << std::endl;
   o << "Niggli/Roof name " << dfr.GetName() << std::endl;
   o << "Bravais Type " << dfr.GetBravaisType() << std::endl;
   o << "General Bravais Type " << dfr.GetBravaisLatticeGeneral() << std::endl;
   o << "Lattice character " << dfr.GetCharacter() << std::endl;
   o << "Free parameter count " << dfr.GetFreeParams() << std::endl;
   o << "Projector " << dfr.GetStringProjector() << std::endl;
   o << "Projector\n" << dfr.GetPrj() << std::endl;
   //std::string m_toCentered_E3;
   //MatG6  m_toCentered;
   //std::string m_ITnumber;
   //double m_prjPrefix;
   //MatG6 m_projector;
   //std::string& m_name; // either Delone name (C5) or Niggli name (55B)
   //std::string& m_bravaisLatticeGeneral; // like oS, or mS for face centered
   //std::string& m_bravaisType;
   //std::string& m_character;
   //int m_freeParams;
   //std::string m_strProjector;
   return o;
}

//std::ostream& operator<< (std::ostream& o, const GenerateDeloneBase& dfr) {
//   o << "m_latticeType           " << dfr.GetBravaisType() << std::endl;
//   return o;
//}
