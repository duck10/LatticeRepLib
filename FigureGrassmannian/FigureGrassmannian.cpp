// FigureGrassmannian.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_DataToSVG.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_Vector3.h"
#include "SVG_Tools.h"

static const std::string axisWidth("\"18\"");
static const std::string mainArcWidth("\"16\"");
static const std::string secondArcWidth("\"12\"");

static const double radius = 1000.0;
static const double axisLength = 1200.;
const size_t stepCount = 100;

static const double rad = 180.0 / (4.0 * atan(1.0));
const Vector_3 displacement(500, 100, 0);
const double xrotation = -80. / rad;
const double zrotation = 10. / rad;
const double stereoRotation = 0.0 / rad;
const Matrix_3x3 matrixToOrientVectors(
   Vector_3(1, 0, 0).Rotmat(stereoRotation) *
   Vector_3(1, 0, 0).Rotmat(xrotation)* 
   Vector_3(0, 0, 1).Rotmat(zrotation));

std::vector<Vector_3> pdb;


std::vector<std::string> PDB_Circles(const std::vector<Vector_3>& v) {
   static const std::string quote("\"");
   static const double rad(20);
   std::vector<std::string> vs;
   for (size_t i = 0; i < v.size(); ++i) {
      const std::string s = std::string("<circle ") +
         std::string(" cx=") + quote + LRL_ToString(v[i][0]) + quote +
         std::string(" cy=") + quote + LRL_ToString(v[i][1]) + quote +
         std::string(" r=") + quote + LRL_ToString(2) + quote +
         std::string(" color= \"black\" ") +
         " />\n";
      vs.push_back(s);
   }
   return vs;
}

std::vector<Vector_3>  RelineSegment(const Vector_3& u, const Vector_3& v) {

   std::vector<Vector_3> vv;

   const double sizeu = u.norm();

   const Vector_3 stepSize = (v - u) / stepCount;
   for (size_t i = 0; i < stepCount + 1; ++i) {
      const Vector_3 nextpos = u + double(i) * stepSize;
      vv.push_back(sizeu * nextpos / nextpos.norm());
   }
   return vv;
}

std::vector<Vector_3> Reline(const double radius) {
   const Vector_3 a1(radius * Vector_3(1, 0, 0));
   const Vector_3 a2(radius * Vector_3(0, 1, 0));
   const Vector_3 a3(radius * Vector_3(0, 0, 1));

   std::vector<Vector_3> vv;
   const std::vector<Vector_3> v1(RelineSegment(a1, a2));
   const std::vector<Vector_3> v2(RelineSegment(a2, a3));
   const std::vector<Vector_3> v3(RelineSegment(a3, a1));
   vv = v1;
   vv.insert(vv.end(), v2.begin(), v2.end());
   vv.insert(vv.end(), v3.begin(), v3.end());
   return vv;
}

std::string DrawCell(const std::string& color, const Vector_3& vin) {
   const Vector_3 v(matrixToOrientVectors * vin + displacement);
   return
      "<circle fill=\"none\" stroke=\"" + color + "\" stroke-width=" + axisWidth
      + LRL_DataToSVG(" x1=\"", displacement[0], "\"")
      + LRL_DataToSVG(" y1=\"", displacement[1], "\"")
      + LRL_DataToSVG(" x2=\"", v[0], "\"")
      + LRL_DataToSVG(" y2=\"", v[1], "\" ")
      + " />";
}

std::string ToSVGFromOriginToPoint(const std::string& color, const Vector_3& vin) {
   const Vector_3 v(matrixToOrientVectors * vin + displacement);
   return
      "<line fill=\"none\" stroke=\"" + color + "\" stroke-width=" + axisWidth
      + LRL_DataToSVG(" x1=\"", displacement[0], "\"")
      + LRL_DataToSVG(" y1=\"", displacement[1], "\"")
      + LRL_DataToSVG(" x2=\"", v[0], "\"")
      + LRL_DataToSVG(" y2=\"", v[1], "\" ")
      + " />";
}

std::string MakeRotate(const double angle, const double x, const double y) {
   return
      " transform = \"rotate(" + LRL_ToString(angle) + ", " + LRL_ToString(x) + ", " + LRL_ToString(y) + ")\"";
}

std::string ToSVGFromText(const std::string& label, const std::string& color, const Vector_3& vin) {
   const Vector_3 v(matrixToOrientVectors * vin + displacement);

   return
      "<text  stroke= \"" + color + "\" font-family = \"sans-serif\" font-size=\"60\" fill=\""
      +  color + "\" stroke-width=\"3\" " 
      + LRL_DataToSVG(" x=\"", v[0], "\"")
      + LRL_DataToSVG(" y=\"", v[1], "\"")
      + MakeRotate(90,v[0],v[1])
      + " > "
      + label 
      + " </text>";
}

std::vector<std::string> CreateAxisLabels() {
   std::vector<std::string> vsvg;
   const Vector_3 v1(axisLength, 0, 0);
   const Vector_3 v2(0, axisLength, 0);
   const Vector_3 v3(0, 0, axisLength);
   const Vector_3 v4(0, 0, radius);

   vsvg.push_back( ToSVGFromText("s4 = a.d", "black", v1));
   vsvg.push_back( ToSVGFromText("s5 = b.d", "black", v2));
   vsvg.push_back( ToSVGFromText("s6 = c.d", "black", v3));

   return vsvg;
}

std::string CreateAxes() {
   const Matrix_3x3 m = Vector_3(1., 1., 1.).Rotmat(120.0 / rad);

   const Vector_3 v1(axisLength, 0, 0);
   const Vector_3 v2(0, axisLength, 0);
   const Vector_3 v3(0, 0, axisLength);
   const Vector_3 v4(0, 0, radius);

   const std::string s3 = ToSVGFromOriginToPoint("black", v3);
   const std::string s4 = ToSVGFromOriginToPoint("rgb(150, 150, 150)", v4);
   const std::string s5 = ToSVGFromOriginToPoint("black", 0.18 * v4);
   const std::string s1 = ToSVGFromOriginToPoint("black", v1);
   const std::string s2 = ToSVGFromOriginToPoint("black", v2);

   return
      s1 + " \n" +
      s2 + " \n" +
      s3 + " \n" +
      s4 + " \n" +
      s5 + " \n\n";
}

std::string CreatePolylineFromPoints(const std::string& color, const std::string& width, const std::vector<Vector_3>& v) {
   const std::string dashMode("");
   const std::string strokeWidth = "\" stroke-width=" + width + "";
   std::string svg = LRL_DataToSVG("   <polyline fill=\"none\" stroke=\"", color, strokeWidth, "  points=\" ");
   for (size_t i = 0; i < v.size(); ++i) {
      const Vector_3 vd(matrixToOrientVectors * v[i] + displacement);
      svg += LRL_DataToSVG(vd[0]) + " " + LRL_DataToSVG(vd[1]) + ",  ";
   }

   svg += "\"/> \n\n";
   return svg;
}

std::vector<Vector_3> RotateArc(const Matrix_3x3& m, const std::vector<Vector_3>& vin) {
   std::vector<Vector_3> v;

   for (size_t i = 0; i < vin.size(); ++i) {
      v.push_back(m * vin[i]);
   }
   return v;
}

std::string  CreateArcs() {
   const double angle = 90.0 / rad;
   const double step = angle / 100.0;
   const Vector_3 rotationAxis(0., 0., 1.);
   const Matrix_3x3 stepMatrix = rotationAxis.Rotmat(angle / double(stepCount));

   const Vector_3 initialPoint(radius, 0., 0.);
   std::vector<Vector_3> arc_xy(1, initialPoint);
   Vector_3 nextPoint(initialPoint);
   arc_xy.push_back(nextPoint);
   for (size_t i = 0; i < stepCount; ++i) {
      nextPoint = stepMatrix * nextPoint;
      arc_xy.push_back(nextPoint);
   }

   static const Matrix_3x3 m90x(Vector_3(1, 0, 0).Rotmat(90. / rad));
   static const Matrix_3x3 m90y(Vector_3(0, 1, 0).Rotmat(-90. / rad));
   static const Matrix_3x3 m90z(Vector_3(0, 0, 1).Rotmat(0. / rad));
   static const Matrix_3x3 m45x(Vector_3(1, 0, 0).Rotmat(45. / rad));
   static const Matrix_3x3 m45y(Vector_3(0, 1, 0).Rotmat(45. / rad));
   static const Matrix_3x3 m90temp(Vector_3(0, 1, 0).Rotmat(0. / rad));
   static const Matrix_3x3 m45z(Vector_3(0, 0, 1).Rotmat(45. / rad) * m90temp);

   std::vector<Vector_3> arc_yz(RotateArc(m90x, arc_xy));
   std::vector<Vector_3> partial(arc_xy);
   partial.erase(partial.begin(), partial.begin() + stepCount / 1.135);
   std::vector<Vector_3> arc_yz_hidden(RotateArc(m90x, partial));
   std::vector<Vector_3> arc_xz(RotateArc(m90y, arc_xy));

   std::string s;
   //s += CreatePolylineFromPoints("black", mainArcWidth, arc_xy);
   //s += CreatePolylineFromPoints("black", mainArcWidth, arc_yz);
   //s += CreatePolylineFromPoints("black", mainArcWidth, arc_xz);
   std::vector<Vector_3> v = Reline(radius);
   s += CreatePolylineFromPoints("black", mainArcWidth, v);
   s += CreatePolylineFromPoints("rgb(150,150,150)", mainArcWidth, arc_yz_hidden);

   s += CreatePolylineFromPoints("black\" stroke-dasharray = \"20, 10", secondArcWidth, RotateArc(m45x, arc_xy)); //OK
   s += CreatePolylineFromPoints("black\" stroke-dasharray = \"20, 10", secondArcWidth, RotateArc(m45z, arc_yz)); //OK
   s += CreatePolylineFromPoints("black\" stroke-dasharray = \"20, 10", secondArcWidth, RotateArc(m45y, arc_xz));

   return s;
}

std::vector<Vector_3> ScalePoints(const std::vector < Vector_3>& vin) {
   std::vector<Vector_3> vout;
   for (size_t i = 0; i < vin.size(); ++i) {
      const Vector_3 vtemp = -radius * vin[i] / vin[i].Norm();
      const Vector_3 v(matrixToOrientVectors * vtemp + displacement);
      vout.push_back(v);
   }
   return vout;
}

std::string ConcatenateStrings(const std::vector<std::string>& vs) {
   std::string s;
   for (size_t i = 0; i < vs.size(); ++i) {
      s += vs[i];
   }
   return s;
}

std::string CreateCells() {
   const std::vector<Vector_3> scaled(ScalePoints(pdb));
   const std::vector<std::string> vs(PDB_Circles(scaled));
   return (ConcatenateStrings(vs));
}

std::vector<std::string> CreateContent() {

   const std::string s = CreateAxes() +
      "\n\n" + CreateArcs() +
      "\n\n" + CreateCells() + "\n";

   const std::vector<std::string> vs(1, s);
   return vs;
}

std::string MoveAxisLabel(const std::string& s, const std::string& delx, const std::string dely) {
   std::string output;
   output += "<g transform=\"translate(" + delx + ", " + dely + ") scale(1 1) rotate(0)\">\n";
   output += "<g transform=\"translate(0, 2000) scale(1 1) rotate(-90)\">\n";
   output += s;
   output += "</g>\n</g>\n\n";
   return output;
}

std::string TransformText(const std::string s) {
   std::string output;
   output += "\n";
   output += "<g transform=\"translate(0, 0) scale(1 1) rotate(0)\">\n";
   output += "<g transform=\"translate(0, 2000) scale(1 1) rotate(-90)\">\n";
   output += s;
   output += "</g>\n</g>\n";
   return output;
}

std::string TransformOutput(const std::string& s) {
   std::string output;
   output += "<g transform=\"translate(0, 2000) scale(1 1) rotate(-90)\">\n";
   output += s;
   output += "\n</g>\n";
   return output;
}

void ReadTheCellData() {
   // assumes primitive orthorhombic !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//   std::vector<Vector_3> pdb;

   const std::string latticeTypes("aAbBcCiIfFpPhHrR");
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   for (size_t i = 0; i < inputList.size(); ++i) {
      const S6 s6 = inputList[i].GetCell();
      const Vector_3 v3(s6[3],s6[4],s6[5]);
      pdb.push_back(v3);
   }
   const int i19191 = 19191;
}

int main()
{

   ReadTheCellData();


   std::string header = ImageHeader(2000, 2000) + "\n\n";

   std::vector<std::string> content(CreateContent());
   const std::vector<std::string> labels = CreateAxisLabels();

   std::vector<std::string> footer = ImageFooter("");

   std::string output = header + TransformOutput(LRL_StringTools::ConcatanateStrings(content));

   std::vector<std::string> transformedLabels;
   output += MoveAxisLabel(labels[0] + "\n", "20", "20");
   output += MoveAxisLabel(labels[1] + "\n", "20", "20");
   output += MoveAxisLabel(labels[2] + "\n", "-120", "-20");
   output += LRL_StringTools::ConcatanateStrings(footer);

   std::cout << output << std::endl;
}
