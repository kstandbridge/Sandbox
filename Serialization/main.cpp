
#include <WinSock2.h>
#include "RoboCat.h"
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"

void NaivelySendRoboCat(int inSocket, const RoboCat* inRoboCat)
{
	send(inSocket,
	     reinterpret_cast<const char*>(inRoboCat),
		 sizeof(RoboCat), 0);
}


void SendRoboCat(int inSocket, const RoboCat* inRoboCat)
{
	OutputMemoryStream stream;
	inRoboCat->Write(stream);
	send(inSocket, stream.GetBufferPtr(), stream.GetLength(), 0);
}

void NaivelyReceiveRoboCat(int inSocket, RoboCat* outRoboCat)
{
	recv(inSocket,
		 reinterpret_cast<char*>(outRoboCat),
		 sizeof(RoboCat), 0);
}

const uint32_t kMaxPacketSize = 1470;

void ReceiveRoboCat(int inSocket, RoboCat* outRoboCat)
{
	char* temporaryBuffer = static_cast<char*>(std::malloc(kMaxPacketSize));
	size_t receivedByteCount = recv(inSocket, temporaryBuffer, kMaxPacketSize, 0);

	if(receivedByteCount > 0)
	{
		InputMemoryStream stream(temporaryBuffer,
								 static_cast<uint32_t>(receivedByteCount));
		outRoboCat->Read(stream);
	}
	else
	{
		std::free(temporaryBuffer);
	}
}

