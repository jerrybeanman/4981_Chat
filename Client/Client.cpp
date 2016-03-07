#include "Client.h"

int Client::InitializeSocket(const char * name, short port)
{

    // create TCP socket
    if((_ClientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "socket() failed with errno " << errno << std::endl;
        return -1;
    }


    //set up port and protocol of address structure
    memset(&_ServerAddress, 0, sizeof(_ServerAddress));
    _ServerAddress.sin_family      = AF_INET;
    _ServerAddress.sin_port        = htons(port);
    _ServerAddress.sin_addr.s_addr = inet_addr(name);

    return 0;
}


int Client::Connect()
{
    //Connect to Server
    if(connect(_ClientSocket, (struct sockaddr*) &_ServerAddress, sizeof(_ServerAddress)) == -1)
    {
        std::cout << "connect() failed with errno " << errno << std::endl;
        return -1;
    }
    return 0;
}


void * Client::RecvThread(void * client)
{
    return ((Client *)client)->Receive();
}

void * Client::Receive()
{
    int bytesRead;
    while(1)
    {
        int bytesToRead = PACKET_LEN;
        char *message = (char *) malloc(PACKET_LEN);
        while((bytesRead = recv(_ClientSocket, message, bytesToRead, 0)) < PACKET_LEN)
        {
            //printf("Recv\n");
            if(bytesRead < 0)
            {
                printf("recv() failed with errno: %d\n", errno);
                free(message);
                return (void *)errno;
            }
            if(bytesRead == 0)
            {
                printf("Server has closed the socket\n");
                free(message);
                return NULL;
            }
            message += bytesRead;
            bytesToRead -= bytesRead;
        }

        std::cout << message << std::endl;

        free(message);
    }
    return NULL;
}

/*
    Wrapper function for TCP send function. Failing to send prints an error
    message with the data intended to send.
*/
int Client::Send(const char * message, int size)
{
    if (send(_ClientSocket, message, size, 0) == -1)
    {
      std::cerr << "send() failed with errno: " << errno << std::endl;
      return -1;
    }
    return 0;
}
