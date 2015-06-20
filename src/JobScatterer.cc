#include "api.h"
#include "JobScatterer.h"

using boost::asio::ip::tcp;

void
JobScatterer::bootstrap()
{

	/* Spawn our workers */
	for (auto it = _workers.begin(); it != _workers.end(); it++)
		(*it)->spawn();

	/* Spawn our associated gatherer */
	_gatherer.spawn();

	/* Spawn our thread */
	this->spawn();
}

void
JobScatterer::processJob(tcp::socket sock)
{
	boost::system::error_code error;
	size_t length;

	fibonacci_api::query query;

	length = sock.read_some(boost::asio::buffer(&query, sizeof query), error);
	if (error)
		throw boost::system::system_error(error);

	if (length != sizeof query)
		std::runtime_error("Unable to read full query");

#if notyet
	/* Gracefully handle invalid checksum */
	if (!fibonacci_api::verify_checksum(query)) {
		fibonacci_api::reply reply(
		    fibonacci_api::latest_version,
		    fibonacci_api::ERR_INVALID_CHECKSUM);

		length = sock.write_some(boost::asio::buffer(&reply, sizeof reply), error);
		if (error)
			throw boost::system::system_error(error);

		if (length != sizeof reply)
			std::runtime_error("Unable to write full reply");
	}

	if (query.value > _backend->upper_limit) {
		fibonacci_api::reply reply(
		    fibonacci_api::latest_version,
		    fibonacci_api::ERR_OUT_OF_RANGE);

		length = sock.write_some(boost::asio::buffer(&reply, sizeof reply), error);
		if (error)
			throw boost::system::system_error(error);

		if (length != sizeof reply)
			std::runtime_error("Unable to write full reply");
	}
#endif

	Job job(std::move(sock), query.value);

	_workers[_last_worker]->insertJob(std::move(job));

	_last_worker = (_last_worker + 1) % _workers.size();
}
