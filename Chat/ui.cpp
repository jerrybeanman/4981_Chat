#include "ui.h"
#include "ui_ui.h"


UI::UI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UI)
{
    ui->setupUi(this);
    ui->menuBar->setStyleSheet("background:##388E3C");
   // ui->listView
}

UI::~UI()
{
    delete ui;
}
void UI::on_pushButton_1_clicked()
{

    ui->stackedWidget->setCurrentIndex(1);
}
