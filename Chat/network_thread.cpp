#include "network_thread.h"
#include <QtDebug>

void network_thread::receiveThread() {
    while(connected) {
        QString receivedMessage = client.Receive();
        qDebug() << "messaged received: " << receivedMessage;
        if(receivedMessage.isEmpty()) {
            break;
        }

        if(receivedMessage[0] == (char)17) {
            emit userConnected(receivedMessage.section(' ', 2, 2));
        } else if(receivedMessage[0] == (char)18) {
            emit userDisconnected(receivedMessage.section(' ', 2, 2));
        } else if(receivedMessage[0] == (char) 19){
            receivedMessage.remove(0,1);
            emit userList(receivedMessage);
        } else {
             emit messageReceived(receivedMessage);
        }
    }
}

void network_thread::requestThread(Client &c) {
    client = c;
    emit threadRequested();
}
