#include "ui.h"
#include "ui_ui.h"
#include <QMessageBox>
#include <QListView>
#include <QListWidget>

UI::UI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UI)
{
    ui->setupUi(this);
    ui->menuBar->setStyleSheet("background:##388E3C");
    generateWhisperPage("Scott");
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
    this->userName = "";
    ui->stackedWidget->setCurrentIndex(0);
}

char *UI::getServerAddress() {
    QString serverIP = ui->serverIPAddress->text();
    if(serverIP == "...") {
        return nullptr;
    }
    return (char *)serverIP.toLatin1().data();
}

char *UI::getUserName() {
    QString name = ui->userNameInput->text();
    if(name.isEmpty()) {
        return nullptr;
    }
    this->userName = name;
    name.prepend("1 User: ");
    name.append(" connected");
    return (char *)name.toLatin1().data();

}

void UI::on_enterChat_pressed()
{   QMessageBox loginError;
    char *ipAddr = getServerAddress();
    if(ipAddr == nullptr) {
        loginError.critical(0, "Login Error", "Enter a valid IP address");
        loginError.setFixedSize(500,200);
        return;
    }
    char *userConnectInfo = getUserName();
    if(userConnectInfo == nullptr) {
        loginError.critical(0, "Login Error", "Enter a username");
        loginError.setFixedSize(500,200);
        return;
    }
    //connect to server
    //send connect information to the server
    ui->userNameInput->clear();
    ui->serverIPAddress->clear();
    ui->stackedWidget->setCurrentIndex(1);
    updateUserList(userName.toLatin1().data());
}

void UI::generateWhisperPage(char *whisperName) {
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
    ui->chatMenu->addItem(input);
    ui->inputField->clear();
    //Call send function (char *) input.toLatin1().data();
}

void UI::updateChatMenu(char *input) {
    QString newChatInput(input);
    ui->chatMenu->addItem(newChatInput);
}

void UI::updateUserList(char *user) {
    QString newUser(user);
    ui->userList->addItem(newUser);
}
