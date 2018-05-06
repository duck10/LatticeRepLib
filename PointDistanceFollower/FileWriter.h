
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

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void Write(void)
      /*-------------------------------------------------------------------------------------*/
   {
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalWeb)
         return;

      std::ofstream folOut;
      FileOperations::OpenOutputFile(folOut, sFileName.c_str());

      const LRL_Path<S6> pathS6 = m_multiFollow.GetS6();
      const LRL_Path<G6> pathG6 = m_multiFollow.GetG6();
      const LRL_Path<D7> pathD7 = m_multiFollow.GetD7();
      const LRL_Path<S6> pathCS = m_multiFollow.GetCS();
      const std::vector<std::pair<S6, S6> > pointsS6 = pathS6.GetPath();
      const std::vector<std::pair<G6, G6> > pointsG6 = pathG6.GetPath();
      const std::vector<std::pair<D7, D7> > pointsD7 = pathD7.GetPath();
      const std::vector<std::pair<S6, S6> > pointsCS = pathCS.GetPath();

      const std::set<unsigned long> glitches = pathS6.GetGlitches();
      const std::vector<double> distances = pathS6.GetDistances();
      const unsigned long ndist = (unsigned long)(maxNC(pointsS6.size(), pointsG6.size(), pointsD7.size(), pointsCS.size()));
      const std::string nameS6Dist = pointsS6.empty() ? "" : "S6Dist ";
      const std::string nameG6Dist = pointsG6.empty() ? "" : "NCDist ";
      const std::string nameD7Dist = pointsD7.empty() ? "" : "D7Dist ";
      const std::string nameCS6Dist = pointsCS.empty() ? "" : "CS6Dist ";
      const std::string distanceNames = nameS6Dist + nameG6Dist + nameD7Dist + nameCS6Dist;
      if (folOut.is_open() )
      {
         folOut << sFileName << "     Output is S6 probe cell, C3 reduced cell, " << distanceNames << "distance between" << std::endl;
         for (unsigned long counter = 0; counter < ndist; ++counter) {
            const std::string strS6 = (pointsS6.empty()) ? "" : LRL_ToString(pathS6.GetDistances()[counter], " ");
            const std::string strG6 = (pointsG6.empty()) ? "" : LRL_ToString(pathG6.GetDistances()[counter], " ");
            const std::string strD7 = (pointsD7.empty()) ? "" : LRL_ToString(pathD7.GetDistances()[counter], " ");
            const std::string strCS = (pointsCS.empty()) ? "" : LRL_ToString(pathCS.GetDistances()[counter], " ");
            const std::string strDists = strS6 + strG6 + strD7 + strCS;
            std::string glitchLabel;
            if (!glitches.empty()) glitchLabel = glitches.find(counter) != glitches.end() ? "    ***** glitch" : "";
            folOut << counter << "    " <<
               pointsS6[counter].first << "    " << C3(pointsS6[counter].second) <<
               strDists << glitchLabel << std::endl;
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
