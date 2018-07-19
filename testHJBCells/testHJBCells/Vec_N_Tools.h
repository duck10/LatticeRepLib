#ifndef VEC_N_TOOLS_H
#define VEC_N_TOOLS_H

#include <string>
#include <vector>

namespace Vec_N_Tools {

std::vector<double> FromString(const std::string& s);

};

template<typename T>
bool CheckProjector( const T& m ) {
   const double check = (m*m.transpose( ) - m ).norm();
   return check < 1.e-12;
}

#endif
