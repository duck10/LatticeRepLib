#include "C6.h"
#include "S6.h"

#include <complex>

C6::C6()
    : m_dim(6)
    , m_valid(true)
{
}

C6::C6(const C6& c)
    : m_dim(6)
    , m_valid(c.m_valid)
    , m_vec(c.m_vec)
{
}

C6::C6(const S6& s)
   : m_dim(6)
   , m_valid(true)
{
    static const double pi = 4.0 * atan(1.0);
    const double asq = -(s[3] + s[2] + s[1]);
    const double bsq = -(s[4] + s[2] + s[0]);
    const double csq = -(s[5] + s[1] + s[0]);
    const std::complex<double> a = std::sqrt(std::complex<double>(asq, 0.0));
    const std::complex<double> b = std::sqrt(std::complex<double>(bsq, 0.0));
    const std::complex<double> c = std::sqrt(std::complex<double>(csq, 0.0));

    const std::complex<double> cosal = s[0] / (b * c);
    const std::complex<double> cosbe = s[1] / (a * c);
    const std::complex<double> cosga = s[2] / (a * b);

    const std::complex<double> ac1 = std::acos(cosal);
    const std::complex<double> ac2 = std::acos(cosbe);
    const std::complex<double> ac3 = std::acos(cosga);



    const std::complex<double> al = std::acos(cosal) * 180.0 / pi;
    const std::complex<double >be = std::acos(cosbe) * 180.0 / pi;
    const std::complex<double> ga = std::acos(cosga) * 180.0 / pi;
}

C6::C6(const std::vector<double>& v)
   : m_dim(6)
   , m_valid(v.size() == 6)
{
   m_vec.resize(6);
   for (size_t i = 0; i < 6; ++i) { m_vec[i] = v[i]; }
}

C6::C6(const std::vector<std::complex<double> >& v)
   : m_dim(6)
   , m_valid(v.size() == 6)
   , m_vec(v)
{
}


C6 C6::operator+(const C6& c) {
   std::vector<std::complex<double> > v(6);
   for (size_t i = 0; i < 6; ++i) v[i] = (*this).m_vec[i] + c[i];
   return v;
}

C6 C6::operator-(const C6& c) {
   std::vector<std::complex<double> > v(6);
   for (size_t i = 0; i < 6; ++i) v[i] = (*this).m_vec[i] - c[i];
   return v;
}
