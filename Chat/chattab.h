#ifndef CHATTAB_H
#define CHATTAB_H

#include <QTabWidget>

namespace Ui {
class ChatTab;
}

class ChatTab : public QTabWidget
{
    Q_OBJECT

public:
    explicit ChatTab(QWidget *parent = 0);
    ~ChatTab();

private:
    Ui::ChatTab *ui;
};

#endif // CHATTAB_H
