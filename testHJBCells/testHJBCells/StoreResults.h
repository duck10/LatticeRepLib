#ifndef STORERESULTS_H
#define STORERESULTS_H
#ifdef _MSC_VER
#pragma once
#endif

#include "Reducer.h"
#include "TNear.h"

#include <algorithm>    // std::sort
#include <functional>   // std::greater
#include <iostream>
#include <sstream>
#include <vector>

template <typename TKey, typename TData>
class StoreResults {
public:
   StoreResults( const size_t nMax=1 )
      : m_nmax( nMax )
      , m_storeCount( ) {
      m_storeCount.resize( 1000 );
   }

   size_t size( void ) const { return m_tree.size( ); }

   bool empty(void) const { return m_tree.empty(); }

   TKey GetKey( const size_t index ) const {
      return m_tree[index];
   }

   void Store( const TKey& key, const TData& data ) {
      size_t index;
      typename CNearTree<TKey>::iterator it = m_tree.NearestNeighbor( 0.01, key );
      if ( it == m_tree.end( ) ) {
         m_tree.insert( key );
         it = m_tree.NearestNeighbor( 0.01, key );
         index = it.get_position( );
         const size_t total = m_data.size( );
         if ( index >= total ) {
            m_data.resize( index + 1 );
            m_storeCount.resize( index + 1 );
         }
         m_data[index].push_back( data );
         m_storeCount[index] = 1;
      }
      else {
         index = it.get_position( );
         ++m_storeCount[index];
         if ( m_data[index].size( ) < m_nmax )
            m_data[index].push_back( data );
      }
   }

   std::vector<TData> FindResult( const TKey& key ) const {
      const typename CNearTree<TKey>::iterator it =  m_tree.NearestNeighbor( 1.e-5, key );
      if ( it == m_tree.end( ) ) {
         return std::vector<TData>( );
      }
      else {
         const size_t index = it.get_position( );
         return m_data[index];
      }
   }

   void ShowItem( const TKey& key ) const {
      typename CNearTree<TKey>::iterator it = m_tree.NearestNeighbor( 0.0, key );
      ShowItem( it.get_position( ) );
   }

   void ShowItem( const size_t index ) const {
      if ( index <m_data.size( ) ) {
         std::cout << "item " << index << "    (total=" << m_data[index][0] << ")" << std::endl;
         std::cout << m_tree[index] << std::endl;
         for ( size_t k=0; k<m_data[index].size( ); ++k ) {
            std::cout << std::endl;
            std::cout << m_data[index][k] << std::endl;
         }
      }
   }

//   void GetItem( const size_t index ) {
//      if (index < m_data.size( )) {
//         const TKey tk =
//      }
//   }

   void ShowResults( void ) const {
      typename CNearTree<TKey>::iterator it;
      std::vector<std::pair<int, size_t> > indexToSort;

      for ( it=m_tree.begin( ); it!=m_tree.end( ); ++it ) {
         const size_t index = it.get_position( );
         indexToSort.push_back( std::make_pair( m_storeCount[index], index ) );
      }

      std::sort( indexToSort.begin( ), indexToSort.end( ), std::greater<std::pair<int, size_t> >( ) );

      for ( size_t i=0; i<indexToSort.size( ); ++i ) {
         const size_t index = indexToSort[i].second;
         if ( ! m_itemSeparator.empty() ) std::cout << m_itemSeparator << std::endl;
         std::cout << "item " << index << "    (total=" << indexToSort[i].first << ")" << std::endl;
         std::cout << m_tree[index] << std::endl;
         for ( size_t k=0; k<m_data[index].size( ); ++k ) {
            if ( ! m_valueSeparator.empty() ) std::cout << m_valueSeparator << std::endl;
            std::cout << m_data[index][k] << std::endl;
         }
         if ( m_itemSeparator.empty() ) std::cout << std::endl;
      }

      std::cout << "index    count" <<std::endl;
      for ( size_t i=0; i<indexToSort.size( ); ++i )
         if ( indexToSort[i].first > 0 ) std::cout << "   " << indexToSort[i].second << "      " << indexToSort[i].first << std::endl;
      std::cout <<std::endl <<std::endl;

   }

   std::vector<TData> FindData( const TKey& key ) {
      const typename CNearTree<TKey>::iterator it = m_tree.NearestNeighbor( 1.0E-10, key );
      if ( it != m_tree.end( ) ) {
         return std::vector<TData>( );
      }
      else {
         const size_t index = it.get_position( );
         //  TData temp = m_data[index];
         if ( index >= m_data.size( ) )
            return std::vector<TData>( );
         else
            return m_data[index];
      }
   }

   void SetMaxItemStore( const int n ) { m_nmax = n; }
   int GetItemCount( const int n ) const { return m_storeCount[n]; }
   void SetItemSeparator( const std::string& s ) { m_itemSeparator = s; }
   void SetValueSeparator( const std::string& s ) { m_valueSeparator = s; }

   StoreResults operator= ( const StoreResults& sr ) {
      m_nmax           = sr.m_nmax          ;
      m_tree           = sr.m_tree          ;
      m_data           = sr.m_data          ;
      m_storeCount     = sr.m_storeCount    ;
      m_itemSeparator  = sr.m_itemSeparator ;
      m_valueSeparator = sr.m_valueSeparator;
      return *this;
   }

private:
   size_t m_nmax;
   CNearTree<TKey> m_tree;
   std::vector<std::vector<TData> > m_data;
   std::vector<int> m_storeCount;
   std::string m_itemSeparator;
   std::string m_valueSeparator;
};

#endif
