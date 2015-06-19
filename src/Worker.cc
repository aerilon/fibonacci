#include <stdexcept>

#include "api.h"
#include "Worker.h"
#include "JobGatherer.h"

using boost::asio::ip::tcp;

void
Worker::processJob(struct Job job)
{
	job.reply = _backend->compute(job.query);

	_gatherer->insertJob(std::move(job));
}
