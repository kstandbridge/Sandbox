#include <iostream>

#include "MemoryBitStream.h"

class Person
{
public:
	int age;

	void Write(OutputMemoryBitStream& inStream) const
	{
		inStream.Write(age);
	}

	void Read(InputMemoryBitStream& inStream)
	{
		inStream.Read(age);
	}
};

int main(int argc, char* argv[])
{
	Person p;
	p.age = 42;

	OutputMemoryBitStream stream;
	p.Write(stream);

	Person b;
	InputMemoryBitStream input((char*)stream.GetBufferPtr(), stream.GetBitLength());
	b.Read(input);

	std::cout << b.age << std::endl;


	return EXIT_SUCCESS;
}
