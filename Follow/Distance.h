#ifndef DISTANCE_H
#define DISTANCE_H

#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "DC7u.h"
#include "DC7uDist.h"
#include "G6.h"
//#include "NCDist_McColl.h"
#include "NCDist_McCoy_2.h"
#include "NCDist.h"
#include "P3.h"
#include "RI.h"
#include "S6.h"
#include "V7Dist.h"
#include "Selling.h"

#include <memory>
#include <string>

/*
HOW TO ADD A NEW DISTANCE TYPE
1. add the idenifier to the list of VALID_TYPES in DistanceTypesUtils
2. add a new branch in the DistanceFactory::createEnabledDistances
3. add a new class in Distance.h; NOTE: base class Distance is abstract
   You need to create explict constructors for both G6 and G6.

  That should do the job, once it is compilable.
*/

class Distance {
public:
   virtual ~Distance() = default;
   virtual double dist(const S6& s1, const S6& s2) const = 0;
   virtual double dist(const G6& g1, const G6& g2) const = 0;
   virtual std::string getName() const = 0;
   virtual std::string getColor() const = 0;
};

class CS6Distance : public Distance {
public:
   double dist(const S6& s1, const S6& s2) const override {
      S6 reduced1, reduced2;
      Selling::Reduce(s1, reduced1);
      Selling::Reduce(s2, reduced2);
      return CS6Dist(reduced1.data(), reduced2.data());
   }

   double dist(const G6& g1, const G6& g2) const override {
      S6 s1(g1), s2(g2);
      return dist(s1, s2);
   }

   std::string getName() const override { return "CS6Dist"; }
   std::string getColor() const override { return "#0066FF"; }  // Bright Blue
};

class NCDistance : public Distance {
public:
   double dist(const S6& s1, const S6& s2) const override {
      G6 reduced1, reduced2;
      Niggli::Reduce(G6(s1), reduced1);
      Niggli::Reduce(G6(s2), reduced2);
      const double dist = NCDist(reduced1.data(), reduced2.data());
      return NCDist(reduced1.data(), reduced2.data());
   }

   double dist(const G6& g1, const G6& g2) const override {
      return dist(S6(g1), S6(g2));
   }

   std::string getName() const override { return "NCDist"; }
   std::string getColor() const override { return "#FF6600"; }  // Bright Orange
};

class P3Distance : public Distance {
   template<typename T>
   double dist(const T& t1, const T& t2) {
      return (P3(t1) - P3(t2)).norm();
   }
   double dist(const S6& s1, const S6& s2) const {
      return (P3(s1) - P3(s2)).norm();
   }
   double dist(const G6& g1, const G6& g2) const {
      return (P3(g1) - P3(g2)).norm();
   }

   std::string getName() const override { return "P3"; }
   std::string getColor() const override { return "#9900FF"; }  // Bright Purple
};

class RIDistance : public Distance {
public:
   template<typename T>
   static double distRI(const T& t1, const T& t2) {
      return ((RI(t1) - RI(t2)).norm());
   }

   double dist(const S6& s1, const S6& s2) const override { return distRI(s1, s2); }
   double dist(const G6& g1, const G6& g2) const override { return distRI(g1, g2); }

   std::string getName() const override {
      return "RI";
   }

   std::string getColor() const override {
      return "#00CCFF";  // Bright Cyan
   }
};

// Distance_V7Dist.h
class V7Distance : public Distance {
   //double V7Dist(const DC7u& v1, const DC7u& v2) const {

   //   double v1abc[7], v2abc[7], v2acb[7], v2bac[7], v2bca[7], v2cab[7], v2cba[7];

   //   double distsq, distsqabc, distsqacb, distsqbac, distsqbca, distsqcab, distsqcba;;

   //   int ii;

   //   v1abc[DC7u_AA_idx] = v1[DC7u_AA_idx];

   //   v1abc[DC7u_BB_idx] = v1[DC7u_BB_idx];

   //   v1abc[DC7u_CC_idx] = v1[DC7u_CC_idx];

   //   v1abc[DC7u_MIN_BC_diagsq_idx] = v1[DC7u_MIN_BC_diagsq_idx];

   //   v1abc[DC7u_MIN_AC_diagsq_idx] = v1[DC7u_MIN_AC_diagsq_idx];

   //   v1abc[DC7u_MIN_AB_diagsq_idx] = v1[DC7u_MIN_AB_diagsq_idx];

   //   v1abc[DC7u_MIN_ABC_diagsq_idx] = v1[DC7u_MIN_ABC_diagsq_idx];



   //   v2cba[DC7u_CC_idx] = v2cab[DC7u_CC_idx] = v2bca[DC7u_BB_idx] = v2bac[DC7u_BB_idx] = v2acb[DC7u_AA_idx] = v2abc[DC7u_AA_idx] = v2[DC7u_AA_idx];

   //   v2cba[DC7u_BB_idx] = v2cab[DC7u_AA_idx] = v2bca[DC7u_CC_idx] = v2bac[DC7u_AA_idx] = v2acb[DC7u_CC_idx] = v2abc[DC7u_BB_idx] = v2[DC7u_BB_idx];

   //   v2cba[DC7u_CC_idx] = v2cab[DC7u_BB_idx] = v2bca[DC7u_AA_idx] = v2bac[DC7u_CC_idx] = v2acb[DC7u_BB_idx] = v2abc[DC7u_CC_idx] = v2[DC7u_CC_idx];

   //   v2cba[DC7u_MIN_AB_diagsq_idx] = v2cab[DC7u_MIN_AB_diagsq_idx] = v2bca[DC7u_MIN_AC_diagsq_idx] = v2bac[DC7u_MIN_AC_diagsq_idx]

   //      = v2acb[DC7u_MIN_BC_diagsq_idx] = v2abc[DC7u_MIN_BC_diagsq_idx] = v2[DC7u_MIN_BC_diagsq_idx];

   //      v2cba[DC7u_MIN_AC_diagsq_idx] = v2cab[DC7u_MIN_BC_diagsq_idx] = v2bca[DC7u_MIN_AB_diagsq_idx] = v2bac[DC7u_MIN_BC_diagsq_idx]

   //         = v2acb[DC7u_MIN_AB_diagsq_idx] = v2abc[DC7u_MIN_AC_diagsq_idx] = v2[DC7u_MIN_AC_diagsq_idx];

   //         v2cba[DC7u_MIN_BC_diagsq_idx] = v2cab[DC7u_MIN_AC_diagsq_idx] = v2bca[DC7u_MIN_BC_diagsq_idx] = v2bac[DC7u_MIN_AB_diagsq_idx]

   //            = v2acb[DC7u_MIN_AC_diagsq_idx] = v2abc[DC7u_MIN_AB_diagsq_idx] = v2[DC7u_MIN_AB_diagsq_idx];

   //            v2cba[DC7u_MIN_ABC_diagsq_idx] = v2cab[DC7u_MIN_ABC_diagsq_idx] = v2bca[DC7u_MIN_ABC_diagsq_idx] = v2bac[DC7u_MIN_ABC_diagsq_idx]

   //               = v2acb[DC7u_MIN_ABC_diagsq_idx] = v2abc[DC7u_MIN_ABC_diagsq_idx] = v2[DC7u_MIN_ABC_diagsq_idx];



   //               distsqabc = distsqacb = distsqbac = distsqbca = distsqcab = distsqcba = 0.;

   //               for (ii = 0; ii < 7; ii++) {

   //                  distsqabc += (v1abc - v2abc) * (v1abc - v2abc);

   //                  distsqacb += (v1abc - v2acb) * (v1abc - v2acb);

   //                  distsqbac += (v1abc - v2bac) * (v1abc - v2bac);

   //                  distsqbca += (v1abc - v2bca) * (v1abc - v2bca);

   //                  distsqcab += (v1abc - v2cab) * (v1abc - v2cab);

   //                  distsqcba += (v1abc - v2cba) * (v1abc - v2cba);

   //               }

   //               distsq = distsqabc;

   //               if (distsqacb < distsq) { distsq = distsqacb; }

   //               if (distsqbac < distsq) { distsq = distsqbac; }

   //               if (distsqbca < distsq) { distsq = distsqbca; }

   //               if (distsqcab < distsq) { distsq = distsqcab; }

   //               if (distsqcba < distsq) { distsq = distsqcba; }



   //               return sqrt(distsq);

   //}

public:
   double dist(const S6& s1, const S6& s2) const override {
      return V7Dist(s1, s2);
   }

   double dist(const G6& g1, const G6& g2) const override {
      return V7Dist(S6(g1), S6(g2));
   }

   std::string getName() const override {
      return "V7";
   }

   std::string getColor() const override {
      return "#FFCC00";  // Bright Yellow
   }
};

// Distance_DC7U
class DC7uDistance : public Distance {
public:
   double dist(const S6& s1, const S6& s2) const override {
      return DC7uPermDistanceBetween(s1, s2);
   }

   double dist(const G6& g1, const G6& g2) const override {
      return DistanceBetween(S6(g1), S6(g2));
   }

   std::string getName() const override {
      return "DC7u";
   }

   std::string getColor() const override {
      return "#FF0099";  // Bright Magenta/Pink
   }
};

class PHDistance : public Distance {
public:
   double dist(const S6& s1, const S6& s2) const override {
      G6 reduced1, reduced2;
      Niggli::Reduce(s1, reduced1);
      Niggli::Reduce(s2, reduced2);
      return phasertng::NCDist(reduced1.data(), reduced2.data());
   }

   double dist(const G6& g1, const G6& g2) const override {
      G6 reduced1, reduced2;
      Niggli::Reduce(g1, reduced1);
      Niggli::Reduce(g2, reduced2);
      return phasertng::NCDist(reduced1.data(), reduced2.data());
   }

   std::string getName() const override { return "PHDist"; }
   std::string getColor() const override { return "#0066FF"; }  // Bright Blue
};


#endif // DISTANCE_H