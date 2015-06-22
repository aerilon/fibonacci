#ifndef BACKEND_H
#define BACKEND_H

#include <cstdint>

#include <map>
#include <mutex>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "api.h"

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
	fibonacci_api::reply_t compute(fibonacci_api::query_t);

	// Inclusive implementation upper-limit
	const unsigned int upper_limit = 93;
private:
	boost::multiprecision::uint1024_t compute_slow(fibonacci_api::query_t);

	std::mutex	_mapLock;
	std::map<fibonacci_api::query_t, boost::multiprecision::uint1024_t>	_map;
	boost::numeric::ublas::matrix<boost::multiprecision::uint1024_t> _m;
};

#endif /* BACKEND_H */
