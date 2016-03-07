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
        int InitializeSocket(short port);

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
        int Accept(Client * client);

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
        int Receive(int index);

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
        void Broadcast(char * message, int ExcludeIndex);

    private:
        struct sockaddr_in     _ServerAddress;


};

#endif
