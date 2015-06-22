#ifndef BACKEND_H
#define BACKEND_H

#include <cstdint>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/numeric/ublas/matrix.hpp>

class Backend
{
public:
	Backend () :
		_m(2, 2)
	{
	        _m(0, 0) = 1;
	        _m(0, 1) = 1;
	        _m(1, 0) = 1;
	        _m(1, 1) = 0;
	}
	uint64_t compute(uint8_t);

	// Inclusive implementation upper-limit
	const uint16_t upper_limit = 93;
private:
	boost::numeric::ublas::matrix<boost::multiprecision::uint1024_t> _m;
};

#endif /* BACKEND_H */
