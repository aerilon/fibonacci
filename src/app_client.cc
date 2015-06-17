#include <iostream>

#include "api.h"
#include "app_client.h"

using boost::asio::ip::tcp;

int
app_client::run()
{
	tcp::socket s(io_service);
	tcp::resolver resolver(io_service);

	boost::asio::connect(s, resolver.resolve({_host, _port}));

	fibonacci_api::request request(0, 3);

	boost::system::error_code error;
	size_t length;

	length = s.write_some(boost::asio::buffer(&request, sizeof request), error);
	if (error)
		throw boost::system::system_error(error);

	if (length != sizeof request)
		throw std::runtime_error("Unable to send full request");

	fibonacci_api::reply reply;

	length = s.read_some(boost::asio::buffer(&reply, sizeof reply), error);
	if (error)
		throw boost::system::system_error(error);

	if (length != sizeof reply)
		throw std::runtime_error("Invalid reply length");

	if (!fibonacci_api::verify_checksum(reply))
		throw std::runtime_error("Invalid reply checksum");

	return 0;
}


