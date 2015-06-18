#include <stdexcept>

#include "api.h"
#include "worker.h"

using boost::asio::ip::tcp;

void
Worker::run()
{

	for (;;) {
		waitForJobs();
		tcp::socket sock = std::move(_queue.front());
		_queue.pop();

		try {
			processJob(std::move(sock));
		} catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

void
Worker::spawn()
{
	_running = true;
	_thread = std::thread(&Worker::run, this);
	_thread.detach();
}

void
Worker::insertJob(tcp::socket sock)
{
	bool notify = !hasJobs();

	_queue.push(std::move(sock));

	_queueEmptyCond.notify_all();
}

void
Worker::waitForJobs()
{

	if (hasJobs())
		return;

	std::unique_lock<std::mutex> lk(_queueEmptyLock);
	while (!hasJobs())
		_queueEmptyCond.wait(lk);
}

void
Worker::processJob(tcp::socket sock)
{
	boost::system::error_code error;
	size_t length;

	fibonacci_api::request request;

	length = sock.read_some(boost::asio::buffer(&request, sizeof request), error);
	if (error)
		throw boost::system::system_error(error);

	if (length != sizeof request)
		std::runtime_error("Unable to read full request");

#ifdef notyet
	// Don't expect to read any more data, shut down
	// receiving side
	sock.shutdown(boost::asio::ip::tcp::socket::shutdown_receive, error);
	if (error)
		throw boost::system::system_error(error);
#endif

	/* Gracefully handle invalid checksum */
	if (!fibonacci_api::verify_checksum(request)) {
		fibonacci_api::reply reply(
		    fibonacci_api::latest_version,
		    fibonacci_api::ERR_INVALID_CHECKSUM);

		length = sock.write_some(boost::asio::buffer(&reply, sizeof reply), error);
		if (error)
			throw boost::system::system_error(error);

		if (length != sizeof reply)
			std::runtime_error("Unable to write full reply");
	}

	uint64_t v = _backend->compute(request._request);

	fibonacci_api::reply reply(
	    fibonacci_api::latest_version,
	    v);

	sock.write_some(boost::asio::buffer(&reply, sizeof reply), error);
	if (error)
		throw boost::system::system_error(error);
}
