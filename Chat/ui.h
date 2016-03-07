#ifndef UI_H
#define UI_H

#include <QMainWindow>
#include <QString>
#include <QByteArray>
#include "Client.h"
namespace Ui {
class UI;
}

class UI : public QMainWindow
{
    Q_OBJECT

public:
    explicit UI(QWidget *parent = 0);
    ~UI();
    void updateChatMenu(QByteArray input);
    void updateUserList(QByteArray user);
    void generateWhisperPage(QByteArray whisperName);
private slots:
    void getUserInput();
    void exit();
    void on_enterChat_pressed();

private:
    Ui::UI *ui;
    QString userName;
    Client client;
    QByteArray getServerAddress();
    QByteArray getUserName();
    QString generateTimeStamp();
};

#endif // UI_H
