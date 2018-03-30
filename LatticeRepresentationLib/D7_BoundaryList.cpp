
//#include "stdafx.h"

#include <algorithm>

#include "D7_BoundaryList.h"
#include "D7_ClassifySubboundaries.h"
#include "D7_Subboundary.h"

std::vector<MatD7> D7_BoundaryList::m_interchangematrics = D7_BoundaryList::Calculate24InterchangeMatrices();
std::vector<D7_Boundary> D7_BoundaryList::m_boundaries;

D7_Boundary D7_BoundaryList::GetBoundary(const std::string& name) {
   int nreturn = -1;
   for (unsigned int i = 0; i < D7_BoundaryList::size(); ++i) {
      const std::string subname = m_boundaries[i].GetName();
      if (m_boundaries[i].GetName() == name) {
         nreturn = i;
         break;
      }
   }

   if (nreturn < 0) throw; // name is not the name of a boundary
   return m_boundaries[nreturn];
}

int D7_BoundaryList::GetBoundaryIndex(const std::string& nameIn) {
   int nreturn = -1;
   const unsigned long found = (unsigned long)(nameIn.find('.'));
   const std::string name = (found == std::string::npos) ? nameIn : nameIn.substr(0, found);
   if (nameIn.find("."))
      for (unsigned int i = 0; i < D7_BoundaryList::size(); ++i) {
         const std::string subname = m_boundaries[i].GetName();
         if (m_boundaries[i].GetName() == name) {
            nreturn = i;
            break;
         }
      }

   return nreturn;
}

D7_Boundary D7_BoundaryList::Case1() {
   std::vector<D7_Subboundary> caseXSub(1);
   caseXSub[0].SetName("Case 1");
   caseXSub[0].SetTransform(MatD7("0 1 0 0 0 0 0   1 0 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 1 0 0 0   0 0 0 0 0 1 0   0 0 0 0 1 0 0   0 0 0 0 0 0 1"));
   caseXSub[0].SetCondition("Case 1: d1 = d2, -a-> b, -b-> -a, c-> -c");
   caseXSub[0].SetTransformsTo("Case 1");

   D7_Boundary caseX;
   caseX.SetName("Case 1");
   caseX.SetProjector(1.0 / 14.0 * MatD7(std::string("5 5 -2 -2 2 2 2 5 5 -2 -2 2 2 2 -2 -2 12 -2 2 2 2 -2 -2 -2 12 2 2 2 2 2 2 2 12 -2 -2 2 2 2 2 -2 12 -2 2 2 2 2 -2 -2 12")));
   const MatD7 d7 = MatD7::Eye();
   caseX.SetPerp(d7 - caseX.GetProjector());
   caseX.SetSubboundaries(caseXSub);
   caseX.SetSubspace("(r; r; s; t; u; v; 2r + s + t - u - v)");
   caseX.SetCondition("Case 1: d1 = d2, -a-> b, -b-> -a, c-> -c");

   return caseX;
}

D7_Boundary D7_BoundaryList::Case2() {
   //   vstrings.push_back( std::make_pair( std::string( "PD2"     ), 1.0/14.0 * MatD7( std::string( "12 -2 -2 -2 2 2 2   -2 5 5 -2 2 2 2   -2 5 5 -2 2 2 2   -2 -2 -2 12 2 2 2   2 2 2 2 12 -2 -2   2 2 2 2 -2 12 -2   2 2 2 2 -2 -2 12" ) ) ) );
   //   m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 0 1 0 0 0 0   0 1 0 0 0 0 0   0 0 0 1 0 0 0   0 0 0 0 1 0 0   0 0 0 0 0 0 1   0 0 0 0 0 1 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_2", "(r; s; s; t; u; v; r + 2s + t - u - v)", "Case 2: g2=g3, special position g5=g6" ) );
   std::vector<D7_Subboundary> caseXSub(1);
   caseXSub[0].SetName("Case 2");
   caseXSub[0].SetTransform(MatD7("1 0 0 0 0 0 0   0 0 1 0 0 0 0   0 1 0 0 0 0 0   0 0 0 1 0 0 0   0 0 0 0 1 0 0   0 0 0 0 0 0 1   0 0 0 0 0 1 0"));
   caseXSub[0].SetCondition("Case 2: g2=g3, special position g5=g6");
   caseXSub[0].SetTransformsTo("Case 2");

   D7_Boundary caseX;
   caseX.SetName("Case 2");
   caseX.SetProjector(1.0 / 14.0 * MatD7(std::string("12 -2 -2 -2 2 2 2   -2 5 5 -2 2 2 2   -2 5 5 -2 2 2 2   -2 -2 -2 12 2 2 2   2 2 2 2 12 -2 -2   2 2 2 2 -2 12 -2   2 2 2 2 -2 -2 12")));
   const MatD7 d7 = MatD7::Eye();
   caseX.SetPerp(d7 - caseX.GetProjector());
   caseX.SetSubboundaries(caseXSub);
   caseX.SetSubspace("(r; s; s; t; u; v; r + 2s + t - u - v)");
   caseX.SetCondition("Case 2: d3 = d3, special position g5=g6");
   return caseX;
}

D7_Boundary D7_BoundaryList::Case3() {
   //   m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 1 0 0 0 0 0   0 0 0 1 0 0 0   0 0 1 0 0 0 0   0 0 0 0 0 1 0   0 0 0 0 1 0 0   0 0 0 0 0 0 1" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_3", "d3 = b4, -a-> -a; -b-> -b; -c->-a +-b +-c", "Case 3: d3 = b4, -a-> -a; -b-> -b; -c->-a +-b +-c" ) );
   std::vector<D7_Subboundary> caseXSub(1);
   caseXSub[0].SetName("Case 3");
   caseXSub[0].SetTransform(MatD7("1 0 0 0 0 0 0   0 1 0 0 0 0 0   0 0 0 1 0 0 0   0 0 1 0 0 0 0   0 0 0 0 0 1 0   0 0 0 0 1 0 0   0 0 0 0 0 0 1"));
   caseXSub[0].SetCondition("Case 3: d3 = b4, -a-> -a; -b-> -b; -c->-a +-b +-c");
   caseXSub[0].SetTransformsTo("Case 3");

   //   vstrings.push_back( std::make_pair( std::string( "PD3"     ), 1.0/14.0 * MatD7( std::string( "12 -2 -2 -2 2 2 2   -2 12 -2 -2 2 2 2   -2 -2 5 5 2 2 2   -2 -2 5 5 2 2 2   2 2 2 2 12 -2 -2   2 2 2 2 -2 12 -2   2 2 2 2 -2 -2 12" ) ) ) );
   D7_Boundary caseX;
   caseX.SetName("Case 3");
   caseX.SetProjector(1.0 / 14.0 * MatD7(std::string("12 -2 -2 -2 2 2 2   -2 12 -2 -2 2 2 2   -2 -2 5 5 2 2 2   -2 -2 5 5 2 2 2   2 2 2 2 12 -2 -2   2 2 2 2 -2 12 -2   2 2 2 2 -2 -2 12")));
   const MatD7 d7 = MatD7::Eye();
   caseX.SetPerp(d7 - caseX.GetProjector());
   caseX.SetSubboundaries(caseXSub);
   caseX.SetSubspace("(r; s; t; t; u; v; r + s + 2t -u -v)");
   caseX.SetCondition("Case 3: d3 = b4, -a-> -a; -b-> -b; -c->-a +-b +-c");

   return caseX;
}

D7_Boundary D7_BoundaryList::Case4() {
   //   Case 4: d5 = d2+d3
   /*

   */
   //   m_mat.push_back(MatD7( "0 1 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 0 0 1 0   0 0 0 0 0 0 1   1 0 0 0 0 0 0   0 0 0 1 0 0 0   0 2 2 0 -1 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_4.1", "Case 4: d5 = d2+d3, (see equation 5.3)", "Case 4" ) );
   std::vector<D7_Subboundary> caseXSub(8);
   caseXSub[0].SetName("Case 4.1");
   caseXSub[0].SetTransform(MatD7("0 1 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 0 0 1 0   0 0 0 0 0 0 1   1 0 0 0 0 0 0   0 0 0 1 0 0 0   0 2 2 0 -1 0 0"));
   caseXSub[0].SetCondition("Case 4: d5 = d2+d3, (see equation 5.3)");
   caseXSub[0].SetTransformsTo("Case 8.6");

   //   m_mat.push_back(MatD7( "0 1 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 0 0 0 1   0 0 0 0 0 1 0   0 0 0 1 0 0 0   1 0 0 0 0 0 0   0 2 2 0 -1 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_4.2", "Case 4: d5 = d2+d3, (see equation 5.3)", "Case 4" ) );
   caseXSub[1].SetName("Case 4.2");
   caseXSub[1].SetTransform(MatD7("0 1 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 0 0 0 1   0 0 0 0 0 1 0   0 0 0 1 0 0 0   1 0 0 0 0 0 0   0 2 2 0 -1 0 0"));
   caseXSub[1].SetCondition("Case 4: d5 = d2+d3, (see equation 5.3)");
   caseXSub[1].SetTransformsTo("Case 8.5");

   //   m_mat.push_back(MatD7( "0 1 0 0 0 0 0   0 0 0 0 0 1 0   0 0 1 0 0 0 0   0 0 0 0 0 0 1   1 0 0 0 0 0 0   0 2 2 0 -1 0 0   0 0 0 1 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_4.3", "Case 4: d5 = d2+d3, (see equation 5.3)", "Case 4" ) );
   caseXSub[2].SetName("Case 4.3");
   caseXSub[2].SetTransform(MatD7("0 1 0 0 0 0 0   0 0 0 0 0 1 0   0 0 1 0 0 0 0   0 0 0 0 0 0 1   1 0 0 0 0 0 0   0 2 2 0 -1 0 0   0 0 0 1 0 0 0"));
   caseXSub[2].SetCondition("Case 4: d5 = d2+d3, (see equation 5.3)");
   caseXSub[2].SetTransformsTo("Case 6.7");

   //   m_mat.push_back(MatD7( "0 1 0 0 0 0 0   0 0 0 0 0 1 0   0 0 0 0 0 0 1   0 0 1 0 0 0 0   0 2 2 0 -1 0 0   1 0 0 0 0 0 0   0 0 0 1 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_4.4", "Case 4: d5 = d2+d3, (see equation 5.3)", "Case 4" ) );
   caseXSub[3].SetName("Case 4.4");
   caseXSub[3].SetTransform(MatD7("0 1 0 0 0 0 0   0 0 0 0 0 1 0   0 0 0 0 0 0 1   0 0 1 0 0 0 0   0 2 2 0 -1 0 0   1 0 0 0 0 0 0   0 0 0 1 0 0 0"));
   caseXSub[3].SetCondition("Case 4: d5 = d2+d3, (see equation 5.3)");
   caseXSub[3].SetTransformsTo("Case 5.7");

   //   m_mat.push_back(MatD7( "0 1 0 0 0 0 0   0 0 0 0 0 0 1   0 0 1 0 0 0 0   0 0 0 0 0 1 0   0 0 0 1 0 0 0   0 2 2 0 -1 0 0   1 0 0 0 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_4.5", "Case 4: d5 = d2+d3, (see equation 5.3)", "Case 4" ) );
   caseXSub[4].SetName("Case 4.5");
   caseXSub[4].SetTransform(MatD7("0 1 0 0 0 0 0   0 0 0 0 0 0 1   0 0 1 0 0 0 0   0 0 0 0 0 1 0   0 0 0 1 0 0 0   0 2 2 0 -1 0 0   1 0 0 0 0 0 0"));
   caseXSub[4].SetCondition("Case 4: d5 = d2+d3, (see equation 5.3)");
   caseXSub[4].SetTransformsTo("Case 6.8");

   //   m_mat.push_back(MatD7( "0 1 0 0 0 0 0   0 0 0 0 0 0 1   0 0 0 0 0 1 0   0 0 1 0 0 0 0   0 2 2 0 -1 0 0   0 0 0 1 0 0 0   1 0 0 0 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_4.6", "Case 4: d5 = d2+d3, (see equation 5.3)", "Case 4" ) );
   caseXSub[5].SetName("Case 4.6");
   caseXSub[5].SetTransform(MatD7("0 1 0 0 0 0 0   0 0 0 0 0 0 1   0 0 0 0 0 1 0   0 0 1 0 0 0 0   0 2 2 0 -1 0 0   0 0 0 1 0 0 0   1 0 0 0 0 0 0"));
   caseXSub[5].SetCondition("Case 4: d5 = d2+d3, (see equation 5.3)");
   caseXSub[5].SetTransformsTo("Case 5.8");

   //   m_mat.push_back(MatD7( "0 0 0 0 0 1 0   0 1 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 0 0 0 1   0 2 2 0 -1 0 0   1 0 0 0 0 0 0   0 0 0 1 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_4.7", "Case 4: d5 = d2+d3, (see equation 5.3)", "Case 4" ) );
   caseXSub[6].SetName("Case 4.7");
   caseXSub[6].SetTransform(MatD7("0 0 0 0 0 1 0   0 1 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 0 0 0 1   0 2 2 0 -1 0 0   1 0 0 0 0 0 0   0 0 0 1 0 0 0"));
   caseXSub[6].SetCondition("Case 4: d5 = d2+d3, (see equation 5.3)");
   caseXSub[6].SetTransformsTo("Case 4.7");

   //   m_mat.push_back(MatD7( "0 0 0 0 0 0 1   0 1 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 0 0 1 0   0 2 2 0 -1 0 0   0 0 0 1 0 0 0   1 0 0 0 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_4.8", "Case 4: d5 = d2+d3, (see equation 5.3)", "Case 4" ) );
   caseXSub[7].SetName("Case 4.8");
   caseXSub[7].SetTransform(MatD7("0 0 0 0 0 0 1   0 1 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 0 0 1 0   0 2 2 0 -1 0 0   0 0 0 1 0 0 0   1 0 0 0 0 0 0"));
   caseXSub[7].SetCondition("Case 4: d5 = d2+d3, (see equation 5.3)");
   caseXSub[7].SetTransformsTo("Case 4.8");

   //   vstrings.push_back( std::make_pair( std::string( "PB4"     ), 1.0/12.0 * MatD7( std::string( "9 0 0 -3 0 3 3   0 8 -4 0 4 0 0   0 -4 8 0 4 0 0   -3 0 0 9 0 3 3   0 4 4 0 8 0 0   3 0 0 3 0 9 -3   3 0 0 3 0 -3 9" ) ) ) );
   D7_Boundary caseX;
   caseX.SetName("Case 4");
   caseX.SetProjector(1.0 / 12.0 * MatD7(std::string("9 0 0 -3 0 3 3   0 8 -4 0 4 0 0   0 -4 8 0 4 0 0   -3 0 0 9 0 3 3   0 4 4 0 8 0 0   3 0 0 3 0 9 -3   3 0 0 3 0 -3 9")));
   const MatD7 d7 = MatD7::Eye();
   caseX.SetPerp(d7 - caseX.GetProjector());
   caseX.SetSubboundaries(caseXSub);
   caseX.SetSubspace("????");
   caseX.SetCondition("Case 4: d5 = d2+d3, (see equation 5.3)");

   return caseX;
}
D7_Boundary D7_BoundaryList::Case5() {
   //   Case 5: d5 = d2 +d3 (see equation 5.4)
   std::vector<D7_Subboundary> caseXSub(8);

   //   m_mat.push_back(MatD7( "0 0 0 0 0 1 0   1 0 0 0 0 0 0   0 0 0 1 0 0 0   0 0 0 0 0 0 1   0 -2 -2 0 1 2 2   0 1 0 0 0 0 0   0 0 1 0 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_5.5", "Case 5: d5 = d2 +d3 (see equation 5.4)", "Case 5" ) );
   caseXSub[0].SetName("Case 5.1");
   caseXSub[0].SetTransform(MatD7("1 0 0 0 0 0 0   0 0 0 1 0 0 0   0 0 0 0 0 1 0   0 0 0 0 0 0 1   0 1 0 0 0 0 0   0 0 1 0 0 0 0    0 -2 -2 0 1 2 2"));
   caseXSub[0].SetCondition("Case 5: d5 = d2 +d3 (see equation 5.4)");
   caseXSub[0].SetTransformsTo("Case 8.x");

   //   m_mat.push_back(MatD7( "0 0 0 0 0 0 1   1 0 0 0 0 0 0   0 0 0 1 0 0 0   0 0 0 0 0 1 0   0 -2 -2 0 1 2 2   0 0 1 0 0 0 0   0 1 0 0 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_5.6", "Case 5: d5 = d2 +d3 (see equation 5.4)", "Case 5" ) );
   caseXSub[1].SetName("Case 5.2");
   caseXSub[1].SetTransform(MatD7("1 0 0 0 0 0 0   0 0 0 1 0 0 0   0 0 0 0 0 0 1   0 0 0 0 0 1 0   0 0 1 0 0 0 0   0 1 0 0 0 0 0   0 -2 -2 0 1 2 2"));
   caseXSub[1].SetCondition("Case 5: d5 = d2 +d3 (see equation 5.4)");
   caseXSub[1].SetTransformsTo("Case 8.y");

   //   m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 0 0 0 0 1 0   0 0 0 1 0 0 0   0 0 0 0 0 0 1   0 1 0 0 0 0 0   0 -2 -2 0 1 2 2   0 0 1 0 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_5.1", "Case 5: d5 = d2 +d3 (see equation 5.4)", "Case 5" ) );
   caseXSub[2].SetName("Case 5.3");
   caseXSub[2].SetTransform(MatD7("1 0 0 0 0 0 0   0 0 0 0 0 1 0   0 0 0 1 0 0 0   0 0 0 0 0 0 1   0 1 0 0 0 0 0   0 -2 -2 0 1 2 2   0 0 1 0 0 0 0"));
   caseXSub[2].SetCondition("Case 5: d5 = d2 +d3 (see equation 5.4)");
   caseXSub[2].SetTransformsTo("Case 6.4");

   //   m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 0 0 0 0 1 0   0 0 0 0 0 0 1   0 0 0 1 0 0 0   0 -2 -2 0 1 2 2   0 1 0 0 0 0 0   0 0 1 0 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_5.2", "Case 5: d5 = d2 +d3 (see equation 5.4)", "Case 5" ) );
   caseXSub[3].SetName("Case 5.4");
   caseXSub[3].SetTransform(MatD7("1 0 0 0 0 0 0   0 0 0 0 0 1 0   0 0 0 0 0 0 1   0 0 0 1 0 0 0   0 -2 -2 0 1 2 2   0 1 0 0 0 0 0   0 0 1 0 0 0 0"));
   caseXSub[3].SetCondition("Case 5: d5 = d2 +d3 (see equation 5.4)");
   caseXSub[3].SetTransformsTo("Case 5.4");

   //   m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 0 0 0 0 0 1   0 0 0 1 0 0 0   0 0 0 0 0 1 0   0 0 1 0 0 0 0   0 -2 -2 0 1 2 2   0 1 0 0 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_5.3", "Case 5: d5 = d2 +d3 (see equation 5.4)", "Case 5" ) );
   caseXSub[4].SetName("Case 5.5");
   caseXSub[4].SetTransform(MatD7("1 0 0 0 0 0 0   0 0 0 0 0 0 1   0 0 0 1 0 0 0   0 0 0 0 0 1 0   0 0 1 0 0 0 0   0 -2 -2 0 1 2 2   0 1 0 0 0 0 0"));
   caseXSub[4].SetCondition("Case 5: d5 = d2 +d3 (see equation 5.4)");
   caseXSub[4].SetTransformsTo("Case 6.6");

   //   m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 0 0 0 0 0 1   0 0 0 0 0 1 0   0 0 0 1 0 0 0   0 -2 -2 0 1 2 2   0 0 1 0 0 0 0   0 1 0 0 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_5.4", "Case 5: d5 = d2 +d3 (see equation 5.4)", "Case 5" ) );
   caseXSub[5].SetName("Case 5.6");
   caseXSub[5].SetTransform(MatD7("1 0 0 0 0 0 0   0 0 0 0 0 0 1   0 0 0 0 0 1 0   0 0 0 1 0 0 0   0 -2 -2 0 1 2 2   0 0 1 0 0 0 0   0 1 0 0 0 0 0"));
   caseXSub[5].SetCondition("Case 5: d5 = d2 +d3 (see equation 5.4)");
   caseXSub[5].SetTransformsTo("Case 5.6");


   //   m_mat.push_back(MatD7( "0 0 0 0 0 1 0   1 0 0 0 0 0 0   0 0 0 1 0 0 0   0 0 0 0 0 0 1   0 -2 -2 0 1 2 2   0 1 0 0 0 0 0   0 0 1 0 0 0 0" ) );
//   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_5.5", "Case 5: d5 = d2 +d3 (see equation 5.4)", "Case 5" ) );
   caseXSub[6].SetName("Case 5.7");
   caseXSub[6].SetTransform(MatD7("0 0 0 0 0 1 0   1 0 0 0 0 0 0   0 0 0 1 0 0 0   0 0 0 0 0 0 1   0 -2 -2 0 1 2 2   0 1 0 0 0 0 0   0 0 1 0 0 0 0"));
   caseXSub[6].SetCondition("Case 5: d5 = d2 +d3 (see equation 5.4)");
   caseXSub[6].SetTransformsTo("Case 4.4");

   //   m_mat.push_back(MatD7( "0 0 0 0 0 0 1   1 0 0 0 0 0 0   0 0 0 1 0 0 0   0 0 0 0 0 1 0   0 -2 -2 0 1 2 2   0 0 1 0 0 0 0   0 1 0 0 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_5.6", "Case 5: d5 = d2 +d3 (see equation 5.4)", "Case 5" ) );
   caseXSub[7].SetName("Case 5.8");
   caseXSub[7].SetTransform(MatD7("0 0 0 0 0 0 1   1 0 0 0 0 0 0   0 0 0 1 0 0 0   0 0 0 0 0 1 0   0 -2 -2 0 1 2 2   0 0 1 0 0 0 0   0 1 0 0 0 0 0"));
   caseXSub[7].SetCondition("Case 5: d5 = d2 +d3 (see equation 5.4)");
   caseXSub[7].SetTransformsTo("Case 4.6");

   //   vstrings.push_back( std::make_pair( std::string( "PD5"     ), 1.0/12.0 * MatD7( std::string( "8 0 0 -4 4 0 0   0 9 -3 0 0 3 3   0 -3 9 0 0 3 3   -4 0 0 8 4 0 0   4 0 0 4 8 0 0   0 3 3 0 0 9 -3   0 3 3 0 0 -3 9" ) ) ) );
   D7_Boundary caseX;
   caseX.SetName("Case 5");
   caseX.SetProjector(1.0 / 12.0 * MatD7(std::string("8 0 0 -4 4 0 0   0 9 -3 0 0 3 3   0 -3 9 0 0 3 3   -4 0 0 8 4 0 0   4 0 0 4 8 0 0   0 3 3 0 0 9 -3   0 3 3 0 0 -3 9")));
   const MatD7 d7 = MatD7::Eye();
   caseX.SetPerp(d7 - caseX.GetProjector());
   caseX.SetSubboundaries(caseXSub);
   caseX.SetSubspace("????");
   caseX.SetCondition("Case 5: d5 = d2 +d3 (see equation 5.4)");

   return caseX;
}
D7_Boundary D7_BoundaryList::Case6() {
   //   Case 6: s6 = d1+d3, (see equation 5.5)

   std::vector<D7_Subboundary> caseXSub(8);
   //   m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 0 1 0 0   0 0 0 0 0 0 1   0 1 0 0 0 0 0   0 0 0 1 0 0 0   2 0 2 0 0 -1 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_6.1", "Case 6: s6 = d1+d3, (see equation 5.5)", "Case 6" ) );
   caseXSub[0].SetName("Case 6.1");
   caseXSub[0].SetTransform(MatD7("1 0 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 0 1 0 0   0 0 0 0 0 0 1   0 1 0 0 0 0 0   0 0 0 1 0 0 0   2 0 2 0 0 -1 0"));
   caseXSub[0].SetCondition("Case 6: s6 = d1+d3, (see equation 5.5)");
   caseXSub[0].SetTransformsTo("Case 8.3");

   //   m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 0 0 0 1   0 0 0 0 1 0 0   0 0 0 1 0 0 0   0 1 0 0 0 0 0   2 0 2 0 0 -1 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_6.2", "Case 6: s6 = d1+d3, (see equation 5.5)", "Case 6" ) );
   caseXSub[1].SetName("Case 6.2");
   caseXSub[1].SetTransform(MatD7("1 0 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 0 0 0 1   0 0 0 0 1 0 0   0 0 0 1 0 0 0   0 1 0 0 0 0 0   2 0 2 0 0 -1 0"));
   caseXSub[1].SetCondition("Case 6: s6 = d1+d3, (see equation 5.5)");
   caseXSub[1].SetTransformsTo("Case 8.4");

   //   m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 0 0 0 1 0 0   0 0 1 0 0 0 0   0 0 0 0 0 0 1   0 1 0 0 0 0 0   2 0 2 0 0 -1 0   0 0 0 1 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_6.3", "Case 6: s6 = d1+d3, (see equation 5.5)", "Case 6" ) );
   caseXSub[2].SetName("Case 6.3");
   caseXSub[2].SetTransform(MatD7("1 0 0 0 0 0 0   0 0 0 0 1 0 0   0 0 1 0 0 0 0   0 0 0 0 0 0 1   0 1 0 0 0 0 0   2 0 2 0 0 -1 0   0 0 0 1 0 0 0"));
   caseXSub[2].SetCondition("Case 6: s6 = d1+d3, (see equation 5.5)");
   caseXSub[2].SetTransformsTo("Case 6.3");

   //   m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 0 0 0 1 0 0   0 0 0 0 0 0 1   0 0 1 0 0 0 0   2 0 2 0 0 -1 0   0 1 0 0 0 0 0   0 0 0 1 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_6.4", "Case 6: s6 = d1+d3, (see equation 5.5)", "Case 6" ) );
   caseXSub[3].SetName("Case 6.4");
   caseXSub[3].SetTransform(MatD7("1 0 0 0 0 0 0   0 0 0 0 1 0 0   0 0 0 0 0 0 1   0 0 1 0 0 0 0   2 0 2 0 0 -1 0   0 1 0 0 0 0 0   0 0 0 1 0 0 0"));
   caseXSub[3].SetCondition("Case 6: s6 = d1+d3, (see equation 5.5)");
   caseXSub[3].SetTransformsTo("Case 5.3");

   //   m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 0 0 0 0 0 1   0 0 1 0 0 0 0   0 0 0 0 1 0 0   0 0 0 1 0 0 0   2 0 2 0 0 -1 0   0 1 0 0 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_6.5", "Case 6: s6 = d1+d3, (see equation 5.5)", "Case 6" ) );
   caseXSub[4].SetName("Case 6.5");
   caseXSub[4].SetTransform(MatD7("1 0 0 0 0 0 0   0 0 0 0 0 0 1   0 0 1 0 0 0 0   0 0 0 0 1 0 0   0 0 0 1 0 0 0   2 0 2 0 0 -1 0   0 1 0 0 0 0 0"));
   caseXSub[4].SetCondition("Case 6: s6 = d1+d3, (see equation 5.5)");
   caseXSub[4].SetTransformsTo("Case 6.5");

   //   m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 0 0 0 0 0 1   0 0 0 0 1 0 0   0 0 1 0 0 0 0   2 0 2 0 0 -1 0   0 0 0 1 0 0 0   0 1 0 0 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_6.6", "Case 6: s6 = d1+d3, (see equation 5.5)", "Case 6" ) );
   caseXSub[5].SetName("Case 6.6");
   caseXSub[5].SetTransform(MatD7("1 0 0 0 0 0 0   0 0 0 0 0 0 1   0 0 0 0 1 0 0   0 0 1 0 0 0 0   2 0 2 0 0 -1 0   0 0 0 1 0 0 0   0 1 0 0 0 0 0"));
   caseXSub[5].SetCondition("Case 6: s6 = d1+d3, (see equation 5.5)");
   caseXSub[5].SetTransformsTo("Case 5.5");

   //   m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 0 0 0 0 0 1   0 0 0 0 1 0 0   0 0 1 0 0 0 0   2 0 2 0 0 -1 0   0 0 0 1 0 0 0   0 1 0 0 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_6.7", "Case 6: s6 = d1+d3, (see equation 5.5)", "Case 6" ) );
   caseXSub[6].SetName("Case 6.7");
   caseXSub[6].SetTransform(MatD7("0 0 0 0 1 0 0   1 0 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 0 0 0 1   2 0 2 0 0 -1 0   0 1 0 0 0 0 0   0 0 0 1 0 0 0"));
   caseXSub[6].SetCondition("Case 6: s6 = d1+d3, (see equation 5.5)");
   caseXSub[6].SetTransformsTo("Case 4.3");

   //   m_mat.push_back(MatD7( "0 0 0 0 0 0 1   1 0 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 0 1 0 0   2 0 2 0 0 -1 0   0 0 0 1 0 0 0   0 1 0 0 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_6.8", "Case 6: s6 = d1+d3, (see equation 5.5)", "Case 6" ) );
   caseXSub[7].SetName("Case 6.8");
   caseXSub[7].SetTransform(MatD7("0 0 0 0 0 0 1   1 0 0 0 0 0 0   0 0 1 0 0 0 0   0 0 0 0 1 0 0   2 0 2 0 0 -1 0   0 0 0 1 0 0 0   0 1 0 0 0 0 0"));
   caseXSub[7].SetCondition("Case 6: s6 = d1+d3, (see equation 5.5)");
   caseXSub[7].SetTransformsTo("Case 4.5");

   //   vstrings.push_back( std::make_pair( std::string( "PS6"     ), 1.0/12.0 * MatD7( std::string( "8 0 -4 0 0 4 0   0 9 0 -3 3 0 3   -4 0 8 0 0 4 0   0 -3 0 9 3 0 3   0 3 0 3 9 0 -3   4 0 4 0 0 8 0   0 3 0 3 -3 0 9" ) ) ) );
   D7_Boundary caseX;
   caseX.SetName("Case 6");
   caseX.SetProjector(1.0 / 12.0 * MatD7(std::string("8 0 -4 0 0 4 0   0 9 0 -3 3 0 3   -4 0 8 0 0 4 0   0 -3 0 9 3 0 3   0 3 0 3 9 0 -3   4 0 4 0 0 8 0   0 3 0 3 -3 0 9")));
   const MatD7 d7 = MatD7::Eye();
   caseX.SetPerp(d7 - caseX.GetProjector());
   caseX.SetSubboundaries(caseXSub);
   caseX.SetSubspace("????");
   caseX.SetCondition("Case 6: s6 = d1+d3, (see equation 5.5)");

   return caseX;
}
D7_Boundary D7_BoundaryList::Case7() {
   ////   Case 7: s6 = d1 +d3 (see equation 5.6)
   //   m_mat.push_back(MatD7( "0 1 0 0 0 0 0   0 0 0 0 0 0 1   0 0 0 0 1 0 0   0 0 0 1 0 0 0   -2 0 -2 0 2 1 2   0 0 1 0 0 0 0   1 0 0 0 0 0 0" ) );
   //   m_descriptions.push_back( D7_Boundary( 5, 2, "D7_7.1", "Case 7: s6 = d1 +d3 (see equation 5.6)", "Case 7" ) );
   std::vector<D7_Subboundary> caseXSub(1);
   caseXSub[0].SetName("Case 7.1");
   caseXSub[0].SetTransform(MatD7("0 1 0 0 0 0 0   0 0 0 0 0 0 1   0 0 0 0 1 0 0   0 0 0 1 0 0 0   -2 0 -2 0 2 1 2   0 0 1 0 0 0 0   1 0 0 0 0 0 0"));
   caseXSub[0].SetCondition("Case 7: s6 = d1 +d3 (see equation 5.6)");
   caseXSub[0].SetTransformsTo("Case 5.x");

   //   vstrings.push_back( std::make_pair( std::string( "PD7"     ), 1.0/12.0 * MatD7( std::string( "9 0 -3  0 3 0 3   0 8 0 -4 0 4 0   -3 0 9 0 3 0 3   0 -4 0 8 0 4 0    3 0 3 0 9 0 -3   0 4 0 4 0 8 0   3 0 3 0 -3 0 9" ) ) ) );
   D7_Boundary caseX;
   caseX.SetName("Case 7");
   caseX.SetProjector(1.0 / 12.0 * MatD7(std::string("9 0 -3  0 3 0 3   0 8 0 -4 0 4 0   -3 0 9 0 3 0 3   0 -4 0 8 0 4 0    3 0 3 0 9 0 -3   0 4 0 4 0 8 0   3 0 3 0 -3 0 9")));
   const MatD7 d7 = MatD7::Eye();
   caseX.SetPerp(d7 - caseX.GetProjector());
   caseX.SetSubboundaries(caseXSub);
   caseX.SetSubspace("????");
   caseX.SetCondition("Case 7: s6 = d1 +d3 (see equation 5.6)");

   return caseX;
}
D7_Boundary D7_BoundaryList::Case8() {
   //   Case 8: d7 = d1 +d2 (see equation 5.7)
   std::vector<D7_Subboundary> caseXSub(6);
   //m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 1 0 0 0 0 0   0 0 0 0 1 0 0   0 0 0 0 0 1 0   0 0 1 0 0 0 0   0 0 0 1 0 0 0   2 2 0 0 0 0 -1" ) );
   //m_descriptions.push_back( D7_Boundary( 5, 2, "D7_8.1", "Case 8: d7 = d1 +d2 (see equation 5.7)", "Case 8" ) );
   caseXSub[0].SetName("Case 8.1");
   caseXSub[0].SetTransform(MatD7("1 0 0 0 0 0 0   0 1 0 0 0 0 0   0 0 0 0 1 0 0   0 0 0 0 0 1 0   0 0 1 0 0 0 0   0 0 0 1 0 0 0   2 2 0 0 0 0 -1"));
   caseXSub[0].SetCondition("Case 8: d7 = d1 +d2 (see equation 5.7)");
   caseXSub[0].SetTransformsTo("Case 8.1");

   //m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 1 0 0 0 0 0   0 0 0 0 0 1 0   0 0 0 0 1 0 0   0 0 0 1 0 0 0   0 0 1 0 0 0 0   2 2 0 0 0 0 -1" ) );
   //m_descriptions.push_back( D7_Boundary( 5, 2, "D7_8.2", "Case 8: d7 = d1 +d2 (see equation 5.7)", "Case 8" ) );
   caseXSub[1].SetName("Case 8.2");
   caseXSub[1].SetTransform(MatD7("1 0 0 0 0 0 0   0 1 0 0 0 0 0   0 0 0 0 0 1 0   0 0 0 0 1 0 0   0 0 0 1 0 0 0   0 0 1 0 0 0 0   2 2 0 0 0 0 -1"));
   caseXSub[1].SetCondition("Case 8: d7 = d1 +d2 (see equation 5.7)");
   caseXSub[1].SetTransformsTo("Case 8.2");

   //m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 0 0 0 1 0 0   0 1 0 0 0 0 0   0 0 0 0 0 1 0   0 0 1 0 0 0 0   2 2 0 0 0 0 -1   0 0 0 1 0 0 0" ) );
   //m_descriptions.push_back( D7_Boundary( 5, 2, "D7_8.3", "Case 8: d7 = d1 +d2 (see equation 5.7)", "Case 8" ) );
   caseXSub[2].SetName("Case 8.3");
   caseXSub[2].SetTransform(MatD7("1 0 0 0 0 0 0   0 0 0 0 1 0 0   0 1 0 0 0 0 0   0 0 0 0 0 1 0   0 0 1 0 0 0 0   2 2 0 0 0 0 -1   0 0 0 1 0 0 0"));
   caseXSub[2].SetCondition("Case 8: d7 = d1 +d2 (see equation 5.7)");
   caseXSub[2].SetTransformsTo("Case 6.1");

   //m_mat.push_back(MatD7( "1 0 0 0 0 0 0   0 0 0 0 0 1 0   0 1 0 0 0 0 0   0 0 0 0 1 0 0   0 0 0 1 0 0 0   2 2 0 0 0 0 -1   0 0 1 0 0 0 0" ) );
   //m_descriptions.push_back( D7_Boundary( 5, 2, "D7_8.4", "Case 8: d7 = d1 +d2 (see equation 5.7)", "Case 8" ) );
   caseXSub[3].SetName("Case 8.4");
   caseXSub[3].SetTransform(MatD7("1 0 0 0 0 0 0   0 0 0 0 0 1 0   0 1 0 0 0 0 0   0 0 0 0 1 0 0   0 0 0 1 0 0 0   2 2 0 0 0 0 -1   0 0 1 0 0 0 0"));
   caseXSub[3].SetCondition("Case 8: d7 = d1 +d2 (see equation 5.7)");
   caseXSub[3].SetTransformsTo("Case 6.2");

   //m_mat.push_back(MatD7( "0 0 0 0 0 1 0   1 0 0 0 0 0 0   0 1 0 0 0 0 0   0 0 0 0 1 0 0   2 2 0 0 0 0 -1   0 0 0 1 0 0 0   0 0 1 0 0 0 0" ) );
   //m_descriptions.push_back( D7_Boundary( 5, 2, "D7_8.5", "Case 8: d7 = d1 +d2 (see equation 5.7)", "Case 8" ) );
   caseXSub[4].SetName("Case 8.5");
   caseXSub[4].SetTransform(MatD7("0 0 0 0 0 1 0   1 0 0 0 0 0 0   0 1 0 0 0 0 0   0 0 0 0 1 0 0   2 2 0 0 0 0 -1   0 0 0 1 0 0 0   0 0 1 0 0 0 0"));
   caseXSub[4].SetCondition("Case 8: d7 = d1 +d2 (see equation 5.7)");
   caseXSub[4].SetTransformsTo("Case 4.2");

   //m_mat.push_back(MatD7( "0 0 0 0 1 0 0   1 0 0 0 0 0 0   0 1 0 0 0 0 0   0 0 0 0 0 1 0   2 2 0 0 0 0 -1   0 0 1 0 0 0 0   0 0 0 1 0 0 0" ) );
   //m_descriptions.push_back( D7_Boundary( 5, 2, "D7_8.6", "Case 8: d7 = d1 +d2 (see equation 5.7)", "Case 8" ) );
   caseXSub[5].SetName("Case 8.6");
   caseXSub[5].SetTransform(MatD7("0 0 0 0 1 0 0   1 0 0 0 0 0 0   0 1 0 0 0 0 0   0 0 0 0 0 1 0   2 2 0 0 0 0 -1   0 0 1 0 0 0 0   0 0 0 1 0 0 0"));
   caseXSub[5].SetCondition("Case 8: d7 = d1 +d2 (see equation 5.7)");
   caseXSub[5].SetTransformsTo("Case 4.1");

   //   vstrings.push_back( std::make_pair( std::string( "PD8"     ), 1.0/12.0 * MatD7( std::string( "8 -4 0 0 0 0 4   -4 8 0 0 0 0 4    0 0 9 -3 3 3 0   0 0 -3 9 3 3 0    0 0 3 3 9 -3 0   0 0 3 3 -3 9 0   4 4 0 0 0 0 8" ) ) ) );
   D7_Boundary caseX;
   caseX.SetName("Case 8");
   caseX.SetProjector(1.0 / 12.0 * MatD7(std::string("8 -4 0 0 0 0 4   -4 8 0 0 0 0 4    0 0 9 -3 3 3 0   0 0 -3 9 3 3 0    0 0 3 3 9 -3 0   0 0 3 3 -3 9 0   4 4 0 0 0 0 8")));
   const MatD7 d7 = MatD7::Eye();
   caseX.SetPerp(d7 - caseX.GetProjector());
   caseX.SetSubboundaries(caseXSub);
   caseX.SetSubspace("????");
   caseX.SetCondition("Case 8: d7 = d1 +d2 (see equation 5.7)");

   return caseX;
}
D7_Boundary D7_BoundaryList::Case9() {
   //   Case 9: d7 = d3 +b4 (see equation 5.8)
   //m_mat.push_back(MatD7( "0 0 1 0 0 0 0   0 0 0 0 0 1 0   0 0 0 0 1 0 0   0 0 0 1 0 0 0   -2 -2 0 0 2 2 1   0 1 0 0 0 0 0   1 0 0 0 0 0 0" ) );
   //m_descriptions.push_back( D7_Boundary( 5, 2, "D7_9.1", "Case 9: d7 = d3 +b4 (see equation 5.8)", "Case 9" ) );
   std::vector<D7_Subboundary> caseXSub(1);
   caseXSub[0].SetName("Case 9");
   caseXSub[0].SetTransform(MatD7("0 0 1 0 0 0 0   0 0 0 0 0 1 0   0 0 0 0 1 0 0   0 0 0 1 0 0 0   -2 -2 0 0 2 2 1   0 1 0 0 0 0 0   1 0 0 0 0 0 0"));
   caseXSub[0].SetCondition("Case 9: d7 = d3 +b4 (see equation 5.8)");
   caseXSub[0].SetTransformsTo("Case 4+5.x");

   //   vstrings.push_back( std::make_pair( std::string( "PD9-2D"  ), 1.0/01.0 * MatD7( "0.1 0.1 0.1 0.1 0.1 0.1 0.2 0.1 0.1 0.1 0.1 0.1 0.1 0.2 0.1 0.1 0.1 0.1 0.1 0.1 0.2 0.1 0.1 0.1 0.1 0.1 0.1 0.2 0.1 0.1 0.1 0.1 0.6 -0.4 0.2 0.1 0.1 0.1 0.1 -0.4 0.6 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.4" ) ) ); // D7_9 2-D
   D7_Boundary caseX;
   caseX.SetName("Case 9");
//   caseX.SetProjector(1.0 / 10.0 * MatD7(" 1 1 1 1 1 1 2   1 1 1 1 1 1 2   1 1 1 1 1 1 2   1 1 1 1 1 1 2   1 1 1 1 6 -4 2   1 1 1 1 -4 6 2   2 2 2 2 2 2 4")); // D7_9 2-D
   caseX.SetProjector(1.0 / 12.0 * MatD7(" 9 -3 0 0 3 3 0   -3 9 0 0 3 3 0   0 0 8 -4 0 0 4   0 0 -4 8 0 0 4   3 3 0 0 9 -3 0   3 3 0 0 -3 9 0   0 0 4 4 0 0 8")); // D7_9 1-D
   const MatD7 d7 = MatD7::Eye();
   caseX.SetPerp(d7 - caseX.GetProjector());
   caseX.SetSubboundaries(caseXSub);
   caseX.SetSubspace("????");
   caseX.SetCondition("Case 9: d7 = d3 +b4 (see equation 5.8)");

   return caseX;
}

/*





   */

D7_BoundaryList::D7_BoundaryList()
{
   //ConstructPrimaryBoundaryList();
   m_boundaries.push_back(Case1());
   m_boundaries.push_back(Case2());
   m_boundaries.push_back(Case3());
   m_boundaries.push_back(Case4());
   m_boundaries.push_back(Case5());
   m_boundaries.push_back(Case6());
   m_boundaries.push_back(Case7());
   m_boundaries.push_back(Case8());
   m_boundaries.push_back(Case9());


   //std::cout << "Outward Pointing Normals" << std::endl;
   //for (unsigned long i = 0; i < (*this).size(); ++i) {
   //   const D7 d7 = m_boundaries[i].GetExternalNormalUnitVector();
   //   std::cout << m_boundaries[i].GetName() << "   " << d7 << std::endl;
   //}

   //if ( (*this).size() != 9 ) throw "incorrect number of primary boundaries (should be 9)";
}

std::vector<D7_Boundary> D7_BoundaryList::GetVectorOfBoundaries(void) {
   //ConstructPrimaryBoundaryList();
   return m_boundaries;
}


std::vector<MatD7> D7_BoundaryList::Calculate24InterchangeMatrices(void) {
   std::vector<MatD7> vm;
   if (vm.empty()) {
      MatD7 unitm = MatD7::Eye();
      vm.push_back(unitm);
      vm.push_back(Case1().GetSubboundary(0).GetTransform());
      vm.push_back(Case2().GetSubboundary(0).GetTransform());
      vm.push_back(Case3().GetSubboundary(0).GetTransform());

      for (unsigned int i = 0; i < vm.size(); ++i) {
         for (unsigned int j = i + 1; j < vm.size(); ++j) {
            std::vector<MatD7>::const_iterator itb = vm.begin();
            std::vector<MatD7>::const_iterator ite = vm.end();
            MatD7 mx = *itb;
            itb = ite;
            const MatD7 m = vm[i] * vm[j];
            std::find(vm.begin(), vm.end(), m);
            if (std::find(vm.begin(), vm.end(), m) == vm.end()) {
               vm.push_back(m);
               if (vm.size() >= 24) break;
            }
            if (vm.size() >= 24) break;
         }
      }
      if (vm.size() != 24) throw;
      m_interchangematrics = vm;
   }

   return vm;
}



D7_BoundaryList::~D7_BoundaryList()
{
}
