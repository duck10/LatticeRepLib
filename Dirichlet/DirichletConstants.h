#ifndef DIRICHLETCONSTANTS_H
#define DIRICHLETCONSTANTS_H

#include "ParseData.h"
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
   extern double rotateY; //ok
   extern double rotateZ;
   extern double scale; //ok
   extern std::string note;
   extern std::string hiddenLineColor;
   extern std::string faceColor;
   extern std::string fileNamePrefix;

   extern std::vector<std::string> cellData;

   extern std::string S6;
   extern std::string G6;
   extern std::string D7;
   extern std::string C3;
   extern std::string P;
   extern std::string I;
   extern std::string F;
   extern std::string R;
   extern std::string H;
   extern std::string C;
   extern std::string A;
   extern std::string B;

   extern std::vector<ParseData> BuildParseStructure(void);

}


#endif  // DIRICHLETCONSTANTS_H
