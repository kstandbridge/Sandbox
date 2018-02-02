project(Network)

set(SRC

	StringUtils.cpp	
	SocketUtil.cpp

	SocketAddress.cpp
	SocketAddressFactory.cpp

	UDPSocket.cpp
	TCPSocket.cpp
	
	main.cpp
)

add_library(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} Ws2_32)