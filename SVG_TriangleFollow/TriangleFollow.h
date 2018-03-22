#ifdef _MSC_VER
#define USE_LOCAL_HEADERS
#endif

#ifndef TRIANGLEFOLLOW_H
#define TRIANGLEFOLLOW_H

#include <iomanip>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

#include "LRL_CreateFileName.h"
#include "Delone.h"
#include "S6Dist.h"
#include "D7Dist.h"
#include "GlobalTriangleFollowerConstants.h"
#include "haar.hpp"
#include "ProgressData.h"
#include "SVG_Tools.h"
#include "SVG_FollowTriangle.h"
#include "TriangleFollowTools.h"
#include "triple.h"

template<typename TVEC, typename TREDUCEMETHOD, typename TFOLLOWMETHOD> class SVG_FollowTriangle;

template<typename TVEC, typename TREDUCEMETHOD, typename TFOLLOWMETHOD>
class TriangleFollow
{
public:

   TriangleFollow<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD >(const int seed, const int trialNumber, const int npoints)
      : m_fileNameFragment(LRL_CreateFileName::Create("FT_", ""))
      , m_points(npoints)
      , m_worstViolationFound(-DBL_MAX)
      , m_seed(seed)
      , m_trialNumber(trialNumber)
   {
      SetMinMaxX(1, npoints);
   }

   void clear()
   {
      for (unsigned long i = 0; i < 3; ++i) {
         m_followData[i].clear();
         m_dist23Delta[i].clear();
         m_dist13[i].clear();
         m_dist23[i].clear();
         m_angleDiff[i].clear();
         m_triangleDiff[i].clear();
         m_triangleArea[i].clear();
         m_tanhDist23Delta[i].clear();
         m_dist12[i].clear();
         m_dist13[i].clear();
         m_dist23[i].clear();
         m_normedDist23[i].clear();
         m_followData[i].resize(3);
         m_dist23Delta[i].resize(3);
         m_dist13[i].resize(3);
         m_dist23[i].resize(3);
         m_angleDiff[i].resize(3);
         m_triangleDiff[i].resize(3);
         //m_triangleArea[i].resize(3);
         m_tanhDist23Delta[i].resize(3);
         m_dist12[i].resize(3);
         m_dist13[i].resize(3);
         m_dist23[i].resize(3);
         m_normedDist23[i].resize(3);
      }

   }

   std::string Follow(const TVEC& vin1, const TVEC& vin2, const TVEC& vin3, const double minimumPercentViolationToReport) {
      m_inputVector1 = vin1;
      m_inputVector2 = vin2;
      m_inputVector3 = vin3;

      m_maxRawDistance = ScalingInitialPoint(1.0, vin1, vin2, vin3);

      clear();
      //   SetMinMaxX( 1000, 2000 );
      std::ofstream folOut;
      std::string sFileName(GetFileNameFragment() + "log");
#ifdef DEBUGLOG
      folOut.open(sFileName.c_str(), std::ios::out);

      if (!folOut.is_open()) {
         std::cout << "Could not open file " << sFileName << " for write" << std::endl;
      }
#endif
      std::string svgTriangles;
      bool b = CreateVectorList("first line, point 1 constant, followed by 2,3 ", vin1, vin2, vin3, m_points);

      if (b) {
         const std::string svg1 = SetProgressData(folOut, "blue", m_points, "(first line, point 1 constant, followed by 2,3, blue)", m_dist23Delta[0], m_tanhDist23Delta[0], m_angleDiff[0], m_triangleDiff[0], m_triangleArea[0],
            m_normedDist23[0], m_dist12[0], m_dist13[0], m_dist23[0]);
         svgTriangles += svg1;

         b = CreateVectorList("second line, point 1 constant, followed by 1,2 ", vin3, vin1, vin2, m_points);
         if (b) {
            const std::string  svg2 = SetProgressData(folOut, "orange", m_points, "(second line, point 1 constant, followed by 1,2, orange)", m_dist23Delta[1], m_tanhDist23Delta[1], m_angleDiff[1], m_triangleDiff[1], m_triangleArea[1],
               m_normedDist23[1], m_dist12[1], m_dist13[1], m_dist23[1]);
            svgTriangles += svg2;

            b = CreateVectorList("third line, point 1 constant, followed by3,1", vin2, vin3, vin1, m_points);
            if (b) {
               const std::string svg3 = SetProgressData(folOut, "black",  m_points, "(third line, point 1 constant, followed by3,1, black)", m_dist23Delta[2], m_tanhDist23Delta[2], m_angleDiff[2], m_triangleDiff[2], m_triangleArea[2],
                  m_normedDist23[2], m_dist12[2], m_dist13[2], m_dist23[2]);
               svgTriangles += svg3;

               //ReportTriangleResults(folOut, "Del23", "DELTA DIST23",m_dist23Delta);
               //ReportTriangleResults(folOut, "Angle", "ANGLE DIFFERENCE BETWEEN LARGEST AND SMALLEST",m_angleDiff);
               //ReportTriangleResults(folOut, "Viol", "TRIANGLE VIOLATION PERCENT",m_triangleDiff);
               //ReportTriangleResults(folOut, "Area", "TRIANGLE AREA",m_triangleArea);
               //ReportTriangleResults(folOut, "tanh", "TANH DELTA DIST23",m_tanhDist23Delta);

               const int boxWidth = 1800;
               const int boxHeight = (boxWidth * 1600) / 1800;
               const int border = (boxWidth * 30) / 1800;;


               if (m_worstViolationFound >= minimumPercentViolationToReport) {
                  SVG_FollowTriangle<TVEC, TREDUCEMETHOD, TFOLLOWMETHOD> svg_FT(boxWidth, boxHeight, border, svgTriangles, *this);
                  this->m_OutputFilename = svg_FT.GetOutputFilename();
               }
            }
         }
      }

      return svgTriangles;
   }

   std::string PrepareTrianglesOutput( const std::string& color, const std::string& label, const FollowVectors<TVEC>& vin1,
      const FollowVectors<TVEC>& vin2, const FollowVectors<TVEC>& vin3, 
      const ProgressData<double>& vdist12,
      const ProgressData<double>& vdist13,
      const ProgressData<double>& vdist23 ) {
      std::string svg;
      svg += "  start  " + label + "\n";
      const unsigned long nmax = maxNC(vin1.size(), vin2.size(), vin3.size());
      for (unsigned long i = 0; i < nmax; ++i) {
         const TVEC& v1(vin1[i]);
         const TVEC& v2(vin2[i]);
         const TVEC& v3(vin3[i]);
         svg += LRL_ToString(v1, "\n");
         svg += LRL_ToString(v2, "\n");
         svg += LRL_ToString(v3, "\n");
         const double& dist12 = vdist12[i];
         const double& dist13 = vdist13[i];
         const double& dist23 = vdist23[i];

         svg += LRL_ToString(color, "distances 12,13,23 ", i+1, dist12, dist13, dist23, "\n");
         svg += LRL_ToString("====  ", label, i + 1, "\n");
      }
      svg += LRL_ToString("  end    ", label, "\n\n");

      return svg;
   }

   double ScalingInitialPoint(const double targetScale, const TVEC& v1, const TVEC& v2, const TVEC& v3) {
      const double d12 = TVEC(v1 - v2).norm();
      const double d13 = TVEC(v1 - v3).norm();
      const double d23 = TVEC(v2 - v3).norm();
      const double dmax = maxNC(d12, d13, d23);
      return targetScale / dmax;
   }

   bool CreateVectorList(const std::string& label, const TVEC& vin1, const TVEC& vin2, const TVEC& vin3, const int npoints)
   {
      m_followData[0].clear();
      m_followData[1].clear();
      m_followData[2].clear();
      const double maxd = ScalingInitialPoint(1.0, vin1, vin2, vin3);
      const double minFactor = 0.01;   /// FOR HJB
      const double maxFactor = double(m_maxX - 1) / double(npoints - 1);
      const double smallestFactorAllowed = 0.001 / double(npoints - 1);
      const double deltaFactor = (maxFactor - minFactor) / double(npoints - 1);

      const TVEC centerPoint(vin1);
      const TVEC& v2(vin2);
      const TVEC& v3(vin3);

      S6 reducedCenterPoint;  // used in TREDUCEMETHOD::Reduce
      MatS6 m;
      const bool btemp = TREDUCEMETHOD::Reduce(S6(centerPoint), m, reducedCenterPoint, 0.0);
      double factor = std::max(minFactor, smallestFactorAllowed);

      if (btemp) {
         for (int i = 0; i < npoints; ++i)
         {
            {
               const double factorM = 1.0 - factor;;
               S6 red2, red3;  // used in TREDUCEMETHOD::Reduce
               S6 red2A, red3A;  // used in TREDUCEMETHOD::Reduce
               const bool btemp2 = TREDUCEMETHOD::Reduce(S6(factorM*centerPoint + factor * v2), m, red2, 0.0);
               const bool btemp3 = TREDUCEMETHOD::Reduce(S6(factorM*centerPoint + factor * v3), m, red3, 0.0);
               factor += deltaFactor;
               if (btemp2 && btemp3) {
                  m_followData[0].push_back(TVEC(reducedCenterPoint));;   /// FOR HJB
                  m_followData[1].push_back(TVEC(red2));;   /// FOR HJB
                  m_followData[2].push_back(TVEC(red3));;   /// FOR HJB
               }
            }
         }
      }

      return btemp;
   }

   static double DIST(const D7& v1, const D7& v2) {
      return D7Dist(v1.GetVector().data(), v2.GetVector().data());
   }

   static double DIST(const G6& v1, const G6& v2) {
      return NCDist(v1.data(), v2.data());
   }

   static double DIST(const S6& v1, const S6& v2) {
      S6Dist s6dist(50000.0);
      return s6dist.DistanceBetween(v1, v2);
   }

   static double DIST(const B4& v1, const B4& v2) {
      throw; // not implemented for Bravais tetrahedron
             //double a1[6], a2[6];
             //ProjectorTools::ConvertB4ToArray(v1, a1);
             //ProjectorTools::ConvertB4ToArray(v2, a2);
             //return B4Dist(a1, a2);
      return -19191.;
   }

   void ScalePointsAndStoreDistances(const int npoints,
      ProgressData<double>& vdist12,
      ProgressData<double>& vdist13,
      ProgressData<double>& vdist23) {

      const double d12 = (m_inputVector1 - m_inputVector2).norm();
      const double d13 = (m_inputVector1 - m_inputVector3).norm();
      const double d23 = (m_inputVector2 - m_inputVector3).norm();
      const double sqrtInputArea(SqrtTriangleAreaFromSides(d12, d13, d23));


      for ( unsigned long i=0; i<(unsigned long)(npoints); ++i ) {
         {
            double dist12(DIST(m_followData[0][i], m_followData[1][i]));
            double dist13(DIST(m_followData[0][i], m_followData[2][i]));
            double dist23(DIST(m_followData[1][i], m_followData[2][i]));
            double sqrtArea(SqrtTriangleAreaFromSides(dist12, dist13, dist23));
            dist12 *= sqrtInputArea / sqrtArea;
            dist13 *= sqrtInputArea / sqrtArea; //  !!!!!!!!!!!!!!!!!!!!!!! but followData has not been SCALED !!!!!!!!!!!!!!!!
            dist23 *= sqrtInputArea / sqrtArea;

            vdist12.push_back(dist12);
            vdist13.push_back(dist13);
            vdist23.push_back(dist23);

            m_followData[0][i] *= sqrtInputArea / sqrtArea;
            m_followData[1][i] *= sqrtInputArea / sqrtArea;
            m_followData[2][i] *= sqrtInputArea / sqrtArea;
         }

      }
   }

   std::string SetProgressData(std::ostream& folOut, const std::string& color, const int npoints, const std::string& label,
      ProgressData<double>& dist23Delta,
      ProgressData<double>& tanhdist23Delta,
      ProgressData<double>&  angleDiff,
      ProgressData<double>& triangleDiff,
      ProgressData<double>& triangleArea,
      ProgressData<double>& normedDist23,
      ProgressData<double>& vdist12,
      ProgressData<double>& vdist13,
      ProgressData<double>& vdist23) {

      triangleDiff.clear();
      vdist12.clear();
      vdist13.clear();
      vdist23.clear();

      int worstViolationIndex = -INT_MAX;
      double worstViolation = -DBL_MAX;
      //      const unsigned long dataPointCount = maxNC(m_data.sl1.size(), m_data.sl2.size(), m_data.sl3.size());
      const unsigned long dataPointCount = (unsigned long)maxNC(m_followData[0].size(), m_followData[1].size(), m_followData[2].size());

      ScalePointsAndStoreDistances(npoints, vdist12, vdist13, vdist23);

      for (unsigned long i = 0; i < dataPointCount; ++i)
      {
         const double& dist12(vdist12[i]);
         const double& dist13(vdist13[i]);
         const double& dist23(vdist23[i]);
         const double maxDist = maxNC(dist12, dist13, dist23);
         const double triangleDifference(100.0*(2.0*maxDist - (dist12 + dist13 + dist23)) / maxDist);
         if (triangleDifference > worstViolation) {
            worstViolation = triangleDifference;
            worstViolationIndex = i;
         }

         if (worstViolation > m_worstViolationFound) {
            m_worstViolationFound = worstViolation;
            m_worstVec1 = m_followData[0][worstViolationIndex];
            m_worstVec2 = m_followData[1][worstViolationIndex];
            m_worstVec3 = m_followData[2][worstViolationIndex];
         }

         triangleDiff.push_back(triangleDifference);
      }

      std::string svg;
      svg += PrepareTrianglesOutput(color, label, 
         m_followData[0], m_followData[1], m_followData[2], 
         vdist12, vdist13, vdist23);
      svg += PrepareDistancesOutput(color, vdist12, vdist13, vdist23);

      return svg;
   }

   std::string PrepareDistancesOutput(const std::string& color, 
      const ProgressData<double>& vdist12, 
      const ProgressData<double>& vdist13, 
      const ProgressData<double>& vdist23) {
      std::string svg;
      svg += "  start distances " + color + "\n";
      for ( unsigned long i=0; i<vdist12.size(); ++i ){
         svg += LRL_ToString(color, i + 1, vdist12[i], vdist13[i], vdist23[i], "\n");
      }
      svg += "  end distances " + color + "\n";
      return svg;
   }


   std::string GetFileNameFragment(void) const { return m_fileNameFragment; }
   int GetMinX(void) const { return m_minX; }
   int GetMaxX(void) const { return m_maxX; }

   triple<TVEC, TVEC, TVEC> GetWorstTriple(void) const {
      return triple<TVEC, TVEC, TVEC>(m_worstVec1, m_worstVec2, m_worstVec3);
   }


   std::string GetOutputFilename(void) const {
      return m_OutputFilename;
   }

   double GetMaxRawDistance(void) const { return m_maxRawDistance; }
   int GetSeed(void) const { return m_seed; }
   int GetTrialNumber(void) const { return m_trialNumber; }
   TVEC GetWorstVector1(void) const { return m_worstVec1; }
   TVEC GetWorstVector2(void) const { return m_worstVec2; }
   TVEC GetWorstVector3(void) const { return m_worstVec3; }

   TVEC m_inputVector1;
   TVEC m_inputVector2;
   TVEC m_inputVector3;

   FollowVectors<TVEC>  m_followData[3];
   ProgressData<double> m_normedDist23[3];
   ProgressData<double> m_dist23Delta[3];
   ProgressData<double> m_tanhDist23Delta[3];
   ProgressData<double> m_angleDiff[3];
   ProgressData<double> m_triangleDiff[3];
   ProgressData<double> m_triangleArea[3];
   ProgressData<double> m_dist12[3];
   ProgressData<double> m_dist13[3];
   ProgressData<double> m_dist23[3];

   std::vector<std::string> m_ProblemLog;
   int m_minX;
   int m_maxX;

private:
   const int m_points;
   const std::string m_fileNameFragment;
   double m_worstViolationFound;
   std::string m_OutputFilename;
   double m_maxRawDistance;
   TVEC m_worstVec1;
   TVEC m_worstVec2;
   TVEC m_worstVec3;
   const int m_seed;
   const int m_trialNumber;

public:

   void ReportTriangleResults(std::ostream& folOut, const std::string& prefix, const std::string& label, const ProgressData<double> t[3]) {
      folOut << std::endl << label << std::endl;
      const unsigned long n1 = t[0].size();
      const unsigned long n2 = t[1].size();
      const unsigned long n3 = t[2].size();

      const unsigned long n = maxNC(n3, n1, n2);
      for (unsigned long i = 0; i < n; ++i)
         folOut << prefix << " " << i << "  " <<
         ((i < n1) ? LRL_ToString(t[0][i]) : "XXXX") + "  "
         + ((i < n2) ? LRL_ToString(t[1][i]) : "XXXX") + "  "
         + ((i < n3) ? LRL_ToString(t[2][i]) : "XXXX") << std::endl;
   }

   void SetMinMaxX(const int minX, const int maxX) { m_minX = minX; m_maxX = maxX; }

   void WorstTriangleViolation(const Triple<double>& triangleDiff) {
      const std::vector<double>::const_iterator it1 = std::max_element(triangleDiff.sl1.begin(), triangleDiff.sl1.end());
      const std::vector<double>::const_iterator it2 = std::max_element(triangleDiff.sl2.begin(), triangleDiff.sl2.end());
      const std::vector<double>::const_iterator it3 = std::max_element(triangleDiff.sl3.begin(), triangleDiff.sl3.end());

      std::cout << "worst tri-d 1 " << *it1 << std::endl;
      std::cout << "worst tri-d 2 " << *it2 << std::endl;
      std::cout << "worst tri-d 3 " << *it3 << std::endl;

      if (*it1 > std::max(*it2, *it3)) FindWorstInSeries(triangleDiff.sl1.begin(), it1);
      else if (*it2 > *it3)            FindWorstInSeries(triangleDiff.sl2.begin(), it2);
      else                             FindWorstInSeries(triangleDiff.sl3.begin(), it3);
   }

   void FindWorstInSeries(const std::vector<double>::const_iterator& seriesBegin, const std::vector<double>::const_iterator it) {
      const int n = std::distance(seriesBegin, it);
      std::cout << n << std::endl;
      m_worstVec1 = m_followData[2][n];
      m_worstVec2 = m_followData[2][n];
      m_worstVec3 = m_followData[2][n];
   }

private:

   static std::string PrintHighPrecisionArrayVectorData(const std::string& label, const double ar[]) {
      std::ostringstream ostr;
      ostr << std::setprecision(16) << label << " "
         << ar[0] << " "
         << ar[1] << " "
         << ar[2] << " "
         << ar[3] << " "
         << ar[4] << " "
         << ar[5] << std::endl;
      return ostr.str();
   }

   static double DifferenceOfCosinesOfTwoAngles(const double cos1, const double cos2) {
      const double sinSq1 = 1.0 - cos1*cos1;
      const double sinSq2 = 1.0 - cos2*cos2;
      if ((sinSq1 < 0.0 && sinSq2 < 0.0) ||
         (sinSq1 > 0.0 && sinSq2 > 0.0) ||
         (sinSq1 == 0.0 || sinSq2 == 0.0))
         return std::sqrt(sinSq1*sinSq2) + cos1*cos2;
      else // here, one and only one sinSq is negative, so we're in complex plane
         return std::sqrt(-sinSq1*sinSq2 + cos1*cos2*cos1*cos2);
   }

};

#endif

