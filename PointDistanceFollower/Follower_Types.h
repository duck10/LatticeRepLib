#ifndef FOLLOWER_TYPES
#define FOLLOWER_TYPES

#include <ctime>
#include "D13.h"
#include "MultiFollower.h"



class Follower_Type_D13 {
public:
   Follower_Type_D13();

   MultiFollower CalculateDistances(const MultiFollower& mf) const;
   void SetLatticePointChoiceForDistanceCalculation();
protected:
};

class Follower_Type {
public:
   Follower_Type(const Follower_Type_D13);
  //virtual  std::vector<std::pair<T, T> > GetPath(void) { return m_Path; }
  MultiFollower CalculateDistances(const MultiFollower& mf) const;
  //virtual void SetLatticePointChoiceForDistanceCalculation() = 0;
  //void SetDistance6(const std::vector<double>& v) { LRL_Path<T>::SetDistances(v); }

protected:
   // data
   std::vector<std::pair<S6, S6> >m_SellingPath;
   std::vector<std::pair<G6, G6> >m_NiggliPath;
   std::vector<std::pair<D13, D13> >m_D13Path;
   std::string m_name;
   std::clock_t m_ComputeStartTime;
   double m_seconds2ComputerFrame;

   // functions
   void SetComputeStartTime(void) { m_ComputeStartTime = std::clock(); }
   std::clock_t GetComputeStartTime(void) const { return m_ComputeStartTime; }
   void SetTime2ComputeFrame(const double computeTime) {
      m_seconds2ComputerFrame = computeTime;
   }
   double GetTime2ComputeFrame() const { return m_seconds2ComputerFrame; }

private:
   Follower_Type();  // do not implement
};



#endif // FOLLOWER_TYPES
