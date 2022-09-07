#include "S6BoundaryTransforms.h"

#include "S6.h"

#include <set>

static const std::vector<MatS6> vS6_Refl = MatS6::GetReflections();

size_t FindZero(const MatS6& m)
{
   const static S6 s("1 1 1  1 1 1 ");
   const S6 temp = m * s;
   for (size_t i = 0; i < 6; ++i)
   {
      if (temp[i] == 0.0) return i;
   }
   throw("should never get here");
   return 111111111;
}

S6BoundaryTransforms::S6BoundaryTransforms()
{
   MatS6 one( 0,0,0,0,0,0,   0,1,0,0,0,0,   0,0,0,0,1,0, 0,0,0,1,0,0,   0,0,1,0,0,0,   0,0,0,0,0,1);
   std::vector<MatS6> v;

   for (size_t i = 0; i < vS6_Refl.size(); ++i)
   {
      const MatS6 s = vS6_Refl[i] * one;
      v.push_back(s);
   }

   m_vvm.resize(6);

   for (size_t i = 0; i < v.size(); ++i)
   {
      const size_t n = FindZero(v[i]);
      m_vvm[n].push_back(v[i]);
   }

   if (m_vvm.size() != 6) throw("did not find all examples");

   for (size_t i = 0; i < m_vvm.size(); ++i)
   {
      if (m_vvm[i].size() != 4) throw("all should = 4");
   }
}

S6BoundaryMatricesBase::S6BoundaryMatricesBase()
{
   m_mats = vS6_Refl;
}

std::vector<MatS6> Filter(const std::vector<MatS6>& v) {
   std::vector<MatS6> out;
   std::set<MatS6> sm;
   for (size_t i = 0; i < v.size(); ++i)  
   {
      sm.insert(v[i]);
   }

   //std::cout << v.size() << "    " << sm.size() << std::endl;
   
   for (auto   i = sm.begin(); i != sm.end(); ++i)
   {
      out.push_back(*i);
   }
   return out;
}

std::vector<MatS6> S6BoundaryMatricesBase::GetVector() const
{
   std::vector<MatS6> out;
   //m_mats.clear();
   //m_mats.push_back(MatS6().unit());
   for (size_t i = 0; i < m_mats.size(); ++i)
   {
      for (size_t kk = 0; kk < vS6_Refl.size(); ++kk)
      {
         out.push_back(vS6_Refl[kk] * m_mats[i]);
      }
   }
   return Filter(out);
}

S6BoundaryMatricesZero::S6BoundaryMatricesZero()
   : S6BoundaryMatricesBase()
{
   m_mats.clear();
   m_mats.push_back(MatS6().unit());
   std::cout << "end of S6BoundaryMatricesZero" " m_mats.size " << m_mats.size() << std::endl;
}

S6BoundaryMatricesOne::S6BoundaryMatricesOne(const size_t n)
   : S6BoundaryMatricesBase()
{
   const S6BoundaryTransforms s6bt;
   m_mats = s6bt.GetVector(n);
   m_mats.push_back(MatS6().unit());
   std::cout << "end of S6BoundaryMatricesOne" " m_mats.size " << m_mats.size() << std::endl;
}

S6BoundaryMatricesTwo::S6BoundaryMatricesTwo(const size_t n1, const size_t n2)
   : S6BoundaryMatricesBase()
{
   const S6BoundaryTransforms s6bt;
   const std::vector<MatS6> out1 = s6bt.GetVector(n1);
   const std::vector<MatS6> out2 = s6bt.GetVector(n2);
   m_mats = out1;
   m_mats.insert(m_mats.end(), out2.begin(), out2.end());

   std::vector<MatS6> out12;
   std::vector<MatS6> out21;
   for (size_t i = 0; i < out1.size(); ++i)
   {
      for (size_t kk = 0; kk < out1.size(); ++kk)
      {
         out12.push_back(out1[i] * out2[kk]);
         out21.push_back(out2[i] * out1[kk]);
      }
   }
   m_mats.insert(m_mats.end(), out12.begin(), out12.end());
   m_mats.insert(m_mats.end(), out21.begin(), out21.end());
   
   m_mats.push_back(MatS6().unit());

   std::cout << "end of S6BoundaryMatricesTwo" " m_mats.size " << m_mats.size() << std::endl;
}

void S6BoundaryMatricesTwo::ProcessWithTwoZeros(const std::set<size_t>& exclusions, const std::vector<size_t>& zeros)
{
   std::vector<size_t> toProcess;
   for (size_t i = 0; i < zeros.size(); ++i)
   {
      if (exclusions.find(zeros[i]) != exclusions.end())
      {
         toProcess.push_back(zeros[i]);
      }
   }
   if (toProcess.size() == 1)
   {
      const std::vector<MatS6> out = S6BoundaryMatricesOne(toProcess[0]).GetVector();
      m_mats.insert(m_mats.end(), out.begin(), out.end());
   }
}

S6BoundaryMatricesTwo::S6BoundaryMatricesTwo(const std::set<size_t>& exclusions, const std::vector<size_t>& zeros)
{
   std::set<size_t> oneZero;
   oneZero.insert(zeros[0]);
   const std::vector<MatS6> out1 = S6BoundaryMatricesOne(zeros[0]).GetVector();
   const std::vector<MatS6> out12 = S6BoundaryMatricesTwo(
      oneZero, zeros).GetVector();

   oneZero.clear();
   oneZero.insert(zeros[1]);
   const std::vector<MatS6> out2 = S6BoundaryMatricesOne(zeros[1]).GetVector();
   const std::vector<MatS6> out21 = S6BoundaryMatricesTwo(
      oneZero, zeros).GetVector();
}

S6BoundaryMatricesThree::S6BoundaryMatricesThree(
   const std::vector<MatS6>& v1, 
   const std::vector<MatS6>& v2)
{
   m_mats.clear();
   for (size_t i = 0; i < v1.size(); ++i)
   {
      for (size_t kk = 0; kk < v2.size(); ++kk)
      {
         m_mats.push_back(v1[i]* v2[kk]);
      }
   }
}

S6BoundaryMatricesThree::S6BoundaryMatricesThree(const size_t n1, const size_t n2, const size_t n3)
   : S6BoundaryMatricesBase()
{

   const std::vector<MatS6> out1 = S6BoundaryMatricesOne(n1).GetVector();
   const std::vector<MatS6> out2 = S6BoundaryMatricesOne(n2).GetVector();
   const std::vector<MatS6> out3 = S6BoundaryMatricesOne(n3).GetVector();

   const std::vector<MatS6> out12 = S6BoundaryMatricesTwo(n1, n2).GetVector();
   const std::vector<MatS6> out13 = S6BoundaryMatricesTwo(n1, n3).GetVector();
   const std::vector<MatS6> out23 = S6BoundaryMatricesTwo(n2, n3).GetVector();

   const std::vector<MatS6> out123 = S6BoundaryMatricesThree(out1, out23).GetVector();
   const std::vector<MatS6> out231 = S6BoundaryMatricesThree(out2, out13).GetVector();
   const std::vector<MatS6> out321 = S6BoundaryMatricesThree(out3, out12).GetVector();


   m_mats.insert(m_mats.end(), out1 .begin(), out1 .end());
   m_mats.insert(m_mats.end(), out2 .begin(), out2 .end());
   m_mats.insert(m_mats.end(), out3 .begin(), out3 .end());
                                                   
   m_mats.insert(m_mats.end(), out12.begin(), out12.end());
   m_mats.insert(m_mats.end(), out13.begin(), out13.end());
   m_mats.insert(m_mats.end(), out23.begin(), out23.end());
                                                   
   m_mats.insert(m_mats.end(), out123.begin(), out123.end());
   m_mats.insert(m_mats.end(), out231.begin(), out231.end());
   m_mats.insert(m_mats.end(), out321.begin(), out321.end());

   std::cout << "end of S6BoundaryMatricesThree" " m_mats.size " << m_mats.size() << std::endl;
}

