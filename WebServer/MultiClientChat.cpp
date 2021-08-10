#include "MultiClientChat.h"
#include <string>


void MultiClientChat::onClientConnected(int clientSocket)
{
	std::string welcomeMsg = "Welcome to the Chat Server.\r\n";
	sendToClient(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1);
}

void MultiClientChat::onClientDisconnected(int clientSocket)
{

}

void MultiClientChat::onMessageReceived(int clientSocket, const char* msg, int length)
{
	broadcastToClients(clientSocket, msg, length);
}
