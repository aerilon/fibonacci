#ifndef JOB_SCATTERER_H
#define JOB_SCATTERER_H

#include <boost/asio.hpp>

#include "GenericWorkQueue.h"
#include "Job.h"
#include "JobGatherer.h"
#include "Worker.h"

class JobScatterer : public GenericWorkQueue<boost::asio::ip::tcp::socket>
{
public:
	JobScatterer(std::shared_ptr<Backend> backend) :
		_gatherer(this),
		_last_worker(0)
	{
		unsigned nthread = std::thread::hardware_concurrency();
		for (auto i = 0; i < nthread; i++)
			_workers.push_back(std::unique_ptr<Worker>(new Worker(backend, &_gatherer)));
	}

	void bootstrap();
private:
	void processJob(boost::asio::ip::tcp::socket);

	std::vector<std::unique_ptr<Worker>>	_workers;
	size_t		_last_worker;
	JobGatherer	_gatherer;
};

#endif // JOB_SCATTERER_H
