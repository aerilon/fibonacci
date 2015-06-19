#include "api.h"
#include "JobGatherer.h"
#include "JobScatterer.h"

using boost::asio::ip::tcp;

void
JobGatherer::processJob(struct Job job)
{
	boost::system::error_code error;

	fibonacci_api::reply reply(
	    fibonacci_api::latest_version,
	    job.reply);

	tcp::socket sock = std::move(job.sock);

	sock.write_some(boost::asio::buffer(&reply, sizeof reply), error);
	if (error)
		throw boost::system::system_error(error);

	_scatterer->insertJob(std::move(sock));
}
