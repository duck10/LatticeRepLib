#ifndef LRL_PATH_H
#define LRL_PATH_H

#include <algorithm>
#include <set>
#include <utility>
#include <vector>

template<typename T>
class LRL_Path {
public:
   LRL_Path() {}
   LRL_Path(const std::vector<std::pair<T, T> >& path) 
      :m_path(path)
   {
   }

   size_t size() const { return m_path.size(); }

   bool empty(void) const { return m_path.empty(); }

   std::vector<std::pair<T, T> > GetPath(void) const { return m_path; }

   std::vector<double> GetDistances(void) const { 
      return m_distances; 
   }

   void SetDistances(const std::vector<double>& v) { m_distances = v; }
   void SetBestFit(const T& lattice) { m_bestFit = lattice; }

   double GetMin(void) const {
      // be aware -- negative distance means invalid lattice !!!!!!!!!!!!!!
      double mindist = m_distances[0];
      for (size_t i = 1; i < m_distances.size(); ++i)
         if (m_distances[i] >= 0.0) mindist = std::min(mindist, m_distances[i]);
      return mindist;
   }

   double GetMax(void) const {
      return *std::max_element(m_distances.begin(), m_distances.end());
   }

   std::set<size_t> GetGlitches(void) const { return m_glitches; }
   void SetGlitches(const std::set<size_t>& glitches) { m_glitches = glitches; }

   bool HasGlitches(void) const { return !m_glitches.empty(); }

   void SetComputeStartTime(void) { m_ComputeStartTime = std::clock(); }
   std::clock_t GetComputeStartTime(void) const { return m_ComputeStartTime; }
   void SetTime2ComputeFrame(const double computeTime) {
      m_seconds2ComputerFrame = computeTime;
   }
   double GetTime2ComputeFrame() const { return m_seconds2ComputerFrame;  }

private:
   std::vector<std::pair<T, T> > m_path;
   T m_bestFit;
   std::clock_t m_ComputeStartTime;
   double m_seconds2ComputerFrame;
   std::vector<double> m_distances;
   std::set<size_t> m_glitches;
};


#endif // LRL_PATH_H

