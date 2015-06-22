#ifndef API_H
#define API_H

#include <boost/crc.hpp>

namespace fibonacci_api {

typedef uint8_t		version_t;
typedef uint8_t		error_t;
typedef uint8_t		query_t;
typedef uint64_t	reply_t;
typedef uint32_t	checksum_t;

const version_t latest_version = 0;

const error_t ERR_OK = 0;
const error_t ERR_VERSION_MISMATCH = 1;
const error_t ERR_INVALID_CHECKSUM = 2;
const error_t ERR_OUT_OF_RANGE = 3;

template <typename T>
uint32_t compute_checksum(T *r) {
	boost::crc_32_type crc32;

	crc32.process_bytes(r, sizeof (*r) - sizeof (r->checksum));

	return crc32.checksum();
};

template <typename T>
bool verify_checksum(T& r)
{

	return r.checksum == compute_checksum(&r);
};

/*
 * FIXME -
 * The reply's value, as well as the reply's and query's checksum are
 * multi-byte value whose encoding depends on the host endianness. As-is,
 * communication between host of different endianness is broken.
 */
struct query
{
	version_t	version;
	query_t 	value;
	checksum_t	checksum;
	query ()
	{}
	query (uint8_t version, uint8_t value) :
	    version(version),
	    value(value)
	{

		checksum = compute_checksum(this);
	};
} __attribute__ ((packed));

struct reply
{
	version_t	version;
	error_t		error;
	reply_t		value;
	checksum_t	checksum;
	reply ()
	{}
	reply (version_t version, error_t error) :
	    version(version),
	    error(error),
	    value(0)
	{
		checksum = compute_checksum(this);
	}
	reply (version_t version, reply_t value) :
	    version(version),
	    error(ERR_OK),
	    value(value)
	{
		checksum = compute_checksum(this);
	};
} __attribute__ ((packed));

void dump(struct query &);

static inline bool
reply_is_valid(reply r)
{
	return (r.error == ERR_OK);
}

}

#endif // API_H
