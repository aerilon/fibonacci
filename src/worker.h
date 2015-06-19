#ifndef WORKER_H
#define WORKER_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include <boost/asio.hpp>

#include "backend.h"

template <typename T>
class GenericWorker {
public:
	void run();
	void spawn();
	void insertJob(T);
private:
	bool hasJobs() { return !_queue.empty(); }
	void waitForJobs();
	virtual void processJob(T) = 0;

	bool				_running;
	std::thread			_thread;
	std::mutex			_queueEmptyLock;
	std::condition_variable		_queueEmptyCond;
	std::queue<T>			_queue;
};

template <typename T>
void
GenericWorker<T>::run()
{
	std::cout << __func__ << std::endl;

	for (;;) {
		waitForJobs();
		T t = std::move(_queue.front());
		_queue.pop();

		try {
			processJob(std::move(t));
		} catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

template <typename T>
void
GenericWorker<T>::spawn()
{
	_running = true;
	_thread = std::thread(&GenericWorker::run, this);
	_thread.detach();
}


template <typename T>
void
GenericWorker<T>::insertJob(T t)
{
	bool notify = !hasJobs();

	_queue.push(std::move(t));

	_queueEmptyCond.notify_all();
}

template <typename T>
void
GenericWorker<T>::waitForJobs()
{

	if (hasJobs())
		return;

	std::unique_lock<std::mutex> lk(_queueEmptyLock);
	while (!hasJobs())
		_queueEmptyCond.wait(lk);
}

class Worker : public GenericWorker<boost::asio::ip::tcp::socket>
{
public:
	Worker(std::shared_ptr<Backend> backend) :
		_backend(backend)
	{
	}

private:
	void processJob(boost::asio::ip::tcp::socket);
	std::shared_ptr<Backend>	_backend;
};

#endif /* WORKER_H */
