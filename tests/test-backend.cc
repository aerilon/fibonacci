#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello

#include <boost/test/unit_test.hpp>

#include "Backend.h"

#include <iostream>

BOOST_AUTO_TEST_CASE(backendBasicCompute)
{
	Backend b;

	BOOST_CHECK(b.compute(42) == 267914296);

	BOOST_CHECK(b.compute(64) == 10610209857723ULL);
}

BOOST_AUTO_TEST_CASE(backendUpperLimit)
{
	Backend b;

	BOOST_CHECK_NO_THROW(b.compute(b.upper_limit));
}

BOOST_AUTO_TEST_CASE(backendThrowOutOfRange)
{
	Backend b;

	BOOST_CHECK_THROW(b.compute(b.upper_limit + 1), std::runtime_error);
}
