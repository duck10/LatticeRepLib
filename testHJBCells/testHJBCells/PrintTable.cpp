#include "PrintTable.h"
#include "ToString.h"

PrintTable::PrintTable( const size_t rowCount, const size_t colCount, const size_t colWidth )
: m_table( )
, m_rowCount( rowCount )
, m_colCount( colCount )
, m_colWidth( colWidth )
, m_colSeparator( )
, m_rowSeparator( )
, m_numberColumns( false ) {
   m_rowLength = m_colCount * m_colWidth + 1;
   resize( rowCount, colCount, colWidth );
}

void PrintTable::SetRowCount( const size_t n ) {
   m_rowCount = n;
}

void PrintTable::SetColCount( const size_t n ) {
   m_colCount = n;
}

void PrintTable::SetColWidth( const size_t n ) {
   m_colWidth = n;
}

void PrintTable::insert_center( const size_t row, const size_t col, const std::string& s ) {
   if ( row > m_rowCount - 1 || col > m_colCount - 1 ) return;
   if ( s.length( ) >= m_colWidth ) return;

   const size_t pos = ( m_colWidth - s.length( ) ) / 2;
   const size_t totalinrows = row*m_rowLength;
   const size_t totalinthecol = col*m_colWidth;
   m_table.replace( row*m_rowLength + col*m_colWidth + pos, s.length( ), s );
}

void PrintTable::insert_center( const size_t row, const size_t col, const int n ) {
   insert_center( row, col, ToString( n ) );
}

void PrintTable::insert_center( const size_t row, const size_t col, const double d ) {
   insert_center( row, col, ToString( d ) );
}

void PrintTable::insert_left( const size_t row, const size_t col, const std::string& s ) {
   if ( row > m_rowCount - 1 || col > m_colCount - 1 ) return;
   if ( s.length( ) > m_colWidth ) return;

   const size_t totalinrows = row*m_rowLength;
   const size_t totalinthecol = col*m_colWidth;
   m_table.replace( row*m_rowLength + col*m_colWidth, s.length( ), s );
}

void PrintTable::insert_left( const size_t row, const size_t col, const int n ) {
   insert_left( row, col, ToString( n ) );
}

void PrintTable::insert_left( const size_t row, const size_t col, const double d ) {
   insert_left( row, col, ToString( d ) );
}

void PrintTable::InsertColumnSeparators( void ) {
   for ( int i = m_table.size( ) - 1; i >= 0; i -= m_colWidth ) {
      if ( i > 0 && m_table[i - 1] == '\n' ) {
         m_table.insert( i, m_colSeparator );
         --i;
      }
      m_table.insert( i, m_colSeparator );
   }
}

void PrintTable::InsertRowSeparators( void ) {
   const size_t lineLength = m_table.size( ) / m_colCount;
   std::string sep;
   for ( size_t i = 0; i<lineLength - 1; ++i ) sep += m_rowSeparator;
   sep += "\n";
   for ( int i = m_table.size( ); i >= 0; i -= lineLength ) {
      if ( i > 0 && m_table[i - 1] == '\n' ) {
         //--i;
      }
      m_table.insert( i, sep );
   }
}

std::ostream& operator<< ( std::ostream& o, PrintTable& tbl ) {
   if ( tbl.m_colSeparator.length( ) > 0 ) {
      tbl.InsertColumnSeparators( );
   }

   if ( tbl.m_rowSeparator != '\0' ) {
      tbl.InsertRowSeparators( );
   }

   tbl.NumberColumns( );
   tbl.NumberRows( );

   o << tbl.m_table << std::endl;
   return o;
}

void PrintTable::resize( const size_t rowCount, const size_t colCount, const size_t colWidth ) {
   m_table.resize( rowCount*m_rowLength, ' ' );
   const size_t newlinePos = colCount* colWidth + 1;
   for ( size_t i = newlinePos - 1; i < m_table.size( ); i += newlinePos ) {
      m_table[i] = '\n';
   }
}

void PrintTable::SetColumnSeparator( const std::string& s ) {
   m_colSeparator = s;
}

void PrintTable::SetRowSeparator( const char s ) {
   m_rowSeparator = s;
}


void PrintTable::TestTable( ) {
   const size_t rows = m_rowCount;
   const size_t cols = m_colCount;

   PrintTable tbl( rows, cols, m_colWidth );
   std::cout << "rows " << rows << " columns " << cols << " width " << m_colWidth << std::endl;
   tbl.insert_left( 0, 0, "aaaaa" );
   tbl.insert_left( 0, 2, "aaaaa" );
   tbl.insert_left( 1, 1, "aaaaa" );
   tbl.insert_left( 1, 3, "aaaaa" );
   tbl.SetColumnSeparator( "#" );
   double counter = 0;
   for ( size_t i = 0; i < 3; ++i ) for ( size_t j = 0; j < 4; ++j ) {
      tbl.insert_left( i, j, counter );
      ++counter;
   }

   std::cout << tbl << std::endl;
}

void PrintTable::SetNumberColumns( const bool b /*= true */ ) {
   m_numberColumns = b;
}

void PrintTable::SetNumberRows( const bool b /*= true */ ) {
   m_numberRows = b;
}

std::string CenterText( const size_t fieldWidth, const std::string& s ) {
   std::string temp;
   if ( s.size( ) > fieldWidth ) return temp;

   temp.resize( fieldWidth, ' ' );

   const size_t pos = ( fieldWidth - s.length( ) ) / 2;
   temp.replace( pos, s.length( ), s );
   return temp;
}

void PrintTable::NumberColumns( void ) {
   std::string s;
   if ( !m_numberColumns ) return;
   const size_t lineLength = m_colCount*m_colWidth /*+ ( m_colCount + 1 )*m_colSeparator.size( )*/;
   const size_t numberWidth = lineLength / m_colCount;
   for ( size_t i = 0; i < m_colCount; ++i ) {
      const std::string num = ToString( i );
      const std::string field = CenterText( numberWidth + m_colSeparator.size( ), num );
      s += field;
   }
   m_table.insert( 0, s + "\n" );
}

void PrintTable::NumberRowWithoutRowSeparators( ) {
   if ( m_numberRows ) return;
   std::vector<std::string> vs;
   const size_t rowLabelWidth = 6;
   for ( size_t i = 0; i < m_rowCount; ++i ) {
      vs.push_back( ToString( m_rowCount - i ) + "  " );
   }

   const size_t width = vs[m_rowCount - 1].length( ) + rowLabelWidth;
   const size_t columnWidth = m_colWidth + m_colSeparator.size( );

   const size_t newlinePos = m_colCount* m_colWidth + 1;
   int counter = 0;
   const size_t lineLength = m_table.size( ) / m_rowCount;
   for ( int i = m_table.size( ) - lineLength; i >= 0; i -= lineLength ) {
      m_table.insert( i, vs[counter] );
      ++counter;
   }
}

void PrintTable::NumberRowWithRowSeparators( ) {
   if ( ( !m_numberRows ) | ( m_rowSeparator=='\0' ) ) return;
   std::vector<std::string> vs;
   const size_t rowLabelWidth = 6;
   for ( size_t i = 0; i < m_rowCount; ++i ) {
      vs.push_back( ToString( m_rowCount - i ) + "  " );
   }

   const size_t width = vs[m_rowCount - 1].length( ) + rowLabelWidth;
   const size_t columnWidth = m_colWidth + m_colSeparator.size( );

   const size_t newlinePos = m_colCount* m_colWidth + 1;
   int counter = 0;
   const size_t lineLength = m_table.size( ) / m_rowCount;
   for ( int i = m_table.size( ) - lineLength+2*m_colWidth+m_colSeparator.length()+1; i >= 0; i -= lineLength-m_colSeparator.size() ) {
      m_table.insert( i, vs[counter] );
      ++counter;
   }
}

void PrintTable::NumberRows( void ) {
   if ( !m_numberRows ) return;
   NumberRowWithoutRowSeparators( );
   NumberRowWithRowSeparators( );
   return;
   std::vector<std::string> vs;
   const size_t rowLabelWidth = 6;
   for ( size_t i = 0; i < m_rowCount; ++i ) {
      vs.push_back( ToString( m_rowCount - i ) + "  " );
   }

   const size_t width = vs[m_rowCount - 1].length( ) + rowLabelWidth;
   const size_t columnWidth = m_colWidth + m_colSeparator.size( );

   const size_t newlinePos = m_colCount* m_colWidth + 1;
   int counter = 0;
   const size_t lineLength = m_table.size( ) / m_rowCount;
   for ( int i = m_table.size( ) - lineLength; i >= 0; i -= lineLength ) {
      m_table.insert( i, vs[counter] );
      ++counter;
   }

}