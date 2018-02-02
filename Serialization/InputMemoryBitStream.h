#pragma once
#include "LinkingContext.h"

class InputMemoryBitStream
{
public:
	InputMemoryBitStream()
		: mLinkingContext{nullptr}
	{
	}

	template<typename T>
	void Read(std::vector<T>& outVector)
	{
		size_t elementCount;
		Read(elementCount);
		outVector.resize(elementCount);
		for(T& element: outVector)
		{
			Read(element);
		}
	}

	void Read(GameObject* outGameObject)
	{
		uint32_t networkId;
		Read(networkId);
		outGameObject = mLinkingContext->GetGameObject(networkId);
	}
private:
	LinkingContext* mLinkingContext;
};