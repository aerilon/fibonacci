#include <iostream>
#include <thread>

#include <boost/asio.hpp>

#include "api.h"

#include "app_server.h"

using boost::asio::ip::tcp;

void session(tcp::socket sock)
{
	try
	{
		boost::system::error_code error;
		size_t length;

		fibonacci_api::request request;

		length = sock.read_some(boost::asio::buffer(&request, sizeof request), error);
		if (error)
			throw boost::system::system_error(error);

		if (length != sizeof request)
			std::runtime_error("Unable to read full request");

#ifdef notyet
		// Don't expect to read any more data, shut down
		// receiving side
		sock.shutdown(boost::asio::ip::tcp::socket::shutdown_receive, error);
		if (error)
			throw boost::system::system_error(error);
#endif

		/* Gracefully handle invalid checksum */
		if (!fibonacci_api::verify_checksum(request)) {
			fibonacci_api::reply reply(
			    fibonacci_api::latest_version,
			    fibonacci_api::ERR_INVALID_CHECKSUM);

			length = sock.write_some(boost::asio::buffer(&reply, sizeof reply), error);
			if (error)
				throw boost::system::system_error(error);

			if (length != sizeof reply)
				std::runtime_error("Unable to write full reply");
		}


		fibonacci_api::reply reply(
		    fibonacci_api::latest_version,
		    (uint64_t)42);

		sock.write_some(boost::asio::buffer(&reply, sizeof reply), error);
		if (error)
			throw boost::system::system_error(error);
	} catch (std::exception& e) {
		std::cerr << "Exception caught in thread: " << e.what() << "\n";
	}
}

int
app_server::run()
{
	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), _port));

	for (;;) {
		tcp::socket sock(io_service);
		acceptor.accept(sock);

		std::thread(session, std::move(sock)).detach();
	}
	return 0;
}


