#include "RoofFigures.h"

#include <iomanip>

std::ostream& operator<< (std::ostream& os, const BaseCell& bc) {
   os << "\nm_name " << bc.m_name << std::endl;
   os << "\nm_orient \n" << bc.m_orient << std::endl;
   os << "\ninput cell " << LRL_Cell_Degrees(bc.m_cell) << std::endl;
   os << "\n Cartesian matrix \n" << bc.m_cart << std::endl;
   os << "\noriginal corners \n";
   for (const auto& point : bc.m_corners) {
      std::cout << point << std::endl;
   }
   os << "\nplaced corners \n";
   for (const auto& point : bc.m_placedCorners) {
      std::cout << point << std::endl;
   }

   std::cout << bc.DrawCell();
   return os;
}

Figure_44A::Figure_44A()
   : BaseCell(LRL_Cell(10, 10, 10, 90, 90, 90))
{
   m_name = "44A";
   m_orient.UnitMatrix();
}
static const double rad = 4.0 * atan(1.0) / 180.0;
BaseCell::BaseCell(const LRL_Cell& cell)
   : m_name("BASE")
   , m_cell(cell)
   , m_cart(Matrix_3x3::Cart(cell[0], cell[1], cell[2], cell[3] / rad, cell[4] / rad, cell[5] / rad))
   , m_orient(Vector_3(0, 1, 0).Rotmat(15.0 * rad)* Vector_3(1, 0, 0).Rotmat(15.0 * rad)* m_cart)
   , m_corners({
         Vector_3(0,0,0), Vector_3(1,0,0), Vector_3(1,1,0), Vector_3(0,1,0),
         Vector_3(0,0,1), Vector_3(1,0,1), Vector_3(1,1,1), Vector_3(0,1,1)
      })
{
   m_placedCorners = MV(m_orient, m_corners);
}

static std::string Vector_3To2DString(const Vector_3& v) {
   std::string out;
   std::stringstream o;
   o << std::setprecision(2) << " " << v[0] << "," << v[1] << " ";
   return o.str();
}

static std::string xxx(const Vector_3& v1, const Vector_3& v2) {
   std::stringstream o;
   o << std::setprecision(2) << " " << Vector_3To2DString(v1) << " " << Vector_3To2DString(v2) << " ";

   return o.str();
}

static std::string xxx(const Vector_3& v1, const Vector_3& v2, const Vector_3& v3, const Vector_3& v4) {
   std::string out;
   std::stringstream o;
   o << std::setprecision(2) << " " << xxx(v1, v2) << " " << xxx(v3, v4);
   return o.str();
}

std::string BaseCell::DrawCell() const {
   const auto& p(m_placedCorners);
   //const std::vector<Vector_3> nodes({
   //   p[0],p[1],p[2],p[3],p[0],
   //   p[4],p[5],p[6],p[7],p[4],
   //   p[2],p[5],
   //   p[3],p[6],
   //   p[1],p[4]
   //   });

   std::string total;
   const std::string pathLeader =
      "<path stroke=\"black\" fill=\"none\" stroke-width = \"2\" vector-effect=\"non-scaling-stroke\"  d=\"M ";

   const std::string path1 = pathLeader + xxx(p[0], p[1], p[2], p[3]) + "Z\"/>\n";
   const std::string path2 = pathLeader + xxx(p[4], p[5], p[6], p[7]) + "Z\"/>\n";
   const std::string path3 = pathLeader + xxx(p[0], p[4]) + "\"/>\n";
   const std::string path4 = pathLeader + xxx(p[1], p[5]) + "\"/>\n";
   const std::string path5 = pathLeader + xxx(p[2], p[6]) + "\"/>\n";
   const std::string path6 = pathLeader + xxx(p[3], p[7]) + "\"/>\n";
   const std::string path = path1 + path2 + path3 + path4 + path5 + path6;
   
   //std::cout << path << std::endl;
   //
   //
   //for (size_t i = 0; i < nodes.size(); ++i) {
   //   std::stringstream o;
   //   if (i == 0 || i == 11 || i == 13 || i == 15) total += " M ";
   //   o << std::setprecision(2) << nodes[i][0] << ",";
   //   o << std::setprecision(2) << nodes[i][1] << "    ";
   //   total += o.str();
   //}
   //total += "\" />";
   return path;
}