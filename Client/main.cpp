#include <string>
#include <pthread.h>
#include "Client.h"

#define SERVER_IP   "192.168.1.65"
#define PORT        7000
int main()
{
    pthread_t readThread;
    std::string message;

    Client client;


    if(client.InitializeSocket(SERVER_IP, PORT) < 0)
        return 0;

    if(client.Connect() < 0)
        return 0;

    std::cout << "Connected to server " << SERVER_IP << std::endl;

    // Create thread to start reading
    if(pthread_create(&readThread, NULL, &Client::RecvThread, (void *) &client) < 0)
    {
        std::cerr << "thread creation failed" << std::endl;
    }

    while(std::getline(std::cin, message))
    {
        if(client.Send(message.c_str(), PACKET_LEN) < 0)
            return 0;
    }
    return 1;
}
