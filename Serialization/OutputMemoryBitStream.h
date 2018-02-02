#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "LinkingContext.h"

class GameObject;

class OutputMemoryBitStream
{
public:
	OutputMemoryBitStream()
		: mLinkingContext{nullptr}
	{
		ReallocBuffer(256);
	};

	~OutputMemoryBitStream() { std::free(mBuffer); };

	void WriteBits(uint8_t inData, size_t inBitCount);
	void WriteBits(const void* inData, size_t inBitCount);
	
	const char* GetBufferPtr() const { return mBuffer; }
	uint32_t GetBitLength() const { return mBitHead; }
	uint32_t GetByteLength() const { return (mBitHead + 7) >> 3; }

	void WriteBytes(const void* inData, size_t inByteCount)	
	{
		WriteBits(inData, inByteCount << 3);
	}

	void Write(uint32_t inData, size_t inBitCount = sizeof(uint32_t) * 8)
	{
		WriteBits(&inData, inBitCount);
	}

	void Write(int inData, size_t inBitCount = sizeof(int) * 8)
	{
		WriteBits(&inData, inBitCount);
	}

	void Write(float inData)
	{
		WriteBits(&inData, sizeof(float) * 8);
	}

	void Write(uint16_t inData, size_t inBitCount = sizeof(uint16_t) * 8)
	{
		WriteBits(&inData, inBitCount);
	}

	void Write(int16_t inData, size_t inBitCount = sizeof(int16_t) * 8)
	{
		WriteBits(&inData, inBitCount);
	}

	void write(uint8_t inData, size_t inBitCount = sizeof(uint8_t) * 8)
	{
		WriteBits(&inData, inBitCount);
	}
	void Write(bool inData)
	{
		WriteBits(&inData, 1);
	}

	template<typename T>
	void Write(T inData, size_t inBitCount = sizeof(T) * 8)
	{
		static_assert(std::is_arithmetic<T>::value ||
					  std::is_enum<T>::value,
					  "Generic Write only supports primitive data types");
		WriteBits(&inData, inBitCount);
	}

	void Write(const std::vector<int32_t>& inIntVector)
	{
		size_t elementCount = inIntVector.size();
		Write(elementCount);
		Write(inIntVector.data(), elementCount * sizeof(int32_t));
	}

	template<typename T>
	void Write(const std::vector<T>& inVector)
	{
		size_t elementCount = inVector.size();
		Write(elementCount);
		for(const T& element: inVector)
		{
			Write(element);
		}
	}

	void Write(const GameObject* inGameObject)
	{
		uint32_t networkId = mLinkingContext->GetNetworkId(inGameObject);
		Write(networkId);
	}

	// Assume we have a 32bit float for the location
	// The server can still use 32bit floats for accuracy,
	// when calculating collision etc
	// But the clients don't need this level of precision,
	// so When serializaing we can save some space using a fixed point number
	inline uint32_t ConvertToFixed(float inNumber, float inMin, float inPrecision)
	{
		return static_cast<uint32_t>((inNumber - inMin) / inPrecision);
	}
	inline float ConvertFromFixed(uint32_t inNumber, float inMin, float inPrecision)
	{
		return static_cast<float>(inNumber) * inPrecision + inMin;
	}
	void WritePos(float x, float y, float z)
	{
		Write(ConvertToFixed(x, -2000.f, 0.1f), 16);
		Write(ConvertToFixed(z, -2000.f, 0.1f), 16);
		// Write y component here
	}

	// The same is true for Quaternion, as the final component is a sum
	// we only need to store if its negative, as we can recalucate on deserializion
	struct Quaternion
	{
		float x, y, z, w;
	};
	const float PRECISION = (2.f / 65535.f);
	void Write(const Quaternion& inQuat)
	{
		Write(ConvertToFixed(inQuat.x, -1.f, PRECISION), 16);
		Write(ConvertToFixed(inQuat.y, -1.f, PRECISION), 16);
		Write(ConvertToFixed(inQuat.z, -1.f, PRECISION), 16);
		write(inQuat.w < 0);
	}
	void Read(Quaternion& outQuat)
	{
		uint32_t f = 0;
		Read(f, 16);
		outQuat.x = ConvertFromFixed(f, -1.f, PRECISION);
		Read(f, 16);
		outQuat.y = ConvertFromFixed(f, -1.f, PRECISION);
		Read(f, 16);
		outQuat.z = ConvertFromFixed(f, -1.f, PRECISION);

		outQuat.w = sqrtf(1.f -
						  (outQuat.x * outQuat.x + 
						   outQuat.y * outQuat.y + 
						   outQuat.z * outQuat.z));
		bool isNegative;
		Read(isNegative);
		if(isNegative)
		{
			outQuat.w *= -1;
		}
	}

private:
	void ReallocBuffer(uint32_t inNewBitCapacity);

	char* mBuffer;
	uint32_t mBitHead;
	uint32_t mBitCapacity;

	LinkingContext* mLinkingContext;
};