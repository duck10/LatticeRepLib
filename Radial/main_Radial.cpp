
#include <iostream>

#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "G6.h"
#include "LatticeConverter.h"

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"
#include "NCDist.h"
#include "Niggli.h"
#include "Radial.h"
#include "Selling.h"
#include "Vector_2.h"

#include <algorithm>
#include <complex>
#include <string>
#include <vector>
#include "main_Radial.h"

G6 G6Reduce(const std::string& lattice, const LRL_Cell& in) {
   G6 out;
   const G6 gin(in);
   Niggli::Reduce(gin, out);
   return LatticeConverter::NiggliReduceCell("P", in);
}



std::pair<double, double> GetDataMin(const std::vector<Vector_2>& v) {
   std::pair<double, double> p(DBL_MAX, DBL_MAX);
   for (size_t i = 0; i < v.size(); ++i) {
      if (p.first > v[i][0] && p.first > 0) p.first = v[i][0];
      if (p.second > v[i][1] && p.second > 0) p.second = v[i][1];
   }
   return p;
}

std::pair<double, double> GetDataMax(const std::vector<Vector_2>& v) {
   std::pair<double, double> p(-DBL_MAX, -DBL_MAX);
   for (size_t i = 0; i < v.size(); ++i) {
      if (p.first < v[i][0]) p.first = v[i][0];
      if (p.second < v[i][1]) p.second = v[i][1];
   }
   return p;
}

std::vector<Vector_2> Rescale(const std::vector<Vector_2>& vv) {
   std::vector<Vector_2>out;
   const std::pair<double, double> maxes = GetDataMax(vv);
   const std::pair<double, double> mins = GetDataMin(vv);

   for (size_t i = 0; i < vv.size(); ++i) {
      const double x = (vv[i][0] - mins.first) / (maxes.first - mins.first);
      const double y = (vv[i][1] - mins.second) / (maxes.second - mins.second);
      out.emplace_back(x, y);
   }
   return out;
}

std::vector<Vector_2> RotateDataSetToXAxis(const std::vector<Vector_2>& vv) {
   Radial radial;
   const Vector_2 cm = Vector_2::CenterOfMass(vv);
   std::cout << " cm of input " << cm << std::endl;
   const double angle = cm.angle(Vector_2(0.0, 1.0));
   std::cout << "angle of input to x axis " << cm.angleDegrees(Vector_2(1.0, 0.0))
      << " degrees" << std::endl;
   return Vector_2::Rotate(-angle, vv);
}

void TEST( const std::vector<Vector_2>& vv) {
   const std::vector<Vector_2> rotated = RotateDataSetToXAxis(vv);
   const Vector_2 cmRotated = Vector_2::CenterOfMass(rotated);
   std::cout << "cm of rotated " << cmRotated << std::endl;
   std::vector<Vector_2> shifted = Vector_2::ShiftList(-cmRotated, rotated);
   const Vector_2 cmShifted = Vector_2::CenterOfMass(shifted);
   std::cout << "cm of shifted " << cmShifted << std::endl;

   const Matrix_2 inertia = Matrix_2::InertiaTensor(shifted);
   std::cout << "inertia tensor " << inertia << std::endl;
   const Vector_2 eigen1 = Matrix_2::Eigen1(inertia);
   const Vector_2 eigen2{ eigen1[1], eigen1[0] };
   std::cout << "eigenvector of the largest eigenvalue " << eigen1 << std::endl;

   const double maxEigenValue = (inertia * eigen1).norm();
   const double minEigenValue = (inertia * eigen2).norm();
   std::wcout << "eigenvalues " << minEigenValue << " " << maxEigenValue << std::endl;

   for (size_t i = 0; i < shifted.size(); ++i) {
      std::cout << shifted[i][0] * maxEigenValue / minEigenValue << "  " << shifted[i][1]
         << std::endl;
   }

   const std::pair<double, double> maxes = GetDataMax(shifted);
   const std::pair<double, double> mins = GetDataMin(shifted);
   std::cout << "max x,y " << maxes.first << " " << maxes.second << std::endl;
   std::cout << "min x,y " << mins.first << " " << mins.second << std::endl;

// eigen vector of the largest eigenvalue -0.168518 0.985699
   exit(0);
}

std::vector<Vector_2> RotateDataSetToXAxis(std::vector<LRL_ReadLatticeData> inputList) {
   Radial radial;
   std::vector<Vector_2> vd;
   for (size_t i = 0; i < inputList.size(); ++i) {
      const LRL_Cell currentCell = inputList[i].GetCell();
      const double x = radial.XAxisDist(inputList[i].GetLattice(), currentCell);
      const double y = radial.YAxisDist(inputList[i].GetLattice(), currentCell);
      vd.emplace_back(x, y);
   }
   const Vector_2 cm = Vector_2::CenterOfMass(vd);
   std::cout << " cm " << cm << std::endl;
   const double angle = cm.angle(Vector_2(1.0, 0.0));
   //const double angleDegrees = cm.angleDegrees(Vector_2(1.0, 0.0));
   //std::cout << " angle " << angle << "   " << angleDegrees << std::endl;
   //std::cout << "cm of rotated " << Vector_2::CenterOfMass(rotated) << std::endl;
   return Vector_2::Rotate(-angle, vd);
}

std::vector<Vector_2>  RotateRecenterAndRescale(const std::vector<LRL_ReadLatticeData>& inputList)
{
   const std::vector<Vector_2> rotated = RotateDataSetToXAxis(inputList);
   std::cout << "cm of rotated " << Vector_2::CenterOfMass(rotated) << std::endl;


   //for (size_t i = 0; i < rotated.size(); ++i) {
   //   std::cout << rotated[i] << std::endl;
   //}

   const double secondAngle = Vector_2::CenterOfMass(rotated).angle(Vector_2(1.0, 0.0));
   std::cout << "second angle " << secondAngle << std::endl;
   const std::vector<Vector_2> rescaled = Rescale(rotated);

   for (size_t i = 0; i < rotated.size(); ++i) {
      std::cout << rescaled[i] << std::endl;
   }
   std::cout << "third angle " <<
      Vector_2::CenterOfMass(rescaled).angle(Vector_2(1.0, 0.0)) << std::endl;
   std::cout << "    ==============" << std::endl;

   return rescaled;
}

int main(int argc, char* argv[])
{
   Radial radial;
   radial.SetParameters(argc, argv);
   std::cout << "; Radial distances from first cell, radial dist (rd), ncdist (nc)" << std::endl;
   std::cout << "; Parameters: " << radial.GetRadialParameters() << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   radial.SetBaseCell(inputList[0].GetLattice(), inputList[0].GetCell());

   std::cout << "; first input cell  " << LRL_Cell_Degrees(radial.GetBaseCell()) << std::endl;

   const std::vector<Vector_2> vxy = Vector_2::ConvertCellsToXYAndRemoveReferenceCell(radial, inputList);
   TEST(vxy);


   RotateRecenterAndRescale(inputList);
   exit(0); 


   for (size_t i = 0; i < inputList.size(); ++i) {
      const LRL_Cell currentCell = inputList[i].GetCell();
      const double xdist = radial.XAxisDist(inputList[i].GetLattice(), currentCell);
      const double ydist = radial.YAxisDist(inputList[i].GetLattice(), currentCell);

      std::cout << "x/y " << xdist << " " <<
         ydist << "  --  cell " <<
         LRL_Cell_Degrees(currentCell) << std::endl;
   }
}
