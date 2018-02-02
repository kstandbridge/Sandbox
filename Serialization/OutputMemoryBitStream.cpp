#include "OutputMemoryBitStream.h"
#include <algorithm>

void OutputMemoryBitStream::WriteBits(uint8_t inData, size_t inBitCount)
{
	uint32_t nextBitHead = mBitHead + static_cast<uint32_t>(inBitCount);
	if(nextBitHead > mBitCapacity)
	{
		ReallocBuffer(std::max(mBitCapacity * 2, nextBitHead));
	}

	// Calculate the byteOffset into our buffer
	// by dividing the head by 8 and the bitoffset
	// by taking the last 3 bits
	uint32_t byteOffset = mBitHead >> 3;
	uint32_t bitOffset = mBitHead & 0x7;

	// Calculate which bits of the current byte to preserve
	uint8_t currentMask = ~(0xff << bitOffset);
	mBuffer[byteOffset] = (mBuffer[byteOffset] & currentMask) | (inData << bitOffset);

	// Calculate how many bits were not yet used in
	// our target byte in the buffer
	uint32_t bitsFreeThisByte = 8 - bitOffset;

	// If we needed more than that, carry to the next byte
	if(bitsFreeThisByte < inBitCount)
	{
		// We need another byte
		mBuffer[byteOffset + 1] = inData >> bitsFreeThisByte;
	}

	mBitHead = nextBitHead;
}

void OutputMemoryBitStream::WriteBits(const void* inData, size_t inBitCount)
{
	const char* srcByte = static_cast<const char*>(inData);
	// Write all the bytes
	while(inBitCount > 8)
	{
		WriteBits(*srcByte, 8);
		++srcByte;
		inBitCount -= 8;
	}
	// Write anyhting left
	if(inBitCount > 0)
	{
		WriteBits(*srcByte, inBitCount);
	}
}
