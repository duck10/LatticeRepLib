#include "FileOperations.h"


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool FileOperations::OpenOutputFile( std::ofstream& fileout, const std::string& sFileName ) {
   if (fileout.is_open( )) return true;
   else if (!fileout) {
      std::cout << "Did not create the file" << std::endl;
   }
   else {
      fileout.open( sFileName.c_str( ), std::ios::out | std::ios::app | std::ios::ate );
      int count = 0;

      // do our best to open the output file
      while (count < 1000 && !fileout.is_open( )) {
         ++count;
      }
   }
   return fileout.is_open( );
}


void FileOperations::WriteFile( std::ofstream& fileout, const std::string& svg ) {
   fileout << svg << std::endl;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void FileOperations::Write( const std::string& sFileName, const std::string& text ) {
   std::ofstream fileout;
   if (OpenOutputFile( fileout, sFileName.c_str( ) ) )
      WriteFile( fileout, text );
   fileout.close( );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void FileOperations::Write( const std::string& sFileName, const std::vector<std::string>& text ) {
   std::ofstream fileout;
   if (OpenOutputFile( fileout, sFileName.c_str( ) )) {
      std::vector<std::string>::const_iterator it;
      for (it = text.begin( ); it != text.end( ); ++it)
         WriteFile( fileout, *it );
      fileout.close( );
   }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void FileOperations::Write( const std::string& sFileName, const std::list<std::string>& text ) {
   std::ofstream fileout;
   if (OpenOutputFile( fileout, sFileName.c_str( ) )) {
      std::list<std::string>::const_iterator it;
      for (it = text.begin( ); it != text.end( ); ++it)
         WriteFile( fileout, *it );
      fileout.close( );
   }
}
