#ifndef APP_SERVER_H
#define APP_SERVER_H

#include <cstddef>
#include <cstdint>

#include "App.h"

class AppServer : public App
{
public:
	AppServer(const uint16_t port) :
	    _port(port)
	{
	}

	int run();
private:
	const uint16_t _port;
};

#endif // APP_SERVER_H
