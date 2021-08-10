#include "TcpListener.h"

int TcpListener::init()
{
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		std::cerr << "Cannot initialize winsock... Quitting..." << std::endl;
		return wsOk;
	}

	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		std::cerr << "Cannot create socket... Quitting..." << std::endl;
		return WSAGetLastError();
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(m_port);
	inet_pton(AF_INET, m_ipAddr, &hint.sin_addr);

	if (bind(m_socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}


	FD_ZERO(&m_master);

	FD_SET(m_socket, &m_master);

	return 0;
}

int TcpListener::run()
{
	while (true)
	{
		fd_set copy = m_master;

		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++)
		{
			SOCKET sock = copy.fd_array[i];

			if (sock == m_socket)
			{
				sockaddr_in clientAddr;
				int clientSize = sizeof(clientAddr);

				SOCKET client = accept(m_socket, (sockaddr*)&clientAddr, &clientSize);

				char host[NI_MAXHOST]; // Client's remote name
				char service[NI_MAXSERV]; // Port the client is connected on

				ZeroMemory(host, NI_MAXHOST);
				ZeroMemory(service, NI_MAXSERV);

				if (getnameinfo((sockaddr*)&clientAddr, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
				{
					std::cout << host << " connected on port " << service << std::endl;
				}
				else
				{
					inet_ntop(AF_INET, &clientAddr.sin_addr, host, NI_MAXHOST);
					std::cout << host << " connected on port " << ntohs(clientAddr.sin_port) << std::endl;
				}

				FD_SET(client, &m_master);

				onClientConnected(client);

			}
			else
			{
				const int bufSz = 4096;
				char buf[bufSz];
				ZeroMemory(buf, bufSz);
				int bytesIn = recv(sock, buf, bufSz, 0);

				if (bytesIn <= 0)
				{
					onClientDisconnected(sock);
					closesocket(sock);
					FD_CLR(sock, &m_master);
				}
				else
				{
					onMessageReceived(sock, buf, bytesIn);
				}
			}
		}
	}

	WSACleanup();

	return 0;
}

void TcpListener::sendToClient(int clientSocket, const char* msg, int length)
{
	send(clientSocket, msg, length, 0);
}

void TcpListener::broadcastToClients(int sendingClient, const char* msg, int length)
{
	for (int i = 0; i < m_master.fd_count; i++)
	{
		SOCKET outSock = m_master.fd_array[i];
		if (outSock != m_socket && outSock != sendingClient)
		{

			sendToClient(outSock, msg, length);

		}
	}
}

void TcpListener::onClientConnected(int clientSocket)
{

}

void TcpListener::onClientDisconnected(int clientSocket)
{

}

void TcpListener::onMessageReceived(int clientSocket, const char* msg, int length)
{

}

