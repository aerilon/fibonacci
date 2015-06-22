#ifndef JOB_GATHERER_H
#define JOB_GATHERER_H

#include "GenericWorkQueue.h"
#include "Job.h"

class JobScatterer;

class JobGatherer : public GenericWorkQueue<struct Job>
{
public:
	JobGatherer(JobScatterer *scatterer) :
		_scatterer(scatterer)
	{
	}

private:
	void processJob(struct Job);
	void postReply(boost::asio::ip::tcp::socket, fibonacci_api::reply);
	JobScatterer *_scatterer;
};

#endif // JOB_GATHERER_H
