#include <iostream>
#include "SocketUtil.h"


int main()
{
	TCPSocketPtr tcp = SocketUtil::CreateTCPSocket(INET);
	

	system("PAUSE");
	return EXIT_SUCCESS;
}



