#include <iostream>
#include "SocketUtil.h"

void ProcessReceivedData(char str[1500], int bytesReceived, const SocketAddress& socketAddress);
void DoGameFrame();

void DoGameLoop()
{
	UDPSocketPtr mySock = SocketUtil::CreateUDPSocket(INET);
	mySock->SetNonBlockingMode(true);
	while(true)
	{
		char data[1500];
		SocketAddress socketAddress;
		int bytesReceived = mySock->ReceiveFrom(data, sizeof(data), socketAddress);
		if(bytesReceived > 0)
		{
			ProcessReceivedData(data, bytesReceived, socketAddress);
		}
		DoGameFrame();
	}
}


void ProcessNewClient(const TCPSocketPtr& shared, const SocketAddress& socketAddress);
void ProcessDataFromClient(const TCPSocketPtr& socket, char str[1024], int dataReceived);

void DoTCPLoop()
{
	TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(INET);
	SocketAddress receivingAddress(INADDR_ANY, 48000);
	if(listenSocket->Bind(receivingAddress) != NO_ERROR)
	{
		return;
	}
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
					const int GOOD_SEGMENT_SIZE = 1024; // Set this appropriately

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
}


