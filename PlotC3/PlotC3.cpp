// PlotC3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "C3Plot.h"
#include "LRL_ToString.h"
#include "LRL_DataToSVG.h"
#include "LRL_ReadLatticeData.h"
#include "S6.h"
#include "LRL_Vector3.h"

#include <algorithm>
#include <cmath>
#include <utility>
#include <iostream>
#include <string>
#include <vector>


std::string PlotC3(const int wx, const int wy, const std::string& s) {
   //<g transform = "translate(150,975) scale(1 -1)">
   const std::string x1 = LRL_DataToSVG_ToQuotedString(-0);
   const std::string y1 = LRL_DataToSVG_ToQuotedString(0);
   const std::string x2 = LRL_DataToSVG_ToQuotedString(0);
   const std::string y2 = LRL_DataToSVG_ToQuotedString(-wy);
   const std::string x3 = LRL_DataToSVG_ToQuotedString(-wx);
   const std::string y3 = LRL_DataToSVG_ToQuotedString(0);

   const std::string line1 = "\n<line x1=" + x1 + " y1 = " + y1 + " x2 =" + x2
      + " y2 = " + y2 + " stroke-width = \"2\" stroke=\"black\" > </line>";
   const std::string line2 = "\n<line x1=" + x1 + " y1 = " + y1 + " x2 =" + x3
      + " y2 = " + y3 + " stroke-width = \"2\" stroke=\"black\" > </line>";

   const std::string plotc3 = LRL_DataToSVG("<g transform = \"translate( ", wx, ",", wy, ")\">\n",
      line1 + "\n" +
      line2, "\n\n") + s;

   return plotc3;;
}

std::string WrapSVG(const std::string& fileName,
   const int wx = 2000, const int wy = 2000, const std::string& s = "") {
   const std::string swx = LRL_DataToSVG_ToQuotedString(wx);
   const std::string swy = LRL_DataToSVG_ToQuotedString(wy);

   const std::string testtext = "<text x = "+swx+ " y="+swy + "> . </text>\n";

      return    "\n\n" + s + testtext + "</g>";
}

std::vector<S6> PrepareCells() {
   std::vector<S6> v;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      v.push_back(S6(inputList[i].GetCell()));
   }
   return v;
}

size_t scalar = 19191;

int main(int argc, char* argv[])
{
   if (argc != 2) {
      std::cout << " input param must be which scalar (1..3) you want to plot";
      exit(0);
   }
   else {
      scalar = atoi(argv[1]);
   }



   C3Plot c3plot(2000, 2000, 500, 500);
   const std::vector<S6> v = PrepareCells();
   //const double cellScale = c3plot.CellScale(v);

   const std::vector<S6> vv = c3plot.FindNearestReflection(v);

   std::string line;
   std::string cells;

      line += c3plot.CreatePolylineFromPoints(scalar, "2", vv);
      cells += c3plot.DrawCells(scalar, vv);

   std::cout << c3plot.GetIntro() << std::endl;

   const std::string plotc3 = PlotC3(500, 500, line + "  " + cells);
   std::cout << WrapSVG("xxx", c3plot.GetGx(), c3plot.GetGy(), plotc3) << std::endl;

   std::cout << c3plot.GetFoot() << std::endl;
}

