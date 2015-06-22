#include <boost/numeric/ublas/io.hpp>

#include "Backend.h"

fibonacci_api::reply_t
Backend::compute(fibonacci_api::query_t n)
{
	boost::multiprecision::uint1024_t v;

	{
		std::lock_guard<std::mutex> lk(_mapLock);

		auto it = _map.find(n);
		if (it != _map.end())
			return static_cast<fibonacci_api::reply_t>(it->second);
	}

	v = compute_slow(n);

	{
		std::lock_guard<std::mutex> lk(_mapLock);
		_map[n] = v;
	}

	return static_cast<fibonacci_api::reply_t>(v);
}

boost::multiprecision::uint1024_t
Backend::compute_slow(fibonacci_api::query_t n)
{
	boost::numeric::ublas::matrix<boost::multiprecision::uint1024_t> p = _m;

	for (auto i = 0; i < n; i++)
		p = boost::numeric::ublas::prod(p, _m);

	return p(1, 1);
}
