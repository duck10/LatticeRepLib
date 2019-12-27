
#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <list>
#include <string>

#include "C3.h"
#include "FileOperations.h"
#include "FollowerConstants.h"
#include "G6.h"

#include "LRL_CreateFileName.h"
#include "LRL_Path.h"
#include "LRL_ToString.h"
#include "MultiFollower.h"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>

template<typename TVEC>
class FileWriter
{
public:
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   FileWriter(const std::string& sFileName, const MultiFollower& mf)
      : sFileName(sFileName)
      , m_multiFollow(mf)
   {   }

   std::vector<std::string> PopulateGlitchStrings( void ) {
      m_multiFollow.GetS6().GetDistances();
      const std::set<size_t> glSetS6 = m_multiFollow.DetermineOutliers(m_multiFollow.GetS6().GetDistances());
      const std::set<size_t> glSetG6 = m_multiFollow.DetermineOutliers(m_multiFollow.GetG6().GetDistances());
      const std::set<size_t> glSetD7 = m_multiFollow.DetermineOutliers(m_multiFollow.GetD7().GetDistances());
      const std::set<size_t> glSetCS = m_multiFollow.DetermineOutliers( m_multiFollow.GetCS( ).GetDistances( ) );
      const std::set<size_t> glSetLM = m_multiFollow.DetermineOutliers( m_multiFollow.GetLM( ).GetDistances( ) );


      const size_t nS6 = m_multiFollow.GetS6().GetDistances().size();
      const size_t nG6 = m_multiFollow.GetG6().GetDistances().size();
      const size_t nD7 = m_multiFollow.GetD7().GetDistances().size();
      const size_t nCS = m_multiFollow.GetCS( ).GetDistances( ).size( );
      const size_t nLM = m_multiFollow.GetLM( ).GetDistances( ).size( );
      const size_t nmaxDist = maxNC( nS6, nG6, nD7,nCS, nLM );

      std::vector<std::string> glitches(nmaxDist, "");
      for ( size_t i=0; i<nmaxDist; ++i ) {
         if (i < nS6 && glSetS6.find( i ) != glSetS6.end( )) glitches[i] = "   glitch";
         if (i < nG6 && glSetG6.find( i ) != glSetG6.end( )) glitches[i] = "   glitch";
         if (i < nD7 && glSetD7.find( i ) != glSetD7.end( )) glitches[i] = "   glitch";
         if (i < nCS && glSetCS.find( i ) != glSetCS.end( )) glitches[i] = "   glitch";
         if (i < nLM && glSetLM.find( i ) != glSetLM.end( )) glitches[i] = "   glitch";
      }
      return glitches;
   }

   std::string ConstructListOfNamesOfOutputDistances() {
      const LRL_Path<S6> pathS6 = m_multiFollow.GetS6();
      const LRL_Path<G6> pathG6 = m_multiFollow.GetG6();
      const LRL_Path<D7> pathD7 = m_multiFollow.GetD7();
      const LRL_Path<S6> pathCS = m_multiFollow.GetCS( );
      const LRL_Path<S6> pathLM = m_multiFollow.GetLM( );

      const std::string nameS6Dist = pathS6.GetDistances().empty() ? "" : "S6Dist ";
      const std::string nameG6Dist = pathG6.GetDistances().empty() ? "" : "NCDist ";
      const std::string nameD7Dist = pathD7.GetDistances().empty() ? "" : "D7Dist ";
      const std::string nameCS6Dist = pathCS.GetDistances( ).empty( ) ? "" : "CS6Dist ";
      const std::string nameLMDist = pathLM.GetDistances( ).empty( ) ? "" : "LMDist ";

      return nameS6Dist + nameG6Dist + nameD7Dist + nameCS6Dist + nameLMDist;
   }

   void GetDistancesToVectorsAndPoints(
      std::vector<std::pair<S6, S6> >& pointsS6,
      std::vector<std::pair<G6, G6> >& pointsG6,
      std::vector<std::pair<D7, D7> >& pointsD7,
      std::vector<std::pair<S6, S6> >& pointsCS,
      std::vector<std::pair<S6, S6> >& pointsLM,
      std::vector<double>& distancesS6,
      std::vector<double>& distancesG6,
      std::vector<double>& distancesD7,
      std::vector<double>& distancesCS,
      std::vector<double>& distancesLM)
   {
      LRL_Path<S6> pathS6 = m_multiFollow.GetS6();
      LRL_Path<G6> pathG6 = m_multiFollow.GetG6();
      LRL_Path<D7> pathD7 = m_multiFollow.GetD7();
      LRL_Path<S6> pathCS = m_multiFollow.GetCS( );
      LRL_Path<S6> pathLM = m_multiFollow.GetLM( );

      pointsS6 = pathS6.GetPath();
      pointsG6 = pathG6.GetPath();
      pointsD7 = pathD7.GetPath();
      pointsCS = pathCS.GetPath( );
      pointsLM = pathLM.GetPath( );

      distancesS6 = pathS6.GetDistances();
      distancesG6 = pathG6.GetDistances();
      distancesD7 = pathD7.GetDistances();
      distancesCS = pathCS.GetDistances( );
      distancesLM = pathLM.GetDistances( );
   }

   std::string Header( ) {
      std::string txt;
      txt += LRL_ToString( "PointDistanceFollower  ", __DATE__, "  mode = ", ReadGlobalData::GetFollowerMode(), "\n");
      txt += LRL_ToString(sFileName, "     Output is S6 probe cell, C3 reduced cell, ",
         ConstructListOfNamesOfOutputDistances(), "distance between", "\n");
      txt += LRL_ToString("Compute times (msec)  ");

      const LRL_Path<S6> pathS6 = m_multiFollow.GetS6();
      const LRL_Path<G6> pathG6 = m_multiFollow.GetG6();
      const LRL_Path<D7> pathD7 = m_multiFollow.GetD7();
      const LRL_Path<S6> pathCS = m_multiFollow.GetCS( );
      const LRL_Path<S6> pathLM = m_multiFollow.GetLM( );

      txt += pathS6.GetDistances( ).empty( ) ? "" : LRL_ToString( "   S6Dist  ", pathS6.GetTime2ComputeFrame( ) );
      txt += pathG6.GetDistances( ).empty( ) ? "" : LRL_ToString( "   NCDist  ", pathG6.GetTime2ComputeFrame( ) );
      txt += pathD7.GetDistances( ).empty( ) ? "" : LRL_ToString( "   D7Dist  ", pathD7.GetTime2ComputeFrame( ) );
      txt += pathCS.GetDistances( ).empty( ) ? "" : LRL_ToString( "   CS6Dist ", pathCS.GetTime2ComputeFrame( ) );
      txt += pathLM.GetDistances( ).empty( ) ? "" : LRL_ToString( "   LMDist  ", pathLM.GetTime2ComputeFrame( ) );

      return txt;
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void Write(void)
      /*-------------------------------------------------------------------------------------*/
   {
      std::ofstream folOut;
      FileOperations::OpenOutputFile(folOut, sFileName.c_str());
      if (folOut.is_open())
      {
         std::vector<double> distancesS6;
         std::vector<double> distancesG6;
         std::vector<double> distancesD7;
         std::vector<double> distancesCS;
         std::vector<double> distancesLM;

         std::vector<std::pair<S6, S6> > pointsS6;
         std::vector<std::pair<G6, G6> > pointsG6;
         std::vector<std::pair<D7, D7> > pointsD7;
         std::vector<std::pair<S6, S6> > pointsCS;
         std::vector<std::pair<S6, S6> > pointsLM;

         GetDistancesToVectorsAndPoints(pointsS6, pointsG6, pointsD7, pointsCS, pointsLM, 
            distancesS6, distancesG6, distancesD7, distancesCS, distancesLM );

         std::vector<std::string> glitches = PopulateGlitchStrings();

         folOut << Header() << std::endl;

         folOut << "PointDistanceFollower  " << __DATE__ << "  mode = " << ReadGlobalData::GetFollowerMode() << std::endl;
         folOut << sFileName << "     Output is S6 probe cell, C3 reduced cell, " <<
            ConstructListOfNamesOfOutputDistances() << "distance between" << std::endl;

         glitches.resize( m_multiFollow.size( ) );
         for (size_t counter = 0; counter < m_multiFollow.size( ); ++counter) {
            const std::string strS6 = (distancesS6.empty( )) ? "" : LRL_ToString( distancesS6[counter], " " );
            const std::string strG6 = (distancesG6.empty( )) ? "" : LRL_ToString( distancesG6[counter], " " );
            const std::string strD7 = (distancesD7.empty( )) ? "" : LRL_ToString( distancesD7[counter], " " );
            const std::string strCS = (distancesCS.empty( )) ? "" : LRL_ToString( distancesCS[counter], " " );
            const std::string strLM = (distancesLM.empty( )) ? "" : LRL_ToString( distancesLM[counter], " " );
            std::string strDists = strS6 + strG6 + strD7 + strCS + strLM;
            const std::string strpoint = (pointsS6[counter].first.CountZeros( ) > 4) ? "   invalid   " : LRL_ToString( pointsS6[counter].first );
            const std::string strpointRed = (pointsS6[counter].second.CountZeros( ) > 4) ? "   invalid   " : LRL_ToString( C3( pointsS6[counter].second ) );
            if (strpoint == "   invalid   " || strpointRed == "   invalid   ") {
               strDists = "   invalid   ";
               glitches[counter] = "   invalid   ";
            }
            folOut << counter << "    " <<
               strpoint << "    " << strpointRed << "     " <<
               strDists << glitches[counter] << std::endl;
         }
      } else
      {
         std::cout << "Could not open file " << sFileName << " for write" << std::endl;
      }
   }
private:
   const std::string sFileName;
   const MultiFollower& m_multiFollow;
};


#endif
