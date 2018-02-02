#include <iostream>
#include "SocketUtil.h"
#include "TCPSocket.h"

void ProcessNewClient(const TCPSocketPtr& shared, const SocketAddress& socketAddress)
{
	std::cout << "ProcessNewClient" << std::endl;
	std::string welcome = "Some welcome message";
	shared->Send(welcome.c_str(), sizeof(welcome));
}
void ProcessDataFromClient(const TCPSocketPtr& socket, char str[1024], int dataReceived)
{
	std::cout << "ProcessDataFromClient" << std::endl;
}

int main(int argc, char* argv[])
{
	std::cout << "Attempting to allow multiple clients..." << std::endl;

	SocketUtil::StaticInit();

	TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(INET);
	SocketAddress receivingAddress(INADDR_ANY, 6125);
	if(listenSocket->Bind(receivingAddress) != NO_ERROR)
	{
		return EXIT_FAILURE;
	}
	listenSocket->Listen();

	std::vector<TCPSocketPtr> readBlockSockets;
	readBlockSockets.push_back(listenSocket);
	
	std::vector<TCPSocketPtr> readableSockets;
	while(true)
	{
		if(SocketUtil::Select(&readBlockSockets, &readableSockets, nullptr, nullptr, nullptr, nullptr))
		{
			// We got a packet-loop through the set ones...
			for(const TCPSocketPtr& socket : readBlockSockets)
			{
				if(socket == listenSocket)
				{
					// Its the listen socket, accept a new connection
					SocketAddress newClientAddress;
					auto newSocket = listenSocket->Accept(newClientAddress);
					readBlockSockets.push_back(newSocket);
					ProcessNewClient(newSocket, newClientAddress);
				}
				else
				{
					const int GOOD_SEGMENT_SIZE = 128; // Set this appropriately

					// Its a regular socket-process the data...
					char segment[GOOD_SEGMENT_SIZE];
					int dataReceived = socket->Receive(segment, GOOD_SEGMENT_SIZE);
					if(dataReceived > 0)
					{
						ProcessDataFromClient(socket, segment, dataReceived);
					}
				}
			}
		}
	}

	system("PAUSE");
	SocketUtil::CleanUp();
	return EXIT_SUCCESS;
}


int main_single(int argc, char* argv[])
{
	SocketUtil::StaticInit();

	TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(INET);
	SocketAddress receivingAddress(INADDR_ANY, 6125);
	if(listenSocket->Bind(receivingAddress) != NO_ERROR)
	{
		return EXIT_FAILURE;
	}
	while(true)
	{
		std::cout << "Listening on 6125..." << std::endl;
		listenSocket->Listen();
		SocketAddress inAddress;
		auto tcpSocket = listenSocket->Accept(inAddress);
		std::cout << "Connection from: " << inAddress.GetSize() << std::endl;
		char buffer[128];
		while(true)
		{
			auto bytes = tcpSocket->Receive(buffer, 128);
			std::cout << "Recieved " << bytes << " bytes" << std::endl;
			if(bytes == 0)
			{
				std::cout << "Connection was closed" << std::endl;
				break;
			}
			else
			{
				std:: cout << buffer << std::endl;
			}
		}
	}

	SocketUtil::CleanUp();
	system("PAUSE");
	return EXIT_SUCCESS;
}