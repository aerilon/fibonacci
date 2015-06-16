#include <iostream>
#include <memory>

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

int main(int argc, char* argv[])
{
	std::unique_ptr<app> app;

	switch (argc) {
	case 2:
	{
		std::unique_ptr<app_server> _server(new app_server(1111));
		app = std::move(_server);
		break;
	}
	case 3:
	{
		std::unique_ptr<app_client> _client(new app_client("127.0.0.1", "1111"));
		app = std::move(_client);
		break;
	}
	default:
		usage(true);
		break;
	}

	try {
		app->run();
	} catch (std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << "\n";
	}


	return 0;
}
