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
   void Write( const std::string& sFileName, const std::vector<std::string>& text );
   void Write( const std::string& sFileName, const std::list<std::string>& text );
};

#endif