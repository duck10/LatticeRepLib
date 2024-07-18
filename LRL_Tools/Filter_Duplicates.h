#ifndef FILTER_DUPLICATES_H
#define FILTER_DUPLICATES_H

#include <iostream>
#include <cstddef>
#include <set>
#include <vector>

template<typename T>
static std::vector<T> FilterRemoveDups(const std::vector<T>& vt) {
   std::set<T> st;
   std::vector<T> out;
   st.insert(vt.begin(), vt.end()); 
   out.insert(out.end(), st.begin(), st.end());
   return out;
}

template<typename T>
static std::vector<T> EliminateDuplicates(const std::vector<T>& vt, const double delta = 1.0E-3) {
   if (vt.empty()) return {};
   if (delta == 0.0) return FilterRemoveDups(vt);

   std::vector<T> out;
   out.reserve(vt.size());  // Optimize for performance

   for (const auto& item : vt) {
      bool isDuplicate = false;
      for (const auto& existing : out) {
         const double datanorm = (item - existing).norm();
         if (datanorm < delta) {
            isDuplicate = true;
            break;
         }
      }
      if (!isDuplicate) {
         out.emplace_back(item);
      }
   }

   return out;
}

#endif // FILTER_DUPLICATES_H
