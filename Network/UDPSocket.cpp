#include "UDPSocket.h"
#include "SocketUtil.h"

int UDPSocket::Bind(const SocketAddress& inBindAddress)
{
	int err = bind(mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize());
	if(err != 0)
	{
		SocketUtil::ReportError("UDPSocket::Bind");
		return SocketUtil::GetLastError();
	}
	return NO_ERROR;
}

int UDPSocket::sendTo(const void* inData, int inLen, const SocketAddress& inTo)
{
	int byteSentCount = sendto(mSocket,
							   static_cast<const char*>(inData),
							   inLen,
							   0, &inTo.mSockAddr, inTo.GetSize());
	if(byteSentCount >= 0)
	{
		return byteSentCount;
	}
	else
	{
		// return error as negative number
		SocketUtil::ReportError("UDPSocket::SendTo");
		return -SocketUtil::GetLastError();
	}
}

int UDPSocket::ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom)
{
	int fromLength = outFrom.GetSize();
	int readByteCount = recvfrom(mSocket,
								 static_cast<char*>(inBuffer),
								 inLen,
								 0, &outFrom.mSockAddr,
								 &fromLength);
	if(readByteCount >= 0)
	{
		return readByteCount;
	}
	else
	{
		SocketUtil::ReportError("UDPSocket::ReceiveFrom");
		return -SocketUtil::GetLastError();
	}
}

UDPSocket::~UDPSocket()
{
	closesocket(mSocket);
}