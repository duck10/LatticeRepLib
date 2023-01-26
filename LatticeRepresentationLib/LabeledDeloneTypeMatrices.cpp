#include "GenerateRandomLattice.h"
#include "LabeledSellaMatrices.h"
#include "LabeledDeloneTypeMatrices.h"
#include "S6Dist.h"
#include "DeloneTypeList.h"

#include <string>
#include <sstream>


bool LabeledDeloneTypeMatrices::AlreadyHasThisProjector( const MatS6& m, const LabeledDeloneTypeMatrices& lsm) const{
   for (size_t i=0; i<lsm.m_prjs.size(); ++i ) {
      const double d = (m - lsm.m_prjs[i]).norm();
      if ((m - lsm.m_prjs[i]).norm() < 1.0E-4) return true;
   }
   return false;
}

std::vector<LabeledDeloneTypeMatrices> 
   LabeledDeloneTypeMatrices::ProcessVectorMapToPerpsAndProjectors
   (const std::map<std::string, std::vector<S6> >& themap)
{
   static const MatS6 unit = MatS6().unit();
   std::vector<LabeledDeloneTypeMatrices> vlsm;
   for (std::map<std::string, std::vector<S6> >::const_iterator it = themap.begin(); it != themap.end(); ++it) {
      const std::pair<std::string, std::vector<S6> > p = *it;
      LabeledDeloneTypeMatrices lsm;
      std::vector<MatS6> vmprj;
      std::vector<MatS6> vmprp;
      std::vector<MatS6> vmcan;
      const std::string label = p.first;
      lsm.m_label = label;
      for (size_t i = 0; i < p.second.size(); ++i) {
         const MatS6 prj = ProjectorFromVector(label, p.second[i]);
         if (!AlreadyHasThisProjector(prj, lsm)) {
            lsm.m_prjs.push_back(ProjectorFromVector(label, p.second[i]));
            lsm.m_perps.push_back(unit - ProjectorFromVector(label, p.second[i]));
            lsm.m_toCanons.push_back(ToCanon(p.second[i]));
         }
      }

      vlsm.push_back(lsm);
   }
   return vlsm;
}

MatS6 LabeledDeloneTypeMatrices::ToCanon(const S6_Ordinals& s) {
   const MatS6 fromCanon = ToCanon(s.m_ordinals);
   MatN mn(36);
   mn.SetVector(fromCanon.GetVector());
   MatS6 ms6I = mn.inverse();
   for (size_t i = 0; i < 36; ++i) if(ms6I[i] == 0.0) ms6I[i] = 0.0;
   return ms6I;
}

MatS6 LabeledDeloneTypeMatrices::ToCanon(const S6& s) {
   MatS6 m(MatS6().Zero());
   //for (size_t i = 0; i < 6; ++i) {
   //   m[6 * i + int(s[i])] = 1.0;
   //}
   return m;
}

std::vector<MatS6> LabeledDeloneTypeMatrices::GetPrj(const std::string& s) {
   return m_prjs;
}

std::vector<MatS6> LabeledDeloneTypeMatrices::GetPerp(const std::string& s) {
   return m_perps;
}

std::vector<MatS6> LabeledDeloneTypeMatrices::GetTocanon(const std::string& s) {
   return m_toCanons;
}

size_t LabeledDeloneTypeMatrices::Index6(const size_t i, const size_t j) {
   return 6 * i + j % 6;
}

static size_t CountEqualButNonZero(const double d, const S6& s) {
   size_t count = 0;
   for (size_t i = 0; i < 6; ++i) {
      if (abs(s[i] - d) < 1.0E-6 && abs(d) > 1.0E-6) ++count;
   }
   return count;
}

 double LabeledDeloneTypeMatrices::Fraction(const double d, const S6& s) {
   const double count = CountEqualButNonZero(d, s);
   return (count == 0.0) ? 0.0 : 1.0 / (double)(count);
}

static S6 ReZeroScalars(const S6& s) {
   S6 s6(s);
   for (size_t i = 0; i < 6; ++i)
      if (abs(s6[i]) < 1.0E-6) s6[i] = 0.0;
   return s6;
}

MatS6 LabeledDeloneTypeMatrices::ProjectorFromVector(const std::string& label, const S6& s6in) {
   MatS6 m;
   m = m.Zero();
   const S6 s = ReZeroScalars(s6in);

   for (size_t j = 0; j < 6; ++j) {
      for (size_t k = j; k < 6; ++k) {
         const double thisFrac = Fraction(s[k], s);
         m[Index6(j, k)] = (abs(s[j] - s[k]) < 1.0E-5 && abs(s[k]) > 1.0E-5) ? thisFrac : 0.0;
         m[Index6(k, j)] = m[Index6(j, k)];
      }
   }
   return m;
}

static std::string WriteOneNumber(const double d) {
   std::ostringstream ostr;
   /*
   * This is done so that _these_ fraction are not
   * written to code as decimals. If they were, then
   * there would truncation error. Let the compiler 
   * work out the best representation.
   */
   if (abs(d - 1.0 / 3.0) < 1.0E-8) ostr << "1.0/3.0";
   else if (abs(d + 1.0 / 3.0) < 1.0E-8) ostr << "-1.0/3.0";
   else if (abs(d - 2.0 / 3.0) < 1.0E-8) ostr << "2.0/3.0";
   else if (abs(d + 2.0 / 3.0) < 1.0E-8) ostr << "-2.0/3.0";
   else if (abs(d - 1.0 / 6.0) < 1.0E-8) ostr << "1.0/6.0";
   else if (abs(d + 1.0 / 6.0) < 1.0E-8) ostr << "-1.0/6.0";
   else if (abs(d - 5.0 / 6.0) < 1.0E-8) ostr << "5.0/6.0";
   else if (abs(d + 5.0 / 6.0) < 1.0E-8) ostr << "-5.0/6.0";
   else ostr << d;
   return ostr.str();
}

std::string LabeledDeloneTypeMatrices::FinalWriteSellaMatrices(const std::string& functionName, const std::string& label, const std::vector<MatS6>& mat) const {

   std::stringstream os;
   os << "\n   std::vector<MatS6> vm_" << functionName << ";\n";
   const std::string name = "   vm_" + functionName + ".push_back(MatS6( ";
   for (size_t k = 0; k < mat.size(); ++k) {
      os << name;
      for (size_t ll = 0; ll < mat[k].size(); ++ll) {
         os << WriteOneNumber(mat[k][ll]);
         const size_t ssss = mat[k].size() - 1;
        os << ((ll < ssss) ? "," : "));\n");
      }
   }
   os << std::endl;
   return os.str();
}

void WriteSellaMatrixBase() {
   std::cout << " class SellaMatrixBase {" << std::endl;
   std::cout << "   ~SellaMatrixBase(){}" << std::endl;;
   std::cout << "protected:" << std::endl;
   std::cout << "   std::vector<MatS6> vm_prjs;" << std::endl;
   std::cout << "   std::vector<MatS6> vm_perps;" << std::endl;
   std::cout << "   std::vector<MatS6> vm_toCanons;" << std::endl;
   std::cout << "};" << std::endl << std::endl;
}

std::string LabeledDeloneTypeMatrices::WriteMatrixTypes(
   const std::string& label, 
   const std::string& functionHead,
   const std::string& matrixType,
   const std::vector<MatS6>& vm) const {

   std::stringstream s;
   
   const std::string sellaMatricesClassName = "LabeledSellaMatrices::Create"
      + functionHead + matrixType + "_"
      + label;

   
      const size_t count = vm.size();
      s << "/*  " + label +"  */\n";
      s << "LabeledSellaMatrices " << sellaMatricesClassName << "() {";
      s << "// count = " << count;
      s << FinalWriteSellaMatrices(matrixType, label, vm)/* << std::endl*/;
      s << "   return LabeledSellaMatrices(\"" << label << "\", vm_";
      s << matrixType << ");" << std::endl;
      s << "}" << std::endl;

      return s.str();
}

void LabeledDeloneTypeMatrices::WriteSellaMatrices(const std::vector<LabeledDeloneTypeMatrices>& matsForAllDeloneTypes) const {

   WriteSellaMatrixBase();

   for (size_t i = 0; i < matsForAllDeloneTypes.size(); ++i) {
      const std::string s = WriteMatrixTypes(
         matsForAllDeloneTypes[i].GetLabel(),
         "",
         "Prjs",
         matsForAllDeloneTypes[i].m_prjs);
      std::cout << s << std::endl;
   }

   for (size_t i = 0; i < matsForAllDeloneTypes.size(); ++i) {
      const std::string s = WriteMatrixTypes(
         matsForAllDeloneTypes[i].GetLabel(),
         "",
         "Perps",
         matsForAllDeloneTypes[i].m_perps);
      std::cout << s << std::endl;
   }

   for (size_t i = 0; i < matsForAllDeloneTypes.size(); ++i) {
      const std::string s = WriteMatrixTypes(
         matsForAllDeloneTypes[i].GetLabel(),
         "",
         "ToCanons",
         matsForAllDeloneTypes[i].m_toCanons);
      std::cout << s << std::endl;
   }
}
