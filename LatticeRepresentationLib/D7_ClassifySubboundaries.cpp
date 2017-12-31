
#include "stdafx.h"

#include "LRL_Cell.h"
#include "D7_ClassifySubboundaries.h"
#include "D7_Boundary.h"
#include "D7_BoundaryList.h"
#include "Delone.h"
#include "MatS6.h"
#include "LRL_MinMaxTools.h"
#include "StoreResults.h"
#include "D7.h"

#include <map>

class StringVecKey {
public:
   StringVecKey(const std::string& s, const D7& v) : m_s(s), m_v(v) {}

   friend std::ostream& operator<< (std::ostream& os, const StringVecKey& svk) {
      os << svk.m_s << std::string("  ") << svk.m_v;
      return os;
   }

   std::ostream& operator<< (std::ostream& os) const {
      os << m_s << std::string("  ") << m_v;
      return os;
   }

   std::string GetString(void) const { return m_s; }
   D7 GetVector(void) const { return m_v; }

private:
   std::string m_s;
   D7 m_v;
};

int HashV7(const double d1, const double d2, const double d3, const double b4) {
   return
      (d1 <= d2) +
      (d1 <= d3) * 2 +
      (d1 <= b4) * 4 +
      (d2 <= d3) * 8 +
      (d2 <= b4) * 16 +
      (d3 <= b4) * 32;
}

int HashV7(const int caseN, const double d1, const double d2, const double d3, const double b4) {
   return
      (d1 <= d2) +
      (d1 <= d3) * 2 +
      (d1 <= b4) * 4 +
      (d2 <= d3) * 8 +
      (d2 <= b4) * 16 +
      (d3 <= b4) * 32 +
      caseN * 64;
}

std::map<std::string, int> storeClassify;
StoreResults<int, StringVecKey > srClassify(5U);

std::string D7_ClassifySubboundaries::ClassifyCase4(const D7& v, const double delta) {
   //const double d1 = v[0];  // |a|^2
   const double d2 = v[1];  // |b|^2
   const double d3 = v[2];  // |c|^2
   //const double b4 = v[3];  // |d|^2
   //const double d5 = v[4];  // |b+c|^2
   const double s6 = v[5];  // |a+c|^2
   const double d7 = v[6];  // |a+b|^2
   std::string result;

   if (d2 - delta <= minNC(d3, s6, d7)) { // Cases 4.1 thru 4.6
      if (d3 - delta <= minNC(s6, d7)) {
         result += "Case 4.1 ";
      }
      if (d3 + delta >= minNC(s6, d7)) {
         result += "Case 4.2 ";
      }

      if (s6 - delta <= minNC(d7, d3)) {
         result += "Case 4.3 ";
      }
      if (s6 + delta >= minNC(d7, d3)) {
         result += "Case 4.4 ";
      }

      if (d7 - delta <= minNC(d3, s6)) {
         result += "Case 4.5 ";
      }
      if (d7 + delta >= minNC(d3, s6)) {
         result += "Case 4.6 ";
      }

      if (result.empty())
      {
         result += "Case unknown in 4.1 to 4.6 ";
      }
   }
   if (s6 - delta <= minNC(d2, d3, d7) && d2 - delta <= minNC(d3, d7) && d3 - delta <= d7) {// Case 4.7
      result += "Case 4.7 ";
   }
   if (d7 - delta <= minNC(d2, d3, s6) && d2 - delta <= minNC(d3, s6) && d3 - delta <= s6) {  // Case 4.8
      result += "Case 4.8 ";
   }
   if ( result.empty() ) {
      result += "Case 4 (not 7,8) unknown ";
   }

   std::map<std::string, int>::iterator it = storeClassify.find(result);
   if (it == storeClassify.end())
      storeClassify[result] = 1;
   else
      storeClassify[result] = ++storeClassify[result];

   srClassify.Store(HashV7(4, d2, d3, s6, d7), StringVecKey(result, v));

   return result;
}

std::string D7_ClassifySubboundaries::ClassifyCase5(const D7& v, const double delta) {
   const double d1 = v[0];  // |a|^2
   //const double d2 = v[1];  // |b|^2
   //const double d3 = v[2];  // |c|^2
   const double b4 = v[3];  // |d|^2
   //const double d5 = v[4];  // |b+c|^2
   const double s6 = v[5];  // |a+c|^2
   const double d7 = v[6];  // |a+b|^2
   std::string result;

   if (d1 - delta <= minNC(b4, s6, d7)) {
      if (b4 - delta <= minNC(s6, d7)) {
         result += "Case 5.1 ";
      }
      if (b4 + delta >= minNC(s6, d7)) {
         result += "Case 5.2 ";
      }

      if (s6 - delta <= minNC(b4, d7)) {
         result += "Case 5.3 ";
      }
      if (s6 + delta >= minNC(b4, d7)) {
         result += "Case 5.4 ";
      }

      if (d7 - delta < minNC(b4, s6)) {
         result += "Case 5.5 ";
      }
      if (d7 + delta > minNC(b4, s6)) {
         result += "Case 5.6 ";
      }

      if (result.empty()) {
         result += "Case unknown in 5.1-5.4 ";
      }
   }
   if (s6 - delta <= minNC(d1, b4, d7) && d1 - delta <= minNC(b4, d7) && b4 - delta <= d7) {
      result += "Case 5.7 ";
   }
   if (d7 - delta <= minNC(d1, b4, s6) && d1 - delta <= minNC(b4, s6) && b4 - delta <= s6) {
      result += "Case 5.8 ";
   }
   if (result.empty()) {
      result += "Case 5 unknown ";
   }

   std::map<std::string, int>::iterator it = storeClassify.find(result);
   if (it == storeClassify.end())
      storeClassify[result] = 1;
   else
      storeClassify[result] = ++storeClassify[result];
   srClassify.Store(HashV7(5, d1, b4, s6, d7), StringVecKey(result, v));
   return result;
}

std::string D7_ClassifySubboundaries::ClassifyCase6(const D7& v, const double delta) {
   const double d1 = v[0];  // |a|^2
   //const double d2 = v[1];  // |b|^2
   const double d3 = v[2];  // |c|^2
   //const double b4 = v[3];  // |d|^2
   const double d5 = v[4];  // |b+c|^2
   //const double s6 = v[5];  // |a+c|^2
   const double d7 = v[6];  // |a+b|^2
   std::string result;

   if (d1 - delta <= minNC(d3, d5, d7)) { // Cases 6.1 thru 6.6
      if (d3 - delta <= minNC(d5, d7)) {
         result += "Case 6.1 ";
      }
      if (d3 + delta >= minNC(d5, d7)) {
         result += "Case 6.2 ";
      }

      if (d5 - delta <= minNC(d7, d3)) {
         result += "Case 6.3 ";
      }
      if (d5 + delta >= minNC(d7, d3)) {
         result += "Case 6.4 ";
      }

      if (d7 - delta <= minNC(d3, d5)) {
         result += "Case 6.5 ";
      }
      if (d7 + delta >= minNC(d3, d5)) {
         result += "Case 6.6 ";
      }

      if (result.empty()) {
         result += "Case unknown in 6.1 to 6.6 ";
      }
   }
   if (d5 - delta <= minNC(d1, d3, d7) && d1 - delta <= minNC(d3, d7) && d3 - delta <= d7) {// Case 6.7
      result += "Case 6.7 ";
   }

   if (d7 - delta <= minNC(d1, d3, d5) && d1 - delta <= minNC(d3, d5) && d3 - delta <= d5) {  // Case 6.8
      result += "Case 6.8 ";
   }

   if (result.empty()) {
      result += "Case 6 unknown ";
   }

   std::map<std::string, int>::iterator it = storeClassify.find(result);
   if (it == storeClassify.end())
      storeClassify[result] = 1;
   else
      storeClassify[result] = ++storeClassify[result];
   srClassify.Store(HashV7(6, d1, d3, d5, d7), StringVecKey(result, v));
   return result;
}

std::string D7_ClassifySubboundaries::ClassifyCase8(const D7& v, const double delta) {
   const double d1 = v[0];  // |a|^2
   const double d2 = v[1];  // |b|^2
   //const double d3 = v[2];  // |c|^2
   //const double b4 = v[3];  // |d|^2
   const double d5 = v[4];  // |b+c|^2
   const double s6 = v[5];  // |a+c|^2
   //const double d7 = v[6];  // |a+b|^2
   std::string result;

   if (d1- delta <= minNC(d2, d5, s6)) {
      // Cases 8.1 - 8.4
      if (d2 - delta <= minNC(d5, s6)) {
         result += "Case 8.1 ";
      }
      if (d2 + delta >= minNC(d5, s6)) {
         result += "Case 8.2 ";
      }

      if (d5 - delta <= minNC(d2, s6)) {
         result += "Case 8.3 ";
      }
      if (d5 + delta >= minNC(d2, s6)) {
         result += "Case 8.x ";
      }

      if (s6 - delta <= minNC(d2, d5)) {
         result += "Case 8.4 ";
      }
      if (s6 + delta >= minNC(d2, d5)) {
         result += "Case 8.y ";
      }

      if (result.empty()) {
         result += "Case unknown in 8.1 to 8.4 ";
      }
   }
   if (s6 - delta <= minNC(d1, d2, s6) && d1 - delta <= minNC(d2, d5) && d2 - delta <= d5) {
      result += "Case 8.5 ";
   }
   if (d5 - delta <= minNC(d1, d2, d5) && d1 - delta <= minNC(d2, s6) && d2 - delta <= s6) {
      result += "Case 8.6 ";
   }
   if ( result.empty()) {
      result += "Case 8 unknown ";
   }

   std::map<std::string, int>::iterator it = storeClassify.find(result);
   if (it == storeClassify.end())
      storeClassify[result] = 1;
   else
      storeClassify[result] = ++storeClassify[result];
   srClassify.Store(HashV7(8, d1, d2, d5, s6), StringVecKey(result, v));
   return result;
}

std::string D7_ClassifySubboundaries::ClassifyCase9(const D7& v, const double delta) {
   //const double d1 = v[0];  // |a|^2
   //const double d2 = v[1];  // |b|^2
   const double d3 = v[2];  // |c|^2
   const double b4 = v[3];  // |d|^2
   const double d5 = v[4];  // |b+c|^2
   const double s6 = v[5];  // |a+c|^2
   //const double d7 = v[6];  // |a+b|^2
   std::string result;

   if (d3 - delta <= minNC(b4, d5, s6) && s6 - delta <= minNC(b4, d5) && b4 - delta <= d5) {
      result = "Case 9.1";
      //    caseXSub[0].SetTransform( MatD7( "0 0 1 0 0 0 0   0 0 0 0 0 1 0   0 0 0 0 1 0 0   0 0 0 1 0 0 0   -2 -2 0 0 2 2 1   0 1 0 0 0 0 0   1 0 0 0 0 0 0" ) );
   }
   else {
      result = "Case unknown in 9";
   }

   std::map<std::string, int>::iterator it = storeClassify.find(result);
   if (it == storeClassify.end())
      storeClassify[result] = 1;
   else
      storeClassify[result] = ++storeClassify[result];
   srClassify.Store(HashV7(9, d3, b4, d5, s6), StringVecKey(result, v));
   return result;
}

std::string D7_ClassifySubboundaries::ClassifyCase7(const D7& v, const double delta) {
   //   caseXSub[0].SetTransform( MatD7( "0 1 0 0 0 0 0   0 0 0 0 0 0 1   0 0 0 0 1 0 0   0 0 0 1 0 0 0   -2 0 -2 0 2 1 2   0 0 1 0 0 0 0   1 0 0 0 0 0 0" ) );
      //const double d1 = v[0];  // |a|^2
   const double d2 = v[1];  // |b|^2
   //const double d3 = v[2];  // |c|^2
   const double b4 = v[3];  // |d|^2
   const double d5 = v[4];  // |b+c|^2
   //const double s6 = v[5];  // |a+c|^2
   const double d7 = v[6];  // |a+b|^2
   std::string result;

   if (d2 - delta <= minNC(b4, d5, d7) && d7 - delta <= minNC(b4, d5) && d5 - delta <= b4) {
      result = "Case 7.1";
   }
   else {
      result = "Case unknown in 7";
   }

   std::map<std::string, int>::iterator it = storeClassify.find(result);
   if (it == storeClassify.end())
      storeClassify[result] = 1;
   else
      storeClassify[result] = ++storeClassify[result];
   srClassify.Store(HashV7(7, d2, b4, d5, d7), StringVecKey(result, v));
   return result;
}

void D7_ClassifySubboundaries::TestClassify(const int ntest, const D7_Boundary& d, std::string(*fn)(const D7& v, const double delta), const double delta) {
   S6 s6out;
   D7 v7out;
   MatS6 m6;
   int maxtest = ntest;
   for (int i = 0; i < maxtest; ++i) {
      if (i%maxtest / 100 == 0 && i > 100 - 1) std::cout << i << std::endl;
      const D7 ranvec = D7(LRL_Cell::RandCell(10, 400));
      s6out = ranvec;
      const bool b1 = Delone::Reduce(S6(ranvec), m6, s6out, 0.0);
      if (!b1) continue;
      const D7 v7_6x = d.GetProjector() * D7(s6out);
      s6out = v7_6x;

      fn(d.GetProjector() * D7(s6out), delta);
   }
}

std::string D7_ClassifySubboundaries::ClassifyCase(const std::string& name, const D7& v, const double delta) {
   if (name == "Case1" || name == "Case 1") return "Case 1"; // cases 1,2,3 don't have subboundaries to determine
   if (name == "Case2" || name == "Case 2") return "Case 2";
   if (name == "Case3" || name == "Case 3") return "Case 3";
   if (name == "Case4" || name == "Case 4") return ClassifyCase4(v, delta);
   if (name == "Case5" || name == "Case 5") return ClassifyCase5(v, delta);
   if (name == "Case6" || name == "Case 6") return ClassifyCase6(v, delta);
   if (name == "Case7" || name == "Case 7") return ClassifyCase7(v, delta);
   if (name == "Case8" || name == "Case 8") return ClassifyCase8(v, delta);
   if (name == "Case9" || name == "Case 9") return ClassifyCase9(v, delta);
   return "";
}

void D7_ClassifySubboundaries::TestClassifyFunctions(void) {
   const D7_Boundary c4 = D7_BoundaryList::Case4();
   const D7_Boundary c5 = D7_BoundaryList::Case5();
   const D7_Boundary c6 = D7_BoundaryList::Case6();
   const D7_Boundary c7 = D7_BoundaryList::Case7();
   const D7_Boundary c8 = D7_BoundaryList::Case8();
   const D7_Boundary c9 = D7_BoundaryList::Case9();

   const double delta = 0.0;
   TestClassify(1000, c4, &D7_ClassifySubboundaries::ClassifyCase4, delta);
   TestClassify(1000, c5, &D7_ClassifySubboundaries::ClassifyCase5, delta);
   TestClassify(1000, c6, &D7_ClassifySubboundaries::ClassifyCase6, delta);
   TestClassify(1000, c7, &D7_ClassifySubboundaries::ClassifyCase7, delta);
   TestClassify(1000, c8, &D7_ClassifySubboundaries::ClassifyCase8, delta);
   TestClassify(1000, c9, &D7_ClassifySubboundaries::ClassifyCase9, delta);

   std::map<std::string, int>::iterator itclassify;
   for (itclassify = storeClassify.begin(); itclassify != storeClassify.end(); ++itclassify) {
      std::cout << (*itclassify).first << "      " << (*itclassify).second << std::endl;
   }

   srClassify.ShowResults();

}
