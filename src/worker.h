#ifndef WORKER_H
#define WORKER_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include <boost/asio.hpp>

#include "backend.h"

class Worker {
public:
	Worker(std::shared_ptr<Backend> backend) :
		_backend(backend)
	{
	}
	void run();
	void spawn();
	void insertJob(boost::asio::ip::tcp::socket);
private:
	bool hasJobs() { return !_queue.empty(); }
	void waitForJobs();
	void processJob(boost::asio::ip::tcp::socket);

	std::shared_ptr<Backend>	_backend;

	bool				_running;
	std::thread			_thread;
	std::mutex			_queueEmptyLock;
	std::condition_variable		_queueEmptyCond;
	std::queue<boost::asio::ip::tcp::socket>	_queue;
};

#endif /* WORKER_H */
