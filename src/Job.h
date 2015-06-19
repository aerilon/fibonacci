#ifndef JOB_H
#define JOB_H

#include <boost/asio.hpp>

struct Job
{
	Job(boost::asio::ip::tcp::socket sock, uint8_t query) :
		sock(std::move(sock)),
		query(query)
	{
	}
	boost::asio::ip::tcp::socket sock;
	uint8_t		query;
	uint64_t	reply;
};

#endif // JOB_H
