#ifndef CLIENT_H_
#define CLIENT_H_
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <utility>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <QString>

#define PACKET_LEN    256
#define PORT        7000
class Client
{
  public:
        /*--------------------------
        -- Constructor            --
        ---------------------------*/
        Client(){}
        ~Client(){}

        /*--------------------------
        -- Function Prototypes    --
        ---------------------------*/
        /*-----------------------------------------------------------------------------------------------
        --    Name:     [InitializeSocket]         Date:         [March 6th, 2016]
        --
        --    Designer: [Jerry Jia]                Programmer:   [Jerry Jia]
        --
        --    Interface:  int InitializeSocket(const char * name, short port)
        --                [name] IP address of peer host
        --                [port] Port number
        --
        --    @return: -1 on failure, 0 on success
        --
        --    Notes: Initialize _ClientSocket and _ServerAddress, and attempt to connect to a peer host
        ------------------------------------------------------------------------------------------------*/
        int InitializeSocket(const char* name, short port);


        /*-----------------------------------------------------------------------------------------------
        --    Name:     [Connect]                  Date:         [March 6th, 2016]
        --
        --    Designer: [Jerry Jia]                Programmer:   [Jerry Jia]
        --
        --    Interface:  int Connect()
        --
        --    @return: -1 on failure, 0 on success
        --
        --    Notes: Wrapper for connect
        ------------------------------------------------------------------------------------------------*/
        int Connect();

        /*-----------------------------------------------------------------------------------------------
        --    Name:     [Receive]                  Date:         [March 6th, 2016]
        --
        --    Designer: [Jerry Jia]                Programmer:   [Jerry Jia]
        --
        --    Interface:  void * Receive()
        --
        --    @return: void
        --
        --    Notes: Continuously calls recieve in a thread, and updates the application GUI whenever a
        --            message has been read from _ClientSocket
        ------------------------------------------------------------------------------------------------*/
        QString Receive();

        /*-----------------------------------------------------------------------------------------------
        --    Name:     [Receive]                  Date:         [March 6th, 2016]
        --
        --    Designer: [Jerry Jia]                Programmer:   [Jerry Jia]
        --
        --    Interface: int Send(const char * message);
        --
        --    @return: void
        --
        --    Notes: Wrapper for send()
        ------------------------------------------------------------------------------------------------*/
        int Send(const char * message);

        /*-----------------------------------------------------------------------------------------------
        --    Name:     [Receive]                  Date:         [March 6th, 2016]
        --
        --    Designer: [Jerry Jia]                Programmer:   [Jerry Jia]
        --
        --    Interface: void Close();
        --
        --    @return: void
        --
        --    Notes: Wrapper for closing a socket
        ------------------------------------------------------------------------------------------------*/
        void Close();




  private:

        int                         _ClientSocket;
        struct sockaddr_in          _ServerAddress;

};

#endif
