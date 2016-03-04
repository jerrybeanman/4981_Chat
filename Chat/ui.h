#ifndef UI_H
#define UI_H

#include <QMainWindow>

namespace Ui {
class UI;
}

class UI : public QMainWindow
{
    Q_OBJECT

public:
    explicit UI(QWidget *parent = 0);
    ~UI();

private slots:
    void on_pushButton_1_clicked();

private:
    Ui::UI *ui;
};

#endif // UI_H
