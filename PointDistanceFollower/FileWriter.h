
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

      const LRL_Path<S6> path = m_multiFollow.GetS6();
      const std::vector<std::pair<S6, S6> > points = path.GetPath();
      const std::set<unsigned long> glitches = path.GetGlitches();
      const std::vector<double> distances = path.GetDistances();
      if (folOut.is_open() && !distances.empty())
      {
         folOut << sFileName << "     Output is probe cell, reduced cell, NCDist distance between" << std::endl;
         for (int counter = 0; counter < points.size(); ++counter) {
            std::string glitchLabel;
            if (!glitches.empty()) glitchLabel = glitches.find(counter) != glitches.end() ? "    ***** glitch" : "";
            folOut << counter << "    " <<
               points[counter].first << "    " << C3(points[counter].second) <<
               distances[counter] << glitchLabel << std::endl;
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
