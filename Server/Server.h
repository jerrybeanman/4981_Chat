#ifndef SERVER_TCP
#define SERVER_TCP
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iterator>
#include <string>

#define MAX_CONNECTIONS  20
#define PACKET_LEN       256


struct Client
{
    int         id;
    int         socket;
    sockaddr_in connection;
};

class Server
{
    public:
        /*--------------------------
        -- Public Variables Field --
        ---------------------------*/
        fd_set                 AllSet;
        Client                 ClientList[FD_SETSIZE];
        int 				   ListeningSocket;
        int                    MaxSocket;

        /*--------------------------
        -- Constructor            --
        ---------------------------*/
        Server(){}
        ~Server(){}

        /*--------------------------
        -- Function Prototypes    --
        ---------------------------*/
        /*-----------------------------------------------------------------------------------------------
        --    Initialize socket, and its own server address
        --
        --    Interface:  int InitializeSocket(short port)
        --                [port] Port number
        --
        --    programmer: Jerry Jia
        --
        --    @return: -1 on failure, 0 on success
        ------------------------------------------------------------------------------------------------*/
        int InitializeSocket(short port);

        /*-----------------------------------------------------------------------------------------------
        --     Calls accept on a player's socket. Sets the returning socket and client address structure to
        --    the client. Add connected player to the list of players
        --
        --    Interface:  int Accept()
        --
        --    programmer: Jerry Jia
        --
        --    return: socket file descriptor
        -------------------------------------------------------------------------------------------------*/
        int Accept(Client * client);

        /*-----------------------------------------------------------------------------------------------
        --    Creates a child process to handle incoming messages from new player that has just connected
        --    to the lobby
        --
        --    Interface:  void * CreateClientManager(void * server)
        --                [server] Pointer to a void, which has to be a Server object
        --
        --    Programmer: Jerry Jia
        --
        --    return: Recieve() address
        -------------------------------------------------------------------------------------------------*/
        static void * CreateClientManager(void * server);

        /*-----------------------------------------------------------------------------------------------
        --        Recieves data from child process that is dedicated for each player's socket
        --
        --        Interface:  void * Receive()
        --
        --        Programmer: Jerry Jia
        --
        --        @return: Thread execution code
        -------------------------------------------------------------------------------------------------*/
        int Receive(int index);

        /*-----------------------------------------------------------------------------------------------
        --    Sends a message to all the clients
        --
        --    Interface:  void Broadcast(char * message)
        --
        --    Programmer: Jerry Jia
        --
        --        @return: void
       -------------------------------------------------------------------------------------------------*/
        void Broadcast(char * message, int ExcludeIndex);

    private:
        struct sockaddr_in     _ServerAddress;


};

#endif
