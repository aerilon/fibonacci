#include "api.h"
#include "JobGatherer.h"
#include "JobScatterer.h"

using boost::asio::ip::tcp;

void
JobGatherer::processJob(struct Job job)
{

	if (job.error != fibonacci_api::ERR_OK) {
		fibonacci_api::reply reply(
		    fibonacci_api::latest_version,
		    job.error);

		postReply(std::move(job.sock),
		    std::move(reply));
	} else {
		fibonacci_api::reply reply(
		    fibonacci_api::latest_version,
		    job.reply);

		postReply(std::move(job.sock),
		    std::move(reply));
	}
}

void
JobGatherer::postReply(tcp::socket sock, fibonacci_api::reply reply)
{
	boost::system::error_code error;

	sock.write_some(boost::asio::buffer(&reply, sizeof reply), error);
	if (error)
		throw boost::system::system_error(error);

	if (reply.error == fibonacci_api::ERR_OK)
		_scatterer->insertJob(std::move(sock));
}
