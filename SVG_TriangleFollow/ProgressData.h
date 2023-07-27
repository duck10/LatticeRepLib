#ifndef PROGRESSDATA_H

#define PROGRESSDATA_H

#pragma warning( disable : 4100) //  unreferenced formal parameter


#include <cstddef>
#include <vector>

template<typename T>
class ProgressData {
public:
   virtual ~ProgressData(void) {}
   virtual const bool PointIsError(const int n, const double rawDeltaDist12, const double rawDeltaDist13, const double rawDeltaDist23) const { return false; }
   virtual const std::vector<int> PointIsError( void ) const { return std::vector<int>(); }
   void clear( void ) { m_progressdata.clear( ); }
   void resize( const unsigned int n ) { m_progressdata.resize( n ); }
   //std::vector<double>& operator[]( const unsigned int n ) const { return m_progressdata[n]; }
   std::vector<T> GetVector( void ) const { return m_progressdata; }
public:
   void push_back( const T& t ) { m_progressdata.push_back( t ); }
   size_t size( void ) const { return m_progressdata.size( ); }
   T operator[] (const unsigned int n) const { return m_progressdata[n]; }
   T& operator[] (const unsigned int n) { return m_progressdata[n]; }
   bool empty( ) const { return m_progressdata.empty( ); }
   void SetVector( const std::vector<T>& v ) { m_progressdata = v; }

protected:
    std::vector<T> m_progressdata;
};

template<typename T>
class FollowVectors : public ProgressData<T>
{
   const bool PointIsError( const int n , const double rawDeltaDist12, const double rawDeltaDist13, const double rawDeltaDist23 ) const { return false; }
   const std::vector<int> PointIsError(void) const { return std::vector<int>(); }
   T operator= (const T& t) const {
      *this = t;
   }
};

class NormedDist : public ProgressData<double>
{
const bool PointIsError( const int n, const double rawDeltaDist12, const double rawDeltaDist13, const double rawDeltaDist23 ) const;
const std::vector<int> PointIsError(void) const { return std::vector<int>(); }
public:
   void SetPointIsError( const std::vector<double>& tanhHaar );

private:
   std::vector<bool> m_iserror;
};

class Dist23Delta : public ProgressData<double>
{
   const bool PointIsError( const int n , const double rawDeltaDist12, const double rawDeltaDist13, const double rawDeltaDist23) const { return false; }
   const std::vector<int> PointIsError(void) const { return std::vector<int>(); }
};

class TanhDist23Delta : public ProgressData<double>
{
   const bool PointIsError( const int n , const double rawDeltaDist12, const double rawDeltaDist13, const double rawDeltaDist23) const { return false; }
   const std::vector<int> PointIsError(void) const { return std::vector<int>(); }
};

class AngleDiff : public ProgressData<double>
{
   const bool PointIsError( const int n , const double rawDeltaDist12, const double rawDeltaDist13, const double rawDeltaDist23) const { return m_progressdata[n] > 180.0; }
   const std::vector<int> PointIsError(void) const { return std::vector<int>(); }
};

class TriangleDiff : public ProgressData<double>
{
   const bool PointIsError( const int n , const double rawDeltaDist12, const double rawDeltaDist13, const double rawDeltaDist23) const { return m_progressdata[n] >0.0; };
   //const std::vector<int> PointIsError(void) const { return std::vector<int>(); }
};

class TriangleArea : public ProgressData<double>
{
   const bool PointIsError( const int n , const double rawDeltaDist12, const double rawDeltaDist13, const double rawDeltaDist23) const { return m_progressdata[n] < 0.0; }
   const std::vector<int> PointIsError(void) const { return std::vector<int>(); }
};

class Dist12 : public ProgressData<double>
{
   const bool PointIsError( const int n , const double rawDeltaDist12, const double rawDeltaDist13, const double rawDeltaDist23) const { return false; }
   const std::vector<int> PointIsError(void) const { return std::vector<int>(); }
};

class Dist13 : public ProgressData<double>
{
   const bool PointIsError( const int n , const double rawDeltaDist12, const double rawDeltaDist13, const double rawDeltaDist23) const { return false; }
   const std::vector<int> PointIsError(void) const { return std::vector<int>(); }
};

class Dist23 : public ProgressData<double>
{
   const bool PointIsError( const int n , const double rawDeltaDist12, const double rawDeltaDist13, const double rawDeltaDist23) const { return false; }
   const std::vector<int> PointIsError(void) const { return std::vector<int>(); }
};

#endif
