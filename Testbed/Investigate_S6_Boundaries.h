#ifndef INVESTIGATE_S6_BOUNDARIES
#define INVESTIGATE_S6_BOUNDARIES

#include "MatS6.h"
#include "S6.h"
#include "TNear.h"

class Investigate_S6_Boundaries {
public:
   Investigate_S6_Boundaries(const double radiusOfConfusion);
   void Process(const S6& s);

   size_t size() { return m_tree.size(); }

   static std::vector<MatS6> CreateS6ReductionMatrices();
   void CreateS6ReductionMatrices24();

   template<typename T>
   CNearTree<S6> Unique(const T& tree) const {
      CNearTree<S6> newTree(m_tree);

      for (size_t i = 0; i < tree.size(); ++i) {
         if (m_tree.NearestNeighbor(tree[i], m_radiusOfConfusion) == m_tree.end()) {
            newTree.insert(tree[i]);
         }
      }
   }

private:
   const double m_radiusOfConfusion;
   CNearTree<S6> m_tree;
   const std::vector<MatS6> m_refl;
   const std::vector<MatS6> m_bounds6;
   std::vector<MatS6> m_bounds24;
};

#endif  // INVESTIGATE_S6_BOUNDARIES
