


#include "LRL_CoordinateConversionMatrices.h"
#include "C3.h"
#include "LRL_inverse.h"
#include "S6.h"
#include "MatS6.h"
#include "MatD7.h"
//#include "MatE3.h"
#include "MatG6.h"
#include "MatMN.h"
#include "MatN.h"
#include "S6BoundaryTransforms.h"

#include <string>
#include <vector>

#pragma warning( disable : 4100) //  unreferenced formal parameter
#pragma warning( disable : 4702) //  unreachable code

std::vector<MatS6> MatS6::vS6_Refl = MatS6::GetReflections();


MatS6::MatS6(void)
   : m_mat(36)
{}

MatS6 MatS6::e3Tos6(const std::vector<double>& m) {
	/*
	*************************************************************************
	Note that the E3 matrix (m) is applied in the order 
	shown in Burzlaff and Zimmermann, 1985. This is the
	transpose of the order shown in the International
	Tables.

	The algebra shown here was derived by Herbert Bernstein on 2019-03-19
	(with modest help from Larry Andrews). HJB used the program maxima
	to do the derivation. Maxima produced the transpose of the matrix
	where the S6 vector multiplies from the right, and the values
	produced by maxima are the negatives of the matrix coefficients.
	*************************************************************************
	*/
	const double m11 = m[0];
	const double m12 = m[1];
	const double m13 = m[2];
	const double m21 = m[3];
	const double m22 = m[4];
	const double m23 = m[5];
	const double m31 = m[6];
	const double m32 = m[7];
	const double m33 = m[8];
	//	%i51) yresult([0, 0, 0], [1, 0, 0], [-1, 0, 0]);

	MatS6 m6;
	m6[0] = m23 * m33 - m22 * m33 - m23 * m32 + m22 * m32;
	m6[6] = m13 * m33 - m12 * m33 - m13 * m32 + m12 * m32;
	m6[12] = m13 * m23 - m12 * m23 - m13 * m22 + m12 * m22;
	m6[18] = (-m13 * m33) + m12 * m33 + m13 * m32 - m12 * m32 - m13 * m23 + m12 * m23 + m13 * m22 - m12 * m22 - m13 * m13 + 2 * m12*m13 - m12 * m12;
	m6[24] = (-m23 * m33) + m22 * m33 + m23 * m32 - m22 * m32 - m23 * m23 + 2 * m22*m23 - m13 * m23 + m12 * m23 - m22 * m22 + m13 * m22 - m12 * m22;
	m6[30] = (-m33 * m33) + 2 * m32*m33 - m23 * m33 + m22 * m33 - m13 * m33 + m12 * m33 - m32 * m32 + m23 * m32 - m22 * m32 + m13 * m32 - m12 * m32;


	//		(%i52) yresult([1, 0, 0], [0, 0, 0], [-1, 0, 0]);

	m6[1] = m23 * m33 - m21 * m33 - m23 * m31 + m21 * m31;
	m6[7] = m13 * m33 - m11 * m33 - m13 * m31 + m11 * m31;
	m6[13] = m13 * m23 - m11 * m23 - m13 * m21 + m11 * m21;
	m6[19] = (-m13 * m33) + m11 * m33 + m13 * m31 - m11 * m31 - m13 * m23 + m11 * m23 + m13 * m21 - m11 * m21 - m13 * m13 + 2 * m11*m13 - m11 * m11;
	m6[25] = (-m23 * m33) + m21 * m33 + m23 * m31 - m21 * m31 - m23 * m23 + 2 * m21*m23 - m13 * m23 + m11 * m23 - m21 * m21 + m13 * m21 - m11 * m21;
	m6[31] = (-m33 * m33) + 2 * m31*m33 - m23 * m33 + m21 * m33 - m13 * m33 + m11 * m33 - m31 * m31 + m23 * m31 - m21 * m31 + m13 * m31 - m11 * m31;

	//		(%i53) yresult([1; 0; 0]; [-1; 0; 0]; [0; 0; 0]);

	m6[2] = m22 * m32 - m21 * m32 - m22 * m31 + m21 * m31;
	m6[8] = m12 * m32 - m11 * m32 - m12 * m31 + m11 * m31;
	m6[14] = m12 * m22 - m11 * m22 - m12 * m21 + m11 * m21;
	m6[20] = (-m12 * m32) + m11 * m32 + m12 * m31 - m11 * m31 - m12 * m22 + m11 * m22 + m12 * m21 - m11 * m21 - m12 * m12 + 2 * m11*m12 - m11 * m11;
	m6[26] = (-m22 * m32) + m21 * m32 + m22 * m31 - m21 * m31 - m22 * m22 + 2 * m21*m22 - m12 * m22 + m11 * m22 - m21 * m21 + m12 * m21 - m11 * m21;
	m6[32] = (-m32 * m32) + 2 * m31*m32 - m22 * m32 + m21 * m32 - m12 * m32 + m11 * m32 - m31 * m31 + m22 * m31 - m21 * m31 + m12 * m31 - m11 * m31;

	//		(%i54) yresult([1, 0, 0], [0, 0, 0], [0, 0, 0]);

	m6[3] = m21 * m31;
	m6[9] = m11 * m31;
	m6[15] = m11 * m21;
	m6[21] = (-m11 * m31) - m11 * m21 - m11 * m11;
	m6[27] = (-m21 * m31) - m21 * m21 - m11 * m21;
	m6[33] = (-m31 * m31) - m21 * m31 - m11 * m31;

	//		(%i55) yresult([0; 0; 0]; [1; 0; 0]; [0; 0; 0]);

	m6[4] = m22 * m32;
	m6[10] = m12 * m32;
	m6[16] = m12 * m22;
	m6[22] = (-m12 * m32) - m12 * m22 - m12 * m12;
	m6[28] = (-m22 * m32) - m22 * m22 - m12 * m22;
	m6[34] = (-m32 * m32) - m22 * m32 - m12 * m32;
	//		(%i56) yresult([0; 0; 0]; [0; 0; 0]; [1; 0; 0]);

	m6[5] = m23 * m33;
	m6[11] = m13 * m33;
	m6[17] = m13 * m23;
	m6[23] = (-m13 * m33) - m13 * m23 - m13 * m13;
	m6[29] = (-m23 * m33) - m23 * m23 - m13 * m23;
	m6[35] = (-m33 * m33) - m23 * m33 - m13 * m33;

   for (size_t i = 0; i < 36; ++i) if (m6[i] == 0.0) m6[i] = -0.0;
	return -m6;
}

MatS6 MatS6::Inverse( const MatS6& min ) {
   MatS6 m( min );
   std::vector<double> arout( 36 );
   inverse( 6, min.GetVector( ).data( ), arout.data( ) );
   m.SetVector( arout );
   return m;
}


MatS6::MatS6(const double s1, const double s2, const double s3, const double s4, const double s5, const double s6, const double s7, const double s8, const double s9, const double s10, const double s11, const double s12, const double s13, const double s14, const double s15, const double s16, const double s17, const double s18, const double s19, const double s20, const double s21, const double s22, const double s23, const double s24, const double s25, const double s26, const double s27, const double s28, const double s29, const double s30, const double s31, const double s32, const double s33, const double s34, const double s35, const double s36)
   : m_mat(36)
{
   m_mat[0] = s1;
   m_mat[1] = s2;
   m_mat[2] = s3;
   m_mat[3] = s4;
   m_mat[4] = s5;
   m_mat[5] = s6;
   m_mat[6] = s7;
   m_mat[7] = s8;
   m_mat[8] = s9;
   m_mat[9] = s10;

   m_mat[10] = s11;
   m_mat[11] = s12;
   m_mat[12] = s13;
   m_mat[13] = s14;
   m_mat[14] = s15;
   m_mat[15] = s16;
   m_mat[16] = s17;
   m_mat[17] = s18;
   m_mat[18] = s19;
   m_mat[19] = s20;

   m_mat[20] = s21;
   m_mat[21] = s22;
   m_mat[22] = s23;
   m_mat[23] = s24;
   m_mat[24] = s25;
   m_mat[25] = s26;
   m_mat[26] = s27;
   m_mat[27] = s28;
   m_mat[28] = s29;
   m_mat[29] = s30;

   m_mat[30] = s31;
   m_mat[31] = s32;
   m_mat[32] = s33;
   m_mat[33] = s34;
   m_mat[34] = s35;
   m_mat[35] = s36;
}

//MatS6::MatS6(const LRL_Cell& m)
//   : m_mat(36)
//{throw;}

MatS6::MatS6(const MatS6& m)
   : m_mat(m.m_mat)
{}

MatS6::MatS6(const MatD7& m)
   : MatS6()
{
   MatMN mn = LRL_CoordinateConversionMatrices::S6_FROM_D7 * m * LRL_CoordinateConversionMatrices::D7_FROM_S6;
   m_mat.SetVector(mn.GetVector());
}

MatS6::MatS6(const MatG6& m)
   : MatS6()
{
	const static LRL_CoordinateConversionMatrices lccm;
	MatMN mn = lccm.S6_FROM_G6 * m * lccm.G6_FROM_S6;
   m_mat.SetVector(mn.GetVector());
}

MatS6::MatS6(const MatN& m)
   : m_mat(m)
{}

MatS6::MatS6(const MatMN& m)
   : m_mat(m.GetDim())
{
   if (m.GetColDim() != m.GetRowDim()) throw;
   const size_t n = m.GetRowDim();
   m_mat.resize(n);
   for (size_t i = 0; i < n; ++i) m_mat[i] = m[i];
   m_mat.SetDim(n*n);
   m_mat.SetRowDim(n);
}

//MatS6::MatS6(const MatE3& m)
//	: m_mat(36)
//{
//	*this = MatE3::E3ToS6(m);
//}

MatS6::MatS6(const std::string& s)
   : MatS6()
{
   MatN mn(m_mat.FromString(s));
   (*this) = MatS6(mn);
}

MatS6::MatS6(const std::vector<double>& v)
   : MatS6()
{
   for (size_t i = 0; i < 36; ++i) {
      m_mat[i] = v[i];
   }
}

MatS6& MatS6::operator= (const MatB4& v) {
   m_mat.resize(36);
   throw("we don't know how to do this");
   return *this;
}

MatS6& MatS6::operator= (const MatS6& v) {
   m_mat.SetVector(v.GetVector());
   m_mat.SetDim(v.size());
   m_mat.SetRowDim((size_t)std::sqrt(v.size()));
   return *this;
}

MatS6& MatS6::operator= (const MatD7& m) {
   m_mat.resize(36);
   MatMN mn = LRL_CoordinateConversionMatrices::S6_FROM_D7 * m * LRL_CoordinateConversionMatrices::D7_FROM_S6;
   m_mat.SetVector(mn.GetVector());

   return *this;
}

MatS6& MatS6::operator= (const MatG6& m) {
   m_mat.resize(36);
   MatMN mn = LRL_CoordinateConversionMatrices::S6_FROM_G6 * m * LRL_CoordinateConversionMatrices::G6_FROM_S6;
   m_mat.SetVector(mn.GetVector());
   return *this;
}

MatS6& MatS6::operator+= (const MatS6& d) {
   for (size_t i = 0; i < 36; ++i) m_mat[i] += d[i];
   return *this;
}

MatS6& MatS6::operator-= (const MatS6& d) {
   for (size_t i = 0; i < 36; ++i) m_mat[i] -= d[i];
   return *this;
}

bool MatS6::operator== (const MatS6& m) const {
   for (size_t i = 0; i < 36; ++i) if (m_mat[i] != m[i]) return false;
   return true;
}

bool MatS6::operator!= (const MatS6& m) const {
   return !((*this) == m);
}

MatS6& MatS6::operator/= (const double d) {
   for (size_t i = 0; i < 36; ++i) m_mat[i] /= d;
   return *this;
}

MatS6& MatS6::operator*= (const double d) {
   for (size_t i = 0; i < 36; ++i) m_mat[i] *= d;
   return *this;
}

MatS6 MatS6::operator+ (const MatS6& v) const {
   MatS6 m(*this);
   return (m += v);
}

MatS6 MatS6::operator- (const MatS6& v) const {
   MatS6 m(*this);
   return (m -= v);
}

MatS6 MatS6::operator- (void) const { // unary
   MatS6 m(*this);
   m.m_mat = -m.m_mat;
   return m; // unary
}

MatS6 MatS6::operator* (const MatS6& m2) const {
   MatS6 m(*this);
   m.m_mat *= m2.m_mat;
   return m;
}

MatS6 MatS6::operator* (const double d) const {
   MatS6 m(*this);
   for (size_t i = 0; i < 36; ++i) m.m_mat[i] *= d;
   return m;
}

MatS6 MatS6::operator/ (const double d) const {
   MatS6 m(*this);
   for (size_t i = 0; i < 36; ++i) m.m_mat[i] /= d;
   return m;
}

bool MatS6::operator< (const MatS6& m) const {
   for (int i = 0; i < m_mat.size(); ++i) {
      if (m_mat[i] < m.m_mat[i]) return true;
      if (m_mat[i] > m.m_mat[i]) return false;
   }
   return false;
}

MatS6 operator*(const double d, const MatS6& m) {
   return m*d;
}

MatS6 operator/(const double d, const MatS6& m) {
   return m / d;
}

S6 MatS6::operator* (const S6& v) const {
   const S6 s6 = m_mat * v.GetVector();
   return s6;
}


double MatS6::operator[] (const size_t n) const {
   return m_mat[n];
}

double& MatS6::operator[] (const size_t n) {
   return m_mat[n];
}


double MatS6::DistanceBetween(const MatS6& v1, const MatS6& v2) {
   return ((v1 - v2).norm());
}

size_t MatS6::size(void) const {
   return 36;
}

double MatS6::norm() const {
   double sum = 0.0;
   for (size_t i = 0; i < 36; ++i) sum += m_mat[i]* m_mat[i];
   return sqrt(sum);
}

double MatS6::norm(const MatS6& t) const {
   return t.norm();
}

double MatS6::Norm() const {
   return norm();
}

double MatS6::Norm(const MatS6& t) const {
   return t.norm();
}

bool MatS6::IsUnit() const {
   long row = -1;
   for (size_t i = 0; i < 36; ++i) {
      long column = i % 6;
      if (column == 0) ++row;
      if (column == row && m_mat[i] != 1.0) return false;
      if (column != row && m_mat[i] != 0.0) return false;
   }
   return true;
}

MatS6 MatS6::Eye(void) {
   MatS6 m;
   for (size_t i = 0; i < 36; ++i)
      m[i] = 0.0;
   for (size_t i = 0; i < 36; i += 7)
      m[i] = 1.0;
   return m;
}

MatS6 MatS6::Zero(void) {
   MatS6 m;
   for (size_t i = 0; i < 36; ++i)
      m[i] = 0.0;
   return m;
}

MatS6 MatS6::transpose(const MatS6& m2) const {
   //  transpose a symmetrical matrix
   const int size = 6;
   MatS6 m;
   for (int count = 0; count < size*size; ++count) {
      const int transposeIndex = count / size + size*(count%size); //'=int(rowindex/6) +6*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
   return m;
}

void MatS6::transpose(void) {
   //  transpose a symmetrical matrix
   const int size = 6;
   MatS6& m(*this);
   MatS6 m2(*this);
   for (int count = 0; count < size*size; ++count) {
      const int transposeIndex = count / size + size*(count%size); //'=int(rowindex/6) +6*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
}


double MatS6::at(const size_t n) const {
   return m_mat[n];
}

MatS6 MatS6::unit(void) {
   m_mat.resize(36);
   for (size_t i = 0; i < 36; ++i)
      m_mat[i] = 0.0;
   for (size_t i = 0; i < 36; i += 6 + 1)
      m_mat[i] = 1.0;
   return *this;
}

MatS6 MatS6::unit(const MatS6& min) {
   MatS6 m(min);
   for (size_t i = 0; i < 36; ++i)
      m.m_mat[i] = 0.0;
   for (size_t i = 0; i < 36; i += 6 + 1)
      m.m_mat[i] = 1.0;
   return m;
}

MatS6 MatS6::GetReflection(const size_t n) {
   if (vS6_Refl.empty()) GetReflections();
   return vS6_Refl[n];
}

std::vector<MatS6> MatS6::GetReflections() {
   if (vS6_Refl.empty()) {
      vS6_Refl.resize(24);
      size_t i = 0;
      vS6_Refl[i++] = MatS6("1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1");
      vS6_Refl[i++] = MatS6("1 0 0 0 0 0  0 0 1 0 0 0  0 1 0 0 0 0  0 0 0 1 0 0  0 0 0 0 0 1  0 0 0 0 1 0");
      vS6_Refl[i++] = MatS6("1 0 0 0 0 0  0 0 0 0 0 1  0 0 0 0 1 0  0 0 0 1 0 0  0 0 1 0 0 0  0 1 0 0 0 0");
      vS6_Refl[i++] = MatS6("1 0 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 1  0 0 0 1 0 0  0 1 0 0 0 0  0 0 1 0 0 0");

      vS6_Refl[i++] = MatS6("0 1 0 0 0 0  1 0 0 0 0 0  0 0 1 0 0 0  0 0 0 0 1 0  0 0 0 1 0 0  0 0 0 0 0 1");
      vS6_Refl[i++] = MatS6("0 0 0 0 0 1  1 0 0 0 0 0  0 0 0 0 1 0  0 0 1 0 0 0  0 0 0 1 0 0  0 1 0 0 0 0");
      vS6_Refl[i++] = MatS6("0 0 1 0 0 0  1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 1  0 0 0 1 0 0  0 0 0 0 1 0");
      vS6_Refl[i++] = MatS6("0 0 0 0 1 0  1 0 0 0 0 0  0 0 0 0 0 1  0 1 0 0 0 0  0 0 0 1 0 0  0 0 1 0 0 0");

      vS6_Refl[i++] = MatS6("0 1 0 0 0 0  0 0 1 0 0 0  1 0 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 1  0 0 0 1 0 0");
      vS6_Refl[i++] = MatS6("0 0 0 0 0 1  0 0 0 0 1 0  1 0 0 0 0 0  0 0 1 0 0 0  0 1 0 0 0 0  0 0 0 1 0 0");
      vS6_Refl[i++] = MatS6("0 0 1 0 0 0  0 1 0 0 0 0  1 0 0 0 0 0  0 0 0 0 0 1  0 0 0 0 1 0  0 0 0 1 0 0");
      vS6_Refl[i++] = MatS6("0 0 0 0 1 0  0 0 0 0 0 1  1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0");

      vS6_Refl[i++] = MatS6("0 0 0 1 0 0  0 0 0 0 1 0  0 0 1 0 0 0  1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 1");
      vS6_Refl[i++] = MatS6("0 0 0 1 0 0  0 0 1 0 0 0  0 0 0 0 1 0  1 0 0 0 0 0  0 0 0 0 0 1  0 1 0 0 0 0");
      vS6_Refl[i++] = MatS6("0 0 0 1 0 0  0 0 0 0 0 1  0 1 0 0 0 0  1 0 0 0 0 0  0 0 1 0 0 0  0 0 0 0 1 0");
      vS6_Refl[i++] = MatS6("0 0 0 1 0 0  0 1 0 0 0 0  0 0 0 0 0 1  1 0 0 0 0 0  0 0 0 0 1 0  0 0 1 0 0 0");

      vS6_Refl[i++] = MatS6("0 0 0 0 1 0  0 0 0 1 0 0  0 0 1 0 0 0  0 1 0 0 0 0  1 0 0 0 0 0  0 0 0 0 0 1");
      vS6_Refl[i++] = MatS6("0 0 0 0 0 1  0 0 0 1 0 0  0 1 0 0 0 0  0 0 1 0 0 0  1 0 0 0 0 0  0 0 0 0 1 0");
      vS6_Refl[i++] = MatS6("0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1  1 0 0 0 0 0  0 1 0 0 0 0");
      vS6_Refl[i++] = MatS6("0 1 0 0 0 0  0 0 0 1 0 0  0 0 0 0 0 1  0 0 0 0 1 0  1 0 0 0 0 0  0 0 1 0 0 0");

      vS6_Refl[i++] = MatS6("0 0 0 0 1 0  0 0 1 0 0 0  0 0 0 1 0 0  0 1 0 0 0 0  0 0 0 0 0 1  1 0 0 0 0 0");
      vS6_Refl[i++] = MatS6("0 0 0 0 0 1  0 1 0 0 0 0  0 0 0 1 0 0  0 0 1 0 0 0  0 0 0 0 1 0  1 0 0 0 0 0");
      vS6_Refl[i++] = MatS6("0 0 1 0 0 0  0 0 0 0 1 0  0 0 0 1 0 0  0 0 0 0 0 1  0 1 0 0 0 0  1 0 0 0 0 0");
      vS6_Refl[i++] = MatS6("0 1 0 0 0 0  0 0 0 0 0 1  0 0 0 1 0 0  0 0 0 0 1 0  0 0 1 0 0 0  1 0 0 0 0 0");

      //const S6 v( "1 2 3 4 5 6" );
      //for (size_t i = 0; i < vS6_Refl.size( ); ++i)
      //   std::cout << C3(vS6_Refl[i] * v ) << std::endl;
   }
   return vS6_Refl;
}

std::vector<MatS6> MatS6::GetBoundaries() {
   S6BoundaryTransforms sbt;
   std::vector<MatS6> out;
   for (size_t i = 0; i < sbt.size(); ++i) {
     const auto xxx =  sbt[i];
   }
   return out;
}

MatS6 MatS6::GetBoundaries(const size_t n) {
   return MatS6();
}
