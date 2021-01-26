#ifndef DIRICHLETCONSTANTS_H
#define DIRICHLETCONSTANTS_H

#include <string>

/*
PROPOSED CONTROL PARAMETERS
latticelimit (fix the off by one current value)
ROTATE (to be applied before stereo or plotting, may be multiples)
number of images
scale
Delone Type (as opposed to cells) (where does this go: header or cell section?)
unit cells
?? canvas size ??
hide/show hidden lines


   extern int globalG6DataLineStrokeWidth; // px
   extern int globalDeloneDataLineStrokeWidth; // px
   extern int globalDataAxisWidth       ; // px
   extern int globalX_AxisTicMarkLength ; // px
   extern int globalY_AxisTicMarkLength ; // px
};

*/

namespace DirichletConstants {
   extern int    latticeLimit; //ok
   extern int    numberOfImages; //ok
   extern int    imageHeightPx; //ok
   extern int    canvas_x_size; //ok
   extern int    canvas_y_size; //ok
   extern bool   showHiddenLines;
   extern double rotateX; //ok
   extern double rotateY; //ok
   extern double rotateZ;
   extern double scale; //ok
   extern std::string note;
   extern std::string hiddenLineColor;
}


#endif  // DIRICHLETCONSTANTS_H
