#ifndef DISTANCE_H
#define DISTANCE_H

#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "G6.h"
#include "NCDist.h"
#include "R9Dist.h"
#include "S6.h"
#include "V7Dist.h"
#include "Selling.h"
#include <string>

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
   std::string getColor() const override { return "#0000FF"; }  // Blue
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
   std::string getColor() const override { return "#FF0000"; }  // Red
};

// Distance_V7Dist.h
class V7Distance : public Distance {
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
      return "#0000FF";  // Blue
   }
};

// Distance_R9Dist.h
class R9Distance : public Distance {
public:
   double dist(const S6& s1, const S6& s2) const override {
      return R9Dist(s1.data(), s2.data());
   }

   double dist(const G6& g1, const G6& g2) const override {
      return R9Dist(S6(g1).data(), S6(g2).data());
   }

   std::string getName() const override {
      return "R9";
   }

   std::string getColor() const override {
      return "#FF00FF";  // Magenta
   }
};
#endif // DISTANCE_H
