project(TestConsole)

set(EXECUTABLE_NAME "Sandbox")
add_executable(${EXECUTABLE_NAME} main.cpp)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

file(COPY GameData DESTINATION ${CMAKE_CURRENT_BINARY_DIR})