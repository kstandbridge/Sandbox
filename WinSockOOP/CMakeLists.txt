project (WinSockOOP)

set(src
	main.cpp
)

set(EXECUTABLE_NAME "WinSockOOP")
add_executable(${EXECUTABLE_NAME} ${src})
target_link_libraries(${EXECUTABLE_NAME} ws2_32.lib)