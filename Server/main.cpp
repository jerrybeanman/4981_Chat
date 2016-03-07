#include "Server.h"

using namespace std;

#define PORT 7000
int main()
{
    fd_set Rset;            // Reset command
    Server TCPServer;
    int    RetVal;
    int    MaxIndex = -1;   // Current maximum  index in TCPServer._ClientList
    int    NumClients;      // Number of clients connected


    // Initialize socket and address
    if(TCPServer.InitializeSocket(PORT) != 0)
    {
        std::cerr << "Server::InitializeSocket() failed" << std::endl;
        return -1;
    }

    std::cerr << "Server running..." << std::endl;

    while(1)
    {
        Client tmpClient;

        Rset = TCPServer.AllSet;

        // Blocking call for kernel socket events
        NumClients = select(TCPServer.MaxSocket + 1, &Rset, NULL, NULL, NULL);

        // New client connecection
        if(FD_ISSET(TCPServer.ListeningSocket, &Rset))
        {
            if((RetVal = TCPServer.Accept(&tmpClient)) < 0)
                break;
             if(RetVal > MaxIndex)
                 MaxIndex = RetVal;
            if(--NumClients <= 0)
                continue;
        }

        // Socket event recieved from a client
        // Check for which client
        for(int i = 0; i <= MaxIndex; i++)
        {
            if(TCPServer.ClientList[i].socket < 0)
                continue;
            if(FD_ISSET(TCPServer.ClientList[i].socket, &Rset))
            {
                if(TCPServer.Receive(i) < 0)
                    return -1;
                if(--NumClients <= 0)
                    break;
            }
        }
    }


    return 0;
}
