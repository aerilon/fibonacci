#ifndef APP_H
#define APP_H

#include <stdexcept>

#include <boost/asio.hpp>

class App
{
public:
	App()
	{
	}

	virtual int run() = 0;

protected:
	boost::asio::io_service _io_service;
};

#endif // APP_H
