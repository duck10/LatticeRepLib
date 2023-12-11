#ifndef PRINTTABLE_H
#define PRINTTABLE_H

#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>


class PrintTable {
public:
   PrintTable( const size_t rowCount, const size_t colCount, const size_t colWidth );
   void SetRowCount( const size_t n );
   void SetColCount( const size_t n );
   void SetColWidth( const size_t n );
   void SetColumnSeparator( const std::string& s);
   void SetRowSeparator( const char c);
   void SetNumberColumns( const bool b = true );
   void SetNumberRows( const bool b = true );

   void insert_center( const size_t row, const size_t col, const std::string& s );
   void insert_center( const size_t row, const size_t col, const int n );
   void insert_center( const size_t row, const size_t col, const double d );
   void insert_left( const size_t row, const size_t col, const std::string& s );
   void insert_left( const size_t row, const size_t col, const int n );
   void insert_left( const size_t row, const size_t col, const double d );
   void InsertColumnSeparators( void );
   void InsertRowSeparators( void );
   void NumberColumns( void );
   void NumberRows( void );
   void NumberRowWithoutRowSeparators( void );
   void NumberRowWithRowSeparators( void );

   void TestTable();

   friend std::ostream& operator<< ( std::ostream& o, PrintTable& s);

private:

   void resize( const size_t rowCount, const size_t colCount, const size_t colWidth );
   std::string m_table;
   size_t m_rowCount;
   size_t m_colCount;
   size_t m_colWidth;
   size_t m_rowLength;
   char m_rowSeparator;
   std::string m_colSeparator;
   bool m_numberColumns;
   bool m_numberRows;
};

#endif

