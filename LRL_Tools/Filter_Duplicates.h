#ifndef FILTER_DUPLICATES_H
#define FILTER_DUPLICATES_H

#include <set>
#include <vector>
#include <cstddef>

template<typename T>
static std::vector<T> FilterRemoveDups(const std::vector<T>& vt) {
   std::set<T> st;
   std::vector<T> out;
   st.insert(vt.begin(), vt.end());
   out.insert(out.end(), st.begin(), st.end());
   return out;
}

template<typename T>
std::vector<T> EliminateDuplicates(const std::vector<T>& vt, const double delta= 1.0E-6) {
   if (delta == 0.0) return FilterRemoveDups(vt);

   std::vector<T> out(1, vt[0]);
   for (size_t i = 1; i < vt.size(); ++i) {
      std::vector<T> toAdd(1, vt[i]);
      for (size_t kkk = 0; kkk < out.size(); ++kkk)
      {
         const double d = (vt[i] - out[kkk]).norm();
         if (d < delta) {
            toAdd.clear();
            break;
         }
      }
      out.insert(out.end(), toAdd.begin(), toAdd.end());
   }
   return out;
}

#endif // FILTER_DUPLICATES_H
