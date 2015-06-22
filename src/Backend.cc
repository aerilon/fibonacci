#include <boost/numeric/ublas/io.hpp>

#include "Backend.h"

uint64_t
Backend::compute(uint8_t n)
{
	boost::multiprecision::uint1024_t v;

	{
		std::lock_guard<std::mutex> lk(_mapLock);

		auto it = _map.find(n);
		if (it != _map.end())
			return static_cast<uint64_t>(it->second);
	}

	v = compute_slow(n);

	{
		std::lock_guard<std::mutex> lk(_mapLock);
		_map[n] = v;
	}

	return static_cast<uint64_t>(v);
}

boost::multiprecision::uint1024_t
Backend::compute_slow(uint8_t n)
{
	boost::numeric::ublas::matrix<boost::multiprecision::uint1024_t> p = _m;

	for (auto i = 0; i < n; i++)
		p = boost::numeric::ublas::prod(p, _m);

	return p(1, 1);
}
