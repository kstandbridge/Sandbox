#include <iostream>
#include <string>
#include <cstdlib>
#include "SocketUtil.h"
#include "TCPSocket.h"
#include "SocketAddressFactory.h"


int main(int argc, char* argv[])
{
	SocketUtil::StaticInit();
	SocketAddressPtr address = SocketAddressFactory::CreateIPv4FromString("127.0.0.1:6125");
	auto socket = SocketUtil::CreateTCPSocket(INET);
	auto err = socket->Connect(*address);
	if(NO_ERROR != err)
	{
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
	
	std::string input;

	const int GOOD_SEGMENT_SIZE = 128; // Set this appropriately
	char segment[GOOD_SEGMENT_SIZE];
	

	while(true)
	{
		int dataReceived = socket->Receive(segment, GOOD_SEGMENT_SIZE);
		if(dataReceived > 0)
		{
			std::cout << "Recieved " << dataReceived << " bytes" << std::endl;
			std::cout << segment << std::endl;
		}
		std::cout << "Exit to quit, or enter a message to send: " << std::endl;
		std::cin >> input;
		if(input == "Exit" || input == "exit")
		{
			break;
		}
		socket->Send(input.c_str(), sizeof(input.length()));
	}

	system("PAUSE");
	return EXIT_SUCCESS;
}
