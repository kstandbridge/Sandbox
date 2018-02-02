#pragma once
#include <cstdint>
#include <vector>

class InputMemoryStream;
class OutputMemoryStream;

class GameObject
{
	
};


class RoboCat : public GameObject
{
public:
	RoboCat() 
	: mHealth{10}, mMeowCount{3}
	{
		mName[0] = '\0';
	}

	virtual void Update();

	void Write(OutputMemoryStream& inStream) const;
	void Read(InputMemoryStream& inStream);

private:
	int32_t mHealth;
	int32_t mMeowCount;
	GameObject* mHomeBase;
	char mName[128];
	std::vector<int32_t> mMiceIndices;
};

class HomeBase : public GameObject
{
	std::vector<RoboCat*> mRoboCats;
};

