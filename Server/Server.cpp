#include "Server.h"

/*
	Initialize socket, server address to lookup to, and connect to the server

	@return: socket file descriptor
*/
int Server::InitializeSocket(short port)
{
    int optval = -1;
    /* Create a TCP streaming socket */
    if ((_ListeningSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        std::cout << "socket() failed with errno " << errno << std::endl;
        return -1;
    }

    /* Allows other sockets to bind() to this port, unless there is an active listening socket bound to the port already. */
    setsockopt(_ListeningSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    /* Fill in server address information */
    memset(&_ServerAddress, 0, sizeof(struct sockaddr_in));
    _ServerAddress.sin_family = AF_INET;
    _ServerAddress.sin_port = htons(port);
    _ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

    /* bind server address to accepting socket */
    if (bind(_ListeningSocket, (struct sockaddr *)&_ServerAddress, sizeof(_ServerAddress)))
    {
        std::cout << "InitializeSocket: bind() failed with errno " << errno << std::endl;
        return -1;
    }

    /* Listen for connections */
    listen(_ListeningSocket, MAX_CONNECTIONS);

    return 0;
}

/*
	Calls accept on a player's socket. Sets the returning socket and client address structure to the player.
	Add connected player to the list of players

	@return: id that is assigned to the player
*/
int Server::Accept(Client * client)
{
    char                buf[PACKET_LEN];
    unsigned int        ClientLen = sizeof(client->connection);
    /* Accepts a connection from the client */
    if ((client->socket = accept(_ListeningSocket, (struct sockaddr *)&client->connection, &ClientLen)) == -1)
    {
        std::cerr << "Accept() failed with errno" << errno << std::endl;
        return -1;
    }
    /* Not the best way to do it since we're using vectors */
    client->id = _ClientList.size();

    _ClientList.push_back(*client);

    sprintf(buf, "Player %d has joined the lobby\n", (int)_ClientList.size());

    printf("%s",buf);
    this->Server::Broadcast(buf);
    _NewClient = *client;
    return client->id;
}


/*
	Creates a child process to handle incoming messages from new player that has just connected to the lobby

	@return: child PDI (0 for child process)
*/
void * Server::CreateClientManager(void * server)
{
    /* God forbid */
    return ((Server *)server)->Receive();
}


/*
	Recieves data from child process that is dedicated for each player's socket

	@return: 1 on success, -1 on error, 0 on disconnect
*/
void * Server::Receive()
{
    Client TmpCLient = _NewClient;
    int BytesRead;
    char * buf;						          	/* buffer read from one recv call      	  */
    int code;
    int idValue;
    int team;

    buf = (char *)malloc(PACKET_LEN); 	/* allocates memory 							        */
    while (1)
    {
        BytesRead = recv (TmpCLient.socket, buf, PACKET_LEN, 0);

        if(BytesRead < 0) /* recv() failed */
        {
            printf("recv() failed with errno: %d", errno);
            return 0;
        }
        if(BytesRead == 0) /* client disconnected */
        {
            printf("Player %d has left the lobby \n", TmpCLient.id + 1);
            return 0;
        }

        std::cout << buf << std::endl;
        /* Broadcast echo packet back to all players */
        this->Server::Broadcast(buf);
    }
    free(buf);
    return 0;
}

/*
	Sends a message to all the clients

*/
void Server::Broadcast(char * message)
{
    for(std::vector<int>::size_type i = 0; i != _ClientList.size(); i++)
    {
        if(send(_ClientList[i].socket, message, sizeof(message), 0) == -1)
        {
            std::cerr << "Broadcast() failed for player id: " << _ClientList[i].id + 1 << std::endl;
            std::cerr << "errno: " << errno << std::endl;
            return;
        }
    }
}