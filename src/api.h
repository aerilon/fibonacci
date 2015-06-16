#ifndef API_H
#define API_H

#include <boost/crc.hpp>

namespace fibonacci_api {

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
	uint8_t		_version;
	uint8_t 	_request;
	uint32_t	_checksum;
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
	uint8_t		_version;
	uint8_t		_error;
	uint64_t	_reply;
	uint32_t	_checksum;
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

