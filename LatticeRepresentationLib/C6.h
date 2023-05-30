#ifndef C6_H
#define C6_H

#include "BasisBase.h"
#include "S6.h"
#include "VecN.h"

#include <complex>
#include <vector>


class C6 {
public:
	friend std::ostream& operator<< (std::ostream&, const C6&);


	C6();
	explicit C6(const C6& c6);
	explicit C6(const S6& s6);
	C6(const std::vector<double>& v);
	C6(const std::vector<std::complex<double> >& v);
	std::complex<double> c3_s1(const std::complex<double>& a);

	std::vector<std::complex<double> > GetVector() const { return m_vec; }

	size_t size() const { return m_dim; }
	bool GetValid() const { return m_valid; }

	C6 operator+(const C6&);
	C6 operator-(const C6&);

	std::complex<double> operator[](const size_t n) const { return m_vec[n]; }
	std::complex<double>& operator[](const size_t n)  { return m_vec[n]; }

	operator S6() const;

protected:
	std::vector<std::complex<double> >  m_vec;
	size_t m_dim;
	bool m_valid;
	S6 m_s6Input;
	bool m_hasS6Input;

};

#endif // C6_H
