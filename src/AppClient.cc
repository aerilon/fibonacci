#include <iostream>

#include "api.h"
#include "AppClient.h"

using boost::asio::ip::tcp;

bool
AppClient::process_one_line(std::string line)
{
	int v;

	std::cout << line << std::endl;

	try {
		v = std::stoi(line);
	} catch (std::exception& e) {
		return false;
	}

	/* Ensure the requested value will fit in the request */
	if (v < 0 || v > UINT8_MAX)
		return false;

	fibonacci_api::query query(0, v);

	boost::system::error_code error;
	size_t length;

	length = _sock.write_some(boost::asio::buffer(&query, sizeof query), error);
	if (error)
		throw boost::system::system_error(error);

	if (length != sizeof query)
		throw std::runtime_error("Unable to send full query");

	fibonacci_api::reply reply;

	length = _sock.read_some(boost::asio::buffer(&reply, sizeof reply), error);
	if (error)
		throw boost::system::system_error(error);

	if (length != sizeof reply)
		throw std::runtime_error("Invalid reply length");

	if (!fibonacci_api::verify_checksum(reply))
		throw std::runtime_error("Invalid reply checksum");

	if (!fibonacci_api::reply_is_valid(reply))
		throw std::runtime_error("Computation failed");

	std::cout << reply.value << std::endl;

	return true;
}

int
AppClient::run()
{
	std::string line;

	tcp::resolver resolver(_io_service);

	boost::asio::connect(_sock, resolver.resolve({_host, _port}));

	while (std::getline(std::cin, line)) {
		if (!process_one_line(line))
			break;
	}

	return 0;
}
