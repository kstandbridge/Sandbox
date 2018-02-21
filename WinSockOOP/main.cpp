#include <list>
#include <algorithm>
#include <memory>
#include <iostream>
#include <WinSock2.h>

class Server
{
    std::list<SOCKET> clients;
    SOCKET master;
public:
    Server()
    {
#if _WIN32
        WSADATA wsa;
        printf("\nInitialising Winsock...");
        if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
        {
            printf("Failed. Error Code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
#endif
        printf("Initialised.\n");
    }

    ~Server()
    {
        WSACleanup();
    }

    void Listen(int16_t port)
    {
        //Create a socket
        if((master = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        {
            printf("Could not create socket : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        printf("Socket created.\n");

        //Prepare the sockaddr_in structure
        sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(port);

        //Bind
        if(bind(master, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
        {
            printf("Bind failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        puts("Bind done");

        //Listen to incoming connections
        listen(master, 3);
    }

    void Run()
    {
        const auto MAXRECV = 1024;
        const auto buffer = static_cast<char*>(malloc((MAXRECV + 1) * sizeof(char)));

        //set of socket descriptors
        fd_set readfds;
        SOCKET new_socket;
        sockaddr_in address;
        auto addrlen = sizeof(struct sockaddr_in);
        while(true)
        {
            //clear the socket fd set
            FD_ZERO(&readfds);

            //add master socket to fd set
            FD_SET(master, &readfds);

            // Add child sockets to fd set
            for(auto client : clients)
            {
                if(client > 0)
                {
                    FD_SET(client, &readfds);
                }
            }

            // Wait here for activity on any of the sockets.
            auto activity = select(0, &readfds, nullptr, nullptr, nullptr);

            if(SOCKET_ERROR == activity)
            {
                std::cout << "Select failed with error code: " << WSAGetLastError();
                exit(EXIT_FAILURE);
            }

            // Master socket hit so must be incoming connection
            if(FD_ISSET(master, &readfds))
            {
                new_socket = accept(master, (struct sockaddr *)&address, (int *)&addrlen);


                //inform user of socket number - used in send and receive commands
                printf("New connection , socket fd is %d , ip is : %s , port : %d \n",
                       new_socket,
                       inet_ntoa(address.sin_addr),
                       ntohs(address.sin_port));

                clients.push_back(new_socket);
            }

            // Some IO Operations?
            auto it = clients.begin();
            while(it != clients.end())
            {
                const auto client = *it;

                if(!FD_ISSET(client, &readfds))
                {
                    ++it;
                }
                else
                {
                    getpeername(client, (struct sockaddr*)&address, (int*)&addrlen);

                    auto valRead = recv(client, buffer, MAXRECV, 0);
                    if(valRead == 0 || SOCKET_ERROR == valRead)
                    {
                        if(SOCKET_ERROR == valRead)
                        {
                            int error_code = WSAGetLastError();
                            if(WSAECONNRESET == error_code)
                            {
                                printf("Host disconnected unexpectedly , ip %s , port %d \n",
                                       inet_ntoa(address.sin_addr),
                                       ntohs(address.sin_port));
                            }
                            else
                            {
                                printf("recv failed with error code : %d", error_code);
                            }
                            closesocket(client);
                            it = clients.erase(it);
                        }
                        else
                        {
                            printf("Host disconnected , ip %s , port %d \n",
                                   inet_ntoa(address.sin_addr),
                                   ntohs(address.sin_port));

                            closesocket(client);
                            it = clients.erase(it);
                        }
                    }
                    else
                    {
                        buffer[valRead] = '\0'; // Add null terminator to end
                        printf("%s:%d - %s \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port), buffer);
                        ++it;
                    }
                }
            }
        }
    }
};

int main(int argc, char* argv[])
{
    Server server;
    server.Listen(6125);
    server.Run();

    getchar();
    return EXIT_SUCCESS;
}
