#ifndef GENERIC_WORK_QUEUE_H
#define GENERIC_WORK_QUEUE_H

#include "config.h"

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif

template <typename T>
class GenericWorkQueue {
public:
	GenericWorkQueue(std::string thread_name) :
		_njobs(0),
		_thread_name(thread_name)
	{
	}
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
	std::mutex			_queueLock;
	std::queue<T>			_queue;

	uint64_t			_njobs;

	std::string			_thread_name;
};

template <typename T>
void
GenericWorkQueue<T>::run()
{

#ifdef HAVE_PTHREAD_H
	if (_thread_name.length() != 0)
		pthread_setname_np(pthread_self(), _thread_name.c_str());
#endif

	for (;;) {
		waitForJobs();
		T t = std::move(_queue.front());
		_queue.pop();

		try {
			_njobs++;
			processJob(std::move(t));
		} catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

template <typename T>
void
GenericWorkQueue<T>::spawn()
{
	_running = true;
	_thread = std::thread(&GenericWorkQueue::run, this);
	_thread.detach();
}


template <typename T>
void
GenericWorkQueue<T>::insertJob(T t)
{

	{
		std::lock_guard<std::mutex> lock(_queueLock);
		_queue.push(std::move(t));
	}

	_queueEmptyCond.notify_all();
}

template <typename T>
void
GenericWorkQueue<T>::waitForJobs()
{

	if (hasJobs())
		return;

	std::unique_lock<std::mutex> lk(_queueEmptyLock);
	while (!hasJobs())
		_queueEmptyCond.wait(lk);
}

#endif // GENERIC_WORK_QUEUE_H
