#include <iostream>
#include <memory>
#include <string>

#include "app.h"
#include "app_client.h"
#include "app_server.h"

void
usage(bool fatal)
{
	std::cerr << "Usage: echo [host] <port>\n";
	if (fatal)
		exit(1);
}

void
usage_fatal()
{
	usage(true);
}

int main(int argc, char* argv[])
{
	std::unique_ptr<app> app;

	switch (argc) {
	case 2:
	{
		int port;

		try {
			port = std::stoi(argv[1]);
		} catch (std::exception& e) {
			std::cerr << "Invalid argument: " << argv[1] <<
				std::endl << std::endl;
			usage_fatal();
		}

		if (port < 0 || port > UINT16_MAX) {
			std::cerr << "Server port out-of-range: " << port <<
				std::endl << std::endl;
			usage_fatal();
		}

		std::unique_ptr<app_server> _server(new app_server(port));
		app = std::move(_server);
		break;
	}
	case 3:
	{
		std::unique_ptr<app_client> _client(new app_client(argv[1], argv[2]));
		app = std::move(_client);
		break;
	}
	default:
		usage_fatal();
		break;
	}

	try {
		app->run();
	} catch (std::exception& e) {
		std::cerr << e.what() << "\n";
	}


	return 0;
}
