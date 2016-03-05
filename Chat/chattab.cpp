#include "chattab.h"
#include "ui_chattab.h"

ChatTab::ChatTab(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::ChatTab)
{
    ui->setupUi(this);
}

ChatTab::~ChatTab()
{
    delete ui;
}
