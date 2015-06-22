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

	Job job(std::move(sock));

	/* Gracefully handle invalid checksum */
	if (!fibonacci_api::verify_checksum(query)) {
		job.error = fibonacci_api::ERR_INVALID_CHECKSUM;
		_gatherer.insertJob(std::move(job));
		return;
	}

	/* Check for query's upper limit bound */
	if (query.value > _backend->upper_limit) {
		job.error = fibonacci_api::ERR_OUT_OF_RANGE;
		_gatherer.insertJob(std::move(job));
		return;
	}

	job.query = query.value;

	_workers[_last_worker]->insertJob(std::move(job));

	_last_worker = (_last_worker + 1) % _workers.size();
}
