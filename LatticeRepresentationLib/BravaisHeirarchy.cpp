#include "BravaisHeirarchy.h"
#include "DeloneFitResults.h"
#include "FileOperations.h"
#include "LRL_CreateFileName.h"
#include "LRL_DataToSVG.h"
#include "LRL_ToString.h"
#include "S6.h"
#include "S6BoundaryTransforms.h"

#include <iomanip>
#include <iostream>

std::string BravaisHeirarchy::BoilerPlate_2() {
   return
      "\n"
      "   </g>\n"
      "\n"
      "   </svg>\n"
      "\n"
      ;
}

std::string SetWP(const double d) {
   std::ostringstream os;
   int precision = 4;
   if (d < 10.0) precision = 3;
   os << std::setw(11) << std::setprecision(precision) << d;
   return os.str();
}

std::string BravaisHeirarchy::ScoreLabels(const std::vector<std::pair<std::string, double> >& scores)
{
   std::string out;

   for (size_t i = 0; i < scores.size(); ++i) {
      const std::pair<std::string, double>& scr = scores[i];
      const std::string strAngstroms = SetWP(sqrt(scr.second));
      if (scr.first == "cP") 
         out +=
         "   <rect id=\"cP\" x=\"215\" y=\"230\" width=\"80\" height=\"30\" stroke-width=\"2\" stroke=\"orange\" fill=\"white\" />\n"
         "   <text id=\"cP\" x=\"220\" y=\"250\" font-size=\"18\">"+
         strAngstroms+
         "</text>\n"
         "\n";


      if (scores[i].first == "cF") out +=
         "   <rect id=\"cF\" x=\"365\" y=\"230\" width=\"80\" height=\"30\" stroke-width=\"2\" stroke=\"orange\" fill=\"white\" />\n"
         "   <text id=\"cF\" x=\"370\" y=\"250\" font-size=\"18\">" +
         strAngstroms +
         "</text>\n"
         "\n";

      if (scores[i].first == "cI") out +=
         "   <rect id=\"cI\" x=\"515\" y=\"230\" width=\"80\" height=\"30\" stroke-width=\"2\" stroke=\"orange\" fill=\"white\" />\n"
         "   <text id=\"cI\" x=\"520\" y=\"250\" font-size=\"18\">" +
         strAngstroms +
         " </text>\n"
         "\n";

      if (scores[i].first == "tP") out +=
         "   <g transform=\" translate(20 30)  \"> "
         "   <rect id=\"tP\" x=\"125\" y=\"380\" width=\"80\" height=\"30\" stroke-width=\"2\" stroke=\"orange\" fill=\"white\" />\n"
         "   <text id=\"tP\" x=\"135\" y=\"400\" font-size=\"18\">" +
         strAngstroms +
         " </text>\n"
         "</g>\n";

      if (scores[i].first == "hP") out +=
         "   <g transform=\" translate(-10 10)  \"> "
         "   <rect id=\"hP\" x=\"285\" y=\"380\" width=\"80\" height=\"30\" stroke-width=\"2\" stroke=\"orange\" fill=\"white\" />\n"
         "   <text id=\"hP\" x=\"300\" y=\"400\" font-size=\"18\">" +
         strAngstroms +
         " </text>\n"
         "</g>\n";

      if (scores[i].first == "hR") out +=
         "   <g transform=\" translate(10 10)  \"> "
         "   <rect id=\"hR\" x=\"385\" y=\"380\" width=\"80\" height=\"30\" stroke-width=\"2\" stroke=\"orange\" fill=\"white\" />\n"
         "   <text id=\"hR\" x=\"400\" y=\"400\" font-size=\"18\">" +
         strAngstroms +
         " </text>\n"
         "</g>\n";

      if (scores[i].first == "tI") out +=
         "   <g transform=\" translate(20 10)  \"> "
         "   <rect id=\"tI\" x=\"535\" y=\"380\" width=\"80\" height=\"30\" stroke-width=\"2\" stroke=\"orange\" fill=\"white\" />\n"
         "   <text id=\"tI\" x=\"550\" y=\"400\" font-size=\"18\">" +
         strAngstroms +
         " </text>\n"
         "</g>\n";

      if (scores[i].first == "oP") out +=
         "   <g transform=\" translate(20 30)  \"> "
         "   <rect id=\"oP\" x=\"125\" y=\"530\" width=\"80\" height=\"30\" stroke-width=\"2\" stroke=\"orange\" fill=\"white\" />\n"
         "   <text id=\"oP\" x=\"135\" y=\"550\" font-size=\"18\">" +
         strAngstroms +
         " </text>\n"
         "</g>\n";

      if (scores[i].first == "oC" || scores[i].first == "os") out +=
          "   <rect id=\"oS\" x=\"285\" y=\"530\" width=\"80\" height=\"30\" stroke-width=\"2\" stroke=\"orange\" fill=\"white\" />\n"
        "   <text id=\"oS\" x=\"300\" y=\"550\" font-size=\"18\">" +
         strAngstroms +
         " </text>\n"
         "\n";

      if (scores[i].first == "oF") out +=
         "   <rect id=\"oF\" x=\"460\" y=\"530\" width=\"80\" height=\"30\" stroke-width=\"2\" stroke=\"orange\" fill=\"white\" />\n"
         "   <text id=\"oF\" x=\"470\" y=\"550\" font-size=\"18\">" +
         strAngstroms +
         " </text>\n"
         "\n";

      if (scores[i].first == "oI") out +=
         "   <g transform=\" translate(40 30)  \"> "
         "   <rect id=\"oI\" x=\"535\" y=\"530\" width=\"80\" height=\"30\" stroke-width=\"2\" stroke=\"orange\" fill=\"white\" />\n"
         "   <text id=\"oI\" x=\"550\" y=\"550\" font-size=\"18\">" +
         strAngstroms +
         " </text>\n"
         "</g>\n"
         "\n"
         "\n";

      if (scores[i].first == "mP") out +=
         "   <g transform=\" translate(-70 30)  \"> "
         "   <rect id=\"mP\" x=\"285\" y=\"650\" width=\"80\" height=\"30\" stroke-width=\"2\" stroke=\"orange\" fill=\"white\" />\n"
         "   <text id=\"mP\" x=\"300\" y=\"670\" font-size=\"18\">" +
         strAngstroms +
         " </text>\n"
         "</g>\n";

      if (scores[i].first == "mC" ||scores[i].first == "mS") out +=
         "   <g transform=\" translate(90 20)  \"> "
         "   <rect id=\"mS\" x=\"435\" y=\"650\" width=\"80\" height=\"30\" stroke-width=\"2\" stroke=\"orange\" fill=\"white\" />\n"
         "   <text id=\"mS\" x=\"450\" y=\"670\" font-size=\"18\">" +
         strAngstroms +
         " </text>\n"
         "</g>\n";

   }
   return out;
}

std::string BravaisHeirarchy::BoilerPlate_1() {
   return
      "<?xml version=\"1.0\" standalone=\"no\" ?>\n"
      "<!--the width and height set the total pixel output of the image in the browser-->\n"
      "<!--whatever is within the viewbox will be remapped to show in this size-->\n"
      "<svg width=\"1000\" height=\"800\" viewBox=\"1 1000 800\"  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"
      "\n"
      "<g\n"
      "   transform=\" translate(-50 -100)   scale( .8 .8)\">\n"
      "\n"
      "   <line id=\"cP-tP\" x1=\"250\" y1=\"300\" x2=\"250\" y2=\"450\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"tP-oP\" x1=\"250\" y1=\"450\" x2=\"250\" y2=\"600\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"cF-hR\" x1=\"400\" y1=\"300\" x2=\"400\" y2=\"450\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"cF-tI\" x1=\"400\" y1=\"300\" x2=\"550\" y2=\"450\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"cI-tI\" x1=\"550\" y1=\"300\" x2=\"550\" y2=\"450\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"tI-oI\" x1=\"550\" y1=\"450\" x2=\"550\" y2=\"600\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "\n"
      "\n"
      "   <line id=\"cI-hR\" x1=\"250\" y1=\"300\" x2=\"400\" y2=\"450\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"cP-hR\" x1=\"550\" y1=\"300\" x2=\"400\" y2=\"450\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "\n"
      "   <line id=\"hP-oS\" x1=\"325\" y1=\"450\" x2=\"325\" y2=\"600\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"oS-mP\" x1=\"325\" y1=\"600\" x2=\"325\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"tP-oS\" x1=\"250\" y1=\"450\" x2=\"325\" y2=\"600\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"

      "   <line id=\"oS-mS\" x1=\"325\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:6\" />\n"
      "   <line id=\"oS-mS\" x1=\"325\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(255,255,255);stroke-width:2\" />\n"

      "   <line id=\"oP-mP\" x1=\"250\" y1=\"600\" x2=\"325\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:10\" />\n"
      "   <line id=\"oP-mP\" x1=\"250\" y1=\"600\" x2=\"325\" y2=\"700\" style=\"stroke:rgb(255,255,255); stroke-width:6\" />\n"
      "   <line id=\"oP-mP\" x1=\"250\" y1=\"600\" x2=\"325\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:2\" />\n"

      "   <line id=\"hR-mS\" x1=\"400\" y1=\"450\" x2=\"495\" y2=\"700\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"tI-oF\" x1=\"550\" y1=\"450\" x2=\"495\" y2=\"600\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"

      "   <line id=\"oI-mS\" x1=\"550\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:10\" />\n"
      "   <line id=\"oI-mS\" x1=\"550\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(255,255,255);stroke-width:6\" />\n"
      "   <line id=\"oI-mS\" x1=\"550\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:2\" />\n"

      "   <line id=\"oF-mS\" x1=\"495\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:10\" />\n"
      "   <line id=\"oF-mS\" x1=\"495\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(255,255,255);stroke-width:6\" />\n"
      "   <line id=\"oF-mS\" x1=\"495\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:2\" />\n"

      "   <line id=\"mp-aP\" x1=\"323\" y1=\"700\" x2=\"405\" y2=\"800\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"mS-aP\" x1=\"495\" y1=\"700\" x2=\"405\" y2=\"800\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"

      //< line x1 = "200" y1 = "500" x2 = "400" y2 = "500" stroke = "black" stroke - width = "10" / >
      //<line x1 = "200" y1 = "500" x2 = "400" y2 = "500" stroke = "white" stroke - width = "6" / >
      //<line x1 = "200" y1 = "500" x2 = "400" y2 = "500" stroke = "black" stroke - width = "2" / >


      //<line x1 = "200" y1 = "500" x2 = "400" y2 = "500" stroke = "black" stroke - width = "6" / >
      //<line x1 = "200" y1 = "500" x2 = "400" y2 = "500" stroke = "white" stroke - width = "4" / >

      "<circle id=\"cP\" cx=\"250\" cy=\"300\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"cF\" cx=\"400\" cy=\"300\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle cx=\"550\" cy=\"300\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"

      "<circle id=\"tP\" cx=\"250\" cy=\"450\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"hP\" cx=\"325\" cy=\"450\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"hR\" cx=\"400\" cy=\"450\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"tI\" cx=\"550\" cy=\"450\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"

      "<circle id=\"oP\" cx=\"250\" cy=\"600\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"oS\" cx=\"325\" cy=\"600\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"oF\" cx = \"495\" cy=\"600\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"mI\" cx=\"550\" cy=\"600\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"

      "<circle id=\"mP\" cx=\"323\" cy=\"700\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"mS\" cx=\"495\" cy=\"700\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"

      "<circle id=\"aP\" cx=\"405\" cy=\"800\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"

      "<text x=\"239\" y=\"307\" font-size=\"25\">cP</text>\n"
      "<text x=\"386\" y=\"308\" font-size=\"25\" >cF</text>\n"
      "<text x=\"537\" y=\"308\" font-size=\"25\" >cI</text>\n"

      "<text x=\"238\" y=\"458\" font-size=\"25\" >tP</text>\n"
      "<text x=\"313\" y=\"458\" font-size=\"25\" >hP</text>\n"
      "<text x=\"386\" y=\"458\" font-size=\"25\" >hR</text>\n"
      "<text x=\"542\" y=\"458\" font-size=\"25\" >tI</text>\n"

      "<text x=\"236\" y=\"608\" font-size=\"25\" >oP</text>\n"
      "<text x=\"310\" y=\"608\" font-size=\"25\" >oS</text>\n"
      "<text x=\"480\" y=\"608\" font-size=\"25\" >oF</text>\n"
      "<text x=\"535\" y=\"608\" font-size=\"25\" >oI</text>\n"

      "<text x=\"307\" y=\"708\" font-size=\"25\" >mP</text>\n"
      "<text x=\"478\" y=\"708\" font-size=\"25\" >mS</text>\n"

      "<text x=\"392\" y=\"808\" font-size=\"25\" >aP</text>\n"


      //Grimmer, Hans. "Partial order among the 14 Bravais 
      //types of lattices: basics and applications." Acta Crystallographica 
      //Section A : Foundations and Advances 71, no. 2 (2015) : 143 - 149.
      "<text x=\"150\" y=\"850\" font-size=\"15\" >Diagram based on:</text>\n"
      "<text x=\"150\" y=\"870\" font-size=\"15\" >Grimmer, Hans. \"Partial order among the 14 Bravais  </text>\n"
      "<text x=\"150\" y=\"890\" font-size=\"15\" >types of lattices: basics and applications.\" Acta Crystallographica </text>\n"
      "<text x=\"150\" y=\"910\" font-size=\"15\" >Section A : Foundations and Advances 71, no. 2 (2015) : 143 - 149.</text>\n"

      "<text x=\"150\" y=\"940\" font-size=\"20\" >The dashed lines indicate \"conjugate\" relatsionships between the types.</text>\n"
      ;
}

void SendSellaToFile(const std::string& s, const size_t ordinal) {
   std::cout << std::endl << ";Send Sella Plot To File " << std::endl;
   //std::string filename = LRL_CreateFileName::Create("SEL_", "svg");
   //std::cout << filename << std::endl;
   std::ofstream fileout;
   int count = 0;

   const std::string suffix = LRL_DataToSVG(ordinal);
   const std::string filename = LRL_CreateFileName::Create("SEL_", suffix, "svg", true);
   std::cout << filename << std::endl;


   //while ( !FileOperations::OpenOutputFile(fileout, filename) ||
   //   (filename == LRL_CreateFileName::Create("SEL_", "svg") && count < 100000))
   //{
   //   filename = LRL_CreateFileName::Create("SEL_", "svg");
   //   //std::cout << filename << "  " << count << "\n";
   //   ++count;
   //}
      FileOperations::OpenOutputFile(fileout, filename);

   if (fileout.is_open())
   {
      fileout.seekp(0);
      fileout << s << std::endl;
   }
   else
      std::cout << "Could not open file " << filename << " for write in SendSellaToFile.h" << std::endl;

   fileout.close();
}

std::string FormatCellData(
   const LRL_ReadLatticeData& input,
   const S6& reducedCell)
{
   std::string inputText;

 //  inputText +=
 //      "<text x=\"700\" y=\"235\" font-size=\"20\" >S6 Length:  "+
	//LRL_ToString(sqrt(reducedCell.norm())) +
 //     " </text>\n";



 //  inputText +=
 //     "<text x=\"700\" y=\"325\" font-size=\"20\" >INPUT:  </text>\n"
 //     "<text x=\"720\" y=\"350\" font-size=\"12\" >" +
 //     input.GetStrCell() + "</text>\n" +
 //     "<text x=\"720\" y=\"375\" font-size=\"12\" >" +
 //     LRL_ToString("Cell ", LRL_Cell_Degrees(input.GetCell())) + "</text>\n" +
 //     "<text x=\"720\" y=\"400\" font-size=\"12\" >" +
 //     LRL_ToString("G6 ", G6(LRL_Cell_Degrees(input.GetCell()))) + "</text>\n" +
 //     "<text x=\"720\" y=\"425\" font-size=\"12\" >" +
 //     LRL_ToString("S6 ",S6(LRL_Cell_Degrees(input.GetCell()))) + "</text>\n";

 //
 //
 //  inputText +=
 //     "<text x=\"700\" y=\"450\" font-size=\"20\" >SELLING REDUCED:  </text>\n"
 //     "<text x=\"720\" y=\"475\" font-size=\"12\" >" +
 //     LRL_ToString("Cell ", LRL_Cell_Degrees(reducedCell)) + "</text>\n" +
 //     "<text x=\"720\" y=\"500\" font-size=\"12\" >" +
 //     LRL_ToString("G6 ", G6((reducedCell))) + "</text>\n" +
 //     "<text x=\"720\" y=\"525\" font-size=\"12\" >" +
 //     LRL_ToString("S6 ", S6((reducedCell))) + "</text>\n";
 //
 //  std::cout << inputText << std::endl;
   inputText +=
      "INPUT:\n" +
      input.GetStrCell() + "\n" +
      LRL_ToString("Cell ", LRL_Cell_Degrees(input.GetCell())) + "\n" +
      LRL_ToString("G6 ", G6(LRL_Cell_Degrees(input.GetCell()))) + "\n" +
      LRL_ToString("S6 ",S6(LRL_Cell_Degrees(input.GetCell()))) + "\n";



   inputText +=
      "\nSELLING REDUCED:\n" +
      LRL_ToString("Cell ", LRL_Cell_Degrees(reducedCell)) + "\n" +
      LRL_ToString("G6 ", G6((reducedCell))) + "\n" +
      LRL_ToString("S6 ", S6((reducedCell))) + "\n";

   std::cout << inputText << std::endl;
   return std::string();
}

bool BravaisHeirarchy::CheckOneBravaisChain(
   const std::vector<std::string>& bravaisChain,
   const std::vector<DeloneFitResults>& v,
   std::map<std::string, double>& valueMap,
   std::vector<std::string>& errorList)
{

   bool okCheck = true;
   for (size_t i = 0; i < bravaisChain.size() - 1; ++i)
   {
      const std::string name1 = bravaisChain[i];
      if (name1 == "aP") continue;
      const std::string name2 = bravaisChain[i + 1];
      double value1 = valueMap[name1];
      double ddddd = sqrt(value1);
      double value2 = valueMap[name2];
      if (value1 < 1.0E-3) value1 = 0;
      if (value2 < 1.0E-3) value2 = 0;
      const std::string error = name1 + name2;
      if ((value2 - value1) < -0.01 &&
         std::find(errorList.begin(), errorList.end(), error) == errorList.end())
      {
         errorList.emplace_back(error);
         okCheck = false;
         std::cout << std::endl << ";################ Bravais chain failure  "
            << name1 << " " << value1 << " "
            << name2 << " " << value2 << "  \ts6 "
            << v[i].GetOriginalInput() << "\tP "
            << LRL_Cell_Degrees(v[i].GetOriginalInput()) << std::endl;
         errorList.emplace_back(error);
      }
   }
   return okCheck;
}


std::string BravaisHeirarchy::ProduceSVG(
   const LRL_ReadLatticeData& input,
   const S6& reducedCell,
   const std::vector<std::pair<std::string, double> >& scores) {

   //std::cout << "enter ProduceSVG" << std::endl;
   const std::string inputText = "<text x=\"175\" y=\"175\" font-size=\"25\" >SELLA RESULTS  (Angstroms)"
      "</text>\n";
   const std::string reduced = "<text x=\"175\" y=\"210\" font-size=\"25\" >      " +
      input.GetStrCell() + "</text>\n";

   std::string s =
      BravaisHeirarchy::BoilerPlate_1() +
      inputText +
      reduced +
      FormatCellData(input, reducedCell) +
      BravaisHeirarchy::ScoreLabels(scores) +
      BravaisHeirarchy::BoilerPlate_2();

   SendSellaToFile(s, input.GetOrdinal());
   return s;
}



std::vector<std::vector<std::string> > BravaisHeirarchy::CreateBravaisChains()
{
   std::vector<std::vector<std::string> > v;
   v.push_back({ "aP", "mP", "oP", "tP", "cP" });
   v.push_back({ "aP", "mP", "oS", "tP", "cP" });
   v.push_back({ "aP", "mS", "oS", "tP", "cP" });
   v.push_back({ "aP", "mS", "hR", "cP" });

   v.push_back({ "aP", "mP", "oS", "hP" });
   v.push_back({ "aP", "mS", "oS", "hP" });

   v.push_back({ "aP", "mS", "hR", "cI" });
   v.push_back({ "aP", "mS", "oF", "tI", "cI" });
   v.push_back({ "aP", "mS", "oI", "tI", "cI" });

   v.push_back({ "aP", "mS", "oI", "tI", "cF" });
   v.push_back({ "aP", "mS", "oF", "tI", "cF" });
   v.push_back({ "aP", "mS", "hR", "cF" });
   return v;
}

std::map<std::string, double> BravaisHeirarchy::GetBestOfEachBravaisType(
   const std::vector<DeloneFitResults>& vDeloneFitResults)
{

   std::map<std::string, DeloneFitResults> msdfr = CreateMapForBestExamples(vDeloneFitResults);
   std::map<std::string, double>  bravaisMap;
   //std::map<std::string, double>  bravaisMap = CreateBasicBravaisMap();

   //for (size_t i = 0; i < vDeloneFitResults.size(); ++i) {
   //   std::string name = vDeloneFitResults[i].GetGeneralType();
   //   //if (name == "oC") name = "oS";
   //   const double& delta = vDeloneFitResults[i].GetDifference().norm();

   //   auto mapElement = bravaisMap.find(name);
   //   if (mapElement == bravaisMap.end()) 
   //      bravaisMap.insert(std::make_pair(name, delta));
   //   else
   //      if (delta < (*mapElement).second)
   //         (*mapElement).second = delta;
   //}

   for (auto it = msdfr.begin(); it != msdfr.end(); ++it) {
      bravaisMap.insert(make_pair((*it).first, (*it).second.GetDifference().norm()));
   }
   return bravaisMap;
}

void AnalyseDeloneFitResultsForBravaisChainErrors
(const std::vector<DeloneFitResults>& v) {

}

bool BravaisHeirarchy::CheckBravaisChains(const std::vector<DeloneFitResults>& v)
{
   //for (size_t i = 0; i < v.size(); ++i)
   //{
   //   std::cout << "CheckBravaisChains  DeloneFitResults= \n" << v[i]
   //      << std::endl;
   //}

   std::vector<std::string> errorList;
   std::map<std::string, double> valueMap = GetBestOfEachBravaisType(v);
   bool okCheck = true;
   static const std::vector<std::vector<std::string> > bravaisChains = CreateBravaisChains();
   for (size_t i = 0; i < bravaisChains.size() - 1; ++i)
   {
      if (!BravaisHeirarchy::CheckOneBravaisChain(bravaisChains[i], v, valueMap, errorList)) {
         okCheck = false;
      }
   }

   return okCheck;
}

std::map<std::string, DeloneFitResults>  BravaisHeirarchy::CreateMapForBestExamples(
   const std::vector<DeloneFitResults>& vDeloneFitResults)
{
   std::map<std::string, DeloneFitResults>  bravaisMap;

   for (size_t i = 0; i < vDeloneFitResults.size(); ++i) {
      std::string name = vDeloneFitResults[i].GetGeneralType();
      //if (name == "oC") name = "oS";
      const double& delta = vDeloneFitResults[i].GetDifference().norm();

      auto mapElement = bravaisMap.find(name);
      if (mapElement == bravaisMap.end())
         bravaisMap.insert(std::make_pair(name, vDeloneFitResults[i]));
      else
         if (delta < (*mapElement).second.GetDifference().norm())
            (*mapElement).second = vDeloneFitResults[i];
   }
   return bravaisMap;
}
