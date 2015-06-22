#include <iostream>

#include "AppClient.h"

using boost::asio::ip::tcp;

void
AppClient::sendQuery(uint8_t v)
{
	boost::system::error_code error;
	size_t length;

	fibonacci_api::query query(0, v);

	length = _sock.write_some(boost::asio::buffer(&query, sizeof query), error);
	if (error)
		throw boost::system::system_error(error);

	if (length != sizeof query)
		throw std::runtime_error("Unable to send full query");
}

fibonacci_api::reply
AppClient::receiveReply()
{
	boost::system::error_code error;
	size_t length;

	fibonacci_api::reply reply;

	length = _sock.read_some(boost::asio::buffer(&reply, sizeof reply), error);
	if (error)
		throw boost::system::system_error(error);

	if (length != sizeof reply)
		throw std::runtime_error("Invalid reply length");

	return reply;
}

void
AppClient::processReply(fibonacci_api::reply reply)
{

	if (!fibonacci_api::verify_checksum(reply))
		throw std::runtime_error("Invalid reply checksum");

	if (!fibonacci_api::reply_is_valid(reply))
		throw std::runtime_error("Computation failed");

	std::cout << reply.value << std::endl;
}

bool
AppClient::processOneLine(std::string line)
{
	int v;

	try {
		v = std::stoi(line);
	} catch (std::exception& e) {
		return false;
	}

	/* Ensure the requested value will fit in the request */
	if (v < 0 || v > UINT8_MAX)
		return false;

	sendQuery(v);

	processReply(receiveReply());

	return true;
}

int
AppClient::run()
{
	std::string line;

	tcp::resolver resolver(_io_service);

	boost::asio::connect(_sock, resolver.resolve({_host, _port}));

	while (std::getline(std::cin, line)) {
		if (!processOneLine(line))
			break;
	}

	return 0;
}
