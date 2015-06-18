#ifndef WORKER_H
#define WORKER_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include <boost/asio.hpp>

class Worker {
public:
	void run();
	void spawn();
	void insertJob(boost::asio::ip::tcp::socket);
private:
	bool hasJobs() { return !_queue.empty(); }
	void waitForJobs();
	void processJob(boost::asio::ip::tcp::socket);

	bool				_running;
	std::thread			_thread;
	std::mutex			_queueEmptyLock;
	std::condition_variable		_queueEmptyCond;
	std::queue<boost::asio::ip::tcp::socket>	_queue;
};

#endif /* WORKER_H */
