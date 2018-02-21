#include <iostream>
#include <unordered_map>

class GameObject;

enum PacketType
{
	PT_Hello,
	PT_ReplicationData,
	PT_Disconnect,
	PT_MAX
};

class LinkingContext
{
public:
private:
	std::unordered_map<uint32_t, GameObject*> mNetworkIdToGameObjectMap;
};

int main(int argc, char* argv[])
{
	std::cout << "Replication yo!" << std::endl;

	system("PAUSE");
	return EXIT_SUCCESS;
}
