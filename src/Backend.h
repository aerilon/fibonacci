#ifndef BACKEND_H
#define BACKEND_H

#include <cstdint>

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
private:
	boost::numeric::ublas::matrix<uint64_t> _m;
};

#endif /* BACKEND_H */
