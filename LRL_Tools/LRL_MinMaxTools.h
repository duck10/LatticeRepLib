#ifndef LRL_MinMaxTools_H
#define LRL_MinMaxTools_H

template<typename T>
T maxNC(const T& t1, const T& t2) {
   return (std::max(t1, t2));
}
;
template<typename T>
T maxNC(const T& t1, const T& t2, const T& t3) {
   const T t4 = std::max(t2, t3);
   return (std::max(t1, t4));
}
;
template<typename T>
T maxNC(const T& t1, const T& t2, const T& t3, const T& t4) {
   return (std::max(t1, maxNC(t2, t3, t4)));
}
;
template<typename T>
T maxNC(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5) {
   return (std::max(t1, maxNC(t2, t3, t4, t5)));
}
;
template<typename T>
T maxNC(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5,
   const T& t6) {
   return (std::max(t1, maxNC(t2, t3, t4, t5, t6)));
}
;
template<typename T>
T maxNC(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5,
   const T& t6, const T& t7) {
   return (std::max(t1, maxNC(t2, t3, t4, t5, t6, t7)));
}
;

template<typename T>
T minNC(const T& t1, const T& t2) {
   return (std::min(t1, t2));
}
;
template<typename T>
T minNC(const T& t1, const T& t2, const T& t3) {
   return (std::min(t1, std::min(t2, t3)));
}
;
template<typename T>
T minNC(const T& t1, const T& t2, const T& t3, const T& t4) {
   return (std::min(t1, minNC(t2, t3, t4)));
}
;
template<typename T>
T minNC(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5) {
   return (std::min(t1, minNC(t2, t3, t4, t5)));
}
;
template<typename T>
T minNC(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5,
   const T& t6) {
   return (std::min(t1, minNC(t2, t3, t4, t5, t6)));
}
;
template<typename T>
T minNC(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5,
   const T& t6, const T& t7) {
   return (std::min(t1, minNC(t2, t3, t4, t5, t6, t7)));
}
;

#endif // LRL_MinMaxTools_H

