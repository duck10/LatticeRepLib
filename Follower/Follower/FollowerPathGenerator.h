#ifndef FOLLOWERPATHGENERATOR_H
#define FOLLOWERPATHGENERATOR_H

#include <string>
#include <vector>

#include "S6.h"

class FollowerPathGenerator {
public:
   friend std::ostream& operator<< (std::ostream&, const FollowerPathGenerator&);

   size_t size() const { return m_result.size(); }
   std::vector<std::pair<S6, S6> > GetPath() const { return m_result; }
   virtual std::string GetName() const = 0;
   void OutputPath() const;
   std::vector<S6> GetInput() const { return m_input; }

protected:
   std::vector<std::pair<S6, S6> > GenerateFollowerPointList(const size_t n, const S6& s1, const S6& s2);

protected:
   std::vector<std::pair<S6, S6> > m_result;
   std::vector<S6> m_input;
};

class FollowerPoint : public FollowerPathGenerator {
public:
   FollowerPoint(const size_t n, const S6& s1);
   std::string GetName() const { return "FollowerPoint"; }
};

class FollowerLine : public FollowerPathGenerator {
public:
   FollowerLine(const size_t n, const S6& s1, const S6& s2);
   std::string GetName() const { return "FollowerLine"; }
};

class FollowerChord : public FollowerPathGenerator {
public:
   FollowerChord(const size_t n, const S6& s1, const S6& s2);
   std::string GetName() const { return "FollowerChord"; }
};

class FollowerChord3 : public FollowerPathGenerator {
public:
   FollowerChord3(const size_t n, const S6& s1, const S6& s2, const S6& s3);
   std::string GetName() const { return "FollowerChord3"; }
};

class FollowerTriangle : public FollowerPathGenerator {
public:
   FollowerTriangle(const size_t n, const S6& s1, const S6& s2, const S6& s3);
   std::string GetName() const { return "FollowerTriangle"; }
};

#endif  // FOLLOWERPATHGENERATOR_H
