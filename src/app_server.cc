#include <thread>

#include <boost/asio.hpp>

#include "api.h"
#include "app_server.h"
#include "worker.h"

using boost::asio::ip::tcp;



int
app_server::run()
{
	std::vector<std::unique_ptr<Worker>> workers;
	unsigned nthread = std::thread::hardware_concurrency();

	if (nthread == 0)
		nthread = 1;

	for (auto i = 0; i < nthread; i++)
		workers.push_back(std::unique_ptr<Worker>(new Worker));

	for (auto i = 0; i < nthread; i++)
		(workers[i])->spawn();

	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), _port));
	auto i = 0;

	for (;;) {
		tcp::socket sock(io_service);
		acceptor.accept(sock);

		(workers[i])->insertJob(std::move(sock));

		i = (i + 1) % nthread;
	}

	return 0;
}
