#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#if _MSC_VER
#pragma once
#endif

#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

class FileOperations {
public:
   static bool OpenOutputFile( std::ofstream& fileout, const std::string& sFileName );
   static void WriteFile( std::ofstream& fileout, const std::string& svg );
   static bool CloseFile( std::ofstream& fileout ) { fileout.close( ); }
   static void Write( const std::string& sFileName, const std::string& svg );

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   template<typename CONTAINER>
   void Write(const std::string& sFileName, const typename CONTAINER& text) {
      std::ofstream fileout;
      if (OpenOutputFile(fileout, sFileName.c_str())) {
         CONTAINER<std::string>::const_iterator it;
         for (it = text.begin(); it != text.end(); ++it)
            WriteFile(fileout, *it);
         fileout.close();
      }
   }
};

#endif