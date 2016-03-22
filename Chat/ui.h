/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: ui.h - An application to let users comminucate in a chat room
--
-- PROGRAM: Chat
--
-- FUNCTIONS:
-- explicit UI(QWidget *parent = 0);
--  ~UI();
--  void updateChatMenu(QByteArray input);
--  void updateUserList(QByteArray user);
--  void generateWhisperPage(QByteArray whisperName);
--  void getUserInput();
--  void exit();
--  void on_enterChat_pressed();
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
-- This is the header file for the UI implementation of the program. This contains custom UI functions along with
-- QT required UI functions
----------------------------------------------------------------------------------------------------------------------*/

#ifndef UI_H
#define UI_H

#include <QMainWindow>
#include <QString>
#include <QByteArray>
#include "Client.h"
#include "network_thread.h"
namespace Ui {
class UI;
}

class UI : public QMainWindow
{
    Q_OBJECT

public:
    /*------------------------------------------------------------------------------------------------------------------
    -- Constructor: UI
    --
    -- DATE: March 6, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE:    explicit UI(QWidget *parent = 0)
    --                      *parent: Pointer t the parent window
    --
    -- NOTES:
    -- Constructor for the UI
    ----------------------------------------------------------------------------------------------------------------------*/
    explicit UI(QWidget *parent = 0);
    /*------------------------------------------------------------------------------------------------------------------
    -- Deconstructor: UI
    --
    -- DATE: March 6, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE:     ~UI()
    --
    -- NOTES:
    -- Deconstructor for the UI
    ----------------------------------------------------------------------------------------------------------------------*/
    ~UI();
signals:
    void operate(Client& c);
public slots:
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: updateChatMenu
    --
    -- DATE: March 6, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE:    void updateChatMenu(QByteArray input)
    --                      input: The user input
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Updates the main chat window with the input the user entered
    ----------------------------------------------------------------------------------------------------------------------*/
    void updateChatMenu(QString chatInput);
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: updateUserList
    --
    -- DATE: March 6, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE:    void updateUserList(QByteArray newUser)
    --                      newUser: The user that just entered the chat
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Updates the list view of the chat room with the newuser
    ----------------------------------------------------------------------------------------------------------------------*/
    void updateUserList(QString newUser);

    void removeUser(const QString& user);

    void tokenizeUserList(const QString& list);
private slots:
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: getUserInput
    --
    -- DATE: March 6, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void getUserInput()
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Gets the user input from the line edit input field.
    ----------------------------------------------------------------------------------------------------------------------*/
    void getUserInput();
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: exit
    --
    -- DATE: March 6, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void exit()
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Sends the server a user disconnection message, as well as swaping the user back to the main menu of the chat program
    ----------------------------------------------------------------------------------------------------------------------*/
    void exit();
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: on_enterChat_pressed
    --
    -- DATE: March 6, 2016
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Scott Plummer
    --
    -- PROGRAMMER: Scott Plummer
    --
    -- INTERFACE: void on_enterChat_pressed()
    --
    -- RETURNS: void.
    --
    -- NOTES:
    -- Swaps the user to the chat menu of the program
    ----------------------------------------------------------------------------------------------------------------------*/
    void on_enterChat_pressed();

    void setFileWrite(bool value);

private:
    Ui::UI *ui;
    QThread *readThread;
    QString userName;
    Client client;
    QByteArray getServerAddress();
    QByteArray getUserName();
    QString generateTimeStamp();
    bool writeToFile = false;

    void writeFile(const QString& data);
};

#endif // UI_H
