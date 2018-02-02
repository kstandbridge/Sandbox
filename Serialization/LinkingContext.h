#pragma once
#include "RoboCat.h"
#include <unordered_map>

class LinkingContext
{
public:
	uint32_t GetNetworkId(GameObject* inGameObject)
	{
		auto it = mGameObjectToNetworkIpMap.find(inGameObject);
		if(it != mGameObjectToNetworkIpMap.end())
		{
			return it->second;
		}
		else
		{
			return 0;
		}
	}
	GameObject* GetGameObject(uint32_t inNetworkId)
	{
		auto it = mNetworkIdToGameObjectMap.find(inNetworkId);
		if(it != mNetworkIdToGameObjectMap.end())
		{
			return it->second;
		}
		else
		{
			return nullptr;
		}
	}
private:
	std::unordered_map<uint32_t, GameObject*> mNetworkIdToGameObjectMap;
	std::unordered_map<GameObject*, uint32_t> mGameObjectToNetworkIpMap;
};
