#include "RoboCat.h"
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"

void RoboCat::Write(OutputMemoryStream& inStream) const
{
	inStream.Write(mHealth);
	inStream.Write(mMeowCount);
	// No solution for mHomeBase yet
	
	// The name may not take the full 128
	//inStream.Write(mName, 128);
	uint8_t nameLength = static_cast<uint8_t>(strlen(mName));
	inStream.Write(nameLength);
	inStream.Write(mName, nameLength);
	
	// No solution for mMiceIndices yet
}

void RoboCat::Read(InputMemoryStream& inStream)
{
	inStream.Read(mHealth);
	inStream.Read(mMeowCount);
	// No solution for mHomeBase yet
	inStream.Read(mName, 128);
	// No solution for mMiceIndices
}