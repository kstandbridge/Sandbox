#pragma once
#include "SocketAddress.h"

#include <memory>

class TCPSocket
{
public:
	~TCPSocket();
	int Connect(const SocketAddress& inAddress);
	int Bind(const SocketAddress& inAddress);
	int Listen(int inBacklog = 32);
	std::shared_ptr<TCPSocket> Accept(SocketAddress& inFromAddress);
	int Send(const void* inData, int inLen);
	int Receive(void* inBuffer, int inLen);

	int SetNonBlockingMode(bool inShouldBeNonBlocking);

private:
	friend class SocketUtil;

	TCPSocket(SOCKET inSocket)
		: mSocket(inSocket)
	{
	}

	SOCKET mSocket;
};

typedef std::shared_ptr<TCPSocket> TCPSocketPtr;
