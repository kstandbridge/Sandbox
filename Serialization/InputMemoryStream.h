#pragma once
#include <cstdint>
#include <memory>

class InputMemoryStream
{
public:
	InputMemoryStream(char* inBuffer, uint32_t inByteCount)
		: mCapacity{inByteCount}, mHead{0}
	{}
	
	~InputMemoryStream() { std::free(mBuffer); }

	uint32_t GetRemainingDataSize() const { return mCapacity - mHead; }

	void Read(void* outData, uint32_t inByteCount);
	void Read(uint32_t& outData) { Read(&outData, sizeof(outData)); }
	void Read(int32_t& outData) { Read(&outData, sizeof(outData)); }

private:
	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};