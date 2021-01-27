#include "DirichletConstants.h"
#include "LRL_StringTools.h"
#include "ReadGlobalData.h"


   int    DirichletConstants::numberOfImages = 12;
   int    DirichletConstants::imageHeightPx = 90;
   int    DirichletConstants::canvas_x_size = 2000;
   int    DirichletConstants::canvas_y_size = 2000;
   double DirichletConstants::rotateX = 25;
   double DirichletConstants::rotateY = 5;
   double DirichletConstants::rotateZ = 0;
   double DirichletConstants::scale = 1;
   std::string DirichletConstants::hiddenLineColor = "lightgray";
   std::string DirichletConstants::note = "\"dummy note\"";
   std::string DirichletConstants::fileNamePrefix = "DC";

   std::vector<std::string> DirichletConstants::cellData = std::vector<std::string>();


   std::string DirichletConstants::S6 = "S6";
   std::string DirichletConstants::G6 = "G6";
   std::string DirichletConstants::D7 = "D7";
   std::string DirichletConstants::C3 = "C3";
   std::string DirichletConstants::P = "P";
   std::string DirichletConstants::I = "I";
   std::string DirichletConstants::F = "F";
   std::string DirichletConstants::R = "R";
   std::string DirichletConstants::H = "H";
   std::string DirichletConstants::C = "C";
   std::string DirichletConstants::A = "A";
   std::string DirichletConstants::B = "B";




   std::string Type(const int i) { return "int"; }
   std::string Type(const double i) { return "double"; }
   std::string Type(const std::string& i) { return "string"; }
   std::string Type(const std::vector<std::string>& i) { return "vector"; }

   template<typename T>
   ParseData  ParseDataT(const std::string& commandName, const T& t) {
      return ParseData(LRL_StringTools::strToupper(commandName), Type(t), (void*)&t);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::vector<ParseData>  DirichletConstants::BuildParseStructure(void) {

      std::vector<ParseData> v;

      v.push_back(ParseDataT("images", DirichletConstants::numberOfImages));
      v.push_back(ParseDataT("xsize", DirichletConstants::canvas_x_size));
      v.push_back(ParseDataT("ysize", DirichletConstants::canvas_y_size));
      v.push_back(ParseDataT("ImageHeight", DirichletConstants::imageHeightPx));
      v.push_back(ParseDataT("xrotate", DirichletConstants::rotateX));
      v.push_back(ParseDataT("yrotate", DirichletConstants::rotateY));
      v.push_back(ParseDataT("zrotate", DirichletConstants::rotateZ));
      v.push_back(ParseDataT("scale", DirichletConstants::scale));
      v.push_back(ParseDataT("hiddencolor", DirichletConstants::hiddenLineColor));
      v.push_back(ParseDataT("note", DirichletConstants::note));
      v.push_back(ParseDataT("FilePrefix", DirichletConstants::fileNamePrefix));

      v.push_back(ParseDataT("CELL", DirichletConstants::cellData));

      v.push_back(ParseDataT("S6", DirichletConstants::S6));
      v.push_back(ParseDataT("G6", DirichletConstants::G6));
      v.push_back(ParseDataT("D7", DirichletConstants::D7));
      v.push_back(ParseDataT("C3", DirichletConstants::C3));
      v.push_back(ParseDataT("P", DirichletConstants::P));
      v.push_back(ParseDataT("I", DirichletConstants::I));
      v.push_back(ParseDataT("F", DirichletConstants::F));
      v.push_back(ParseDataT("R", DirichletConstants::R));
      v.push_back(ParseDataT("H", DirichletConstants::H));
      v.push_back(ParseDataT("C", DirichletConstants::C));
      v.push_back(ParseDataT("A", DirichletConstants::A));
      v.push_back(ParseDataT("B", DirichletConstants::B));


      const int i19191 = 19191;

      return(v);
   }
