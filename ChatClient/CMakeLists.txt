project(ChatClient)

set(SRC

	main.cpp
)

include_directories(${PROJECT_NAME} ${Network_SOURCE_DIR})
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} Network)