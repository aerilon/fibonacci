#include <stdexcept>

#include "api.h"
#include "worker.h"
#include "JobGatherer.h"

using boost::asio::ip::tcp;

void
Worker::processJob(struct Job job)
{
	job._reply = _backend->compute(job._request);

	_gatherer->insertJob(std::move(job));
}
