#include "DirichletProgramConstants.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "ParseData.h"
#include <set>


   int    DirichletProgramConstants::numberOfImages = 12;
   int    DirichletProgramConstants::imageHeightPx = 90;
   int    DirichletProgramConstants::yposition = 90;
   int    DirichletProgramConstants::latticeLimit = 1;
   int    DirichletProgramConstants::canvas_x_size = 2000;
   int    DirichletProgramConstants::canvas_y_size = 2000;
   double DirichletProgramConstants::rotateX = 25;
   double DirichletProgramConstants::rotateY = 0;
   double DirichletProgramConstants::rotateZ = 0;
   double DirichletProgramConstants::rotateStereo = 15;
   double DirichletProgramConstants::scale = 1;

   bool DirichletProgramConstants::timestamp = true;

   std::string DirichletProgramConstants::hiddenLineColor = LRL_StringTools::strToupper( "gray" );
   std::string DirichletProgramConstants::faceColor       = LRL_StringTools::strToupper( "cyan" );
   std::string DirichletProgramConstants::fileNamePrefix  = LRL_StringTools::strToupper( "DC" );
   std::string DirichletProgramConstants::sellingNiggli   = LRL_StringTools::strToupper( "NIGGLI" );

   std::vector<std::string> DirichletProgramConstants::note;
   std::vector<std::string> DirichletProgramConstants::cellData = std::vector<std::string>();
   std::vector<std::string> DirichletProgramConstants::inputErrors = std::vector<std::string>();


   //std::string DirichletProgramConstants::S6 = "S6";
   //std::string DirichletProgramConstants::G6 = "G6";
   //std::string DirichletProgramConstants::D7 = "D7";
   //std::string DirichletProgramConstants::C3 = "C3";
   //std::string DirichletProgramConstants::P = "P";
   //std::string DirichletProgramConstants::I = "I";
   //std::string DirichletProgramConstants::F = "F";
   //std::string DirichletProgramConstants::R = "R";
   //std::string DirichletProgramConstants::H = "H";
   //std::string DirichletProgramConstants::C = "C";
   //std::string DirichletProgramConstants::A = "A";
   //std::string DirichletProgramConstants::B = "B";
   //std::string DirichletProgramConstants::S = "S";
   //std::string DirichletProgramConstants::G = "G";
   //std::string DirichletProgramConstants::V = "V";
   std::string DirichletProgramConstants::RANDOM = "RANDOM";




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
   std::vector<ParseData>  DirichletProgramConstants::BuildParseStructure(void) {

      std::vector<ParseData> v;

      v.push_back(ParseDataT("images", DirichletProgramConstants::numberOfImages));
      v.push_back(ParseDataT("xsize", DirichletProgramConstants::canvas_x_size));
      v.push_back(ParseDataT("ysize", DirichletProgramConstants::canvas_y_size));
      v.push_back(ParseDataT("latticeLimit", DirichletProgramConstants::latticeLimit));
      v.push_back(ParseDataT("ImageHeight", DirichletProgramConstants::imageHeightPx));
      v.push_back(ParseDataT("xrotate", DirichletProgramConstants::rotateX));
      v.push_back(ParseDataT("Yrotate", DirichletProgramConstants::rotateY));
      v.push_back(ParseDataT("zrotate", DirichletProgramConstants::rotateZ));
      v.push_back(ParseDataT("rotateStereo", DirichletProgramConstants::rotateStereo));
      v.push_back(ParseDataT("scale", DirichletProgramConstants::scale));
      v.push_back(ParseDataT("hiddencolor", DirichletProgramConstants::hiddenLineColor));
      v.push_back(ParseDataT("facecolor", DirichletProgramConstants::faceColor));
      v.push_back(ParseDataT("note", DirichletProgramConstants::note));
      v.push_back(ParseDataT("FilePrefix", DirichletProgramConstants::fileNamePrefix));
      v.push_back(ParseDataT("InputErrors", DirichletProgramConstants::inputErrors));
      v.push_back(ParseDataT("TIMESTAMP", DirichletProgramConstants::timestamp));
      v.push_back(ParseDataT("yposition", DirichletProgramConstants::yposition));

      v.push_back(ParseDataT("selling", DirichletProgramConstants::sellingNiggli));
      v.push_back(ParseDataT("S6", DirichletProgramConstants::cellData));
      v.push_back(ParseDataT("G6", DirichletProgramConstants::cellData));
      v.push_back(ParseDataT("D7", DirichletProgramConstants::cellData));
      v.push_back(ParseDataT("C3", DirichletProgramConstants::cellData));
      v.push_back(ParseDataT("P",  DirichletProgramConstants::cellData));
      v.push_back(ParseDataT("I",  DirichletProgramConstants::cellData));
      v.push_back(ParseDataT("F",  DirichletProgramConstants::cellData));
      v.push_back(ParseDataT("R",  DirichletProgramConstants::cellData));
      v.push_back(ParseDataT("H",  DirichletProgramConstants::cellData));
      v.push_back(ParseDataT("C",  DirichletProgramConstants::cellData));
      v.push_back(ParseDataT("A",  DirichletProgramConstants::cellData));
      v.push_back(ParseDataT("B", DirichletProgramConstants::cellData));
      v.push_back(ParseDataT("S", DirichletProgramConstants::cellData));
      v.push_back(ParseDataT("G", DirichletProgramConstants::cellData));
      v.push_back(ParseDataT("V", DirichletProgramConstants::cellData));
      v.push_back(ParseDataT("RANDOM", DirichletProgramConstants::cellData));

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

      v.push_back(ParseDataT("xsize", DirichletProgramConstants::canvas_x_size));
      return(v);
   }


   std::string DirichletProgramConstants::OutputConstants() {
      const std::vector<ParseData> inputLabels = DirichletProgramConstants::BuildParseStructure();
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

      //std::cout << s << std::endl;
      return s;
   }
