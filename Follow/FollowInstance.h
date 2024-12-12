#ifndef FOLLOWINSTANCE_H
#define FOLLOWINSTANCE_H

#include "LatticeCell.h"
#include "S6.h"

static RHrand ran2;

#include <iostream>

template<typename T>
T CreateTotallyRandomUnitOrthogonalComponent(const T& t) {
   // assume t is not null and rand is not parallel to t
   const T base = t / t.norm();
   S6 temp;
   for (size_t i = 0; i < 6; ++i) temp[i] = ran2.urand() - 0.5;
   temp /= temp.norm();
   const double d = dot(VecN(base.GetVector()), VecN(T(temp).GetVector()));
   T ortho = temp - d * base;
   ortho.SetValid(true);
   return ortho / ortho.norm();
}


class FollowInstance {
public:
   friend std::ostream& operator<<(std::ostream& os, const FollowInstance& fi) {
      os << "file name " << fi.m_RawFileName << std::endl;
      os << "file name " << fi.m_basicFileName << std::endl;
      os << "file name " << fi.m_FullFileName << std::endl;
      for (size_t rrr = 0; rrr < fi.m_followSeed.size(); ++rrr)
      {
         os << "seed " << rrr << "  " << S6(fi.m_followSeed[rrr].getCell()) << std::endl;
      }
      os << "trial # " << fi.m_trial << std::endl;
      os << "perturbation # " << fi.m_perturbation << std::endl;
      return os;
   }

   FollowInstance() = default;
   FollowInstance(const std::vector<LatticeCell>& cells, const int trial, const int perturbation)
      : m_followSeed(cells)
      , m_trial(trial)
      , m_perturbation(perturbation)
   {}

   void AddFileNames (
      const std::string_view& RawName,
      const std::string_view& basicName,
      const std::string_view& fullName)
   {
      m_RawFileName = RawName;
      m_basicFileName = basicName;
      m_FullFileName = fullName;
   }

   static std::vector<LatticeCell> Perturb(const std::vector<LatticeCell>& vlc, const double perturbby)
   {
      std::vector<LatticeCell> out;
      for (const auto& vl : vlc) {
         const S6 ortho = S6(vl).norm() * S6(CreateTotallyRandomUnitOrthogonalComponent(S6(vl)));
         double orthonorm = ortho.norm();
         double s6norm = S6(vl).norm();
         const S6 s6 = S6(vl) + perturbby * ortho / 100.0;  // perturbation is in percent
         out.emplace_back(s6);
         const S6 diff = (s6 - vl);
         const double ratio = diff.norm() / S6(vl).norm();
      }
      return out;
   }

   size_t size() const { return m_followSeed.size(); }
   int GetTrial() const { return m_trial; }
   int GetPerturbation() const { return m_perturbation; };
   std::vector<LatticeCell> GetFollowSeed() { return m_followSeed; }
   std::string GetRawFileName() const { return  m_RawFileName;}
   std::string GetBasicFileName() const { return  m_basicFileName;}
   std::string GetFullFileName() const { return  m_FullFileName;}
   std::vector<LatticeCell> GetFollowSeed() const { return m_followSeed; }

private:
   std::vector<LatticeCell> m_followSeed;
   std::string m_RawFileName;
   std::string m_basicFileName;
   std::string m_FullFileName;
   int m_trial;
   int m_perturbation;
};


#endif // FOLLOWINSTANCE_H