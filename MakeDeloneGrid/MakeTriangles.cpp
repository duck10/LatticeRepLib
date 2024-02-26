#include "MakeTriangles.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>
#include <utility>

std::string Slashes::GetSlashes(const std::string& character) {
   std::string slashes{ "from GetSlashes" };
   std::string bare = character.substr(1, 3) + character.substr(5, 3);
   int r_count = std::count(bare.begin(), bare.end(), 'r');
   int s_count = std::count(bare.begin(), bare.end(), 's');
   int t_count = std::count(bare.begin(), bare.end(), 't');
   int zero_count = std::count(bare.begin(), bare.end(), '0');



   //enum class eSlashCount { none, one, two, three, zero };
   //enum class eTetrahedronSide { P, Q, R, S, T, U };
   //eTetrahedronSide esides;
   //eSlashCount ecounts;
   std::queue<eSlashCount> slashCountTypes;
   slashCountTypes.emplace(eSlashCount::one);
   slashCountTypes.emplace(eSlashCount::two);
   slashCountTypes.emplace(eSlashCount::three);

   std::vector<eSlashCount> out(6);
   std::vector<std::string> outchar(6);

   for (auto& side : {  // looking for the zeros
      eTetrahedronSide::P, eTetrahedronSide::Q, eTetrahedronSide::R,
      eTetrahedronSide::S, eTetrahedronSide::T, eTetrahedronSide::U })
   {
      int index = static_cast<int>(side);
      if (bare[index] == '0') {
         out[index] = eSlashCount::zero;
         outchar[index] = '0';
      }
   }

   for (auto& side : {
      eTetrahedronSide::P, eTetrahedronSide::Q, eTetrahedronSide::R,
      eTetrahedronSide::S, eTetrahedronSide::T, eTetrahedronSide::U })
   {
      const int index = static_cast<int>(side);

      if (bare[index] == 'r' && r_count < 2) {
         out[index] = eSlashCount::none;
         outchar[index] = ' ';
      }
      else if (bare[index] == 'r' && r_count >1) {
         const eSlashCount forType = slashCountTypes.front();
         for ( size_t i=0; i<6; ++i ) 
         {
            if ( bare[i] == 'r')
            {
               out[i] = forType;
               outchar[index] = '1';
            }
         }
      }
   }
   if (r_count > 1) {
      slashCountTypes.pop();
   }

   for (auto& side : {
      eTetrahedronSide::P, eTetrahedronSide::Q, eTetrahedronSide::R,
      eTetrahedronSide::S, eTetrahedronSide::T, eTetrahedronSide::U })
   {
      const int index = static_cast<int>(side);
      if (bare[index] == 's' && s_count < 2) {
         out[index] = eSlashCount::none;
         outchar[index] = ' ';
      }
      else if (bare[index] == 's' && s_count > 1) {
         const eSlashCount forType = slashCountTypes.front();
         for (size_t i = 0; i < 6; ++i)
         {
            if (bare[i] == 's')
            {
               out[i] = forType;
               outchar[index] = '2';
            }
         }
      }
   }
   if (s_count > 1) {
      slashCountTypes.pop();
   }

   for (auto& side : {
      eTetrahedronSide::P, eTetrahedronSide::Q, eTetrahedronSide::R,
      eTetrahedronSide::S, eTetrahedronSide::T, eTetrahedronSide::U })
   {
      const int index = static_cast<int>(side);
      if (bare[index] == 't' && t_count < 2) {
         out[index] = eSlashCount::none;
         outchar[index] = ' ';
      }
      else if (bare[index] == 't' && t_count > 1) {
         const eSlashCount forType = slashCountTypes.front();
         for (size_t i = 0; i < 6; ++i)
         {
            if (bare[i] == 't')
            {
               out[i] = forType;
               outchar[index] = '2';
            }
         }
      }
   }

   if (slashMap.empty()) {
      BuildSlashMap();
   }

   std::string outString;
   for (const auto& side : {
      eTetrahedronSide::P, eTetrahedronSide::Q, eTetrahedronSide::R,
      eTetrahedronSide::S, eTetrahedronSide::T, eTetrahedronSide::U })
   {
      const int index = static_cast<int>(side);
      outString += slashMap[std::make_pair(side, out[index])];
   }

   return outString;

   //return std::string(outchar[0] + outchar[1] + outchar[2] + outchar[3] + outchar[4] + outchar[5]);
}

std::string  Slashes::CharacterToSlashes(const std::string& crtr) {
   std::string out;
   std::string bare = crtr.substr(1, 3) + crtr.substr(5, 3);
   // there 6 places to put eSlashCount
   // 
   // there are 5 states: zero, nothing, and 1, 2, or 3 eSlashCount

   int r_count = std::count(bare.begin(), bare.end(), 'r');
   int s_count = std::count(bare.begin(), bare.end(), 's');
   int t_count = std::count(bare.begin(), bare.end(), 't');
   int zero_count = std::count(bare.begin(), bare.end(), '0');

   std::vector<eSlashCount> slash(6);

   //// the posiions in the crtr list are: P,Q,R,S,T,Y OR s1,s2,s3,s4,s5,s6
   //std::map<std::pair< eTetrahedronSide, eSlashCount>, std::string > slashMap;
   //enum class eSlashCount { none, one, two, three, zero };


   for ( auto& side : {
      eTetrahedronSide::P, eTetrahedronSide::Q, eTetrahedronSide::R,
      eTetrahedronSide::S, eTetrahedronSide::T, eTetrahedronSide::U })
   {
      const int index = static_cast<int>(side);
      if (std::string("uvw").find(bare[index]) !=std::string::npos) continue;
      const int charCount = std::count(bare.begin(), bare.end(), bare[index]);

      if (slashMap.empty()) {
         BuildSlashMap();
      }
      out += slashMap[std::make_pair(side, eSlashCount::one)];
   }

   return out;
}

std::string Slashes::ConvertSlashToSVG(const eTetrahedronSide& side, const std::pair<Vector_3, Vector_3>& one) {
   const Vector_3 shift = SideLabelToShiftToMidpoint(side);

   const Vector_3 p1 = MatForSide(side)*one.first + shift;
   const Vector_3 p2 = MatForSide(side)*one.second + shift;

   std::ostringstream o;
   o
      << "<line id=\"oneSlash\" x1=\""
      << p1[0]
      << "\" y1=\""
      << p1[1]
      << "\" x2=\""
      << p2[0]
      << "\" y2=\""
      << p2[1]
      << "\" style=\"stroke:rgb(0,0,0);stroke-width:3\"/>\n";
   return o.str();
}

Vector_3 Slashes::SideLabelToShiftToMidpoint(const eTetrahedronSide& side) {

   Triangles tr;
   std::string out;
   Vector_3 shift;
   if (side == eTetrahedronSide::P) {
      shift = tr.midBC;
   }
   else if (side == eTetrahedronSide::Q) {
      shift = tr.midAB;
   }
   else if (side == eTetrahedronSide::R) {
      shift = tr.midAC;
   }
   else if (side == eTetrahedronSide::S) {
      shift = tr.midAD;
   }
   else if (side == eTetrahedronSide::T) {
      shift = tr.midCD;
   }
   else if (side == eTetrahedronSide::U) {
      shift = tr.midBD;
   }
   return Triangles::Rescale(shift);
}

std::string Slashes::ConvertZeroToSVG(const eTetrahedronSide& side, const std::pair<Vector_3, Vector_3>& one) {
   const Vector_3 shift = SideLabelToShiftToMidpoint(side);

   const Vector_3 p1 = one.first + shift;
   const Vector_3 p2 = one.second + shift;
   const Vector_3 p3 = (p1 + p2) / 2.0;
   //      "<circle id=\"aP\" cx=\"405\" cy=\"800\" r=\"20\" fill=\"white\" stroke=\"black\" stroke-width=\"3\"/>\n"

   constexpr double radius = 3.0;
   std::ostringstream o;

   o <<
      "   <text id=\"cP\" x=\""
      << p3[0] - 4
      << "\" y=\""
      << p3[1] + 6
      << "\" style=\"font-family:sans-serif; font-size=:26; font-weight:bold; \">"
      << "0"
      << "</text>\n"
      << "\n";




      //<< "<circle id=\"zero\" cx=\""
      //<< p1[0] -0.1*radius
      //<< "\" cy=\""
      //<< p1[1] + 2.0*radius
      //<< "\" r=\""
      //<< radius
      //<< "\" fill=\"none\" stroke=\"black\" stroke-width=\"0.75\"/>\n";
   return o.str();
}



std::string Slashes::ConvertSlashToSVG(const eTetrahedronSide& side,
   const std::vector<std::pair<Vector_3, Vector_3> >& mult) {
   const Vector_3 shift = SideLabelToShiftToMidpoint(side);

   const size_t n = mult.size();
   const Vector_3 pa1 = MatForSide(side)*mult[0].first + shift;
   const Vector_3 pa2 = MatForSide(side)*mult[0].second + shift;
   const Vector_3 pb1 = MatForSide(side)*mult[1].first + shift;
   const Vector_3 pb2 = MatForSide(side)*mult[1].second + shift;
   Vector_3 pc1;
   Vector_3 pc2;

   if ( n == 3) 
   {
      pc1 = MatForSide(side)*mult[2].first + shift;
      pc2 = MatForSide(side)*mult[2].second + shift;
   }

   std::ostringstream o;
   o
      << "<line id=\"mult"
      << mult.size()
      << "Slash\" x1=\""
      << pa1[0]
      << "\" y1=\""
      << pa1[1]
      << "\" x2=\""
      << pa2[0]
      << "\" y2=\""
      << pa2[1]
      << "\" style=\"stroke:rgb(0,0,0);stroke-width:3\"/>\n";

   o
      << "<line id=\"mult"
      << mult.size()
      << "Slash\" x1=\""
      << pb1[0]
      << "\" y1=\""
      << pb1[1]
      << "\" x2=\""
      << pb2[0]
      << "\" y2=\""
      << pb2[1]
      << "\" style=\"stroke:rgb(0,0,0);stroke-width:3\"/>\n";

   if ( n == 3)
   {
      o
         << "<line id=\"mult"
         << mult.size()
         << "Slash\" x1=\""
         << pc1[0]
         << "\" y1=\""
         << pc1[1]
         << "\" x2=\""
         << pc2[0]
         << "\" y2=\""
         << pc2[1]
         << "\" style=\"stroke:rgb(0,0,0);stroke-width:3\"/>\n";

      o
         << "<line id=\"mult"
         << mult.size()
         << "Slash\" x1=\""
         << pb1[0]
         << "\" y1=\""
         << pb1[1]
         << "\" x2=\""
         << pb2[0]
         << "\" y2=\""
         << pb2[1]
         << "\" style=\"stroke:rgb(0,0,0);stroke-width:3\"/>\n";

      o
         << "<line id=\"mult"
         << mult.size()
         << "Slash\" x1=\""
         << pb1[0]
         << "\" y1=\""
         << pb1[1]
         << "\" x2=\""
         << pb2[0]
         << "\" y2=\""
         << pb2[1]
         << "\" style=\"stroke:rgb(0,0,0);stroke-width:3\"/>\n";
   }
   return o.str();
}

Matrix_3x3 Slashes::MatForSide(const eTetrahedronSide& side) const {
   static const double degreesPerRad = 180.0 / 4.0 / atan(1.0);
   static const Matrix_3x3 sideP = Vector_3(0, 0, 1).Rotmat(60.0/degreesPerRad);
   static const Matrix_3x3 sideQ = Vector_3(0, 0, 1).Rotmat(-60.0/degreesPerRad); //ok
   static const Matrix_3x3 sideR = Vector_3(0, 0, 1).Rotmat(0.0/degreesPerRad); //ok
   static const Matrix_3x3 sideS = Vector_3(0, 0, 1).Rotmat(-30.0/degreesPerRad); //ok
   static const Matrix_3x3 sideT = Vector_3(0, 0, 1).Rotmat(30.0/degreesPerRad); //ok
   static const Matrix_3x3 sideU = Vector_3(0, 0, 1).Rotmat(90.0/degreesPerRad); //ok
   static std::map<eTetrahedronSide, Matrix_3x3> matmap;
   matmap[eTetrahedronSide::P] = sideP;
   matmap[eTetrahedronSide::Q] = sideQ;
   matmap[eTetrahedronSide::R] = sideR;
   matmap[eTetrahedronSide::S] = sideS;
   matmap[eTetrahedronSide::T] = sideT;
   matmap[eTetrahedronSide::U] = sideU;

   return matmap[side];
}

void Slashes::BuildSlashMap() {

   // these are the coordinates of the slashes, before translations
   const Vector_3 zero;
   const std::pair<Vector_3, Vector_3>               one   = OneSlashAC(); // correctly in place at origin and scaled for 100
   const std::vector<std::pair<Vector_3, Vector_3> > two   = TwoSlashs();  // corrected
   const std::vector<std::pair<Vector_3, Vector_3> > three = ThreeSlashs();

   //for (const auto& side : {
   //   eTetrahedronSide::P, eTetrahedronSide::Q, eTetrahedronSide::R,
   //   eTetrahedronSide::S, eTetrahedronSide::T, eTetrahedronSide::U })
   //{
   //   slashMap[std::make_pair(side, eSlashCount::none)] = "";  //correct 2023-02-18
   //}

   for (const auto& side : {
      eTetrahedronSide::P, eTetrahedronSide::Q, eTetrahedronSide::R,
      eTetrahedronSide::S, eTetrahedronSide::T, eTetrahedronSide::U })
   {
      const std::string s = ConvertZeroToSVG(side, one);
      slashMap[std::make_pair(side, eSlashCount::zero)] = s;
   }

   for (const auto& side : {
      eTetrahedronSide::P, eTetrahedronSide::Q, eTetrahedronSide::R,
      eTetrahedronSide::S, eTetrahedronSide::T, eTetrahedronSide::U })
   {
      const std::string s = ConvertSlashToSVG(side, one);
      slashMap[std::make_pair(side, eSlashCount::one)] = s;
   }

   for (const auto& side : {
      eTetrahedronSide::P, eTetrahedronSide::Q, eTetrahedronSide::R,
      eTetrahedronSide::S, eTetrahedronSide::T, eTetrahedronSide::U })
   {
      const std::string s = ConvertSlashToSVG(side, two);
      slashMap[std::make_pair(side, eSlashCount::two)] = s;
   }

   for (const auto& side : {
      eTetrahedronSide::P, eTetrahedronSide::Q, eTetrahedronSide::R,
      eTetrahedronSide::S, eTetrahedronSide::T, eTetrahedronSide::U })
   {
      const std::string s = ConvertSlashToSVG(side, three);
      slashMap[std::make_pair(side, eSlashCount::three)] = s;
   }

   //for (const auto& element : slashMap) {
   //   std::cout << element.second;
   //}
}

std::pair<Vector_3, Vector_3>  Slashes::OneSlashAC() const {
   const Vector_3 slashstartR = Triangles::Rescale( Vector_3(0, slashHalfHeight, 0));
   const Vector_3 slashendR   = Triangles::Rescale(-Vector_3(0, slashHalfHeight, 0));
   return std::make_pair(slashstartR, slashendR);
}

std::vector<std::pair<Vector_3, Vector_3>> Slashes::TwoSlashs() {
   Triangles tr;

   const Vector_3 shift = Vector_3(slashShift, 0, 0);
   const Vector_3 slashstart1R = Triangles::Rescale(Vector_3(0, slashHalfHeight, 0)) - shift;
   const Vector_3 slashend1R = Triangles::Rescale(-Vector_3(0, slashHalfHeight, 0))  - shift;

   const Vector_3 slashstart2R = Triangles::Rescale(Vector_3(0, slashHalfHeight, 0)) + shift;
   const Vector_3 slashend2R   = Triangles::Rescale(-Vector_3(0, slashHalfHeight, 0)) + shift;

   return{
      std::make_pair(slashstart1R, slashend1R) , 
      std::make_pair(slashstart2R, slashend2R) };


   //vvec.emplace_back(slashstartR);
   //vvec.emplace_back(slashendR);

   //vvec = Triangles::Rescale(vvec);

   //std::ostringstream o;
   //o << "   <line id=\"oneSlash\" x1=\""
   //   << vvec[0][0]
   //   << "\" y1=\""
   //   << vvec[0][1]
   //   << "\" x2=\""
   //   << vvec[1][0]
   //   << "\" y2=\""
   //   << vvec[1][1]
   //   << "\"  style=\"stroke:rgb(0,0,0);stroke-width:"
   //   << slashWidth
   //   << "\" />\n";

   //std::cout << o.str() << std::endl;
}

std::vector<std::pair<Vector_3, Vector_3>> Slashes::ThreeSlashs() {

   const Vector_3 shift = Vector_3( slashShift + slashWidth, 0, 0);
   const Vector_3 height = Vector_3(0, Triangles::Rescale(slashHalfHeight), 0);

   const Vector_3 slashstart1R = height - shift;
   const Vector_3 slashend1R = -height - shift;

   const Vector_3 slashstart2R = height;
   const Vector_3 slashend2R = -height;

   const Vector_3 slashstart3R = height + shift;
   const Vector_3 slashend3R = -height + shift;

  return{
      std::make_pair(slashstart1R, slashend1R) ,
      std::make_pair(slashstart2R, slashend2R) ,
      std::make_pair(slashstart3R, slashend3R) };
}







Triangles::Triangles() {
   MakeTrianglePointsAndMatrices();
}

static const double rescaleFactor = 70.0;

double Triangles::Rescale(const double x) {
   return x * rescaleFactor;
}

Vector_3 Triangles::Rescale(const Vector_3& x) {
   Vector_3 out(x);
   out[1] = -out[1];
   return rescaleFactor * out;
}

std::vector<Vector_3> Triangles::Rescale(const std::vector<Vector_3>& vvec) {
   std::vector<Vector_3> out;
   for (const auto& vec : vvec) {
      out.emplace_back(Rescale(vec));
   }
   return out;
}

std::string Triangles::MakeTrianglePolyline(const std::vector<Vector_3>& vvec) {
   std::ostringstream o;
   o << "<polyline stroke=\"black\" points=\"";
   for (const auto& vec : vvec) {
      o << vec[0] << "," << vec[1] << "   ";
   }
   o << "\" style=\"fill:none; stroke-width:3 stroke-linejoin:round; stroke-linecap:round\" \n />\n";
   return o.str();
}

std::vector<Vector_3> Triangles::OneSlashAC() {
   const Vector_3 slashstartR =  Vector_3(0, slashHalfHeight, 0);
   const Vector_3 slashendR =  -Vector_3(0, slashHalfHeight, 0);

   std::vector<Vector_3> vvec;

   vvec.emplace_back(slashstartR);
   vvec.emplace_back(slashendR);

   vvec = Triangles::Rescale(vvec);

   std::ostringstream o;
   o << "   <line id=\"oneSlash\" x1=\""
      << vvec[0][0]
      << "\" y1=\""
      << vvec[0][1]
      << "\" x2=\""
      << vvec[1][0]
      << "\" y2=\""
      << vvec[1][1]
      << "\"  style=\"stroke:rgb(0,0,0);stroke-width:"
      << slashWidth
      << "\"/>\n";

   std::cout << o.str() << std::endl;

   //const Vector_3 slashstartP = matm60 * (slashstartR - midAC) + midBC;
   //const Vector_3 slashendP = matm60 * (slashendR - midAC) + midBC;
   ////const Vector_3 slashstartR = midAC + Vector_3(0, slashHalfHeight, 0);
   ////const Vector_3 slashendR = midAC - Vector_3(0, slashHalfHeight, 0);
   //vvec.clear();
   //vvec.emplace_back(slashstartP);
   //vvec.emplace_back(slashendP);

   //vvec = Triangles::Rescale(vvec);
   //o.str();
   //o << "   <line id=\"oneSlash\" x1=\""
   //   << vvec[0][0]
   //   << "\" y1=\""
   //   << vvec[0][1]
   //   << "\" x2=\""
   //   << vvec[1][0]
   //   << "\" y2=\""
   //   << vvec[1][1]
   //   << "\"  style=\"stroke:rgb(0,0,0);stroke-width:"
   //   << slashWidth
   //   << "\" />\n";

   //std::cout << o.str() << std::endl;
   return vvec;
}

std::vector<Vector_3> Triangles::TwoSlashs() {
   Triangles tr;
   const Vector_3 slashstartR = Vector_3(0, slashHalfHeight, 0);
   const Vector_3 slashendR = -Vector_3(0, slashHalfHeight, 0);

   const Vector_3 slashstart1R = Vector_3(0, slashHalfHeight, 0);
   const Vector_3 slashend1R = -Vector_3(0, slashHalfHeight, 0);

   const Vector_3 slashstart2R = Vector_3(0, slashHalfHeight, 0);
   const Vector_3 slashend2R = -Vector_3(0, slashHalfHeight, 0);



   std::vector<Vector_3> vvec;
   vvec.emplace_back(slashstartR);
   vvec.emplace_back(slashendR);

   vvec = Triangles::Rescale(vvec);

   std::ostringstream o;
   o << "   <line id=\"twoSlashes\" x1=\""
      << vvec[0][0]
      << "\" y1=\""
      << vvec[0][1]
      << "\" x2=\""
      << vvec[1][0]
      << "\" y2=\""
      << vvec[1][1]
      << "\"  style=\"stroke:rgb(0,0,0);stroke-width:"
      << slashWidth
      << "\"/>\n";

   std::cout << o.str() << std::endl;
   return std::vector<Vector_3>();;
}

std::vector<Vector_3> Triangles::ThreeSlashs() {
   throw;
   return std::vector<Vector_3>();;
}

void Triangles::MakeTrianglePointsAndMatrices() {

   mat30 = Vector_3(0, 0, 1).Rotmat(30.0 / 180.0 * (4.0 * atan(1.0)));
   mat60 = Vector_3(0, 0, 1).Rotmat(60.0 / 180.0 * (4.0 * atan(1.0)));
   mat90 = Vector_3(0, 0, 1).Rotmat(90.0 / 180.0 * (4.0 * atan(1.0)));
   matm30 = Vector_3(0, 0, 1).Rotmat(-30.0 / 180.0 * (4.0 * atan(1.0)));
   matm60 = Vector_3(0, 0, 1).Rotmat(-60.0 / 180.0 * (4.0 * atan(1.0)));

   A = Vector_3(0, 0, 0);
   C = Vector_3(1, 0, 0);
   B = Vector_3(mat60 * C);
   D = Vector_3((A + B + C) / 3.0);

   midAB = Vector_3((A + B) / 2.0);
   midAC = Vector_3((A + C) / 2.0);
   midBC = Vector_3((B + C) / 2.0);

   midAD = Vector_3((A + D) / 2.0);
   midBD = Vector_3((B + D) / 2.0);
   midCD = Vector_3((C + D) / 2.0);
}


void Triangles::MakeSlashes() {

   //const Vector_3 slashstartR = midAC + Vector_3(0, slashHalfHeight, 0);
   //const Vector_3 slashendR = midAC - Vector_3(0, slashHalfHeight, 0);
   //std::ostringstream o;

   //std::vector<Vector_3> vvec;

   //vvec.emplace_back(slashstartR);
   //vvec.emplace_back(slashendR);

   //vvec = Triangles::Rescale(vvec);
   ////const std::string bareTriangle = Triangles::MakeTrianglePolyline(vvec);

   //std::cout << " slashstartR  " << slashstartR << std::endl;
   //std::cout << " slashendR  " << slashendR << std::endl;

   //o << "   <line id=\"oneSlash\" x1=\""
   //   << vvec[0][0]
   //   << "\" y1=\""
   //   << vvec[0][1]
   //   << "\" x2=\""
   //   << vvec[1][0]
   //   << "\" y2=\""
   //   << vvec[1][1]
   //   << "\"  style=\"stroke:rgb(0,0,0);stroke-width:"
   //   << slashWidth
   //   << "\" />\n";

   ////std::cout << o.str() << std::endl;

   //const Vector_3 slashstartP = matm60 * (slashstartR-midAC) + midBC ; 
   //const Vector_3 slashendP = matm60 * (slashendR-midAC) + midBC;
   ////const Vector_3 slashstartR = midAC + Vector_3(0, slashHalfHeight, 0);
   ////const Vector_3 slashendR = midAC - Vector_3(0, slashHalfHeight, 0);
   //vvec.clear();
   //vvec.emplace_back(slashstartP);
   //vvec.emplace_back(slashendP);

   //vvec = Triangles::Rescale(vvec);
   //o.str();
   //o << "   <line id=\"oneSlash\" x1=\""
   //   << vvec[0][0]
   //   << "\" y1=\""
   //   << vvec[0][1]
   //   << "\" x2=\""
   //   << vvec[1][0]
   //   << "\" y2=\""
   //   << vvec[1][1]
   //   << "\"  style=\"stroke:rgb(0,0,0);stroke-width:"
   //   << slashWidth
   //   << "\" />\n";

   //std::cout << o.str() << std::endl;

}

std::string Triangles::MakeBasicTriangle() {
   std::vector<Vector_3> vvec;

   const Vector_3 A(0, 0, 0);
   const Vector_3 C(1, 0, 0);
   const Matrix_3x3 mat = Vector_3(0, 0, 1).Rotmat(60.0 / 180.0 * (4.0 * atan(1.0)));
   const Vector_3 B(mat * C);
   const Vector_3 D((A + B + C) / 3.0);

   vvec.emplace_back(A);
   vvec.emplace_back(C);
   vvec.emplace_back(B);
   vvec.emplace_back(A-0.005*B); //-delta
   vvec.emplace_back(A+0.005*D);
   vvec.emplace_back(D);
   vvec.emplace_back(B);
   vvec.emplace_back(D);
   vvec.emplace_back(C);

   vvec = Triangles::Rescale(vvec);
   const std::string bareTriangle = Triangles::MakeTrianglePolyline(vvec);
   return bareTriangle;
}

std::vector<Vector_3> Triangles::MakeBasicElements()
{
   std::vector<Vector_3> vvec;

   const Vector_3 single_slash1(.5, 1./14, 0);
   const Vector_3 single_slash2(.5, -1./14, 0);

   const Vector_3 double_slash1(0.5-.01, 1./14,0);
   const Vector_3 double_slash2(0.5 - .01, -1. / 14, 0);
   const Vector_3 double_slash3(0.5 + .01, 1. / 14, 0);
   const Vector_3 double_slash4(0.5 + .01, -1. / 14, 0);

   const Vector_3 triple_slash1(0.5 - 0.4, 1. / 14, 0);
   const Vector_3 triple_slash2(0.5 - 0.4, -1. / 14, 0);
   const Vector_3 triple_slash3(0.5, 1. / 14, 0);
   const Vector_3 triple_slash4(0.5, -1. / 14, 0);
   const Vector_3 triple_slash5(0.5 + 0.4, 1. / 14, 0);
   const Vector_3 triple_slash6(0.5 -0.4, -1. / 14, 0);


   return vvec;
}
