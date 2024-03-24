
#include <algorithm>
#include <cmath>
#include <iostream>     // std::cout, std::endl
#include <iomanip>      // std::setfill, std::setw
#include <string>
#include <utility>
#include <vector>

#include "D7.h"
#include "Dirichlet.h"
#include "DirichletConstants.h"
#include "DirichletCellFaces.h"
#include "FileOperations.h"
#include "G6.h"
#include "LatticeConverter.h"
#include "LRL_CreateFileName.h"
#include "LRL_DataStreamToString.h"
#include "LRL_DataToSVG.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"
#include "ReadDirichletGlobalData.h"
#include "SVG_Tools.h"
#include "TNear.h"
#include "TriangleAreaFromSides.h"
#include "LRL_Vector3.h"

template <typename T>
std::vector<T> CircularizeVector(const std::vector<T>& vin) {
   std::vector<T> v(vin);
   v.push_back(vin[0]);
   return v;
}

template<typename T>
int roundNumber(T num)
{
   return num < 0 ? int(num - 0.5) : int(num + 0.5);
}

Vector_3 round(const Vector_3& vin) {
   Vector_3 vout;
   vout[0] = roundNumber(vin[0]);
   vout[1] = roundNumber(vin[1]);
   vout[2] = roundNumber(vin[2]);
   return vout;
}

std::string DrawOneDirichletRing(const double scale, const ANGLELIST& ring, const size_t nColor) {
   const double xshift = 30;
   const double yshift = 30;
   ANGLELIST colorTest;
   std::string s;
   const Vector_3 cm = DirichletCell::CenterOfMassForOneFace(ring);

   if (ring.size() >= 3) {
      const std::string color = (cm[2] < 0.0) ? "black" : "black";
      //const std::string color = "black";
      const std::string polygonStrokeWidth = (cm[2] < 0.0) ? "2" : "2";
      s += "  <polygon fill=\"" + /*colors[nColor]*/  std::string(DirichletConstants::faceColor) +
         " \" stroke=\"" + color + "\" stroke-width=\"" + polygonStrokeWidth + "\" points=\"";
      for (size_t point = 0; point < ring.size(); ++point) {
         const Vector_3& v = ring[point].second;
         s += LRL_DataToSVG(xshift + scale * v[0], ", ", yshift + scale * v[1], " ");
      }
      s += "\" />\n";
   }
   else if (ring.size() == 1)
   {
      const double x = ring[0].second[0];
      const double y = ring[0].second[1];
      s += "<circle cx = \"" + LRL_ToString(xshift + scale * x) + "\" cy = \"" +
         LRL_ToString(yshift + scale * y) + "\" r = \"2\" stroke = \"blue\" stroke-width = \"1\" fill = \"blue\" />\n";
   }
   return s;
}

ANGLELIST RotateOneFace(const Matrix_3x3& m, const ANGLELIST& input) {
   ANGLELIST list(input);
   {
      for (size_t point = 0; point < list.size(); ++point)
         list[point].second = m * (list[point].second);
   }
   return list;
}

ANGLESFORFACES RotateAllFaces(const Matrix_3x3& m, const ANGLESFORFACES& input) {
   ANGLESFORFACES output(input);
   for (size_t face = 0; face < output.size(); ++face) {
      output[face] = RotateOneFace(m, output[face]);
   }
   return output;
}

std::vector<Vector_3> CentersOfMassForAllFaces(const ANGLESFORFACES& list) {
   std::vector<Vector_3> vcm;
   if (list.size() > 3) {
      for (size_t face = 0; face < list.size(); ++face)
         vcm.push_back(DirichletCell::CenterOfMassForOneFace(list[face]));
   }
   return vcm;
}

std::vector<double> Z_Angles(const std::vector<Vector_3>& vcm) {
   static const double degreesPerRad = 180.0 / 4.0 / atan(1.0);
   std::vector<double> output;
   for (size_t face = 0; face < vcm.size(); ++face) {
      const double angle = degreesPerRad * Vector_3::Angle(Vector_3(0, 0, 10000.0), Vector_3(0, 0, 0), vcm[face]);
      output.push_back(angle);
   }
   return output;
}

ANGLESFORFACES Z_SortAllFaces(const ANGLESFORFACES& newRinged) {
   ANGLESFORFACES sorted(newRinged);
   const std::vector<Vector_3> vcm = CentersOfMassForAllFaces(newRinged);
   std::vector<double> vAngles = Z_Angles(vcm);

   for (size_t face = 0; face < sorted.size() - 1; ++face) {
      for (size_t k = face + 1; k < sorted.size(); ++k) {
         if (vAngles[face] < vAngles[k]) {
            std::swap(vAngles[face], vAngles[k]);
            std::swap(sorted[face], sorted[k]);
         }
      }
   }
   return sorted;
}

ANGLESFORFACES RotateObject(const Matrix_3x3& m, const ANGLESFORFACES& ring) {
   ANGLESFORFACES list;
   for (size_t face = 0; face < ring.size(); ++face) {
      list.push_back(RotateOneFace(m, ring[face]));
   }
   return list;
}

std::vector<ANGLESFORFACES> CreateSeriesOfImages(const ANGLESFORFACES& inputRingsOfFace,
   const size_t nImages, const Matrix_3x3& initialRotation, const Matrix_3x3& rotationBetween) {
   std::vector<ANGLESFORFACES> vout;
   const ANGLESFORFACES oneImage(RotateObject(initialRotation, inputRingsOfFace));
   Matrix_3x3 m2(1, 0, 0, 0, 1, 0, 0, 0, 1);
   for (size_t face = 0; face < nImages; ++face) {
      vout.push_back(RotateObject(m2, oneImage));
      m2 *= rotationBetween;
   }
   return vout;
}

std::vector<Vector_3> GetVerticesFromFaces(const ANGLESFORFACES& faces) {
   CNearTree<Vector_3> tree;
   double maxCoord = -DBL_MAX;
   std::vector<Vector_3> unique;

   for (size_t face = 0; face < faces.size(); ++face) {
      const ANGLELIST& thisFace = faces[face];
      for (size_t face = 0; face < thisFace.size(); ++face) {
         const ANGLE_COORDS thisVertex = thisFace[face];  //this needs to move out of here so scale doesn't change with rotation
         maxCoord = std::max(maxCoord, thisVertex.second[0]);
         maxCoord = std::max(maxCoord, thisVertex.second[1]);
         maxCoord = std::max(maxCoord, thisVertex.second[2]);
         if (tree.NearestNeighbor(1.0E-4, thisVertex.second) == tree.end()) {
            tree.insert(thisVertex.second);
            unique.emplace_back(thisVertex.second);
         }
      }
   }
   return unique;
}

std::vector<std::string> DrawDirichletRings(const ANGLESFORFACES& newRinged) {
   const ANGLESFORFACES sorted = Z_SortAllFaces(newRinged);

   static const double degreesPerRad = 180.0 / 4.0 / atan(1.0);

   std::vector<std::string> vs;
   const Matrix_3x3 m1 = Vector_3(1, 0, 0).Rotmat(20.0 / degreesPerRad);
   const Matrix_3x3 m2 = Vector_3(0, 1, 0).Rotmat(15.0 / degreesPerRad);
   Matrix_3x3 m(1, 0, 0, 0, 1, 0, 0, 0, 1);

   double maxCoord = -DBL_MAX;
   const ANGLESFORFACES faces(RotateAllFaces(m1, sorted));

   for (size_t face = 0; face < faces.size(); ++face) {
      const ANGLELIST& thisFace = faces[face];
      for (size_t face = 0; face < thisFace.size(); ++face) {
         const ANGLE_COORDS thisVertex = thisFace[face];  //this needs to move out of here so scale doesn't change with rotation
         maxCoord = std::max(maxCoord, thisVertex.second[0]);
         maxCoord = std::max(maxCoord, thisVertex.second[1]);
         maxCoord = std::max(maxCoord, thisVertex.second[2]);
      }
   }

   const double scale = DirichletConstants::imageHeightPx * DirichletConstants::scale / maxCoord;
   for (size_t cycle = 0; cycle < 1; ++cycle) {
      const ANGLESFORFACES faces(RotateAllFaces(m, sorted));
      m = m2 * m;

      std::vector<std::pair<double, size_t> >zandindex;
      for (size_t thisFace = 0; thisFace < faces.size(); ++thisFace) {
         const Vector_3 cm = DirichletCell::CenterOfMassForOneFace(faces[thisFace]);
         zandindex.emplace_back(std::make_pair(cm[3], thisFace));
      }

      std::sort(zandindex.begin(), zandindex.end(), std::greater<>());

      for (size_t i = 0; i < zandindex.size(); ++i) {
         vs.push_back(DrawOneDirichletRing(scale, faces[zandindex[i].second], zandindex[i].second));

      }
   }
   return vs;
}


std::vector<std::string> DrawSeriesOfObjects(const std::vector<ANGLESFORFACES>& series) {
   std::vector<std::string> s;

   std::vector<std::string> vs;
   for (size_t face = 0; face < series.size(); ++face) {
      vs.push_back(LRL_StringTools::ConcatanateStrings(DrawDirichletRings(series[face])));
   }
   return vs;
}

std::vector<std::string> MadeStereo(const std::vector<std::string>& vsin) {
   int xplace = 50;
   int xdelta = 200;

   std::vector<std::string> vsout;
   const int yPlace = DirichletConstants::yposition;
   for (size_t face = 0; face < vsin.size(); ++face) {
      const std::vector<std::string> vs = PlaceSubPicture(xplace, yPlace, vsin[face]); // origin shift
      vsout.push_back(LRL_StringTools::ConcatanateStrings(vs));
      xplace += xdelta;
   }
   return vsout;
}

std::string EdgeLengths(const ANGLELIST& list) {
   std::vector<Vector_3> vin;
   for (size_t i = 0; i < list.size(); ++i) {
      vin.push_back(list[i].second);
   }
   std::vector<Vector_3> v(CircularizeVector(vin));
   std::ostringstream ostr;
   for (size_t i = 0; i < v.size() - 1; ++i) {
      const double d = (v[i] - v[i + 1]).Norm();
      ostr << d << " ";
   }
   return ostr.str();
}

static double CleanNearZero(const double d) {
   return (abs(d) < 1.0E-6) ? 0 : d;
}

static Vector_3 CleanNearZero(const Vector_3& v) {
   return { CleanNearZero(v[0]), CleanNearZero(v[1]), CleanNearZero(v[2]) };
}

std::string OneFaceRecord(const ANGLELIST& face, const Vector_3& index) {
   const double area = DirichletCell::AreaOfOneFace(face);
   const Vector_3 cm = DirichletCell::CenterOfMassForOneFace(face);
   std::ostringstream ostr;
   //ostr << std::setw(2) << std::setfill('0') << std::hex << r << g << b;
   ostr << "area= " << area << "   " << " index " << index[0] << " " << index[1] << " " << index[2] <<
      "  Center of mass " <<
      CleanNearZero(cm[0]) << " " << CleanNearZero(cm[1]) << " " << CleanNearZero(cm[2]) << std::endl;
   ostr << "   Edge lengths " << EdgeLengths(face) << std::endl;
   return (ostr.str());
}

void PrintOneFaceRecord(const ANGLELIST& face, const Vector_3& index) {
   std::cout << OneFaceRecord(face, index);
}

std::string OutputOneFace(const size_t index, const size_t vertices, const std::string& faceRecord) {
   std::ostringstream ostr;
   ostr << "FACE  " << index + 1 << " vertex count= " << vertices << "   " << faceRecord;
   return ostr.str();
}

std::string FaceRecords(const ANGLESFORFACES& rings, const std::vector<Vector_3>& indices) {
   std::ostringstream ostr;
   for (size_t faceIndex = 0; faceIndex < rings.size() / 2; ++faceIndex) {
      const ANGLELIST face1 = rings[faceIndex];
      const ANGLELIST face2 = rings[rings.size() - faceIndex - 1];
      const size_t vertexCount1 = face1.size();
      const size_t vertexCount2 = face2.size();
      const std::string record1 = OneFaceRecord(face1, indices[faceIndex]);
      const std::string record2 = OneFaceRecord(face2, indices[rings.size() - faceIndex - 1]);
      const std::string testString1 = OutputOneFace(faceIndex, vertexCount1, record1);
      const std::string testString2 = OutputOneFace(rings.size() - faceIndex - 1, vertexCount2, record2);
      ostr << testString1.c_str();
      ostr << testString2.c_str();
      ostr << std::endl;
   }

   size_t count = 0;
   for (size_t faceIndex = 0; faceIndex < rings.size(); ++faceIndex) {
      double area = DirichletCell::AreaOfOneFace(rings[faceIndex]);
      if (area > 1.0E-4) {
         ++count;
         //std::cout << count << "  " << area << "  " << indices[faceIndex];
      }
   }
   //std::cout << std::endl;

   ostr << std::endl;
   return ostr.str();
}

std::string CreateStereoSVGText(const DirichletCell& dc) {
   //------------- create rotated image
   static const double degreesPerRad = 180.0 / 4.0 / atan(1.0);
   const Matrix_3x3 m1 = Vector_3(1, 0, 0).Rotmat(DirichletConstants::rotateX / degreesPerRad);
   const Matrix_3x3 m2 = Vector_3(0, 1, 0).Rotmat(DirichletConstants::rotateStereo / degreesPerRad);

   // get the list of faces with their vertices
   const ANGLESFORFACES ringed = dc.GetAnglesForFaces();;
   //-------------create series of images
   const std::vector<ANGLESFORFACES> rings = CreateSeriesOfImages(ringed, DirichletConstants::numberOfImages, m1, m2);
   const std::vector<std::string> series = DrawSeriesOfObjects(rings);
   const std::vector<std::string> stereoImages = MadeStereo(series);
   const auto vertices = GetVerticesFromFaces(ringed);
   const std::vector<Vector_3> indices = DirichletCell::RecoverIndicesOfFaces(dc.GetCartesianMatrix(), ringed);
   const std::string records = FaceRecords(ringed, indices);
   const std::string constants = ReadCellData::GetConstantsAsString();
   const Cell_Faces cellFaces = dc.GetCellFaces();

   //std::cout << LRL_ToString(indices) << std::endl;
   //std::cout << LRL_ToString(dc.GetAreas()) << std::endl;

   const DirichletSVG dsvg(dc);
   const std::string strCell = dc.GetStrCell();
   const std::string svg = dsvg.OutputSVG(stereoImages, cellFaces,
      records + "\nGlobal Constants\n" + constants);
   return svg;
}

std::string HandleOneCell(const DirichletCell& dc) {
   std::string svg = CreateStereoSVGText(dc);
   return svg;
}

std::vector<std::string> RetrieveCellsAsStringsFromDirichletConstants() {
   std::vector<std::string> strCells;
   const std::vector<std::string>& cellText = DirichletConstants::cellData;
   for (size_t i = 0; i < cellText.size(); ++i) {
      const std::string s = cellText[i];
      strCells.push_back(s);
   }
   return strCells;
}

std::vector<std::string> GetUpwardSymmetry(const std::string& s) {
   // per Grimmer, 2015
   if (s == "aP")  return std::vector<std::string> {"mP", "mS"};
   else if (s == "mP")  return std::vector<std::string> {"oP", "oS"};
   else if (s == "mS")  return std::vector<std::string> {"oS", "oF", "oI", "hR"};
   else if (s == "oP")  return std::vector<std::string> {"tP"};
   else if (s == "oS")  return std::vector<std::string> {"tP", "hP"};
   else if (s == "oF")  return std::vector<std::string> {"tI"};
   else if (s == "oI")  return std::vector<std::string> {"tI"};
   else if (s == "tP")  return std::vector<std::string> {"cP"};
   //else if ( s == "hP") return std::vector<std::string>();
   else if (s == "hR")  return std::vector<std::string> {"cP", "cF", "cI"};
   else if (s == "tI")  return std::vector<std::string> {"cF", "cI"};
   else { return std::vector<std::string>(); }
}

void ListVertices(const DirichletCell& dc) {

   const ANGLESFORFACES ringed = dc.GetAnglesForFaces();;
   const auto vertices = GetVerticesFromFaces(ringed);

   std::cout << "; vertices" << std::endl;
   for (size_t i = 0; i < vertices.size(); ++i) {
      std::cout << i+1 << "  " << vertices[i] << std::endl;
   }
   std::cout << std::endl;

}

int main(int argc, char* argv[]) {
   LRL_ReadLatticeData reader;
   std::cout << "; Dirichlet (Voronoi) cells" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = reader.ReadLatticeData();
   std::vector<std::string> basicfileNameList;
   std::vector<std::string> FileNameList;
   std::vector<std::string> FullfileNameList;
   std::string host=std::string("");
   std::string rawprefix=std::string("");
   std::string htmlprefix=std::string("");
   bool usetimestamp=true;
   bool usehttps=false;
   bool usetarget=true;
   size_t blockstart= 0;
   size_t blocksize= 20;
   int ii;

   for(ii=1;ii<argc;ii++){
       // std::cout << "argv[" << ii <<"] = "<< argv[ii] << std::endl;
       if (std::string(argv[ii]).compare(std::string("--help"))==0) {
           std::cout << "; Dirichlet [--help ] [--host hostname] [--rawprefix prefix] [--htmlprefix htmlprefix]" << std::endl;
           std::cout << ";  [--usetimestamp {true|false}] [--usehttps {true|false}] [--usetarget {true|false}]" << std::endl;
       } else if (std::string(argv[ii]).compare(std::string("--host"))==0) {
           host = std::string(argv[++ii]);
           // std::cout << "; host: " << host;
       } else if (std::string(argv[ii]).compare(std::string("--rawprefix"))==0) {
           rawprefix = std::string(argv[++ii]);
           // std::cout << "; rawprefix: " << rawprefix;
       } else if (std::string(argv[ii]).compare(std::string("--htmlprefix"))==0) {
           htmlprefix = std::string(argv[++ii]);
           // std::cout << "; htmlprefix: " << htmlprefix;
       } else if (std::string(argv[ii]).compare(std::string("--usetimestamp"))==0) {
           usetimestamp =  (std::string(argv[++ii])).compare(std::string("true"))==0?true:false;
           // std::cout << "; usetimestamp: " << usetimestamp;
       } else if (std::string(argv[ii]).compare(std::string("--usehttps"))==0) {
           usehttps =  (std::string(argv[++ii])).compare(std::string("true"))==0?true:false;
           // std::cout << "; usehttps: " << usehttps;
       } else if (std::string(argv[ii]).compare(std::string("--usetarget"))==0) {
           usetarget =  (std::string(argv[++ii])).compare(std::string("true"))==0?true:false;
           // std::cout << "; usetarget: " << usetarget;
       } else if (std::string(argv[ii]).compare(std::string("--blockstart"))==0) {
           blockstart = std::stoul(std::string(argv[++ii]));
           // std::cout << "; blockstart: " << blockstart;
       } else if (std::string(argv[ii]).compare(std::string("--blocksize"))==0) {
           blocksize = std::stoul(std::string(argv[++ii]));
           // std::cout << "; blocksize: " << blocksize;
       }
   }

   if (blockstart + blocksize > inputList.size()) {
       if (blockstart >= inputList.size()) {
           blockstart = 0;
           blocksize = 0;
       } else {
           blocksize = inputList.size() - blockstart;
       }
   }

   basicfileNameList
       = LRL_CreateFileName::CreateListOfFilenames(inputList.size(), 
       "DC","svg",usetimestamp,blockstart,blocksize);
   FileNameList 
       = LRL_CreateFileName::CreateRawListOfFilenames(basicfileNameList,rawprefix); 
   if(htmlprefix.compare(std::string(""))==0) {
       FullfileNameList = std::vector<std::string>(FileNameList);
   } else {
       FullfileNameList = LRL_CreateFileName::CreateHTMLListOfFilenames(
           basicfileNameList, host, htmlprefix, usehttps, usetarget);
   }

   std::cout << "; Dirichlet cell block start " << blockstart << std::endl;
   std::cout << "; Dirichlet cell block size " << blocksize << std::endl;


   for (size_t i = blockstart; i < (inputList.size()) && (i < blockstart + blocksize); ++i)
   {
      std::cout << "; Dirichlet graphics file " << FullfileNameList[i - blockstart] << std::endl;
   }

   for (size_t whichCell = blockstart; 
       whichCell < inputList.size() && whichCell < blockstart+blocksize; ++whichCell) {
       const DirichletCell dc = (inputList[whichCell]);
       std::cout << inputList[whichCell].GetStrCell() << std::endl;
       ListVertices(dc);
       const std::string svg = HandleOneCell(dc);
       const std::string fileName = FileNameList[whichCell-blockstart];
       const std::string fullfileName = FullfileNameList[whichCell-blockstart];
       if (!svg.empty())
       {
           FileOperations::Write(fileName, svg);
           std::cout << "; Dirichlet graphics file " << fullfileName << std::endl;
       }
   }

   exit(0);
}

// 24 vertices in a Tetrakaidecahedron. It's a 14-sided polyhedron

/*
Bard.google.com says: (which is wrong for cubic)
Truncated Octahedron (Simple Cubic and FCC Lattices): 24 vertices
Rhombic Dodecahedron (BCC Lattice): 14 vertices
Hexagonal Prism (HCP Lattice): 14 vertices
Tetrakaidecahedron (Kelvin Cell, Diamond Lattice): 24 vertices

wikipedia
FCC The truncated octahedron has 14 faces (8 regular hexagons and 6 squares), 36 edges, and 24 vertices. 

the rhombic dodecahedron is a convex polyhedron with 12 congruent rhombic faces. It has 24 edges, and 14 vertices of 2 types.

Hexagonal Prism 12 vertices

i got 14 for FCC
i got 8 for hexagonal prism
i got 14 for random
i got 6 for P cubic
*/
