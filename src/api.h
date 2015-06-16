#ifndef API_H
#define API_H

#include <boost/crc.hpp>

namespace fibonacci_api {

typedef uint8_t		version_t;
typedef uint8_t		error_t;
typedef uint8_t		request_t;
typedef uint64_t	reply_t;
typedef uint32_t	checksum_t;

const version_t latest_version = 0;

const error_t ERR_OK = 0;
const error_t ERR_VERSION_MISMATCH = 1;
const error_t ERR_INVALID_CHECKSUM = 2;

template <typename T>
uint32_t compute_checksum(T *r) {
	boost::crc_32_type crc32;

	crc32.process_bytes(r, sizeof (*r) - sizeof (r->_checksum));

	return crc32.checksum();
};

template <typename T>
bool verify_checksum(T& r)
{

	return r._checksum == compute_checksum(&r);
};

struct request
{
	version_t	_version;
	request_t 	_request;
	checksum_t	_checksum;
	request ()
	{}
	request (uint8_t version, uint8_t request) :
	    _version(version),
	    _request(request)
	{

		_checksum = compute_checksum(this);
	};
} __attribute__ ((packed));

struct reply
{
	version_t	_version;
	error_t		_error;
	reply_t		_reply;
	checksum_t	_checksum;
	reply ()
	{}
	reply (uint8_t version, error_t error) :
	    _version(version),
	    _error(error),
	    _reply(0)
	{
		_checksum = compute_checksum(this);
	}
	reply (uint8_t version, uint64_t reply) :
	    _version(version),
	    _error(ERR_OK),
	    _reply(reply)
	{
		_checksum = compute_checksum(this);
	};
} __attribute__ ((packed));

void dump(struct request &);

}

#endif /* API_H */

