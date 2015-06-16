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
	    _port(port)
	{
	}

	int run();
private:
	const char *_host;
	const char *_port;
};

#endif // APP_CLIENT_H
