//*
//*  TNear.h
//*  NearTree
//*
//*  Copyright 2001, 2008 Larry Andrews.  All rights reserved
//*  Revised 12 Dec 2008 for sourceforge release -- H. J. Bernstein
//*  Revised 30 May 2009, release with full containerization of C++
//*                       version and KNear/Far in C++ and C, LCA + HJB
//*  Revised 13 Nov 2010, revisions to C++ version for balanced
//*                       searches, LCA+HJB


//**********************************************************************
//*                                                                    *
//* YOU MAY REDISTRIBUTE NearTree UNDER THE TERMS OF THE LGPL          *
//*                                                                    *
//**********************************************************************/

//************************* LGPL NOTICES *******************************
//*                                                                    *
//* This library is free software; you can redistribute it and/or      *
//* modify it under the terms of the GNU Lesser General Public         *
//* License as published by the Free Software Foundation; either       *
//* version 2.1 of the License, or (at your option) any later version. *
//*                                                                    *
//* This library is distributed in the hope that it will be useful,    *
//* but WITHOUT ANY WARRANTY; without even the implied warranty of     *
//* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  *
//* Lesser General Public License for more details.                    *
//*                                                                    *
//* You should have received a copy of the GNU Lesser General Public   *
//* License along with this library; if not, write to the Free         *
//* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,    *
//* MA  02110-1301  USA                                                *
//*                                                                    *
//**********************************************************************/

//  This is a revised release of
//  template <typename T> class CNearTree;
//
// Nearest Neighbor algorithm after Kalantari and McDonald,
// (IEEE Transactions on Software Engineering, v. SE-9, pp.
//    631-634,1983)
//  modified to use recursion instead of a double-linked tree
//  and simplified so that it does a bit less checking for
//  things like is the distance to the right less than the
//  distance to the left; it was found that these checks made little
//  to no difference in timing.
//  Later revisions have replaced the use of recursion with a stack,
//  except for the case of inserting data into the tree.


// This template is used to contain a collection of objects. After the
// collection has been loaded into this structure, it can be quickly
// queried for which object is "closest" to some probe object of the
// same type. The major restriction on applicability of the near-tree
// is that the algorithm only works if the objects obey the triangle
// inequality. The triangle rule states that the length of any side of
// a triangle cannot exceed the sum of the lengths of the other two sides.


// The user of this class needs to provide at least the following
// functionality for the template to work. For the built-in
// numerics of C++, they are provided here (or else you should create them).

//    DistanceType Norm( );   // conversion constructor from the templated class to DistanceType
//                                (usually will return a "length" of type double)
//    operator- ( );          // geometrical (vector) difference of two objects
//    a copy constructor would be nice
//    a constructor would be nice
//    a destructor would be nice

// The provided interface is:
//
//    #include "TNear.h"
//
//    CNearTree( void )   // constructor
//       instantiated by something like:      CNearTree <T> vTree;
//       for some type T
//       the following additional convenience constructors are available
//
//    CNearTree( const ContainerType<T> )   // constructor from containers, std::vector, ..., or CNearTree
//
//    void insert( const T& t )
//       where t is an object of the type T
//       the following additional convenience insert template available
//       all inserts are delayed until a search is performed or until an explicit call to CompleteDelayedInsertions
//       is called or a search is called. The purpose is to distribute the objects a bit more
//       randomly. Excessively ordered objects leads to less than optimal trees.
//       Places objects in a queue for insertion later when CompleteDelayInsert
//
//    void insert( ContainerType ) // for containers, std::vector, ..., or CNearTree
//       all inserts are delayed until a search is performed or until an explicit call to CompleteDelayedInsertions
//
//    bool NearestNeighbor ( const DistanceType dRadius,  T& tClosest,   const T& t ) const
//       dRadius is the largest radius within which to search; make it
//          very large if you want to include every point that was loaded; dRadius
//          is returned as the closest distance to the probe (or the search radius
//          if nothing is found)
//       tClosest is returned as the object that was found closest to the probe
//          point (if any were within radius dRadius of the probe)
//       t is the probe point, used to search in the group of points insert'ed

//       return value is true if some object was found within the search radius, false otherwise
//
//    iterator NearestNeighbor( const DistanceType radius, const T& probe ); returns an iterator
//       to the nearest point to the probe point or end() if there is none
//
//    bool FarthestNeighbor ( T& tFarthest,   const T& t ) const
//       tFarthest is returned as the object that was found farthest to the probe
//          point
//       t is the probe point, used to search in the group of points insert'ed
//       return value is true if some object was found, false otherwise
//
//    iterator FarthestNeighbor( const T& probe ); returns an iterator
//       to the farthest point to the probe point or end() if there is none
//
//
//    the following functions (FindInSphere, FindOutSphere, and FindInAnnulus) all return a container
//    (ContainerType) that can be any standard library container (such as std::vector< T >) or CNearTree.
//    each has an alternate version in which, in addition the indices of the objects in the
//    object store are returned in a second parallel vector.
//
//    long FindInSphere ( const DistanceType dRadius,  ContainerType& tClosest, const T& t ) const
//    long FindInSphere ( const DistanceType dRadius,  ContainerType& tClosest, std::vector<size_t>& tIndices, const T& t ) const
//       dRadius is the radius within which to search; make it very large if you want to
//           include every point that was loaded;
//       tClosest is returned as the ContainerType of objects that were found within a radius dRadius
//          of the probe point
//       if tIndices is used, it is a vector to which to add the indices of the points found
//       t is the probe point, used to search in the group of points insert'ed
//       return value is the number of objects found within the search radius
//
//    long FindOutSphere ( const DistanceType dRadius,  ContainerType& tClosest, const T& t ) const
//    long FindOutSphere ( const DistanceType dRadius,  ContainerType& tClosest, std::vector<size_t>& tIndices, const T& t ) const
//       dRadius is the radius outside which to search; make it very small if you want to
//           include every point that was loaded;
//       tClosest is returned as the ContainerType of objects that were found within a radius dRadius
//          of the probe point
//       if tIndices is used, it is a vector to which to add the indices of the points found
//       t is the probe point, used to search in the group of points insert'ed
//       return value is the number of objects found within the search radius
//
//    long FindInAnnulus (const DistanceType dRadius1, const DistanceType dRadius2, ContainerType& tClosest,   const T& t ) const
//    long FindInAnnulus (const DistanceType dRadius1, const DistanceType dRadius2, ContainerType& tClosest, std::vector<size_t>& tIndices,  const T& t ) const
//       dRadius1 and dRadius2 are the two radii between which to find  data points
//       tClosest is returned ContainerType of the objects found in the annulus
//       if tIndices is used, it is a vector to which to add the indices of the points found
//       t is the probe point, used to search in the group of points insert'ed
//       return value is the number of objects found within the search radius
//
//    long FindK_NearestNeighbors ( const size_t k, const DistanceType& radius,  OutputContainerType& tClosest,   const T& t )
//    long FindK_NearestNeighbors ( const size_t k, const DistanceType& radius,  OutputContainerType& tClosest, std::vector<size_t>& tIndices,  const T& t )
//       k is the maximum number of nearest neighbors to return. Finds this many if possible
//       radius Within a sphere defined by radius, search for the k-nearest-neighbors
//       tClosest is returned ContainerType of the objects found within the sphere
//       if tIndices is used, it is a vector to which to add the indices of the points found
//       t is the probe point, used to search in the group of points insert'ed
//
//    long FindK_FarthestNeighbors ( const size_t k, OutputContainerType& tClosest,   const T& t )
//    long FindK_FarthestNeighbors ( const size_t k, OutputContainerType& tClosest, std::vector<size_t>& tIndices,  const T& t )
//       k is the maximum number of farthest neighbors to return. Finds this many if possible
//       tClosest is returned ContainerType of the objects found
//       if tIndices is used, it is a vector to which to add the indices of the points found
//       t is the probe point, used to search in the group of points insert'ed
//
//    The variants LeftNearestNeighbor, LeftFarthestNeighbor, LeftFindInSphere, LeftFindOutSphere,
//    LeftFindInAnnulus, LeftFindK_NearestNeighbors, and LeftFindK_FarthestNeighbors are the
//    older, search-left-first versions, retained for exiting applications that may require support
//    for those versions and for testing and validation.  Those older versions are deprecated
//    and may be removed in an upcoming release.
//
//    ~CNearTree( void )  // destructor
//
// =====================================================================================================
// access functions
//
// T at( const size_t n ) const
//     returns the n'th item of the internal data store
//
// T operator[] ( const size_t n)
//     returns the n'th item of the internal data store
//
// operator ContainerType( void ) const
//     returns all of the inserted objects in the tree in a container of type ContainerType.
//     ContainerType can be std::vector<T>, etc, or other containers.
//     The returned vector contents are not guaranteed to be returned in the order loaded.
//
// iterator begin ( void ) const
//     returns an iterator to the beginning of the internal data store
//
// iterator end ( void ) const
//     returns an iterator to the end of the data store (one beyond the last item)
//
// iterator back ( void ) const
//     returns an iterator to the last data item of the internal data store
//
// =====================================================================================================
// information and special operation functions
// =====================================================================================================
//
//    void ImmediateInsert( void ) Places objects immediately into the tree. The usual insert function
//       delays insertions, allowing them to be inserted into the tree in a more random order. The delay
//       can improve the structure of the tree and speed searches.
//
//    void CompleteDelayedInsert( void ) Calls insert for all delayed objects. sqrt(n) of them are inserted
//       by random choice. The rest are inserted in linear order as originally queued. CompleteDelayedInsert
//       is invoked at the beginning of all searches, so the average user will never need
//       to call it.
//
//    size_t GetDeferredSize( void ) Returns the number of delayed objects that have not
//       yet been insert'ed. This is mainly for information about details of the tree.
//
//    size_t GetTotalSize( void ) Returns the number of objects that have been insert'ed plus
//       those DelayInsert'ed
//
//    size_t size( void ) identical to GetTotalSize
//
//    size_t GetDepth( void ) Returns the maximum tree layers from the root.  This is
//       mainly for information about details of the tree.
//
//    bool empty( void )  returns true if the tree is empty, otherwise false
//
// =====================================================================================================
// iterators
//     Random access iterators are provided for accessing the data in a CNearTree. The most important
//     expected use is to retrieve the objects returned from one of the sphere search functions that
//     return a CNearTree. However, they can be used with any CNearTree.
//     They should function in a fashion essentially the same as STL iterators. There is no assurance
//     that data will be returned in the order it was loaded, just that it is accessible. The same set is
//     provided for const_iterator.
// =====================================================================================================
//      iterator( void ) { }; // constructor
//
//      iterator& operator=  ( const iterator& s )
//      iterator  operator++ ( const int n )
//      iterator  operator-- ( const int n )
//      iterator& operator++ ( void )
//      iterator& operator-- ( void )
//      iterator  operator+  ( const long n ) const
//      iterator  operator-  ( const long n ) const
//      iterator& operator+= ( const long n )
//      iterator& operator-= ( const long n )
//      T         operator*  ( void )         const
//
//      bool      operator== ( const iterator& t ) const
//      bool      operator!= ( const iterator& t ) const
//
//      const T * const operator-> ( void )   const
//
// =====================================================================================================
//
// So a complete program is:
//
// #include "TNear.h"
// #include <cstdio>
// void main()
// {
//   CNearTree< double > dT;
//   double dNear;
//   dT.insert( 1.5 );
//   if ( dT.FindNearestNeighbor( 10000.0,   dNear,  2.0 )) printf( "%f\n",DistanceType(dNear-2.0) );
// }
//
// and it should print 0.5 (that's how for 2.0 is from 1.5)
//
//
//-------------------------------------------------------------------------


#if !defined(TNEAR_H_INCLUDED)
#define TNEAR_H_INCLUDED

#include <stdlib.h>
#include <limits.h>
#include <cfloat>
#include <algorithm>
#include <cmath>

#define USE_LOCAL_HEADERS

#define USE_ARMADILLO_LIBRARY

#ifdef _MSC_VER
#define USE_LOCAL_HEADERS
#endif
#ifndef USE_LOCAL_HEADERS
#include <rhrand.h>
#include <triple.h>
#else
#include "rhrand.h"
#include "triple.h"
#endif
//
//#ifdef USE_ARMADILLO_LIBRARY
//#define ARMA_DONT_USE_BLAS
//#define ARMA_DONT_USE_LAPACK
//#include <armadillo>
//#endif

#include <vector>
#include <set>
#include <iterator>

#ifdef CNEARTREE_SAFE_TRIANG
#define TRIANG(a,b,c) (  (((b)+(c))-(a) >= 0) \
|| ((b)-((a)-(c)) >= 0) \
|| ((c)-((a)-(b)) >= 0))
#else
#define TRIANG(a,b,c) (  (((b)+(c))-(a) >= 0))
#endif

#define CNEARTREE_COLLIDE 1.e-38

//=======================================================================
// CNearTree is the root class for the neartree. The actual data of the
// tree is stored in NearTreeNode objects descending from a CNearTree.
//=======================================================================

template <typename T, typename DistanceType=double, int distMinValue=-1 > class CNearTree
{
//=======================================================================
//   NOTES:
//
// The types of objects that can be stored in the tree is quite broad. The
// biggest limitation is that the objects must reside in some sort of metric
// space and must obey the triangle rule. They must also be all of the same
// size because they are stored in an std::vector. If your application
// requires object of varying storage, then your best way to use this
// code is to store pointers or handles and to write your own distance functions.
//
// The type of the objects to be stored is the only _required_ template argument.
// The type of the distance measure (DistanceType) defaults to double. If your
// applications is for an integer type then the type for DistanceType can be your
// integer type. This has the potential for speeding the calculations by
// avoiding FP computation. Other general types can be used if desired, but you
// may need to also input a value of distMinValue.
//
// The template argument distMinValue must be something that your class will
// understand as a negative number. The default input is negative one. Internally,
// that is cast to DistanceType. Since most uses will be for DistanceType
// to be double, that is a simple conversion. Obviously, for integer types,
// there is no problem either. The need for this value is to have something
// internally that is recognizable as smaller than the smallest "distance"
// that can exist between any two objects in your type. For most users,
// there is no need to input anything other than the default, -1. -1 must
// be castable to DistanceType. It seems unlikely that anyone would actually
// need this optional parameter, but it is here for completeness.
//
//  It is a design decision that this class cannot work for unsigned types.
//  It is hard to see how to verify the triangle rule for unsigned types,
//  and distance computations become more complex. Sorry, unsigned types
//  are left as an exercise for the reader.
//
//=======================================================================

// insert copies the input objects into a binary NEAR tree. When a node has
// two entries, a descending node is used or created. The current datum is
// put into the branch descending from the nearer of the two
// objects in the current node.

// NearestNeighbor retrieves the object nearest to some probe by descending
// the tree to search out the appropriate object. Speed is gained
// by pruning the tree if there can be no data below that are
// nearer than the best so far found.

// The tree is built in time O(n log n), and retrievals take place in
// average time O(log n). However, worst case is O(n).

public:
// DistanceBetween
// template function for calculating the "distance" between two objects.
// The specific functions for the built-in types must be here also. For
// the common types (int, float, ...) they are provided.
template <typename TT>
static inline DistanceType DistanceBetween( const TT& t1, const TT& t2 )
{
DistanceType d = ( t1-t2 ).norm( );
return( d>0?d:-d );  // apparent compiler error makes this necessary
}

static inline size_t DistanceBetween( const size_t& t1, const size_t& t2 )
{
   size_t diff = t1-t2;
   return diff >0?diff:-diff;  // apparent compiler error makes this necessary
}

static inline DistanceType DistanceBetween( const double t1, const double t2 )
{
    return( (DistanceType)fabs( t1-t2 ) ); // encourage the compiler to get the correct abs
}

//static inline DistanceType DistanceBetween( const long double t1, const long double t2 )
//{
//    return(  (DistanceType)fabsl(t1-t2) ); // encourage the compiler to get the correct abs
//}

static inline DistanceType DistanceBetween( const float t1, const float t2 )
{
    return( (DistanceType)fabsf( t1-t2 )); // encourage the compiler to get the correct abs
}

static inline DistanceType DistanceBetween( const int t1, const int t2 )
{
    return( (DistanceType)abs(t1-t2) ); // encourage the compiler to get the correct abs
}

static inline DistanceType DistanceBetween( const long t1, const long t2 )
{
    return( (DistanceType)labs(t1-t2) ); // encourage the compiler to get the correct abs
}

//static inline DistanceType DistanceBetween( const long long t1, const long long t2 )
//{
//    return( (DistanceType)llabs(t1-t2) ); // encourage the compiler to get the correct abs
//}

static inline DistanceType DistanceBetween( const short t1, const short t2 )
{
    return( (DistanceType)abs(t1-t2) ); // encourage the compiler to get the correct abs
}


private:

RHrand rhr;

// forward declaration of nested class NearTreeNode
template <typename TNode, typename DistanceTypeNode, int distMinValueNode >
class NearTreeNode;
public:
// Forward declaration for the nested classes, iterator and const_iterator. Friend is necessary
// for the access to the appropriate data elements
class iterator;
friend class iterator;
class const_iterator;
friend class const_iterator;

static const long        NTF_NoPrePrune        = 1; //flag to supress all search prepruning
static const long        NTF_ForcePrePrune     = 2; //flag to force search prepruning
static const long        NTF_NoFlip            = 4; //flag to suppress flips on insert
static const long        NTF_ForceFlip         = 8; //flag to force flips on insert
static const long        NTF_NoDefer           =16; //flag to prevent deferred insert

#ifdef CNEARTREE_FORCEPREPRUNE
static const long        NFT_FlagDefaultPrune  = NTF_ForcePrePrune;
#ifdef CNEARTREE_NOPREPRUNE
#error "CNEARTREE_NOPREPRUNE conflicts with  CNEARTREE_FORCEPREPRUNE"
#endif
#else
#ifdef CNEARTREE_NOPREPRUNE
static const long        NFT_FlagDefaultPrune  = NTF_NoPrePrune;
#else
static const long        NFT_FlagDefaultPrune  = 0;
#endif
#endif



#ifdef CNEARTREE_FORCEFLIP
static const long        NFT_FlagDefaultFlip  = NTF_ForceFlip;
#ifdef CNEARTREE_NOFLIP
#error "CNEARTREE_NOFLIP conflicts with  CNEARTREE_FORCEFLIP"
#endif
#else
#ifdef CNEARTREE_NOFLIP
static const long        NFT_FlagDefaultFlip  = NTF_NoFlip;
#else
static const long        NFT_FlagDefaultFlip  = 0;
#endif
#endif

#ifdef CNEARTREE_NODEFER
static const long        NFT_FlagDefaultDefer = NTF_NoDefer;
#else
static const long        NFT_FlagDefaultDefer = 0;
#endif

static const long        NTF_FlagsDefault      =  NFT_FlagDefaultPrune|NFT_FlagDefaultFlip|NFT_FlagDefaultDefer;



private: // start of real definition of CNearTree
std::vector<long> m_DelayedIndices;    // objects queued for insertion, possibly in random order
std::vector<T>    m_ObjectStore;       // all inserted objects go here
std::vector<size_t>
                  m_ObjectCollide;     // overflow chain of colliding objects
size_t            m_DeepestDepth;      // maximum depth of the tree


NearTreeNode<T, DistanceType, distMinValue>      m_BaseNode; // the tree's data is stored down from here
long              m_Flags;             // flags for operational control (mainly for testing)
DistanceType      m_DiamEstimate;      // estimated diameter
DistanceType      m_SumSpacings;       // sum of spacings at time of insertion
DistanceType      m_SumSpacingsSq;     // sum of squares of spacings at time of insertion
double            m_DimEstimate;       // estimated dimension
double            m_DimEstimateEsd;    // estimated dimension estimated standard deviation
#ifdef CNEARTREE_INSTRUMENTED
mutable size_t            m_NodeVisits;        // number of node visits
#endif

public:


//=======================================================================
//  CNearTree ( )
//
//  Default constructor for class CNearTree
//  creates an empty tree with no right or left node and with the dMax-below
//  set to negative values so that any match found will be stored since it will
//  greater than the negative value
//
//=======================================================================
CNearTree ( void )  // constructor
: m_DelayedIndices (   )
, m_ObjectStore    (   )
, m_ObjectCollide  (   )
, m_DeepestDepth   ( 0 )
, m_BaseNode       (   )
, m_Flags          ( NTF_FlagsDefault )
, m_DiamEstimate  ( DistanceType( 0 ) )
, m_SumSpacings   ( DistanceType( 0 ) )
, m_SumSpacingsSq ( DistanceType( 0 ) )
, m_DimEstimate   ( 0 )
, m_DimEstimateEsd( 0 )
#ifdef CNEARTREE_INSTRUMENTED
, m_NodeVisits( 0 )
#endif
{
    
}  //  CNearTree constructor

//=======================================================================
// CNearTree ( const InputContainer& o )
//
// templated constructor for class CNearTree for input of containers.
// The containers can be standard library containers or a CNearTree.
//
//=======================================================================
template<typename InputContainer>
CNearTree ( const InputContainer& o )  // constructor
: m_DelayedIndices (   )
, m_ObjectStore    (   )
, m_ObjectCollide  (   )
, m_DeepestDepth   ( 0 )
, m_BaseNode       (   )
, m_Flags          ( NTF_FlagsDefault )
, m_DiamEstimate  ( DistanceType( 0 ) )
, m_SumSpacings   ( DistanceType( 0 ) )
, m_SumSpacingsSq ( DistanceType( 0 ) )
, m_DimEstimate   ( 0 )
, m_DimEstimateEsd( 0 )
#ifdef CNEARTREE_INSTRUMENTED
, m_NodeVisits( 0 )
#endif
{
    typename InputContainer::const_iterator it;
    for( it=o.begin(); it!=o.end(); ++it )
    {
        insert( *it );
    }
}  //  CNearTree constructor

//=======================================================================
// CNearTree ( InputContainer& o )
//
// templated constructor for class CNearTree for input of containers.
// The containers can be standard library containers or a CNearTree.
//
//=======================================================================
template<typename InputContainer>
explicit CNearTree ( InputContainer& o )  // constructor
: m_DelayedIndices (   )
, m_ObjectStore    (   )
, m_ObjectCollide  (   )
, m_DeepestDepth   ( 0 )
, m_BaseNode       (   )
, m_Flags          ( NTF_FlagsDefault )
, m_DiamEstimate  ( DistanceType( 0 ) )
, m_SumSpacings   ( DistanceType( 0 ) )
, m_SumSpacingsSq ( DistanceType( 0 ) )
, m_DimEstimate   ( 0 )
, m_DimEstimateEsd( 0 )
#ifdef CNEARTREE_INSTRUMENTED
, m_NodeVisits( 0 )
#endif
{
    typename InputContainer::iterator it;
    for( it=o.begin(); it!=o.end(); ++it )
    {
        insert( *it );
    }
}  //  CNearTree constructor

//=======================================================================
// CNearTree ( const InputContainer& o1, const InputContainer& o1 )
//
// templated constructor for class CNearTree for merging multiple 
// containers.
// The containers can be standard library containers or CNearTrees.
//
//=======================================================================
template<typename InputContainer1, typename InputContainer2>
CNearTree ( const InputContainer1& o1, const InputContainer2& o2 ) // constructor
: m_DelayedIndices (   )
, m_ObjectStore    (   )
, m_ObjectCollide  (   )
, m_DeepestDepth   ( 0 )
, m_BaseNode       (   )
, m_Flags          ( NTF_FlagsDefault )
, m_DiamEstimate  ( DistanceType( 0 ) )
, m_SumSpacings   ( DistanceType( 0 ) )
, m_SumSpacingsSq ( DistanceType( 0 ) )
, m_DimEstimate   ( 0 )
, m_DimEstimateEsd( 0 )
#ifdef CNEARTREE_INSTRUMENTED
, m_NodeVisits( 0 )
#endif
{
    typename InputContainer1::const_iterator it1;
    for( it1=o1.begin(); it1!=o1.end(); ++it1 )
    {
        insert( *it1 );
    }

    typename InputContainer2::const_iterator it2;
    for( it2=o2.begin(); it2!=o2.end(); ++it2 )
    {
        insert( *it2 );
    }

}  //  CNearTree constructor

//=======================================================================
//  ~CNearTree ( )
//
//  Destructor for class CNearTree
//
//=======================================================================
~CNearTree ( void )  // destructor
{
    clear ( );
}  //  ~CNearTree



//=======================================================================
// Name: Get and Set Flags
// Description: get and set tree flags
//
//=======================================================================
long GetFlags( void ) const
{
    return m_Flags;
}

void SetFlags( const long flags ) 
{
    m_Flags = flags;
}

long GetFlags( const long mask ) const
{
    return m_Flags&mask;
}

void SetFlags( const long flags, const long mask ) 
{
    m_Flags = (flags&mask)|(m_Flags&(~mask));
}

//=======================================================================
// Name: operator=()
// Description: put container's contents into a NearTree,
// wiping out the current contents
//
//=======================================================================
template<typename InputContainer>
CNearTree& operator= ( const InputContainer& o )
{
    if (this != &o) {

        this->clear();
        this->insert( o );
    
        this->CompleteDelayedInsert( );
    
    }
    return( *this );
}

template<typename InputContainer>
CNearTree& operator= ( InputContainer& o )
{
    if (this != &o) {

        this->clear();
        this->insert( o );
    
        this->CompleteDelayedInsert( );
        
    }
    return( *this );
}

/*

template<typename InputContainer>
CNearTree& operator= ( const InputContainer& o ) const
{
    if (this != &o) {
        
        this->clear();
        this->insert( o );
        
        this->CompleteDelayedInsert( );
        
    }
    return( *this );
}


template<typename InputContainer>
CNearTree& operator= ( InputContainer& o ) const
{
    if (this != &o) {
        
        this->clear();
        this->insert( o );
        
        this->CompleteDelayedInsert( );
        
    }
    return( *this );
}
 */



//=======================================================================
// Name: operator+=()
// Description: add a container's contents to a NearTree
//
//=======================================================================
template<typename InputContainer>
CNearTree& operator+= ( const InputContainer& o )
{

    if ( this->empty( ) )
    {  // if "this" is empty, all that will remain is "o"
        this->insert( o );
    }
    else if ( o.empty( ) )
    { // do nothing if there is nothing to be added to "this"
    }
    else
    {
        std::set<T> s1, s2, s3;
        s1.insert( this->begin( ), this->end( ) );

        s2.insert( o.begin(), o.end( ) );

        this->clear( );
        std::set_union(
            s1.begin( ), s1.end( ),
            s2.begin( ), s2.end( ),
            std::inserter( s3, s3.end( ) ) );

        this->insert( s3 );
    }

    this->CompleteDelayedInsert( );
    return( *this );
}

template<typename InputContainer>
CNearTree& operator+= ( InputContainer& o )
{
    
    if ( this->empty( ) )
    {  // if "this" is empty, all that will remain is "o"
        this->insert( o );
    }
    else if ( o.empty( ) )
    { // do nothing if there is nothing to be added to "this"
    }
    else
    {
        std::set<T> s1, s2, s3;
        s1.insert( this->begin( ), this->end( ) );
        
        s2.insert( o.begin(), o.end( ) );
        
        this->clear( );
        std::set_union(
                       s1.begin( ), s1.end( ),
                       s2.begin( ), s2.end( ),
                       std::inserter( s3, s3.end( ) ) );
        
        this->insert( s3 );
    }
    
    this->CompleteDelayedInsert( );
    return( *this );
}



//=======================================================================
// Name: operator-=()
// Description: removes a container's contents from a NearTree
//
//=======================================================================
template<typename InputContainer>
CNearTree& operator-= ( const InputContainer& o )
{

    if ( this->empty( ) )
    {// nothing to do if there's nothing to remove from
    }
    else if ( o.empty( ) )
    { // do nothing if there is nothing to be removed
    }
    else
    {
        std::set<T> s1, s2, s3;
        s1.insert( this->begin( ), this->end( ) );

        s2.insert( o.begin(), o.end( ) );

        this->clear( );
        std::set_difference(
            s1.begin( ), s1.end( ),
            s2.begin( ), s2.end( ),
            std::inserter( s3, s3.end( ) ) );

        this->insert( s3 );
    }

    this->CompleteDelayedInsert( );
    return( *this );
}

template<typename InputContainer>
CNearTree& operator-= ( InputContainer& o )
{
    
    if ( this->empty( ) )
    {// nothing to do if there's nothing to remove from
    }
    else if ( o.empty( ) )
    { // do nothing if there is nothing to be removed
    }
    else
    {
        std::set<T> s1, s2, s3;
        s1.insert( this->begin( ), this->end( ) );
        
        s2.insert( o.begin(), o.end( ) );
        
        this->clear( );
        std::set_difference(
                            s1.begin( ), s1.end( ),
                            s2.begin( ), s2.end( ),
                            std::inserter( s3, s3.end( ) ) );
        
        this->insert( s3 );
    }
    
    this->CompleteDelayedInsert( );
    return( *this );
}

//=======================================================================
// Name: set_symmetric_difference()
// Description: removes the portion container's contents from a NearTree
//              that is already in the NearTree and add in the portion
//              of the container's contents that is not already in the
//              NearTree
//   (= Sheffer stroke operation and NAND = exclusive or)
//
//=======================================================================
template<typename InputContainer>
CNearTree& set_symmetric_difference ( const InputContainer& o )
{

    if ( o.empty( ) )
    { // do nothing if "this" is already complete
    }
    else if ( this->empty( ) )
    { // all that will remain is the content of "o"
        this->insert( o );
    }
    else
    {
        std::set<T> s1, s2, s3;
        s1.insert( this->begin( ), this->end( ) );

        s2.insert( o.begin(), o.end( ) );

        this->clear( );
        std::set_symmetric_difference(
            s1.begin( ), s1.end( ),
            s2.begin( ), s2.end( ),
            std::inserter( s3, s3.end( ) ) );

        this->insert( s3 );
    }

    this->CompleteDelayedInsert( );
    return( *this );
}

template<typename InputContainer>
CNearTree& set_symmetric_difference ( InputContainer& o )
{
    
    if ( o.empty( ) )
    { // do nothing if "this" is already complete
    }
    else if ( this->empty( ) )
    { // all that will remain is the content of "o"
        this->insert( o );
    }
    else
    {
        std::set<T> s1, s2, s3;
        s1.insert( this->begin( ), this->end( ) );
        
        s2.insert( o.begin(), o.end( ) );
        
        this->clear( );
        std::set_symmetric_difference(
                                      s1.begin( ), s1.end( ),
                                      s2.begin( ), s2.end( ),
                                      std::inserter( s3, s3.end( ) ) );
        
        this->insert( s3 );
    }
    
    this->CompleteDelayedInsert( );
    return( *this );
}


//=======================================================================
// clear( void )
//
// removes all content from a tree
//
//=======================================================================
void clear ( void )
{
    this->m_BaseNode       .clear( ); // clear the nodes of the tree
    if ( ! this->m_DelayedIndices.empty( ) )
    {
        std::vector<long> vtempLong;
        m_DelayedIndices.swap( vtempLong );  // release any delayed indices list
    }
    if ( ! this->m_ObjectStore.empty( ) )
    {
        std::vector<T> vtempT;
        this->m_ObjectStore.swap( vtempT );  // release the object store
        std::vector<size_t> vtempOC;
        this->m_ObjectCollide.swap( vtempOC); // release the object collision store;
    }
    this->m_DeepestDepth   = 0;
    this->m_DiamEstimate   = DistanceType( 0 );
    this->m_SumSpacings    = DistanceType( 0 );
    this->m_SumSpacingsSq  = DistanceType( 0 );
    this->m_DimEstimate    = 0;
    this->m_DimEstimateEsd = 0;
#ifdef CNEARTREE_INSTRUMENTED
    this->m_NodeVisits     = 0;
#endif
    
}

//=======================================================================
//  empty ( )
//
//  Test for an empty CNearTree
//
//=======================================================================
bool empty ( void ) const
{
    return ( m_ObjectStore.empty( ) );
}

//=======================================================================
//  void insert ( const T& t )
//
//  Function to insert some "point" as an object into a CNearTree for
//  later searching
//
//     t is an object of the templated type which is to be inserted into a
//     NearTree
//
//  The function ImmediateInsert immediately inserts the object into the tree.
//  insert keeps the object in an internal store, but does not
//  immediately insert it. The object in the internal store are only inserted
//  when CompleteDelayedInsert is called or when one of the search functions
//  is invoked (they call CompleteDelayedInsert). When that is called, all
//  of the stored objects are then inserted into the list in a way designed
//  to give a relatively more balanced tree even if the data are strongly sorted.
//
//=======================================================================
void insert ( const T& t )
{
    m_ObjectStore    .push_back( t );
    m_ObjectCollide  .push_back( ULONG_MAX);
    m_DelayedIndices .push_back( (long)m_ObjectStore.size( ) - 1 );
    if ((m_Flags & NTF_NoDefer) && (m_DeepestDepth < 100)) {
        this->CompleteDelayedInsert();
    }
    
    m_DimEstimate = 0;
    m_DimEstimateEsd= 0;

};

//=======================================================================
//  insert( const iterator& i, const T& t )
//
//  dummy here just for compatibility with std::vector and std::list, etc.
//=======================================================================
void insert( const iterator& /*i*/, const T& t )
{
    insert( t );
}

//=======================================================================
// insert ( const InputContainer& o )
//
// Function to insert a containerful for data into a CNearTree. Standard
// Library containers and CNearTree's can be used.
//
//  insert keeps the object in an internal store, but does not
//  immediately insert it. The object in the internal store are only inserted
//  when CompleteDelayedInsert is called or when one of the search functions
//  is invoked (they call CompleteDelayedInsert). When that is called, all
//  of the stored objects are then inserted into the list in a way designed
//  to give a relatively more balanced tree even if the data are strongly sorted.
//
//=======================================================================
template< typename InputContainer >
void insert ( const InputContainer& o )
{
    typename InputContainer::const_iterator it;

    for( it=o.begin(); it!=o.end(); ++it )
    {
        m_ObjectStore    .push_back( *it );
        m_ObjectCollide  .push_back( ULONG_MAX);
        m_DelayedIndices .push_back( (long)m_ObjectStore.size( ) - 1 );
    }
        
    if ((m_Flags & NTF_NoDefer) && (m_DeepestDepth < 100)) {
        this->CompleteDelayedInsert();
    }
        
    m_DimEstimate = 0;
    m_DimEstimateEsd= 0;
        
}



//=======================================================================
//  void ImmediateInsert ( const T& t )
//
//  Function to insert some "point" as an object into a CNearTree for
//  later searching. Data is immediately put into the tree, instead of
//  being delayed (as function insert does). Use insert unless there is
//  some known need to insert immediately.
//
//     t is an object of the templated type which is to be inserted into a
//     NearTree
//
//  Three possibilities exist: put the datum into the left
//  position (first test),into the right position, or else
//  into a node descending from the nearer of those positions
//  when they are both already used.
//
//=======================================================================
void ImmediateInsert ( const T& t )
{
    size_t localDepth = 0;
    const long n = m_ObjectStore.size();
    m_ObjectStore.push_back(t);
    m_ObjectCollide.push_back(ULONG_MAX);
    if ( (m_Flags & NTF_ForceFlip) ) {
        m_BaseNode.InserterDelayed_Flip( n, localDepth, m_ObjectStore, m_ObjectCollide, m_SumSpacings, m_SumSpacingsSq );
    } else if ( !(m_Flags & NTF_NoFlip) ) { 
        m_BaseNode.InserterDelayed_Flip( n, localDepth, m_ObjectStore, m_ObjectCollide, m_SumSpacings, m_SumSpacingsSq );
    } else {
        m_BaseNode.InserterDelayed( n, localDepth, m_ObjectStore,
            m_ObjectCollide, m_SumSpacings, m_SumSpacingsSq );
    }
    m_DeepestDepth = std::max( localDepth, m_DeepestDepth );
    m_DiamEstimate = m_BaseNode.GetDiamEstimate(m_ObjectStore);
    m_DimEstimate = 0;
    m_DimEstimateEsd= 0;
}

//=======================================================================
// ImmediateInsert ( const InputContainer& o )
//
// see the description of ImmediateInsert above
//
//=======================================================================
template< typename InputContainer >
void ImmediateInsert ( const InputContainer& o )
{
    size_t localDepth = 0;
    typename InputContainer::const_iterator it;
    long n;

    if ( (m_Flags & NTF_ForceFlip) || !(m_Flags & NTF_NoFlip) ) {
        for( it=o.begin(); it!=o.end(); ++it )
        {
            n = m_ObjectStore.size();
            m_ObjectStore.push_back(*it);
            m_ObjectCollide.push_back(ULONG_MAX);
            localDepth = 0;
            m_BaseNode.InserterDelayed_Flip( n, localDepth, m_ObjectStore, m_ObjectCollide, m_SumSpacings, m_SumSpacingsSq );
            m_DeepestDepth = std::max( localDepth, m_DeepestDepth );
        }
    } else {
        for( it=o.begin(); it!=o.end(); ++it )
        {
            n = m_ObjectStore.size();
            m_ObjectStore.push_back(*it);
            localDepth = 0;
            m_BaseNode.InserterDelayed( n, localDepth, m_ObjectStore, m_ObjectCollide, m_SumSpacings, m_SumSpacingsSq );
            m_DeepestDepth = std::max( localDepth, m_DeepestDepth );
        }
    }
    m_DiamEstimate = m_BaseNode.GetDiamEstimate(m_ObjectStore);
    m_DimEstimate = 0;
    m_DimEstimateEsd= 0;
}

//=======================================================================
//  iterator NearestNeighbor ( const DistanceType &radius, const T& t ) const
//
//  Function to search a NearTree for the object closest to some probe point, t. This function
//  is only here so that the function Nearest can be called without having the radius const.
//  This was necessary because Nearest is recursive, but needs to keep the current smallest radius.
//
//    dRadius is the maximum search radius - any point farther than dRadius from the probe
//             point will be ignored
//    t  is the probe point
//
//    the return is an iterator to the templated type and is the returned nearest point
//             to the probe point (t) that can be found in the NearTree
//             or iterator::end if no point was found
//
//  This version used the balanced search
//=======================================================================
inline iterator NearestNeighbor ( const DistanceType& radius, const T& t ) const
{
    T closest;
    size_t index = ULONG_MAX;
    DistanceType tempRadius = radius;
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );

    if( this->empty( ) || radius < DistanceType( 0 ) )
    {
        return ( iterator(end( )) );
    }
    else if ( m_BaseNode.Nearest( tempRadius, closest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
, m_NodeVisits
#endif
                                 ) )
    {
        return ( iterator( (long)index, this ) );
    }
    else
    {
        return ( iterator(end( )) );
    }
}// NearestNeighbor

//=======================================================================
//  iterator LeftNearestNeighbor ( const DistanceType &radius, const T& t ) const
//
//  Function to search a NearTree for the object closest to some probe point, t. This function
//  is only here so that the function Nearest can be called without having the radius const.
//  This was necessary because Nearest is recursive, but needs to keep the current smallest radius.
//
//    dRadius is the maximum search radius - any point farther than dRadius from the probe
//             point will be ignored
//    t  is the probe point
//
//    the return is an iterator to the templated type and is the returned nearest point
//             to the probe point (t) that can be found in the NearTree
//             or iterator::end if no point was found
//
//  This version used the left-first search
//=======================================================================
inline iterator LeftNearestNeighbor ( const DistanceType& radius, const T& t ) const
{
    T closest;
    size_t index = ULONG_MAX;
    DistanceType tempRadius = radius;
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) || radius < DistanceType( 0 ) )
    {
        return ( iterator(end( )) );
    }
    else if ( m_BaseNode.LeftNearest( tempRadius, closest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                 , m_NodeVisits
#endif
                                 ) )
    {
        return ( iterator( (long)index, this ) );
    }
    else
    {
        return ( iterator(end( )) );
    }
}// LeftNearestNeighbor


//=======================================================================
//  bool NearestNeighbor ( const DistanceType& dRadius,  T& tClosest,   const T& t ) const
//
//  Function to search a NearTree for the object closest to some probe point, t. This function
//  is only here so that the function Nearest can be called without having the radius const.
//  This was necessary because Nearest is recursive, but needs to keep the current smallest radius.
//
//    dRadius is the maximum search radius - any point farther than dRadius from the probe
//             point will be ignored
//    tClosest is an object of the templated type and is the returned nearest point
//             to the probe point that can be found in the NearTree
//    t  is the probe point
//
//    the return value is true only if a point was found
//
//
//  This version used the balanced search
//=======================================================================
inline bool NearestNeighbor ( const DistanceType& dRadius,  T& tClosest,   const T& t ) const
{
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if ( dRadius < DistanceType(0) )
    {
        return ( false );
    }
    else if ( this->empty( ) )
    {
        return ( false );
    }
    else
    {
        DistanceType dSearchRadius = dRadius;
        size_t index = ULONG_MAX;
        return ( this->m_BaseNode.Nearest ( dSearchRadius, tClosest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                           , m_NodeVisits
#endif
                                           ) );
    }
}  //  NearestNeighbor

//=======================================================================
//  bool LeftNearestNeighbor ( const DistanceType& dRadius,  T& tClosest,   const T& t ) const
//
//  Function to search a NearTree for the object closest to some probe point, t. This function
//  is only here so that the function Nearest can be called without having the radius const.
//  This was necessary because Nearest is recursive, but needs to keep the current smallest radius.
//
//    dRadius is the maximum search radius - any point farther than dRadius from the probe
//             point will be ignored
//    tClosest is an object of the templated type and is the returned nearest point
//             to the probe point that can be found in the NearTree
//    t  is the probe point
//
//    the return value is true only if a point was found
//
//
//  This version used the left-first search
//=======================================================================
inline bool LeftNearestNeighbor ( const DistanceType& dRadius,  T& tClosest,   const T& t ) const
{
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if ( dRadius < DistanceType(0) )
    {
        return ( false );
    }
    else if ( this->empty( ) )
    {
        return ( false );
    }
    else
    {
        DistanceType dSearchRadius = dRadius;
        size_t index = ULONG_MAX;
        return ( this->m_BaseNode.LeftNearest ( dSearchRadius, tClosest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                           , m_NodeVisits
#endif
                                           ) );
    }
}  //  LeftNearestNeighbor


//=======================================================================
//  iterator ShortNearestNeighbor ( const DistanceType &radius, const T& t )
//
//  Function to search a NearTree for the object closest to some probe point, t. This function
//  is only here so that the function Nearest can be called without having the radius const.
//  This was necessary because Nearest is recursive, but needs to keep the current smallest radius.
//
//    dRadius is the maximum search radius - any point farther than dRadius from the probe
//             point will be ignored
//    t  is the probe point
//
//    the return is an iterator to the templated type and is the returned nearest point
//             to the probe point (t) that can be found in the NearTree
//             or iterator::end if no point was found
//
//  This version uses the short-radius balanced search
//=======================================================================
inline iterator ShortNearestNeighbor ( const DistanceType& radius, const T& t )
{
    T closest;
    size_t dimest;
    size_t index = ULONG_MAX;
    DistanceType tempRadius = radius;
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) || radius < DistanceType( 0 ) )
    {
        return ( iterator(end( )) );
    }
    else if (!(m_Flags & NTF_NoPrePrune) && (dimest=(this)->GetDimEstimate())>0) {
        DistanceType shortRadius = m_DiamEstimate/DistanceType((1+m_ObjectStore.size()));
        DistanceType limitRadius = 10*m_DiamEstimate/DistanceType((1+m_ObjectStore.size()));
        DistanceType meanSpacing = m_SumSpacings/DistanceType((1+m_ObjectStore.size()));
        DistanceType varSpacing = m_SumSpacingsSq/DistanceType((1+m_ObjectStore.size()))-meanSpacing*meanSpacing;
        if (limitRadius > radius/2.) limitRadius = radius/2.;
        if (limitRadius > meanSpacing/2.) limitRadius = meanSpacing/2.;
        double lineardensity=pow((double)m_ObjectStore.size(),1./(dimest));
        if (shortRadius > DistanceType( 0 ) &&
            ( (varSpacing < 0.25*meanSpacing*meanSpacing/(dimest)
               || (lineardensity*((double)meanSpacing) > 1.)          
               || (m_Flags & NTF_ForcePrePrune)))) {
            DistanceType testRadius;
            while (shortRadius <= limitRadius) {
                testRadius = shortRadius;
                if (m_BaseNode.Nearest ( testRadius, closest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                        , m_NodeVisits
#endif
                                        )) 
                    return iterator( (long)index, this );
                shortRadius *= DistanceType(10);
            }
        }
        if ( m_BaseNode.Nearest( tempRadius, closest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                , m_NodeVisits
#endif
                                ) )
        {
            return ( iterator( (long)index, this ) );
        }
        else
        {
            return ( iterator(end( )) );
        }        
    }
    else if ( m_BaseNode.Nearest( tempRadius, closest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                 , m_NodeVisits
#endif
                                 ) )
    {
        return ( iterator( (long)index, this ) );
    }
    else
    {
        return ( iterator(end( )) );
    }
} // end ShortNeartestNeighbor


//=======================================================================
//  bool ShortNearestNeighbor ( const DistanceType& dRadius,  T& tClosest,   const T& t ) const
//
//  Function to search a NearTree for the object closest to some probe point, t. This function
//  is only here so that the function Nearest can be called without having the radius const.
//  This was necessary because Nearest is recursive, but needs to keep the current smallest radius.
//
//    dRadius is the maximum search radius - any point farther than dRadius from the probe
//             point will be ignored
//    tClosest is an object of the templated type and is the returned nearest point
//             to the probe point that can be found in the NearTree
//    t  is the probe point
//
//    the return value is true only if a point was found
//
//    This version will handle the short radius pruning
//
//
//  This version uses the short-radius balanced search
//=======================================================================
inline bool ShortNearestNeighbor ( const DistanceType& dRadius,  T& tClosest,   const T& t )
{
    size_t index = ULONG_MAX;
    double dimest;
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    DistanceType dSearchRadius = dRadius;

    if ( dRadius < DistanceType(0) )
    {
        return ( false );
    }
    else if ( this->empty( ) )
    {
        return ( false );
    }
    else
    {
        if (!(m_Flags & NTF_NoPrePrune) && (dimest=(this)->GetDimEstimate())>0.) {
          DistanceType shortRadius = m_DiamEstimate/DistanceType((1+m_ObjectStore.size()));
          DistanceType limitRadius = 10.0*m_DiamEstimate/DistanceType((1+m_ObjectStore.size()));
          DistanceType meanSpacing = m_SumSpacings/DistanceType((1+m_ObjectStore.size()));
          DistanceType varSpacing = m_SumSpacingsSq/DistanceType((1+m_ObjectStore.size()))-meanSpacing*meanSpacing;
          if (limitRadius > dRadius/2.) limitRadius = dRadius/2.;
          if (limitRadius > meanSpacing/2.) limitRadius = meanSpacing/2.;
          bool bReturn;
          double lineardensity=pow((double)m_ObjectStore.size(),1./(dimest));
          if (shortRadius > DistanceType( 0 ) &&
              ( (varSpacing < 0.25*meanSpacing*meanSpacing/(dimest)
                || (lineardensity*((double)meanSpacing) > 1.)          
                || (m_Flags & NTF_ForcePrePrune)))) {
            DistanceType testRadius;
            while (shortRadius <= limitRadius) {
                testRadius = shortRadius;
                if (bReturn = this->m_BaseNode.Nearest ( testRadius, tClosest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                                        , m_NodeVisits
#endif
                                                        ), bReturn) return bReturn;
                shortRadius *= DistanceType(10);
            }
          }
        }
        return ( this->m_BaseNode.Nearest ( dSearchRadius, tClosest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                           , m_NodeVisits
#endif
                                           ) );
    }
}  //  end ShortNearestNeighbor


//=======================================================================
//  iterator LeftShortNearestNeighbor ( const DistanceType &radius, const T& t )
//
//  Function to search a NearTree for the object closest to some probe point, t. This function
//  is only here so that the function Nearest can be called without having the radius const.
//  This was necessary because Nearest is recursive, but needs to keep the current smallest radius.
//
//    dRadius is the maximum search radius - any point farther than dRadius from the probe
//             point will be ignored
//    t  is the probe point
//
//    the return is an iterator to the templated type and is the returned nearest point
//             to the probe point (t) that can be found in the NearTree
//             or iterator::end if no point was found
//
//  This version used the short-radius left-first search 
//=======================================================================
inline iterator LeftShortNearestNeighbor ( const DistanceType& radius, const T& t )
{
    T closest;
    size_t dimest;
    size_t index = ULONG_MAX;
    DistanceType tempRadius = radius;
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) || radius < DistanceType( 0 ) )
    {
        return ( iterator(end( )) );
    }
    else if (!(m_Flags & NTF_NoPrePrune) && (dimest=(this)->GetDimEstimate())>0) {
        DistanceType shortRadius = m_DiamEstimate/DistanceType((1+m_ObjectStore.size()));
        DistanceType limitRadius = 10*m_DiamEstimate/DistanceType((1+m_ObjectStore.size()));
        DistanceType meanSpacing = m_SumSpacings/DistanceType((1+m_ObjectStore.size()));
        DistanceType varSpacing = m_SumSpacingsSq/DistanceType((1+m_ObjectStore.size()))-meanSpacing*meanSpacing;
        if (limitRadius > radius/2.) limitRadius = radius/2.;
        if (limitRadius > meanSpacing/2.) limitRadius = meanSpacing/2.;
        double lineardensity=pow((double)m_ObjectStore.size(),1./(dimest));
        if (shortRadius > DistanceType( 0 ) &&
            ( (varSpacing < 0.25*meanSpacing*meanSpacing/(dimest)
               || (lineardensity*((double)meanSpacing) > 1.)          
               || (m_Flags & NTF_ForcePrePrune)))) {
            DistanceType testRadius;
            while (shortRadius <= limitRadius) {
                testRadius = shortRadius;
                if (m_BaseNode.Nearest ( testRadius, closest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                        , m_NodeVisits
#endif
                                        )) 
                    return iterator( (long)index, this );
                shortRadius *= DistanceType(10);
            }
        }
        if ( m_BaseNode.LeftNearest( tempRadius, closest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                , m_NodeVisits
#endif
                                ) )
        {
            return ( iterator( (long)index, this ) );
        }
        else
        {
            return ( iterator(end( )) );
        }        
    }
    else if ( m_BaseNode.LeftNearest( tempRadius, closest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                 , m_NodeVisits
#endif
                                 ) )
    {
        return ( iterator( (long)index, this ) );
    }
    else
    {
        return ( iterator(end( )) );
    }
} // sedn LeftShortNearestNeightbor


//=======================================================================
//  bool LeftShortNearestNeighbor ( const DistanceType& dRadius,  T& tClosest,   const T& t ) const
//
//  Function to search a NearTree for the object closest to some probe point, t. This function
//  is only here so that the function Nearest can be called without having the radius const.
//  This was necessary because Nearest is recursive, but needs to keep the current smallest radius.
//
//    dRadius is the maximum search radius - any point farther than dRadius from the probe
//             point will be ignored
//    tClosest is an object of the templated type and is the returned nearest point
//             to the probe point that can be found in the NearTree
//    t  is the probe point
//
//    the return value is true only if a point was found
//
//  This version uses the short-radius left-first search
//=======================================================================
inline bool LeftShortNearestNeighbor ( const DistanceType& dRadius,  T& tClosest,   const T& t )
{
    size_t index = ULONG_MAX;
    double dimest;
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    DistanceType dSearchRadius = dRadius;
    
    if ( dRadius < DistanceType(0) )
    {
        return ( false );
    }
    else if ( this->empty( ) )
    {
        return ( false );
    }
    else
    {
        if (!(m_Flags & NTF_NoPrePrune) && (dimest=(this)->GetDimEstimate())>0.) {
            DistanceType shortRadius = m_DiamEstimate/DistanceType((1+m_ObjectStore.size()));
            DistanceType limitRadius = 10.0*m_DiamEstimate/DistanceType((1+m_ObjectStore.size()));
            DistanceType meanSpacing = m_SumSpacings/DistanceType((1+m_ObjectStore.size()));
            DistanceType varSpacing = m_SumSpacingsSq/DistanceType((1+m_ObjectStore.size()))-meanSpacing*meanSpacing;
            if (limitRadius > dRadius/2.) limitRadius = dRadius/2.;
            if (limitRadius > meanSpacing/2.) limitRadius = meanSpacing/2.;
            bool bReturn;
            double lineardensity=pow((double)m_ObjectStore.size(),1./(dimest));
            if (shortRadius > DistanceType( 0 ) &&
                ( (varSpacing < 0.25*meanSpacing*meanSpacing/(dimest)
                   || (lineardensity*((double)meanSpacing) > 1.)          
                   || (m_Flags & NTF_ForcePrePrune)))) {
                DistanceType testRadius;
                while (shortRadius <= limitRadius) {
                    testRadius = shortRadius;
                    if ((bReturn = this->m_BaseNode.LeftNearest ( testRadius, tClosest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                                            , m_NodeVisits
#endif
                                                            ))) return bReturn;
                    shortRadius *= DistanceType(10);
                }
            }
        }
        return ( this->m_BaseNode.LeftNearest ( dSearchRadius, tClosest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                           , m_NodeVisits
#endif
                                           ) );
    }
}  //  LeftNearestNearestNeighbor


//=======================================================================
//  iterator FarthestNeighbor ( const T& t ) const
//
//  Function to search a NearTree for the object farthest from some probe point, t. This function
//  is only here so that the function Farthest can be called without having the radius const.
//  This was necessary because Farthest is recursive, but needs to keep the current largest radius.
//
//    t  is the probe point
//
//    the return is an iterator to the templated type and is the returned farthest point
//             from the probe point (t) that can be found in the NearTree
//             or iterator::end if no point was found
//
// This version uses the balanced search
//=======================================================================
iterator FarthestNeighbor ( const T& t ) const
{
    T farthest;
    size_t index = ULONG_MAX;
    DistanceType radius = DistanceType( distMinValue );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );

    if( this->empty( ) )
    {
        return ( iterator(this->end( )) );
    }
    else if ( m_BaseNode.Farthest( radius, farthest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                  , m_NodeVisits
#endif
                                  ) )
    {
        return ( iterator( (long)index, this ) );
    }
    else
    {
        return ( iterator(this->end( )) );
    }
}
//=======================================================================
//  iterator LeftFarthestNeighbor ( const T& t ) const
//
//  Function to search a NearTree for the object farthest from some probe point, t. This function
//  is only here so that the function Farthest can be called without having the radius const.
//  This was necessary because Farthest is recursive, but needs to keep the current largest radius.
//
//    t  is the probe point
//
//    the return is an iterator to the templated type and is the returned farthest point
//             from the probe point (t) that can be found in the NearTree
//             or iterator::end if no point was found
//
// This version uses the left-first search
//=======================================================================
iterator LeftFarthestNeighbor ( const T& t ) const
{
    T farthest;
    size_t index = ULONG_MAX;
    DistanceType radius = DistanceType( distMinValue );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
        return ( end( ) );
    }
    else if ( m_BaseNode.LeftFarthest( radius, farthest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                  , m_NodeVisits
#endif
                                  ) )
    {
        return ( iterator( (long)index, this ) );
    }
    else
    {
        return ( end( ) );
    }
}

//=======================================================================
//  bool FarthestNeighbor ( T& tFarthest, const T& t ) const
//
//  Function to search a NearTree for the object farthest from some probe point, t. This function
//  is only here so that the function FarthestNeighbor can be called without the user
//  having to input a search radius and so the search radius can be guaranteed to be
//  negative at the start.
//
//    tFarthest is an object of the templated type and is the returned farthest point
//             from the probe point that can be found in the NearTree
//    t  is the probe point
//
//    the return value is true only if a point was found (should only be false for
//             an empty tree)
//
//=======================================================================
bool FarthestNeighbor ( T& tFarthest, const T& t ) const
{
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );

    if ( this->empty( ) )
    {
        return ( false );
    }
    else
    {
        DistanceType dSearchRadius = DistanceType( distMinValue );
        size_t index = ULONG_MAX;
        return ( this->m_BaseNode.Farthest ( dSearchRadius, tFarthest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                            , m_NodeVisits
#endif
                                            ) );
    }
}  //  FarthestNeighbor

//=======================================================================
//  bool LeftFarthestNeighbor ( T& tFarthest, const T& t ) const
//
//  Function to search a NearTree for the object farthest from some probe point, t. This function
//  is only here so that the function FarthestNeighbor can be called without the user
//  having to input a search radius and so the search radius can be guaranteed to be
//  negative at the start.
//
//    tFarthest is an object of the templated type and is the returned farthest point
//             from the probe point that can be found in the NearTree
//    t  is the probe point
//
//    the return value is true only if a point was found (should only be false for
//             an empty tree)
//
//  This version uses the left-first search
//=======================================================================
bool LeftFarthestNeighbor ( T& tFarthest, const T& t ) const
{
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if ( this->empty( ) )
    {
        return ( false );
    }
    else
    {
        DistanceType dSearchRadius = DistanceType( distMinValue );
        size_t index = ULONG_MAX;
        return ( this->m_BaseNode.LeftFarthest ( dSearchRadius, tFarthest, t, index, m_ObjectStore
#ifdef CNEARTREE_INSTRUMENTED
                                            , m_NodeVisits
#endif
                                            ) );
    }
}  //  LeftFarthestNeighbor


//=======================================================================
template<typename ContainerType>
void BelongsToPoints( const T& t1, const T& t2, ContainerType& group1, ContainerType& group2 )
{
    group1.clear();
    group2.clear();
    typename CNearTree<T>::iterator it;

    for ( it=this->begin( ); it!=this->end( ); ++it )
    {
        if( DistanceBetween( (*it), t1 ) < DistanceBetween( (*it), t2) )
        {
            group1.insert( group1.end( ), (*it) );
        }
        else
        {
            group2.insert( group2.end(), (*it) );
        }
    }
}  // end BelongsToPoints
template<typename ContainerType>
void BelongsToPoints( const T& t1, const T& t2, ContainerType& group1, ContainerType& group2,
                     std::vector<size_t>& group1_ordinals, std::vector<size_t>& group2_ordinals)
{
    group1.clear();
    group2.clear();
    typename CNearTree<T>::iterator it;
    
    for ( it=this->begin( ); it!=this->end( ); ++it )
    {
        if( DistanceBetween( (*it), t1 ) < DistanceBetween( (*it), t2) )
        {
            group1.insert( group1.end( ), (*it) );
            group1_ordinals.insert( group1_ordinals.end( ), it.get_position( ) );
        }
        else
        {
            group2.insert( group2.end(), (*it) );
            group2_ordinals.insert( group2_ordinals.end( ), it.get_position( ) );
        }
    }
}  // end BelongsToPoints

//=======================================================================
template<typename ContainerTypeInside, typename ContainerTypeOutside>
void SeparateByRadius( const DistanceType radius, const T& probe, ContainerTypeInside& inside, ContainerTypeOutside& outside )
{
    inside.clear();
    outside.clear();
    typename CNearTree<T>::iterator it;

    for ( it=this->begin( ); it!=this->end( ); ++it )
    {
        if( DistanceBetween( (*it), probe ) < radius )
        {
            inside.insert( inside.end( ), (*it) );
        }
        else
        {
            outside.insert( outside.end(), (*it) );
        }
    }

    // The following was the first cut, but it's slower.
    //const long nInside  = FindInSphere ( radius, inside,  tProbe );
    //const long nOutside = FindOutSphere( radius, outside, tProbe );
} // end SeparateByRadius

//=======================================================================
template<typename ContainerTypeInside, typename ContainerTypeOutside>
void SeparateByRadius( const DistanceType radius, const T& probe, 
                      ContainerTypeInside& inside, ContainerTypeOutside& outside,
                      std::vector<size_t>& inside_ordinals, std::vector<size_t>& outside_ordinals)
{
    inside.clear();
    outside.clear();
    typename CNearTree<T>::iterator it;
    
    for ( it=this->begin( ); it!=this->end( ); ++it )
    {
        if( DistanceBetween( (*it), probe ) < radius )
        {
            inside.insert( inside.end( ), (*it) );
            inside_ordinals.insert( inside_ordinals.end( ), it.get_position());
        }
        else
        {
            outside.insert( outside.end(), (*it) );
            inside_ordinals.insert( outside_ordinals.end( ), it.get_position());
        }
    }
    
    // The following was the first cut, but it's slower.
    //const long nInside  = FindInSphere ( radius, inside,  tProbe );
    //const long nOutside = FindOutSphere( radius, outside, tProbe );
} // end SeparateByRadius

//=======================================================================
//  long FindInSphere ( const DistanceType& dRadius,  OutputContainerType& tClosest,   const T& t ) const
//
//  Function to search a NearTree for the set of objects closer to some probe point, t,
//  than dRadius. This is only here so that tClosest can be cleared before starting the work.
//
//    dRadius is the maximum search radius - any point farther than dRadius from the probe
//             point will be ignored
//    tClosest is returned as a container of objects of the templated type and is the
//             returned set of nearest points to the probe point that can be found
//             in the NearTree. The container can be a Standard Library container or
//             a CNearTree
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
//=======================================================================
template<typename OutputContainerType>
inline long FindInSphere ( const DistanceType& dRadius,  OutputContainerType& tClosest,   const T& t ) const
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tClosest.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );

    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        return ( m_BaseNode.InSphere( dRadius, tClosest, t,
                                     m_ObjectStore, m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                     , m_NodeVisits
#endif
                                     ) );
    }
}  //  FindInSphere
template<typename OutputContainerType>
inline long FindInSphere ( const DistanceType& dRadius,  OutputContainerType& tClosest, 
                          std::vector<size_t>& tIndices, const T& t ) const
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tClosest.clear( );
    tIndices.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        return ( m_BaseNode.InSphere( dRadius, tClosest, tIndices, t, m_ObjectStore, m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                     , m_NodeVisits
#endif
                                     ) );
    }
}  //  FindInSphere

//=======================================================================
//  long LeftFindInSphere ( const DistanceType& dRadius,  OutputContainerType& tClosest,   const T& t ) const
//
//  Function to search a NearTree for the set of objects closer to some probe point, t,
//  than dRadius. This is only here so that tClosest can be cleared before starting the work.
//
//    dRadius is the maximum search radius - any point farther than dRadius from the probe
//             point will be ignored
//    tClosest is returned as a container of objects of the templated type and is the
//             returned set of nearest points to the probe point that can be found
//             in the NearTree. The container can be a Standard Library container or
//             a CNearTree
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
// This version used the left-first search
//=======================================================================
template<typename OutputContainerType>
inline long LeftFindInSphere ( const DistanceType& dRadius, OutputContainerType& tClosest, const T& t ) const
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tClosest.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        return ( m_BaseNode.LeftInSphere( dRadius, tClosest, t,
                        m_ObjectStore, m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                     , m_NodeVisits
#endif
                                     ) );
    }
}  //  LeftFindInSphere

template<typename OutputContainerType>
inline long LeftFindInSphere ( const DistanceType& dRadius,  OutputContainerType& tClosest, 
                          std::vector<size_t>& tIndices, const T& t ) const
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tClosest.clear( );
    tIndices.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        return ( m_BaseNode.LeftInSphere( dRadius, tClosest, tIndices, t, m_ObjectStore, m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                     , m_NodeVisits
#endif
                                     ) );
    }
}  //  LeftFindInSphere

//=======================================================================
//  long FindOutSphere ( const DistanceType& dRadius,  OutputContainerType& tFarthest,   const T& t ) const
//
//  Function to search a NearTree for the set of objects farther from some probe point, t,
//  than dRadius. This is only here so that tFarthest can be cleared before starting the work.
//
//    dRadius is the maximum search radius - any point nearer than dRadius from the probe
//             point will be ignored
//    tFarthest is returned as a container of objects of the templated type and is the
//             returned set of nearest points to the probe point that can be found
//             in the NearTree. The container can be a Standard Library container or
//             a CNearTree
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
// This version uses the balanced search
//=======================================================================
template<typename OutputContainerType>
long FindOutSphere (
                    const DistanceType& dRadius,
                    OutputContainerType& tFarthest,
                    const T& t
                    ) const
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tFarthest.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );

    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        return ( m_BaseNode.OutSphere( dRadius, tFarthest, t,
                        m_ObjectStore, m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                      , m_NodeVisits
#endif
                                      ) );
    }
}  //  FindOutSphere
template<typename OutputContainerType>
long FindOutSphere (
                    const DistanceType& dRadius,
                    OutputContainerType& tFarthest,
                    std::vector<size_t>& tIndices,
                    const T& t
                    ) const
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tFarthest.clear( );
    tIndices.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        return ( m_BaseNode.OutSphere( dRadius, tFarthest, tIndices, t, m_ObjectStore, m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                      , m_NodeVisits
#endif
                                      ) );
    }
}  //  FindOutSphere

//=======================================================================
//  long leftFindOutSphere ( const DistanceType& dRadius,  OutputContainerType& tFarthest,   const T& t ) const
//
//  Function to search a NearTree for the set of objects farther from some probe point, t,
//  than dRadius. This is only here so that tFarthest can be cleared before starting the work.
//
//    dRadius is the maximum search radius - any point nearer than dRadius from the probe
//             point will be ignored
//    tFarthest is returned as a container of objects of the templated type and is the
//             returned set of nearest points to the probe point that can be found
//             in the NearTree. The container can be a Standard Library container or
//             a CNearTree
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
// This version uses the left-first search
//=======================================================================
template<typename OutputContainerType>
long LeftFindOutSphere (
                    const DistanceType& dRadius,
                    OutputContainerType& tFarthest,
                    const T& t
                    ) const
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tFarthest.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        return ( m_BaseNode.LeftOutSphere( dRadius, tFarthest, t,
                    m_ObjectStore, m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                      , m_NodeVisits
#endif
                                      ) );
    }
}  //  LeftFindOutSphere

template<typename OutputContainerType>
long LeftFindOutSphere (
                    const DistanceType& dRadius,
                    OutputContainerType& tFarthest,
                    std::vector<size_t>& tIndices,
                    const T& t
                    ) const
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tFarthest.clear( );
    tIndices.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        return ( m_BaseNode.LeftOutSphere( dRadius, tFarthest, tIndices, t, m_ObjectStore, m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                      , m_NodeVisits
#endif
                                      ) );
    }
}  //  LeftFindOutSphere


//=======================================================================
//  long FindInAnnulus ( const DistanceType& dRadius1, const DistanceType dRadius2, OutputContainerType& tAnnular, const T& t ) const
//
//  Function to search a NearTree for the set of objects within a "spherical" annulus
//
//    dRadius1 is the minimum search radius - any point nearer than dRadius1 from the probe
//             point will be ignored
//    dRadius2 is the maximum search radius - any point farther than dRadius2 from the probe
//             point will be ignored
//    tAnnular is returned as a container of objects of the templated type and is the
//             returned set of nearest points to the probe point that can be found
//             in the NearTree. The container can be a Standard Library container or
//             a CNearTree
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
// This version uses the balanced search
//=======================================================================
template<typename OutputContainerType>
long FindInAnnulus (
                    const DistanceType& dRadius1,
                    const DistanceType& dRadius2,
                    OutputContainerType& tAnnular,
                    const T& t
                    ) const
{
    long lReturn = 0;
    // clear the contents of the return vector so that things don't accidentally accumulate
    tAnnular.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );

    if( this->empty( ) )
    {
    }
    else if ( dRadius1 > dRadius2 )
    {
        // Make sure that r1 < r2
        return ( FindInAnnulus( dRadius2, dRadius1, tAnnular, t ) );
    }
    else
    {
        lReturn = this->m_BaseNode.InAnnulus( dRadius1, dRadius2, tAnnular, t, m_ObjectStore, m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                             , m_NodeVisits
#endif
                                             );
    }

    return ( lReturn );
}  //  FindInAnnulus
template<typename OutputContainerType>
long FindInAnnulus (
                    const DistanceType& dRadius1,
                    const DistanceType& dRadius2,
                    OutputContainerType& tAnnular,
                    std::vector<size_t>& tIndices,
                    const T& t
                    ) const
{
    long lReturn = 0;
    // clear the contents of the return vector so that things don't accidentally accumulate
    tAnnular.clear( );
    tIndices.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
    }
    else if ( dRadius1 > dRadius2 )
    {
        // Make sure that r1 < r2
        return ( FindInAnnulus( dRadius2, dRadius1, tAnnular, tIndices, t ) );
    }
    else
    {
        lReturn = this->m_BaseNode.InAnnulus( dRadius1, dRadius2, tAnnular, tIndices, t, m_ObjectStore, m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                             , m_NodeVisits
#endif
                                             );
    }
    
    return ( lReturn );
}  //  FindInAnnulus

//=======================================================================
//  long LeftFindInAnnulus ( const DistanceType& dRadius1, const DistanceType dRadius2, OutputContainerType& tAnnular, const T& t ) const
//
//  Function to search a NearTree for the set of objects within a "spherical" annulus
//
//    dRadius1 is the minimum search radius - any point nearer than dRadius1 from the probe
//             point will be ignored
//    dRadius2 is the maximum search radius - any point farther than dRadius2 from the probe
//             point will be ignored
//    tAnnular is returned as a container of objects of the templated type and is the
//             returned set of nearest points to the probe point that can be found
//             in the NearTree. The container can be a Standard Library container or
//             a CNearTree
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
// This version uses the left-first search
//=======================================================================
template<typename OutputContainerType>
long LeftFindInAnnulus (
                    const DistanceType& dRadius1,
                    const DistanceType& dRadius2,
                    OutputContainerType& tAnnular,
                    const T& t
                    ) const
{
    long lReturn = 0;
    // clear the contents of the return vector so that things don't accidentally accumulate
    tAnnular.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
    }
    else if ( dRadius1 > dRadius2 )
    {
        // Make sure that r1 < r2
        return ( LeftFindInAnnulus( dRadius2, dRadius1, tAnnular, t ) );
    }
    else
    {
        lReturn = this->m_BaseNode.LeftInAnnulus( dRadius1, dRadius2, tAnnular, t, m_ObjectStore, m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                             , m_NodeVisits
#endif
                                             );
    }
    
    return ( lReturn );
}  //  LeftFindInAnnulus
template<typename OutputContainerType>
long LeftFindInAnnulus (
                    const DistanceType& dRadius1,
                    const DistanceType& dRadius2,
                    OutputContainerType& tAnnular,
                    std::vector<size_t>& tIndices,
                    const T& t
                    ) const
{
    long lReturn = 0;
    // clear the contents of the return vector so that things don't accidentally accumulate
    tAnnular.clear( );
    tIndices.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
    }
    else if ( dRadius1 > dRadius2 )
    {
        // Make sure that r1 < r2
        return ( LeftFindInAnnulus( dRadius2, dRadius1, tAnnular, tIndices, t ) );
    }
    else
    {
        lReturn = this->m_BaseNode.LeftInAnnulus( dRadius1, dRadius2, tAnnular, tIndices, t, m_ObjectStore, m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                             , m_NodeVisits
#endif
                                             );
    }
    
    return ( lReturn );
}  //  LeftFindInAnnulus



//=======================================================================
//  long FindK_NearestNeighbors(  const size_t k, const DistanceType& dRadius, OutputContainerType& tClosest, const T& t ) const
//
//  Function to search a NearTree for the set of objects closer to some probe point, t,
//  than dRadius. This is only here so that tClosest can be cleared before starting the work
//   and radius can be updated while processing.
//
//    k is the maximum number of points to return
//    dRadius is the maximum search radius - any point farther than dRadius from the probe
//             point will be ignored
//    tClosest is returned as a container of objects of the templated type and is the
//             returned set of nearest points to the probe point that can be found
//             in the NearTree. The container can be a Standard Library container or
//             a CNearTree
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
//=======================================================================
template<typename OutputContainerType>
long FindK_NearestNeighbors ( const size_t k, const DistanceType& radius,  OutputContainerType& tClosest,   const T& t )
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tClosest.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );

    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        std::vector<std::pair<DistanceType, T> > K_Storage;
        DistanceType dRadius = radius;
        const long lFound = m_BaseNode.K_Near( k, dRadius, K_Storage, t, this->m_ObjectStore, this->m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                              , m_NodeVisits
#endif
                                              );
        for( size_t i=0; i<K_Storage.size( ); ++i )
        {
            tClosest.insert( tClosest.end( ), K_Storage[i].second );
        }
        return( lFound );
    }
}  //  FindK_NearestNeighbors
template<typename OutputContainerType>
long FindK_NearestNeighbors ( const size_t k, const DistanceType& radius,  
                             OutputContainerType& tClosest,
                             std::vector<size_t>& tIndices, const T& t )
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tClosest.clear( );
    tIndices.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        std::vector<triple<DistanceType, T, size_t> > K_Storage;
        DistanceType dRadius = radius;
        const long lFound = m_BaseNode.K_Near( k, dRadius, K_Storage, t, this->m_ObjectStore, this->m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                              , m_NodeVisits
#endif
                                              );
        for( size_t i=0; i<K_Storage.size( ); ++i )
        {
            tClosest.insert( tClosest.end( ), K_Storage[i].GetSecond() );
            tIndices.insert( tIndices.end( ), K_Storage[i].GetThird() );
        }
        return( lFound );
    }
}  //  FindK_NearestNeighbors

//=======================================================================
//  long FindK_NearestNeighbors(  const size_t k, const DistanceType& dRadius, OutputContainerType& tClosest, const T& t ) const
//
//  Function to search a NearTree for the set of objects closer to some probe point, t,
//  than dRadius. This is only here so that tClosest can be cleared before starting the work
//   and radius can be updated while processing.
//
//    k is the maximum number of points to return
//    dRadius is the maximum search radius - any point farther than dRadius from the probe
//             point will be ignored
//    tClosest is returned as a container of objects of the templated type and is the
//             returned set of nearest points to the probe point that can be found
//             in the NearTree. The container can be a Standard Library container or
//             a CNearTree
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
// This version uses the left-first search
//=======================================================================
template<typename OutputContainerType>
long LeftFindK_NearestNeighbors ( const size_t k, const DistanceType& radius,  OutputContainerType& tClosest,   const T& t )
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tClosest.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        std::vector<std::pair<DistanceType, T> > K_Storage;
        DistanceType dRadius = radius;
        const long lFound = m_BaseNode.LeftK_Near( k, dRadius, K_Storage, t, this->m_ObjectStore, this->m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                              , m_NodeVisits
#endif
                                              );
        for( size_t i=0; i<K_Storage.size( ); ++i )
        {
            tClosest.insert( tClosest.end( ), K_Storage[i].second );
        }
        return( lFound );
    }
}  //  FindK_NearestNeighbors
template<typename OutputContainerType>
long LeftFindK_NearestNeighbors ( const size_t k, const DistanceType& radius,  
                             OutputContainerType& tClosest,
                             std::vector<size_t>& tIndices, const T& t )
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tClosest.clear( );
    tIndices.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        std::vector<triple<DistanceType, T, size_t> > K_Storage;
        DistanceType dRadius = radius;
        const long lFound = m_BaseNode.LeftK_Near( k, dRadius, K_Storage, t, this->m_ObjectStore, this->m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                              , m_NodeVisits
#endif
                                              );
        for( size_t i=0; i<K_Storage.size( ); ++i )
        {
            tClosest.insert( tClosest.end( ), K_Storage[i].GetSecond() );
            tIndices.insert( tIndices.end( ), K_Storage[i].GetThird() );
        }
        return( lFound );
    }
}  //  LeftFindK_NearestNeighbors


//=======================================================================
//  long FindK_FarthestNeighbors const size_t k,OutputContainerType& tFarthest, const T& t ) const
//
//  Function to search a NearTree for the set of objects farthest from some probe point, t.
//  This is only here so that tClosest can be cleared before starting the work.
//
//    k is the maximum number of points to return
//    tClosest is returned as a container of objects of the templated type and is the
//             returned set of nearest points to the probe point that can be found
//             in the NearTree. The container can be a Standard Library container or
//             a CNearTree
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
// This version uses the balanced search
//=======================================================================
template<typename OutputContainerType>
long FindK_FarthestNeighbors ( const size_t k, OutputContainerType& tFarthest,   const T& t )
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tFarthest.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );

    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        std::vector<std::pair<DistanceType, T> > K_Storage;
        DistanceType dRadius = 0;
        const long lFound = m_BaseNode.K_Far( k, dRadius, K_Storage, t, this->m_ObjectStore, this->m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                             , m_NodeVisits
#endif
                                             );
        for( size_t i=0; i<K_Storage.size( ); ++i )
        {
            tFarthest.insert( tFarthest.end( ), K_Storage[i].second );
        }
        return( lFound );
    }
}  //  FindK_FarthestNeighbors
template<typename OutputContainerType>
long FindK_FarthestNeighbors ( const size_t k, OutputContainerType& tFarthest, std::vector<size_t>& tIndices, const T& t )
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tFarthest.clear( );
    tIndices.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        std::vector<triple<DistanceType, T, size_t> > K_Storage;
        DistanceType dRadius = 0;
        const long lFound = m_BaseNode.K_Far( k, dRadius, K_Storage, t, this->m_ObjectStore, this->m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                             , m_NodeVisits
#endif
                                             );
        for( size_t i=0; i<K_Storage.size( ); ++i )
        {
            tFarthest.insert( tFarthest.end( ), K_Storage[i].second);
            tIndices.insert( tIndices.end( ), K_Storage[i].third);
        }
        return( lFound );
    }
}  //  FindK_FarthestNeighbors

//=======================================================================
//  long FindK_FarthestNeighbors const size_t k,OutputContainerType& tFarthest, const T& t ) const
//
//  Function to search a NearTree for the set of objects farthest from some probe point, t.
//  This is only here so that tClosest can be cleared before starting the work.
//
//    k is the maximum number of points to return
//    tClosest is returned as a container of objects of the templated type and is the
//             returned set of nearest points to the probe point that can be found
//             in the NearTree. The container can be a Standard Library container or
//             a CNearTree
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
// This version uses the left-first search
//=======================================================================
template<typename OutputContainerType>
long LeftFindK_FarthestNeighbors ( const size_t k, OutputContainerType& tFarthest,   const T& t )
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tFarthest.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        std::vector<std::pair<DistanceType, T> > K_Storage;
        DistanceType dRadius = 0;
        const long lFound = m_BaseNode.LeftK_Far( k, dRadius, K_Storage, t, this->m_ObjectStore, this->m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                             , m_NodeVisits
#endif
                                             );
        for( size_t i=0; i<K_Storage.size( ); ++i )
        {
            tFarthest.insert( tFarthest.end( ), K_Storage[i].second );
        }
        return( lFound );
    }
}  //  LeftFindK_FarthestNeighbors

template<typename OutputContainerType>
long LeftFindK_FarthestNeighbors ( const size_t k, OutputContainerType& tFarthest, std::vector<size_t>& tIndices, const T& t )
{
    // clear the contents of the return vector so that things don't accidentally accumulate
    tFarthest.clear( );
    tIndices.clear( );
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    
    if( this->empty( ) )
    {
        return( 0L );
    }
    else
    {
        std::vector<triple<DistanceType, T, size_t> > K_Storage;
        DistanceType dRadius = 0;
        const long lFound = m_BaseNode.LeftK_Far( k, dRadius, K_Storage, t, this->m_ObjectStore, this->m_ObjectCollide
#ifdef CNEARTREE_INSTRUMENTED
                                             , m_NodeVisits
#endif
                                             );
        for( size_t i=0; i<K_Storage.size( ); ++i )
        {
            tFarthest.insert( tFarthest.end( ), K_Storage[i].second);
            tIndices.insert( tIndices.end( ), K_Storage[i].third);
        }
        return( lFound );
    }
}  //  LeftFindK_FarthestNeighbors


//=======================================================================
//  void CompleteDelayedInsert ( void )
//
//  When CompleteDelayedInsert is invoked, if there are any objects in the
//  delayed store they are then inserted into the neartree. CompleteDelayedInsert
//  randomly selects enough objects to partly fill a well-balanced tree. Specifically,
//  if there are n objects in the internal store, it randomly selects and inserts
//  sqrt(n) objects. After that, the remaining objects are inserted in a linear
//  sequence as they were entered.
//
//=======================================================================
inline void CompleteDelayedInsert ( void )
{
    if ( m_DelayedIndices.empty( ) )
    {
        return;
    }

    // insert a random selection of the objects
    const size_t vectorSize = m_DelayedIndices.size( );
    size_t ntarget = vectorSize;
    long npass;
    size_t errsize;
    long added;
    const size_t toRandomlyInsert = (size_t)::sqrt( (double)vectorSize );
    for ( size_t i=0; i<toRandomlyInsert; ++i )
    {

        size_t n = (size_t)((double)(vectorSize-1u) * (DistanceType)(rhr.urand()));
        rhr.urand( ); rhr.urand( );

        // Find the next pointer that hasn't already had its object "insert"ed
        // We can do this blindly since sqrt(n)<=n for all cases. n=1 would be the only
        // bad case here, and that will not trigger the later loop.
        while ( m_DelayedIndices[n] == -1 )
        {
            ++n;
            n = n% vectorSize;
        }
        insertDelayed( (long)m_DelayedIndices[n] );
        m_DelayedIndices[n] = -1;
        ntarget--;
    }

    npass=0;
    while (ntarget > 0) {
        npass++;
        errsize = (m_BaseNode.GetTreeSize())>>(-1+m_DeepestDepth);
        if ( errsize < 1 ) {
            size_t n = (size_t)((double)(vectorSize-1u) * (DistanceType)(rhr.urand()));
            rhr.urand( ); rhr.urand( );
            
            // Find the next pointer that hasn't already had its object "insert"ed
            // We can do this blindly since sqrt(n)<=n for all cases. n=1 would be the only
            // bad case here, and that will not trigger the later loop.
            while ( m_DelayedIndices[n] == -1 )
            {
                ++n;
                n = n% vectorSize;
            }
            insertDelayed( (long)m_DelayedIndices[n] );
            m_DelayedIndices[n] = -1;
            ntarget--;            
        }
        // finish by inserting all the remaining objects
        added=0;
        for ( size_t i=0; i<vectorSize; ++i )
        {
            if ( m_DelayedIndices[i] != -1 )
            {
                insertDelayed( (long)m_DelayedIndices[i] );
                m_DelayedIndices[i] = -1;
                ntarget--;
                added++;
                if (added > 100 || added > npass*8) {
                    errsize = (m_BaseNode.GetTreeSize())>>(-1+m_DeepestDepth);
                    if (errsize < 1) break; 
                }
            }
        }
    }

    // now get rid of the temporary storage that was used for delayed
    // insertions (fast way, faster than clear() )
    std::vector<long> DelayedPointersTemp;
    DelayedPointersTemp  .swap( m_DelayedIndices );
    m_DiamEstimate = m_BaseNode.GetDiamEstimate(m_ObjectStore);
};
//=======================================================================
//  void CompleteDelayedInsertRandom ( void )
//
//  When CompleteDelayedInsertRandom is invoked, if there are any objects in the
//  delayed store they are then inserted into the neartree. CompleteDelayedInsertRandom
//  randomly selects all objects and inserts them. m_DelayedIndices is empty after
//  a call to CompleteDelayedInsertRandom.
//
//=======================================================================
inline void CompleteDelayedInsertRandom ( void )
{
    if ( m_DelayedIndices.empty( ) )
    {
        return;
    }

    // insert a random selection of the objects
    const size_t vectorSize = m_DelayedIndices.size( );
    for ( size_t i=0; i<m_DelayedIndices.size( ); ++i )
    {

        size_t n = (size_t)((double)(vectorSize-1u) * (DistanceType)(rhr.urand()));
        rhr.urand( ); rhr.urand( );

        // Find the next pointer that hasn't already had its object "insert"ed
        // We can do this blindly since sqrt(n)<=n for all cases. n=1 would be the only
        // bad case here, and that will not trigger the later loop.
        while ( m_DelayedIndices[n] == -1 )
        {
            ++n;
            n = n% vectorSize;
        }
        insertDelayed( (long)m_DelayedIndices[n] );
        m_DelayedIndices[n] = -1;
    }

    // now get rid of the temporary storage that was used for delayed
    // insertions (fast way, faster than clear() )
    std::vector<long> DelayedPointersTemp;
    DelayedPointersTemp.swap( m_DelayedIndices );
    m_DiamEstimate = m_BaseNode.GetDiamEstimate(m_ObjectStore);
};

//=======================================================================
//  size_t GetDeferredSize (  void )
//
//  The number of objects currently queued for insertion.
//
//=======================================================================
size_t GetDeferredSize ( void ) const
{
    return ( m_DelayedIndices.size( ) );
};

//=======================================================================
//  DistanceType GetMeanSpacing (  void )
//
//  Get an estimate of the spacing of points
//  
//
//=======================================================================
DistanceType GetMeanSpacing (  void )
{
    return m_SumSpacings/DistanceType((1+m_ObjectStore.size()));
}

//=======================================================================
//  DistanceType GetVarSpacing (  void )
//
//  Get an estimate of variance of the spacing of points
//  
//
//=======================================================================
DistanceType GetVarSpacing (  void )
{
    DistanceType meanSpacing = m_SumSpacings/DistanceType((1+m_ObjectStore.size()));
    return m_SumSpacingsSq/DistanceType((1+m_ObjectStore.size()))-meanSpacing*meanSpacing;
}

//=======================================================================
//  size_t GetNodeVisits (  void )
//
//  Get the number of visits to nodes
//  Uninstrumented returns 0
//  
//
//=======================================================================
inline size_t GetNodeVisits (  void )
{
#ifdef CNEARTREE_INSTRUMENTED
    return m_NodeVisits;
#else
    return 0;
#endif
}

#ifdef CNEARTREE_INSTRUMENTED
//=======================================================================
//  void SetNodeVisits (  const size_t visits )
//
//  Set the number of visits to nodes
//  
//
//=======================================================================
void SetNodeVisits (  const size_t visits )
{
    m_NodeVisits = visits;
}
#endif    

//=======================================================================
//  DistanceType GetDiamEstimate (  void )
//
//  Get an estimate of the diameter
//  
//
//=======================================================================
DistanceType GetDiamEstimate (  void )
{
    return m_DiamEstimate;
}

//=======================================================================
//  double GetDimEstimate (  void )
//
//  Get an estimate of the dimension of the collection of points
//  in the tree.  If no argument, estimate to within esd of 0.1
//  
//
//=======================================================================
double GetDimEstimate ( void )
{
    return GetDimEstimate(0.1);
};


//=======================================================================
//  double GetDimEstimateEsd ( void )
//
//  Get the current best estimate of the dimension esd
//
//=======================================================================
double GetDimEstimateEsd ( void )
{
    if (m_DimEstimate <= 0.) {
      if (GetDimEstimate(0.1) <= 0.) return DBL_MAX;
    }
    return m_DimEstimateEsd;
} 


//=======================================================================
//  double GetDimEstimate (  const double DimEstimateEsd )
//
//  Get an estimate of the dimension of the collection of points
//  in the tree, to within the specified esd
//  
//
//=======================================================================
double GetDimEstimate ( const double DimEstimateEsd )
{
    const_cast<CNearTree*>(this)->CompleteDelayedInsert( );
    if ( m_DimEstimate == DBL_MAX ) return ( 0. );
    if ( m_DimEstimate > 0. 
        && (m_DimEstimateEsd <= DimEstimateEsd || DimEstimateEsd <= 0.) ) return (m_DimEstimate);
    size_t estsize = m_ObjectStore.size();
    std::vector<T> sampledisklarge;
    std::vector<T> sampledisksmall;
    size_t trials;
    double estd;
    double estdim = 0.;
    double estdimsq = 0.;
    double testlim = (DimEstimateEsd<=0.)?0.01:(DimEstimateEsd*DimEstimateEsd);
    DistanceType meanSpacing = m_SumSpacings/DistanceType((1+m_ObjectStore.size()));
    size_t n;
    long poplarge, popsmall, poptrial;
    T probe = m_ObjectStore[0];
    
    
    /*  Do not try to get a dimension extimate with fewer than
        32 points or a diameter less than DBL_EPSILON*/
    
    if (estsize < 32 || (double)m_DiamEstimate < DBL_EPSILON) {
        m_DimEstimate = m_DimEstimateEsd = DBL_MAX;
        return( 0 );
    }
    
    /* Estimate the number of points per unit distance
       and a target radius that would produce 4096 points
       in dimension 1.  If this would bring us beyond
       the diameter/1.1, reduce to that size.  */
    double pointdensity = ((double)estsize)/((double)m_DiamEstimate);
    double targetradius = 4096./pointdensity;
    double rat;
    double shrinkfactor;
    if (targetradius <  meanSpacing*10.) targetradius = meanSpacing*10.;
    if (targetradius > (double)m_DiamEstimate/1.1) targetradius = (double)m_DiamEstimate/1.1;
    
    /*  Now try to find a smaller adjusted target radius that will
     contain a reasonable number of points*/
    
    shrinkfactor = 4.;
    n = (size_t)(((double)estsize-1u) * ((DistanceType)rhr.urand()));
    rhr.urand( ); rhr.urand( );
    probe = m_ObjectStore[n];
    poptrial=FindInSphere((DistanceType)(targetradius/shrinkfactor),sampledisklarge,probe);
    do { 
        shrinkfactor = shrinkfactor/1.1;
        popsmall=poptrial;
        poptrial=FindInSphere((DistanceType)(targetradius/shrinkfactor),sampledisklarge,probe);
        n = (size_t)(((double)estsize-1u) * ((DistanceType)rhr.urand()));
        rhr.urand( ); rhr.urand( );
        probe = m_ObjectStore[n];
    } while (poptrial < 256 && shrinkfactor > 1. && poptrial <= popsmall+10);
    
    targetradius /= shrinkfactor;
    targetradius *= 1.1;

    long goodtrials = 0;
    trials = (size_t)sqrt(0.5+(double)estsize);
    if (trials < 10) trials = 10;
        
    n = (size_t)(((double)estsize-1u) * ((DistanceType)rhr.urand()));
    rhr.urand( ); rhr.urand( );
    probe = m_ObjectStore[n];
    
     
    for(size_t ii = 0; ii < trials; ii++) {
        n = (size_t)(((double)estsize-1u) * ((DistanceType)rhr.urand()));
        rhr.urand( ); rhr.urand( );
        probe = m_ObjectStore[n];
        
        if ((poplarge=FindInSphere((DistanceType)targetradius,sampledisklarge,probe))>0
            &&(popsmall=FindInSphere((DistanceType)(targetradius/1.1),sampledisksmall,probe))>0 
            && popsmall < poplarge)
        {
            rat = (double)poplarge/(double)popsmall;
            estd = log(rat)/log(1.1);
            estdim += estd;
            estdimsq += estd*estd;
            goodtrials++;
            if (goodtrials > (1L+(long)(trials))/2 && 
                fabs(estdimsq/((double)goodtrials) - estdim*estdim/((double)(goodtrials*goodtrials))) <= testlim) break;
        }
    }
    if (goodtrials < 1) {
        m_DimEstimate = m_DimEstimateEsd = DBL_MAX;
        return(0);
    }
    m_DimEstimate = estdim/((double)goodtrials);
    m_DimEstimateEsd = sqrt(fabs(estdimsq/((double)goodtrials) -  m_DimEstimate*m_DimEstimate));
    if (m_DimEstimate + 3.*m_DimEstimateEsd< 0.) {
        m_DimEstimate = m_DimEstimateEsd = DBL_MAX;
    }
    return(m_DimEstimate);
};


//=======================================================================
//  size_t GetTotalSize (  void )
//
//  The total number of objects that have been inserted plus those
//  queued for insertion.
//
//=======================================================================
size_t GetTotalSize ( void ) const
{
    return m_ObjectStore.size( );
};

//=======================================================================
//  size_t size ( void )
//
//  The total number of objects that have been inserted plus those
//  queued for insertion.
//
//=======================================================================
size_t size ( void ) const
{
    return ( GetTotalSize( ) );
};

//=======================================================================
//  size_t GetDepth ( void ) const
//
//  The greatest depth of the tree (1-based) from the root.
//
//=======================================================================
size_t GetDepth ( void ) const
{
    return ( m_DeepestDepth );
};

//=======================================================================
//  size_t GetHeight( void ) const
//
//  The greatest the height of the root.
//
//=======================================================================
size_t GetHeight ( void ) const
{
#ifdef CNEARTREE_INSTRUMENTED
    return ( m_BaseNode.GetTreeHeight() );
#else
    return ( m_DeepestDepth );
#endif
};


//=======================================================================
//  T Centroid ( void ) const
//
//  compute the centroid of a neartree
//
//=======================================================================
T Centroid( void ) const
{
    T t( T(0.0) );
    const size_t count = m_ObjectStore.size( );

    if ( count == 0 )
    {
    }
    else
    {
        for( size_t i=0; i<count; ++i )
        {
            t += m_ObjectStore[i];
        }
        t = T( DistanceType(t) / DistanceType(count) );
    }
    return( t );
}


//=======================================================================
//  T Centroid ( void ) const
//
//  compute the centroid of a neartree
//
//=======================================================================
template< typename CentroidContainerType >
static T Centroid(  CentroidContainerType& vt )
{
    T t( T(0.0) );
    const size_t count = vt.size( );

    if ( count == 0 )
    {
    }
    else
    {
        typename CentroidContainerType::iterator it;
        for ( it=vt.begin( ); it!=vt.end( ); ++it )
        {
            t += (*it);
        }
        t = T( DistanceType(t) / DistanceType(count) );
    }
    return( t );
}

//=======================================================================
//  std::vector<T> GetObjectStore ( void ) const
//
//  Utility function to copy the data object to a user's container object.
//
//=======================================================================
std::vector<T> GetObjectStore ( void ) const
{
    return ( m_ObjectStore );
}


//=======================================================================
//  operator ContainerType ( void ) const
//
//  Utility function to copy the data object to a user's container object.
//
//=======================================================================
template<typename ContainerType>
operator ContainerType ( void ) const
{
    return ( m_ObjectStore );
}

public:
iterator begin ( void ) { return ( iterator( 0, this ) ); };
iterator end   ( void ) { return ( iterator( m_ObjectStore.empty( )? 1 :(long)m_ObjectStore.size( )  , this ) ); };
iterator back  ( void ) { return ( iterator( m_ObjectStore.empty( )? 1 :(long)m_ObjectStore.size( )-1, this ) ); };
const_iterator begin ( void ) const { return ( const_iterator( 0, this ) ); };
const_iterator end   ( void ) const { return ( const_iterator( m_ObjectStore.empty( )? 1 :(long)m_ObjectStore.size( )  , this ) ); };
const_iterator back  ( void ) const { return ( const_iterator( m_ObjectStore.empty( )? 1 :(long)m_ObjectStore.size( )-1, this ) ); };

T at( const size_t n ) const { return ( m_ObjectStore[n] ); };
T operator[] ( const size_t position ) const { return ( m_ObjectStore[position] ); };


private:

//=======================================================================
void insertDelayed ( const long n )
{
    size_t localDepth = 0;
    if ((m_Flags & NTF_ForceFlip) || !(m_Flags & NTF_NoFlip)) {
        m_BaseNode.InserterDelayed_Flip( n, localDepth, m_ObjectStore,
                    m_ObjectCollide, m_SumSpacings, m_SumSpacingsSq );
    } else {
        m_BaseNode.InserterDelayed( n, localDepth, m_ObjectStore,
                    m_ObjectCollide, m_SumSpacings, m_SumSpacingsSq );
    }
    if ( localDepth > m_DeepestDepth ) m_DeepestDepth = localDepth;
}

//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//  end of CNearTree
//=======================================================================
template<typename U> class KT : private U
    {
    private:
        DistanceType d;
        KT( void ) : U() { };
    };



// start of nested class NearTreeNode
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================

//=======================================================================
//
// NEARTREENODE - nested class to hold the actual neartree and the indices to its data objects
//
// For the description of the template parameters, see the text above in the description
// of CNearTree.
//=======================================================================
template <typename TNode, typename DistanceTypeNode=DistanceType, int distMinValueNode=-1 >
class NearTreeNode
{
size_t            m_ptLeft;            // index of left object (of type TNode) stored in this node
size_t            m_ptRight;           // index of right object (of type TNode) stored in this node
DistanceTypeNode      m_dMaxLeft;      // longest distance from the left object to
// anything below it in the tree
DistanceTypeNode      m_dMaxRight;     // longest distance from the right object to
// anything below it in the tree
NearTreeNode *    m_pLeftBranch;       // tree descending from the left object
NearTreeNode *    m_pRightBranch;      // tree descending from the right object
size_t            m_iTreeSize;         // size of this node tree
#ifdef CNEARTREE_INSTRUMENTED
mutable size_t    m_iHeight;           // height of the tree
#endif


public:

NearTreeNode( void )   //  NearTreeNode constructor
: m_ptLeft            ( ULONG_MAX )
, m_ptRight           ( ULONG_MAX )
, m_dMaxLeft          ( DistanceTypeNode( distMinValueNode ) )
, m_dMaxRight         ( DistanceTypeNode( distMinValueNode ) )
, m_pLeftBranch       ( 0 )
, m_pRightBranch      ( 0 )
, m_iTreeSize         ( 0 )
#ifdef CNEARTREE_INSTRUMENTED
, m_iHeight           ( 0 )
#endif

{
};  //  NearTreeNode constructor

//=======================================================================
~NearTreeNode( void )  //  NearTreeNode destructor
{
    NearTreeNode * left;
    NearTreeNode * right;
    std::vector<NearTreeNode *> desStack;
    NearTreeNode * pc;
    
    pc = this;
    
    while ((void *)pc) {
        if (pc->m_ptRight != ULONG_MAX || pc->m_ptLeft !=ULONG_MAX ) {
            left = pc->m_pLeftBranch;
            right = pc->m_pRightBranch;
            pc->m_pLeftBranch  =0;
            pc->m_pRightBranch =0;
            if (pc != this) delete pc;
            if ( left ) desStack.push_back(left);
            if ( right ) desStack.push_back(right);
        }
        pc = 0;
        if (!desStack.empty()){
            pc = desStack.back();
            desStack.pop_back();
        }
    }
};  //  end NearTreeNode destructor

//=======================================================================
void clear( void )
{
    NearTreeNode * left;
    NearTreeNode * right;
    std::vector<NearTreeNode *> clearStack;
    NearTreeNode * pc;
    
    pc = this;
    
    while ((void *)pc) {
        if (pc->m_ptRight != ULONG_MAX || pc->m_ptLeft !=ULONG_MAX) {
        left = pc->m_pLeftBranch;
        right = pc->m_pRightBranch;
        pc->m_pLeftBranch  =0;
        pc->m_pRightBranch =0;
        pc->m_ptLeft     = ULONG_MAX;
        pc->m_ptRight    = ULONG_MAX;
        pc->m_dMaxLeft   = DistanceTypeNode( distMinValueNode );
        pc->m_dMaxRight  = DistanceTypeNode( distMinValueNode );
        pc->m_iTreeSize  = 0;
#ifdef CNEARTREE_INSTRUMENTED
        pc->m_iHeight    = 0;
#endif
            
        if (pc != this) delete pc;
        if ( left ) clearStack.push_back(left);
        if ( right ) clearStack.push_back(right);
        }
        pc = 0;
        if (!clearStack.empty()){
           pc = clearStack.back();
           clearStack.pop_back();
        }
    }
    

};  //  end clear

//=======================================================================

inline size_t GetTreeSize( void ) const
{
    return m_iTreeSize;
}

inline size_t GetLeftTreeSize( void ) const
{
    return (!m_pLeftBranch)?0:(m_pLeftBranch->m_iTreeSize);
}

inline size_t GetRightTreeSize( void ) const
{
    return (!m_pRightBranch)?0:(m_pRightBranch->m_iTreeSize);
}

inline size_t GetTreeHeight( void ) const
{
#ifdef CNEARTREE_INSTRUMENTED
    return m_iHeight;
#else
    return 0;
#endif
}


//=======================================================================
DistanceTypeNode GetDiamEstimate( std::vector<TNode>& objectStore ) const
{
    DistanceTypeNode temp = DistanceTypeNode(0);
    if (m_dMaxRight > temp) temp=m_dMaxRight;
    if (m_dMaxLeft >temp) temp=m_dMaxLeft;
    if (m_ptLeft != ULONG_MAX && m_ptRight != ULONG_MAX &&
        DistanceBetween(objectStore[m_ptLeft],objectStore[m_ptRight])> temp)
        temp= DistanceBetween(objectStore[m_ptLeft],objectStore[m_ptRight]);
    return temp;
}


//=======================================================================
//  void InserterDelayed_Flip ( const TNode& t, size_t& localDepth, std::vector<TNode>& objectStore, std::vector<size_t>& objectCollide,
//                  DistanceTypeNode& SumSpacings,  DistanceTypeNode& SumSpacingsSq )
//
//  Function to insert some "point" as an object into a CNearTree for
//  later searching
//
//     t is an object of the templated type which is to be inserted into a
//     NearTree
//
//     localDepth is the returned deepest tree level reached for the current insert
//
//  Three possibilities exist: put the datum into the left
//  position (first test),into the right position, or else
//  into a node descending from the nearer of those positions
//  when they are both already used.
//
//  This version will do a flip on insertions to try to drive the top pairs
//  apart, starting just before a terminal node
//
//=======================================================================
//=======================================================================
//  void InserterDelayed ( const TNode& t, size_t& localDepth, std::vector<TNode>& objectStore, std::vector<size_t>& objectCollide,
//                  DistanceTypeNode& SumSpacings,  DistanceTypeNode& SumSpacingsSq )
//
//  Function to insert some "point" as an object into a CNearTree for
//  later searching
//
//     t is an object of the templated type which is to be inserted into a
//     NearTree
//
//     localDepth is the returned deepest tree level reached for the current insert
//
//  Three possibilities exist: put the datum into the left
//  position (first test),into the right position, or else
//  into a node descending from the nearer of those positions
//  when they are both already used.
//
//=======================================================================


//=======================================================================
void InserterDelayed_Flip ( const long n, size_t& localDepth,
                           std::vector<TNode>& objectStore,
                           std::vector<size_t>& objectCollide,
                           DistanceTypeNode& SumSpacings,  DistanceTypeNode& SumSpacingsSq )
{
    DistanceTypeNode dTempRight =  DistanceTypeNode(0);
    DistanceTypeNode dTempLeft  =  DistanceTypeNode(0);
    DistanceTypeNode dTempLeftRight =  DistanceTypeNode(0);

    ++localDepth;
    ++m_iTreeSize;
    

    if ( m_ptLeft == ULONG_MAX )
    {
        m_ptLeft = n;
#ifdef CNEARTREE_INSTRUMENTED
        m_iHeight = 1;
#endif        
        return;
    }
    
    dTempLeft   = DistanceBetween( objectStore[n], objectStore[m_ptLeft]  );
    if ( dTempLeft < CNEARTREE_COLLIDE ) {
        size_t ctemp;
        ctemp = objectCollide[m_ptLeft];
        objectCollide[m_ptLeft] = n;
        return;
    }

    
    if ( m_ptRight == ULONG_MAX )
    {
        m_ptRight = n;
        SumSpacings += dTempLeft;
        SumSpacingsSq += dTempLeft*dTempLeft;
        return;
    }
    
    dTempRight  = DistanceBetween( objectStore[n], objectStore[m_ptRight] );
    if ( dTempRight < CNEARTREE_COLLIDE ) {
        size_t ctemp;
        ctemp = objectCollide[m_ptRight];
        objectCollide[m_ptRight] = n;
        return;
    }
    dTempLeftRight = DistanceBetween(objectStore[m_ptLeft], objectStore[m_ptRight]);

    if ( dTempLeft > dTempRight )
    {
        if ( m_pRightBranch == 0 ) {
            m_pRightBranch = new NearTreeNode;
        }
        // note that the next line assumes that m_dMaxRight is negative for a new node
        if ( m_dMaxRight < dTempRight ) m_dMaxRight = dTempRight;
        // If the left branch is empty, we are going to put something here
        if ( m_pRightBranch->m_ptLeft == ULONG_MAX ) {
            m_pRightBranch->m_ptLeft = n;
            SumSpacings += dTempRight;
            SumSpacingsSq += dTempRight*dTempRight;
            ++localDepth;
            ++(m_pRightBranch->m_iTreeSize);
#ifdef CNEARTREE_INSTRUMENTED
            m_pRightBranch->m_iHeight = 1;
            if (m_iHeight < 2) m_iHeight = 2;
#endif            
            // See if it would be better to put the new node at this level and drop the current
            // Right node down one level
            if (dTempRight > dTempLeftRight) {
                m_pRightBranch->m_ptLeft = m_ptRight;
                m_ptRight = n;
            }
            return;
        }        
        m_pRightBranch->InserterDelayed_Flip( n, localDepth,
                objectStore, objectCollide, SumSpacings, SumSpacingsSq );
#ifdef CNEARTREE_INSTRUMENTED
        m_iHeight = 1+m_pRightBranch->m_iHeight;
        if (m_pLeftBranch && m_pLeftBranch->m_iHeight >= m_iHeight) m_iHeight = 1+m_pLeftBranch->m_iHeight;
#endif
    }
    else  // ((DistanceTypeNode)(t - *m_tLeft) <= (DistanceTypeNode)(t - *m_tRight) )
    {
        if ( m_pLeftBranch  == 0 )  {
            m_pLeftBranch  = new NearTreeNode;
        }
        // note that the next line assumes that m_dMaxLeft is negative for a new node
        if ( m_dMaxLeft < dTempLeft ) m_dMaxLeft  = dTempLeft;
        // If the left branch is empty, we are going to put something here
        if ( m_pLeftBranch->m_ptLeft == ULONG_MAX ) {
            m_pLeftBranch->m_ptLeft = n;
            SumSpacings += dTempLeft;
            SumSpacingsSq += dTempLeft*dTempLeft;
            ++localDepth;
            ++(m_pLeftBranch->m_iTreeSize);
#ifdef CNEARTREE_INSTRUMENTED
            m_pLeftBranch->m_iHeight = 1;
            if (m_iHeight < 2) m_iHeight = 2;
#endif            
            // See if it would be better to put the new node at this level and drop the current
            // Left node down one level
            if (dTempLeft > dTempLeftRight) {
                m_pLeftBranch->m_ptLeft = m_ptLeft;
                m_ptLeft = n;
            }
            return;
        }        
        
        m_pLeftBranch->InserterDelayed_Flip( n, localDepth,
                objectStore, objectCollide, SumSpacings, SumSpacingsSq );
#ifdef CNEARTREE_INSTRUMENTED
        m_iHeight = 1+m_pLeftBranch->m_iHeight;
        if (m_pRightBranch && m_pRightBranch->m_iHeight >= m_iHeight) m_iHeight = 1+m_pRightBranch->m_iHeight;
#endif
    }
}  //   end InserterDelayed_Flip


//=======================================================================
    void InserterDelayed ( const long n, size_t& localDepth, std::vector<TNode>& objectStore, std::vector<size_t> objectCollide,
                      DistanceTypeNode& SumSpacings,  DistanceTypeNode& SumSpacingsSq )
{

    DistanceTypeNode dTempRight =  DistanceTypeNode(0);
    DistanceTypeNode dTempLeft  =  DistanceTypeNode(0);
    ++localDepth;
    ++m_iTreeSize;
    
    if ( m_ptLeft == ULONG_MAX )
    {
        m_ptLeft = n;
#ifdef CNEARTREE_INSTRUMENTED
        m_iHeight = 1;
#endif        
        return;
    }
    
    dTempLeft   = DistanceBetween( objectStore[n], objectStore[m_ptLeft]  );
    if ( dTempLeft < CNEARTREE_COLLIDE ) {
        size_t ctemp;
        ctemp = objectCollide[m_ptLeft];
        objectCollide[m_ptLeft] = n;
        return;
    }

    
    if ( m_ptRight == ULONG_MAX )
    {
        m_ptRight = n;
        SumSpacings += dTempLeft;
        SumSpacingsSq += dTempLeft*dTempLeft;
        return;
    }
        
    dTempRight  = DistanceBetween( objectStore[n], objectStore[m_ptRight] );

    if ( dTempRight < CNEARTREE_COLLIDE ) {
        size_t ctemp;
        ctemp = objectCollide[m_ptRight];
        objectCollide[m_ptRight] = n;
        return;
    }

    if ( dTempLeft > dTempRight )
    {

        if ( m_pRightBranch == 0 ) {
            m_pRightBranch = new NearTreeNode;
        }
        // note that the next line assumes that m_dMaxRight is negative for a new node
        if ( m_dMaxRight < dTempRight ) m_dMaxRight = dTempRight;
        if ( m_pRightBranch->m_ptLeft == ULONG_MAX) {
            m_pRightBranch->m_ptLeft = n;
            SumSpacings += dTempRight;
            SumSpacingsSq += dTempRight*dTempRight;
            ++localDepth;
            ++(m_pRightBranch->m_iTreeSize);
#ifdef CNEARTREE_INSTRUMENTED
            m_pRightBranch->m_iHeight = 1;
            if (m_iHeight < 2) m_iHeight = 2;
#endif                        
            return;
        }
        m_pRightBranch->InserterDelayed( n, localDepth, objectStore, objectCollide, SumSpacings, SumSpacingsSq );
#ifdef CNEARTREE_INSTRUMENTED
        m_iHeight = 1+m_pRightBranch->m_iHeight;
        if (m_pLeftBranch && m_pLeftBranch->m_iHeight >= m_iHeight) m_iHeight = 1+m_pLeftBranch->m_iHeight;
#endif
    }
    else  // ((DistanceTypeNode)(t - *m_tLeft) <= (DistanceTypeNode)(t - *m_tRight) )
    {
        if ( m_pLeftBranch  == 0 ) {
            m_pLeftBranch  = new NearTreeNode;
        }
        // note that the next line assumes that m_dMaxLeft is negative for a new node
        if ( m_dMaxLeft < dTempLeft ) m_dMaxLeft  = dTempLeft;
        if ( m_pLeftBranch->m_ptLeft == ULONG_MAX ) {
            m_pLeftBranch->m_ptLeft = n;
            SumSpacings += dTempLeft;
            SumSpacingsSq += dTempLeft*dTempLeft;
            ++localDepth;
            ++(m_pLeftBranch->m_iTreeSize);
#ifdef CNEARTREE_INSTRUMENTED
            m_pLeftBranch->m_iHeight = 1;
            if (m_iHeight < 2) m_iHeight = 2;
#endif            
            return;
        }        
        m_pLeftBranch->InserterDelayed( n, localDepth,
            objectStore, objectCollide, SumSpacings, SumSpacingsSq );
#ifdef CNEARTREE_INSTRUMENTED
        m_iHeight = 1+m_pLeftBranch->m_iHeight;
        if (m_pRightBranch && m_pRightBranch->m_iHeight >= m_iHeight) m_iHeight = 1+m_pRightBranch->m_iHeight;
#endif
    }
}  //   end InserterDelayed

//=======================================================================
//  void ReInserter_Flip ( const NearTreeNode * pntn, size_t& localDepth, std::vector<TNode>& objectStore ),
//
//  Function to reinsert the elements from the object store  
//  contained in a NearTreeNode into a CNearTree for later searching
//
//     pntn is a point to a neartree node, the objects from which are to be re-inserted into a
//     NearTree
//
//     localDepth is the returned deepest tree level reached for the current insert
//
//  This version will do a flip on insertions to try to drive the top pairs
//  apart
//
//=======================================================================
void ReInserter_Flip ( const NearTreeNode * pntn, size_t& localDepth, std::vector<TNode>& objectStore)
{   
    size_t tempdepth1, tempdepth2, tempdepth3, tempdepth4;
    
    DistanceTypeNode SumSpacings, SumSpacingsSq;
    
    tempdepth1 = tempdepth2 = tempdepth3 = tempdepth4 = localDepth;
    
    if ( pntn->m_ptRight != ULONG_MAX) 
        InserterDelayed_Flip( pntn->m_ptRight, tempdepth1, objectStore, SumSpacings, SumSpacingsSq );
    
    if ( pntn->m_ptLeft != ULONG_MAX) 
        InserterDelayed_Flip( pntn->m_ptLeft, tempdepth2, objectStore, SumSpacings, SumSpacingsSq );

    if ( pntn->m_pLeftBranch ) ReInserter_Flip(pntn->m_pLeftBranch, tempdepth3, objectStore );

    if ( pntn->m_pRightBranch ) ReInserter_Flip(pntn->m_pRightBranch, tempdepth4, objectStore );
    
    localDepth = tempdepth1>localDepth?tempdepth1:localDepth;
    localDepth = tempdepth2>localDepth?tempdepth2:localDepth;
    localDepth = tempdepth3>localDepth?tempdepth3:localDepth;
    localDepth = tempdepth4>localDepth?tempdepth4:localDepth;
    
}  //  end ReInserter_Flip


//=======================================================================
//  bool Nearest ( DistanceTypeNode& dRadius,  TNode& tClosest,   const TNode& t,
//                 const std::vector<TNode>& objectStore) const
//
//  Private function to search a NearTree for the object closest to some probe point, t.
//  This function is only called by NearestNeighbor.
//
//    dRadius is the smallest currently known distance of an object from the probe point.
//    tClosest is an object of the templated type and is the returned closest point
//             to the probe point that can be found in the NearTree
//    t  is the probe point
//    objectStore is the complete object store of the NearTree
//
//    the return value is true only if a point was found within dRadius
//
//=======================================================================
bool Nearest (
              DistanceTypeNode& dRadius,
              TNode& tClosest,
              const TNode& t,
              size_t& pClosest,
              const std::vector<TNode>& objectStore
#ifdef CNEARTREE_INSTRUMENTED
              , size_t& VisitCount
#endif
              ) const
{
    std::vector <NearTreeNode* > sStack;
    DistanceTypeNode dDL=DistanceTypeNode(0), dDR=DistanceTypeNode(0);
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
    pClosest = ULONG_MAX;
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif                
    if ( pt->m_ptLeft == ULONG_MAX &&  pt->m_ptRight == ULONG_MAX) return false; // test for empty
    while ( pt->m_ptLeft != ULONG_MAX ||
           pt->m_ptRight != ULONG_MAX ||
           !sStack.empty( ) ) 
    {
        if (pt->m_ptLeft == ULONG_MAX && pt->m_ptRight == ULONG_MAX) {
            if (!sStack.empty( )) {
                pt = sStack.back();
                sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                            
                continue;
            }
            break;
        }
        if (pt->m_ptLeft != ULONG_MAX) {
            dDL = DistanceBetween( t, objectStore[pt->m_ptLeft] );
            if ( dDL <= dRadius )
            {
                dRadius = dDL;
                pClosest = pt->m_ptLeft;
            }            
        }
        if (pt->m_ptRight != ULONG_MAX) {
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                                            
            dDR = DistanceBetween( t, objectStore[pt->m_ptRight]);
            if ( dDR <= dRadius )
            {
                dRadius = dDR;
                pClosest = pt->m_ptRight;
            }            
        }
        
        /*
         See if both branches are populated.  In that case, save one branch
         on the stack, and process the other one based on which one seems
         smaller, but useful first]
         */
        if (pt->m_pLeftBranch != 0 && pt->m_pRightBranch != 0 ) {
            if (dDL+pt->m_dMaxLeft < dDR+pt->m_dMaxRight || pt->m_pRightBranch == 0) {
                if ( TRIANG(dDL,pt->m_dMaxLeft,dRadius)) {
                    if ( TRIANG(dDR,pt->m_dMaxRight,dRadius)) {
                        sStack.push_back(pt->m_pRightBranch);
                    }
                    pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                    ++VisitCount;
#endif                                
                    continue;
                }
                /* If we are here, the left branch was not useful
                   Fall through to use the right
                 */
            }
            
            /* We come here either because pursuing the left branch was not useful
               of the right branch look shorter
             */
            if ( TRIANG(dDR,pt->m_dMaxRight,dRadius)) {
                if ( TRIANG(dDL,pt->m_dMaxLeft,dRadius)) {
                    sStack.push_back(pt->m_pLeftBranch);
                }
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            } 
        }
        
        /* Only one branch is viable, try them one at a time
         */
        if ( pt->m_pLeftBranch != 0 && TRIANG(dDL,pt->m_dMaxLeft,dRadius)) {
            pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        if ( pt->m_pRightBranch != 0 && TRIANG(dDR,pt->m_dMaxRight,dRadius)) {
            pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        /* We have procesed both sides, we need to go to the stack */
        
        if (!sStack.empty( )) {
            pt = sStack.back();
            sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        break;
    }
    if ( !sStack.empty( ) ) // for safety !!!
    {
        std::vector <NearTreeNode* > sTemp;
        sTemp.swap( sStack );
    }
    if ( pClosest != ULONG_MAX )
        tClosest = objectStore[pClosest];
    return ( pClosest != ULONG_MAX );
};   // end Nearest


//=======================================================================
//  bool LeftNearest ( DistanceTypeNode& dRadius,  TNode& tClosest,   const TNode& t,
//                 const std::vector<TNode>& objectStore) const
//
//  Private function to search a NearTree for the object closest to some probe point, t.
//  This function is only called by NearestNeighbor.
//
//    dRadius is the smallest currently known distance of an object from the probe point.
//    tClosest is an object of the templated type and is the returned closest point
//             to the probe point that can be found in the NearTree
//    t  is the probe point
//    objectStore is the complete object store of the NearTree
//
//    the return value is true only if a point was found within dRadius
//
//    This version differs from Nearest by searching to the left first
//
//=======================================================================
bool LeftNearest (
              DistanceTypeNode& dRadius,
              TNode& tClosest,
              const TNode& t,
              size_t& pClosest,
              const std::vector<TNode>& objectStore
#ifdef CNEARTREE_INSTRUMENTED
              , size_t& VisitCount
#endif
              ) const
{
    std::vector <NearTreeNode* > sStack;
    enum  { left, right, end } eDir;
    eDir = left; // examine the left nodes first
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
    pClosest = ULONG_MAX;
    if ( pt->m_ptLeft == ULONG_MAX) return false; // test for empty
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif            
    while ( ! ( eDir == end && sStack.empty( ) ) )
    {
        if ( eDir == right )
        {
            const DistanceTypeNode dDR = DistanceBetween( t, objectStore[pt->m_ptRight] );
            if ( dDR <= dRadius )
            {
                dRadius = dDR;
                pClosest = pt->m_ptRight;
            }
            if ( pt->m_pRightBranch != 0 && TRIANG(dDR,pt->m_dMaxRight,dRadius))
            { // we did the left and now we finished the right, go down
                pt = pt->m_pRightBranch;
                eDir = left;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
            }
            else
            {
                eDir = end;
            }
        }
        if ( eDir == left )
        {
            const DistanceTypeNode dDL = DistanceBetween( t, objectStore[pt->m_ptLeft]  );
            if ( dDL <= dRadius )
            {
                dRadius = dDL;
                pClosest = pt->m_ptLeft;
            }
            if ( pt->m_ptRight != ULONG_MAX ) // only stack if there's a right object
            {
                sStack.push_back( pt );
            }
            if ( pt->m_pLeftBranch != 0 && TRIANG(dDL,pt->m_dMaxLeft,dRadius))
            { // we did the left, go down
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
                pt = pt->m_pLeftBranch;
            }
            else
            {
                eDir = end;
            }
        }
        
        if ( eDir == end && !sStack.empty( ) )
        {
            pt = sStack.back( );
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif            
            sStack.pop_back( );
            eDir = right;
        }
    }
    if ( !sStack.empty( ) ) // for safety !!!
    {
        std::vector <NearTreeNode* > sTemp;
        sTemp.swap( sStack );
    }
    if ( pClosest != ULONG_MAX )
        tClosest = objectStore[pClosest];
    return ( pClosest != ULONG_MAX );
};   // end LeftNearest

//=======================================================================
//  bool Farthest ( DistanceTypeNode& dRadius,  TNode& tFarthest,   const TNode& t ) const
//
//  Private function to search a NearTree for the object farthest from some probe point, t.
//  This function is only called by FarthestNeighbor.
//
//    dRadius is the largest currently known distance of an object from the probe point.
//    tFarthest is an object of the templated type and is the returned farthest point
//             from the probe point that can be found in the NearTree
//    t  is the probe point
//
//    the return value is true only if a point was found (should only be false for
//             an empty tree)
//
//=======================================================================
bool Farthest (
               DistanceTypeNode& dRadius,
               TNode& tFarthest,
               const TNode& t, size_t& pFarthest,
               const std::vector<TNode>& objectStore
#ifdef CNEARTREE_INSTRUMENTED
               , size_t& VisitCount
#endif
               ) const
{
    std::vector <NearTreeNode* > sStack;
    DistanceTypeNode dDL=DistanceTypeNode(0), dDR=DistanceTypeNode(0);
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
    pFarthest = ULONG_MAX;
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif                
    if ( pt->m_ptLeft == ULONG_MAX &&  pt->m_ptRight == ULONG_MAX) return false; // test for empty
    while ( pt->m_ptLeft != ULONG_MAX ||
           pt->m_ptRight != ULONG_MAX ||
           !sStack.empty( ) ) 
    {
        if (pt->m_ptLeft == ULONG_MAX && pt->m_ptRight == ULONG_MAX) {
            if (!sStack.empty( )) {
                pt = sStack.back();
                sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            }
            break;
        }
        if (pt->m_ptLeft != ULONG_MAX) {
            dDL = DistanceBetween( t, objectStore[pt->m_ptLeft] );
            if ( dDL >= dRadius )
            {
                dRadius = dDL;
                pFarthest = pt->m_ptLeft;
            }            
        }
        if (pt->m_ptRight != ULONG_MAX) {
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                                                        
            dDR = DistanceBetween( t, objectStore[pt->m_ptRight]);
            if ( dDR >= dRadius )
            {
                dRadius = dDR;
                pFarthest = pt->m_ptRight;
            }            
        }
        
        /*
         See if both branches are populated.  In that case, save one branch
         on the stack, and process the other one based on which one seems
         larger, but useful first]
         */
        if (pt->m_pLeftBranch != 0 && pt->m_pRightBranch != 0 ) {
            if (dDL+pt->m_dMaxLeft > dDR+pt->m_dMaxRight || pt->m_pRightBranch == 0) {
                if ( TRIANG(dRadius,dDL,pt->m_dMaxLeft)) {
                    if ( TRIANG(dRadius,dDR,pt->m_dMaxRight)) {
                        sStack.push_back(pt->m_pRightBranch);
                    }
                    pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                    ++VisitCount;
#endif                                
                    continue;
                }
                /* If we are here, the left branch was not useful
                 Fall through to use the right
                 */
            }
            
            /* We come here either because pursuing the left branch was not useful
             of the right branch look shorter
             */
            if ( TRIANG(dRadius,dDR,pt->m_dMaxRight)) {
                if ( TRIANG(dRadius,dDL,pt->m_dMaxLeft)) {
                    sStack.push_back(pt->m_pLeftBranch);
                }
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            } 
        }
        
        /* Only one branch is viable, try them one at a time
         */
        if ( pt->m_pLeftBranch != 0 && TRIANG(dRadius,dDL,pt->m_dMaxLeft)) {
            pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        if ( pt->m_pRightBranch != 0 && TRIANG(dRadius,dDR,pt->m_dMaxRight)) {
            pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        /* We have procesed both sides, we need to go to the stack */
        
        if (!sStack.empty( )) {
            pt = sStack.back();
            sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        break;
    }
    if ( !sStack.empty( ) ) // for safety !!!
    {
        std::vector <NearTreeNode* > sTemp;
        sTemp.swap( sStack );
    }
    if ( pFarthest != ULONG_MAX )
        tFarthest = objectStore[pFarthest];
    return ( pFarthest != ULONG_MAX );
};   // end Farthest
            
//=======================================================================
//  bool LeftFarthest ( DistanceTypeNode& dRadius,  TNode& tFarthest,   const TNode& t ) const
//
//  Private function to search a NearTree for the object farthest from some probe point, t.
//  This function is only called by FarthestNeighbor.
//
//    dRadius is the largest currently known distance of an object from the probe point.
//    tFarthest is an object of the templated type and is the returned farthest point
//             from the probe point that can be found in the NearTree
//    t  is the probe point
//
//    the return value is true only if a point was found (should only be false for
//             an empty tree)
//
//    This version differs from Farthest by searching to the left first
//
//=======================================================================
bool LeftFarthest (
               DistanceTypeNode& dRadius,
               TNode& tFarthest,
               const TNode& t, size_t& pFarthest,
               const std::vector<TNode>& objectStore
#ifdef CNEARTREE_INSTRUMENTED
               , size_t& VisitCount
#endif
               ) const
{
    std::vector <NearTreeNode* > sStack;
    enum  { left, right, end } eDir;
    eDir = left; // examine the left nodes first
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
    pFarthest = ULONG_MAX;
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif            
    if (pt->m_ptLeft == ULONG_MAX) return false; // test for empty
    while ( ! ( eDir == end && sStack.empty( ) ) )
    {
        if ( eDir == right )
        {
            const DistanceTypeNode dDR = DistanceBetween( t , objectStore[pt->m_ptRight] );
            if ( dDR >= dRadius )
            {
                dRadius = dDR;
                pFarthest = pt->m_ptRight;
            }
            if ( pt->m_pRightBranch != 0 && TRIANG(dRadius,dDR,pt->m_dMaxRight))
            { // we did the left and now we finished the right, go down
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                            
                eDir = left;
            }
            else
            {
                eDir = end;
            }
        }
        if ( eDir == left )
        {
            const DistanceTypeNode dDL = DistanceBetween( t , objectStore[pt->m_ptLeft] );
            if ( dDL >= dRadius )
            {
                dRadius = dDL;
                pFarthest = pt->m_ptLeft;
            }
            if ( pt->m_ptRight != ULONG_MAX ) // only stack if there's a right object
            {
                sStack.push_back( pt );
            }
            if ( pt->m_pLeftBranch != 0 && TRIANG(dRadius,dDL,pt->m_dMaxLeft) )
            { // we did the left, go down
                pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                            
            }
            else
            {
                eDir = end;
            }
        }
        
        if ( eDir == end && !sStack.empty( ) )
        {
            pt = sStack.back( );
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            sStack.pop_back( );
            eDir = right;
        }
    }
    if ( !sStack.empty( ) ) // for safety !!!
    {
        std::vector <NearTreeNode* > sTemp;
        sTemp.swap( sStack );
    }
    if ( pFarthest != ULONG_MAX )
        tFarthest = objectStore[pFarthest];
    return ( pFarthest != ULONG_MAX );
};   //  end LeftFarthest


//=======================================================================
//  long InSphere (
//                const DistanceTypeNode& dRadius,
//                ContainerType& tClosest,
//                const TNode& t,
//                const std::vector<TNode>& objectStore,
//                const std::vector<size_t>& objectCollide
//                ) const
//
//  Private function to search a NearTree for the objects inside of the specified radius
//     from the probe point
//  This function is only called by FindInSphere.
//
//    dRadius is the search radius
//    tClosest is a CNearTree of objects of the templated type found within dRadius of the
//         probe point
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
//=======================================================================
template<typename ContainerType>
long InSphere (
               const DistanceTypeNode& dRadius,
               ContainerType& tClosest,
               const TNode& t,
               const std::vector<TNode>& objectStore,
               const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
               , size_t& VisitCount
#endif
               ) const
{
    std::vector <NearTreeNode* > sStack;
    DistanceTypeNode dDL=DistanceTypeNode(0), dDR=DistanceTypeNode(0);
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif                            
    if ( pt->m_ptLeft == ULONG_MAX &&  pt->m_ptRight == ULONG_MAX) return false; // test for empty
    while ( pt->m_ptLeft != ULONG_MAX ||
           pt->m_ptRight != ULONG_MAX ||
           !sStack.empty( ) ) 
    {
        if (pt->m_ptLeft == ULONG_MAX && pt->m_ptRight == ULONG_MAX) {
            if (!sStack.empty( )) {
                pt = sStack.back();
                sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            }
            break;
        }
        if (pt->m_ptLeft != ULONG_MAX) {
            dDL = DistanceBetween( t, objectStore[pt->m_ptLeft] );
            if ( dDL <= dRadius )
            {
                size_t collide;
                tClosest.insert( tClosest.end(), objectStore[pt->m_ptLeft] );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), objectStore[objectCollide[collide]] );
                    collide = objectCollide[collide];
                }
            }            
        }
        if (pt->m_ptRight != ULONG_MAX) {
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                                            
            dDR = DistanceBetween( t, objectStore[pt->m_ptRight]);
            if ( dDR <= dRadius )
            {
                size_t collide;
                tClosest.insert( tClosest.end(), objectStore[pt->m_ptRight] );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), objectStore[objectCollide[collide]] );
                    collide = objectCollide[collide];
                }

            }
        }
        
        /*
         See if both branches are populated.  In that case, save one branch
         on the stack, and process the other one based on which one seems
         smaller, but useful first]
         */
        if (pt->m_pLeftBranch != 0 && pt->m_pRightBranch != 0 ) {
            if (dDL+pt->m_dMaxLeft < dDR+pt->m_dMaxRight || pt->m_pRightBranch == 0) {
                if ( TRIANG(dDL,pt->m_dMaxLeft,dRadius)) {
                    if ( TRIANG(dDR,pt->m_dMaxRight,dRadius)) {
                        sStack.push_back(pt->m_pRightBranch);
                    }
                    pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                    ++VisitCount;
#endif                                
                    continue;
                }
                /* If we are here, the left branch was not useful
                 Fall through to use the right
                 */
            }
            
            /* We come here either because pursuing the left branch was not useful
             of the right branch look shorter
             */
            if ( TRIANG(dDR,pt->m_dMaxRight,dRadius)) {
                if ( TRIANG(dDL,pt->m_dMaxLeft,dRadius)) {
                    sStack.push_back(pt->m_pLeftBranch);
                }
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            } 
        }
        
        /* Only one branch is viable, try them one at a time
         */
        if ( pt->m_pLeftBranch != 0 && TRIANG(dDL,pt->m_dMaxLeft,dRadius)) {
            pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        if ( pt->m_pRightBranch != 0 && TRIANG(dDR,pt->m_dMaxRight,dRadius)) {
            pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        /* We have procesed both sides, we need to go to the stack */
        
        if (!sStack.empty( )) {
            pt = sStack.back();
            sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        break;
    }
    if ( !sStack.empty( ) ) // for safety !!!
    {
        std::vector <NearTreeNode* > sTemp;
        sTemp.swap( sStack );
    }
    return ( (long)tClosest.size() );
    
}   // end InSphere


template<typename ContainerType>
long InSphere (
               const DistanceTypeNode& dRadius,
               ContainerType& tClosest,
               std::vector<size_t>& tIndices,
               const TNode& t,
               const std::vector<TNode>& objectStore,
               const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
               , size_t& VisitCount
#endif
               ) const
{
    std::vector <NearTreeNode* > sStack;
    DistanceTypeNode dDL=0., dDR=0.;
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif                            
    if ( pt->m_ptLeft == ULONG_MAX &&  pt->m_ptRight == ULONG_MAX) return false; // test for empty
    while ( pt->m_ptLeft != ULONG_MAX ||
           pt->m_ptRight != ULONG_MAX ||
           !sStack.empty( ) ) 
    {
        if (pt->m_ptLeft == ULONG_MAX && pt->m_ptRight == ULONG_MAX) {
            if (!sStack.empty( )) {
                pt = sStack.back();
                sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            }
            break;
        }
        if (pt->m_ptLeft != ULONG_MAX) {
            dDL = DistanceBetween( t, objectStore[pt->m_ptLeft] );
            if ( dDL <= dRadius )
            {
                size_t collide;
                tClosest.insert( tClosest.end(), objectStore[pt->m_ptLeft] );
                tIndices.insert( tIndices.end(), pt->m_ptLeft);
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), objectStore[objectCollide[collide]] );
                    tIndices.insert( tIndices.end(), objectCollide[collide]);
                    collide = objectCollide[collide];
                }

            }
        }
        if (pt->m_ptRight != ULONG_MAX) {
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                                            
            dDR = DistanceBetween( t, objectStore[pt->m_ptRight]);
            if ( dDR <= dRadius )
            {
                size_t collide;
                tClosest.insert( tClosest.end(), objectStore[pt->m_ptRight] );
                tIndices.insert( tIndices.end(), pt->m_ptRight);
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), objectStore[objectCollide[collide]] );
                    tIndices.insert( tIndices.end(), objectCollide[collide]);
                    collide = objectCollide[collide];
                }

            }
        }
        
        /*
         See if both branches are populated.  In that case, save one branch
         on the stack, and process the other one based on which one seems
         smaller, but useful first]
         */
        if (pt->m_pLeftBranch != 0 && pt->m_pRightBranch != 0 ) {
            if (dDL+pt->m_dMaxLeft < dDR+pt->m_dMaxRight || pt->m_pRightBranch == 0) {
                if ( TRIANG(dDL,pt->m_dMaxLeft,dRadius)) {
                    if ( TRIANG(dDR,pt->m_dMaxRight,dRadius)) {
                        sStack.push_back(pt->m_pRightBranch);
                    }
                    pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                    ++VisitCount;
#endif                                
                    continue;
                }
                /* If we are here, the left branch was not useful
                 Fall through to use the right
                 */
            }
            
            /* We come here either because pursuing the left branch was not useful
             of the right branch look shorter
             */
            if ( TRIANG(dDR,pt->m_dMaxRight,dRadius)) {
                if ( TRIANG(dDL,pt->m_dMaxLeft,dRadius)) {
                    sStack.push_back(pt->m_pLeftBranch);
                }
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            } 
        }
        
        /* Only one branch is viable, try them one at a time
         */
        if ( pt->m_pLeftBranch != 0 && TRIANG(dDL,pt->m_dMaxLeft,dRadius)) {
            pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        if ( pt->m_pRightBranch != 0 && TRIANG(dDR,pt->m_dMaxRight,dRadius)) {
            pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        /* We have procesed both sides, we need to go to the stack */
        
        if (!sStack.empty( )) {
            pt = sStack.back();
            sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        break;
    }
    if ( !sStack.empty( ) ) // for safety !!!
    {
        std::vector <NearTreeNode* > sTemp;
        sTemp.swap( sStack );
    }
    return ( (long)tClosest.size() );
    
}   // end InSphere


//=======================================================================
//  long LeftInSphere (
//                const DistanceTypeNode& dRadius,
//                ContainerType& tClosest,
//                const TNode& t,
//                const std::vector<TNode>& objectStore,
//                const std::vector<size_t>& objectCollide,
//                ) const
//
//  Private function to search a NearTree for the objects inside of the specified radius
//     from the probe point
//  This function is only called by FindInSphere.
//
//    dRadius is the search radius
//    tClosest is a CNearTree of objects of the templated type found within dRadius of the
//         probe point
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
// This version searches to the left first
//
//=======================================================================
template<typename ContainerType>
long LeftInSphere (
               const DistanceTypeNode& dRadius,
               ContainerType& tClosest,
               const TNode& t,
               const std::vector<TNode>& objectStore,
               const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
               , size_t& VisitCount
#endif
               ) const
{
    std::vector <NearTreeNode* > sStack;
    enum  { left, right, end } eDir;
    eDir = left; // examine the left nodes first
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
    if (pt->m_ptLeft == ULONG_MAX) return false; // test for empty
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif                            
    while ( ! ( eDir == end && sStack.empty( ) ) )
    {
        if ( eDir == right )
        {
            const DistanceTypeNode dDR =  DistanceBetween( t, objectStore[pt->m_ptRight] );
            if ( dDR <= dRadius )
            {
                size_t collide;
                tClosest.insert( tClosest.end(), objectStore[pt->m_ptRight] );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), objectStore[objectCollide[collide]] );
                    collide = objectCollide[collide];
                }
           }
            if ( pt->m_pRightBranch != 0 && TRIANG(dDR,pt->m_dMaxRight,dRadius) )
            { // we did the left and now we finished the right, go down
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
                eDir = left;
            }
            else
            {
                eDir = end;
            }
        }
        if ( eDir == left )
        {
            const DistanceTypeNode dDL = DistanceBetween( t, objectStore[pt->m_ptLeft]  );
            if ( dDL <= dRadius )
            {
                size_t collide;
                tClosest.insert( tClosest.end(), objectStore[pt->m_ptLeft] );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), objectStore[objectCollide[collide]] );
                    collide = objectCollide[collide];
                }
            }
            if ( pt->m_ptRight != ULONG_MAX ) // only stack if there's a right object
            {
                sStack.push_back( pt );
            }
            if ( pt->m_pLeftBranch != 0 && TRIANG(dDL,pt->m_dMaxLeft,dRadius) )
            { // we did the left, go down
                pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
            }
            else
            {
                eDir = end;
            }
        }
        
        if ( eDir == end && !sStack.empty( ) )
        {
            pt = sStack.back( );
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif            
            sStack.pop_back( );
            eDir = right;
        }
    }
    
    return ( (long)tClosest.size() );
}  //  end LeftInSphere

template<typename ContainerType>
long LeftInSphere (
               const DistanceTypeNode& dRadius,
               ContainerType& tClosest,
               std::vector<size_t>& tIndices,
               const TNode& t,
               const std::vector<TNode>& objectStore,
               const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
               , size_t& VisitCount
#endif
               ) const
{
    std::vector <NearTreeNode* > sStack;
    enum  { left, right, end } eDir;
    eDir = left; // examine the left nodes first
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif            
    if (pt->m_ptLeft == ULONG_MAX) return false; // test for empty
    while ( ! ( eDir == end && sStack.empty( ) ) )
    {
        if ( eDir == right )
        {
            const DistanceTypeNode dDR =  DistanceBetween( t, objectStore[pt->m_ptRight] );
            if ( dDR <= dRadius )
            {
                size_t collide;
                tClosest.insert( tClosest.end(), objectStore[pt->m_ptRight] );
                tIndices.insert( tIndices.end(), pt->m_ptRight);
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), objectStore[objectCollide[collide]] );
                    tIndices.insert( tIndices.end(), objectCollide[collide]);
                    collide = objectCollide[collide];
                }

            }
            if ( pt->m_pRightBranch != 0 && TRIANG(dDR,pt->m_dMaxRight,dRadius) )
            { // we did the left and now we finished the right, go down
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
                eDir = left;
            }
            else
            {
                eDir = end;
            }
        }
        if ( eDir == left )
        {
            const DistanceTypeNode dDL = DistanceBetween( t, objectStore[pt->m_ptLeft]  );
            if ( dDL <= dRadius )
            {
                size_t collide;
                tClosest.insert( tClosest.end(), objectStore[pt->m_ptLeft] );
                tIndices.insert( tIndices.end(), pt->m_ptLeft);
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), objectStore[objectCollide[collide]] );
                    tIndices.insert( tIndices.end(), objectCollide[collide]);
                    collide = objectCollide[collide];
                }
            }
            if ( pt->m_ptRight != ULONG_MAX ) // only stack if there's a right object
            {
                sStack.push_back( pt );
            }
            if ( pt->m_pLeftBranch != 0 && TRIANG(dDL,pt->m_dMaxLeft,dRadius) )
            { // we did the left, go down
                pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
            }
            else
            {
                eDir = end;
            }
        }
        
        if ( eDir == end && !sStack.empty( ) )
        {
            pt = sStack.back( );
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif            
            sStack.pop_back( );
            eDir = right;
        }
    }
    
    return ( (long)tClosest.size() );
}  //  end LeftInSphere


//=======================================================================
//  long OutSphere (
//                const DistanceTypeNode& dRadius,
//                ContainerType& tFarthest,
//                const TNode& t,
//                const std::vector<TNode>& objectStore,
//                const std::vector<size_t>& objectCollide
//                ) const
//
//  Private function to search a NearTree for the objects outside of the specified radius
//     from the probe point
//  This function is only called by FindOutSphere.
//
//    dRadius is the search radius
//    tFarthest is a CNearTree of objects of the templated type found within dRadius of the
//         probe point
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
//=======================================================================
template<typename ContainerType>
long OutSphere (
                const DistanceTypeNode& dRadius,
                ContainerType& tFarthest,
                const TNode& t,
                const std::vector<TNode>& objectStore,
                const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
                , size_t& VisitCount
#endif
                ) const
{
    std::vector <NearTreeNode* > sStack;
    DistanceTypeNode dDL=0., dDR=0.;
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif                
    if ( pt->m_ptLeft == ULONG_MAX &&  pt->m_ptRight == ULONG_MAX) return false; // test for empty
    while ( pt->m_ptLeft != ULONG_MAX ||
           pt->m_ptRight != ULONG_MAX ||
           !sStack.empty( ) ) 
    {
        if (pt->m_ptLeft == ULONG_MAX && pt->m_ptRight == ULONG_MAX) {
            if (!sStack.empty( )) {
                pt = sStack.back();
                sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            }
            break;
        }
        if (pt->m_ptLeft != ULONG_MAX) {
            dDL = DistanceBetween( t, objectStore[pt->m_ptLeft] );
            if ( dDL >= dRadius )
            {   size_t collide;
                tFarthest.insert( tFarthest.end(), objectStore[pt->m_ptLeft] );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), objectStore[objectCollide[collide]] );
                    collide = objectCollide[collide];
                }

            }
        }
        if (pt->m_ptRight != ULONG_MAX) {
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                                            
            dDR = DistanceBetween( t, objectStore[pt->m_ptRight]);
            if ( dDR >= dRadius )
            {   size_t collide;
                tFarthest.insert( tFarthest.end(), objectStore[pt->m_ptRight] );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), objectStore[objectCollide[collide]] );
                    collide = objectCollide[collide];
                }
            }
        }
        
        /*
         See if both branches are populated.  In that case, save one branch
         on the stack, and process the other one based on which one seems
         larger, but useful first]
         */
        if (pt->m_pLeftBranch != 0 && pt->m_pRightBranch != 0 ) {
            if (dDL+pt->m_dMaxLeft > dDR+pt->m_dMaxRight || pt->m_pRightBranch == 0) {
                if ( TRIANG(dRadius,dDL,pt->m_dMaxLeft)) {
                    if ( TRIANG(dRadius,dDR,pt->m_dMaxRight)) {
                        sStack.push_back(pt->m_pRightBranch);
                    }
                    pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                    ++VisitCount;
#endif                                
                    continue;
                }
                /* If we are here, the left branch was not useful
                 Fall through to use the right
                 */
            }
            
            /* We come here either because pursuing the left branch was not useful
             of the right branch look shorter
             */
            if ( TRIANG(dRadius,dDR,pt->m_dMaxRight)) {
                if ( TRIANG(dRadius,dDL,pt->m_dMaxLeft)) {
                    sStack.push_back(pt->m_pLeftBranch);
                }
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            } 
        }
        
        /* Only one branch is viable, try them one at a time
         */
        if ( pt->m_pLeftBranch != 0 && TRIANG(dRadius,dDL,pt->m_dMaxLeft)) {
            pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        if ( pt->m_pRightBranch != 0 && TRIANG(dRadius,dDR,pt->m_dMaxRight)) {
            pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        /* We have procesed both sides, we need to go to the stack */
        
        if (!sStack.empty( )) {
            pt = sStack.back();
            sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        break;
    }
    if ( !sStack.empty( ) ) // for safety !!!
    {
        std::vector <NearTreeNode* > sTemp;
        sTemp.swap( sStack );
    }
    return ( (long)tFarthest.size() );
}   // end OutSphere


template<typename ContainerType>
long OutSphere (
                const DistanceTypeNode& dRadius,
                ContainerType& tFarthest,
                std::vector<size_t>& tIndices,
                const TNode& t,
                const std::vector<TNode>& objectStore,
                const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
                , size_t& VisitCount
#endif
                ) const
{
    std::vector <NearTreeNode* > sStack;
    DistanceTypeNode dDL=0., dDR=0.;
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif                
    if ( pt->m_ptLeft == ULONG_MAX &&  pt->m_ptRight == ULONG_MAX) return false; // test for empty
    while ( pt->m_ptLeft != ULONG_MAX ||
           pt->m_ptRight != ULONG_MAX ||
           !sStack.empty( ) ) 
    {
        if (pt->m_ptLeft == ULONG_MAX && pt->m_ptRight == ULONG_MAX) {
            if (!sStack.empty( )) {
                pt = sStack.back();
                sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            }
            break;
        }
        if (pt->m_ptLeft != ULONG_MAX) {
            dDL = DistanceBetween( t, objectStore[pt->m_ptLeft] );
            if ( dDL >= dRadius )
            {   size_t collide;
                tFarthest.insert( tFarthest.end(), objectStore[pt->m_ptLeft] );
                tIndices.insert( tIndices.end(), pt->m_ptLeft);
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), objectStore[objectCollide[collide]] );
                    tIndices.insert( tIndices.end(), objectCollide[collide] );
                    collide = objectCollide[collide];
                }

            }
        }
        if (pt->m_ptRight != ULONG_MAX) {
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                                            
            dDR = DistanceBetween( t, objectStore[pt->m_ptRight]);
            if ( dDR >= dRadius )
            {
                size_t collide;
                tFarthest.insert( tFarthest.end(), objectStore[pt->m_ptRight] );
                tIndices.insert( tIndices.end(), pt->m_ptRight);
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), objectStore[objectCollide[collide]] );
                    tIndices.insert( tIndices.end(), objectCollide[collide] );
                    collide = objectCollide[collide];
                }

            }            
        }
        
        /*
         See if both branches are populated.  In that case, save one branch
         on the stack, and process the other one based on which one seems
         larger, but useful first]
         */
        if (pt->m_pLeftBranch != 0 && pt->m_pRightBranch != 0 ) {
            if (dDL+pt->m_dMaxLeft > dDR+pt->m_dMaxRight || pt->m_pRightBranch == 0) {
                if ( TRIANG(dRadius,dDL,pt->m_dMaxLeft)) {
                    if ( TRIANG(dRadius,dDR,pt->m_dMaxRight)) {
                        sStack.push_back(pt->m_pRightBranch);
                    }
                    pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                    ++VisitCount;
#endif                                
                    continue;
                }
                /* If we are here, the left branch was not useful
                 Fall through to use the right
                 */
            }
            
            /* We come here either because pursuing the left branch was not useful
             of the right branch look shorter
             */
            if ( TRIANG(dRadius,dDR,pt->m_dMaxRight)) {
                if ( TRIANG(dRadius,dDL,pt->m_dMaxLeft)) {
                    sStack.push_back(pt->m_pLeftBranch);
                }
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            } 
        }
        
        /* Only one branch is viable, try them one at a time
         */
        if ( pt->m_pLeftBranch != 0 && TRIANG(dRadius,dDL,pt->m_dMaxLeft)) {
            pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        if ( pt->m_pRightBranch != 0 && TRIANG(dRadius,dDR,pt->m_dMaxRight)) {
            pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        /* We have procesed both sides, we need to go to the stack */
        
        if (!sStack.empty( )) {
            pt = sStack.back();
            sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        break;
    }
    if ( !sStack.empty( ) ) // for safety !!!
    {
        std::vector <NearTreeNode* > sTemp;
        sTemp.swap( sStack );
    }
    return ( (long)tFarthest.size() );
}   // end OutSphere


//=======================================================================
//  long LeftOutSphere (
//                const DistanceTypeNode& dRadius,
//                ContainerType& tFarthest,
//                const TNode& t,
//                const std::vector<TNode>& objectStore,
//                const std::vector<size_t>& objectCollide
//                ) const
//
//  Private function to search a NearTree for the objects outside of the specified radius
//     from the probe point
//  This function is only called by FindOutSphere.
//
//    dRadius is the search radius
//    tFarthest is a CNearTree of objects of the templated type found within dRadius of the
//         probe point
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
//=======================================================================
template<typename ContainerType>
long LeftOutSphere (
                const DistanceTypeNode& dRadius,
                ContainerType& tFarthest,
                const TNode& t,
                    const std::vector<TNode>& objectStore,
                    const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
                , size_t& VisitCount
#endif
                ) const
{
    std::vector <NearTreeNode* > sStack;
    enum  { left, right, end } eDir;
    eDir = left; // examine the left nodes first
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif            
    if (pt->m_ptLeft == ULONG_MAX) return false; // test for empty
    while ( ! ( eDir == end && sStack.empty( ) ) )
    {
        if ( eDir == right )
        {
            const DistanceTypeNode dDR = DistanceBetween( t, objectStore[pt->m_ptRight] );
            if ( dDR >= dRadius )
            {   size_t collide;
                tFarthest.insert( tFarthest.end(), objectStore[pt->m_ptRight] );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), objectStore[objectCollide[collide]] );
                    collide = objectCollide[collide];
                }
            }
            if ( pt->m_pRightBranch != 0 && TRIANG(dRadius,dDR,pt->m_dMaxRight) )
            { // we did the left and now we finished the right, go down
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
                eDir = left;
            }
            else
            {
                eDir = end;
            }
        }
        if ( eDir == left )
        {
            const DistanceTypeNode dDL = DistanceBetween( t, objectStore[pt->m_ptLeft]  );
            if ( dDL >= dRadius )
            {
                size_t collide;
                tFarthest.insert( tFarthest.end(), objectStore[pt->m_ptLeft] );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), objectStore[objectCollide[collide]] );
                    collide = objectCollide[collide];
                }

            }
            if ( pt->m_ptRight != ULONG_MAX ) // only stack if there's a right object
            {
                sStack.push_back( pt );
            }
            if ( pt->m_pLeftBranch != 0 && TRIANG(dRadius,dDL,pt->m_dMaxLeft) )
            { // we did the left, go down
                pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
            }
            else
            {
                eDir = end;
            }
        }
        
        if ( eDir == end && !sStack.empty( ) )
        {
            pt = sStack.back( );
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif            
            sStack.pop_back( );
            eDir = right;
        }
    }
    
    return ( (long)tFarthest.size() );
}  //  end LeftOutSphere

template<typename ContainerType>
long LeftOutSphere (
                const DistanceTypeNode& dRadius,
                ContainerType& tFarthest,
                std::vector<size_t>& tIndices,
                const TNode& t,
                const std::vector<TNode>& objectStore,
                const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
                , size_t& VisitCount
#endif
                ) const
{
    std::vector <NearTreeNode* > sStack;
    enum  { left, right, end } eDir;
    eDir = left; // examine the left nodes first
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif            
    if (pt->m_ptLeft == ULONG_MAX) return false; // test for empty
    while ( ! ( eDir == end && sStack.empty( ) ) )
    {
        if ( eDir == right )
        {
            const DistanceTypeNode dDR = DistanceBetween( t, objectStore[pt->m_ptRight] );
            if ( dDR >= dRadius )
            {   size_t collide;
                tFarthest.insert( tFarthest.end(), objectStore[pt->m_ptRight] );
                tIndices.insert( tIndices.end(), pt->m_ptRight );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), objectStore[objectCollide[collide]] );
                    tIndices.insert( tIndices.end(), objectCollide[collide] );
                    collide = objectCollide[collide];
                }

            }
            if ( pt->m_pRightBranch != 0 && TRIANG(dRadius,dDR,pt->m_dMaxRight) )
            { // we did the left and now we finished the right, go down
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
                eDir = left;
            }
            else
            {
                eDir = end;
            }
        }
        if ( eDir == left )
        {
            const DistanceTypeNode dDL = DistanceBetween( t, objectStore[pt->m_ptLeft]  );
            if ( dDL >= dRadius )
            {   size_t collide;
                tFarthest.insert( tFarthest.end(), objectStore[pt->m_ptLeft] );
                tIndices.insert( tIndices.end(), pt->m_ptLeft );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), objectStore[objectCollide[collide]] );
                    tIndices.insert( tIndices.end(), objectCollide[collide] );
                    collide = objectCollide[collide];
                }
            }
            if ( pt->m_ptRight != ULONG_MAX ) // only stack if there's a right object
            {
                sStack.push_back( pt );
            }
            if ( pt->m_pLeftBranch != 0 && TRIANG(dRadius,dDL,pt->m_dMaxLeft) )
            { // we did the left, go down
                pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
            }
            else
            {
                eDir = end;
            }
        }
        
        if ( eDir == end && !sStack.empty( ) )
        {
            pt = sStack.back( );
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif            
            sStack.pop_back( );
            eDir = right;
        }
    }
    
    return ( (long)tFarthest.size() );
}  //  end LeftOutSphere

//=======================================================================
//  long InAnnulus ( const DistanceTypeNode& dRadius1,
//  const DistanceTypeNode& dRadius2, CNearTree< TNode >& tAnnular,
//  const std::vector<TNode>& objectStore,
//  const std::vector<size_t>& objectCollide,
//  const TNode& t ) const
//
//  Private function to search a NearTree for the objects within a specified annulus from probe point
//  This function is only called by FindInAnnulus.
//
//    dRadius1, dRadius2 specifies the range of the annulus
//    tAnnular is a NearTree of objects of the templated type found between the two radii
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
//=======================================================================
template<typename ContainerType>
long InAnnulus (
                const DistanceTypeNode& dRadius1,
                const DistanceTypeNode& dRadius2,
                ContainerType& tAnnular,
                const TNode& t,
                const std::vector<TNode>& objectStore,
                const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
                , size_t& VisitCount
#endif
                ) const
{
    std::vector <NearTreeNode* > sStack;
    DistanceTypeNode dDL=0., dDR=0.;
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif                
    if ( pt->m_ptLeft == ULONG_MAX &&  pt->m_ptRight == ULONG_MAX) return false; // test for empty
    while ( pt->m_ptLeft != ULONG_MAX ||
           pt->m_ptRight != ULONG_MAX ||
           !sStack.empty( ) ) 
    {
        if (pt->m_ptLeft == ULONG_MAX && pt->m_ptRight == ULONG_MAX) {
            if (!sStack.empty( )) {
                pt = sStack.back();
                sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            }
            break;
        }
        if (pt->m_ptLeft != ULONG_MAX) {
            dDL = DistanceBetween( t, objectStore[pt->m_ptLeft] );
            if ( dDL <= dRadius2 && dDL >= dRadius1 )
            {   size_t collide;
                tAnnular.insert( tAnnular.end(), objectStore[pt->m_ptLeft] );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tAnnular.insert( tAnnular.end(), objectStore[objectCollide[collide]] );
                    collide = objectCollide[collide];
                }
            }
        }
        if (pt->m_ptRight != ULONG_MAX) {
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                                            
            dDR = DistanceBetween( t, objectStore[pt->m_ptRight]);
            if ( dDR <= dRadius2 && dDR >= dRadius1 )
            {   size_t collide;
                tAnnular.insert( tAnnular.end(), objectStore[pt->m_ptRight] );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tAnnular.insert( tAnnular.end(), objectStore[objectCollide[collide]] );
                    collide = objectCollide[collide];
                }
            }
        }
        
        /*
         See if both branches are populated.  In that case, save one branch
         on the stack, and process the other one based on which one seems
         closer, but useful first
         */
        if (pt->m_pLeftBranch != 0 && pt->m_pRightBranch != 0 ) {
            if (dDL+pt->m_dMaxLeft < dDR+pt->m_dMaxRight || pt->m_pRightBranch == 0) {
                if ( (TRIANG(dRadius1,dDL,pt->m_dMaxLeft)) && (TRIANG(dDL,pt->m_dMaxLeft,dRadius2)  )) {
                    if ( (TRIANG(dRadius1,dDR,pt->m_dMaxRight)) && (TRIANG(dDR,pt->m_dMaxRight,dRadius2) )) {
                        sStack.push_back(pt->m_pRightBranch);
                    }
                    pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                    ++VisitCount;
#endif                                
                    continue;
                }
                /* If we are here, the left branch was not useful
                 Fall through to use the right
                 */
            }
            
            /* We come here either because pursuing the left branch was not useful
             of the right branch look shorter
             */
            if ( (TRIANG(dRadius1,dDR,pt->m_dMaxRight)) && (TRIANG(dDR,pt->m_dMaxRight,dRadius2) ) ) {
                if ( (TRIANG(dRadius1,dDL,pt->m_dMaxLeft)) && (TRIANG(dDL,pt->m_dMaxLeft,dRadius2)  )) {
                    sStack.push_back(pt->m_pLeftBranch);
                }
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            } 
        }
        
        /* Only one branch is viable, try them one at a time
         */
        if ( pt->m_pLeftBranch != 0 && (TRIANG(dRadius1,dDL,pt->m_dMaxLeft)) && (TRIANG(dDL,pt->m_dMaxLeft,dRadius2)  )) {
            pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        if ( pt->m_pRightBranch != 0 && (TRIANG(dRadius1,dDR,pt->m_dMaxRight)) && (TRIANG(dDR,pt->m_dMaxRight,dRadius2) )) {
            pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        /* We have procesed both sides, we need to go to the stack */
        
        if (!sStack.empty( )) {
            pt = sStack.back();
            sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        break;
    }
    if ( !sStack.empty( ) ) // for safety !!!
    {
        std::vector <NearTreeNode* > sTemp;
        sTemp.swap( sStack );
    }
    return ( (long)tAnnular.size() );
}   // end InAnnulus


template<typename ContainerType>
long InAnnulus (
                const DistanceTypeNode& dRadius1,
                const DistanceTypeNode& dRadius2,
                ContainerType& tAnnular,
                std::vector<size_t>& tIndices,
                const TNode& t,
                const std::vector<TNode>& objectStore,
                const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
                , size_t& VisitCount
#endif
                ) const
{
    std::vector <NearTreeNode* > sStack;
    DistanceTypeNode dDL=0., dDR=0.;
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif                
    if ( pt->m_ptLeft == ULONG_MAX &&  pt->m_ptRight == ULONG_MAX) return false; // test for empty
    while ( pt->m_ptLeft != ULONG_MAX ||
           pt->m_ptRight != ULONG_MAX ||
           !sStack.empty( ) ) 
    {
        if (pt->m_ptLeft == ULONG_MAX && pt->m_ptRight == ULONG_MAX) {
            if (!sStack.empty( )) {
                pt = sStack.back();
                sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            }
            break;
        }
        if (pt->m_ptLeft != ULONG_MAX) {
            dDL = DistanceBetween( t, objectStore[pt->m_ptLeft] );
            if ( dDL <= dRadius2 && dDL >= dRadius1 )
            {   size_t collide;
                tAnnular.insert( tAnnular.end(), objectStore[pt->m_ptLeft] );
                tIndices.insert( tIndices.end(), pt->m_ptLeft);
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tAnnular.insert( tAnnular.end(), objectStore[objectCollide[collide]] );
                    tIndices.insert( tIndices.end(), objectCollide[collide]);
                    collide = objectCollide[collide];
                }
            }            
        }
        if (pt->m_ptRight != ULONG_MAX) {
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                                            
            dDR = DistanceBetween( t, objectStore[pt->m_ptRight]);
            if ( dDR <= dRadius2 && dDR >= dRadius1 )
            {   size_t collide;
                tAnnular.insert( tAnnular.end(), objectStore[pt->m_ptRight] );
                tIndices.insert( tIndices.end(), pt->m_ptRight);
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tAnnular.insert( tAnnular.end(), objectStore[objectCollide[collide]] );
                    tIndices.insert( tIndices.end(), objectCollide[collide]);
                    collide = objectCollide[collide];
                }
            }            
        }
        
        /*
         See if both branches are populated.  In that case, save one branch
         on the stack, and process the other one based on which one seems
         larger, but useful first]
         */
        if (pt->m_pLeftBranch != 0 && pt->m_pRightBranch != 0 ) {
            if (dDL+pt->m_dMaxLeft < dDR+pt->m_dMaxRight || pt->m_pRightBranch == 0) {
                if ( (TRIANG(dRadius1,dDL,pt->m_dMaxLeft)) && (TRIANG(dDL,pt->m_dMaxLeft,dRadius2)  )) {
                    if ( (TRIANG(dRadius1,dDR,pt->m_dMaxRight)) && (TRIANG(dDR,pt->m_dMaxRight,dRadius2) )) {
                        sStack.push_back(pt->m_pRightBranch);
                    }
                    pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                    ++VisitCount;
#endif                                
                    continue;
                }
                /* If we are here, the left branch was not useful
                 Fall through to use the right
                 */
            }
            
            /* We come here either because pursuing the left branch was not useful
             of the right branch look shorter
             */
            if ( (TRIANG(dRadius1,dDR,pt->m_dMaxRight)) && (TRIANG(dDR,pt->m_dMaxRight,dRadius2) ) ) {
                if ( (TRIANG(dRadius1,dDL,pt->m_dMaxLeft)) && (TRIANG(dDL,pt->m_dMaxLeft,dRadius2)  )) {
                    sStack.push_back(pt->m_pLeftBranch);
                }
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            } 
        }
        
        /* Only one branch is viable, try them one at a time
         */
        if ( pt->m_pLeftBranch != 0 && (TRIANG(dRadius1,dDL,pt->m_dMaxLeft)) && (TRIANG(dDL,pt->m_dMaxLeft,dRadius2)  )) {
            pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        if ( pt->m_pRightBranch != 0 && (TRIANG(dRadius1,dDR,pt->m_dMaxRight)) && (TRIANG(dDR,pt->m_dMaxRight,dRadius2) )) {
            pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        /* We have procesed both sides, we need to go to the stack */
        
        if (!sStack.empty( )) {
            pt = sStack.back();
            sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        break;
    }
    if ( !sStack.empty( ) ) // for safety !!!
    {
        std::vector <NearTreeNode* > sTemp;
        sTemp.swap( sStack );
    }
    return ( (long)tAnnular.size() );
}   // end InAnnulus


//=======================================================================
//  long LeftInAnnulus ( const DistanceTypeNode& dRadius1,
//  const DistanceTypeNode& dRadius2, CNearTree< TNode >& tAnnular,
//  const std::vector<TNode>& objectStore,
//  const std::vector<size_t>& objectCollide,
//  const TNode& t ) const
//
//  Private function to search a NearTree for the objects within a specified annulus from probe point
//  This function is only called by FindInAnnulus.
//
//    dRadius1, dRadius2 specifies the range of the annulus
//    tAnnular is a NearTree of objects of the templated type found between the two radii
//    t  is the probe point
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
//=======================================================================
template<typename ContainerType>
long LeftInAnnulus (
                const DistanceTypeNode& dRadius1,
                const DistanceTypeNode& dRadius2,
                ContainerType& tAnnular,
                const TNode& t,
                const std::vector<TNode>& objectStore,
                const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
                , size_t& VisitCount
#endif
                ) const
{
    std::vector <NearTreeNode* > sStack;
    enum  { left, right, end } eDir;
    eDir = left; // examine the left nodes first
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif            
    if (pt->m_ptLeft == ULONG_MAX) return false; // test for empty
    while ( ! ( eDir == end && sStack.empty( ) ) )
    {
        if ( eDir == right )
        {
            const DistanceTypeNode dDR = DistanceBetween( t, objectStore[pt->m_ptRight] );
            if ( dDR <= dRadius2 && dDR >= dRadius1 )
            {   size_t collide;
                tAnnular.insert( tAnnular.end( ), objectStore[pt->m_ptRight] );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tAnnular.insert( tAnnular.end(), objectStore[objectCollide[collide]] );
                    collide = objectCollide[collide];
                }
            }
            if ( pt->m_pRightBranch != 0 && (TRIANG(dRadius1,dDR,pt->m_dMaxRight)) && (TRIANG(dDR,pt->m_dMaxRight,dRadius2) ) )
            { // we did the left and now we finished the right, go down
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
                eDir = left;
            }
            else
            {
                eDir = end;
            }
        }
        if ( eDir == left )
        {
            const DistanceTypeNode dDL = DistanceBetween( t, objectStore[pt->m_ptLeft]  );
            if ( dDL <= dRadius2 && dDL >= dRadius1 )
            {
                size_t collide;
                tAnnular.insert( tAnnular.end(), objectStore[pt->m_ptLeft] );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tAnnular.insert( tAnnular.end(), objectStore[objectCollide[collide]] );
                    collide = objectCollide[collide];
                }
            }
            if ( pt->m_ptRight != ULONG_MAX ) // only stack if there's a right object
            {
                sStack.push_back( pt );
            }
            if ( pt->m_pLeftBranch != 0 && (TRIANG(dRadius1,dDL,pt->m_dMaxLeft)) && (TRIANG(dDL,pt->m_dMaxLeft,dRadius2)  ) )
            { // we did the left, go down
                pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
            }
            else
            {
                eDir = end;
            }
        }
        
        if ( eDir == end && !sStack.empty( ) )
        {
            pt = sStack.back( );
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif            
            sStack.pop_back( );
            eDir = right;
        }
    }
    
    return ( (long)tAnnular.size() );
}  // end LeftInAnnulus

template<typename ContainerType>
long LeftInAnnulus (
                const DistanceTypeNode& dRadius1,
                const DistanceTypeNode& dRadius2,
                ContainerType& tAnnular,
                std::vector<size_t>& tIndices,
                const TNode& t,
                const std::vector<TNode>& objectStore,
                const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
                , size_t& VisitCount
#endif
                ) const
{
    std::vector <NearTreeNode* > sStack;
    enum  { left, right, end } eDir;
    eDir = left; // examine the left nodes first
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif            
    if (pt->m_ptLeft == ULONG_MAX) return false; // test for empty
    while ( ! ( eDir == end && sStack.empty( ) ) )
    {
        if ( eDir == right )
        {
            const DistanceTypeNode dDR = DistanceBetween( t, objectStore[pt->m_ptRight] );
            if ( dDR <= dRadius2 && dDR >= dRadius1 )
            {   size_t collide;
                tAnnular.insert( tAnnular.end( ), objectStore[pt->m_ptRight] );
                tIndices.insert( tIndices.end( ), pt->m_ptRight );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tAnnular.insert( tAnnular.end(), objectStore[objectCollide[collide]] );
                    tIndices.insert( tIndices.end( ), objectCollide[collide] );
                    collide = objectCollide[collide];
                }
            }
            if ( pt->m_pRightBranch != 0 && (TRIANG(dRadius1,dDR,pt->m_dMaxRight)) && (TRIANG(dDR,pt->m_dMaxRight,dRadius2) ) )
            { // we did the left and now we finished the right, go down
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
                eDir = left;
            }
            else
            {
                eDir = end;
            }
        }
        if ( eDir == left )
        {
            const DistanceTypeNode dDL = DistanceBetween( t, objectStore[pt->m_ptLeft]  );
            if ( dDL <= dRadius2 && dDL >= dRadius1 )
            {   size_t collide;
                tAnnular.insert( tAnnular.end(), objectStore[pt->m_ptLeft] );
                tIndices.insert( tIndices.end( ), pt->m_ptLeft );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tAnnular.insert( tAnnular.end(), objectStore[objectCollide[collide]] );
                    tIndices.insert( tIndices.end( ), objectCollide[collide] );
                    collide = objectCollide[collide];
                }
            }
            if ( pt->m_ptRight != ULONG_MAX ) // only stack if there's a right object
            {
                sStack.push_back( pt );
            }
            if ( pt->m_pLeftBranch != 0 && (TRIANG(dRadius1,dDL,pt->m_dMaxLeft)) && (TRIANG(dDL,pt->m_dMaxLeft,dRadius2)  ) )
            { // we did the left, go down
                pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
            }
            else
            {
                eDir = end;
            }
        }
        
        if ( eDir == end && !sStack.empty( ) )
        {
            pt = sStack.back( );
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif            
            sStack.pop_back( );
            eDir = right;
        }
    }
    
    return ( (long)tAnnular.size() );
}  // end LeftInAnnulus



//=======================================================================
//  long K_Near ( const DistanceTypeNode dRadius,  
//                std::vector<std::pair<DistanceTypeNode,T> >& tClosest,
//                const TNode& t, const std::vector<TNode>& objectStore,
//                const std::vector<size_t>& objectCollide) const
//  long K_Near ( const DistanceTypeNode dRadius,  
//                std::vector<triple<DistanceTypeNode,T,size_t> >& tClosest,
//                const TNode& t, const std::vector<TNode>& objectStore,
//                const std::vector<size_t>& objectCollide ) const
//
//  Private function to search a NearTree for the objects inside of the specified radius
//     from the probe point
//  This function is only called by FindK_Nearest.
//
// k:           the maximum number of object to return, giving preference to the nearest
// dRadius:     the search radius, which will be updated when the internal store is resized
// tClosest:    is a vector of pairs of Nodes and objects or of triples 
//                 of Nodes, objects and ordinals of objects where the objects
//                 are of the templated type found within dRadius of the
//                 probe point, limited by the k-near search
// t:           is the probe point
// objectStore: the internal vector storing the object in CNearTree
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
/*=======================================================================*/
long K_Near (
             const size_t k,
             DistanceTypeNode& dRadius,
             std::vector<std::pair<DistanceTypeNode,T> >& tClosest,
             const TNode& t,
             const std::vector<TNode>& objectStore,
             const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
             , size_t& VisitCount
#endif
             )
{
    std::vector <NearTreeNode* > sStack;
    DistanceTypeNode dDL=0., dDR=0.;
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif                            
    if ( pt->m_ptLeft == ULONG_MAX &&  pt->m_ptRight == ULONG_MAX) return false; // test for empty
    while ( pt->m_ptLeft != ULONG_MAX ||
           pt->m_ptRight != ULONG_MAX ||
           !sStack.empty( ) ) 
    {
        if (pt->m_ptLeft == ULONG_MAX && pt->m_ptRight == ULONG_MAX) {
            if (!sStack.empty( )) {
                pt = sStack.back();
                sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            }
            break;
        }
        if (pt->m_ptLeft != ULONG_MAX) {
            dDL = DistanceBetween( t, objectStore[pt->m_ptLeft] );
            if ( dDL <= dRadius )
            {   size_t collide;
                tClosest.insert( tClosest.end(), std::make_pair( dDL, objectStore[pt->m_ptLeft] ) );
                if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), std::make_pair( dDL, objectStore[objectCollide[collide]] ));
                    if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                    collide = objectCollide[collide];
                }

            }
        }
        if (pt->m_ptRight != ULONG_MAX) {
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                                            
            dDR = DistanceBetween( t, objectStore[pt->m_ptRight]);
            if ( dDR <= dRadius )
            {   size_t collide;
                tClosest.insert( tClosest.end(), std::make_pair( dDR, objectStore[pt->m_ptRight] ) );
                if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), std::make_pair( dDR, objectStore[objectCollide[collide]] ));
                    if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                    collide = objectCollide[collide];
                }
            }
        }
        
        /*
         See if both branches are populated.  In that case, save one branch
         on the stack, and process the other one based on which one seems
         smaller, but useful first]
         */
        if (pt->m_pLeftBranch != 0 && pt->m_pRightBranch != 0 ) {
            if (dDL+pt->m_dMaxLeft < dDR+pt->m_dMaxRight || pt->m_pRightBranch == 0) {
                if ( TRIANG(dDL,pt->m_dMaxLeft,dRadius)) {
                    if ( TRIANG(dDR,pt->m_dMaxRight,dRadius)) {
                        sStack.push_back(pt->m_pRightBranch);
                    }
                    pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                    ++VisitCount;
#endif                                
                    continue;
                }
                /* If we are here, the left branch was not useful
                 Fall through to use the right
                 */
            }
            
            /* We come here either because pursuing the left branch was not useful
             of the right branch look shorter
             */
            if ( TRIANG(dDR,pt->m_dMaxRight,dRadius)) {
                if ( TRIANG(dDL,pt->m_dMaxLeft,dRadius)) {
                    sStack.push_back(pt->m_pLeftBranch);
                }
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            } 
        }
        
        /* Only one branch is viable, try them one at a time
         */
        if ( pt->m_pLeftBranch != 0 && TRIANG(dDL,pt->m_dMaxLeft,dRadius)) {
            pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        if ( pt->m_pRightBranch != 0 && TRIANG(dDR,pt->m_dMaxRight,dRadius)) {
            pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        /* We have procesed both sides, we need to go to the stack */
        
        if (!sStack.empty( )) {
            pt = sStack.back();
            sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        break;
    }
    if ( !sStack.empty( ) ) // for safety !!!
    {
        std::vector <NearTreeNode* > sTemp;
        sTemp.swap( sStack );
    }
    if( tClosest.size( ) > 1 ) K_Resize( k, t, tClosest, dRadius );
    return ( (long)tClosest.size( ) );
    
}   // end KNear

long K_Near (
             const size_t k,
             DistanceTypeNode& dRadius,
             std::vector<triple<DistanceTypeNode,T,size_t> >& tClosest,
             const TNode& t,
             const std::vector<TNode>& objectStore,
             const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
             , size_t& VisitCount
#endif
             )
{
    std::vector <NearTreeNode* > sStack;
    DistanceTypeNode dDL=0., dDR=0.;
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif                            
    if ( pt->m_ptLeft == ULONG_MAX &&  pt->m_ptRight == ULONG_MAX) return false; // test for empty
    while ( pt->m_ptLeft != ULONG_MAX ||
           pt->m_ptRight != ULONG_MAX ||
           !sStack.empty( ) ) 
    {
        if (pt->m_ptLeft == ULONG_MAX && pt->m_ptRight == ULONG_MAX) {
            if (!sStack.empty( )) {
                pt = sStack.back();
                sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            }
            break;
        }
        if (pt->m_ptLeft != ULONG_MAX) {
            dDL = DistanceBetween( t, objectStore[pt->m_ptLeft] );
            if ( dDL <= dRadius )
            {   size_t collide;
                tClosest.insert( tClosest.end(), make_triple( dDL, objectStore[pt->m_ptLeft], pt->m_ptLeft ) );
                if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), make_triple( dDL, objectStore[objectCollide[collide]], objectCollide[collide] ) );
                    if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                    collide = objectCollide[collide];
                }

            }
        }
        if (pt->m_ptRight != ULONG_MAX) {
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                                            
            dDR = DistanceBetween( t, objectStore[pt->m_ptRight]);
            if ( dDR <= dRadius )
            {   size_t collide;
                tClosest.insert( tClosest.end(), make_triple( dDR, objectStore[pt->m_ptRight], pt->m_ptRight ) );
                if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), make_triple( dDR, objectStore[objectCollide[collide]],  objectCollide[collide]));
                    if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                    collide = objectCollide[collide];
                }
            }
        }
        
        /*
         See if both branches are populated.  In that case, save one branch
         on the stack, and process the other one based on which one seems
         smaller, but useful first]
         */
        if (pt->m_pLeftBranch != 0 && pt->m_pRightBranch != 0 ) {
            if (dDL+pt->m_dMaxLeft < dDR+pt->m_dMaxRight || pt->m_pRightBranch == 0) {
                if ( TRIANG(dDL,pt->m_dMaxLeft,dRadius)) {
                    if ( TRIANG(dDR,pt->m_dMaxRight,dRadius)) {
                        sStack.push_back(pt->m_pRightBranch);
                    }
                    pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                    ++VisitCount;
#endif                                
                    continue;
                }
                /* If we are here, the left branch was not useful
                 Fall through to use the right
                 */
            }
            
            /* We come here either because pursuing the left branch was not useful
             of the right branch look shorter
             */
            if ( TRIANG(dDR,pt->m_dMaxRight,dRadius)) {
                if ( TRIANG(dDL,pt->m_dMaxLeft,dRadius)) {
                    sStack.push_back(pt->m_pLeftBranch);
                }
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            } 
        }
        
        /* Only one branch is viable, try them one at a time
         */
        if ( pt->m_pLeftBranch != 0 && TRIANG(dDL,pt->m_dMaxLeft,dRadius)) {
            pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        if ( pt->m_pRightBranch != 0 && TRIANG(dDR,pt->m_dMaxRight,dRadius)) {
            pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        /* We have procesed both sides, we need to go to the stack */
        
        if (!sStack.empty( )) {
            pt = sStack.back();
            sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        break;
    }
    if ( !sStack.empty( ) ) // for safety !!!
    {
        std::vector <NearTreeNode* > sTemp;
        sTemp.swap( sStack );
    }
    if( tClosest.size( ) > 1 ) K_Resize( k, t, tClosest, dRadius );
    return ( (long)tClosest.size( ) );
    
}   // end KNear


//=======================================================================
//  long K_Far ( const DistanceTypeNode dRadius, std::vector<std::pair<DistanceTypeNode,T> >& tFarthest, const TNode& t ) const
//  long K_Far ( const DistanceTypeNode dRadius, std::vector<triple<DistanceTypeNode,T,size_t> >& tFarthest, tFarthest, const TNode& t ) const
//
//  Private function to search a NearTree for the objects inside of the specified radius
//     from the probe point. Distances are stored in an intermediate array as negative values
//     so that the same logic as K_Near can be used.
//  This function is only called by FindK_Farthest.
//
// k:           the maximum number of object to return, giving preference to the nearest
// dRadius:     the search radius, which will be updated when the internal store is resized
// tFarthest:    is a vector of pairs of Nodes and objects or of triples 
//                 of Nodes, objects and ordinals of objects where the objects
//                 are of the templated type found outside of dRadius of the
//                 probe point, limited by the k-farthest search
// t:           is the probe point
// objectStore: the internal vector storing the object in CNearTree
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
/*=======================================================================*/
long K_Far (
            const size_t k,
            DistanceTypeNode& dRadius,
            std::vector<std::pair<DistanceTypeNode,T> >& tFarthest,
            const TNode& t,
            const std::vector<TNode>& objectStore,
            const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
            , size_t& VisitCount
#endif
            )
{
    std::vector <NearTreeNode* > sStack;
    DistanceTypeNode dDL=0., dDR=0.;
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif                
    if ( pt->m_ptLeft == ULONG_MAX &&  pt->m_ptRight == ULONG_MAX) return false; // test for empty
    while ( pt->m_ptLeft != ULONG_MAX ||
           pt->m_ptRight != ULONG_MAX ||
           !sStack.empty( ) ) 
    {
        if (pt->m_ptLeft == ULONG_MAX && pt->m_ptRight == ULONG_MAX) {
            if (!sStack.empty( )) {
                pt = sStack.back();
                sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            }
            break;
        }
        if (pt->m_ptLeft != ULONG_MAX) {
            dDL = DistanceBetween( t, objectStore[pt->m_ptLeft] );
            if ( dDL >= dRadius )
            {   size_t collide;
                tFarthest.insert( tFarthest.end(), std::make_pair( -dDL, objectStore[pt->m_ptLeft] ) );
                if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), std::make_pair( -dDL, objectStore[objectCollide[collide]] ));
                    if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                    collide = objectCollide[collide];
                }

            }
        }
        if (pt->m_ptRight != ULONG_MAX) {
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                                            
            dDR = DistanceBetween( t, objectStore[pt->m_ptRight]);
            if ( dDR >= dRadius )
            {   size_t collide;
                tFarthest.insert( tFarthest.end(), std::make_pair( -dDR, objectStore[pt->m_ptRight] ) );
                if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), std::make_pair( -dDR, objectStore[objectCollide[collide]] ));
                    if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                    collide = objectCollide[collide];
                }

            }            
        }
        
        /*
         See if both branches are populated.  In that case, save one branch
         on the stack, and process the other one based on which one seems
         larger, but useful first]
         */
        if (pt->m_pLeftBranch != 0 && pt->m_pRightBranch != 0 ) {
            if (dDL+pt->m_dMaxLeft > dDR+pt->m_dMaxRight || pt->m_pRightBranch == 0) {
                if ( TRIANG(dRadius,dDL,pt->m_dMaxLeft)) {
                    if ( TRIANG(dRadius,dDR,pt->m_dMaxRight)) {
                        sStack.push_back(pt->m_pRightBranch);
                    }
                    pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                    ++VisitCount;
#endif                                
                    continue;
                }
                /* If we are here, the left branch was not useful
                 Fall through to use the right
                 */
            }
            
            /* We come here either because pursuing the left branch was not useful
             of the right branch look shorter
             */
            if ( TRIANG(dRadius,dDR,pt->m_dMaxRight)) {
                if ( TRIANG(dRadius,dDL,pt->m_dMaxLeft)) {
                    sStack.push_back(pt->m_pLeftBranch);
                }
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            } 
        }
        
        /* Only one branch is viable, try them one at a time
         */
        if ( pt->m_pLeftBranch != 0 && TRIANG(dRadius,dDL,pt->m_dMaxLeft)) {
            pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        if ( pt->m_pRightBranch != 0 && TRIANG(dRadius,dDR,pt->m_dMaxRight)) {
            pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        /* We have procesed both sides, we need to go to the stack */
        
        if (!sStack.empty( )) {
            pt = sStack.back();
            sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        break;
    }
    if ( !sStack.empty( ) ) // for safety !!!
    {
        std::vector <NearTreeNode* > sTemp;
        sTemp.swap( sStack );
    }
    if( tFarthest.size( ) > k ) K_Resize( k, t, tFarthest, dRadius );
    return ( (long)tFarthest.size( ) );
}   // end KFar

long K_Far (
            const size_t k,
            DistanceTypeNode& dRadius,
            std::vector<triple<DistanceTypeNode,T,size_t> >& tFarthest,
            const TNode& t,
            const std::vector<TNode>& objectStore,
            const std::vector<TNode>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
            , size_t& VisitCount
#endif
            )
{
    std::vector <NearTreeNode* > sStack;
    DistanceTypeNode dDL=0., dDR=0.;
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif                
    if ( pt->m_ptLeft == ULONG_MAX &&  pt->m_ptRight == ULONG_MAX) return false; // test for empty
    while ( pt->m_ptLeft != ULONG_MAX ||
           pt->m_ptRight != ULONG_MAX ||
           !sStack.empty( ) ) 
    {
        if (pt->m_ptLeft == ULONG_MAX && pt->m_ptRight == ULONG_MAX) {
            if (!sStack.empty( )) {
                pt = sStack.back();
                sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            }
            break;
        }
        if (pt->m_ptLeft != ULONG_MAX) {
            dDL = DistanceBetween( t, objectStore[pt->m_ptLeft] );
            if ( dDL >= dRadius )
            {   size_t collide;
                tFarthest.insert( tFarthest.end(), make_triple( -dDL, objectStore[pt->m_ptLeft], pt->m_ptLeft) );
                if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), make_triple( -dDL, objectStore[objectCollide[collide]],  objectCollide[collide]));
                    if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                    collide = objectCollide[collide];
                }
            }
        }
        if (pt->m_ptRight != ULONG_MAX) {
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif            
            dDR = DistanceBetween( t, objectStore[pt->m_ptRight]);
            if ( dDR >= dRadius )
            {   size_t collide;
                tFarthest.insert( tFarthest.end(), make_triple( -dDR, objectStore[pt->m_ptRight], pt->m_pt_Right ) );
                if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), make_triple( -dDR, objectStore[objectCollide[collide]],  objectCollide[collide]));
                    if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                    collide = objectCollide[collide];
                }
            }
        }
        
        /*
         See if both branches are populated.  In that case, save one branch
         on the stack, and process the other one based on which one seems
         larger, but useful first]
         */
        if (pt->m_pLeftBranch != 0 && pt->m_pRightBranch != 0 ) {
            if (dDL+pt->m_dMaxLeft > dDR+pt->m_dMaxRight || pt->m_pRightBranch == 0) {
                if ( TRIANG(dRadius,dDL,pt->m_dMaxLeft)) {
                    if ( TRIANG(dRadius,dDR,pt->m_dMaxRight)) {
                        sStack.push_back(pt->m_pRightBranch);
                    }
                    pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                    ++VisitCount;
#endif                                
                    continue;
                }
                /* If we are here, the left branch was not useful
                 Fall through to use the right
                 */
            }
            
            /* We come here either because pursuing the left branch was not useful
             of the right branch look shorter
             */
            if ( TRIANG(dRadius,dDR,pt->m_dMaxRight)) {
                if ( TRIANG(dRadius,dDL,pt->m_dMaxLeft)) {
                    sStack.push_back(pt->m_pLeftBranch);
                }
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif                
                continue;
            } 
        }
        
        /* Only one branch is viable, try them one at a time
         */
        if ( pt->m_pLeftBranch != 0 && TRIANG(dRadius,dDL,pt->m_dMaxLeft)) {
            pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        if ( pt->m_pRightBranch != 0 && TRIANG(dRadius,dDR,pt->m_dMaxRight)) {
            pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        
        /* We have procesed both sides, we need to go to the stack */
        
        if (!sStack.empty( )) {
            pt = sStack.back();
            sStack.pop_back();
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif                            
            continue;
        }
        break;
    }
    if ( !sStack.empty( ) ) // for safety !!!
    {
        std::vector <NearTreeNode* > sTemp;
        sTemp.swap( sStack );
    }
    if( tFarthest.size( ) > 1 ) K_Resize( k, t, tFarthest, dRadius );
    return ( (long)tFarthest.size( ) );
}   // end KFar


//=======================================================================
//  long LeftK_Near ( const DistanceTypeNode dRadius,  
//                std::vector<std::pair<DistanceTypeNode,T> >& tClosest,
//                const TNode& t, const std::vector<TNode>& objectStore ) const
//  long LeftK_Near ( const DistanceTypeNode dRadius,  
//                std::vector<triple<DistanceTypeNode,T,size_t> >& tClosest,
//                const TNode& t, const std::vector<TNode>& objectStore ) const
//
//  Private function to search a NearTree for the objects inside of the specified radius
//     from the probe point
//  This function is only called by LeftFindK_Nearest.
//
// k:           the maximum number of object to return, giving preference to the nearest
// dRadius:     the search radius, which will be updated when the internal store is resized
// tClosest:    is a vector of pairs of Nodes and objects or of triples 
//                 of Nodes, objects and ordinals of objects where the objects
//                 are of the templated type found within dRadius of the
//                 probe point, limited by the k-near search
// t:           is the probe point
// objectStore: the internal vector storing the object in CNearTree
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
/*=======================================================================*/
long LeftK_Near (
             const size_t k,
             DistanceTypeNode& dRadius,
             std::vector<std::pair<DistanceTypeNode,T> >& tClosest,
             const TNode& t,
             const std::vector<TNode>& objectStore,
             const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
             , size_t& VisitCount
#endif
             )
{
    std::vector <NearTreeNode* > sStack;
    enum  { left, right, end } eDir;
    eDir = left; // examine the left nodes first
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif            
    if (pt->m_ptLeft == ULONG_MAX) return false; // test for empty
    while ( ! ( eDir == end && sStack.empty( ) ) )
    {
        if ( eDir == right )
        {
            const DistanceTypeNode dDR =  DistanceBetween( t, objectStore[pt->m_ptRight] );
            if ( dDR <= dRadius )
            {   size_t collide;
                tClosest.insert( tClosest.end(), std::make_pair( dDR, objectStore[pt->m_ptRight] ) );
                if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), std::make_pair( dDR, objectStore[objectCollide[collide]] ));
                    if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                    collide = objectCollide[collide];
                }
            }
            if ( pt->m_pRightBranch != 0 && TRIANG(dDR,pt->m_dMaxRight,dRadius) )
            { // we did the left and now we finished the right, go down
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
                eDir = left;
            }
            else
            {
                eDir = end;
            }
        }
        if ( eDir == left )
        {
            const DistanceTypeNode dDL = DistanceBetween( t, objectStore[pt->m_ptLeft]  );
            if ( dDL <= dRadius )
            {   size_t collide;
                tClosest.insert( tClosest.end(), std::make_pair( dDL, objectStore[pt->m_ptLeft] ) );
                if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), std::make_pair( dDL, objectStore[objectCollide[collide]] ));
                    if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                    collide = objectCollide[collide];
                }
            }
            if ( pt->m_ptRight != ULONG_MAX ) // only stack if there's a right object
            {
                sStack.push_back( pt );
            }
            if ( pt->m_pLeftBranch != 0 && TRIANG(dDL,pt->m_dMaxLeft,dRadius) )
            { // we did the left, go down
                pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
            }
            else
            {
                eDir = end;
            }
        }
        
        if ( eDir == end && !sStack.empty( ) )
        {
            pt = sStack.back( );
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif            
            sStack.pop_back( );
            eDir = right;
        }
    }
    
    if( tClosest.size( ) > k ) K_Resize( k, t, tClosest, dRadius );
    return ( (long)tClosest.size( ) );
}  // end LeftK_Near
long LeftK_Near (
             const size_t k,
             DistanceTypeNode& dRadius,
             std::vector<triple<DistanceTypeNode,T,size_t> >& tClosest,
             const TNode& t,
             const std::vector<TNode>& objectStore,
             const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
             , size_t& VisitCount
#endif
             )
{
    std::vector <NearTreeNode* > sStack;
    enum  { left, right, end } eDir;
    eDir = left; // examine the left nodes first
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif            
    if (pt->m_ptLeft == ULONG_MAX) return false; // test for empty
    while ( ! ( eDir == end && sStack.empty( ) ) )
    {
        if ( eDir == right )
        {
            const DistanceTypeNode dDR =  DistanceBetween( t, objectStore[pt->m_ptRight] );
            if ( dDR <= dRadius )
            {   size_t collide;
                tClosest.insert( tClosest.end(), make_triple( dDR, objectStore[pt->m_ptRight], pt->m_ptRight ) );
                if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), make_triple( dDR, objectStore[objectCollide[collide]], objectCollide[collide]) );
                    if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                    collide = objectCollide[collide];
                }
            }
            if ( pt->m_pRightBranch != 0 && TRIANG(dDR,pt->m_dMaxRight,dRadius) )
            { // we did the left and now we finished the right, go down
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
                eDir = left;
            }
            else
            {
                eDir = end;
            }
        }
        if ( eDir == left )
        {
            const DistanceTypeNode dDL = DistanceBetween( t, objectStore[pt->m_ptLeft]  );
            if ( dDL <= dRadius )
            {   size_t collide;
                tClosest.insert( tClosest.end(), make_triple( dDL, objectStore[pt->m_ptLeft], pt->m_ptLeft ) );
                if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tClosest.insert( tClosest.end(), make_triple( dDL, objectStore[objectCollide[collide]], objectCollide[collide] ) );
                    if( tClosest.size( ) > 2*k ) K_Resize( k, t, tClosest, dRadius );
                    collide = objectCollide[collide];
                }

            }
            if ( pt->m_ptRight != ULONG_MAX ) // only stack if there's a right object
            {
                sStack.push_back( pt );
            }
            if ( pt->m_pLeftBranch != 0 && TRIANG(dDL,pt->m_dMaxLeft,dRadius) )
            { // we did the left, go down
                pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
            }
            else
            {
                eDir = end;
            }
        }
        
        if ( eDir == end && !sStack.empty( ) )
        {
            pt = sStack.back( );
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif            
            sStack.pop_back( );
            eDir = right;
        }
    }
    
    if( tClosest.size( ) > 1 ) K_Resize( k, t, tClosest, dRadius );
    return ( (long)tClosest.size( ) );
}  // end LeftK_Near

//=======================================================================
//  long LeftK_Far ( const DistanceTypeNode dRadius, std::vector<std::pair<DistanceTypeNode,T> >& tFarthest, const TNode& t ) const
//  long LeftK_Far ( const DistanceTypeNode dRadius, std::vector<triple<DistanceTypeNode,T,size_t> >& tFarthest, tFarthest, const TNode& t ) const
//
//  Private function to search a NearTree for the objects inside of the specified radius
//     from the probe point. Distances are stored in an intermediate array as negative values
//     so that the same logic as K_Near can be used.
//  This function is only called by LeftFindK_Farthest.
//
// k:           the maximum number of object to return, giving preference to the nearest
// dRadius:     the search radius, which will be updated when the internal store is resized
// tFarthest:    is a vector of pairs of Nodes and objects or of triples 
//                 of Nodes, objects and ordinals of objects where the objects
//                 are of the templated type found outside of dRadius of the
//                 probe point, limited by the k-farthest search
// t:           is the probe point
// objectStore: the internal vector storing the object in CNearTree
//
// returns the number of objects returned in the container (for sets, that may not equal the number found)
//
/*=======================================================================*/
long LeftK_Far (
            const size_t k,
            DistanceTypeNode& dRadius,
            std::vector<std::pair<DistanceTypeNode,T> >& tFarthest,
            const TNode& t,
            const std::vector<TNode>& objectStore,
            const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
            , size_t& VisitCount
#endif
            )
{
    std::vector <NearTreeNode* > sStack;
    enum  { left, right, end } eDir;
    eDir = left; // examine the left nodes first
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif            
    if (pt->m_ptLeft == ULONG_MAX) return false; // test for empty
    while ( ! ( eDir == end && sStack.empty( ) ) )
    {
        if ( eDir == right )
        {
            const DistanceTypeNode dDR =  DistanceBetween( t, objectStore[pt->m_ptRight] );
            if ( dDR >= dRadius )
            {   size_t collide;
                tFarthest.insert( tFarthest.end(), std::make_pair( -dDR, objectStore[pt->m_ptRight] ) );
                if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), std::make_pair( -dDR, objectStore[objectCollide[collide]]) );
                    if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                    collide = objectCollide[collide];
                }
            }
            if ( pt->m_pRightBranch != 0 && TRIANG(dRadius,dDR,pt->m_dMaxRight) )
            { // we did the left and now we finished the right, go down
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
                eDir = left;
            }
            else
            {
                eDir = end;
            }
        }
        if ( eDir == left )
        {
            const DistanceTypeNode dDL = DistanceBetween( t, objectStore[pt->m_ptLeft]  );
            if ( dDL >= dRadius )
            {   size_t collide;
                tFarthest.insert( tFarthest.end(), std::make_pair( -dDL, objectStore[pt->m_ptLeft] ) );
                if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), std::make_pair( -dDL, objectStore[objectCollide[collide]]) );
                    if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                    collide = objectCollide[collide];
                }
            }
            if ( pt->m_ptRight != ULONG_MAX ) // only stack if there's a right object
            {
                sStack.push_back( pt );
            }
            if ( pt->m_pLeftBranch != 0 && TRIANG(dRadius,dDL,pt->m_dMaxLeft) )
            { // we did the left, go down
                pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
            }
            else
            {
                eDir = end;
            }
        }
        
        if ( eDir == end && !sStack.empty( ) )
        {
            pt = sStack.back( );
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif            
            sStack.pop_back( );
            eDir = right;
        }
    }
    
    if( tFarthest.size( ) > 1 ) K_Resize( k, t, tFarthest, dRadius );
    return ( (long)tFarthest.size( ) );
}  //  end K_Far

long LeftK_Far (
            const size_t k,
            DistanceTypeNode& dRadius,
            std::vector<triple<DistanceTypeNode,T,size_t> >& tFarthest,
            const TNode& t,
            const std::vector<TNode>& objectStore,
            const std::vector<size_t>& objectCollide
#ifdef CNEARTREE_INSTRUMENTED
            , size_t& VisitCount
#endif
            )
{
    std::vector <NearTreeNode* > sStack;
    enum  { left, right, end } eDir;
    eDir = left; // examine the left nodes first
    NearTreeNode* pt = const_cast<NearTreeNode*>(this);
#ifdef CNEARTREE_INSTRUMENTED
    ++VisitCount;
#endif            
    if (pt->m_ptLeft == ULONG_MAX) return false; // test for empty
    while ( ! ( eDir == end && sStack.empty( ) ) )
    {
        if ( eDir == right )
        {
            const DistanceTypeNode dDR =  DistanceBetween( t, objectStore[pt->m_ptRight] );
            if ( dDR >= dRadius )
            {   size_t collide;
                tFarthest.insert( tFarthest.end(), make_triple( -dDR, objectStore[pt->m_ptRight], pt->m_pt_Right ) );
                if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                collide = pt->m_ptRight;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), make_triple( -dDR, objectStore[objectCollide[collide]], objectCollide[collide]) );
                    if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                    collide = objectCollide[collide];
                }
            }
            if ( pt->m_pRightBranch != 0 && TRIANG(dRadius,dDR,pt->m_dMaxRight) )
            { // we did the left and now we finished the right, go down
                pt = pt->m_pRightBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
                eDir = left;
            }
            else
            {
                eDir = end;
            }
        }
        if ( eDir == left )
        {
            const DistanceTypeNode dDL = DistanceBetween( t, objectStore[pt->m_ptLeft]  );
            if ( dDL >= dRadius )
            {   size_t collide;
                tFarthest.insert( tFarthest.end(), make_triple( -dDL, objectStore[pt->m_ptLeft], pt->m_ptLeft) );
                if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                collide = pt->m_ptLeft;
                while (objectCollide[collide] != ULONG_MAX ) {
                    tFarthest.insert( tFarthest.end(), make_triple( -dDL, objectStore[objectCollide[collide]], objectCollide[collide]) );
                    if( tFarthest.size( ) > 2*k ) K_Resize( k, t, tFarthest, dRadius );
                    collide = objectCollide[collide];
                }
            }
            if ( pt->m_ptRight != ULONG_MAX ) // only stack if there's a right object
            {
                sStack.push_back( pt );
            }
            if ( pt->m_pLeftBranch != 0 && TRIANG(dRadius,dDL,pt->m_dMaxLeft) )
            { // we did the left, go down
                pt = pt->m_pLeftBranch;
#ifdef CNEARTREE_INSTRUMENTED
                ++VisitCount;
#endif            
            }
            else
            {
                eDir = end;
            }
        }
        
        if ( eDir == end && !sStack.empty( ) )
        {
            pt = sStack.back( );
#ifdef CNEARTREE_INSTRUMENTED
            ++VisitCount;
#endif            
            sStack.pop_back( );
            eDir = right;
        }
    }
    
    if( tFarthest.size( ) > 1 ) K_Resize( k, t, tFarthest, dRadius );
    return ( (long)tFarthest.size( ) );
}  //  end LeftK_Far


//=======================================================================
// static bool K_Sorter2( const std::pair<DistanceTypeNode, T>& t1, const std::pair<DistanceTypeNode, T>& t2 )
// static bool K_Sorter3( const triple<DistanceTypeNode, T, size_t>& t1, const std::pair<DistanceTypeNode, T>& t2 )
//
//  Private static function used to sort the K-near/far internal data stores. This 
//  replaces the default less<>, which doesn't necessarily exist for all objects.
//  All this does is compare the distances, so only the .first element needs
//  to be examined.
//
//=======================================================================
static bool K_Sorter2( const std::pair<DistanceTypeNode, T>& t1, const std::pair<DistanceTypeNode, T>& t2 )
{
    return ( t1.first < t2.first );
}
static bool K_Sorter3( const triple<DistanceTypeNode, T, size_t>& t1, const triple<DistanceTypeNode, T, size_t>& t2 )
{
    return ( t1.GetFirst() < t2.GetFirst() );
}

//=======================================================================
//  void K_Resize ( const size_t k, const TNode& t, std::vector<std::pair<DistanceTypeNode, T> >& tClosest, DistanceTypeNode& dRadius ) const
//  void K_Resize ( const size_t k, const TNode& t, std::vector<triple<DistanceTypeNode, T, size_t> >& tClosest, DistanceTypeNode& dRadius ) const
//
//  Private function to limit the size of internally stored data for K-nearest/farthest-neighbor searches
//  This function is only called by K_Near and K_Far.
//
//    dRadius is the search radius, updated to the best-known value
//    tClosest is a vector of pairs of Nodes and objects or of triples 
//         of Nodes, objects and ordinals of objects where the objects
//         are of the templated type found within dRadius of the
//         probe point
//    t  is the probe point
//
//=======================================================================
void K_Resize( const size_t k, const TNode& t, std::vector<std::pair<DistanceTypeNode, T> >& tClosest, DistanceTypeNode& dRadius )
{
    size_t target = tClosest.size();
    if (target > k ) target = k;
    std::sort( tClosest.begin(), tClosest.end(), &K_Sorter2 );
    tClosest.resize( target );
    dRadius = DistanceBetween( t, tClosest[tClosest.size()-1].second );
}  // end K_Resize
void K_Resize( const size_t k, const TNode& t, std::vector<triple<DistanceTypeNode, T, size_t> >& tClosest, DistanceTypeNode& dRadius )
{
    size_t target = tClosest.size();
    if (target > k ) target = k;
    std::sort( tClosest.begin(), tClosest.end(), &K_Sorter3 );
    tClosest.resize( target );
    dRadius = DistanceBetween( t, tClosest[tClosest.size()-1].GetSecond() );
}  // end K_Resize



}; // end NearTreeNode
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
// end NearTreeNode nested class in CNearTree
//=======================================================================
// start of iterator, a nested class in CNearTree
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================
//=======================================================================

//====================================================================================
// class iterator
//
// nested class within CNearTree
//====================================================================================
public:
    class iterator
        : public std::iterator< std::random_access_iterator_tag, T, int, T*, T& >
    {
    public:
        friend class CNearTree< T, DistanceType, distMinValue >;
    private:
        long position;
        const CNearTree< T, DistanceType, distMinValue >* parent;

    public:
        iterator( void ) { }; // constructor
        explicit iterator( const const_iterator& s ) { position = ((const_iterator)s).get_position(); parent = ((const_iterator)s).get_parent(); };// constructor

        iterator& operator=  ( const iterator& s )       { position = s.position; parent = s.parent; return ( *this ); };
        iterator& operator=  ( const const_iterator& s ) { position = ((const_iterator&)s).get_position(); parent = ((const_iterator&)s).get_parent(); return ( *this ); };
        iterator  operator++ ( const int n )             { iterator it(*this); position+=1+n; return ( it ); };
        iterator  operator-- ( const int n )             { iterator it(*this); position-=1+n; return ( it ); };
        iterator& operator++ ( void )                    { ++position; return ( *this ); };
        iterator& operator-- ( void )                    { --position; return ( *this ); };
        iterator  operator+  ( const long n ) const      { iterator it( position+n, parent); return ( it ); };
        iterator  operator-  ( const long n ) const      { iterator it( position-n, parent); return ( it ); };
        iterator& operator+= ( const long n )            { position += n; return ( *this ); };
        iterator& operator-= ( const long n )            { position -= n; return ( *this ); };
        T         operator*  ( void )         const      { return ( parent->m_ObjectStore[position] ); };

        bool      operator== ( const iterator& t ) const { return ( t.position==(parent->m_ObjectStore.empty( )?1:position) && t.parent==parent ); };
        bool      operator!= ( const iterator& t ) const { return ( ! (*this==t )); };
        bool      operator== ( const const_iterator& t ) const { return ( ((const_iterator&)t).get_position()==(parent->m_ObjectStore.empty( )?1:position) &&  ((const_iterator&)t).get_parent()==parent ); };
        bool      operator!= ( const const_iterator& t ) const { return ( ! (*this==t )); };
        bool      operator>  ( const iterator& t       ) const { return ( (*this).get_position()>t.get_position() ); };
        bool      operator>  ( const const_iterator& t ) const { return ( (*this).get_position()>t.get_position() ); };
        bool      operator<  ( const iterator& t       ) const { return ( (*this).get_position()<t.get_position() ); };
        bool      operator<  ( const const_iterator& t ) const { return ( (*this).get_position()<t.get_position() ); };

        const T * const operator-> ( void )   const      { return ( &(const_cast<CNearTree*>(parent)->m_ObjectStore[position]) ); };
        long get_position( void ) const {return position;};
        const CNearTree< T, DistanceType, distMinValue >* get_parent( void ) {return parent;};

    private:
        iterator ( const long s, const CNearTree* const nt ) { position = s; parent = nt; }; // constructor

    }; // class iterator
//====================================================================================
// end of nested class "iterator"
//====================================================================================

class const_iterator
        : public std::iterator< std::random_access_iterator_tag, T, int, T*, T& >
    {
    public:
        friend class CNearTree< T, DistanceType, distMinValue >;
    private:
        long position;
        const CNearTree< T, DistanceType, distMinValue >* parent;

    public:
        const_iterator( void ) { }; // constructor
        explicit const_iterator( const iterator& s ) { position = ((iterator)s).get_position(); parent = ((iterator)s).get_parent(); }; // constructor

        const_iterator& operator=  ( const const_iterator& s ) { position = s.position; parent = s.parent; return ( *this ); };
        const_iterator& operator=  ( const       iterator& s ) { position = ((iterator &)s).get_position(); parent = ((iterator &)s).get_parent(); return ( *this ); };
        const_iterator  operator++ ( const int n )             { const_iterator it(*this); position+=1+n; return ( it ); };
        const_iterator  operator-- ( const int n )             { const_iterator it(*this); position-=1+n; return ( it ); };
        const_iterator& operator++ ( void )                    { ++position; return ( *this ); };
        const_iterator& operator-- ( void )                    { --position; return ( *this ); };
        const_iterator  operator+  ( const long n ) const      { const_iterator it( position+n, parent); return ( it ); };
        const_iterator  operator-  ( const long n ) const      { const_iterator it( position-n, parent); return ( it ); };
        const_iterator& operator+= ( const long n )            { position += n; return ( *this ); };
        const_iterator& operator-= ( const long n )            { position -= n; return ( *this ); };
        T               operator*  ( void )         const      { return ( parent->m_ObjectStore[position] ); };

        bool            operator== ( const const_iterator& t ) const { return ( t.position==(parent->m_ObjectStore.empty( )?1:position) && t.parent==parent ); };
        bool            operator!= ( const const_iterator& t ) const { return ( ! (*this==t )); };
        bool            operator== ( const iterator& t ) const { return ( ((iterator &)t).get_position()==(parent->m_ObjectStore.empty( )?1:position) && ((iterator &)t).get_parent()==parent ); };
        bool            operator!= ( const iterator& t ) const { return ( ! (*this==t )); };
        bool      operator>  ( const iterator& t       ) const { return ( (*this).get_position()>t.get_position() ); };
        bool      operator>  ( const const_iterator& t ) const { return ( (*this).get_position()>t.get_position() ); };
        bool      operator<  ( const iterator& t       ) const { return ( (*this).get_position()<t.get_position() ); };
        bool      operator<  ( const const_iterator& t ) const { return ( (*this).get_position()<t.get_position() ); };

        const T * const operator-> ( void )   const      { return ( &(const_cast<CNearTree*>(parent)->m_ObjectStore[position]) ); };
        long get_position          ( void ) const  {return position;};
        const CNearTree< T, DistanceType, distMinValue >* get_parent( void ) {return parent;};

    private:
        const_iterator ( const long s, const CNearTree* nt ) { position = s; parent = nt; }; // constructor

    }; // end class const_iterator
//====================================================================================
// end of nested class "const_iterator"
//====================================================================================


}; // template class CNearTree

#endif // !defined(TNEAR_H_INCLUDED)





