#pragma once

#include "TcpListener.h"

class MultiClientChat : public TcpListener
{

public:

	MultiClientChat(const char* ipAddr, int port) :
		TcpListener(ipAddr, port) { }

protected:
	virtual void onClientConnected(int clientSocket);

	virtual void onClientDisconnected(int clientSocket);

	virtual void onMessageReceived(int clientSocket, const char* msg, int length);

};

