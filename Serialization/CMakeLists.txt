project(Serialization)

set(SRC
	
	OutputMemoryStream.cpp
	InputMemoryStream.cpp

	OutputMemoryBitStream.cpp
	InputMemoryBitStream.cpp

	MemorySteam.cpp

	RoboCat.cpp

	main.cpp
)

add_library(${PROJECT_NAME} ${SRC})