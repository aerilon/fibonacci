#include <boost/numeric/ublas/io.hpp>

#include "Backend.h"

uint64_t
Backend::compute(uint8_t n)
{
	if (n == 0)
		return 0;

	boost::numeric::ublas::matrix<boost::multiprecision::uint1024_t> p = _m;

	for (auto i = 0; i < (n - 2); i++)
		p = boost::numeric::ublas::prod(p, _m);

	return static_cast<uint64_t>(p(0, 0));
}
