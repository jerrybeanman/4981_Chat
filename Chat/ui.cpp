#include "ui.h"
#include "ui_ui.h"
#include <QMessageBox>
#include <QListView>
#include <QListWidget>
#include <pthread.h>
#include <QTime>
#include "Client.h"

UI::UI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UI)
{
    ui->setupUi(this);
    connect(ui->inputField, SIGNAL(returnPressed()), this, SLOT(getUserInput()));
    connect(ui->menuSettings->actions().at(2), SIGNAL(triggered(bool)), this, SLOT(exit()));
    connect(ui->enterChat, SIGNAL(clicked(bool)), this, SLOT(on_enterChat_pressed()));
    connect(ui->userNameInput, SIGNAL(returnPressed()), this, SLOT(on_enterChat_pressed()));
}

UI::~UI()
{
    delete ui;
}

void UI::exit() {
    ui->userList->clear();
    ui->chatMenu->clear();
    client.Send("0 "  + generateTimeStamp().toLocal8Bit() + " user " +
                this->userName.toLocal8Bit() + " disconnected");
    client.Close();
    this->userName = "";
    ui->stackedWidget->setCurrentIndex(0);
}

QByteArray UI::getServerAddress() {
    QString serverIP = ui->serverIPAddress->text();
    if(serverIP == "...") {
        return nullptr;
    }
    return serverIP.toLocal8Bit();
}

QByteArray UI::getUserName() {
    QString name = ui->userNameInput->text();
    if(name.isEmpty()) {
        return nullptr;
    }

    this->userName = name;
    name.prepend(generateTimeStamp() + " User: ");
    name.append(" connected");
    return name.toLocal8Bit();

}

void UI::on_enterChat_pressed()
{   QMessageBox loginError;
    QByteArray ipAddr = getServerAddress();
    if(ipAddr.isEmpty()) {
        loginError.critical(0, "Login Error", "Enter a valid IP address");
        loginError.setFixedSize(500,200);
        return;
    }
    QByteArray userConnectInfo = getUserName();
    if(userConnectInfo.isEmpty()) {
        loginError.critical(0, "Login Error", "Enter a username");
        loginError.setFixedSize(500,200);
        return;
    }

    if(client.InitializeSocket(ipAddr, PORT) < 0) {
            return;
    }

    if(client.Connect() < 0) {
        return;
    }

    client.Send(userConnectInfo.data());

    pthread_t readThread;
    if(pthread_create(&readThread, NULL, &Client::RecvThread, (void *) &client) < 0) {
          return;
    }

    ui->userNameInput->clear();
    ui->serverIPAddress->clear();
    ui->stackedWidget->setCurrentIndex(1);
    updateUserList(userName.toLatin1().data());
}

void UI::generateWhisperPage(QByteArray whisperName) {
    QVBoxLayout *layout = new QVBoxLayout();
    QListWidget *chatItems = new QListWidget();
    chatItems->setWrapping(true);
    chatItems->setWordWrap(true);
    chatItems->setAutoScroll(true);
    layout->addWidget(chatItems);

    QWidget *whisperTab = new QWidget();
    whisperTab->setLayout(layout);
    ui->tabWidget->addTab(whisperTab, whisperName);
}

void UI::getUserInput() {
    QString input = ui->inputField->text();
    input.prepend(generateTimeStamp() + " " + this->userName + ":");
    ui->chatMenu->addItem(input);
    ui->inputField->clear();
    client.Send(input.toLocal8Bit().data());
}

void UI::updateChatMenu(QByteArray input) {
    QString newChatInput(input);
    ui->chatMenu->addItem(newChatInput);
}

void UI::updateUserList(QByteArray user) {
    QString newUser(user);
    ui->userList->addItem(newUser);
}

QString UI::generateTimeStamp() {
    QTime currentTime = QTime::currentTime();

    return currentTime.toString();
}
