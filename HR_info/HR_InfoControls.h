#ifndef HR_INFO_CONTROLS_H
#define HR_INFO_CONTROLS_H

#include "BaseControlVariables.h"
#include "BlockUtils.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"

#include <sstream>

static void showHelp()
{
   std::cout <<
      "Available commands (spelling approximation is active – you do not need to type the exact words):\n\n"
      "  sg <number>      Show the space-group whose number is <number>.\n"
      "                   Examples: sg 186, spacegroup 152, spg 194\n\n"
      "  bravais hP       List every space-group whose Bravais lattice is hP.\n"
      "  bravais hR       List every space-group whose Bravais lattice is hR.\n"
      "                   Examples: bravais hp, bravais hr, lattice hp\n\n"
      "  system Trigonal  List every space-group belonging to the Trigonal system.\n"
      "  system Hexagonal List every space-group belonging to the Hexagonal system.\n"
      "                   Examples: sys Trigonal, system hex, crystal hexagonal\n\n"
      "  family Hexagonal List every space-group that belongs to the Hexagonal family.\n"
      "                   Examples: fam Hexagonal, family hex, hexagonal family\n\n"
      "  r_sttings        some information about rhombohedral lattices including\n"
      "                   obverse/reverse and indexing.\n\n"
      "  q                Quit the program.\n"
      "                   Any close spelling of \"quit\" also exits.\n\n";
}


class HR_InforControls : public BaseControlVariables {
public:

   friend std::ostream& operator<< (std::ostream& os, const HR_InforControls& csc) {

      return os;
   }

   HR_InforControls() {

         InputHandler::registerHandler("HELP", 0.30,
            [this](const BaseControlVariables&, const std::string& value) {
               showHelp();
            });


   }

private:



};

#endif // HR_INFO_CONTROLS_H
