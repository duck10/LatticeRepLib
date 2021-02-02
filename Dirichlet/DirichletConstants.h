#ifndef DIRICHLETCONSTANTS_H
#define DIRICHLETCONSTANTS_H

#include "ParseData.h"

#include <iostream>     // std::cout, std::endl
#include <string>
#include <vector>

/*
PROPOSED CONTROL PARAMETERS
ROTATE (to be applied before stereo or plotting, may be multiples)
number of images
scale
Delone Type (as opposed to cells) (where does this go: header or cell section?)
unit cells
?? canvas size ??
hide/show hidden lines

*/

namespace DirichletConstants {
   extern int    numberOfImages; //ok
   extern int    imageHeightPx; //ok
   extern int    canvas_x_size; //ok
   extern int    canvas_y_size; //ok
   extern double rotateX; //ok
   extern double rotateStereo; //ok
   extern double rotateZ;
   extern double scale; //ok
   extern std::string note; //ok
   extern std::string hiddenLineColor; //ok
   extern std::string faceColor; //ok
   extern std::string fileNamePrefix;

   extern std::vector<std::string> cellData; //ok

   extern std::string S6; //ok
   extern std::string G6;
   extern std::string D7;
   extern std::string C3;
   extern std::string P; //ok
   extern std::string I; //ok
   extern std::string F;
   extern std::string R;
   extern std::string H;
   extern std::string C;
   extern std::string A;
   extern std::string B;
   extern std::string S;
   extern std::string G;
   extern std::string V;
   extern std::string RANDOM; //ok

   extern std::vector<ParseData> BuildParseStructure(void);
   extern std::string OutputConstants ();


}


#endif  // DIRICHLETCONSTANTS_H
