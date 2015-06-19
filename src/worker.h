#ifndef WORKER_H
#define WORKER_H

#include <boost/asio.hpp>

#include "backend.h"

#include "GenericWorkQueue.h"

#include "Job.h"

/*
 *
 */
class JobGatherer;

class Worker : public GenericWorkQueue<struct Job>
{
public:
	Worker(std::shared_ptr<Backend> backend, JobGatherer *gatherer) :
		_backend(backend),
		_gatherer(gatherer)
	{
	}

private:
	void processJob(struct Job);
	std::shared_ptr<Backend>	_backend;
	JobGatherer *			_gatherer;
};

#endif /* WORKER_H */
