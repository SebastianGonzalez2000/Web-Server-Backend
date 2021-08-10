#include <string>
#include <istream>
#include <iostream>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <iterator>
#include <vector>

#include "WebServer.h"

void WebServer::onMessageReceived(int clientSocket, const char* msg, int length)
{
	// Format of Web Browser requets to server
	// GET /index.html HTTP/1.1

	std::istringstream iss(msg);
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	

	std::string content = "<h1>404 Not Found</h1>";
	std::string htmlFile = "/index.html";
	int errorCode = 404;

	

	if (parsed.size() >= 3 && parsed[0] == "GET")
	{
		htmlFile = parsed[1];

		if (htmlFile == "/")
		{
			htmlFile = "/index.html";
		}
	}


	std::ifstream f(".\\wwwroot\\" + htmlFile);

	if (f.good())
	{

		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());			content = str;
		errorCode = 200;
	}

	f.close();
	

	


	std::ostringstream oss;
	oss << "HTTP/1.1 " << errorCode << " OK\r\n";
	oss << "Cache-Control: no-cache, private\r\n";
	oss << "Content-Type: text/html\r\n";
	oss << "Content-Length: " << content.size() << "\r\n";
	oss << "\r\n";
	oss << content;

	
	std::string output = oss.str();
	int size = output.size() + 1;

	sendToClient(clientSocket, output.c_str(), size);
}

void WebServer::onClientConnected(int clientSocket)
{

}

void WebServer::onClientDisconnected(int clientSocket)
{

}
