#include "Server.h"

/*
	Initialize socket, server address to lookup to, and connect to the server

	@return: socket file descriptor
*/
int Server::InitializeSocket(short port)
{
    int optval = -1;
    /* Create a TCP streaming socket */
    if ((ListeningSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        std::cout << "socket() failed with errno " << errno << std::endl;
        return -1;
    }

    /* Allows other sockets to bind() to this port, unless there is an active listening socket bound to the port already. */
    setsockopt(ListeningSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    /* Fill in server address information */
    memset(&_ServerAddress, 0, sizeof(struct sockaddr_in));
    _ServerAddress.sin_family = AF_INET;
    _ServerAddress.sin_port = htons(port);
    _ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

    /* bind server address to accepting socket */
    if (bind(ListeningSocket, (struct sockaddr *)&_ServerAddress, sizeof(_ServerAddress)))
    {
        std::cout << "InitializeSocket: bind() failed with errno " << errno << std::endl;
        return -1;
    }

    /* Listen for connections */
    listen(ListeningSocket, MAX_CONNECTIONS);

    MaxSocket = ListeningSocket;

    for(int i =0; i < FD_SETSIZE; i++)
        ClientList[i].socket = -1;

    FD_ZERO(&AllSet);
    FD_SET(ListeningSocket, &AllSet);

    return 0;
}

/*
	Calls accept on a player's socket. Sets the returning socket and client address structure to the player.
	Add connected player to the list of players

	@return: id that is assigned to the player
*/
int Server::Accept(Client * client)
{
    int             i;
    unsigned int    ClientLen = sizeof(client->connection);

    /* Accepts a connection from the client */
    if ((client->socket = accept(ListeningSocket, (struct sockaddr *)&client->connection, &ClientLen)) == -1)
    {
        std::cerr << "Accept() failed with errno" << errno << std::endl;
        return -1;
    }

    std::cout << "Client Connect! Remote Address: " << inet_ntoa(client->connection.sin_addr) << std::endl;

    for(i = 0; i < FD_SETSIZE; i++)
    {
        if(ClientList[i].socket < 0)
        {
            ClientList[i] = *client;
            break;
        }
    }
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


/*
	Recieves data from child process that is dedicated for each player's socket

	@return: 1 on success, -1 on error, 0 on disconnect
*/
int Server::Receive(int index)
{
    int BytesRead;
    char * buf;						          	/* buffer read from one recv call      	  */
    std::string packet;

    buf = (char *)malloc(PACKET_LEN); 	        /* allocates memory 	    			  */

    while((BytesRead = recv (ClientList[index].socket, buf, PACKET_LEN, 0)) < PACKET_LEN)
    {
        if(BytesRead < 0) /* recv() failed */
        {
            printf("recv() failed with errno: %d", errno);
            return -1;
        }
        packet += buf;
        if(BytesRead == 0) /* client disconnected */
        {
            free(buf);
            printf("Client %d has disconnected \n",  index+1);
            close(ClientList[index].socket);
            FD_CLR(ClientList[index].socket, &AllSet);
            return 0;
        }

    }

    std::cout << buf << std::endl;

    /* Broadcast echo packet back to all players */

    this->Server::Broadcast(buf, index);
    free(buf);
    return 0;
}

/*
	Sends a message to all the clients

*/
void Server::Broadcast(char * message, int ExcludeIndex)
{
    for(int i = 0; ClientList[i].socket !=  -1; i++)
    {
        if(i == ExcludeIndex)
            continue;
        if(send(ClientList[i].socket, message, PACKET_LEN, 0) == -1)
        {
            std::cerr << "Broadcast() failed for player id: " << ClientList[i].id + 1 << std::endl;
            std::cerr << "errno: " << errno << std::endl;
            return;
        }
    }
}
