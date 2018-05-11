
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
   //FileWriter( const std::string& fileName, const Follow<G6,Mat66> & follow, const std::set<Glitch<G6> >& glitches );

   //void Write( void );
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   FileWriter(const std::string& sFileName, const MultiFollower& mf)
      : sFileName(sFileName)
      , m_multiFollow(mf)
   {   }

   ///*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   //unsigned long FindGlitch(const unsigned long n, const std::set<unsigned long>& glitches)const
   //   /*-------------------------------------------------------------------------------------*/
   //{
   //   const unsigned long nGlitches = (unsigned long)(glitches.size());

   //   if (glitches.empty())
   //   {
   //      return unsigned long();
   //   }
   //   else
   //   {
   //      for (unsigned int i = 0; i<nGlitches; ++i)
   //      {
   //         if (n == glitches[i].GetGlitchElement1().GetPosition())
   //         {
   //            return(glitches[i]);
   //         }
   //      }
   //   }
   //   return(Glitch<TVEC>());
   //}

   ///*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   //std::string OnePosition(const std::list<double>::const_iterator& itDist,
   //   const typename std::vector<TVEC>::const_iterator& itP1,
   //   const typename std::vector<TVEC>::const_iterator& itP2,
   //   const int counter) const {
   //   std::string s =
   //      LRL_ToString(*itP1, "    ", C3(*itP2), "    ")
   //      + LRL_ToString(*itDist, follow.GetBoundaryString(counter));
   //   const Glitch<unsigned long>  singleGlitch = FindGlitch(counter, m_glitches);
   //   if (singleGlitch.GetGlitchElement1().GetPosition() > 0)
   //   {
   //      s += "    ***** glitch";
   //   }
   //   s += "\n";

   //   return(s);
   //}

   void PopulateGlitchStrings(std::vector<std::string>& glitches ) {
      m_multiFollow.GetS6().GetDistances();
      const std::set<unsigned long> glSetS6 = m_multiFollow.DetermineOutliers(m_multiFollow.GetS6().GetDistances());
      const std::set<unsigned long> glSetG6 = m_multiFollow.DetermineOutliers(m_multiFollow.GetG6().GetDistances());
      const std::set<unsigned long> glSetD7 = m_multiFollow.DetermineOutliers(m_multiFollow.GetD7().GetDistances());
      const std::set<unsigned long> glSetCS = m_multiFollow.DetermineOutliers(m_multiFollow.GetCS().GetDistances());


      const unsigned long nS6 = m_multiFollow.GetS6().GetDistances().size();
      const unsigned long nG6 = m_multiFollow.GetG6().GetDistances().size();
      const unsigned long nD7 = m_multiFollow.GetD7().GetDistances().size();
      const unsigned long nCS = m_multiFollow.GetCS().GetDistances().size();
      const unsigned long nmaxDist = maxNC( nS6, nG6, nD7,nCS );

      glitches.resize(nmaxDist, "");
      for ( unsigned long i=0; i<nmaxDist; ++i ) {
         if (i < nS6 && glSetS6.find(i) != glSetS6.end()) glitches[i] = "   glitch";
         if (i < nG6 && glSetG6.find(i) != glSetG6.end()) glitches[i] = "   glitch";
         if (i < nD7 && glSetD7.find(i) != glSetD7.end()) glitches[i] = "   glitch";
         if (i < nCS && glSetCS.find(i) != glSetCS.end()) glitches[i] = "   glitch";
      }

   }
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void Write(void)
      /*-------------------------------------------------------------------------------------*/
   {
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalWeb)
         return;

      std::ofstream folOut;
      FileOperations::OpenOutputFile(folOut, sFileName.c_str());

      LRL_Path<S6> pathS6 = m_multiFollow.GetS6();
      LRL_Path<G6> pathG6 = m_multiFollow.GetG6();
      LRL_Path<D7> pathD7 = m_multiFollow.GetD7();
      LRL_Path<S6> pathCS = m_multiFollow.GetCS();
      std::vector<std::pair<S6, S6> > pointsS6 = pathS6.GetPath();
      std::vector<std::pair<G6, G6> > pointsG6 = pathG6.GetPath();
      std::vector<std::pair<D7, D7> > pointsD7 = pathD7.GetPath();
      std::vector<std::pair<S6, S6> > pointsCS = pathCS.GetPath();

      const std::vector<double> distances = pathS6.GetDistances();
      const unsigned long ndist = (unsigned long)(maxNC(pointsS6.size(), pointsG6.size(), pointsD7.size(), pointsCS.size()));
      const std::string nameS6Dist  = pathS6.GetDistances().empty() ? "" : "S6Dist ";
      const std::string nameG6Dist  = pathG6.GetDistances().empty() ? "" : "NCDist ";
      const std::string nameD7Dist  = pathD7.GetDistances().empty() ? "" : "D7Dist ";
      const std::string nameCS6Dist = pathCS.GetDistances().empty() ? "" : "CS6Dist ";
      const std::string distanceNames = nameS6Dist + nameG6Dist + nameD7Dist + nameCS6Dist;

      const std::vector<double> distancesS6 = pathS6.GetDistances();
      const std::vector<double> distancesG6 = pathG6.GetDistances();
      const std::vector<double> distancesD7 = pathD7.GetDistances();
      const std::vector<double> distancesCS = pathCS.GetDistances();



      if (folOut.is_open() )
      {
         std::vector<std::string> glitches;
         PopulateGlitchStrings(glitches);

         folOut << sFileName << "     Output is S6 probe cell, C3 reduced cell, " << distanceNames << "distance between" << std::endl;
         for (unsigned long counter = 0; counter < ndist; ++counter) {
            const std::string strS6 = (distancesS6.empty()) ? "" : LRL_ToString(distancesS6[counter], " ");
            const std::string strG6 = (distancesG6.empty()) ? "" : LRL_ToString(distancesG6[counter], " ");
            const std::string strD7 = (distancesD7.empty()) ? "" : LRL_ToString(distancesD7[counter], " ");
            const std::string strCS = (distancesCS.empty()) ? "" : LRL_ToString(distancesCS[counter], " ");
            const std::string strDists = strS6 + strG6 + strD7 + strCS;
            folOut << counter << "    " <<
               pointsS6[counter].first << "    " << C3(pointsS6[counter].second) <<
               strDists << glitches[counter] << std::endl;
         }
      }
      else
      {
         std::cout << "Could not open file " << sFileName << " for write" << std::endl;
      }
   }
private:
   const std::string sFileName;
   const MultiFollower& m_multiFollow;
};


#endif
