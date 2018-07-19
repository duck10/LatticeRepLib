
#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <list>
#include <string>

#include "Follow.h"
#include "Glitch.h"
#include "Mat66.h"
#include "G6.h"

#include "CreateFileName.h"
#include "FollowerConstants.h"
#include "FollowerTools.h"
#include "Mat66.h"
#include "G6.h"
#include "ReportTools.h"
#include "SVG_Writer.h"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>

template<typename TVEC, typename TMAT>
class FileWriter
{
public:
   //FileWriter( const std::string& fileName, const Follow<G6,Mat66> & follow, const std::vector<Glitch<G6> >& glitches );

   //void Write( void );
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   FileWriter(const std::string& sFileName, const Follow<TVEC, TMAT> & follow, const std::vector<Glitch<TVEC> >& glitches)
      : sFileName(sFileName)
      , follow(follow)
      , glitches(glitches)
      , probePath(follow.GetProbeList())
      , secondProbePath(follow.GetSecondProbeList())
      , distances(follow.GetDistances())
      , itDist(distances.begin())
      , itP1(probePath.begin())
      , itP2(secondProbePath.begin())
      , itLastDist(distances.end())
      /*-------------------------------------------------------------------------------------*/
   {
      assert(!follow.empty());
      assert(!probePath.empty());
      assert(probePath.size() == secondProbePath.size());
      assert(probePath.size() == distances.size());
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   Glitch<TVEC> FindGlitch(const int n, const std::vector<Glitch<TVEC> >& glitches)const
      /*-------------------------------------------------------------------------------------*/
   {
      const size_t nGlitches = glitches.size();

      if (glitches.empty())
      {
         return(Glitch<TVEC>());
      }
      else
      {
         for (unsigned int i = 0; i<nGlitches; ++i)
         {
            if (n == glitches[i].GetGlitchElement1().GetPosition())
            {
               return(glitches[i]);
            }
         }
      }
      return(Glitch<TVEC>());
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string OnePosition(const std::list<double>::const_iterator& itDist,
      const typename std::vector<TVEC>::const_iterator& itP1,
      const typename std::vector<TVEC>::const_iterator& itP2,
      const int counter) const {
      std::string s =
         ToString(*itP1, "    ", *itP2, "    ")
         + ToString(*itDist, follow.GetBoundaryString(counter));
      const Glitch<TVEC>  singleGlitch = FindGlitch(counter, glitches);
      if (singleGlitch.GetGlitchElement1().GetPosition() > 0)
      {
         s += "    *****";
      }
      s += "\n";

      return(s);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void Write(void)
      /*-------------------------------------------------------------------------------------*/
   {
      if (FollowerConstants::globalFollowerMode == FollowerConstants::globalWeb)
         return;

      std::ofstream folOut;
      FollowerTools::OpenOutputFile(folOut, sFileName.c_str());

      if (folOut.is_open())
      {
         itLastDist = distances.end();
         itDist = distances.begin();
         folOut << sFileName << "     Output is probe cell, reduced cell, NCDist distance between" << std::endl;
         typename std::list<double>::const_iterator itDist = distances.begin();
         typename std::vector<TVEC>::const_iterator itP1 = probePath.begin();
         typename std::vector<TVEC>::const_iterator itP2 = secondProbePath.begin();
         const std::list<double>::const_iterator itLastDist = distances.end();
         for (int counter = 0; itDist != itLastDist; ++itP1, ++itP2, ++itDist, ++counter)
            folOut << OnePosition(itDist, itP1, itP2, counter);
         folOut.close();
      }
      else
      {
         std::cout << "Could not open file " << sFileName << " for write" << std::endl;
      }
   }
private:
   const std::string sFileName;
   const Follow<TVEC,TMAT> & follow;
   const std::vector<Glitch<TVEC> >& glitches;
   const std::vector<TVEC> probePath;
   const std::vector<TVEC> secondProbePath;
   const std::list<double> distances;

   std::list<double>::       const_iterator itDist;
   typename std::vector<TVEC>:: const_iterator itP1;
   typename std::vector<TVEC>:: const_iterator itP2;
   typename std::list<double>:: const_iterator itLastDist;

   //std::string OnePosition( const std::list<double>::const_iterator& itDist,
   //                         const std::vector<G6>:: const_iterator& itP1,
   //                         const std::vector<G6>:: const_iterator& itP2,
   //                         const int counter ) const;
};


#endif