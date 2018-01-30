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

set(EXECUTABLE_NAME "Network")
add_executable(${EXECUTABLE_NAME} ${SRC})
target_link_libraries(${EXECUTABLE_NAME} Ws2_32)