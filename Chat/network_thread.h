/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: network_thread - An application to let users comminucate in a chat room
--
-- PROGRAM: Chat
--
-- FUNCTIONS:
--  explicit network_thread(QObject *parent = 0) : QObject(parent) {}
--  void requestThread(Client& c);
--  void receiveThread();
--
--
-- DATE: March 6, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Scott Plummer
--
-- PROGRAMMER: Scott Plummer
--
-- NOTES:
-- Handles the thread manager, which calls receives and emits signals of the appropriate type depending on the message received
----------------------------------------------------------------------------------------------------------------------*/
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
    /*------------------------------------------------------------------------------------------------------------------
    -- Constructor: network_thread
    --
    -- DATE: March 6, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE:    explicit network_thread(QWidget *parent = 0)
    --                      *parent: Pointer t the parent window
    --
    -- NOTES:
    ----------------------------------------------------------------------------------------------------------------------*/
    explicit network_thread(QObject *parent = 0) : QObject(parent) {}
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: requestThread
    --
    -- DATE: March 6, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE:   requestThread(Client& c)
    --                          client -- Client object to do the recieve on
    --
    -- RETURN: void
    --
    -- NOTES: emits the threadRequested signal to start the receive thread
    ----------------------------------------------------------------------------------------------------------------------*/
    void requestThread(Client& client);
    bool connected;
public slots:
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: receiveThread
    --
    -- DATE: March 6, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE:   receiveThread()
    --
    -- RETURN: void
    --
    -- NOTES: emits the type of message receieved
    ----------------------------------------------------------------------------------------------------------------------*/
    void receiveThread();
signals:
    void messageReceived(const QString& message);
    void userList(const QString& userList);
    void threadRequested();
    void finished();
    void userConnected(const QString& user);
    void userDisconnected(const QString& user);
private:
    Client client;
};

#endif // NETWORK_THREAD_H
