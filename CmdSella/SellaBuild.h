#ifndef SELLABUILD_H
#define SELLABUILD_H
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "EnhancedS6.h"
#include "VCPVector.h"
#include "LabeledDeloneTypeMatrices.h"
#include "LabeledSellaMatrices.h"
#include "MatS6.h"
#include "StoreResults.h"
class SellaBuild {
public:
   SellaBuild();
   std::vector<LabeledDeloneTypeMatrices> Build(const bool printMatrices = false,
      const std::string& seedBoundariesMode = "");

   // SeedBoundaryGaps -- for every (parent, child) pair TestDeloneParentage
   // confirms as genuinely nested, seeds xfmap[parent] with representatives
   // reachable by crossing child's own zero-boundaries
   // (ExpandAcrossBoundaryZeros). mode == "USE" actually adds them; any
   // other value (e.g. "PRINT") reports what would be added without
   // modifying xfmap. Called from Build(), between
   // ProcessItemStoreToVectorMap() (raw samples collected) and
   // ProcessVectorMapToPerpsAndProjectors() (samples turned into deduped
   // projectors) -- see that call site's comment for why there, not
   // elsewhere.
   void SeedBoundaryGaps(const std::string& mode);
   double TestOneType(const std::string& label, const EnhancedS6& s6, const std::vector<MatS6>& vm);
   void TestAllTypes(const EnhancedS6& s6);
   void TestPerps();
   void ShowIndexResults() const;
private:
   std::map<std::string, std::vector<EnhancedS6>> xfmap;
   bool FindDuplicate(const std::vector<EnhancedS6>& out, const EnhancedS6& s6);
   std::vector<LabeledSellaMatrices> projectors;
   std::vector<LabeledSellaMatrices> perps;
   StoreResults<std::string, EnhancedS6> store;
   StoreResults<std::string, Transformations> xfStore;
   static std::vector<std::pair<std::string, MatS6>> vDeloneTypes;
   StoreResults<std::string, std::string> indexstore;
   VCPVector vcpVector;
public:
   std::vector<size_t> DetermineToProcess(const std::set<size_t>& exclusions,
      const EnhancedS6& zeros);
   std::vector<size_t> DetermineToProcess(const EnhancedS6& zeros,
      const std::set<size_t>& exclusions) {
      return DetermineToProcess(exclusions, zeros);
   }
   void ProcessItemStoreToVectorMap();
   void StoreAllReflections(const std::string& label, const EnhancedS6& s);
   void StoreAllReflections(const std::string& label, const std::set<EnhancedS6>& s);
   static std::set<EnhancedS6> GenerateAllReflections(const EnhancedS6& s);
   static std::set<EnhancedS6> GenerateAllReflections(const std::set<EnhancedS6>& s);

   // NEW: Iterative boundary expansion - replaces OneBound, ProcessTwoZeros, ProcessThreeZeros
   std::set<EnhancedS6> ExpandBoundary(const EnhancedS6& s6, const size_t depth);

   // ApplyVCPSequence -- current replacement for OneBound/ProcessTwoZeros/
   // ProcessThreeZeros (see ApplyVCPSequence2.cpp for the full rationale).
   // Recurses through VCP reflections at each currently-active zero
   // position, re-finding zero positions at every level rather than using
   // stale slot indices from the parent point. `parent` is the point one
   // level up the recursion, passed so the single immediately-reversing
   // VCP (each VCP is an involution) can be skipped without separate cycle
   // tracking. Declaration was missing here, which is why
   // ApplyVCPSequence2.cpp's out-of-line definition failed to bind to
   // class scope and StoreAllReflections/FindS6Zeros/vcpVector inside its
   // body were (incorrectly) looked up at global scope instead.
   void ApplyVCPSequence(const std::string& label, const EnhancedS6& s6, const EnhancedS6& parent);

   // OLD methods - now commented out in .cpp but kept in header for compatibility
   void OneBound(const std::string& label, const EnhancedS6& s1, const size_t zeroBound);
   void ProcessZeros(const std::string& label, const EnhancedS6& s6);
   void Expand(const std::string& label, const MatS6& fromCanon);
   void Expand2(const std::string& label, const MatS6& fromCanon);
   void ProcessTwoZeros(const std::string& label, const EnhancedS6& s6);
   void ProcessThreeZeros(const std::string& label, const EnhancedS6& s6);

   std::set<EnhancedS6> BoundAndRefl(const size_t n, const EnhancedS6& s);
   static bool FindDuplicate(const std::vector<Transformations>& out, const EnhancedS6& s6);
   static EnhancedS6 MakeSampleType(const MatS6& m);
   static std::vector<size_t> FindS6Zeros(const EnhancedS6& s);
   static std::vector<size_t> FindNearS6Zeros(const EnhancedS6& s);
   std::set<EnhancedS6> Xpand1(const std::string& label, const std::set<EnhancedS6>& vs, const MatS6& toCanon);
   std::set<EnhancedS6> Xpand2(const std::string& label, const std::set<EnhancedS6>& vs, const MatS6& toCanon);
   std::set<EnhancedS6> Xpand_A(const std::string& label, const std::set<EnhancedS6>& vs, const MatS6& toCanon);
   std::set<EnhancedS6> Xpand1(const std::string& label, const EnhancedS6& vs, const MatS6& toCanon);
   std::set<EnhancedS6> Xpand2(const std::string& label, const EnhancedS6& vs, const MatS6& toCanon);
   std::set<EnhancedS6> Xpand_A(const std::string& label, const EnhancedS6& vs, const MatS6& toCanon);
   //static EnhancedS6 Rounder(const EnhancedS6& s6);
   //static std::set<EnhancedS6> Rounder(const std::set<EnhancedS6>& ss);
   static EnhancedS6 SetPos(const size_t n, const EnhancedS6& s6);
   static std::set<EnhancedS6> SetPos(const size_t n, const std::set<EnhancedS6>& s6set);
   static EnhancedS6 SetPos(const size_t n1, const size_t n2, const EnhancedS6& s6);
   // Built from MatS6::GetReflections() -- the real, complete, generative
   // 24-element reflection group (confirmed elsewhere this session:
   // next_permutation over the 4 Selling vectors a,b,c,d). Previously a
   // hand-typed array of lambdas, only 7 of 24 entries actually filled in
   // (Reflections 1-6 and 24; 7-23 were left as an unfinished "continue
   // with the remaining 18" placeholder comment) -- every type's library,
   // built through GenerateAllReflections/StoreAllReflections, was
   // silently using a 7-element orbit instead of the full 24. std::function
   // rather than a raw function pointer because each entry now captures
   // its own MatS6, which a plain function pointer can't hold.
   static std::vector<std::function<EnhancedS6(const EnhancedS6&)>> reflectionFunctions;
};
#endif // SELLABUILD_H
