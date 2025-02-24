#ifndef STRING_MATCHER_H
#define STRING_MATCHER_H

#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include "Theta.h"
#include "TNear.h"

// Forward declarations
struct TreeStorage;
struct VectorStorage;
template<typename StorageType> class StringMatcherImpl;
class StringMatcher;

// Common string normalization function
inline std::string normalizeString(std::string s) {
   std::transform(s.begin(), s.end(), s.begin(),
      [](unsigned char c) { return std::toupper(c); });
   s.erase(std::remove_if(s.begin(), s.end(),
      [](unsigned char c) { return !std::isalnum(c); }), s.end());
   return s;
}

// Storage types
struct VectorStorage {
   std::vector<std::string> strings;

   void add(const std::string& s) {
      strings.push_back(s);
   }

   bool empty() const {
      return strings.empty();
   }

   void clear() {
      strings.clear();
   }
};

struct TreeStorage {
   // Helper class for CNearTree
   class MatchableString {
   public:
      std::string name;

      MatchableString() : name("") {}
      explicit MatchableString(const std::string& n) : name(n) {}

      MatchableString operator-(const MatchableString& other) const {
         MatchableString result;
         result.name = name + "\n" + other.name;
         return result;
      }

      double norm() const {
         size_t delim = name.find('\n');
         if (delim != std::string::npos) {
            std::string s1 = name.substr(0, delim);
            std::string s2 = name.substr(delim + 1);
            ThetaMatch<std::string> localMatcher;
            return localMatcher(s1, s2);
         }
         return 0.0;
      }
   };

   CNearTree<MatchableString> tree;

   void add(const std::string& s) {
      tree.insert(MatchableString(s));
   }

   bool empty() const {
      return tree.empty();
   }

   void clear() {
      tree.clear();
   }
};

// StringMatcher interface
class StringMatcherInterface {
public:
   virtual ~StringMatcherInterface() = default;
   virtual double getTheta(const std::string& s1, const std::string& s2) const = 0;
   virtual void addString(const std::string& s) = 0;
   virtual bool findBestMatch(const std::string& query, std::string& result) const = 0;
   virtual double getThreshold() const = 0;
   virtual void setThreshold(double d) = 0;
   virtual bool empty() const = 0;
   virtual void clear() = 0;
};

// StringMatcher implementation with parameterized storage type
template<typename StorageType>
class StringMatcherImpl : public StringMatcherInterface {
private:
   ThetaMatch<std::string> matcher;
   double matchThreshold;
   StorageType storage;

public:
   explicit StringMatcherImpl(const double threshold = 0.5)
      : matchThreshold(threshold) {}

   double getTheta(const std::string& s1, const std::string& s2) const override {
      return matcher(normalizeString(s1), normalizeString(s2));
   }

   void addString(const std::string& s) override {
      storage.add(normalizeString(s));
   }

   // Declaration only - implementation will be specialized
   bool findBestMatch(const std::string& query, std::string& result) const override;

   double getThreshold() const override {
      return matchThreshold;
   }

   void setThreshold(double d) override {
      matchThreshold = d;
   }

   bool empty() const override {
      return storage.empty();
   }

   void clear() override {
      storage.clear();
   }
};

// Specialization for vector-based implementation (inline to avoid multiple definition errors)
template<>
inline bool StringMatcherImpl<VectorStorage>::findBestMatch(const std::string& query, std::string& result) const {
   double bestTheta = matchThreshold;
   bool found = false;
   std::string normalizedQuery = normalizeString(query);

   for (const auto& s : storage.strings) {
      double theta = getTheta(normalizedQuery, s);
      if (theta <= bestTheta) {
         bestTheta = theta;
         result = s;
         found = true;
      }
   }
   return found;
}

// Specialization for tree-based implementation (inline to avoid multiple definition errors)
template<>
inline bool StringMatcherImpl<TreeStorage>::findBestMatch(const std::string& query, std::string& result) const {
   TreeStorage::MatchableString closest;
   TreeStorage::MatchableString queryObj(normalizeString(query));
   if (storage.tree.NearestNeighbor(matchThreshold, closest, queryObj)) {
      result = closest.name;
      return true;
   }
   return false;
}

// Type aliases for template specializations
using VectorStringMatcher = StringMatcherImpl<VectorStorage>;
using TreeStringMatcher = StringMatcherImpl<TreeStorage>;

// Original non-template StringMatcher class (for backward compatibility)
class StringMatcher {
private:
   std::unique_ptr<StringMatcherInterface> impl;

public:
   explicit StringMatcher(const double threshold = 0.5, bool useTree = false) {
      if (useTree) {
         impl = std::make_unique<StringMatcherImpl<TreeStorage>>(threshold);
      }
      else {
         impl = std::make_unique<StringMatcherImpl<VectorStorage>>(threshold);
      }
   }

   double getTheta(const std::string& s1, const std::string& s2) const {
      return impl->getTheta(s1, s2);
   }

   void addString(const std::string& s) {
      impl->addString(s);
   }

   bool findBestMatch(const std::string& query, std::string& result) const {
      return impl->findBestMatch(query, result);
   }

   double getThreshold() const {
      return impl->getThreshold();
   }

   void setThreshold(const double d) {
      impl->setThreshold(d);
   }

   bool empty() const {
      return impl->empty();
   }

   void clear() {
      impl->clear();
   }
};

#endif // STRING_MATCHER_H

