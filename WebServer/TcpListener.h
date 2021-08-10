#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ws2tcpip.h>
#include <sstream>

#pragma comment (lib, "ws2_32.lib")

class TcpListener
{

public:

	TcpListener(const char* ipAddr, int port) :
		m_ipAddr(ipAddr), m_port(port) { }

	int init();

	int run();

protected:

	virtual void onClientConnected(int clientSocket);

	virtual void onClientDisconnected(int clientSocket);

	virtual void onMessageReceived(int clientSocket, const char* msg, int length);

	void sendToClient(int clientSocket, const char* msg, int length);

	void broadcastToClients(int sendingClient, const char* msg, int length);

private:

	const char* m_ipAddr;
	int m_port;
	int m_socket;
	fd_set m_master;

};

