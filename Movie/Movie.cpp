
#include <iostream>
#include <sstream>

#include "FileOperations.h"
#include "LRL_CreateFileName.h"
#include "LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "Niggli.h"
#include "S6.h"
#include "Selling.h"
#include "WebIO.h"

enum eReductionType { Delone, Niggli, NONE };
eReductionType reductionType(Delone);
enum eProgressType { circular,linear};
const eProgressType progressType(circular);
const int extraFrames = 5;
int fullRangeSteps(60);

template<typename T>
std::vector<T> DoLinear( const std::vector<T>& vinput, const std::vector<T>& vReduced) {
   std::vector<T> out;
   for (size_t i = 0; i < fullRangeSteps; i++) {
      for ( size_t k=0; k<vinput.size(); ++k)
      {
         double circularFrac;
         const double linearFrac = i / double(fullRangeSteps - 1);
         {
            const double angle = linearFrac * 2.0 * atan(1.0); // 0 to 90 degrees
            const double cosine = cos(angle) / (abs(cos(angle)) + abs(sin(angle)));
            circularFrac = cosine;
         }
         const double frac = linearFrac;
         out.emplace_back((1 - frac) * vinput[k] + frac * vReduced[k]);
      }
   }
   return out;
}

std::vector<S6> DoOneFrame(const double frac, const std::vector<S6>& vin, const std::vector<S6> vred) {
   std::vector<S6> out;
   for (size_t i = 0; i < vin.size(); ++i) {
      out.emplace_back((1 - frac) * vin[i] + frac * vred[i]);
   }
   return out;
}

std::vector<G6> DoOneFrame(const double frac, const std::vector<G6>& vin, const std::vector<G6> vred) {
   std::vector<G6> out;
   for (size_t i = 0; i < vin.size(); ++i) {
      out.emplace_back((1 - frac) * vin[i] + frac * vred[i]);
   }
   return out;
}

template<typename T>
std::vector<LRL_Cell_Degrees> ConvertVectorOfLatticesToCellDegrees(const std::vector<T>& vt) {
   std::vector<LRL_Cell_Degrees> out;
   for (const auto& s : vt) {
      out.emplace_back(s);
   }
   return out;
}

int main(int argc, char* argv[])
{
   int fullRangeSteps=50;
   int testarg = 50;
   if (argc > 1) {
      testarg = atoi(argv[1]);
      if (testarg != 0) fullRangeSteps = testarg;
   }
   if (testarg > 2) {
      if (LRL_StringTools::strToupper(std::string(argv[2])) == "NIGGLI")
         reductionType = Niggli;
   }

   std::cout << "movie.cpp" << std::endl;
   LRL_ReadLatticeData reader;
   const std::vector<LRL_ReadLatticeData> inputList = reader.ReadLatticeData();
   std::vector<S6> inputS6;
   std::vector<S6> reducedS6Input;
   std::vector<G6> inputG6;
   std::vector<G6> reducedG6Input;

   G6 redG6;
   S6 redS6;
   for (const auto& input : inputList) {
      const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(input.GetLattice(), input.GetCell());
      inputS6.emplace_back(pCell);
      inputG6.emplace_back(pCell);
      Niggli::Reduce(pCell, redG6);
      Selling::Reduce(pCell, redS6);
      reducedG6Input.emplace_back(redG6);
      reducedS6Input.emplace_back(redS6);
   }

   std::vector<std::vector<LRL_Cell_Degrees>> frames;
   if (reductionType == Delone)
   {
      for (size_t i = 0; i < fullRangeSteps; ++i) {
         const double frac = double(i) / double(fullRangeSteps - 1);
         const std::vector<S6> s6Frames = DoOneFrame(frac, inputS6, reducedS6Input);
         frames.emplace_back(ConvertVectorOfLatticesToCellDegrees(s6Frames));
      }
   }
   else if (reductionType == Niggli) {
      for (size_t i = 0; i < fullRangeSteps; ++i) {
         const double frac = double(i) / double(fullRangeSteps - 1);
         const std::vector<G6> g6Frames = DoOneFrame(frac, inputG6, reducedG6Input);
         frames.emplace_back(ConvertVectorOfLatticesToCellDegrees(g6Frames));
      }
   }
   else if (reductionType == NONE) {
      for (size_t i = 0; i < fullRangeSteps; ++i) {
         const double frac = double(i) / double(fullRangeSteps - 1);
         const std::vector<G6> g6Frames = DoOneFrame(frac, inputG6, reducedG6Input);
         frames.emplace_back(ConvertVectorOfLatticesToCellDegrees(g6Frames));
      }
   }
   else {
      exit(-1);
   }

   WebIO webio(argc, argv, "Movie", fullRangeSteps+2*extraFrames);
   const size_t& blockstart = webio.m_blockstart;
   //const size_t& blocksize = webio.m_blocksize;
   webio.m_blocksize = fullRangeSteps+2*extraFrames;
   webio.GetWebBlockSize(argc, argv);
   webio.CreateFilenamesAndLinks(fullRangeSteps+2*extraFrames, "MOV", "dat");


   const std::vector<std::string>& basicfileNameList = webio.m_basicfileNameList;
   std::cout << "file name count  = " << basicfileNameList.size() << std::endl;

   size_t currentFileCounter = 0;
   for (size_t i = 0; i < frames.size(); ++i) {
      std::stringstream os;

      os.str("");
      for (const auto& f : frames[i]) {
         os << "P " << LRL_Cell_Degrees(f) << std::endl;
      }

      const size_t repeatCount = (i == 0 || i == frames.size() - 1) ? extraFrames + 1 : 1;
      for (size_t kk = 0; kk < repeatCount; ++kk)
      {
         FileOperations::Write(basicfileNameList[currentFileCounter], os.str());
         std::cout << "PlotPolar < " << basicfileNameList[currentFileCounter] << std::endl;
         std::cout << "ping 127.0.0.1 -n 3 -w 4" << std::endl;
         ++currentFileCounter;
      }
   }

   ////DoCircular(inputList);
      //std::vector<S6> outS6 = DoLinear(inputS6, reducedS6Input);
      //for (const auto& o : outS6) {
      //   std::cout << "P " << LRL_Cell_Degrees(o) << std::endl;
      //}

}
