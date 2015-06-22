#include <boost/numeric/ublas/io.hpp>

#include "Backend.h"

uint64_t
Backend::compute(uint8_t n)
{
	boost::numeric::ublas::matrix<boost::multiprecision::uint1024_t> p = _m;

	for (auto i = 0; i < n; i++)
		p = boost::numeric::ublas::prod(p, _m);

	return static_cast<uint64_t>(p(1, 1));
}
