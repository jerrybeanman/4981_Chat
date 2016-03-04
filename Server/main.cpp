#include "Server.h"

using namespace std;

int main()
{
    int rc;
    Server TCPServer;

    if((rc = TCPServer.InitializeSocket(7000)) != 0)
    {
        std::cerr << "TCP Server initialization failed." << std::endl;
        return -1;
    }

    std::cerr << "TCP Server initialized." << std::endl;

    while(1)
    {
        /* assign it into the player object if we want to manipulate the thread */
        pthread_t readThread;

        int clientID;
        struct Client client;
        if ((clientID = TCPServer.Accept(&client)) == -1)
        {
            std::cerr << "rip.\n" << std::endl;
        }

        /* Creates the thread to handle new clients */
        if(pthread_create(&readThread, NULL, &Server::CreateClientManager, (void *) &TCPServer) < 0)
        {
            std::cerr << "thread creation failed" << std::endl;
        }
    }


    return 0;
}
