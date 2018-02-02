#pragma once
#include <cstdint>
#include <type_traits>
#include <string>

#include "ByteSwapper.h"
#include <vector>

#define STREAM_ENDIANNESS 0
#define PLATFORM_ENDIANNESS 0

class MemoryStream
{
public:
	virtual void Serialize(void* ioData, uint32_t inByteCount) = 0;
	virtual bool IsInput() const = 0;

	template<typename T> void Serialize(T& ioData)
	{
		static_assert(std::is_arithmetic<T>::value ||
					  std::is_enum<T>::value,
					  "Generic Serialize only supports primitive data types");

		if(STREAM_ENDIANNESS == PLATFORM_ENDIANNESS)
		{
			Serialize(&ioData, sizeof(ioData));
		}
		else
		{
			if(IsInput())
			{
				T data;
				Serialize(&data, sizeof(T));
				ioData = ByteSwap(data);
			}
			else
			{
				T swappedData = ByteSwap(ioData);
				Serialize(&swappedData, sizeof(swappedData));
			}
		}
	}
};

class InputMemorySteam : public MemoryStream
{
	void Serialize(void* ioData, uint32_t inByteCount) override
	{
		Read(ioData, inByteCount);
	}
	bool IsInput() const override { return true; }
};
class OutputMemoryStream : public MemoryStream
{
	void Serialize(void* ioData, uint32_t inByteCount) override
	{
		Write(ioData, inByteCount);
	}
	bool IsInput() const override { return false; }
};


enum EPrimitiveType
{
	EPT_Int,
	EPT_String,
	EPT_Float
};

class MemberVariable
{
public:
	MemberVariable(const char* inName, EPrimitiveType inPrimitiveType, uint32_t inOffset)
		: mName{inName}, mPrimitiveType{inPrimitiveType}, mOffset{inOffset}
	{
	}

	EPrimitiveType GetPrimitiveType() const { return mPrimitiveType; }
	uint32_t GetOffset() const { return mOffset; }

private:
	std::string mName;
	EPrimitiveType mPrimitiveType;
	uint32_t mOffset;
};

class DataType
{
public:
	DataType(std::initializer_list<const MemberVariable&> inMVs)
		: mMemberVariables{inMVs}
	{
	}

	const std::vector<MemberVariable>& GetMemberVariables() const
	{
		return mMemberVariables;
	}

private:
	std::vector<MemberVariable> mMemberVariables;

};

#define OffsetOf(c, mv) ((size_t) & (static_cast<c*>(nullptr)->mv))

class MouseStatus
{
public:
	std::string mName;
	int mLegCount, mHeadCount;
	float mHealth;

	static DataType* sDataType;
	static void InitDataType()
	{
		sDataType = new DataType(
			{
				MemberVariable("mName", EPT_String, OffsetOf(MouseStatus, mName)) ,
				MemberVariable("mLegCount", EPT_String, OffsetOf(MouseStatus, mLegCount)),
				MemberVariable("mHeadCount", EPT_String, OffsetOf(MouseStatus, mHeadCount)),
				MemberVariable("mHealth", EPT_String, OffsetOf(MouseStatus, mHealth))
			}
		);
	}
};

void Serialize(MemoryStream* inMemoryStream, const DataType* inDataType, uint8_t* inData)
{
	for(auto& mv : inDataType->GetMemberVariables())
	{
		void* mvData = inData + mv.GetOffset();
		switch(mv.GetPrimitiveType())
		{
			case EPT_Int: 
				inMemoryStream->Serialize(*(int*)mvData);
				break;
			case EPT_String:
				inMemoryStream->Serialize(*(std::string*)mvData);
				break;
			case EPT_Float:
				inMemoryStream->Serialize(*(float*)mvData);
				break;
			default: ;
		}
	}
}