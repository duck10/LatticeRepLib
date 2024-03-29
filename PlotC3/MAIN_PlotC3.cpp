// PlotC3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "C3Plot.h"
#include "ColorTables.h"
#include "GetDate.h"
#include "LRL_ToString.h"
#include "LRL_DataToSVG.h"
#include "LRL_CreateFileName.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "S6.h"
#include "LRL_Vector3.h"

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

static std::string PlotC3( const size_t whichPlot, const int wx, const int wy, const std::string& s) {
   const std::string sScaler = LRL_ToString(whichPlot);
   const std::string sScalerP3 = LRL_ToString(whichPlot+3);
   //<g transform = "translate(150,975) scale(1 -1)">
   const std::string x1 = LRL_DataToSVG_ToQuotedString(-0);
   const std::string y1 = LRL_DataToSVG_ToQuotedString(0);
   const std::string x2 = LRL_DataToSVG_ToQuotedString(0);
   const std::string y2 = LRL_DataToSVG_ToQuotedString(-std::min(wx, wy));
   const std::string x3 = LRL_DataToSVG_ToQuotedString(-std::min(wx,wy));
   const std::string y3 = LRL_DataToSVG_ToQuotedString(0);

   const std::string line1 = "\n<line x1=" + x1 + " y1 = " + y1 + " x2 =" + x2
      + " y2 = " + y2 + " stroke-width = \"2\" stroke=\"black\" > </line>";
   const std::string line2 = "\n<line x1=" + x1 + " y1 = " + y1 + " x2 =" + x3
      + " y2 = " + y3 + " stroke-width = \"2\" stroke=\"black\" > </line>";

   std::string plotc3 = LRL_DataToSVG("<g transform = \"translate( ", wx, ",", wy, ")\">\n",
      line1 + "\n" +
      line2, "\n\n") + s;

   const std::string strScalar = LRL_DataToSVG(whichPlot);

   plotc3 += "<!--plot labeling-->"
      "\n"
      "<g transform = \"translate(0,-265)\" >\n"
      "<text x = \"-350\" y = \"-200\"  font-size = \"40\" font-family = \"Arial, Helvetica, sans-serif\" >Coordinate c</text>\n"
      "<g transform = \"translate(205,10)\">\n"
      "<text x = \"-325\" y = \"-200\"  font-size = \"30\" font-family = \"Arial, Helvetica, sans-serif\" >" + strScalar + " </text >\n"
      "</g>\n"
      "</g>\n"


      "<g transform = \"translate(0,0)\" >\n"
      "<text   x = \"-480\" y = \"-20\"  font-size = \"40\" font-family = \"Arial, Helvetica, sans-serif\" > -s </text>\n"
      "<g transform = \"translate(30,10)\">\n"
      "<text  x = \"-480\" y = \"-20\"  font-size = \"30\" font-family = \"Arial, Helvetica, sans-serif\" >"+sScaler+"</text >\n"
      "</g>\n"
      "</g>\n"

      "<g transform = \"translate(0,0)\" >\n"
      "<text  x = \"-60\" y = \"-450\" font-size = \"40\" font-family = \"Arial, Helvetica, sans-serif\" > -s </text>\n"
      "<g transform = \"translate(30,10)\">\n"
      "<text x = \"-60\" y = \"-450\"  font-size = \"30\" font-family = \"Arial, Helvetica, sans-serif\" >"+sScalerP3+"</text >\n"
      "</g>\n"
      "</g>\n"
   "</g>\n";

   return plotc3;;
}

static std::string WrapSVG(const std::string& fileName,
   const int wx = 1400, const int wy = 600, const std::string& s = "") {
   const std::string swx = LRL_DataToSVG_ToQuotedString(wx);
   const std::string swy = LRL_DataToSVG_ToQuotedString(wy);


   return    "\n\n" + s + "." + "</g>";
}

static void ListInput(const std::vector<LRL_ReadLatticeData>& inputList) {
   for (size_t i = 0; i < std::min(size_t(5), inputList.size()); ++i) {
      std::cout << inputList[i].GetLattice() << "  " << LRL_Cell_Degrees(inputList[i].GetCell()) << std::endl;
   }

   if (inputList.size() > 5) {
      std::cout << " ..." << std::endl;
      for (size_t i = inputList.size() - 3; i < inputList.size(); ++i) {
         std::cout << inputList[i].GetLattice() << "  " << LRL_Cell_Degrees(inputList[i].GetCell()) << std::endl;
      }

   }
}

static std::vector<S6> ConvertInputToS6(const std::vector<LRL_ReadLatticeData>& inputList) {
   std::vector<S6> v;

   for (size_t i = 0; i < inputList.size(); ++i) {
      v.push_back(S6(inputList[i].GetCell()));
   }
   return v;
}

static std::string CellPrecision2(const S6& s) {
   std::stringstream os;
   const LRL_Cell_Degrees cell(s);
   os << std::fixed << std::setprecision(2);
   for (size_t i = 0; i < 6; ++i) {
      os << cell[i] << "  ";
   }
   return os.str();
}

static std::string PrepareLegend(const double x, const double y, const std::vector<S6>& v) {

   //out += commandLine;
   std::string out;

   const std::string count =  "; Number of points: " + LRL_ToString(v.size()) + "\n\n";

   out += count;

   if (v.empty()) {
      std::cout << "; PlotC3 requires at least one lattice" << std::endl;
   }
   else if (v.size() < 5) {
      // just list them all
      for (size_t i = 0; i < v.size(); ++i) {
         const std::string cellText =  "P " + CellPrecision2(v[i]) + "\n";
         out += cellText;
      }
   }
   else {
      // just list the first 3 and the last one
      for (size_t i = 0; i < 3; ++i) {
         const std::string cellText1 = "P " + CellPrecision2(v[i]);

            out += cellText1 + "\n";
      }
      std::stringstream os;
      os << CellPrecision2(v[v.size() - 1]);

      const std::string dots = "\n ...\n\n";

      const std::string cellText2 = "  P " + os.str() + "\n";
      out += dots;
      out += cellText2;
   }
   const std::string ypos = LRL_ToString(y + 3400);
   const std::string commandLine = LRL_ToString("\n edit SVG file to insert command line text\n");

   return out;
}

static std::pair<double, double> GetMinMaxS6(const std::vector<S6>& v) {
   double mins6 = DBL_MAX;
   double maxs6 = -DBL_MAX;

   for (size_t i = 0; i < v.size(); ++i) {
      const S6& vv = v[i];
      mins6 = minNC(mins6, vv[0], vv[1], vv[2], vv[3], vv[4], vv[5]);
      maxs6 = maxNC(maxs6, vv[0], vv[1], vv[2], vv[3], vv[4], vv[5]);
   }
   return { mins6,maxs6 };
}

static std::string AddTextAtBottom(const int x, const int y, const std::string& dataRange) {
   std::string s = "<text x = \"" + LRL_DataToSVG(x) + "\" y = \"" + LRL_DataToSVG(y) + "\""
      " font-size = \"20\" " +
      " font-family = \"Arial, Helvetica, sans-serif \">" + LRL_DataToSVG(dataRange) + "</text>\n";
   //
   //
   // the next line has a blank. It is a place to add a comment such as the command line to generate the plot
   //
   //
   s += "<!--#######################################################-->\n"
      "<text x = \"" + LRL_DataToSVG(x) + "\" y = \"" + LRL_DataToSVG(y + 80) + "\""
      " font-size = \"20\" " +
      " font-family = \"Arial, Helvetica, sans-serif \">\n" +
      ""  // INSERT TEXT HERE
      + " \n</text>   "
      "<!-- add a comment such as the command line-->\n"
      "<!--#######################################################-->\n";

   std::ostringstream os;
   os << "<text x = \""
      << x
      << "\" y = \""
      << y + 40
      << "\"  font-size = \"20\" font-family = \"Arial, Helvetica, sans-serif\" >LRL-WEB  PlotC3   "
      << GetDate()
      << " </text>\n";
   s += os.str();;
   return s;
}

std::string  PrepareColorGuide(const C3Plot& c3plot, const int xint, const int yint) {
   unsigned long r;
   unsigned long g;
   unsigned long b;
   const size_t nguides = 9;
   std::string out;

   for ( size_t i=0; i<=nguides; ++i ) 
   {
      const double frac = double(i) / double(nguides-1);
      c3plot.GetColorRange().GetRGBFromRangeFraction(frac, r, g, b);
      const std::string circle = "";
      const std::string x = LRL_ToString(xint + i * 15);
      const std::string y = LRL_ToString(yint);
      const std::string s = std::string("<circle  r=\"12\" stroke = \"black\" stroke-width=\"0.5\""
         " fill= \"rgb(" + LRL_ToString(r) + ", " + LRL_ToString(g) + ", " + LRL_ToString(b) + ")\""
         " cx=\"" + x + "\""
         " cy=\"" + y + "\"/>\n");
      out += s;
   }
   return out;
}

static std::string  StringFinder(const std::string& toFind, const std::string& defaultvalue, const std::vector<std::string>& args) {
   const auto f = std::find(args.begin(), args.end(), toFind);
   if (f == args.end()) return defaultvalue;
   return *(f + 1);
}

static bool BoolFinder(const std::string& toFind, const bool defaultvalue,  const std::vector<std::string>& args) {
   const auto f = std::find(args.begin(), args.end(), toFind);
   if (f == args.end()) return defaultvalue;
   return *(f + 1) == "true";
}

class WebIO {
public:
   friend std::ostream& operator<< (std::ostream& o, const WebIO& bcf);
   WebIO(int argc, char* argv[], const std::string& progName, const size_t fileCount);
   std::string RecreateFileNames(const std::string& name);
   void GetWebBlockSize(int argc, char* argv[]);

public:
   bool m_hasWebInstructions;
   std::string m_host="";
   std::string m_rawprefix="";
   std::string m_htmlprefix="";
   bool m_usetimestamp = true;
   bool m_usehttps = false;
   bool m_usetarget = true;
   const size_t m_fileNameCount;
   size_t m_blocksize = 20;
   size_t m_blockstart = 0;

};

std::ostream& operator<< (std::ostream& o, const WebIO& w) {
   //o << "; #### Bravais chain failure A\n";
   o << "\n";
   o << "; WebIO data content\n";
   o << ";\thost \t\t" << w.m_host << "\n";
   o << ";\trawprefix \t" << w.m_rawprefix << "\n";
   o << ";\thtmlprefix \t" << w.m_htmlprefix << "\n";
   o << ";\tusetimestamp \t" << w.m_usetimestamp << "\n";
   o << ";\tusehttps \t" << w.m_usehttps << "\n";
   o << ";\tusetarget \t" << w.m_usetarget << "\n";
   o << ";\tfileNameCount \t" << w.m_fileNameCount << "\n";
   o << ";\tblocksize \t" << w.m_blocksize << "\n";
   o << ";\tblockstart \t" << w.m_blockstart << "\n";

   std::cout << std::endl;
   return o;
}


bool containsSubstring(const std::string& str, const std::string& substring) {
   // Use string::find to check if substring exists within str
   return str.find(substring) != std::string::npos;
}

WebIO::WebIO(int argc, char* argv[], const std::string& progName,  const size_t fileCount)
: m_fileNameCount(fileCount)
{
   std::vector<std::string> args;
   for (size_t i = 0; i < argc; ++i) {
      args.emplace_back(argv[i]);
   }

   const std::string searchString("--");
   // Use std::find_if to search for element containing the "--"
   auto it = find_if(args.begin(), args.end(),
      [&searchString](const std::string& element) {
         return containsSubstring(element, searchString); });

   m_hasWebInstructions = it!=args.end();

   m_host = StringFinder("--host", m_host, args);
   m_rawprefix = StringFinder("--rawprefix", m_rawprefix, args);
   m_htmlprefix = StringFinder("--htmlprefix", m_htmlprefix, args);

   m_usetimestamp = BoolFinder("--usetimestamp", m_usetimestamp, args);
   m_usehttps = BoolFinder("--usehttps", m_usehttps, args);
   m_usetarget = BoolFinder("--usetimestamp", m_usetarget, args);

}

void WebIO::GetWebBlockSize(int argc, char* argv[]) {

   std::vector<std::string> args;
   for (size_t i = 0; i < argc; ++i) {
      args.emplace_back(argv[i]);
   }

   m_blockstart = std::stoul(StringFinder("--blockstart", std::to_string(m_blockstart), args));
   m_blocksize = std::stoul(StringFinder("--blocksize", std::to_string(m_blocksize), args));

   if (m_blockstart >= m_fileNameCount) {
      // nothing to do
      m_blockstart = 0;
      m_blocksize = 0;
   }
   else if (m_blockstart + m_blocksize > m_fileNameCount) {
      m_blocksize = m_fileNameCount - m_blockstart;
   }
   else {
      // accept the vaules of blockstart and blocksize
      const int i19191 = 19191;
   }
}

void CompareHtmlFromWebIO(
   const std::string host,
   const std::string rawprefix,
   const std::string htmlprefix,
   const bool usetimestamp,
   const bool usehttps,
   const bool usetarget,
   const WebIO& webio) {
   if (host != webio.m_host) {
      std::cout << " host disagree" << std::endl;
      std::cout << host << "   " << webio.m_host << std::endl;
   }
   if (rawprefix != webio.m_rawprefix) std::cout << " rawprefix disagree" << std::endl;
   if (htmlprefix != webio.m_htmlprefix) std::cout << " htmlprefix disagree" << std::endl;
   if (usetimestamp != webio.m_usetimestamp) std::cout << " usetimestamp disagree" << std::endl;
   if (usehttps != webio.m_usehttps) std::cout << " usehttps disagree" << std::endl;
   if (usetarget != webio.m_usetarget)
   {
      std::cout << " usetarget disagree" << std::endl;
      std::cout << usetarget << "   " << webio.m_usetarget << std::endl;
   }
}

int main(int argc, char* argv[])
{
   std::cout << "; PlotC3" << std::endl;
   std::string host("");
   std::string rawprefix("");
   std::string htmlprefix("");
   bool usetimestamp=true;
   bool usehttps=false;
   bool usetarget=true;

   WebIO webio(argc, argv, "PlotC3", 1);
   webio.GetWebBlockSize(argc, argv);

   int ii;
   for(ii=1;ii<argc;ii++){
       // std::cout << "argv[" << ii <<"] = "<< argv[ii] << std::endl;
       if (std::string(argv[ii]).compare(std::string("--help"))==0) {
           std::cout << "; PlotC3 [--help ] [--host hostname] [--rawprefix rawprefix] [--htmlprefix htmlprefix]" << std::endl;
           std::cout << ";  [--usetimestamp {true|false}] [--usehttps {true|false}] [--usetarget {true|false}]" << std::endl;
       } else if (std::string(argv[ii]).compare(std::string("--host"))==0) {
           host = std::string(argv[++ii]);
            // std::cout << "; host: " << host;
       } else if (std::string(argv[ii]).compare(std::string("--rawprefix"))==0) {
           rawprefix = std::string(argv[++ii]);
            // std::cout << "; rawprefix: " << rawprefix;
       } else if (std::string(argv[ii]).compare(std::string("--htmlprefix"))==0) {
           htmlprefix = std::string(argv[++ii]);
            // std::cout << "; htmlprefix: " << htmlprefix;
       } else if (std::string(argv[ii]).compare(std::string("--usetimestamp"))==0) {
           usetimestamp =  (std::string(argv[++ii])).compare(std::string("true"))==0?true:false;
            // std::cout << "; usetimestamp: " << usetimestamp;
       } else if (std::string(argv[ii]).compare(std::string("--usehttps"))==0) {
          usehttps =  (std::string(argv[++ii])).compare(std::string("true"))==0?true:false;
            // std::cout << "; usehttps: " << usehttps;
       } else if (std::string(argv[ii]).compare(std::string("--usetarget"))==0) {
           usetarget =  (std::string(argv[++ii])).compare(std::string("true"))==0?true:false;
            // std::cout << "; usetarget: " << usetarget;
       } 
   }

   //CompareHtmlFromWebIO(host, rawprefix, htmlprefix, usetimestamp, usehttps, usetarget, webio);


   const std::string filename =
      LRL_CreateFileName::CreateListOfFilenames(1, "PLT", "svg", usetimestamp)[0];

   if(htmlprefix.compare(std::string(""))==0) {
     std::cout << std::string("; Graphical output SVG file = ")
       + rawprefix+filename << std::endl;
   } else {
     if(host.compare(std::string(""))==0) {
       std::cout << std::string("; Graphical output SVG file = <a href=\"")
         +  htmlprefix+filename+std::string("\" target=\"_blank\">")+filename+std::string("</a>") << std::endl;
     } else {
       std::string host_string;
       if (usehttps) {
          host_string=std::string("<a href=\"https://")+host+std::string("/");
       } else {
          host_string=std::string("<a href=\"http://")+host+std::string("/");
       }
       std::cout << std::string("; Graphical output SVG file = ")
         +  host_string+htmlprefix+filename
         + (usetarget?std::string("\" target=\"_blank\" >"):
                  std::string(" >"))
         +filename+std::string("</a>") << std::endl;
     }
   }
  

   C3Plot c3plot(filename, 1400, 600, 500, 500);

   std::string svgOutput;
   const std::string intro = c3plot.GetIntro(filename);
   svgOutput += intro;

   LRL_ReadLatticeData reader;
   const std::vector<LRL_ReadLatticeData> inputList = reader.ReadLatticeData();

   const std::vector<S6> v = ConvertInputToS6(inputList);
   std::pair<double, double> minmax = GetMinMaxS6(v);
   if (abs(minmax.second) < 1.0E-5) minmax.second = 0.0;
   const std::string dataRange = LRL_ToString(" The S6 data range is ", minmax.first, " to ", minmax.second);
   const std::string legend = AddTextAtBottom(350, 550, dataRange) +
      PrepareColorGuide(c3plot, 850, 550);

   svgOutput += legend;

   for (size_t whichPlot = 1; whichPlot < 4; ++whichPlot) {
      const std::string line = c3plot.CreatePolylineFromPoints(whichPlot, ".5", v);

      if (whichPlot == 1)
         svgOutput += PlotC3(whichPlot, 500, 500, line);
      if (whichPlot == 2)
         svgOutput  += PlotC3(whichPlot, 1100, 500, line );
      if (whichPlot == 3)
         svgOutput += PlotC3(whichPlot, 1700, 500, line );
   }

   for (size_t whichPlot = 1; whichPlot < 4; ++whichPlot) {
      const std::string cells = c3plot.DrawCells(whichPlot, v);

      std::string plotc3;
      if (whichPlot == 1)
         plotc3 = PlotC3(whichPlot, 500, 500, /*line +*/ "  " + cells);
      if (whichPlot == 2)
         plotc3 = PlotC3(whichPlot, 1100, 500, /*line +*/ "  " + cells);
      if (whichPlot == 3)
         plotc3 = PlotC3(whichPlot, 1700, 500, /*line +*/ "  " + cells);

      svgOutput += plotc3;
   }

   std::cout << dataRange << std::endl << std::endl;
   ListInput(inputList);
   c3plot.SendFrameToFile(rawprefix+filename, svgOutput + c3plot.GetFoot());
   std::cout << PrepareLegend(600, 600, v);
}
