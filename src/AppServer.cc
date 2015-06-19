#include <thread>

#include <boost/asio.hpp>

#include "api.h"
#include "AppServer.h"
#include "Backend.h"

#include "JobScatterer.h"

#include "Worker.h"

using boost::asio::ip::tcp;

#include <chrono>

int
AppServer::run()
{
	std::shared_ptr<Backend> backend(new Backend);

	JobScatterer scatterer(backend);

	scatterer.bootstrap();

	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), _port));
	for (;;) {
		tcp::socket sock(io_service);
		acceptor.accept(sock);

		scatterer.insertJob(std::move(sock));
	}

	return 0;
}
