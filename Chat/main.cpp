#include "ui.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

void loadStyleSheet(QApplication* a);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loadStyleSheet(&a);
    UI w;
    w.show();

    return a.exec();
}


void loadStyleSheet(QApplication* a) {
    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        a->setStyleSheet(ts.readAll());
    }
}
