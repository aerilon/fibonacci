#ifndef JOB_H
#define JOB_H

#include <boost/asio.hpp>

struct Job
{
	Job(boost::asio::ip::tcp::socket sock, uint8_t request) :
		_sock(std::move(sock)),
		_request(request)
	{
	}
	boost::asio::ip::tcp::socket _sock;
	uint8_t		_request;
	uint64_t	_reply;
};

#endif // JOB_H
