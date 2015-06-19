#include <iostream>
#include <memory>
#include <string>

#include "App.h"
#include "AppClient.h"
#include "AppServer.h"

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
	std::unique_ptr<App> App;

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

		std::unique_ptr<AppServer> _server(new AppServer(port));
		App = std::move(_server);
		break;
	}
	case 3:
	{
		std::unique_ptr<AppClient> _client(new AppClient(argv[1], argv[2]));
		App = std::move(_client);
		break;
	}
	default:
		usage_fatal();
		break;
	}

	try {
		App->run();
	} catch (std::exception& e) {
		std::cerr << e.what() << "\n";
	}


	return 0;
}
