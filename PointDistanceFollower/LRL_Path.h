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
   LRL_Path(const std::vector<std::pair<T, T> >& path, const std::vector<std::pair<T, T> >& secondPath) 
      :m_path(path),
      m_secondPath(secondPath)
   {
   }

   bool HasSecondPath(void) const { return !m_secondPath.empty(); }

   unsigned long size() const { return (unsigned long)(m_path.size()); }

   bool empty(void) const { return m_path.empty(); }

   std::vector<std::pair<T, T> > GetPath(void) const { return m_path; }
   std::vector<std::pair<T, T> > GetSecondPath(void) const { return m_secondPath; }

   std::vector<double> GetDistances(void) const { return m_distances; }

   void SetDistances(const std::vector<double>& v) { m_distances = v; }

   double GetMin(void) const {
      return *std::min_element(m_distances.begin(), m_distances.end());
   }

   double GetMax(void) const {
      return *std::max_element(m_distances.begin(), m_distances.end());
   }

   std::set<unsigned long> GetGlitches(void) const { return m_glitches; }
   void SetGlitches(const std::set<unsigned long>& glitches) { m_glitches = glitches; }

   bool HasGlitches(void) const { return !m_glitches.empty(); }

   void SetComputeStartTime(void) { m_ComputeStartTime = std::clock(); }
   std::clock_t GetComputeStartTime(void) const { return m_ComputeStartTime; }
   void SetTime2ComputeFrame(const double computeTime) {
      m_seconds2ComputerFrame = computeTime;
   }
   double GetTime2ComputeFrame() const { return m_seconds2ComputerFrame;  }

private:
   std::vector<std::pair<T, T> > m_path;
   std::vector<std::pair<T, T> > m_secondPath;
   std::clock_t m_ComputeStartTime;
   double m_seconds2ComputerFrame;
   std::vector<double> m_distances;
   std::set<unsigned long> m_glitches;
};


#endif // LRL_PATH_H

