#ifndef APP_CLIENT_H
#define APP_CLIENT_H

#include <cstddef>
#include <cstdint>

#include "api.h"
#include "App.h"

class AppClient : public App
{
public:
	AppClient(const char *host, const char *port) :
	    _host(host),
	    _port(port),
	    _sock(_io_service)
	{
	}

	int run();
private:
	bool processOneLine(std::string);

	void			sendQuery(uint8_t);
	fibonacci_api::reply	receiveReply();
	void			processReply(fibonacci_api::reply);

	const char *_host;
	const char *_port;
	boost::asio::ip::tcp::socket _sock;
};

#endif // APP_CLIENT_H
