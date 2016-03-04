#ifndef CLIENT_H_
#define CLIENT_H_
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <utility>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <stdlib.h>
#include <string.h>

#define PACKET_LEN    256

class Client
{
  public:

        Client(){}
        ~Client(){}
        /*-----------------------------------------------------------------------------------------------
              --    Initialize socket, server address to lookup to, and connect to the server
              --
              --    Interface:  int InitializeSocket(short port)
              --                [name] Server IP address
              --                [port] Port number
              --
              --    programmer: Jerry Jia
              --
              --    @return: -1 on failure, 0 on success
        ------------------------------------------------------------------------------------------------*/
        int InitializeSocket(const char* name, short port);


          /*-----------------------------------------------------------------------------------------------
          --    Wrapper for WSAConnect
          --
          --    Interface:  int Connect();
          --
          --    programmer: Jerry Jia
          --
          --    @return: -1 on failure, 0 on success
          ------------------------------------------------------------------------------------------------*/
          int Connect();

          /*-----------------------------------------------------------------------------------------------
          --    Thread for recieving so it doesn't block the application
          --
          --    Interface:  void * RecvThread(void * client)
          --                [client] Pointer to a void, which has to be a Client object
          --
          --    Programmer: Jerry Jia
          --
          --    return: Recv() address
          -------------------------------------------------------------------------------------------------*/
          static void * RecvThread(void * client);

          void * Receive();

          int Send(char * message, int size);


  private:

        int                         _ClientSocket;
        struct sockaddr_in          _ServerAddress;
        std::vector<sockaddr_in>    _ClientList;

};

#endif
