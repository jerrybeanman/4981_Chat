#include "Client.h"
#include <QtDebug>
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
QString Client::Receive()
{
    int bytesRead;
    int bytesToRead = PACKET_LEN;
    char *message = (char *) malloc(PACKET_LEN);
    while((bytesRead = recv(_ClientSocket, message, bytesToRead, 0)) < PACKET_LEN)
    {
        //printf("Recv\n");
        if(bytesRead < 0)
        {
            qDebug() << "recv() failed with errno: " << errno;
            free(message);
            return "";
        }
        if(bytesRead == 0)
        {
            printf("Server has closed the socket\n");
            free(message);
            return "";
        }
        message += bytesRead;
        bytesToRead -= bytesRead;
    }
    QString returnMessage(message);
    free(message);
    return returnMessage;
}

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
int Client::Send(const char * message)
{
    if (send(_ClientSocket, message, PACKET_LEN, 0) == -1)
    {
      std::cerr << "send() failed with errno: " << errno << std::endl;
      return -1;
    }
    return 0;
}

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
void Client::Close()
{

    close(this->_ClientSocket);
}
