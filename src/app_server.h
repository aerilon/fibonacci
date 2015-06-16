#ifndef APP_SERVER_H
#define APP_SERVER_H

#include <cstddef>
#include <cstdint>

#include "app.h"

class app_server : public app
{
public:
	app_server(const uint16_t port) :
	    _port(port)
	{
	}

	int run();
private:
	const uint16_t _port;
};

#endif // APP_SERVER_H
