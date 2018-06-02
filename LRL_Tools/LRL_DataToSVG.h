#ifndef LRL_DATATOSVG_H
#define LRL_DATATOSVG_H

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "StoreResults.h"

#include "LRL_DataStreamToString.h"

#pragma warning( disable : 4505) // unreferenced local function has been removed

static std::string LRL_DataToSVG( const char c) {
   return LRL_DataStreamToString("",  std::string(1, c));
}

template<typename T>
std::string LRL_DataToSVG( const T& t) {
   std::ostringstream ostr;
   ostr << t;
   return LRL_DataStreamToString("", ostr.str());
}

template <typename TKEY, typename TDATA>
std::string LRL_DataToSVG( const StoreResults<TKEY, TDATA>& sr) {
   std::ostringstream os;
   os << "Contents of StoreResults" << std::endl;
   os << "m_title          " << sr.GetTitle() << std::endl;
   os << "m_nmax           " << sr.GetNmax() << std::endl;
   os << "m_tree.size()    " << sr.GetTreeSize() << std::endl;
   os << "m_data.size()    " << sr.GetDataSize() << std::endl;
   os << "m_itemSeparator  " << sr.GetItemSeparator() << std::endl;
   os << "m_valueSeparator " << sr.GetValueSeparator() << std::endl;
   return LRL_DataStreamToString("",  os.str());
}

template<typename T1, typename T2>
std::string LRL_DataToSVG( const T1& t1, const T2& t2) {
   return LRL_DataStreamToString("", t1, t2);
}

template<typename T1, typename T2, typename T3>
std::string LRL_DataToSVG( const T1& t1, const T2& t2, const T3& t3) {
   return LRL_DataStreamToString("", t1, t2, t3);
}

template<typename T1, typename T2, typename T3, typename T4>
std::string LRL_DataToSVG( const T1& t1, const T2& t2, const T3& t3, const T4& t4) {
   return LRL_DataStreamToString("", t1, t2, t3, t4);
}

template<typename T1, typename T2, typename T3, typename T4, typename T5>
std::string LRL_DataToSVG( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
   const T5& t5) {
   return LRL_DataStreamToString("", t1, t2, t3, t4, t5);
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
  typename T6>
   std::string LRL_DataToSVG( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
      const T5& t5, const T6& t6) {
   return LRL_DataStreamToString("", t1, t2, t3, t4, t5, t6);
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
  typename T6, typename T7>
   std::string LRL_DataToSVG( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
      const T5& t5, const T6& t6, const T7& t7) {
   return LRL_DataStreamToString("", t1, t2, t3, t4, t5, t6, t7);
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
  typename T6, typename T7, typename T8>
   std::string LRL_DataToSVG( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
      const T5& t5, const T6& t6, const T7& t7, const T8& t8) {
   return LRL_DataStreamToString("", t1, t2, t3, t4, t5, t6, t7, t8);
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
  typename T6, typename T7, typename T8, typename T9>
   std::string LRL_DataToSVG( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
      const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9) {
   return LRL_DataStreamToString("", t1, t2, t3, t4, t5, t6, t7, t8, t9);
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
  typename T6, typename T7, typename T8, typename T9, typename T10>
   std::string LRL_DataToSVG( const T1& t1, const T2& t2, const T3& t3, const T4& t4,
      const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10) {
   return LRL_DataStreamToString("", t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
}

template<typename T>
static std::string LRL_DataToSVG( const std::vector<T>& t) {
   std::string s;
   const unsigned long n = (unsigned long)t.size();
   for (unsigned long i = 0; i < n; ++i) {
      s += t[i], "\t";
   }
   return LRL_DataStreamToString("", s);
}

template<typename T>
std::string LRL_DataToSVG_ToQuotedString(const T& t) {
   return LRL_DataStreamToString("", "\"", t, "\"");
}

#endif  // LRL_DATATOSVG_H

#undef MAKE_TOSTRING

