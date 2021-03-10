#include "DirichletConstants.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "ReadGlobalData.h"

#include <set>


   int    DirichletConstants::numberOfImages = 12;
   int    DirichletConstants::imageHeightPx = 90;
   int    DirichletConstants::yposition = 90;
   int    DirichletConstants::latticeLimit = 1;
   int    DirichletConstants::canvas_x_size = 2000;
   int    DirichletConstants::canvas_y_size = 2000;
   double DirichletConstants::rotateX = 25;
   double DirichletConstants::rotateStereo = 15;
   double DirichletConstants::rotateZ = 0;
   double DirichletConstants::scale = 1;

   bool DirichletConstants::timestamp = true;

   std::string DirichletConstants::hiddenLineColor = LRL_StringTools::strToupper( "lightgray" );
   std::string DirichletConstants::faceColor       = LRL_StringTools::strToupper( "none" );
   std::string DirichletConstants::fileNamePrefix  = LRL_StringTools::strToupper( "DC" );
   std::string DirichletConstants::sellingNiggli   = LRL_StringTools::strToupper( "NIGGLI" );

   std::vector<std::string> DirichletConstants::note;
   std::vector<std::string> DirichletConstants::cellData = std::vector<std::string>();
   std::vector<std::string> DirichletConstants::inputErrors = std::vector<std::string>();


   //std::string DirichletConstants::S6 = "S6";
   //std::string DirichletConstants::G6 = "G6";
   //std::string DirichletConstants::D7 = "D7";
   //std::string DirichletConstants::C3 = "C3";
   //std::string DirichletConstants::P = "P";
   //std::string DirichletConstants::I = "I";
   //std::string DirichletConstants::F = "F";
   //std::string DirichletConstants::R = "R";
   //std::string DirichletConstants::H = "H";
   //std::string DirichletConstants::C = "C";
   //std::string DirichletConstants::A = "A";
   //std::string DirichletConstants::B = "B";
   //std::string DirichletConstants::S = "S";
   //std::string DirichletConstants::G = "G";
   //std::string DirichletConstants::V = "V";
   std::string DirichletConstants::RANDOM = "RANDOM";




   std::string Type(const int i) { return "int"; }
   std::string Type(const bool i) { return "bool"; }
   std::string Type(const double i) { return "double"; }
   std::string Type(const std::string& i) { return "string"; }
   std::string Type(const std::vector<std::string>& i) { return "vectorStr"; }

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
      v.push_back(ParseDataT("latticeLimit", DirichletConstants::latticeLimit));
      v.push_back(ParseDataT("ImageHeight", DirichletConstants::imageHeightPx));
      v.push_back(ParseDataT("xrotate", DirichletConstants::rotateX));
      v.push_back(ParseDataT("yrotate", DirichletConstants::rotateStereo));
      v.push_back(ParseDataT("zrotate", DirichletConstants::rotateZ));
      v.push_back(ParseDataT("scale", DirichletConstants::scale));
      v.push_back(ParseDataT("hiddencolor", DirichletConstants::hiddenLineColor));
      v.push_back(ParseDataT("facecolor", DirichletConstants::faceColor));
      v.push_back(ParseDataT("note", DirichletConstants::note));
      v.push_back(ParseDataT("FilePrefix", DirichletConstants::fileNamePrefix));
      v.push_back(ParseDataT("InputErrors", DirichletConstants::inputErrors));
      v.push_back(ParseDataT("TIMESTAMP", DirichletConstants::timestamp));
      v.push_back(ParseDataT("yposition", DirichletConstants::yposition));

      v.push_back(ParseDataT("selling", DirichletConstants::sellingNiggli));
      v.push_back(ParseDataT("S6", DirichletConstants::cellData));
      v.push_back(ParseDataT("G6", DirichletConstants::cellData));
      v.push_back(ParseDataT("D7", DirichletConstants::cellData));
      v.push_back(ParseDataT("C3", DirichletConstants::cellData));
      v.push_back(ParseDataT("P",  DirichletConstants::cellData));
      v.push_back(ParseDataT("I",  DirichletConstants::cellData));
      v.push_back(ParseDataT("F",  DirichletConstants::cellData));
      v.push_back(ParseDataT("R",  DirichletConstants::cellData));
      v.push_back(ParseDataT("H",  DirichletConstants::cellData));
      v.push_back(ParseDataT("C",  DirichletConstants::cellData));
      v.push_back(ParseDataT("A",  DirichletConstants::cellData));
      v.push_back(ParseDataT("B", DirichletConstants::cellData));
      v.push_back(ParseDataT("S", DirichletConstants::cellData));
      v.push_back(ParseDataT("G", DirichletConstants::cellData));
      v.push_back(ParseDataT("V", DirichletConstants::cellData));
      v.push_back(ParseDataT("RANDOM", DirichletConstants::cellData));

      std::string s;
      for (size_t i = 0; i < v.size(); ++i) {
         if (v[i].GetDataType() == "int")
            s += v[i].GetLabel() + " " + v[i].GetDataType() + " " + LRL_ToString(*((int*)v[i].GetLocation())) + "\n";
         if (v[i].GetDataType() == "double")
            s += v[i].GetLabel() + " " + v[i].GetDataType() + " " + LRL_ToString(*((double*)v[i].GetLocation())) + "\n";
         if (v[i].GetDataType() == "int")
            s += v[i].GetLabel() + " " + v[i].GetDataType() + " " + LRL_ToString(*((int*)v[i].GetLocation())) + "\n";
         if (v[i].GetDataType() == "string")
            s += v[i].GetLabel() + " " + v[i].GetDataType() + " " + LRL_ToString(*((std::string*)v[i].GetLocation())) + "\n";
         if (v[i].GetDataType() == "vectorStr")
            s += v[i].GetLabel() + " " + v[i].GetDataType() + " " + LRL_ToString(*((std::vector<std::string>*)(v[i].GetLocation()))) + "\n";
         const int i19191 = 19191;
      }

      //std::cout << s << std::endl;

      v.push_back(ParseDataT("xsize", DirichletConstants::canvas_x_size));
      return(v);
   }


   std::string DirichletConstants::OutputConstants() {
      const std::vector<ParseData> inputLabels = DirichletConstants::BuildParseStructure();
      const std::set<std::string> labelExclude({"S6","D7", "C3","G6", "P", "I", "F", "R", "H", "C", "A", "B", "C", "S", "G", "V", "RANDOM"});
      std::string s;
      for (size_t i = 0; i < inputLabels.size(); ++i) {
         const ParseData current = inputLabels[i];
         const void* location = current.GetLocation();
         const std::string label = current.GetLabel();
         const std::string dataType = current.GetDataType();
         if (labelExclude.find(current.GetLabel()) != labelExclude.end()) {
            if (current.GetLabel() == "RANDOM")
               s += "CELL INPUT  " + dataType + " " + LRL_ToString(*((std::vector<std::string>*)(location))) + "\n";
         }
         else if (dataType == "int")
            s += label + " " + dataType + " " + LRL_ToString(*((int*)location)) + "\n";
         else if (dataType == "bool")
            s += label + " " + dataType + " " + LRL_ToString(*((bool*)location)) + "\n";
         else if (dataType == "double")
            s += label + " " + dataType + " " + LRL_ToString(*((double*)location)) + "\n";
         else if (dataType == "string")
            s += label + " " + dataType + " " + LRL_ToString(*((std::string*)location)) + "\n";
         else if (dataType == "vectorStr") {
            const std::string temp = LRL_ToString(*((std::vector<std::string>*)(location)));
            const std::string nextAdd = label + " " + dataType + ((temp.empty()) ? "" : "\n\t") + temp + "\n";
            s += nextAdd;
         }
         else 
            throw "UNKNOWN DATA TYPE";
         const int i19191 = 19191;
      }

      std::cout << s << std::endl;
      return s;
   }
