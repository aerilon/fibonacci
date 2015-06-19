#include <thread>

#include <boost/asio.hpp>

#include "api.h"
#include "app_server.h"
#include "backend.h"

#include "JobScatterer.h"

#include "worker.h"

using boost::asio::ip::tcp;

int
app_server::run()
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
