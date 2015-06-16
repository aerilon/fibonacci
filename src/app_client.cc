#include <iostream>

#include "app_client.h"

using boost::asio::ip::tcp;

int
app_client::run()
{

	std::size_t max_length = 1024;

	tcp::socket s(io_service);
	tcp::resolver resolver(io_service);

	boost::asio::connect(s, resolver.resolve({_host, _port}));

	std::cout << "Enter message: ";
	char request[max_length];

	std::cin.getline(request, max_length);
	size_t request_length = std::strlen(request);
	boost::asio::write(s, boost::asio::buffer(request, request_length));

	char reply[max_length];
	size_t reply_length = boost::asio::read(s, boost::asio::buffer(reply, request_length));
	std::cout << "Reply is: ";
	std::cout.write(reply, reply_length);
	std::cout << "\n";

	return 0;
}


