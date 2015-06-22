#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello

#include <boost/test/unit_test.hpp>

#include "api.h"

BOOST_AUTO_TEST_CASE(fibonacciQuery)
{
	fibonacci_api::query query(fibonacci_api::latest_version, 42);

	BOOST_CHECK(query.version == fibonacci_api::latest_version);

	BOOST_CHECK(query.value == 42);

	BOOST_CHECK(query.checksum == 0x9a62db29);
}

BOOST_AUTO_TEST_CASE(fibonacciReplyOk)
{
	fibonacci_api::reply reply(fibonacci_api::latest_version, (fibonacci_api::reply_t)42);

	BOOST_CHECK(reply.version == fibonacci_api::latest_version);

	BOOST_CHECK(reply.error == fibonacci_api::ERR_OK);

	BOOST_CHECK(reply.value == 42);

	BOOST_CHECK(reply.checksum == 0x8b3c16e8);
}

BOOST_AUTO_TEST_CASE(fibonacciReplyError)
{
	fibonacci_api::reply reply(fibonacci_api::latest_version, fibonacci_api::ERR_VERSION_MISMATCH);

	BOOST_CHECK(reply.error == fibonacci_api::ERR_VERSION_MISMATCH);

	BOOST_CHECK(reply.checksum == 0xf4f17c35);
}
