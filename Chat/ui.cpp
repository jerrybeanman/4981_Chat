#include "ui.h"
#include "ui_ui.h"
#include <QMessageBox>
#include <QListView>
#include <QListWidget>
#include <pthread.h>
#include <QTime>
#include "Client.h"
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QtAlgorithms>

UI::UI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UI)
{
    ui->setupUi(this);

    connect(ui->menuSettings->actions().at(3), SIGNAL(triggered(bool)), this, SLOT(exit()));
    connect(ui->menuSettings->actions().at(2), SIGNAL(toggled(bool)), this, SLOT(setFileWrite(bool)));

    connect(ui->enterChat, SIGNAL(clicked(bool)), this, SLOT(on_enterChat_pressed()));
    connect(ui->userNameInput, SIGNAL(returnPressed()), this, SLOT(on_enterChat_pressed()));
    connect(ui->inputField, SIGNAL(returnPressed()), this, SLOT(getUserInput()));
}

UI::~UI()
{
    delete ui;
}

void UI::exit() {
    ui->userList->clear();
    ui->chatMenu->clear();
    client.Send((char )18 + generateTimeStamp().toLocal8Bit() + " user " +
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
    name.prepend((char) 17 + generateTimeStamp() + " User: ");
    name.append(" connected");
    return name.toLocal8Bit();

}

void UI::on_enterChat_pressed()
{   QMessageBox error;
    QByteArray ipAddr = getServerAddress();
    if(ipAddr.isEmpty()) {
        error.critical(0, "Login Error", "Enter a valid IP address");
        error.setFixedSize(500,200);
        return;
    }
    QByteArray userConnectInfo = getUserName();
    if(userConnectInfo.isEmpty()) {
        error.critical(0, "Login Error", "Enter a username");
        error.setFixedSize(500,200);
        return;
    }

    if(client.InitializeSocket(ipAddr, PORT) < 0) {
            return;
    }

    if(client.Connect() < 0) {
        error.critical(0, "Connection Error", "Unable to connect to the server, please try again later");
        return;
    }



    readThread = new QThread();
    network_thread *worker = new network_thread();
    worker->moveToThread(readThread);
    connect(worker, SIGNAL(messageReceived(QString)), this, SLOT(updateChatMenu(QString)));
    connect(worker, SIGNAL(userConnected(QString)), this, SLOT(updateUserList(QString)));
    connect(worker, SIGNAL(userDisconnected(QString)), this, SLOT(removeUser(QString)));
    connect(worker, SIGNAL(threadRequested()), readThread, SLOT(start()));
    connect(readThread, SIGNAL(started()), worker, SLOT(receiveThread()));
    connect(worker, SIGNAL(finished()), readThread, SLOT(quit()), Qt::DirectConnection);
    worker->requestThread(client);

    client.Send(userConnectInfo.data());
    ui->userNameInput->clear();
    ui->serverIPAddress->clear();
    ui->stackedWidget->setCurrentIndex(1);
    updateUserList(userName.toLatin1().data());
}

void UI::getUserInput() {
    QString input = ui->inputField->text();
    input.prepend(generateTimeStamp() + " " + this->userName + ":");
    ui->chatMenu->addItem(input);
    ui->inputField->clear();
    client.Send(input.toLocal8Bit().data());
    if(writeToFile) {
        writeFile(input);
    }
}

void UI::updateChatMenu(QString chatInput) {
    ui->chatMenu->addItem(chatInput);
    if(writeToFile) {
        writeFile(chatInput);
    }
}

void UI::updateUserList(QString newUser) {
    ui->userList->addItem(newUser);
}

QString UI::generateTimeStamp() {
    QTime currentTime = QTime::currentTime();
    return currentTime.toString();
}

void UI::setFileWrite(bool value) {
    writeToFile = value;
}

void UI::removeUser(const QString& user) {
  QList<QListWidgetItem *> users = ui->userList->findItems(user, Qt::MatchFixedString | Qt::MatchCaseSensitive);
  qDeleteAll(users);

}

void UI::writeFile(const QString& data) {
    QFile file("output.txt");

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        QMessageBox::critical(0, "File Error", "Unable to log chat please try again later.");
        writeToFile = false;
        return;
    }

    QTextStream fileOutput(&file);
    fileOutput << data << "\n";

    file.close();
}
