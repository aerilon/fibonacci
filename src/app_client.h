#ifndef APP_CLIENT_H
#define APP_CLIENT_H

#include <cstddef>
#include <cstdint>

#include "app.h"

class app_client : public app
{
public:
	app_client(const char *host, const char *port) :
	    _host(host),
	    _port(port),
	    _sock(io_service)
	{
	}

	int run();
private:
	bool process_one_line(std::string);

	const char *_host;
	const char *_port;
	boost::asio::ip::tcp::socket _sock;
};

#endif // APP_CLIENT_H
