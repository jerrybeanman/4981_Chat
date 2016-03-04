#ifndef UI_H
#define UI_H

#include <QMainWindow>
#include <QString>

namespace Ui {
class UI;
}

class UI : public QMainWindow
{
    Q_OBJECT

public:
    explicit UI(QWidget *parent = 0);
    ~UI();
    void updateChatMenu(char *input);
    void updateUserList(char *user);
    void generateWhisperPage();
private slots:
    void getUserInput();
    void exit();
    void on_enterChat_pressed();

private:
    Ui::UI *ui;
    QString userName;
    char *getServerAddress();
    char *getUserName();
};

#endif // UI_H
