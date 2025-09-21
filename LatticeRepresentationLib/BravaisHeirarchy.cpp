#pragma warning (disable: 4100) // Visual Studio --  unreferenced formal parameter

#include "BravaisHeirarchy.h"
#include "DeloneFitResults.h"
//#include "GenerateLatticeTypeExamples.h"
#pragma warning (disable: 4996) // Visual Studio -- localtime may not be safe


#include "GrimmerTree.h"
#include "LRL_ToString.h"
#include "S6.h"
#include "Sella.h"

#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

std::string GetCurrentDate() {
   std::time_t t = std::time(nullptr);
   std::tm* now = std::localtime(&t);
   char buffer[128];
   std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", now);
   return std::string(buffer);
}


std::string createBHFooter(int width) {
   std::stringstream ss;
   ss << "  <text x=\"" << width / 3 << "\" y=\"980\" text-anchor=\"middle\" "
      << "font-family=\"Arial\" font-size=\"12\" fill=\"#666666\">"
      << "Created by SELLA - " <<GetCurrentDate() << "</text>\n";
   return ss.str();
}


std::string BravaisHeirarchy::BoilerPlate_2() {

   return
      createBHFooter(980)+
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
   // Structure to hold SVG positioning and transformation data
   struct SVGElement {
      std::string id;
      int x, y, width, height;
      std::string transform;
      int text_x, text_y;

      SVGElement(const std::string& _id, int _x, int _y, int _w, int _h,
         const std::string& _transform, int _tx, int _ty)
         : id(_id), x(_x), y(_y), width(_w), height(_h),
         transform(_transform), text_x(_tx), text_y(_ty) {
      }
   };

   // Map Bravais lattice types to their SVG element properties
   static const std::map<std::string, SVGElement> elementMap = {
       {"cP", SVGElement("cP", 215, 230, 80, 30, "", 220, 250)},
       {"cF", SVGElement("cF", 365, 230, 80, 30, "", 370, 250)},
       {"cI", SVGElement("cI", 515, 230, 80, 30, "", 520, 250)},
       {"tP", SVGElement("tP", 125, 380, 80, 30, " translate(20 30)  ", 135, 400)},
       {"hP", SVGElement("hP", 285, 380, 80, 30, " translate(-10 10)  ", 300, 400)},
       {"hR", SVGElement("hR", 385, 380, 80, 30, " translate(10 10)  ", 400, 400)},
       {"tI", SVGElement("tI", 535, 380, 80, 30, " translate(20 10)  ", 550, 400)},
       {"oP", SVGElement("oP", 125, 530, 80, 30, " translate(20 30)  ", 135, 550)},
       {"oC", SVGElement("oS", 285, 530, 80, 30, "", 300, 550)},  // Note: uses "oS" as id
       {"os", SVGElement("oS", 285, 530, 80, 30, "", 300, 550)},  // Alternative key for oC
       {"oF", SVGElement("oF", 460, 530, 80, 30, "", 470, 550)},
       {"oI", SVGElement("oI", 535, 530, 80, 30, " translate(40 30)  ", 550, 550)},
       {"mP", SVGElement("mP", 285, 650, 80, 30, " translate(-70 30)  ", 300, 670)},
       {"mC", SVGElement("mS", 435, 650, 80, 30, " translate(90 20)  ", 450, 670)},  // Note: uses "mS" as id
       {"mS", SVGElement("mS", 435, 650, 80, 30, " translate(90 20)  ", 450, 670)}   // Alternative key for mC
   };

   std::string out;

   for (size_t i = 0; i < scores.size(); ++i) {
      const std::pair<std::string, double>& scr = scores[i];
      const std::string strAngstroms = SetWP(sqrt(scr.second));

      auto it = elementMap.find(scr.first);
      if (it != elementMap.end()) {
         const SVGElement& elem = it->second;

         if (!elem.transform.empty()) {
            out += "   <g transform=\"" + elem.transform + "\"> ";
         }

         out += "   <rect id=\"" + elem.id + "\" x=\"" + std::to_string(elem.x) +
            "\" y=\"" + std::to_string(elem.y) + "\" width=\"" + std::to_string(elem.width) +
            "\" height=\"" + std::to_string(elem.height) +
            "\" stroke-width=\"2\" stroke=\"orange\" fill=\"white\" />\n";

         out += "   <text id=\"" + elem.id + "\" x=\"" + std::to_string(elem.text_x) +
            "\" y=\"" + std::to_string(elem.text_y) + "\" font-size=\"18\">" +
            strAngstroms + " </text>\n";

         if (!elem.transform.empty()) {
            out += "</g>\n";
         }

         out += "\n";
      }
   }

   return out;
}

std::string BravaisHeirarchy::BoilerPlate_1() {
   return
      "<?xml version=\"1.0\" standalone=\"no\" ?>\n"
      "<!--the width and height set the total pixel output of the image in the browser-->\n"
      "<!--whatever is within the viewbox will be remapped to show in this size-->\n"
      "<svg width=\"1000\" height=\"800\" viewBox=\"0,0, 1000 800\"  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"
      "\n"
      "<g\n"
      "   transform=\" translate(-50, -100)   scale( .8 )\">\n"
      "\n"
      "   <line id=\"cP-tP line\" x1=\"250\" y1=\"300\" x2=\"250\" y2=\"450\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"tP-oP line\" x1=\"250\" y1=\"450\" x2=\"250\" y2=\"600\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"cF-hR line\" x1=\"400\" y1=\"300\" x2=\"400\" y2=\"450\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"cF-tI line\" x1=\"400\" y1=\"300\" x2=\"550\" y2=\"450\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"cI-tI line\" x1=\"550\" y1=\"300\" x2=\"550\" y2=\"450\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"tI-oI line\" x1=\"550\" y1=\"450\" x2=\"550\" y2=\"600\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "\n"
      "\n"
      "   <line id=\"cI-hR line\" x1=\"250\" y1=\"300\" x2=\"400\" y2=\"450\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"cP-hR line\" x1=\"550\" y1=\"300\" x2=\"400\" y2=\"450\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "\n"
      "   <line id=\"hP-oS line\" x1=\"325\" y1=\"450\" x2=\"325\" y2=\"600\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"oS-mP line\" x1=\"325\" y1=\"600\" x2=\"325\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"tP-oS line\" x1=\"250\" y1=\"450\" x2=\"325\" y2=\"600\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"

      "   <line id=\"oS-mS linea\" x1=\"325\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:6\" />\n"
      "   <line id=\"oS-mS lineb\" x1=\"325\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(255,255,255);stroke-width:2\" />\n"

      "   <line id=\"oP-mP linea\" x1=\"250\" y1=\"600\" x2=\"325\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:10\" />\n"
      "   <line id=\"oP-mP lineb\" x1=\"250\" y1=\"600\" x2=\"325\" y2=\"700\" style=\"stroke:rgb(255,255,255); stroke-width:6\" />\n"
      "   <line id=\"oP-mP linec\" x1=\"250\" y1=\"600\" x2=\"325\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:2\" />\n"

      "   <line id=\"hR-mS line\" x1=\"400\" y1=\"450\" x2=\"495\" y2=\"700\" stroke-dasharray=\"10, 9\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"tI-oF line\" x1=\"550\" y1=\"450\" x2=\"495\" y2=\"600\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"

      "   <line id=\"oI-mS linea\" x1=\"550\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:10\" />\n"
      "   <line id=\"oI-mS lineb\" x1=\"550\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(255,255,255);stroke-width:6\" />\n"
      "   <line id=\"oI-mS linec\" x1=\"550\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:2\" />\n"

      "   <line id=\"oF-mS linea\" x1=\"495\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:10\" />\n"
      "   <line id=\"oF-mS lineb\" x1=\"495\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(255,255,255);stroke-width:6\" />\n"
      "   <line id=\"oF-mS linec\" x1=\"495\" y1=\"600\" x2=\"495\" y2=\"700\" style=\"stroke:rgb(0,0,0);stroke-width:2\" />\n"

      "   <line id=\"mp-aP line\" x1=\"323\" y1=\"700\" x2=\"405\" y2=\"800\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"
      "   <line id=\"mS-aP line\" x1=\"495\" y1=\"700\" x2=\"405\" y2=\"800\" style=\"stroke:rgb(0,0,0);stroke-width:3\" />\n"

      //< line x1 = "200" y1 = "500" x2 = "400" y2 = "500" stroke = "black" stroke - width = "10" / >
      //<line x1 = "200" y1 = "500" x2 = "400" y2 = "500" stroke = "white" stroke - width = "6" / >
      //<line x1 = "200" y1 = "500" x2 = "400" y2 = "500" stroke = "black" stroke - width = "2" / >


      //<line x1 = "200" y1 = "500" x2 = "400" y2 = "500" stroke = "black" stroke - width = "6" / >
      //<line x1 = "200" y1 = "500" x2 = "400" y2 = "500" stroke = "white" stroke - width = "4" / >

      "<circle id=\"cP circle\" cx=\"250\" cy=\"300\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"cF circle\" cx=\"400\" cy=\"300\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle                  cx=\"550\" cy=\"300\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"

      "<circle id=\"tP circle\" cx=\"250\" cy=\"450\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"hP circle\" cx=\"325\" cy=\"450\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"hR circle\" cx=\"400\" cy=\"450\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"tI circle\" cx=\"550\" cy=\"450\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"

      "<circle id=\"oP circle\" cx=\"250\" cy=\"600\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"oS circle\" cx=\"325\" cy=\"600\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"oF circle\" cx = \"495\" cy=\"600\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"mI circle\" cx=\"550\" cy=\"600\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"

      "<circle id=\"mP circle\" cx=\"323\" cy=\"700\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"
      "<circle id=\"mS circle\" cx=\"495\" cy=\"700\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"

      "<circle id=\"aP circle\" cx=\"405\" cy=\"800\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"

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

      "<text x=\"150\" y=\"940\" font-size=\"20\" >The dashed lines indicate \"conjugate\" relationships between the types.</text>\n"
      ;
}

//std::string FormatCellData(
//   const LRL_ReadLatticeData& input,
//   const S6& reducedCell)
//{
//   std::string inputText;
//   inputText +=
//      "\n;INPUT:\n" +
//      input.GetStrCell() + "\n" +
//      LRL_ToString(";Cell ", LRL_Cell_Degrees(input.GetCell())) + "\n" +
//      LRL_ToString("G6 ", G6((input.GetCell())), "\n") +
//      LRL_ToString("S6 ", S6((input.GetCell())), "\n") +
//      LRL_ToString("C3 ", C3((input.GetCell())), "\n");
//
//   inputText +=
//      "\n;SELLING REDUCED:\n" +
//      LRL_ToString(";Cell ", LRL_Cell_Degrees(reducedCell)) + "\n" +
//      LRL_ToString("G6 ", G6(reducedCell), "\n") +
//      LRL_ToString("S6 ", S6(reducedCell), "\n") +
//      LRL_ToString("C3 ", C3(reducedCell), "\n");
//
//   std::cout << inputText << std::endl;
//   return std::string();
//}

BravaisChainFailures BravaisHeirarchy::CheckOneBravaisChain(
   const std::vector<std::string>& bravaisChain,
   const std::vector<DeloneFitResults>& vDeloneFitResultsForOneLattice,
   std::map<std::string, double>& valueMap,
   std::vector<std::string>& errorList)
{
   std::stringstream sst;
   BravaisChainFailures bcf;

   bool okCheck = true;
   if (vDeloneFitResultsForOneLattice.size() == 1) return bcf;
   for (size_t i = 0; i < bravaisChain.size() - 1; ++i)
   {
      const std::string name0 = (i > 0) ? bravaisChain[i - 1] : "aP";
      const std::string name1 = bravaisChain[i];
      if (name1 == "aP") continue;
      const std::string name2 = bravaisChain[i + 1];
      double value0 = valueMap[name0];
      double value1 = valueMap[name1];
      double value2 = valueMap[name2];
      if (value0 < 1.0E-3) value0 = 0;
      if (value1 < 1.0E-3) value1 = 0;
      if (value2 < 1.0E-3) value2 = 0;
      const std::string error = name1 + name2;
      if ((value2 - value1) < -0.01 &&
         std::find(errorList.begin(), errorList.end(), error) == errorList.end())
      {
         BravaisChainFail bf(std::make_pair(name0, value0), std::make_pair(name1, value1), std::make_pair(name2, value2));
         errorList.emplace_back(error);
         okCheck = false;
         sst << std::endl << ";################ Bravais chain failure  "
            << name0 << " " << value0 << " "
            << name1 << " " << value1 << " "
            << name2 << " " << value2
            << "\n;##  \ts6 " << vDeloneFitResultsForOneLattice[i].GetOriginalInput()
            << "\n;##\tP " << LRL_Cell_Degrees(vDeloneFitResultsForOneLattice[i].GetOriginalInput()) << std::endl;
         bf.SetDescription(sst.str());
         bcf.insert(bf);
         bcf.SetS6(vDeloneFitResultsForOneLattice[i].GetOriginalInput());
         errorList.emplace_back(error);
      }
   }

   return bcf;
}

std::vector<std::string> BravaisHeirarchy::FormatProjectedCells(const std::vector<std::string>& s) {
   std::vector<std::string> strings{ s };

   if (!s.empty())
   {
      std::cout << "; projected best fits ( reported distances (in A^2))" << std::endl;
   }

   return strings;
}

std::string BravaisHeirarchy::ProduceSVG(
   const LatticeCell& input,
   const S6& reducedCell,
   const std::vector<std::pair<std::string, double> >& scores,
   const std::vector<std::string>& projectedCells) {

   const std::string inputText = "<text x=\"175\" y=\"175\" font-size=\"25\" >SELLA RESULTS  (Angstroms)"
      "</text>\n";
   const std::string reduced = "<text x=\"175\" y=\"210\" font-size=\"25\" >      " +
      input.GetInput() + "</text>\n";

   std::string s =
      BravaisHeirarchy::BoilerPlate_1() +
      inputText +
      reduced +
      //FormatCellData(input, reducedCell) +
      BravaisHeirarchy::ScoreLabels(scores);


   if (!projectedCells.empty()) {
      //s += "; projected best fits ( reported distances (in A^2))";
      for (const auto& cell : projectedCells) {
         s += cell;
      }
   }

   s += BravaisHeirarchy::BoilerPlate_2();

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

   for (auto it = msdfr.begin(); it != msdfr.end(); ++it) {
      bravaisMap.insert(make_pair((*it).first, (*it).second.GetDifference().norm()));
   }
   return bravaisMap;
}

std::vector<BravaisChainFailures> BravaisHeirarchy::CheckBravaisChains(const std::vector<DeloneFitResults>& vDeloneFitResultsForOneLattice)
{
   auto fitResults(vDeloneFitResultsForOneLattice);
   std::vector< BravaisChainFailures> outBCF;
   std::vector<std::string> errorList;
   std::map<std::string, double> valueMap = GetBestOfEachBravaisType(vDeloneFitResultsForOneLattice);
   static const std::vector<std::vector<std::string> > bravaisChains = CreateBravaisChains();
   for (size_t i = 0; i < bravaisChains.size() - 1; ++i)
   {
      const auto result = BravaisHeirarchy::CheckOneBravaisChain(bravaisChains[i], vDeloneFitResultsForOneLattice, valueMap, errorList);
      if (!result.empty()) {
         fitResults.emplace_back(result.GetRemediationResult());
         outBCF.emplace_back(result);
      }
   }

   //for ( const auto& out : outBCF)
   //{
   //   std::cout << out << std::endl;  // lca
   //}

   return outBCF;
}

std::map<std::string, DeloneFitResults>  BravaisHeirarchy::CreateMapForBestExamples(
   const std::vector<DeloneFitResults>& vDeloneFitResults)
{
   std::map<std::string, DeloneFitResults>  bravaisMap;

   for (const auto& result : vDeloneFitResults) {
      std::string name = result.GetGeneralType();
      //if (name == "oC") name = "oS";
      const double& delta = result.GetDifference().norm();

      const auto mapElement = bravaisMap.find(name);
      if (mapElement == bravaisMap.end())
         bravaisMap.insert(std::make_pair(name, result));
      else
         if (delta < (*mapElement).second.GetDifference().norm())
            (*mapElement).second = result;
   }
   return bravaisMap;
}

inline int BravaisChainFailures::countC3Zeros(const C3& c, const double upperLimit=1.0E-3) {
   int count = 0;
   if (abs(c[0]) < 1.01 * upperLimit) ++count;
   if (abs(c[1]) < 1.01 * upperLimit) ++count;
   if (abs(c[2]) < 1.01 * upperLimit) ++count;
   return count;
}

inline int BravaisChainFailures::CountS6Zeros(const S6& s6) {
   int count = 0;
   for (size_t i = 0; i < 6; ++i) {
      if (abs(s6[i]) <= 0.0) ++count;
   }
   return count;
}

inline int BravaisChainFailures::CountS6Zeros(const S6& s6, const double upperLimit) {
   int count = 0;
   for (size_t i = 0; i < 6; ++i) {
      if (abs(s6[i]) <= 1.01 * upperLimit) ++count;
   }
   return count;
}

inline int BravaisChainFailures::FindLoneS6Zero(const size_t n1, const size_t n2, const S6& s6, const double upper) {
   for (size_t i = 0; i < 6; ++i) {
      if (i == n1 || i == n2) continue;
      if (abs(s6[i]) < upper)  return int(i);
   }
   return 19191;
}

inline std::vector<BravaisChainFail> BravaisChainFailures::getFailList() const { return m_failList; }
