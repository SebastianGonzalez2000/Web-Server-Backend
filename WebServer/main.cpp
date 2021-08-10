#include "MultiClientChat.h"
#include "WebServer.h"
#include <iostream>

int main()
{
	std::cout << "Server is waiting for connections..." << std::endl;

	const char* ipAddr = "0.0.0.0";
	int port = 8080;

	WebServer webserver(ipAddr, port);

	if (webserver.init() != 0)
	{
		return -1;
	}

	webserver.run();

	return 0;
}