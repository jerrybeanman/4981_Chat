#include "Server.h"

/*-----------------------------------------------------------------------------------------------
--    Name:     [InitializeSocket]         Date:         [March 6th, 2016]
--
--    Designer: [Jerry Jia]                Programmer:   [Jerry Jia]
--
--    Interface:  int InitializeSocket(short port)
--                [port] Port number
--
--    @return: -1 on failure, 0 on success
--
--    Notes: Initialize ListeningSocket, set server address, bind server address to listeningSocket,
--             call listen on ListeningSocket, and set the corresponding parameters to use select()
------------------------------------------------------------------------------------------------*/
int Server::InitializeSocket(short port)
{
    int optval = -1;
    // Create a TCP streaming socket
    if ((ListeningSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        std::cout << "socket() failed with errno " << errno << std::endl;
        return -1;
    }

    // Allows other sockets to bind() to this port, unless there is an active listening socket bound to the port already.
    setsockopt(ListeningSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // Fill in server address information
    memset(&_ServerAddress, 0, sizeof(struct sockaddr_in));
    _ServerAddress.sin_family = AF_INET;
    _ServerAddress.sin_port = htons(port);
    _ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

    // bind server address to accepting socket
    if (bind(ListeningSocket, (struct sockaddr *)&_ServerAddress, sizeof(_ServerAddress)))
    {
        std::cout << "InitializeSocket: bind() failed with errno " << errno << std::endl;
        return -1;
    }

    // Listen for connections
    listen(ListeningSocket, MAX_CONNECTIONS);

    MaxSocket = ListeningSocket;

    for(int i =0; i < FD_SETSIZE; i++)
        ClientList[i].socket = -1;

    FD_ZERO(&AllSet);
    FD_SET(ListeningSocket, &AllSet);

    return 0;
}

/*-----------------------------------------------------------------------------------------------
--    Name:     [Accept]                   Date:         [March 6th, 2016]
--
--    Designer: [Jerry Jia]                Programmer:   [Jerry Jia]
--
--    Interface:  int Accept(Client * client)
--                [client] Pointer to a client structure
--
--    @return: Index in ClientList where the client is stored at
--
--    Notes: Calls accept on a client's socket. Sets the returning socket and client address structure
--    to the client. Add connected client to ClientList
------------------------------------------------------------------------------------------------*/
int Server::Accept(Client * client)
{
    int             i;
    unsigned int    ClientLen = sizeof(client->connection);

    // Accepts a connection from the client
    if ((client->socket = accept(ListeningSocket, (struct sockaddr *)&client->connection, &ClientLen)) == -1)
    {
        std::cerr << "Accept() failed with errno" << errno << std::endl;
        return -1;
    }

    std::cout << "Client Connect! Remote Address: " << inet_ntoa(client->connection.sin_addr) << std::endl;

    // Look for an available slot in CLientList
    for(i = 0; i < FD_SETSIZE; i++)
    {
        if(ClientList[i].socket < 0)
        {
            ClientList[i] = *client;
            break;
        }
    }

    // ClientList is full
    if(i == FD_SETSIZE)
    {
        std::cerr << "Too many clients to accept" << std::endl;
        return -1;
    }

    FD_SET(client->socket, &AllSet);
    if(client->socket > MaxSocket)
        MaxSocket = client->socket;

    return i;
}


/*-----------------------------------------------------------------------------------------------
--    Name:     [Receive]                  Date:         [March 6th, 2016]
--
--    Designer: [Jerry Jia]                Programmer:   [Jerry Jia]
--
--    Interface:  int Receive(int index)
--                [index] Index in CLientList to call recv() on
--
--    @return: -1 on error, 0 for success, 1 for client disconnect
--
--    Notes: Recieves data from the specific client's socket indicated by index, which will be
--        accessed on ClientList using index
------------------------------------------------------------------------------------------------*/
int Server::Receive(int index)
{
    int BytesRead;
    char * buf;

    buf = (char *)malloc(PACKET_LEN); 	        // allocates memory

    while((BytesRead = recv (ClientList[index].socket, buf, PACKET_LEN, 0)) < PACKET_LEN)
    {
        // recv() failed
        if(BytesRead < 0)
        {
            printf("recv() failed with errno: %d", errno);
            return -1;
        }

        // client disconnected
        if(BytesRead == 0)
        {
            free(buf);
            printf("Client %d has disconnected \n",  index+1);
            close(ClientList[index].socket);
            FD_CLR(ClientList[index].socket, &AllSet);
            return 1;
        }

    }
    std::cout << buf << std::endl;
    if(buf[0] == (char)17) {
        this->Server::addUser(buf);
        this->Server::SendToClient((char *)this->Server::generateUserList().c_str(), index);

    } else if(buf[0] == (char) 18) {
        this->Server::removeUser(buf);
        close(ClientList[index].socket);
        FD_CLR(ClientList[index].socket, &AllSet);
    }

    /* Broadcast echo packet back to all players */
    this->Server::Broadcast(buf, index);

    free(buf);
    return 0;
}

/*-----------------------------------------------------------------------------------------------
--    Name:     [Broadcast]                Date:         [March 6th, 2016]
--
--    Designer: [Jerry Jia]                Programmer:   [Jerry Jia]
--
--    Interface:  void Broadcast(char * message, int ExcludeIndex)
--                [message]      message to broadcast
--                [ExcludeIndex] Index in ClientList to exclude the broadcasting
--
--    @return: -1 on error, 0 for success, 1 for client disconnect
--
--    Notes: Sends a message to all the connect client stored in ClientList, exept for the client
--             stored on ExcludeIndex
------------------------------------------------------------------------------------------------*/
void Server::Broadcast(char * message, int ExcludeIndex)
{
    for(size_t i = 0; i < (sizeof(ClientList) / sizeof(ClientList[0])); i++)
    {
        if(i == (size_t)ExcludeIndex)
            continue;
        if(ClientList[i].socket == -1) {
            continue;
        }
        if(send(ClientList[i].socket, message, PACKET_LEN, 0) == -1)
        {
            std::cerr << "Broadcast() failed for player id: " << ClientList[i].id + 1 << std::endl;
            std::cerr << "errno: " << errno << std::endl;
            return;
        }
    }
}

void Server::SendToClient(char *message, int index) 
{
    if(send(ClientList[index].socket, message, PACKET_LEN, 0) == -1) {
            std::cerr << "Broadcast() to single client  failed for player id: " << ClientList[index].id + 1 << std::endl;
            std::cerr << "errno: " << errno << std::endl;
    }
}

void Server::addUser(const char *name) 
{
    std::string userName(name);
    std::size_t userNamePos =  userName.find(": ");
    std::string nameToken = userName.substr(userNamePos + 2, userName.find(" connected") -(userNamePos + 2));

    std::cout << "Added user: " << nameToken << std::endl;
    connectedUsers.push_back(nameToken);
}

void Server::removeUser(const char *name) 
{
    std::string userName(name);
    std::size_t userNamePos =  userName.find(": ");
    std::string nameToken = userName.substr(userNamePos + 2, userName.find(" disconnected") -(userNamePos + 2));

    std::cout << "Removed user: " << nameToken << std::endl;
    connectedUsers.erase(std::remove(connectedUsers.begin(), connectedUsers.end(), nameToken), connectedUsers.end());
    for(std::string& user: connectedUsers)
    {
        std::cout << user << " ";
    }
    std::cout <<  std::endl;
}

std::string Server::generateUserList() 
{
    std::string users(1, (char) 19);

    for(std::string& user: connectedUsers)
    {
        users.append(user + " ");
    }
    return users;
}
