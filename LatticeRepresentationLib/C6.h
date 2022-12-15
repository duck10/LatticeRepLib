#ifndef C6_H
#define C6_H

#include "BasisBase.h"
#include "VecN.h"

#include <complex>
#include <vector>


class C6 {
public:
	C6();
	explicit C6(const C6& c6);
	explicit C6(const S6& s6);
	C6(const std::vector<double>& v);
	C6(const std::vector<std::complex<double> >& v);

	std::vector<std::complex<double> > GetVector() const { return m_vec; }

	size_t size() const { return m_dim; }
	bool GetValid() const { return m_valid; }

	C6 operator+(const C6&);
	C6 operator-(const C6&);

	std::complex<double> operator[](const size_t n) const{ return m_vec[n]; }

protected:
	std::vector<std::complex<double> >  m_vec;
	size_t m_dim;
	bool m_valid;

};

#endif // C6_H
