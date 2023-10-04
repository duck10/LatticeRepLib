#ifndef TEST3_VS_NCDIST_H
#define TEST3_VS_NCDIST_H

#include "G6.h"
#include "MatDC7u.h"

class Test3Results {
public:
   friend std::ostream& operator<< (std::ostream&, const Test3Results&);

   G6 GetG1() const { return m_g1; }
   G6 GetG2() const { return m_g2; }
   double GetNCDist() const { return m_NCDist; }
   double GetDist() const { return m_DC7uDistance; }
   std::vector<MatDC7u> GetMatrices() const { return m_vm; }

   void SetG1(const G6& g) { m_g1 = g; }
   void SetG2(const G6& g) { m_g2 = g; }
   void SetNCDist(const double d) { m_NCDist = d; }
   void SetMatrices(const std::vector<MatDC7u>& v) { m_vm = v; }
   void SetDist(const double d) { m_DC7uDistance = d; }



private:
   double m_DC7uDistance;
   double m_NCDist;
   std::vector<MatDC7u> m_vm;
   G6 m_g1;
   G6 m_g2;

};

class TestDistance {
public:
   TestDistance() {}
   static std::vector<MatDC7u>  SelectMatrices(const G6& g);
   static G6 G6_rand_reduced();
   static std::vector<Test3Results> Test3_VS_NCDist();
   static double BestDistance(const G6& g1, const G6& g2,
      const std::vector<MatDC7u>& vm);
};

;

#endif // TEST3_VS_NCDIST_H
