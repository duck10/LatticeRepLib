#include "D13.h"
#include "S6.h"

#include <iostream>

const std::vector<Vector_3> D13::vertices =
{ Vector_3(0,1,0), Vector_3(1,1,0), Vector_3(1,0,0), Vector_3(1,-1,0),
  Vector_3(-1,1,1), Vector_3(0,1,1), Vector_3(1,1,1), Vector_3(1,0,1),
  Vector_3(1,-1,1), Vector_3(0,-1,1), Vector_3(-1,-1,1), Vector_3(-1,0,1),
  Vector_3(0,0,1) };
//
//std::vector<Vector_3> D13::ComputeIndices() {
//   return { Vector_3(0,1,0), Vector_3(1,1,0), Vector_3(1,0,0), Vector_3(1,-1,0),
//  Vector_3(-1,1,1), Vector_3(0,1,1), Vector_3(1,1,1), Vector_3(1,0,1),
//  Vector_3(1,-1,1), Vector_3(0,-1,1), Vector_3(-1,-1,1), Vector_3(-1,0,1),
//  Vector_3(0,0,1) };
//}
//
//std::vector<std::string> D13::GetStringIndices() {
//   if ( indices.empty()) indices = ComputeIndices();
//   for (size_t i = 0; i < indices.size(); ++i) {
//      const Vector_3& v = indices[i];
//      sIndices.push_back(LRL_ToString(v[0], ",", v[1], ",", v[2]));
//   }
//   return sIndices;
//}
//
//std::vector<Vector_3> D13::GetIndices() {
//   if (indices.empty()) indices = ComputeIndices();
//   return indices;
//}
//
//std::string D13::GetIndex(const size_t n) {
//   if (indices.empty()) indices = ComputeIndices();
//   if (sIndices.empty()) sIndices = GetStringIndices();
//   return LRL_ToString(sIndices[n]);
//}

//void D13::BuildIndices() {
//   if (!sIndices.empty()) return;
//   for (size_t i = 0; i < vertices.size(); ++i) {
//      const std::string s =
//         LRL_ToString(vertices[i][0]) + "," +
//         LRL_ToString(vertices[i][1]) + "," +
//         LRL_ToString(vertices[i][2]);
//      sIndices.push_back(s);
//   }
//}
