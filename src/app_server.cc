#include <iostream>
#include <thread>

#include <boost/asio.hpp>

#include "app_server.h"

using boost::asio::ip::tcp;

void session(tcp::socket sock)
{
	try
	{
		size_t max_length = 1024;
		for (;;) {
			char data[max_length];

			boost::system::error_code error;
			size_t length = sock.read_some(boost::asio::buffer(data, max_length), error);
			if (error == boost::asio::error::eof) {
				// Connection closed cleanly by peer.
				break;
			} else if (error) {
				// Some other error.
				throw boost::system::system_error(error);
			}

			boost::asio::write(sock, boost::asio::buffer(data, length));
		}
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


