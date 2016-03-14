#ifndef NETWORK_THREAD_H
#define NETWORK_THREAD_H
#include <QObject>
#include <QString>
#include <QThread>
#include "Client.h"

class network_thread : public QObject
{
    Q_OBJECT
public:
    explicit network_thread(QObject *parent = 0) : QObject(parent) {}
    void requestThread(Client& c);
public slots:
    void receiveThread();
signals:
    void messageReceived(const QString& message);
    void threadRequested();
    void finished();
    void userConnected(const QString& user);
    void userDisconnected(const QString& user);
private:
    Client client;
};

#endif // NETWORK_THREAD_H
