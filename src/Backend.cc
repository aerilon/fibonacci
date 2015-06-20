#include <boost/numeric/ublas/io.hpp>

#include "Backend.h"

uint64_t
Backend::compute(uint8_t n)
{
	if (n == 0)
		return 0;

	boost::numeric::ublas::matrix<uint64_t> p = _m;

	for (auto i = 0; i < (n - 2); i++)
		p = boost::numeric::ublas::prod(p, _m);

	return p(0, 0);
}
