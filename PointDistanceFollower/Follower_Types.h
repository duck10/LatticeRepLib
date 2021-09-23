#ifndef FOLLOWER_TYPES
#define FOLLOWER_TYPES

#include <ctime>
#include "D13.h"
#include "MultiFollower.h"

template<typename T>
class Follower_Type_Base {
public:
  virtual  std::vector<std::pair<T, T> > GetPath(void) { return m_Path; }
  virtual MultiFollower CalculateDistances(const MultiFollower& mf) const = 0;
  virtual void SetLatticePointChoiceForDistanceCalculation() = 0;
  void SetDistance6(const std::vector<double>& v) { LRL_Path<T>::SetDistances(v); }

protected:
   std::vector<std::pair<T,T> >m_Path;
   std::string m_name;
   void SetComputeStartTime(void) { m_ComputeStartTime = std::clock(); }
   std::clock_t GetComputeStartTime(void) const { return m_ComputeStartTime; }
   void SetTime2ComputeFrame(const double computeTime) {
      m_seconds2ComputerFrame = computeTime;
   }
   double m_seconds2ComputerFrame;
   double GetTime2ComputeFrame() const { return m_seconds2ComputerFrame; }
   std::clock_t GetComputeStartTime(void) const {
      return m_ComputeStartTime
};

class Follower_Type_D13 : public Follower_Type_Base<D13> {
public:
   Follower_Type_D13();

   MultiFollower CalculateDistances(const MultiFollower& mf) const;
   void SetLatticePointChoiceForDistanceCalculation();
protected:
};



#endif // FOLLOWER_TYPES
