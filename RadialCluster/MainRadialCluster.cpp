// RadialCluster.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "dbscan.h"
#include "LLoyd.h"
#include "Radial.h"
#include "TNear.h"
#include "Vector_2.h"

#define MINIMUM_POINTS 4     // minimum number of cluster
#define EPSILON (100)  // distance for clustering, metre^2


std::vector<Vector_2> GetInput() {
   std::string line;
   std::vector<Vector_2> out;

   while (std::cin) {
      std::getline(std::cin, line);
      if (!std::cin || line.empty()) break;
      std::stringstream ss(line);
      double d1;
      double d2;
      std::string temp;
      ss >> temp;
      ss >> d1;
      ss >> d2;
      out.emplace_back(Vector_2(d1, d2));
   }
   return out;
}

std::vector<Centroid> ConvertDataToCentroids(const std::vector<Vector_2>& v) {
   std::vector<Centroid> out;
   for (size_t i = 0; i < v.size(); ++i) {
      out.emplace_back(v[i]);
   }
   return out;
}

std::vector<Point> ConvertXYtoPoint(const std::vector<Vector_2>& vv) {
   std::vector<Point> out;
   for (size_t i = 1; i < vv.size(); ++i) {
      Point p;
      p.x = vv[i][0];
      p.y = vv[i][1];
      p.z = 0.0;
      out.emplace_back(p);
   }
   return out;
}

void printResults(vector<Point>& points, int num_points)
{
   int i = 0;
   printf("Number of points: %u\n"
      " x     y     z     cluster_id\n"
      "-----------------------------\n"
      , num_points);
   while (i < num_points)
   {
      //printf("%5.2lf %5.2lf %5.2lf: %d\n",
      printf("%5.2lf %5.2lf %d\n",
            points[i].x,
         points[i].y, points[i].z,
         points[i].clusterID);
      ++i;
   }
}

int main(int argc, char* argv[])
{
   int nstart = 4;
   int test = 0;
   if (argc > 1) {
      test = atoi(argv[1]);
      if (test != 0) nstart = test;
   }


   Radial radial;
   radial.SetParameters(argc, argv);
   std::cout << "; Radial distances from first cell, radial dist (rd), ncdist (nc)" << std::endl;
   std::cout << "; Parameters: " << radial.GetRadialParameters() << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   radial.SetBaseCell(inputList[0].GetLattice(), inputList[0].GetCell());
   const std::vector<Vector_2> vxy = Vector_2::ConvertCellsToXYAndRemoveReferenceCell(radial, inputList);


   std::vector<Point> dbpoints = ConvertXYtoPoint(vxy);
      // constructor
   DBSCAN ds(MINIMUM_POINTS, EPSILON, dbpoints);

   // main loop
   ds.run();

   // result of DBSCAN algorithm
   printResults(ds.m_points, ds.getTotalPointSize());


   exit(0);
   //std::vector<Vector_2> centroids;
   //for (size_t i = 0; i < 25; ++i) {
   //   std::cout << "====================================================================" << std::endl;
   //   centroids = LloydStep::DoLloydStep(centroids, data);
   //}


}
